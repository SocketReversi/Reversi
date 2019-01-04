#include <stdio.h>
#include "../../libs/game.h"

void createTableList(tableGame table[MAX_TABLE]){
	for(int i=0;i<MAX_TABLE;i++){
		table[i].state = EMPTY;
		table[i].master = EMPTY;
		table[i].guest = EMPTY;
		table[i].id = i;		
	}
}

int createTable(int IDmaster,tableGame table[MAX_TABLE]){
	for(int i=0;i<MAX_TABLE;i++){
		if(table[i].state == EMPTY){
			table[i].master = IDmaster;
			table[i].state  = WAITING;
			return 1; //tao thanh cong 1 ban choi moi
		}
	}
	return 0; //khong tao duoc ban choi
}

int joinTable(int IDguest, tableGame table[MAX_TABLE]){
	for(int i=0 ; i<MAX_TABLE ; i++){
		if(table[i].state == WAITING){
			table[i].guest = IDguest;
			table[i].state = FULL;
			return 1; //tham gia ban choi thanh cong
		}
	}
	return 0; //khong tham gia duoc ban choi nao
}

int findIDgamer(int IDgamer,tableGame table[MAX_TABLE]){
	for(int i=0; i<MAX_TABLE ; i++){
		if(IDgamer == table[i].master)
			return table[i].id;
		else if(IDgamer == table[i].guest){
			return (MAX_TABLE + table[i].id);
		}
	}
	return EMPTY;
}

int findID(int id, tableGame table[MAX_TABLE]){
	for(int i=0; i<MAX_TABLE ; i++){
		if(id == table[i].master || id == table[i].guest)
			return table[i].id;
	}
	return EMPTY;
}

int leaveTable(int IDgamer, tableGame table[MAX_TABLE]){
	int find = findID(IDgamer,table);
	if(find != EMPTY ){
		table[find].master = EMPTY;
		table[find].guest  = EMPTY;
		table[find].state  = EMPTY;
		return find;
	}
	return EMPTY;
}

void printTable(tableGame table[MAX_TABLE]){
	for(int i=0 ; i<MAX_TABLE ; i++){
		printf("IDTable:%d\nState:%d\nMaster:%d\nGuest:%d\n\n",table[i].id,table[i].state,table[i].master,table[i].guest);
	}
}