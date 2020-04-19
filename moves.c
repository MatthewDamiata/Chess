#include <string.h>
#include "moves.h"
#include "chess.h"
#include "io.h"

/* Check if the move provided is trying to castle */
unsigned int detect_castle_move(Move *move, PlayerColor c) {
    if(c == WHITE){
	if(move->from == WKING_START_POS && (move->to == WKING_START_POS + 2 || move->to == WKING_START_POS - 2)) return 1;
    } else {
	if(move->from == BKING_START_POS && (move->to == BKING_START_POS + 2 || move->to == BKING_START_POS - 2)) return 1;
    }
    return 0;
}

void validate_castle(Move *m, unsigned int pos, PlayerColor c){
	if(c == WHITE){
		/* First white rook, bottom left */
		if(!(IS_SET(player[c].castle_flags,2)) && !(IS_SET(player[c].castle_flags,0)) && IS_SET(player[c].r, 56) && (UNOCCUPIED(WEST_OF(pos))) && UNOCCUPIED(WEST_OF(WEST_OF(pos))) && UNOCCUPIED(WEST_OF(WEST_OF(WEST_OF(pos))))){
			m->next_move = (Move*)malloc(sizeof(Move));
			m=m->next_move;
			m->from = pos;
			m->to = WEST_OF(WEST_OF(pos));
			m->piece = KING;
		}
		/* Second white rook, bottom right */
		if(!(IS_SET(player[c].castle_flags,3)) && !(IS_SET(player[c].castle_flags,0)) && IS_SET(player[c].r, 63) && (UNOCCUPIED(EAST_OF(pos))) && UNOCCUPIED(EAST_OF(EAST_OF(pos)))){
			m->next_move = (Move*)malloc(sizeof(Move));
			m=m->next_move;
			m->from = pos;
			m->to = EAST_OF(EAST_OF(pos));
			m->piece = KING;
		}
	}
	if(c == BLACK){
		/* First black rook, top left */
		if(!(IS_SET(player[c].castle_flags,4)) && !(IS_SET(player[c].castle_flags,1)) && IS_SET(player[c].r, 0) && (UNOCCUPIED(WEST_OF(pos))) && UNOCCUPIED(WEST_OF(WEST_OF(pos))) && UNOCCUPIED(WEST_OF(WEST_OF(WEST_OF(pos))))){
			m->next_move = (Move*)malloc(sizeof(Move));
			m=m->next_move;
			m->from = pos;
			m->to = WEST_OF(WEST_OF(pos));
			m->piece = KING;
		}
		/* Second black rook, top right */
		if(!(IS_SET(player[c].castle_flags,5)) && !(IS_SET(player[c].castle_flags,1)) && IS_SET(player[c].r, 7) && (UNOCCUPIED(EAST_OF(pos))) && UNOCCUPIED(EAST_OF(EAST_OF(pos)))){
			m->next_move = (Move*)malloc(sizeof(Move));
			m=m->next_move;
			m->from = pos;
			m->to = EAST_OF(EAST_OF(pos));
			m->piece = KING;
		}
	}
		m->next_move=NULL;

}
/* Perform castling. Moves king and rook and resets castle flags */
void perform_castle(unsigned int castle, PlayerColor c) {
    	if(castle==2){
		RESET_BIT(player[c].k,4);
		SET_BIT(player[c].k,2);
		RESET_BIT(player[c].r,0);
		SET_BIT(player[c].r,3);
		SET_BIT(player[c].castle_flags, 1);
		SET_BIT(player[c].castle_flags, 4);
	}
	else if(castle==6){
		RESET_BIT(player[c].k,4);
		SET_BIT(player[c].k,6);
		RESET_BIT(player[c].r,7);
		SET_BIT(player[c].r,5);
		SET_BIT(player[c].castle_flags, 1);
		SET_BIT(player[c].castle_flags, 5);
	}
	else if(castle==58){
		RESET_BIT(player[c].k,60);
		SET_BIT(player[c].k,58);
		RESET_BIT(player[c].r,56);
		SET_BIT(player[c].r,59);
		SET_BIT(player[c].castle_flags, 0);
		SET_BIT(player[c].castle_flags, 2);
	}
	else if(castle==62){
		RESET_BIT(player[c].k,60);
		SET_BIT(player[c].k,62);
		RESET_BIT(player[c].r,63);
		SET_BIT(player[c].r,61);
		SET_BIT(player[c].castle_flags, 0);
		SET_BIT(player[c].castle_flags, 3);
	}
}


Move *King_moves(unsigned int pos, PlayerColor c){
	Move *m = (Move*)malloc(sizeof(Move));
	m->next_move=(Move*)malloc(sizeof(Move));
	Move * head = m->next_move;
	uint64_t i;
	for(i = 0; i < 64; i++){
		if(IS_SET(BOARD(c), i)) continue;
		else if(i == NORTH_OF(pos) || i == SOUTH_OF(pos) || i ==  EAST_OF(pos) || i ==  WEST_OF(pos) || i == NE_OF(pos) || i == NW_OF(pos) || i == SE_OF(pos) || i == SW_OF(pos)){
				m=m->next_move;
				m->from = pos;
				m->to = i;
				m->piece = KING;
				m->next_move=(Move*)malloc(sizeof(Move));
			}
	}
	m->next_move=NULL;
	validate_castle(m, pos, c);
	return head;
}

Move *Night_moves(unsigned int pos, PlayerColor c){
	Move *m = (Move*)malloc(sizeof(Move));
	m->next_move=(Move*)malloc(sizeof(Move));
	Move * head = m->next_move;
	uint64_t i;
	for(i = 0; i < 64; i++){
		if(IS_SET(BOARD(c), i)) continue;
    	else if(i == NE_OF(NORTH_OF(pos)) || i == NW_OF(NORTH_OF(pos)) || i == NW_OF(WEST_OF(pos)) || i == SW_OF(WEST_OF(pos)) || i == SW_OF(SOUTH_OF(pos)) || i == SE_OF(SOUTH_OF(pos)) || i == SE_OF(EAST_OF(pos)) || i == NE_OF(EAST_OF(pos))){
			m=m->next_move;
			m->from = pos;
			m->to = i;
			m->piece = NIGHT;
			m->next_move = (Move*)malloc(sizeof(Move));
			}
	    }
	m->next_move=NULL;
	return head;
}

Move *Bishop_moves(unsigned int pos, PlayerColor c, Piece piece){
	Move *m = (Move*)malloc(sizeof(Move));
	m->next_move=(Move*)malloc(sizeof(Move));
	Move * head = m->next_move;
	int shmounter=0;
	unsigned int copypos = pos;
	while(NW_OF(copypos) != UNKNOWN_POS){
		if(IS_SET(BOARD(c), NW_OF(copypos))) break;
		m = m->next_move;
		m->from = pos;
		m->to = NW_OF(copypos);
		m->piece = piece;
		m->next_move = (Move*)malloc(sizeof(Move));
		shmounter++;
		if(IS_SET(BOARD(1-c), NW_OF(copypos))) break;
		copypos=NW_OF(copypos);
	}

	copypos=pos;
	while(NE_OF(copypos) != UNKNOWN_POS){
		if(IS_SET(BOARD(c), NE_OF(copypos))) break;
		m = m->next_move;
		m->from = pos;
		m->to = NE_OF(copypos);
		m->piece = piece;
		m->next_move = (Move*)malloc(sizeof(Move));
		shmounter++;
		if(IS_SET(BOARD(1-c), NE_OF(copypos))) break;
		copypos=NE_OF(copypos);

}

	copypos=pos;
	while(SW_OF(copypos) != UNKNOWN_POS){
		if(IS_SET(BOARD(c), SW_OF(copypos))) break;
		m = m->next_move;
		m->from = pos;
		m->to = SW_OF(copypos);
		m->piece = piece;
		m->next_move = (Move*)malloc(sizeof(Move));
		shmounter++;
		if(IS_SET(BOARD(1-c), SW_OF(copypos))) break;
		copypos=SW_OF(copypos);
	}

	copypos=pos;
	while(SE_OF(copypos) != UNKNOWN_POS){
		if(IS_SET(BOARD(c), SE_OF(copypos))) break;
		m = m->next_move;
		m->from = pos;
		m->to = SE_OF(copypos);
		m->piece = piece;
		m->next_move = (Move*)malloc(sizeof(Move));
		shmounter++;
		if(IS_SET(BOARD(1-c), SE_OF(copypos))) break;
		copypos=SE_OF(copypos);
	}
	Move *current = head;
	int i;
	for(i=0; i < shmounter-1; i++)
		current=current->next_move;
	current->next_move=NULL;
	return head;
}

Move *Rook_moves(unsigned int pos, PlayerColor c, Piece piece){
	Move *m = (Move*)malloc(sizeof(Move));
	m->next_move=(Move*)malloc(sizeof(Move));
	Move * head = m->next_move;
	int shmounter=0;
	unsigned int copypos = pos;
	while(NORTH_OF(copypos) != UNKNOWN_POS && !(IS_SET(BOARD(c), NORTH_OF(copypos)))){
		m=m->next_move;
		m->from = pos;
		m->to = NORTH_OF(copypos);
		m->piece = piece;
		m->next_move = (Move*)malloc(sizeof(Move));
		shmounter++;
		if(IS_SET(BOARD(1-c), NORTH_OF(copypos))) break;
		copypos=NORTH_OF(copypos);
	} 

	copypos=pos;
	while(SOUTH_OF(copypos) != UNKNOWN_POS && !(IS_SET(BOARD(c), SOUTH_OF(copypos)))){
		m=m->next_move;
		m->from = pos;
		m->to = SOUTH_OF(copypos);
		m->piece = piece;
		m->next_move = (Move*)malloc(sizeof(Move));
		shmounter++;
		if(IS_SET(BOARD(1-c), SOUTH_OF(copypos))) break;
		copypos=SOUTH_OF(copypos);
	} 

	copypos=pos;
	while(EAST_OF(copypos) != UNKNOWN_POS && !(IS_SET(BOARD(c), EAST_OF(copypos)))){
		m=m->next_move;
		m->from = pos;
		m->to = EAST_OF(copypos);
		m->piece = piece;
		m->next_move = (Move*)malloc(sizeof(Move));
		shmounter++;
		if(IS_SET(BOARD(1-c), EAST_OF(copypos))) break;
		copypos=EAST_OF(copypos);
	} 

	copypos=pos;
	while(WEST_OF(copypos) != UNKNOWN_POS && !(IS_SET(BOARD(c), WEST_OF(copypos)))){
		m=m->next_move;
		m->from = pos;
		m->to = WEST_OF(copypos);
		m->piece = piece;
		m->next_move = (Move*)malloc(sizeof(Move));
		shmounter++;
		if(IS_SET(BOARD(1-c), WEST_OF(copypos))) break;
		copypos=WEST_OF(copypos);
	}

	Move *current = head;
	int i;
	for(i=0; i<shmounter-1; i++)
		current=current->next_move;
	current->next_move=NULL;
	return head;
}

Move *Queen_moves(unsigned int pos, PlayerColor c){
	Move * m1 = Rook_moves(pos, c, QUEEN);
	Move * m2 = Bishop_moves(pos, c, QUEEN);
	Move * head = m1;
	while(m1->next_move !=NULL)
		m1 = m1->next_move;
	m1->next_move = m2;
	return head;
}

Move *Pawn_moves(unsigned int pos, PlayerColor c){
	Move * m = (Move*)malloc(sizeof(Move));
	m->next_move=(Move*)malloc(sizeof(Move));
	Move * head = m->next_move;
	if(c==WHITE){
		if(UNOCCUPIED(NORTH_OF(pos))){
			if(pos<=55 && pos>=48){
				if(UNOCCUPIED(NORTH_OF(NORTH_OF(pos)))){
					m=m->next_move;
					m->from = pos;
					m->to = NORTH_OF(NORTH_OF(pos));
					m->piece = PAWN;
					m->next_move = (Move*)malloc(sizeof(Move));
				}
			}
			if(UNOCCUPIED(NORTH_OF(pos))){
				m=m->next_move;
				m->from = pos;
				m->to = NORTH_OF(pos);
				m->piece = PAWN;
				m->next_move = (Move*)malloc(sizeof(Move));
			}
		}
		if(IS_SET(BOARD(BLACK),NE_OF(pos))){
			m=m->next_move;
			m->from = pos;
			m->to = NE_OF(pos);
			m->piece = PAWN;
			m->next_move = (Move*)malloc(sizeof(Move));
		}
		if(IS_SET(BOARD(BLACK),NW_OF(pos))){
			m=m->next_move;
			m->from = pos;
			m->to = NW_OF(pos);
			m->piece = PAWN;
			m->next_move = (Move*)malloc(sizeof(Move));
		}

		if(NW_OF(pos) == ep_square && IS_SET(player[1-c].p,WEST_OF(pos))){
			m=m->next_move;
			m->from = pos;
			m->to = NW_OF(pos);
			m->piece = PAWN;
			m->next_move = (Move*)malloc(sizeof(Move));
		}

		if(NE_OF(pos) == ep_square && IS_SET(player[1-c].p,EAST_OF(pos))){
			m=m->next_move;
			m->from = pos;
			m->to = NE_OF(pos);
			m->piece = PAWN;
			m->next_move = (Move*)malloc(sizeof(Move));
		}
	}

	if(c==BLACK){
		if(UNOCCUPIED(SOUTH_OF(pos))){
			if(pos<=15 && pos>=8){
				if(UNOCCUPIED(SOUTH_OF(SOUTH_OF(pos)))){
					m=m->next_move;
					m->from = pos;
					m->to = SOUTH_OF(SOUTH_OF(pos));
					m->piece = PAWN;
					m->next_move = (Move*)malloc(sizeof(Move));
				}
			}
			if(UNOCCUPIED(SOUTH_OF(pos))){
				m=m->next_move;
				m->from = pos;
				m->to = SOUTH_OF(pos);
				m->piece = PAWN;
				m->next_move = (Move*)malloc(sizeof(Move));
			}
		}
		if(IS_SET(BOARD(WHITE),SE_OF(pos))){
			m=m->next_move;
			m->from = pos;
			m->to = SE_OF(pos);
			m->piece = PAWN;
			m->next_move = (Move*)malloc(sizeof(Move));
		}
		if(IS_SET(BOARD(WHITE),SW_OF(pos))){
			m=m->next_move;
			m->from = pos;
			m->to = SW_OF(pos);
			m->piece = PAWN;
			m->next_move = (Move*)malloc(sizeof(Move));
		}
		if(SW_OF(pos) == ep_square && IS_SET(player[1-c].p,WEST_OF(pos))){
			m=m->next_move;
			m->from = pos;
			m->to = SW_OF(pos);
			m->piece = PAWN;
			m->next_move = (Move*)malloc(sizeof(Move));
		}

		if(SE_OF(pos) == ep_square && IS_SET(player[1-c].p,EAST_OF(pos))){
			m=m->next_move;
			m->from = pos;
			m->to = SE_OF(pos);
			m->piece = PAWN;
			m->next_move = (Move*)malloc(sizeof(Move));
		}
	}
	m->next_move=NULL;
	return head;
}

/* Given a color, this function returns a singly linked list of all legal Moves with the head at *m. 
 * The function returns TRUE if at least 1 legal move is available. 
 * The total number of moves found is stored in the address pointed to by pcount. */
Move* legal_moves(PlayerColor c, unsigned int *pcount) {
	Move * temporary = (Move*)malloc(sizeof(Move)); 
	Move * headholder = temporary;
	Move*head=headholder;
	int i;
	for(i = 0; i < 64; i++){
		if(IS_SET(player[c].k, i)){
			if(King_moves(i,c)->piece!=UNKNOWN)
				temporary->next_move = King_moves(i, c);
			while(temporary->next_move != NULL) temporary = temporary->next_move; 
		}
		else if(IS_SET(player[c].q, i)){
			if(Queen_moves(i,c)->piece!=UNKNOWN)
				temporary->next_move = Queen_moves(i, c);
			while(temporary->next_move != NULL) temporary = temporary->next_move; 
		}
		else if(IS_SET(player[c].n, i)){
			if(Night_moves(i,c)->piece!=UNKNOWN)
				temporary->next_move = Night_moves(i, c);
			while(temporary->next_move != NULL) temporary = temporary->next_move; 
		}
		else if(IS_SET(player[c].b, i)){
			if(Bishop_moves(i,c,BISHOP)->piece!=UNKNOWN)
				temporary->next_move = Bishop_moves(i, c, BISHOP);
			while(temporary->next_move != NULL) temporary = temporary->next_move; 
		}
		else if(IS_SET(player[c].r, i)){
			if(Rook_moves(i,c,ROOK)->piece!=UNKNOWN)
				temporary->next_move = Rook_moves(i, c, ROOK);
			while(temporary->next_move != NULL) temporary = temporary->next_move; 
		}
		else if(IS_SET(player[c].p, i)){
			if(Pawn_moves(i,c)->piece!=UNKNOWN)
				temporary->next_move = Pawn_moves(i, c);
			while(temporary->next_move != NULL) temporary = temporary->next_move; 
		}
	}
	unsigned int accum = 0;
	while(headholder->next_move != NULL){
		accum++;
		headholder = headholder->next_move;
	}
	pcount=&accum;
	return head->next_move;
}

Bool is_check(PlayerColor c){
	unsigned int * pcount = 0;
	Move * m = legal_moves(1-c,pcount);
	uint64_t i;
	uint64_t king_pos;
	for(i = 0; i < 64; i++){
		if(IS_SET(player[c].k, i)) king_pos = i;
	}
	while(m->next_move != NULL){
		if(m->to == king_pos){
		       free(m);
			return TRUE;}
		m = m->next_move;
	}
	free(m);
	return FALSE;
}

Bool validate_and_move(Move *move,char* errmsg, PlayerColor c) {
	Move * m = (Move *)malloc(sizeof(Move));
	unsigned int *num=0;
	m = legal_moves(c, num);
	if(num){
		printf("No legal moves\n");
		return FALSE;
	}
	Move * temp = m;
	while(temp != NULL){
		if(move->to == temp->to && move->from == temp->from && move->piece == temp->piece){
			if(move->piece == PAWN && move->to == NORTH_OF(NORTH_OF(move->from))){
				 ep_square = (uint64_t) SOUTH_OF(move->to);
			}
			if(move->piece == PAWN && move->to == SOUTH_OF(SOUTH_OF(move->from))){
				ep_square = (uint64_t) NORTH_OF(move->to);
			}
			if(IS_SET(BOARD(1-c), move->to)){
				if(IS_SET(player[1-c].r, move->to))
					RESET_BIT(player[1-c].r, move->to);
				else if(IS_SET(player[1-c].b, move->to))
					RESET_BIT(player[1-c].b, move->to);
				else if(IS_SET(player[1-c].n, move->to))
					RESET_BIT(player[1-c].n, move->to);
				else if(IS_SET(player[1-c].p, move->to))
					RESET_BIT(player[1-c].p, move->to);
				else if(IS_SET(player[1-c].k, move->to))
					RESET_BIT(player[1-c].k, move->to);
				else if(IS_SET(player[1-c].q, move->to))
					RESET_BIT(player[1-c].q, move->to);
			}

			switch(move->piece){
				case(KING):
					if(detect_castle_move(move, c)==1){
						perform_castle(move->to, c);
						break;
					}

					if(c == WHITE) SET_BIT(player[c].castle_flags, 0);
					else SET_BIT(player[c].castle_flags, 1);
					SET_BIT(player[c].k, move->to);
					RESET_BIT(player[c].k, move->from);
					break;
				case(QUEEN):
					SET_BIT(player[c].q, move->to);
					RESET_BIT(player[c].q, move->from);
					break;
				case(PAWN):
					if(move->to == ep_square){
						SET_BIT(player[c].p, move->to);
						RESET_BIT(player[c].p, move->from);
						if(1-c == WHITE) RESET_BIT(player[1-c].p, ep_square-8);
						else RESET_BIT(player[1-c].p, ep_square+8);
					}
					else if((move->to >= 0 && move->to <= 7) || (move->to >= 55 && move->to <= 63)){
						RESET_BIT(player[c].p, move->from);
						if(move->promotion_choice==QUEEN){
								SET_BIT(player[c].q, move->to);
								break;
						}
							else if(move->promotion_choice==ROOK){
								SET_BIT(player[c].r, move->to);
								break;
							}
							else if(move->promotion_choice==BISHOP){
								SET_BIT(player[c].b, move->to);
								break;
							}
							else if(move->promotion_choice==NIGHT){
								SET_BIT(player[c].n, move->to);
								break;
							}
							else{
								SET_BIT(player[c].q, move->to);
								break;
							}
						}
					else{
						SET_BIT(player[c].p, move->to);
						RESET_BIT(player[c].p, move->from);
					}
					break;
				case(ROOK):
					if(move->from == 56 && c == WHITE) SET_BIT(player[c].castle_flags, 2);
					if(move->from == 63 && c == WHITE) SET_BIT(player[c].castle_flags, 3);
					if(move->from == 0 && c == BLACK) SET_BIT(player[c].castle_flags, 4);
					if(move->from == 7 && c == BLACK) SET_BIT(player[c].castle_flags, 5);
					SET_BIT(player[c].r, move->to);
					RESET_BIT(player[c].r, move->from);
					break;
				case(BISHOP):
					SET_BIT(player[c].b, move->to);
					RESET_BIT(player[c].b, move->from);
					break;
				case(NIGHT):
					SET_BIT(player[c].n, move->to);
					RESET_BIT(player[c].n, move->from);
					break;
				case(UNKNOWN):
					strcpy(errmsg,"Not a valid piece");
					break;
			}
			free(m);
			return TRUE;
		}
		temp=temp->next_move;
	}
	strcpy(errmsg,"Not a valid move");
	free(m);
	return FALSE;
}

/* Returns TRUE if the CurrentPlayer is under checkmate, FALSE otherwise. */
Bool is_checkmate(PlayerColor c) {
	PlayerState temp;
	temp.castle_flags=player[c].castle_flags;
	temp.r=player[c].r;
	temp.n=player[c].n;
	temp.b=player[c].b;
	temp.p=player[c].p;
	temp.k=player[c].k;
	temp.q=player[c].q;

	PlayerState temp2;
	temp2.castle_flags=player[1-c].castle_flags;
	temp2.r=player[1-c].r;
	temp2.n=player[1-c].n;
	temp2.b=player[1-c].b;
	temp2.p=player[1-c].p;
	temp2.k=player[1-c].k;
	temp2.q=player[1-c].q;
	char* msg = (char*)malloc(sizeof(char)*20);
	Move * m = (Move *)malloc(sizeof(Move));
	unsigned int * num = 0;
	m = legal_moves(c, num);
	while(m != NULL){
		validate_and_move(m, msg, c);
		if(is_check(c)==FALSE){
			player[c].castle_flags=temp.castle_flags;
			player[c].r=temp.r;
			player[c].n=temp.n;
			player[c].b=temp.b;
			player[c].p=temp.p;
			player[c].k=temp.k;
			player[c].q=temp.q;

			player[1-c].castle_flags=temp2.castle_flags;
			player[1-c].r=temp2.r;
			player[1-c].n=temp2.n;
			player[1-c].b=temp2.b;
			player[1-c].p=temp2.p;
			player[1-c].k=temp2.k;
			player[1-c].q=temp2.q;
			free(m);
			return FALSE;
		}
		player[c].castle_flags=temp.castle_flags;
		player[c].r=temp.r;
		player[c].n=temp.n;
		player[c].b=temp.b;
		player[c].p=temp.p;
		player[c].k=temp.k;
		player[c].q=temp.q;

		player[1-c].castle_flags=temp2.castle_flags;
		player[1-c].r=temp2.r;
		player[1-c].n=temp2.n;
		player[1-c].b=temp2.b;
		player[1-c].p=temp2.p;
		player[1-c].k=temp2.k;
		player[1-c].q=temp2.q;
		m = m->next_move;
	}
	free(m);
	return TRUE;
}

/* Function to decide whether the current position is a draw */
/* Draw can be due to insufficient material, 3 move repetition or stalemate */
/* 3 move repetition also covers perpetual check */
Bool is_draw() {
	int b1=0;
	int b2=0;
	int n1=0;
	int n2=0;
	int i;
	for(i=0; i<64; i++){
		if(get_piece_at(i,0)==BISHOP){
			b1++;
		}
		if(get_piece_at(i,1)==BISHOP){
			b2++;
		}
		if(get_piece_at(i,0)==NIGHT){
			n1++;
		}
		if(get_piece_at(i,1)==NIGHT){
			n2++;
		}
	}
	/* Insufficient Materials */
	if(BOARD(BLACK)==(player[BLACK].k | player[BLACK].n) && BOARD(WHITE)==(player[WHITE]).k && n1<=2){
		printf("Black king or knight against white king");
		return TRUE;
	}
	if(BOARD(WHITE)==(player[WHITE].k | player[WHITE].n) && BOARD(BLACK)==(player[BLACK]).k && n2<=2){
		printf("White king or knight against a black king");
		return TRUE;}
	if(BOARD(BLACK)==(player[BLACK].k | player[BLACK].b) && BOARD(WHITE)==(player[WHITE]).k && b1<=2){
		printf("Black king or bishop against a white king");

		return TRUE;
	}
	if(BOARD(WHITE)==(player[WHITE].k | player[WHITE].b) && BOARD(BLACK)==(player[BLACK]).k && b2<=2){
		printf("White king or bishop against a blakc king");
		return TRUE;
	}
	/* Stalemate */
	Move * m = (Move *)malloc(sizeof(Move));
	unsigned int *num = 0;
	m = legal_moves(CurrentPlayer,num);

	if(m->piece==UNKNOWN && is_check(CurrentPlayer) == FALSE){
	       printf("Theres a stalemate");
	       free(m);
		return TRUE;
	}
	free(m);
	return FALSE;
}

/* Returns the piece on a square belonging to player color c. 
 * If there is no piece with color c, UNKNOWN is returned. */
Piece get_piece_at(Board pos, PlayerColor c) {
	if(IS_SET(player[c].k, pos) != 0) return KING;
	if(IS_SET(player[c].q, pos) != 0) return QUEEN;
	if(IS_SET(player[c].b, pos) != 0) return BISHOP;
	if(IS_SET(player[c].r, pos) != 0) return ROOK;
	if(IS_SET(player[c].n, pos) != 0) return NIGHT;
	if(IS_SET(player[c].p, pos) != 0) return PAWN;
	return UNKNOWN;
}

