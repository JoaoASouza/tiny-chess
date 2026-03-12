#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EMPTY 0
#define WHITE_PAWN 1
#define BLACK_PAWN 2
#define WHITE_KNIGHT 3
#define BLACK_KNIGHT 4
#define WHITE_BISHOP 5
#define BLACK_BISHOP 6
#define WHITE_ROOK 7
#define BLACK_ROOK 8
#define WHITE_QUEEN 9
#define BLACK_QUEEN 10
#define WHITE_KING 11
#define BLACK_KING 12

int isPieceWhite(int piece) {
    return piece != EMPTY && piece % 2 == 1;
}

int isPieceBlack(int piece) {
    return piece != EMPTY && piece % 2 == 0;
}

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

void printBoard(int board[8][8], int capturedWhitePieces[16], int capturedBlackPieces[16], char message[100]) {
    clearScreen();
    for (int i = 0; i < 16 && capturedWhitePieces[i] != 0; i++) {
        printf("%s \033[m", pieceToChar(capturedWhitePieces[i]));
    }
    printf("\n");
    for (int i = 0; i < 8; i++) {
        printf("%d", 8 - i);
        for (int j = 0; j < 8; j++) {
            if ((i + j) % 2 == 0)
                printf("\033[44m %s \033[m", pieceToChar(board[i][j]));
            else
                printf("\033[46m %s \033[m", pieceToChar(board[i][j]));

        }
        printf("\n");
    }
    printf("  a  b  c  d  e  f  g  h\n");
    for (int i = 0; i < 16 && capturedBlackPieces[i] != 0; i++) {
        printf("\033[47m%s \033[m", pieceToChar(capturedBlackPieces[i]));
    }
    printf("\n");
    printf("%s\n", message);
}

void setPiece(int board[8][8], int piece, char coordinate[2]) {
    int row = 8 - (coordinate[1] - '0');
    int col = coordinate[0] - 'a';
    if (row >= 0 && row < 8 && col >= 0 && col < 8) {
        board[row][col] = piece;
    }
}

int getPiece(int board[8][8], char coordinate[2]) {
    int row = 8 - (coordinate[1] - '0');
    int col = coordinate[0] - 'a';
    if (row >= 0 && row < 8 && col >= 0 && col < 8) {
        return board[row][col];
    }
    return -1; // Invalid coordinate
}

void setupBoard(int board[8][8]) {
    setPiece(board, WHITE_ROOK, "a1");
    setPiece(board, WHITE_KNIGHT, "b1");
    setPiece(board, WHITE_BISHOP, "c1");
    setPiece(board, WHITE_QUEEN, "d1");
    setPiece(board, WHITE_KING, "e1");
    setPiece(board, WHITE_BISHOP, "f1");
    setPiece(board, WHITE_KNIGHT, "g1");
    setPiece(board, WHITE_ROOK, "h1");
    for (char file = 'a'; file <= 'h'; file++) {
        char coordinate[3] = {file, '2', '\0'};
        setPiece(board, WHITE_PAWN, coordinate);
    }

    setPiece(board, BLACK_ROOK, "a8");
    setPiece(board, BLACK_KNIGHT, "b8");
    setPiece(board, BLACK_BISHOP, "c8");
    setPiece(board, BLACK_QUEEN, "d8");
    setPiece(board, BLACK_KING, "e8");
    setPiece(board, BLACK_BISHOP, "f8");
    setPiece(board, BLACK_KNIGHT, "g8");
    setPiece(board, BLACK_ROOK, "h8");
    for (char file = 'a'; file <= 'h'; file++) {
        char coordinate[3] = {file, '7', '\0'};
        setPiece(board, BLACK_PAWN, coordinate);
    }

}

int isDiagonalMoveValid(int board[8][8], char move[5], int maxDistance) {
    if (move[0] != move[2] + (move[1] - move[3]) && move[0] != move[2] - (move[1] - move[3])) 
        return 0;

    if (move[0]< move[2] && move[1] < move[3]) {
        for (char file = move[0] + 1, rank = move[1] + 1; file < move[2] && rank < move[3]; file++, rank++) {
            if (getPiece(board, (char[]){file, rank, '\0'}) != EMPTY) return 0;
        }
    } else if (move[0] > move[2] && move[1] < move[3]) {
        for (char file = move[0] - 1, rank = move[1] + 1; file > move[2] && rank < move[3]; file--, rank++) {
            if (getPiece(board, (char[]){file, rank, '\0'}) != EMPTY) return 0;
        }
    } else if (move[0] < move[2] && move[1] > move[3]) {
        for (char file = move[0] + 1, rank = move[1] - 1; file < move[2] && rank > move[3]; file++, rank--) {
            if (getPiece(board, (char[]){file, rank, '\0'}) != EMPTY) return 0;
        }
    } else if (move[0] > move[2] && move[1] > move[3]) {
        for (char file = move[0] - 1, rank = move[1] - 1; file > move[2] && rank > move[3]; file--, rank--) {
            if (getPiece(board, (char[]){file, rank, '\0'}) != EMPTY) return 0;
        }
    }

    if (abs(move[0] - move[2]) <= maxDistance && abs(move[1] - move[3]) <= maxDistance) {
        return 1;
    }

    return 0;

}

int isCrossMoveValid(int board[8][8], char move[5], int maxDistance) {
    if (move[0] != move[2] && move[1] != move[3]) 
        return 0;

    if (move[0] == move[2]) {
        if (move[1] < move[3]) {
            for (char rank = move[1] + 1; rank < move[3]; rank++) {
                if (getPiece(board, (char[]){move[0], rank, '\0'}) != EMPTY) return 0;
            }
        } else {
            for (char rank = move[1] - 1; rank > move[3]; rank--) {
                if (getPiece(board, (char[]){move[0], rank, '\0'}) != EMPTY) return 0;
            }
        }
    } else if (move[1] == move[3]) {
        if (move[0] < move[2]) {
            for (char file = move[0] + 1; file < move[2]; file++) {
                if (getPiece(board, (char[]){file, move[1], '\0'}) != EMPTY) return 0;
            }
        } else {
            for (char file = move[0] - 1; file > move[2]; file--) {
                if (getPiece(board, (char[]){file, move[1], '\0'}) != EMPTY) return 0;
            }
        }
    }

    if ((abs(move[0] - move[2]) <= maxDistance && move[1] == move[3]) ||
        (abs(move[1] - move[3]) <= maxDistance && move[0] == move[2])) {
        return 1;
    }

    return 0;
}

int isMoveValid(int board[8][8], int piece, int destinationPiece, char move[5]) { 
    if (move[0] < 'a' || move[0] > 'h' || move[1] < '1' || move[1] > '8' ||
        move[2] < 'a' || move[2] > 'h' || move[3] < '1' || move[3] > '8') {
        return 0;
    }

    if (piece == WHITE_PAWN) {
        int maxMove = (move[1] == '2') ? 2 : 1; 
        if (destinationPiece == EMPTY) {
            if ((move[0] == move[2]) && ((move[3] - move[1]) <= maxMove)) return 1;
            else return 0;
        } else {
            if ((move[0] + 1 == move[2] || move[0] - 1 == move[2]) && ((move[3] - move[1]) == 1)) return 1;
            else return 0;
        }
    
    } else if (piece == BLACK_PAWN) {
        int maxMove = (move[1] == '7') ? 2 : 1; 
        if (destinationPiece == EMPTY) {
            if ((move[0] == move[2]) && ((move[1] - move[3]) <= maxMove)) return 1;
            else return 0;
        } else {
            if ((move[0] + 1 == move[2] || move[0] - 1 == move[2]) && ((move[1] - move[3]) == 1)) return 1;
            else return 0;
        }

    } else if (piece == WHITE_KNIGHT || piece == BLACK_KNIGHT) {
        if (move[0] == move[2] + 1 || move[0] == move[2] - 1) {
            if (move[1] == move[3] + 2 || move[1] == move[3] - 2) return 1;
        } else if (move[0] == move[2] + 2 || move[0] == move[2] - 2) {
            if (move[1] == move[3] + 1 || move[1] == move[3] - 1) return 1;
        }
        return 0;
    } else if (piece == WHITE_BISHOP || piece == BLACK_BISHOP) {
        return isDiagonalMoveValid(board, move, 8);
    } else if (piece == WHITE_ROOK || piece == BLACK_ROOK) {
        return isCrossMoveValid(board, move, 8);
    } else if (piece == WHITE_QUEEN || piece == BLACK_QUEEN) {
        return isDiagonalMoveValid(board, move, 8) || isCrossMoveValid(board, move, 8);
    } else if (piece == WHITE_KING || piece == BLACK_KING) {
        return isDiagonalMoveValid(board, move, 1) || isCrossMoveValid(board, move, 1);
    }
}

int isKingInCheck(int board[8][8], int isWhite) {
    int kingPiece = isWhite ? WHITE_KING : BLACK_KING;
    int kingRow = -1, kingCol = -1;
    // Find the king
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j] == kingPiece) {
                kingRow = i;
                kingCol = j;
                break;
            }
        }
        if (kingRow != -1) break;
    }
    if (kingRow == -1) return 0; // King not found (should not happen)

    // Check all opponent pieces to see if any can attack the king
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int piece = board[i][j];
            if (piece == EMPTY) continue;
            if (isWhite && isPieceWhite(piece)) continue;
            if (!isWhite && isPieceBlack(piece)) continue;
            // Build move string from (i,j) to (kingRow,kingCol)
            char move[5];
            move[0] = 'a' + j;
            move[1] = '8' - i;
            move[2] = 'a' + kingCol;
            move[3] = '8' - kingRow;
            move[4] = '\0';
            int destPiece = kingPiece;
            if (isMoveValid(board, piece, destPiece, move)) {
                return 1;
            }
        }
    }
    return 0;
}

void promotePawn(int board[8][8], char move[5], int isWhite) {
    int newPiece;

    int pieceSelected = 0;
    while (!pieceSelected) {
        printf("Select promotion piece:\n");
        printf("(1) Queen\n");
        printf("(2) Rook\n");
        printf("(3) Bishop\n");
        printf("(4) Knight\n");
        int choice;
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                newPiece = isWhite ? WHITE_QUEEN : BLACK_QUEEN;
                pieceSelected = 1;
                break;
            case 2:
                newPiece = isWhite ? WHITE_ROOK : BLACK_ROOK;
                pieceSelected = 1;
                break;
            case 3:
                newPiece = isWhite ? WHITE_BISHOP : BLACK_BISHOP;
                pieceSelected = 1;
                break;
            case 4:
                newPiece = isWhite ? WHITE_KNIGHT : BLACK_KNIGHT;
                pieceSelected = 1;
                break;
            default:
                break;
        }
    }
    setPiece(board, newPiece, move + 2);
}

int getEscapeFromCheck(int board[8][8], int whiteTurn) {
    // Try all possible moves for opponent, see if any remove check
    int foundEscape = 0;
    for (int i = 0; i < 8 && !foundEscape; i++) {
        for (int j = 0; j < 8 && !foundEscape; j++) {
            int p = board[i][j];
            if (p == EMPTY) continue;
            if ((!whiteTurn && isPieceWhite(p)) || (whiteTurn && isPieceBlack(p))) {
                for (int ii = 0; ii < 8 && !foundEscape; ii++) {
                    for (int jj = 0; jj < 8 && !foundEscape; jj++) {
                        if (i == ii && j == jj) continue;
                        int destP = board[ii][jj];
                        if ((isPieceWhite(p) && isPieceWhite(destP)) || (isPieceBlack(p) && isPieceBlack(destP))) continue;
                        char moveTry[5];
                        moveTry[0] = 'a' + j;
                        moveTry[1] = '8' - i;
                        moveTry[2] = 'a' + jj;
                        moveTry[3] = '8' - ii;
                        moveTry[4] = '\0';
                        if (!isMoveValid(board, p, destP, moveTry)) continue;
                        // Try move
                        int backupSrc = p;
                        int backupDst = destP;
                        board[i][j] = EMPTY;
                        board[ii][jj] = p;
                        if (!isKingInCheck(board, !whiteTurn))  {
                            foundEscape = 1;
                        }
                        // Undo
                        board[i][j] = backupSrc;
                        board[ii][jj] = backupDst;
                    }
                }
            }
        }
    }
    return foundEscape;
}

int isEnPassantMove(int board[8][8], char move[5], char enPassantCandidates[8][3], char enPassantCandidatesCount) {
    if (enPassantCandidatesCount == 0) return 0;

    int piece = getPiece(board, move);
    int destinationPiece = getPiece(board, move + 2);
    if (piece != WHITE_PAWN && piece != BLACK_PAWN) return 0;
    if (destinationPiece != EMPTY) return 0;

    for (int i = 0; i < enPassantCandidatesCount; i++) {
        if (move[0] != enPassantCandidates[i][0] || move[1] != enPassantCandidates[i][1]) {
            return 0;
        }

        if (piece == WHITE_PAWN) { 
            if ((move[0] + 1 == move[2] || move[0] - 1 == move[2]) && ((move[3] - move[1]) == 1)) {
                if (getPiece(board, (char[]){move[2], move[3] - 1, '\0'}) == BLACK_PAWN) {
                    return 1;
                } else {
                    return 0;
                }
            }
        
        } else if (piece == BLACK_PAWN) {
            if ((move[0] + 1 == move[2] || move[0] - 1 == move[2]) && ((move[1] - move[3]) == 1)) {
                if (getPiece(board, (char[]){move[2], move[3] + 1, '\0'}) == WHITE_PAWN) {
                    return 1;
                } else {
                    return 0;
                }
            }
        }
    }
    return 0;
}

int isCastlingMove(int board[8][8], int piece, char move[5], int canCastle) {
    if (!canCastle) return 0;
    if (piece != WHITE_KING && piece != BLACK_KING) return 0;
    if (move[1] != move[3]) return 0;
    if (move[0] == 'e' && move[2] == 'c') {
        // Queenside castling
        if (getPiece(board, (char[]){'d', move[1], '\0'}) != EMPTY) return 0;
        if (getPiece(board, (char[]){'c', move[1], '\0'}) != EMPTY) return 0;
        if (getPiece(board, (char[]){'b', move[1], '\0'}) != EMPTY) return 0;
        return 1;
    } else if (move[0] == 'e' && move[2] == 'g') {
        // Kingside castling
        if (getPiece(board, (char[]){'f', move[1], '\0'}) != EMPTY) return 0;
        if (getPiece(board, (char[]){'g', move[1], '\0'}) != EMPTY) return 0;
        return 1;
    }
    return 0;
}

int main() {
    int board[8][8] = {0}; 
    int totalCaputredWhitePieces = 0;
    int totalCapturedBlackPieces = 0;
    int capturedWhitePieces[16] = {0};
    int capturedBlackPieces[16] = {0};
    char enPassantCandidates[8][3];
    char enPassantCandidatesCount = 0;
    char message[100] = "";
    int whiteKingMoved = 0, blackKingMoved = 0;
    int whiteKingsideRookMoved = 0, whiteQueensideRookMoved = 0;
    int blackKingsideRookMoved = 0, blackQueensideRookMoved = 0;

    setupBoard(board);

    int whiteTurn = 1;
    while (1) {
        printBoard(board, capturedWhitePieces, capturedBlackPieces, message);

        char move[5];
        printf("%s's move (e.g., e2e4): ", whiteTurn ? "White" : "Black");
        scanf("%s", move);
        if (move[0] == 'q') break;
        if (move[4] != '\0') {
            snprintf(message, sizeof(message), "Invalid move format. Please use e.g., e2e4.\n");
            continue;
        }
        int piece = getPiece(board, move);
        if (piece == EMPTY) {
            snprintf(message, sizeof(message), "No piece at the source square.\n");
            continue;
        }

        int destinationPiece = getPiece(board, (char[]){move[2], move[3], '\0'});

        if ((whiteTurn && !isPieceWhite(piece)) || (!whiteTurn && !isPieceBlack(piece))) {
            snprintf(message, sizeof(message), "It's %s's turn.\n", whiteTurn ? "White" : "Black");
            continue;
        }

        if (isPieceWhite(piece) && isPieceWhite(destinationPiece) || isPieceBlack(piece) && isPieceBlack(destinationPiece)) {
            snprintf(message, sizeof(message), "Cannot move to a square occupied by your own piece.\n");
            continue;
        }

        int whiteCanCastleKingside = !whiteKingMoved && !whiteKingsideRookMoved;
        int whiteCanCastleQueenside = !whiteKingMoved && !whiteQueensideRookMoved;
        int blackCanCastleKingside = !blackKingMoved && !blackKingsideRookMoved;
        int blackCanCastleQueenside = !blackKingMoved && !blackQueensideRookMoved;

        int canCastle = whiteTurn ? (whiteCanCastleKingside || whiteCanCastleQueenside) : (blackCanCastleKingside || blackCanCastleQueenside);

        int moveValid = isMoveValid(board, piece, destinationPiece, move);
        int enPassantMove = isEnPassantMove(board, move, enPassantCandidates, enPassantCandidatesCount);
        int castlingMove = isCastlingMove(board, piece, move, canCastle);
        if (!moveValid && !enPassantMove && !castlingMove) {
            snprintf(message, sizeof(message), "Invalid move for the selected piece.\n");
            continue;
        }


        // Save state for undo
        int backupSrc = piece;
        int backupDst = destinationPiece;
        int srcRow = 8 - (move[1] - '0');
        int srcCol = move[0] - 'a';
        int dstRow = 8 - (move[3] - '0');
        int dstCol = move[2] - 'a';

        // Make the move
        setPiece(board, EMPTY, move); // Remove piece from source
        setPiece(board, piece, move + 2); // Place piece at destination
        if (castlingMove) {
            if (move[2] == 'c') {
                // Queenside castling
                setPiece(board, EMPTY, (char[]){'a', move[1], '\0'});
                setPiece(board, piece == WHITE_KING ? WHITE_ROOK : BLACK_ROOK, (char[]){'d', move[1], '\0'});
            } else if (move[2] == 'g') {
                // Kingside castling
                setPiece(board, EMPTY, (char[]){'h', move[1], '\0'});
                setPiece(board, piece == WHITE_KING ? WHITE_ROOK : BLACK_ROOK, (char[]){'f', move[1], '\0'});
            }
        }

        if (piece == WHITE_PAWN && move[3] == '8') {
            promotePawn(board, move, isPieceWhite(piece));
        } else if (piece == BLACK_PAWN && move[3] == '1') {
            promotePawn(board, move, isPieceWhite(piece));
        }

        if (piece == WHITE_PAWN && move[1] == '2' && move[3] == '4') {
            if ( getPiece(board, (char[]){move[0] + 1, '4', '\0'}) == BLACK_PAWN)
                strcpy(enPassantCandidates[enPassantCandidatesCount++], (char[]){ move[0] + 1, '4', '\0' });
            if ( getPiece(board, (char[]){move[0] - 1, '4', '\0'}) == BLACK_PAWN)
                strcpy(enPassantCandidates[enPassantCandidatesCount++], (char[]){ move[0] - 1, '4', '\0' });

        } else if (piece == BLACK_PAWN && move[1] == '7' && move[3] == '5') {
            if ( getPiece(board, (char[]){move[0] + 1, '5', '\0'}) == WHITE_PAWN)
                strcpy(enPassantCandidates[enPassantCandidatesCount++], (char[]){ move[0] + 1, '5', '\0' });
            if ( getPiece(board, (char[]){move[0] - 1, '5', '\0'}) == WHITE_PAWN)
                strcpy(enPassantCandidates[enPassantCandidatesCount++], (char[]){ move[0] - 1, '5', '\0' });
        } else {
            enPassantCandidatesCount = 0;
        }

        // Check if own king is in check after move
        int inCheck = isKingInCheck(board, whiteTurn);
        if (inCheck) {
            // Undo move
            setPiece(board, backupSrc, move);
            setPiece(board, backupDst, move + 2);
            snprintf(message, sizeof(message), "Move leaves king in check!\n");
            continue;
        }

        if (piece == WHITE_KING) {
            whiteKingMoved = 1;
        } else if (piece == BLACK_KING) {
            blackKingMoved = 1;
        } else if (piece == WHITE_ROOK) {
            if (move[0] == 'a' && move[1] == '1') whiteQueensideRookMoved = 1;
            else if (move[0] == 'h' && move[1] == '1') whiteKingsideRookMoved = 1;
        } else if (piece == BLACK_ROOK) {
            if (move[0] == 'a' && move[1] == '8') blackQueensideRookMoved = 1;
            else if (move[0] == 'h' && move[1] == '8') blackKingsideRookMoved = 1;
        }

        // Handle capture
        int enPassantCapturedPiece = 0;
        if (enPassantMove) {
            if (whiteTurn) {
                enPassantCapturedPiece = getPiece(board, (char[]){move[2], move[3] - 1, '\0'});
                setPiece(board, EMPTY, (char[]){move[2], move[3] - 1, '\0'});
                capturedBlackPieces[totalCapturedBlackPieces++] = enPassantCapturedPiece;
            } else {
                enPassantCapturedPiece = getPiece(board, (char[]){move[2], move[3] + 1, '\0'});
                setPiece(board, EMPTY, (char[]){move[2], move[3] + 1, '\0'});
                capturedWhitePieces[totalCaputredWhitePieces++] = enPassantCapturedPiece;
            }
        } else if (destinationPiece != EMPTY) {
            if (isPieceWhite(destinationPiece)) {
                capturedWhitePieces[totalCaputredWhitePieces++] = destinationPiece;
            } else {
                capturedBlackPieces[totalCapturedBlackPieces++] = destinationPiece;
            }
        }

        message[0] = '\0';

        // Check for checkmate after move (opponent's turn)
        int opponentInCheck = isKingInCheck(board, !whiteTurn);
        if (opponentInCheck) {
            int foundEscape = getEscapeFromCheck(board, !whiteTurn);
            if (!foundEscape) {
                printBoard(board, capturedWhitePieces, capturedBlackPieces, message);
                printf("Checkmate! %s wins!\n", whiteTurn ? "White" : "Black");
                break;
            }
        }
        whiteTurn = !whiteTurn;
    }
    return 0;
}