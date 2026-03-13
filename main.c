#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "engine.h"
#include "renderer.h"

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

void localMultiplayerGame() {
    GameState state;
    initializeGameState(&state);
    setupBoard(state.board);

    char message[100] = "";
    char move[5];

    while (1) {
        printBoard(state, message);
        printf("%s's move (e.g., e2e4): ", state.whiteTurn ? "White" : "Black");
        scanf("%s", move);

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
        handleStateChange(&state, move, piece);

        int opponentInCheck = isKingInCheck(&state, !(state.whiteTurn));
        if (opponentInCheck) {
            if (!getEscapeFromCheck(&state)) {
                printBoard(state, message);
                printf("Checkmate! %s wins!\n", state.whiteTurn ? "White" : "Black");
                break;
            }
        }

        message[0] = '\0';
        state.whiteTurn = !state.whiteTurn;
    }

    printf("Press Enter to return to main menu...");
    getchar(); // Consume leftover newline
    getchar(); // Wait for user to press Enter
}

int main() {

    while(1) {
        printf("[1] Local Multiplayer\n");
        printf("[0] Exit\n");
        printf("Select an option: ");
        int option;
        scanf("%d", &option);
        if (option == 1) {
            localMultiplayerGame();
        } else if (option == 0) {
            break;
        } else {
            printf("Invalid option. Please try again.\n");
        }
    }

    return 0;
}