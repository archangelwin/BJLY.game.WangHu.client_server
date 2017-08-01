#ifndef CMD_OX_HEAD_FILE
#define CMD_OX_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��
#include "Game/Game/GameDefine.h"
#pragma pack(1)

#define GAME_PLAYER						4									//��Ϸ����
#define MAX_COUNT						5									//�����Ŀ

//����ԭ��
#define GER_NO_PLAYER					0x10								//û�����

//��Ϸ״̬
#define GS_TK_FREE						GAME_STATUS_FREE					//�ȴ���ʼ
#define GS_TK_CALL						GAME_STATUS_PLAY					//��ׯ״̬
#define GS_TK_SCORE						GAME_STATUS_PLAY+1					//��ע״̬
#define GS_TK_PLAYING					GAME_STATUS_PLAY+2					//��Ϸ����


#define SERVER_LEN						32 

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START				100									//��Ϸ��ʼ
#define SUB_S_ADD_SCORE					101									//��ע���
#define SUB_S_PLAYER_EXIT				102									//�û�ǿ��
#define SUB_S_SEND_CARD					103									//������Ϣ
#define SUB_S_GAME_END					104									//��Ϸ����
#define SUB_S_OPEN_CARD					105									//�û�̯��
#define SUB_S_CALL_BANKER				106									//�û���ׯ
#define SUB_S_ALL_CARD					107									//������Ϣ
#define SUB_S_AMDIN_COMMAND				108									//ϵͳ����


#ifndef _UNICODE
#define myprintf	_snprintf
#define mystrcpy	strcpy
#define mystrlen	strlen
#define myscanf		_snscanf
#define	myLPSTR		LPCSTR
#define myatoi      atoi
#define myatoi64    _atoi64
#else
#define myprintf	swprintf
#define mystrcpy	wcscpy
#define mystrlen	wcslen
#define myscanf		_snwscanf
#define	myLPSTR		LPWSTR
#define myatoi      _wtoi
#define myatoi64	_wtoi64
#endif

//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONGLONG							lCellScore;							//��������

	//��ʷ����
	LONGLONG							lTurnScore[GAME_PLAYER];			//������Ϣ
	LONGLONG							lCollectScore[GAME_PLAYER];			//������Ϣ
	TCHAR								szGameRoomName[SERVER_LEN];			//��������
};

//��Ϸ״̬
struct CMD_S_StatusCall
{
	WORD								wCallBanker;						//��ׯ�û�
	BYTE                                cbDynamicJoin;                      //��̬���� 
	BYTE                                cbPlayStatus[GAME_PLAYER];          //�û�״̬

	//��ʷ����
	LONGLONG							lTurnScore[GAME_PLAYER];			//������Ϣ
	LONGLONG							lCollectScore[GAME_PLAYER];			//������Ϣ
	TCHAR								szGameRoomName[SERVER_LEN];			//��������
};

//��Ϸ״̬
struct CMD_S_StatusScore
{
	//��ע��Ϣ
	BYTE                                cbPlayStatus[GAME_PLAYER];          //�û�״̬
	BYTE                                cbDynamicJoin;                      //��̬����
	LONGLONG							lTurnMaxScore;						//�����ע
	LONGLONG							lTableScore[GAME_PLAYER];			//��ע��Ŀ
	WORD								wBankerUser;						//ׯ���û�
	TCHAR								szGameRoomName[SERVER_LEN];			//��������

	//��ʷ����
	LONGLONG							lTurnScore[GAME_PLAYER];			//������Ϣ
	LONGLONG							lCollectScore[GAME_PLAYER];			//������Ϣ
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//״̬��Ϣ
	BYTE                                cbPlayStatus[GAME_PLAYER];          //�û�״̬
	BYTE                                cbDynamicJoin;                      //��̬����
	LONGLONG							lTurnMaxScore;						//�����ע
	LONGLONG							lTableScore[GAME_PLAYER];			//��ע��Ŀ
	WORD								wBankerUser;						//ׯ���û�

	//�˿���Ϣ
	BYTE								cbHandCardData[GAME_PLAYER][MAX_COUNT];//�����˿�
	BYTE								bOxCard[GAME_PLAYER];				//ţţ����

	//��ʷ����
	LONGLONG							lTurnScore[GAME_PLAYER];			//������Ϣ
	LONGLONG							lCollectScore[GAME_PLAYER];			//������Ϣ
	TCHAR								szGameRoomName[SERVER_LEN];			//��������
};

//�û���ׯ
struct CMD_S_CallBanker
{
	WORD								wCallBanker;						//��ׯ�û�
	bool								bFirstTimes;						//�״ν�ׯ
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	//��ע��Ϣ
	LONGLONG							lTurnMaxScore;						//�����ע
	WORD								wBankerUser;						//ׯ���û�
};

//�û���ע
struct CMD_S_AddScore
{
	WORD								wAddScoreUser;						//��ע�û�
	LONGLONG							lAddScoreCount;						//��ע��Ŀ
};

//��Ϸ����
struct CMD_S_GameEnd
{
	LONGLONG							lGameTax[GAME_PLAYER];				//��Ϸ˰��
	LONGLONG							lGameScore[GAME_PLAYER];			//��Ϸ�÷�
	BYTE								cbCardData[GAME_PLAYER];			//�û��˿�
};

//�������ݰ�
struct CMD_S_SendCard
{
	BYTE								cbCardData[GAME_PLAYER][MAX_COUNT];	//�û��˿�
};

//�������ݰ�
struct CMD_S_AllCard
{
	bool								bAICount[GAME_PLAYER];
	BYTE								cbCardData[GAME_PLAYER][MAX_COUNT];	//�û��˿�
};

//�û��˳�
struct CMD_S_PlayerExit
{
	WORD								wPlayerID;							//�˳��û�
};

//�û�̯��
struct CMD_S_Open_Card
{
	WORD								wPlayerID;							//̯���û�
	BYTE								bOpen;								//̯�Ʊ�־
};
//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ
#define SUB_C_CALL_BANKER				1									//�û���ׯ
#define SUB_C_ADD_SCORE					2									//�û���ע
#define SUB_C_OPEN_CARD					3									//�û�̯��
#define SUB_C_SPECIAL_CLIENT_REPORT     4                                   //�����ն�
#define SUB_C_AMDIN_COMMAND				5									//ϵͳ����

//�û���ׯ
struct CMD_C_CallBanker
{
	BYTE								bBanker;							//��ׯ��־
};

//�ն�����
struct CMD_C_SPECIAL_CLIENT_REPORT        
{
	WORD                                wUserChairID;                       //�û���λ
};

//�û���ע
struct CMD_C_AddScore
{
	LONGLONG							lScore;								//��ע��Ŀ
};

//�û�̯��
struct CMD_C_OxCard
{
	BYTE								bOX;								//ţţ��־
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//���ƶ���
#define SUB_C_AMDIN_COMMAND			5									//����Ա����

#define RQ_OPTION_CANCLE		1		//ȡ��
#define RQ_OPTION_QUERYING		2		//��ѯ
#define RQ_OPTION_SETING		3		//����

#define CHEAT_TYPE_LOST		0		//��
#define CHEAT_TYPE_WIN		1		//Ӯ

struct CMD_C_AdminReq
{
	BYTE cbReqType;							//��������
	BYTE cbCheatCount;						//���ƴ���
	BYTE cbCheatType;						//��������
	DWORD dwGameID;							//��ұ�ʶ
};

#pragma pack()
#endif
