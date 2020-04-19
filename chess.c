#include "chess.h"
#include "moves.h"

Bool run_mate1(PlayerColor c, Move *soln){
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

	Move * m = (Move *)malloc(sizeof(Move));
	unsigned int * num = 0;
	m=legal_moves(c, num);
	char* msg=(char*)malloc(sizeof(char)*20);
	while(m->next_move!=NULL){
		validate_and_move(m,msg,c);
		if(is_checkmate(1-c)==TRUE){
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
			soln->to=m->to;
			soln->from=m->from;
			soln->piece=m->piece;
			soln->promotion_choice=m->promotion_choice;
			free(m);
			return TRUE;}

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
		m=m->next_move;
	}
	free(m);
	return FALSE;
}

Bool run_mate2(PlayerColor c, Move *soln){
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

	Move *p = (Move*)malloc(sizeof(Move));

	Move * m = (Move *)malloc(sizeof(Move));
	Move * n = (Move*)malloc(sizeof(Move));
	unsigned int * num = 0;
	m=legal_moves(c, num);
	char* msg=(char*)malloc(sizeof(char)*20);

	while(m->next_move!=NULL){
		Bool correct = FALSE;
		validate_and_move(m, msg, c);
		n=legal_moves(1-c, num);
		PlayerState temp3;
		temp3.castle_flags=player[c].castle_flags;
		temp3.r=player[c].r;
		temp3.n=player[c].n;
		temp3.b=player[c].b;
		temp3.p=player[c].p;
		temp3.k=player[c].k;
		temp3.q=player[c].q;

		PlayerState temp4;
		temp4.castle_flags=player[1-c].castle_flags;
		temp4.r=player[1-c].r;
		temp4.n=player[1-c].n;
		temp4.b=player[1-c].b;
		temp4.p=player[1-c].p;
		temp4.k=player[1-c].k;
		temp4.q=player[1-c].q;

		while(n->next_move!=NULL){
			validate_and_move(n, msg, 1-c);
			if(is_check(1-c)==FALSE && run_mate1(c,p)==FALSE){
				correct=TRUE;
				break;
			}
			n=n->next_move;
			player[c].castle_flags=temp3.castle_flags;
			player[c].r=temp3.r;
			player[c].n=temp3.n;
			player[c].b=temp3.b;
			player[c].p=temp3.p;
			player[c].k=temp3.k;
			player[c].q=temp3.q;

			player[1-c].castle_flags=temp4.castle_flags;
			player[1-c].r=temp4.r;
			player[1-c].n=temp4.n;
			player[1-c].b=temp4.b;
			player[1-c].p=temp4.p;
			player[1-c].k=temp4.k;
			player[1-c].q=temp4.q;
		}
		free(n);
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
		run_mate1(c, p);
		if(correct == FALSE && m->from != p->from && m->to != p->to){
			soln->to = m->to;
			soln->from = m->from;
			soln->piece = m->piece;
			soln->promotion_choice = m->promotion_choice;
			free(m);
			free(p);
			return TRUE;}
		m=m->next_move;
	}
	free(m);
	free(p);
	return FALSE;
}
