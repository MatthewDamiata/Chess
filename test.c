#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "chess.h"
#include "io.h"
#include "moves.h"

#define BIT(n) 			(1UL << (n))
#define SET_BIT(x, n)	((x) |= BIT((n)))
#define RESET_BIT(x, n) ((x) &= ~BIT(n))
#define IS_SET(x, n) 	((x) & BIT((n)))

int main(int argc, char *argv[])
{
	player[WHITE].castle_flags = 0xFF;
	player[BLACK].castle_flags = 0xFF;
	Bool ret;
	Move * t = (Move*)malloc(sizeof(Move));
	Move * t2 = (Move*)malloc(sizeof(Move));
	char * msg = (char*)malloc(sizeof(char)*20);
	if(argc != 2) return -1;

	ret = parse_board(argv[1]);
	if(ret == FALSE) {
		printf("Invalid board configuration.\n");
		return -1;
	}
	display_board();
	Move * newmove = (Move*)malloc(sizeof(Move));
	int i = 0;
	int tempcolor;
	Bool mate1;
	Bool mate2;
	PlayerColor localcolor = CurrentPlayer;
	Bool itwaslegal = FALSE;
	while(1){

		if(localcolor == 1) printf("White's move\n");
		else printf("Black's move\n");


		if(is_check(1-localcolor) == TRUE){
			printf("Player failed to get out of check, ");
			if(localcolor == 1) printf("White wins!\n");
			else printf("Black wins!\n");
			break;
		}

		if(is_check(localcolor) == TRUE){
			if(localcolor == 0) printf("Black is under check.\n");
			else printf("White is under check.\n");
		}

		if(is_checkmate(1-localcolor) == TRUE){
			if(1-localcolor == 0) printf("Black is under checkmate, White wins!\n");
			else printf("White is under checkmate, Black wins!\n");
			break;
		}

		if(is_checkmate(localcolor) == TRUE){
			if(localcolor == 1) printf("White is under checkmate, Black wins!\n");
			else printf("Black is under checkmate, White wins!\n");
			break;
		}

		if(is_draw()==TRUE){
			printf("There is a draw.\n");
			break;
		}

		mate1 = run_mate1(localcolor, t);
		printf("Mate in 1: ");
		if(mate1 == 1) printf("True\n");
		else printf("False\n");
		if(mate1 == TRUE) printf("Solution for mate in 1: %c%d-%c%d\n", FILE_OF(t->from), RANK_OF(t->from) - '0', FILE_OF(t->to), RANK_OF(t->to) - '0');

		mate2 = run_mate2(localcolor,t2);
		printf("Mate in 2: ");
		if(mate2 == 1) printf("True\n");
		else printf("False\n");
		if(mate2 == TRUE) printf("Solution for mate in 2: %c%d-%c%d\n", FILE_OF(t2->from), RANK_OF(t2->from) - '0', FILE_OF(t2->to), RANK_OF(t2->to) - '0');

		prompt_move(newmove);
		newmove->piece = get_piece_piece(newmove->from);
		if(newmove->piece == PAWN &&((newmove->to >= 0 && newmove->to <= 7)||(newmove->to <= 63 && newmove->to >= 56))){
			printf("Promotion: ");
			scanf(" %d",&tempcolor);
			switch(tempcolor){
				case(2):
					newmove->promotion_choice = 2;
					break;
				case(3):
					newmove->promotion_choice = 3;
					break;
				case(4):
					newmove->promotion_choice = 4;
					break;
				case(5):
					newmove->promotion_choice = 5;
					break;
				default:
					newmove->promotion_choice = 2;
					break;
			}
			printf("\n");
		}
		if(newmove->from == -1 || newmove->to == -1 || newmove->piece == -1) break;
		itwaslegal = validate_and_move(newmove, msg, localcolor);
		display_board();
		i++;
		if(itwaslegal) localcolor = 1-localcolor;
	}


	if(is_checkmate(argv[1], WHITE)) {
		printf("White is under checkmate\n"); 
	}
	if(is_checkmate(argv[1], BLACK)) {
		printf("Black is under checkmate\n"); 
	}

	return 0; 
}
