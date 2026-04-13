#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "engine.h"
#include "renderer.h"

#define MULTIPLAYER_MODE 1
#define AI_MODE 2

void printState(GameState state) {
    printf("White's turn: %d\n", state.whiteTurn);
    printf("Board:\n");
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            printf("%d ", state.board[i][j]);
        }
        printf("\n");
    }
    printf("Captured White Pieces: ");
    for (int i = 0; i < state.totalCapturedWhitePieces; i++) {
        printf("%d ", state.capturedWhitePieces[i]);
    }
    printf("\nCaptured Black Pieces: ");
    for (int i = 0; i < state.totalCapturedBlackPieces; i++) {
        printf("%d ", state.capturedBlackPieces[i]);
    }
    printf("\nEn Passant Candidates: ");
    for (int i = 0; i < state.enPassantCandidatesCount; i++) {
        printf("%s ", state.enPassantCandidates[i]);
    }
    printf("\n");
}

void handleSaveGame(GameState* state) {
    printf("Do you want to save the game? (y/n): ");
    char choice;
    scanf(" %c", &choice);
    if (choice != 'y' && choice != 'Y') return;

    FILE* file = fopen("saved_game.tcg", "w+");
    if (!file) {
        printf("Error saving game.\n");
        return;
    }

    char * fenString = malloc(100 * sizeof(char));
    stateToFEN(fenString, *state);
    fprintf(file, "%s\n", fenString);
    free(fenString);

    printMoveHistory(state->moveListStart);
    MoveNode* current = state->moveListStart;
    while (current) {
        fprintf(file, "%s ", current->move);
        current = current->next;
    }
    fprintf(file, "\n");
    fclose(file);
}

void getMoveInputFromLocalPlayer(char move[5], int whiteTurn) {
    printf("%s's move (e.g., e2e4): ", whiteTurn ? "White" : "Black");
    scanf("%s", move);
}

FILE *sopen(const char *program) {
    int fds[2];
    pid_t pid;

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, fds) < 0)
        return NULL;

    switch(pid=vfork()) {
    case -1:    /* Error */
        close(fds[0]);
        close(fds[1]);
        return NULL;
    case 0:     /* child */
        close(fds[0]);
        dup2(fds[1], 0);
        dup2(fds[1], 1);
        close(fds[1]);
        execl("/bin/sh", "sh", "-c", program, NULL);
        _exit(127);
    }
    /* parent */
    close(fds[1]);
    return fdopen(fds[0], "r+");
}

void getMoveInputFromAI(char move[5], GameState state) {
    char outputBuffer[128];
    char * fenString = malloc(100 * sizeof(char));

    stateToFEN(fenString, state);

    FILE* fp = sopen("stockfish");
    if (fp == NULL) {
        printf("Failed to run Stockfish.\n");
        move[0] = '\0';
        free(fenString);
        return;
    }

    fprintf(fp, "position fen %s\n", fenString);
    fprintf(fp, "go depth 10\n");

    while (fgets(outputBuffer, sizeof(outputBuffer), fp) != NULL) {
        // printf("Stockfish output: %s", outputBuffer);
        if (strncmp(outputBuffer, "bestmove", 8) == 0) {
            sscanf(outputBuffer, "bestmove %s", move);
            break;
        }
    }
    fprintf(fp, "quit\n");
    fclose(fp);

    free(fenString);
}

void getMoveInput(char move[5], int mode, GameState state) {
    switch (mode) {
        case MULTIPLAYER_MODE:
            getMoveInputFromLocalPlayer(move, state.whiteTurn);
            break;
        case AI_MODE:
            if (state.whiteTurn)
                getMoveInputFromLocalPlayer(move, state.whiteTurn);
            else
                getMoveInputFromAI(move, state);
            break;
        default:
            printf("Invalid game mode.\n");
            move[0] = '\0';
    }
}

void setupBot() {
    FILE *fp = popen("stockfish ucinewgame", "r");
    if (fp == NULL) {
        printf("Failed to run Stockfish.\n");
        return;
    }
    pclose(fp);
}

void loadGameStateFromFEN(GameState* state, char* fen) {
    char* token = strtok(fen, " ");
    int row = 0, col = 0;
    for (int i = 0; token[i] != '\0'; i++) {
        if (token[i] == '/') {
            row++;
            col = 0;
        } else if (token[i] >= '1' && token[i] <= '8') {
            int emptySquares = token[i] - '0';
            for (int j = 0; j < emptySquares; j++) {
                state->board[row][col++] = EMPTY;
            }
        } else {
            state->board[row][col++] = FENCharToPiece(token[i]);
        }
    }

    token = strtok(NULL, " ");
    state->whiteTurn = (token[0] == 'w');

    token = strtok(NULL, " ");
    state->whiteKingMoved = !strchr(token, 'K');
    state->whiteKingsideRookMoved = !strchr(token, 'K');
    state->whiteQueensideRookMoved = !strchr(token, 'Q');
    state->blackKingMoved = !strchr(token, 'k');
    state->blackKingsideRookMoved = !strchr(token, 'k');
    state->blackQueensideRookMoved = !strchr(token, 'q');

    token = strtok(NULL, " ");
    if (token[0] != '-') {
        strcpy(state->enPassantSquare, token);
    } else {
        state->enPassantSquare[0] = '\0';
    }

    token = strtok(NULL, " ");
    state->halfmoveClock = atoi(token);

    token = strtok(NULL, " ");
    state->fullmoveNumber = atoi(token);

}

void promptGameLoad(GameState* state) {
    printf("Do you want to load a saved game? (y/n): ");
    char choice;
    scanf(" %c", &choice);
    if (choice != 'y' && choice != 'Y') return;

    FILE* file = fopen("saved_game.tcg", "r");
    if (!file) {
        printf("No saved game found.\n");
        return;
    }

    char fenString[100];
    fgets(fenString, sizeof(fenString), file);
    loadGameStateFromFEN(state, fenString);

    char moveHistoryLine[1000];
    fgets(moveHistoryLine, sizeof(moveHistoryLine), file);
    char* token = strtok(moveHistoryLine, " ");
    while (token) {
        pushMoveToHistory(&state->moveListStart, &state->moveListEnd, token);
        token = strtok(NULL, " ");
    }

    fclose(file);
}

void gameLoop(int mode) {
    GameState state;
    initializeGameState(&state);
    promptGameLoad(&state);

    char message[100] = "";
    char move[5];

    while (1) {
        printBoard(state, message);
        getMoveInput(move, mode, state);

        if (move[0] == 'q') break;
        if (move[4] != '\0') {
            snprintf(message, sizeof(message), "Invalid move format. Please use e.g., e2e4.\n");
            continue;
        }

        int piece = getPiece(state.board, move);
        int destinationPiece = getPiece(state.board, (char[]){move[2], move[3], '\0'});

        if (piece == EMPTY) {
            snprintf(message, sizeof(message), "No piece at the source square.\n");
            continue;
        }
        if ((state.whiteTurn && !isPieceWhite(piece)) || (!state.whiteTurn && !isPieceBlack(piece))) {
            snprintf(message, sizeof(message), "It's %s's turn.\n", state.whiteTurn ? "White" : "Black");
            continue;
        }
        int moveValid = isMoveValid(state.board, piece, destinationPiece, move);
        int enPassantMove = isEnPassantMove(state, move);
        int castlingMove = isCastlingMove(state, piece, move);
        if (!moveValid && !enPassantMove && !castlingMove) {
            snprintf(message, sizeof(message), "Invalid move for the selected piece.\n");
            continue;
        }

        doMove(&state, move, piece, castlingMove);
        handlePromotion(&state, move, piece);
        checkForEnPassantCandidates(&state, move, piece);
        if (isKingInCheck(&state, state.whiteTurn)) {
            undoMove(&state, move, piece, destinationPiece);
            snprintf(message, sizeof(message), "Move leaves king in check!\n");
            continue;
        }
        handleCapture(&state, move, piece, destinationPiece, enPassantMove);

        int opponentInCheck = isKingInCheck(&state, !(state.whiteTurn));
        handleStateChange(&state, move, piece);
        if (opponentInCheck) {
            if (!getEscapeFromCheck(&state)) {
                printBoard(state, message);
                printf("Checkmate! %s wins!\n", state.whiteTurn ? "White" : "Black");
                break;
            }
        }

        message[0] = '\0';
    }

    handleSaveGame(&state);

    freeMoveHistory(state.moveListStart);
}

int main() {

    while(1) {
        clearScreen();
        printf("[1] Local Multiplayer\n");
        printf("[2] Play against AI\n");
        printf("[0] Exit\n");
        printf("Select an option: ");
        int option;
        scanf("%d", &option);
        if (option == 1) {
            gameLoop(MULTIPLAYER_MODE);
        } else if (option == 2) {
            setupBot();
            gameLoop(AI_MODE);
        } else if (option == 0) {
            break;
        } else {
            printf("Invalid option. Please try again.\n");
        }
    }

    return 0;
}
