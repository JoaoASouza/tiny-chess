#ifndef ENGINE_H
#define ENGINE_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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

typedef struct MoveNode {
    char move[5];
    struct MoveNode* next;
} MoveNode;

typedef struct GameState {
    int whiteTurn;
    int board[8][8];
    int capturedWhitePieces[16];
    int capturedBlackPieces[16];
    int totalCapturedWhitePieces;
    int totalCapturedBlackPieces;
    char enPassantCandidates[8][3];
    char enPassantCandidatesCount;
    int whiteKingMoved, blackKingMoved;
    int whiteKingsideRookMoved, whiteQueensideRookMoved;
    int blackKingsideRookMoved, blackQueensideRookMoved;
    MoveNode* moveList;
} GameState;

void initializeGameState(GameState* state);
void setupBoard(int board[8][8]);
int getPiece(int board[8][8], char coordinate[2]);
void setPiece(int board[8][8], int piece, char coordinate[2]);
int isPieceWhite(int piece);
int isPieceBlack(int piece);
int isMoveValid(int board[8][8], int piece, int destinationPiece, char move[5]);
int isKingInCheck(GameState* state, int isWhite);
int getEscapeFromCheck(GameState* state);
void checkForEnPassantCandidates(GameState* state, char move[5], int piece);
void handleCapture(GameState* state, char move[5], int piece, int destinationPiece, int enPassantMove);
void handlePromotion(GameState* state, char move[5], int piece);
void promotePawn(int board[8][8], char move[5], int isWhite);
void doMove(GameState* state, char move[5], int piece, int castlingMove);
void undoMove(GameState* state, char move[5], int piece, int destinationPiece);
int isEnPassantMove(GameState state, char move[5]);
int isCastlingMove(GameState state, int piece, char move[5]);
int isDiagonalMoveValid(int board[8][8], char move[5], int maxDistance);
int isCrossMoveValid(int board[8][8], char move[5], int maxDistance);
void handleStateChange(GameState* state, char move[5], int piece);
void pushMoveToHistory(MoveNode** head, char move[5]);
void freeMoveHistory(MoveNode* head);
void printMoveHistory(MoveNode* head);

#endif