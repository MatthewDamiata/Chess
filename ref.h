#ifndef _REF_H_
#define _REF_H_
#include "chess.h"

/* Given a board, returns a singly linked list of all legal moves for the given color */
Move *ref_legal_moves(char *board, PlayerColor c);

/* Given a board, a player color and a position on the board 0 <= Pos <= 63, returns a Piece on that square (if any).
 * Returns UNKNOWN if the requested color piece is not on the square or if the square is invalid. 
 */
Piece ref_get_piece_at_pos(char *board, PlayerColor c, Pos pos);

/* Given a board, returns TRUE if the player c is under checkmate, FALSE otherwise. */
Bool ref_is_checkmate(char *board, PlayerColor c);

/* Check if castling is valid */
/* Castling is valid if:
 * 1) Castle flags are set, i.e., King and Rook haven't moved before.
 * 2) King is not under check before and after castling.
 * 3) There are no pieces between the king and the rook.
 * 4) The square that the king crosses while castling is not under check.
 * castle is either NO_CASTLE, CASTLE_KING or CASTLE_QUEEN
 */
Bool ref_is_castling_valid(char *board, unsigned int castle, PlayerColor c); 

/* Structure used to return the parsed board */
typedef struct {
	PlayerState white;
	PlayerState black;
	PlayerColor current_player; 
	Pos ep_square; 
} ParseBoardOut; 

/* Given a board string, parse it. If the input string is invalid, NULL is returned. */
ParseBoardOut* ref_parse_board(char *board);

/* Prints a move on to the screen */
void ref_print_move(Move *m);

/* Given a pointer to the ParseBoardOut structure obtained from calling ref_parse_board, displays the board. */
void ref_display_board(ParseBoardOut *out); 

#endif
