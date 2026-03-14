#include "engine.h"

int isPieceWhite(int piece) {
    return piece != EMPTY && piece % 2 == 1;
}

int isPieceBlack(int piece) {
    return piece != EMPTY && piece % 2 == 0;
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

    if (isPieceWhite(piece) && isPieceWhite(destinationPiece) || isPieceBlack(piece) && isPieceBlack(destinationPiece)) {
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

int isKingInCheck(GameState* state, int isWhite) {
    int kingPiece = isWhite ? WHITE_KING : BLACK_KING;
    int kingRow = -1, kingCol = -1;
    // Find the king
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (state->board[i][j] == kingPiece) {
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
            int piece = state->board[i][j];
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
            if (isMoveValid(state->board, piece, destPiece, move)) {
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

int getEscapeFromCheck(GameState* state) {
    int whiteTurn = !(state->whiteTurn);
    // Try all possible moves for opponent, see if any remove check
    int foundEscape = 0;
    for (int i = 0; i < 8 && !foundEscape; i++) {
        for (int j = 0; j < 8 && !foundEscape; j++) {
            int p = state->board[i][j];
            if (p == EMPTY) continue;
            if ((whiteTurn && isPieceWhite(p)) || (!whiteTurn && isPieceBlack(p))) {
                for (int ii = 0; ii < 8 && !foundEscape; ii++) {
                    for (int jj = 0; jj < 8 && !foundEscape; jj++) {
                        if (i == ii && j == jj) continue;
                        int destP = state->board[ii][jj];
                        if ((isPieceWhite(p) && isPieceWhite(destP)) || (isPieceBlack(p) && isPieceBlack(destP))) continue;
                        char moveTry[5];
                        moveTry[0] = 'a' + j;
                        moveTry[1] = '8' - i;
                        moveTry[2] = 'a' + jj;
                        moveTry[3] = '8' - ii;
                        moveTry[4] = '\0';
                        if (!isMoveValid(state->board, p, destP, moveTry)) continue;
                        // Try move
                        int backupSrc = p;
                        int backupDst = destP;
                        state->board[i][j] = EMPTY;
                        state->board[ii][jj] = p;
                        if (!isKingInCheck(state, whiteTurn))  {
                            foundEscape = 1;
                        }
                        // Undo
                        state->board[i][j] = backupSrc;
                        state->board[ii][jj] = backupDst;
                    }
                }
            }
        }
    }
    return foundEscape;
}

int isEnPassantMove(GameState state, char move[5]) {
    if (state.enPassantCandidatesCount == 0) return 0;

    int piece = getPiece(state.board, move);
    int destinationPiece = getPiece(state.board, move + 2);
    if (piece != WHITE_PAWN && piece != BLACK_PAWN) return 0;
    if (destinationPiece != EMPTY) return 0;

    for (int i = 0; i < state.enPassantCandidatesCount; i++) {
        if (move[0] != state.enPassantCandidates[i][0] || move[1] != state.enPassantCandidates[i][1]) {
            return 0;
        }

        if (piece == WHITE_PAWN) { 
            if ((move[0] + 1 == move[2] || move[0] - 1 == move[2]) && ((move[3] - move[1]) == 1)) {
                if (getPiece(state.board, (char[]){move[2], move[3] - 1, '\0'}) == BLACK_PAWN) {
                    return 1;
                } else {
                    return 0;
                }
            }
        
        } else if (piece == BLACK_PAWN) {
            if ((move[0] + 1 == move[2] || move[0] - 1 == move[2]) && ((move[1] - move[3]) == 1)) {
                if (getPiece(state.board, (char[]){move[2], move[3] + 1, '\0'}) == WHITE_PAWN) {
                    return 1;
                } else {
                    return 0;
                }
            }
        }
    }
    return 0;
}

int isCastlingMove(GameState state, int piece, char move[5]) {
    int whiteCanCastleKingside = !state.whiteKingMoved && !state.whiteKingsideRookMoved;
    int whiteCanCastleQueenside = !state.whiteKingMoved && !state.whiteQueensideRookMoved;
    int blackCanCastleKingside = !state.blackKingMoved && !state.blackKingsideRookMoved;
    int blackCanCastleQueenside = !state.blackKingMoved && !state.blackQueensideRookMoved;

    int canCastle = state.whiteTurn ? (whiteCanCastleKingside || whiteCanCastleQueenside) : (blackCanCastleKingside || blackCanCastleQueenside);
    if (!canCastle) return 0;
    if (piece != WHITE_KING && piece != BLACK_KING) return 0;
    if (move[1] != move[3]) return 0;
    if (move[0] == 'e' && move[2] == 'c') {
        // Queenside castling
        if (getPiece(state.board, (char[]){'d', move[1], '\0'}) != EMPTY) return 0;
        if (getPiece(state.board, (char[]){'c', move[1], '\0'}) != EMPTY) return 0;
        if (getPiece(state.board, (char[]){'b', move[1], '\0'}) != EMPTY) return 0;
        return 1;
    } else if (move[0] == 'e' && move[2] == 'g') {
        // Kingside castling
        if (getPiece(state.board, (char[]){'f', move[1], '\0'}) != EMPTY) return 0;
        if (getPiece(state.board, (char[]){'g', move[1], '\0'}) != EMPTY) return 0;
        return 1;
    }
    return 0;
}

void pushMoveToHistory(MoveNode** head, char move[5]) {
    MoveNode* newNode = malloc(sizeof(MoveNode));
    strncpy(newNode->move, move, 5);
    newNode->next = *head;
    *head = newNode;
}

void printMoveHistory(MoveNode* head) {
    MoveNode* current = head;
    printf("Move History:\n");
    while (current) {
        printf("%s\n", current->move);
        current = current->next;
    }
}

void freeMoveHistory(MoveNode* head) {
    MoveNode* current = head;
    while (current) {
        MoveNode* temp = current;
        current = current->next;
        free(temp);
    }
}

void initializeGameState(GameState* state) {
    memset(state->board, 0, sizeof(state->board));
    memset(state->capturedWhitePieces, 0, sizeof(state->capturedWhitePieces));
    memset(state->capturedBlackPieces, 0, sizeof(state->capturedBlackPieces));
    memset(state->enPassantCandidates, 0, sizeof(state->enPassantCandidates));
    state->totalCapturedWhitePieces = 0;
    state->totalCapturedBlackPieces = 0;
    state->enPassantCandidatesCount = 0;
    state->enPassantSquare[0] = '\0';
    state->whiteKingMoved = 0;
    state->blackKingMoved = 0;
    state->whiteKingsideRookMoved = 0;
    state->whiteQueensideRookMoved = 0;
    state->blackKingsideRookMoved = 0;
    state->blackQueensideRookMoved = 0;
    state->whiteTurn = 1;
    state->moveList = NULL;
    state->halfmoveClock = 0;
    state->fullmoveNumber = 0;

    setupBoard(state->board);
}

void checkForEnPassantCandidates(GameState* state, char move[5], int piece) {
    if (piece == WHITE_PAWN && move[1] == '2' && move[3] == '4') {
        strcpy(state->enPassantSquare, (char[]){move[0], '3', '\0'});
        if ( getPiece(state->board, (char[]){move[0] + 1, '4', '\0'}) == BLACK_PAWN)
            strcpy(state->enPassantCandidates[state->enPassantCandidatesCount++], (char[]){ move[0] + 1, '4', '\0' });
        if ( getPiece(state->board, (char[]){move[0] - 1, '4', '\0'}) == BLACK_PAWN)
            strcpy(state->enPassantCandidates[state->enPassantCandidatesCount++], (char[]){ move[0] - 1, '4', '\0' });

    } else if (piece == BLACK_PAWN && move[1] == '7' && move[3] == '5') {
        strcpy(state->enPassantSquare, (char[]){move[0], '6', '\0'});
        if ( getPiece(state->board, (char[]){move[0] + 1, '5', '\0'}) == WHITE_PAWN)
            strcpy(state->enPassantCandidates[state->enPassantCandidatesCount++], (char[]){ move[0] + 1, '5', '\0' });
        if ( getPiece(state->board, (char[]){move[0] - 1, '5', '\0'}) == WHITE_PAWN)
            strcpy(state->enPassantCandidates[state->enPassantCandidatesCount++], (char[]){ move[0] - 1, '5', '\0' });
    } else {
        state->enPassantSquare[0] = '\0';
        state->enPassantCandidatesCount = 0;
    }
}

void handleCapture(GameState* state, char move[5], int piece, int destinationPiece, int enPassantMove) {
    int enPassantCapturedPiece = 0;
    if (enPassantMove) {
        if (state->whiteTurn) {
            enPassantCapturedPiece = getPiece(state->board, (char[]){move[2], move[3] - 1, '\0'});
            setPiece(state->board, EMPTY, (char[]){move[2], move[3] - 1, '\0'});
            state->capturedBlackPieces[state->totalCapturedBlackPieces++] = enPassantCapturedPiece;
        } else {
            enPassantCapturedPiece = getPiece(state->board, (char[]){move[2], move[3] + 1, '\0'});
            setPiece(state->board, EMPTY, (char[]){move[2], move[3] + 1, '\0'});
            state->capturedWhitePieces[state->totalCapturedWhitePieces++] = enPassantCapturedPiece;
        }
    } else if (destinationPiece != EMPTY) {
        if (isPieceWhite(destinationPiece)) {
            state->capturedWhitePieces[state->totalCapturedWhitePieces++] = destinationPiece;
        } else {
            state->capturedBlackPieces[state->totalCapturedBlackPieces++] = destinationPiece;
        }
    }
}
void handlePromotion(GameState* state, char move[5], int piece) {
    if (piece == WHITE_PAWN && move[3] == '8') {
        promotePawn(state->board, move, isPieceWhite(piece));
    } else if (piece == BLACK_PAWN && move[3] == '1') {
        promotePawn(state->board, move, isPieceWhite(piece));
    }
}

void doMove(GameState* state, char move[5], int piece, int castlingMove) {
    setPiece(state->board, EMPTY, move);
    setPiece(state->board, piece, move + 2);
    if (castlingMove) {
        if (move[2] == 'c') {
            // Queenside castling
            setPiece(state->board, EMPTY, (char[]){'a', move[1], '\0'});
            setPiece(state->board, piece == WHITE_KING ? WHITE_ROOK : BLACK_ROOK, (char[]){'d', move[1], '\0'});
        } else if (move[2] == 'g') {
            // Kingside castling
            setPiece(state->board, EMPTY, (char[]){'h', move[1], '\0'});
            setPiece(state->board, piece == WHITE_KING ? WHITE_ROOK : BLACK_ROOK, (char[]){'f', move[1], '\0'});
        }
    }
}

void undoMove(GameState* state, char move[5], int piece, int destinationPiece) {
    setPiece(state->board, piece, move);
    setPiece(state->board, destinationPiece, move + 2);
}

void handleStateChange(GameState *state, char move[5], int piece) {
    if (piece == WHITE_KING) {
        state->whiteKingMoved = 1;
    } else if (piece == BLACK_KING) {
        state->blackKingMoved = 1;
    } else if (piece == WHITE_ROOK) {
        if (move[0] == 'a' && move[1] == '1') state->whiteQueensideRookMoved = 1;
        else if (move[0] == 'h' && move[1] == '1') state->whiteKingsideRookMoved = 1;
    } else if (piece == BLACK_ROOK) {
        if (move[0] == 'a' && move[1] == '8') state->blackQueensideRookMoved = 1;
        else if (move[0] == 'h' && move[1] == '8') state->blackKingsideRookMoved = 1;
    }

    if (piece == WHITE_PAWN || piece == BLACK_PAWN || getPiece(state->board, move + 2) != EMPTY || isEnPassantMove(*state, move))
        state->halfmoveClock = 0;
    else
        state->halfmoveClock++;
    
    if (!state->whiteTurn)
        state->fullmoveNumber++;

    pushMoveToHistory(&state->moveList, move);

    state->whiteTurn = !state->whiteTurn;
}