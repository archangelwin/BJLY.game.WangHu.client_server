#pragma once

#include "Game/Game/GameDefine.h"

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define GAME_PLAYER					MAX_CHAIR							//��Ϸ����


//״̬����
#define GAME_SCENE_FREE				GAME_STATUS_FREE					//�ȴ���ʼ
#define GAME_SCENE_PLAY				GAME_STATUS_PLAY					//��Ϸ����
#define GAME_SCENE_BET				GAME_STATUS_PLAY					//��ע״̬
#define	GAME_SCENE_END				GAME_STATUS_PLAY+1					//����״̬
#define GAME_SCENE_OPEN				GAME_STATUS_PLAY+2				//����״̬


//�������
#define AREA_XIAN					0									//�м�����
#define AREA_PING					1									//ƽ������
#define AREA_ZHUANG					2									//ׯ������
#define AREA_XIAN_TIAN				3									//������
#define AREA_ZHUANG_TIAN			4									//ׯ����
#define AREA_TONG_DUI				5									//ͬ��ƽ
#define AREA_XIAN_DUI				6									//�ж���
#define AREA_ZHUANG_DUI				7									//ׯ����
#define AREA_MAX					8									//�������

//������multiple
#define MULTIPLE_XIAN				2									//�мұ���
#define MULTIPLE_PING				9									//ƽ�ұ���
#define MULTIPLE_ZHUANG				2									//ׯ�ұ���
#define MULTIPLE_XIAN_TIAN			3									//����������
#define MULTIPLE_ZHUANG_TIAN		3									//ׯ��������
#define MULTIPLE_TONG_DIAN			33									//ͬ��ƽ����
#define MULTIPLE_XIAN_PING			12									//�ж��ӱ���
#define MULTIPLE_ZHUANG_PING		12									//ׯ���ӱ���


//���ʶ���
#define RATE_TWO_PAIR				12									//��������
#define SERVER_LEN					32									//���䳤��

#define IDM_UPDATE_STORAGE			WM_USER+1001

//��¼��Ϣ
struct tagServerGameRecord
{
	BYTE							cbKingWinner;						//����Ӯ��
	bool							bPlayerTwoPair;						//���ӱ�ʶ
	bool							bBankerTwoPair;						//���ӱ�ʶ
	BYTE							cbPlayerCount;						//�мҵ���
	BYTE							cbBankerCount;						//ׯ�ҵ���
};

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_FREE				99									//��Ϸ����
#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_PLACE_JETTON			101									//�û���ע
#define SUB_S_GAME_END				102									//��Ϸ����
#define SUB_S_APPLY_BANKER			103									//����ׯ��
#define SUB_S_CHANGE_BANKER			104									//�л�ׯ��
#define SUB_S_CHANGE_USER_SCORE		105									//���»���
#define SUB_S_SEND_RECORD			106									//��Ϸ��¼
#define SUB_S_PLACE_JETTON_FAIL		107									//��עʧ��
#define SUB_S_CANCEL_BANKER			108									//ȡ������
#define SUB_S_AMDIN_COMMAND			109									//����Ա����
#define SUB_S_UPDATE_STORAGE        110									//���¿��

#define SUB_S_GAME_OPENCARD        111									//����
#define SUB_S_TUCH_TURNCARD        120									//����

#define SUB_S_LEFT_COUT				121//���´洢

#define SUB_S_BANKER_LIST			130									//ׯ�� �б�

#define SUB_S_CANCEL_BET            140									//ȡ����ע


//����ظ�
struct CMD_S_CommandResult
{
	BYTE cbAckType;					//�ظ�����
#define ACK_SET_WIN_AREA  1
#define ACK_PRINT_SYN     2
#define ACK_RESET_CONTROL 3
	BYTE cbResult;
#define CR_ACCEPT  2			//����
#define CR_REFUSAL 3			//�ܾ�
	BYTE cbExtendData[20];			//��������
};


//���¿��
struct CMD_S_UpdateStorage
{
	LONGLONG						lStorage;							//�¿��ֵ
	LONGLONG						lStorageDeduct;						//���˥��
};

//ʧ�ܽṹ
struct CMD_S_PlaceBetFail
{
	word							wPlaceUser;							//��ע���
	BYTE							lBetArea;							//��ע����
	LONGLONG						lPlaceScore;						//��ǰ��ע
};

//����ׯ��
struct CMD_S_ApplyBanker
{
	word							wApplyUser;							//�������
};

//ȡ������
struct CMD_S_CancelBanker
{
	word							wCancelUser;						//ȡ�����
};

//�л�ׯ��
struct CMD_S_ChangeBanker
{
	word							wBankerUser;						//��ׯ���
	LONGLONG						lBankerScore;						//ׯ�ҷ���
};

//��Ϸ״̬
struct CMD_S_StatusFree
{
	//ȫ����Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	//�����Ϣ
	LONGLONG						lPlayFreeSocre;						//������ɽ��

	//ׯ����Ϣ
	word							wBankerUser;						//��ǰׯ��
	LONGLONG						lBankerScore;						//ׯ�ҷ���
	LONGLONG						lBankerWinScore;					//ׯ��Ӯ��
	word							wBankerTime;						//ׯ�Ҿ���

	//�Ƿ�ϵͳ��ׯ
	bool							bEnableSysBanker;					//ϵͳ��ׯ

	dword							cbCardLeft;

	//������Ϣ
	LONGLONG						lApplyBankerCondition;				//��������
	LONGLONG						lAreaLimitScore;					//��������

	//������Ϣ
	TCHAR							szGameRoomName[SERVER_LEN];			//��������
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//ȫ����Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��
	BYTE							cbGameStatus;						//��Ϸ״̬

	//��ע��
	LONGLONG						lAllBet[AREA_MAX];					//����ע
	LONGLONG						lPlayBet[AREA_MAX];					//�����ע
	WORD							wXianChairID;						//�������ע
	WORD							wZhuangChairID;						//���ׯ��ע
	LONGLONG						wXianBet;						//�������ע
	LONGLONG						wZhuangBet;						//���ׯ��ע

	//��һ���
	LONGLONG						lPlayBetScore;						//��������ע	
	LONGLONG						lPlayFreeSocre;						//������ɽ��

	//�����Ӯ
	LONGLONG						lPlayScore[AREA_MAX];				//�����Ӯ
	LONGLONG						lPlayAllScore;						//��ҳɼ�

	//ׯ����Ϣ
	word							wBankerUser;						//��ǰׯ��
	LONGLONG						lBankerScore;						//ׯ�ҷ���
	LONGLONG						lBankerWinScore;					//ׯ��Ӯ��
	word							wBankerTime;						//ׯ�Ҿ���

	//�Ƿ�ϵͳ��ׯ
	bool							bEnableSysBanker;					//ϵͳ��ׯ

	//������Ϣ
	LONGLONG						lApplyBankerCondition;				//��������
	LONGLONG						lAreaLimitScore;					//��������

	//�˿���Ϣ
	BYTE							cbCardCount[2];						//�˿���Ŀ
	BYTE							cbTableCardArray[2][3];				//�����˿�

	dword							cbCardLeft;

	//������Ϣ
	TCHAR							szGameRoomName[SERVER_LEN];			//��������
	//������Ϣ
	BYTE							nOpenCardIndex;  //��ǰ���ƽ׶�
	BYTE							nOpenChairID;
};

//��Ϸ����
struct CMD_S_GameFree
{
	BYTE							cbTimeLeave;						//ʣ��ʱ��
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	word							wBankerUser;						//ׯ��λ��
	LONGLONG						lBankerScore;						//ׯ�ҽ��

	LONGLONG						lPlayBetScore;						//��������ע	
	LONGLONG						lPlayFreeSocre;						//������ɽ��

	int								nChipRobotCount;					//�������� (��ע������)
	int64							nListUserCount;						//�б�����
	int								nAndriodCount;						//�������б�����
};

//�û���ע
struct CMD_S_PlaceBet
{
	word							wChairID;							//�û�λ��
	BYTE							cbBetArea;							//��������
	LONGLONG						lBetScore;							//��ע��Ŀ
	BYTE							cbAndroidUser;						//������ʶ
	BYTE							cbAndroidUserT;						//������ʶ

	WORD							wXianChairID;						//�������ע
	WORD							wZhuangChairID;						//���ׯ��ע
	LONGLONG							wXianBet;						//�������ע
	LONGLONG							wZhuangBet;						//���ׯ��ע
};

//��Ϸ����
struct CMD_S_GameEnd
{
	//�¾���Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	// 	//�˿���Ϣ
	// 	BYTE							cbCardCount[2];						//�˿���Ŀ
	// 	BYTE							cbTableCardArray[2][3];				//�����˿�

	//ׯ����Ϣ
	LONGLONG						lBankerScore;						//ׯ�ҳɼ�
	LONGLONG						lBankerTotallScore;					//ׯ�ҳɼ�
	INT								nBankerTime;						//��ׯ����

	//��ҳɼ�
	LONGLONG						lPlayScore[AREA_MAX];				//��ҳɼ�
	LONGLONG						lPlayAllScore;						//��ҳɼ�


	LONGLONG						IUserWinScore[GAME_PLAYER];			//������ҳɼ�

	//ȫ����Ϣ
	LONGLONG						lRevenue;							//��Ϸ˰��
};
#define  OPEN_INDEX_NULL      -1
#define  OPEN_XIAN_12CARD		 0
#define  OPEN_XIAN_3CARD		 1
#define  OPEN_ZHUANG_12CARD    2
#define  OPEN_ZHUANG_3CARD     3

struct CMD_S_GameOpenCard
{
	BYTE			cbTimeLeave;
	BYTE			cbActTime;
	BYTE			nOpenIndex;    //��������
	BYTE			nChairID;

	//�˿���Ϣ
	BYTE							cbCardCount[2];						//�˿���Ŀ
	BYTE							cbTableCardArray[2][3];				//�����˿�
};

//����
struct CMD_S_TuchTurnCard
{
	word							wChairID;							//�û�λ��
	int nIndex;
	int nTuchType;
	float touchX;
	float touchY;
};

struct CMD_S_BankerList
{
	int  kBankerList[GAME_PLAYER];   //ׯ���б�
};

struct CMD_S_CancleBet
{
	WORD wChairID;
	LONGLONG						lAllBet[AREA_MAX];
	WORD							wXianChairID;						//�������ע
	WORD							wZhuangChairID;						//���ׯ��ע
	LONGLONG							wXianBet;						//�������ע
	LONGLONG							wZhuangBet;						//���ׯ��ע
};
//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_PLACE_JETTON			1									//�û���ע
#define SUB_C_APPLY_BANKER			2									//����ׯ��
#define SUB_C_CANCEL_BANKER			3									//ȡ������
#define SUB_C_AMDIN_COMMAND			4									//����Ա����
#define SUB_C_UPDATE_STORAGE        5									//���¿��
#define SUB_C_TOUCH_TURNCARD        6                                  //����
#define SUB_C_TURNCARD_END         7                                   //���ƽ���
#define SUB_C_BANKER_LIST          8                                   //��ȡׯ���б�
#define SUB_C_CANCLE_BET           9                                   //ȡ����ע

struct CMD_C_AdminReq
{
	BYTE cbReqType;
#define RQ_SET_WIN_AREA	1
#define RQ_RESET_CONTROL	2
#define RQ_PRINT_SYN		3
	BYTE cbExtendData[20];			//��������
};


//struct CMD_C_AdminReq
//{
//	BYTE cbReqType;
//		 #define RQ_SETWINAREA   1 
//	BYTE cbExtendData[120];
//};


//�û���ע
struct CMD_C_PlaceBet
{

	BYTE							cbBetArea;						//��������
	LONGLONG						lBetScore;						//��ע��Ŀ
};

#define RQ_REFRESH_STORAGE		1
#define RQ_SET_STORAGE			2

//���¿��
struct CMD_C_UpdateStorage
{
	BYTE                            cbReqType;						//��������
	LONGLONG						lStorage;						//�¿��ֵ
	LONGLONG						lStorageDeduct;					//���˥��
};

//����
struct CMD_C_TuchTurnCard
{
	int nIndex;
	int nTuchType;
	float touchX;
	float touchY;
};

//�û���ע
struct CMD_C_TurnCardEnd
{
	int nIndex;
	BYTE cbChairID;
};
#pragma pack()
//////////////////////////////////////////////////////////////////////////
