#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "board.h"

#define N_BOARD		20
#define MAX_CHARNAME	200
#define N_PLAYER	3
#define MAX_DIE		6

#define PLAYERSTATUS_LIVE	0
#define PLAYERSTATUS_DIE	1
#define PLAYERSTATUS_END	2

int player_position[N_PLAYER];
char player_name[N_PLAYER][MAX_CHARNAME];
int player_coin[N_PLAYER];
int player_status[N_PLAYER];
char player_statusString[3][MAX_CHARNAME]={"LIVE","DIE","END"};

int getAlivePlayer(void) // END플레이어 명수  
{
	int i;
	int cnt=0;
	
	for(i=0;i<=N_PLAYER;i++)
	{
		if(player_status[i]==PLAYERSTATUS_END) //if 조건 : player 상태 DIE  
			cnt++;
	}
	return cnt;
}
	
int getWinner(void) // winner(코인개수) 
{
	int i;
	int winner=0;
	int max_coin=-1;
	
	for(i=0;i<N_PLAYER;i++)
	{
		if(player_status[i]==PLAYERSTATUS_END && player_coin[i]>max_coin) //if 조건 : 상태 END && 코인 가장 많은 player
		{
			max_coin=player_coin[i];
			winner=i;
		}
	}
	return winner;
}

int getDieWinner(void) // player 상태가 모두 DIE일때의 winner  
{
	int i;
	int Diewinner=0;
	int max_coin=-1;
	
	for(i=0;i<N_PLAYER;i++)
	{
		if(player_status[i]==PLAYERSTATUS_DIE && player_coin[i]>max_coin) //if 조건 : 상태 DIE && 코인 가장 많은 player
		{
			max_coin=player_coin[i];
			Diewinner=i;
		}
	}
	return Diewinner;
}

int game_end(void) // 게임 종료&엔드문구  
{
	int i;
	int flag_end = 0;
	//if all the players are died? = getDieWinner()
	for (i=0;i<N_PLAYER;i++)
	{
		if(player_status[i]==PLAYERSTATUS_LIVE) //if 조건 : 상태 LIVE인 player 존재 
		{
			return 0; //다시 게임 진행  
		}
	}
	flag_end=1;

	if(flag_end){
		int AlivePlayers = getAlivePlayer();
    	int winner = getWinner();
    	int Diewinner = getDieWinner();
    	
		printf("GAME END!!\n"); 
		
		if(AlivePlayers>0) //if 조건 : 상태 END player 존재  
		{
			printf("%d players are alive! Winner is %s\n", AlivePlayers, player_name[winner]);
		}
		else //모든 player가 상태 DIE (DieWinner 사용) 
		{
			printf("All the players have died! Winner is %s\n", player_name[Diewinner]);
		}
		return 1;
	}
	return 0;
}
void opening(void){
	printf("~~+~~+~~+~~+~~+~~+~~+~~+~~+~~+~~+~~+~~+~~+~~+~~\n");
	printf("                 !!SHARK GAME!!                \n");
	printf("~~+~~+~~+~~+~~+~~+~~+~~+~~+~~+~~+~~+~~+~~+~~+~~\n");
}

int rolldie(void) // 주사위 
{
	return rand()%MAX_DIE+1;
}

void printPlayerStatus(int player) //player의 status 출력 함수  
{
	printf("%s : pos %i, coin %i, status %s\n", player_name[player],player_position[player],player_coin[player],player_statusString[player_status[player]]);				
}


void printPlayerPosition(int player) //player의 position 출력 함수  
{   
	int i;

	for (i=0;i<N_BOARD;i++)
	{
		printf("|");
		if(player_position[player]==i)
			printf("%c",player_name[player][0]);
		else
		{
			if(board_getBoardStatus(i) == BOARDSTATUS_NOK)
				printf("X");
			else
				printf(" ");
		}
	}
	printf("|\n");
}

void checkDie(void) //player 상태 확인  
{
    int i;
    for (i = 0; i < N_PLAYER; i++)
    {
        if (player_status[i] == PLAYERSTATUS_LIVE)
        {
            if (board_getBoardStatus(player_position[i]) == BOARDSTATUS_NOK) //if 조건 : player 위치 NOK
            {
                player_status[i] = PLAYERSTATUS_DIE;

                if (player_status[i] == PLAYERSTATUS_DIE) //if 조건 : player 상태 DIE 
                {
                    printf("%s in pos %i has died!! (coin %i)\n", player_name[i], player_position[i], player_coin[i]);
                }
            }
        }
    }
}
int main(int argc, char *argv[]) {
	int pos=0;
	int i;
	int turn=0;
	int flag_end = 0;
	
	srand((unsigned)time(NULL));
	//0.opening
	opening();

	//1. 초기화, 플레이어 이름 결정 
	board_initBoard();
	//1-2.
	for(i=0;i<N_PLAYER;i++)
	{
		player_position[i]=0;
		player_coin[i]=0;
		player_status[i]=PLAYERSTATUS_LIVE;
		//player_name
		printf("Player %i's name: ",i);
		scanf("%s",player_name[i]);
	}
	
	
	//2. 반복문 (플레이어 턴)
	do {
		int step;
		char c;
	
		if(player_status[turn] !=PLAYERSTATUS_LIVE){
			turn=(turn+1)%N_PLAYER;
			continue;
		}
		
		//2.1 보드& 플레이어의 상태 출력  
		board_printBoardStatus();
		
		
		printf("----------------Player status ---------------\n");	
		for (i=0;i<N_PLAYER;i++)
		{			
			printPlayerStatus(i);					
			printPlayerPosition(i);
		}	
		printf("---------------------------------------------\n");	


		//2.2 주사위 던지기
		printf("%s turn!! ",player_name[turn]);
		printf("Press any key to roll a die!\n");

		scanf("%d",&c);
	    fflush(stdin);
		
		step=rolldie();
			
		//2.3 이동
		player_position[turn] += step;
		if(player_position[turn]>=N_BOARD)
			player_position[turn] = N_BOARD-1;
			printf("Die result : %i, %s moved to %i!\n",step,player_name[turn],player_position[turn]);
			
		if (player_position[turn]==N_BOARD-1) // if조건 : player 위치가 보드 끝  
		{ 
			player_status[turn] = PLAYERSTATUS_END;
		
				if(player_status[turn] = PLAYERSTATUS_END) // if조건 : player 상태가 END  
				{
					printf("%s reached the end!! (coin : %i)\n",player_name[turn],player_coin[turn]);
				}
		}

		//2.4 동전 줍기
		int coinResult = board_getBoardCoin(player_position[turn]); // player_position이 코인이 배치된 보드일 때 동전 획득  
		player_coin[turn] += coinResult;
			if (coinResult>=1)
				printf("	-> Lucky! %s got %d coins\n", player_name[turn],coinResult);
		

		//2.5 다음턴 
		turn=(turn+1)%N_PLAYER;//wrap around	
		//2.6 if(조건: 모든 플레이어가 한번씩 턴을 돈다)
		if (turn==0)
		{
			int shark_pos = board_stepShark();
			printf("Shark moved to %i\n", shark_pos); //상어동작  
			checkDie();//player 상태 확인  
			
			game_end(); //정리(승자계산, 출력)  
		}
			
	} while(1);
	return 0;
}
