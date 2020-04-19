#ifndef _MOVES_H_
#define _MOVES_H_

#include "chess.h"
#include "io.h"

/* Given a color, this function returns a singly linked list of all legal Moves with the head at *m. 
 * The function returns TRUE if at least 1 legal move is available. 
 * The total number of moves found is stored in the address pointed to by pcount. */
Move* legal_moves(PlayerColor c, unsigned int *pcount);

Move * King_moves(unsigned int pos, PlayerColor player);
Move * Night_moves(unsigned int pos, PlayerColor player);
Move * Pawn_moves(unsigned int pos, PlayerColor player);
Move * Queen_moves(unsigned int pos, PlayerColor player);
Move * Rook_moves(unsigned int pos, PlayerColor player, Piece p);
Move * Bishop_moves(unsigned int pos, PlayerColor player, Piece p);


Bool is_check(PlayerColor c);

/* Returns TRUE if the CurrentPlayer is under checkmate, FALSE otherwise. */
Bool is_checkmate();

/* Validate a move and make it. Returns TRUE if successful, FALSE if not. 
 *  Error message if any, are stored in *msg. 
 * ep_square (if any) is stored in *ep_sq
 */
Bool validate_and_move(Move *move,char* errmsg, PlayerColor c);

/* Function to decide whether the current position is a draw */
/* Draw can be due to insufficient material, 3 move repetition or stalemate */
	/* 3 move repetition also covers perpetual check */
Bool is_draw();

/* Returns the piece on a square belonging to player color c. 
 * If there is no piece with color c, UNKNOWN is returned. */
Piece get_piece_at(Board pos, PlayerColor c);

/* Check if this move is trying to castle */
unsigned int detect_castle_move(Move* move, PlayerColor c);

/* Perform castling. Moves king and rook and resets castle flags */
void perform_castle(unsigned int castle, PlayerColor c);

#endif
