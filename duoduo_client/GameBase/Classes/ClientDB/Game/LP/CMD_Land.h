#pragma once

#include "Game/Game/GameDefine.h"
//////////////////////////////////////////////////////////////////////////

#pragma pack(1)

//�����궨��

#define KIND_ID						404									//��Ϸ I D
#define GAME_PLAYER					100									//��Ϸ����
#define GAME_NAME					TEXT("����")						//��Ϸ����
//�������
#define VERSION_SERVER				PROCESS_VERSION(6,0,3)				//����汾
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//����汾

#define	HISTORY_NUMBER				13									//��ʷ��¼����
#define	MAX_BET_POS					256									//�����עλ��

//��Ϸ״̬
#define GAME_SCENE_BET					GAME_STATUS_PLAY+1							//��ע״̬
#define GAME_SCENE_PLAY				GAME_STATUS_PLAY+2							//��Ϸ״̬

#define LP_MAX_NUMBER         38
//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_GAME_END				101									//��Ϸ����
#define SUB_S_USER_BET				102									//�û���ע
#define SUB_S_USER_CANCLE_BET		103									//�û�ȡ����ע

#define LP_BET_TIME        30  //��עʱ��
#define LP_END_TIME        30	 //����ʱ��

#define LP_USER_CANCEL_ONCE	0 
#define LP_USER_CANCEL_ALL		1
#define LP_USER_CANCEL_LAST	2


//��ע״̬
struct CMD_S_StatusBet
{
	BYTE							cbTimeLeave;	//ʣ��ʱ��
};
//����״̬
struct CMD_S_StatusPlay
{
	BYTE							cbTimeLeave;	//ʣ��ʱ��
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	BYTE							cbTimeLeave;	//ʣ��ʱ��
};

//��Ϸ����
struct LPCMD_S_GameEnd
{
	BYTE							cbTimeLeave;						//ʣ��ʱ��
	BYTE							bWinNumber;						//ʤ������
	QYLONG							lWinScore[GAME_PLAYER];			//ʤ������
};
//�û���ע
struct CMD_S_UserBet
{
	WORD			  wChairID;										//�û�λ��
	BYTE          cbBetArea;										//��ע����
	QYLONG          lBetScore;										//��ע����
	LONGLONG       lTaltalScore;									//ʣ�����
	float         kPosX;											//����λ��
	float         kPosY;											//����λ��
};

struct CMD_S_UserCancleBet
{
	WORD			  wChairID;										//�û�λ��
	int			  nType;
	LONGLONG       lBetScore;
};
//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_USER_BET					10									//�û��з�
#define SUB_C_USER_CANCLE_BET			11									//�û�ȡ����ע


//�û���ע
struct CMD_C_UserBet
{
	BYTE          cbBetArea;										//��ע����
	QYLONG        lBetScore;										//��ע����
	float         kPosX;											//����λ��
	float         kPosY;											//����λ��
};

struct CMD_C_UserCancleBet
{
	int nType;
};
#pragma pack()