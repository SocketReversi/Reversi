#include "lib.h"

void createTableList(tableGame table[MAX_TABLE]){
	for(int i=0;i<MAX_TABLE;i++){
		table[i].state = TRONG;
		table[i].master = TRONG;
		table[i].guest = TRONG;
		table[i].IDtable = i;		
	}
}
int createTable(int IDmaster,tableGame table[MAX_TABLE]){
	for(int i=0;i<MAX_TABLE;i++){
		if(table[i].state == TRONG){
			table[i].master = IDmaster;
			table[i].state  = CO1NGUOI;
			return 1; //tao thanh cong 1 ban choi moi
		}
	}
	return 0; //khong tao duoc ban choi
}

int joinTable(int IDguest, tableGame table[MAX_TABLE]){
	for(int i=0 ; i<MAX_TABLE ; i++){
		if(table[i].state == CO1NGUOI){
			table[i].guest = IDguest;
			table[i].state = CO2NGUOI;
			return 1; //tham gia ban choi thanh cong
		}
	}
	return 0; //khong tham gia duoc ban choi nao
}

int findIDgamer(int IDgamer,tableGame table[MAX_TABLE]){
	for(int i=0; i<MAX_TABLE ; i++){
		if(IDgamer == table[i].master)
			return table[i].IDtable;
		else if(IDgamer == table[i].guest){
			return (MAX_TABLE + table[i].IDtable);
		}
	}
	return TRONG;
}

int findID(int id, tableGame table[MAX_TABLE]){
	for(int i=0; i<MAX_TABLE ; i++){
		if(id == table[i].master || id == table[i].guest)
			return table[i].IDtable;
	}
	return TRONG;
}

void printInfoAccount(acc account){
	printf("---Info Account---\n");
	printf("Account : %s\n",account.username);
	printf("ID 		: %d\n",account.IDacc);
	printf("Rank 	: %d\n",account.rank);
	printf("Point 	: %d\n",account.point);
}

int leaveTable(int IDgamer, tableGame table[MAX_TABLE]){
	int find = findID(IDgamer,table);
	if(find != TRONG ){
		table[find].master = TRONG;
		table[find].guest  = TRONG;
		table[find].state  = TRONG;
		return find;
	}
	return TRONG;
}

void printTable(tableGame table[MAX_TABLE]){
	for(int i=0 ; i<MAX_TABLE ; i++){
		printf("IDTable:%d\nState:%d\nMaster:%d\nGuest:%d\n\n",table[i].IDtable,table[i].state,table[i].master,table[i].guest);
	}
}

