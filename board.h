#define BOARDSTATUS_OK		1
#define BOARDSTATUS_NOK		0

int board_initBoard(void);  //���� �ʱ�ȭ
int board_printBoardStatus(void);  //��ü ������ ���� ���=����� ��ġ ���  

int board_getBoardStatus(int pos);  //Ư�� ĭ�� �ļ� ���� ���
int board_getBoardCoin(int pos);  //���� ���� ���

int board_stepShark(void);  //��� ���� ���  
