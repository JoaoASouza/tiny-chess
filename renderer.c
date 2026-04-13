#include "engine.h"
#include "renderer.h"

char* pieceToChar(int piece) {
    switch (piece) {
        case WHITE_PAWN: return "\033[37m*";
        case BLACK_PAWN: return "\033[30m*";
        case WHITE_KNIGHT: return "\033[37m&";
        case BLACK_KNIGHT: return "\033[30m&";
        case WHITE_BISHOP: return "\033[37m+";
        case BLACK_BISHOP: return "\033[30m+";
        case WHITE_ROOK: return "\033[37mT";
        case BLACK_ROOK: return "\033[30mT";
        case WHITE_QUEEN: return "\033[37mQ";
        case BLACK_QUEEN: return "\033[30mQ";
        case WHITE_KING: return "\033[37mK";
        case BLACK_KING: return "\033[30mK";
        default: return " ";
    }
}



void clearScreen() {
    printf("\033[H\033[J");
}

void printBoard(GameState state, char message[100]) {
    clearScreen();
    for (int i = 0; i < 16 && state.capturedWhitePieces[i] != 0; i++) {
        printf("%s \033[m", pieceToChar(state.capturedWhitePieces[i]));
    }
    printf("\n");
    for (int i = 0; i < 8; i++) {
        printf("%d", 8 - i);
        for (int j = 0; j < 8; j++) {
            if ((i + j) % 2 == 0)
                printf("\033[44m %s \033[m", pieceToChar(state.board[i][j]));
            else
                printf("\033[46m %s \033[m", pieceToChar(state.board[i][j]));

        }
        printf("\n");
    }
    printf("  a  b  c  d  e  f  g  h\n");
    for (int i = 0; i < 16 && state.capturedBlackPieces[i] != 0; i++) {
        printf("\033[47m%s \033[m", pieceToChar(state.capturedBlackPieces[i]));
    }
    printf("\n");
    printf("%s\n", message);
}
