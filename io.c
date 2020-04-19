#include "chess.h"
#include "io.h"
#include "moves.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

char get_piece(uint8_t pos){
	if(IS_SET(player[WHITE].r, pos)) return 'R';
	if(IS_SET(player[WHITE].q, pos)) return 'Q';
	if(IS_SET(player[WHITE].b, pos)) return 'B';
	if(IS_SET(player[WHITE].k, pos)) return 'K';
	if(IS_SET(player[WHITE].n, pos)) return 'N';
	if(IS_SET(player[WHITE].p, pos)) return 'P';
	if(IS_SET(player[BLACK].r, pos)) return 'r';
	if(IS_SET(player[BLACK].q, pos)) return 'q';
	if(IS_SET(player[BLACK].b, pos)) return 'b';
	if(IS_SET(player[BLACK].k, pos)) return 'k';
	if(IS_SET(player[BLACK].n, pos)) return 'n';
	if(IS_SET(player[BLACK].p, pos)) return 'p';
	return ' ';
}

char * convertmovetostring(Move * m){
	char * str = (char*)malloc(sizeof(char) * 7);
	str[0] = FILE_OF(m->from);
	str[1] = (char)((RANK_OF(m->from)));
	str[2] = '-';
	str[3] = FILE_OF(m->to);
	str[4] = (char)((RANK_OF(m->to)));
	str[5] = '\n';
	str[6] = '\0';
	return str;
}

Piece get_piece_piece(uint64_t pos){
	if(IS_SET(player[WHITE].r, pos)) return ROOK;
	if(IS_SET(player[WHITE].q, pos)) return QUEEN;
	if(IS_SET(player[WHITE].b, pos)) return BISHOP;
	if(IS_SET(player[WHITE].k, pos)) return KING;
	if(IS_SET(player[WHITE].n, pos)) return NIGHT;
	if(IS_SET(player[WHITE].p, pos)) return PAWN;
	if(IS_SET(player[BLACK].r, pos)) return ROOK;
	if(IS_SET(player[BLACK].q, pos)) return QUEEN;
	if(IS_SET(player[BLACK].b, pos)) return BISHOP;
	if(IS_SET(player[BLACK].k, pos)) return KING;
	if(IS_SET(player[BLACK].n, pos)) return NIGHT;
	if(IS_SET(player[BLACK].p, pos)) return PAWN;
	return UNKNOWN;
}

void display_board() {
	unsigned int i = 0, j = 0;
	printf("\n  __________________________________\n");
	for(i = 0; i < 8; i++){
		printf("%d |", 8-i);
		for(j = 0; j < 8; j++){
		printf(" %c |", get_piece(i*8+j));
	}
	printf("\n  __________________________________\n");
	}
	printf("    a   b   c   d   e   f   g   h\n");
	printf("\n");
}

Bool parse_board(char *board) {
	int i = 0;
	int slash_count=0;

	int poscount=0;
	char epfile;
	uint64_t eprank;
	int aftercount = 0;
	while(i<strlen(board)){
		if(poscount >= 63){
			if(board[i] == 'w'){
				CurrentPlayer = WHITE;
				aftercount++;
			}
			else if(board[i] == 'b'){
				if(aftercount == 0){
					CurrentPlayer = BLACK;
					aftercount++;
				}
				else{
					epfile = 'b';
				}
			}
			else if(board[i] == 'a'){
				epfile = 'a';
			}
			else if(board[i] == 'c'){
				epfile = 'c';
			}
			else if(board[i] == 'd'){
				epfile = 'd';
			}
			else if(board[i] == 'e'){
				epfile = 'e';
			}
			else if(board[i] == 'f'){
				epfile = 'f';
			}
			else if(board[i] == 'g'){
				epfile = 'g';
			}
			else if(board[i] == 'h'){
				epfile = 'h';
			}
			else if(board[i] == ' '){
				i++;
				continue;
			}
			else if(board[i] == 'K'){
				RESET_BIT(player[WHITE].castle_flags, 0);
				RESET_BIT(player[WHITE].castle_flags, 3);
			}
			else if(board[i] == 'Q'){
				RESET_BIT(player[WHITE].castle_flags, 0);
				RESET_BIT(player[WHITE].castle_flags, 2);
			}
			else if(board[i] == 'k'){
				RESET_BIT(player[BLACK].castle_flags, 1);
				RESET_BIT(player[BLACK].castle_flags, 5);
			}
			else if(board[i] == 'q'){
				RESET_BIT(player[BLACK].castle_flags, 1);
				RESET_BIT(player[BLACK].castle_flags, 4);
			}
			else if(board[i] == '-'){
				i++;
				continue;
			}
			else{
				eprank = board[i];
				ep_square = TO_POS(epfile, eprank);
			}
			i++;
		} else{
		if(board[i] == 'r'){ 
			SET_BIT(player[BLACK].r, poscount);
			poscount++;}
		else if(board[i] == 'n'){ 
			SET_BIT(player[BLACK].n, poscount);
			poscount++;}
		else if(board[i] == 'b'){ 
			SET_BIT(player[BLACK].b, poscount);
			poscount++;}
		else if(board[i] == 'q'){ 
			SET_BIT(player[BLACK].q, poscount);
			poscount++;}
		else if(board[i] == 'k'){ 
			SET_BIT(player[BLACK].k, poscount);
			poscount++;}
		else if(board[i] == 'p'){
			SET_BIT(player[BLACK].p, poscount);
			poscount++;}
		else if(board[i] == 'R'){ 
			SET_BIT(player[WHITE].r, poscount);
			poscount++;}
		else if(board[i] == 'B'){
			SET_BIT(player[WHITE].b, poscount);
			poscount++;}
		else if(board[i] == 'Q'){ 
			SET_BIT(player[WHITE].q, poscount);
			poscount++;}
		else if(board[i] == 'N'){ 
			SET_BIT(player[WHITE].n, poscount);
			poscount++;}
		else if(board[i] == 'K'){ 
			SET_BIT(player[WHITE].k, poscount);
			poscount++;}
		else if(board[i] == 'P'){
			SET_BIT(player[WHITE].p, poscount);
			poscount++;}
		else if(board[i] == '/'){
		slash_count++;
		if(poscount%8!=0){
			return FALSE;}
		}
		else{
			poscount += (board[i]-'0');
		}

		i++;
	}
	}
	if(slash_count!=7){
		return FALSE;} 
	return TRUE;
}

Move parse_move() {
	Move * m = (Move *)malloc(sizeof(Move));
	m->from=0;
	m->to=0;
	m->piece=UNKNOWN;
	m->promotion_choice=UNKNOWN;
	m->next_move=NULL;
	prompt_move(m);
	if(m->from < 0 || m->from > 63) printf("Invalid from\n");
	if(m->to < 0 || m->to > 63) printf("Invalid to\n");
	return *m;
}


void prompt_move(Move *m) {
	char s[128];
	while(1){
   		printf("Enter move: ");
   		fgets(s, 127, stdin);
		if(s[2]=='-' && strlen(s) == 6) break;
		printf("Missing '-'\n");
   	}
	m->from = TO_POS(s[0], s[1]);
	m->to = TO_POS(s[3], s[4]);
}

void print_move(Move *m) {
	switch(m->piece){
		case(KING):
			printf("KING");
			break;
		case(QUEEN):
			printf("QUEEN");
			break;
		case(BISHOP):
			printf("BISHOP");
			break;
		case(NIGHT):
			printf("KNIGHT");
			break;
		case(PAWN):
			printf("PAWN");
			break;
		case(ROOK):
			printf("ROOK");
			break;
		default:
			printf("UNKNOWN");
			break;
	}
	printf(" %c%d ->", FILE_OF(m->from),RANK_OF(m->from)-'0');
	printf(" %c%d", FILE_OF(m->to),RANK_OF(m->to)-'0');
}

