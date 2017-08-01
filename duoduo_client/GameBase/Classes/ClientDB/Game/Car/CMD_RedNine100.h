#ifndef CMD_BACCARAT_HEAD_FILE
#define CMD_BACCARAT_HEAD_FILE

#include "Game/Game/GameDefine.h"
#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID						108									//��Ϸ I D
#define GAME_PLAYER					100									//��Ϸ����
#define GAME_NAME					TEXT("������")						//��Ϸ����

//�汾��Ϣ
#define VERSION_SERVER			    PROCESS_VERSION(6,0,3)				//����汾
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//����汾

//״̬����
#define	GS_PLACE_JETTON				GAME_STATUS_PLAY						//��ע״̬
#define	GS_GAME_END					GAME_STATUS_PLAY+1						//����״̬
#define	GS_MOVECARD_END				GAME_STATUS_PLAY+2						//����״̬


#define GAME_SCENE_FREE				GAME_STATUS_FREE					//�ȴ���ʼ
#define GAME_SCENE_PLACE_JETTON		GAME_STATUS_PLAY					//��ע״̬
#define GAME_SCENE_GAME_END			GAME_STATUS_PLAY+1					//����״̬
#define GAME_SCENE_MOVECARD_END		GAME_STATUS_PLAY+2					//����״̬

//��������
#define ID_TIAN_MEN					1									//˳��
#define ID_DI_MEN					2									//��߽�
#define ID_XUAN_MEN					3									//��
#define ID_HUANG_MEN				4									//����

//�������
#define BANKER_INDEX				0									//ׯ������
#define SHUN_MEN_INDEX				1									//˳������
#define DUI_MEN_INDEX				2									//��������
#define DAO_MEN_INDEX				3									//��������
#define HUAN_MEN_INDEX				4									//��������

#define AREA_COUNT					8									//������Ŀ

//���ʶ���
#define RATE_TWO_PAIR				12									//��������

//��������Ϣ
struct tagRobotInfo
{
	int nChip[6];														//���붨��
	int nAreaChance[AREA_COUNT];										//������
	TCHAR szCfgFileName[MAX_PATH];										//�����ļ�
	int	nMaxTime;														//�������

	tagRobotInfo()
	{
		int nTmpChip[6] = {100,1000,10000,100000,1000000,5000000};
		int nTmpAreaChance[AREA_COUNT] = {1, 2, 2, 5, 10, 10, 10, 10};
		TCHAR szTmpCfgFileName[MAX_PATH] = ("BumperCarBattle.ini");

		nMaxTime = 10;
		memcpy(nChip, nTmpChip, sizeof(nChip));
		memcpy(nAreaChance, nTmpAreaChance, sizeof(nAreaChance));
		memcpy(szCfgFileName, szTmpCfgFileName, sizeof(szCfgFileName));
	}
};

//��¼��Ϣ
struct tagServerGameRecord
{
	BYTE							bWinMen[AREA_COUNT+1];						//˳��ʤ��
};


//���¿��
struct CMD_S_UpdateStorage
{
	LONGLONG						lStorage;							//�¿��ֵ
	LONGLONG						lStorageDeduct;						//���˥��
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
#define SUB_S_CHECK_IMAGE			109									//ȡ������
#define SUB_S_ADMIN_COMMDN			110									//ϵͳ����
#define SUB_S_UPDATE_STORAGE        111//���´洢


//ʧ�ܽṹ
struct CMD_S_PlaceJettonFail
{
	word							wPlaceUser;							//��ע���
	BYTE							lJettonArea;						//��ע����
	LONGLONG						lPlaceScore;						//��ǰ��ע
};

//���»���
struct CMD_S_ChangeUserScore
{
	word							wChairID;							//���Ӻ���
	DOUBLE							lScore;								//��һ���

	//ׯ����Ϣ
	word							wCurrentBankerChairID;				//��ǰׯ��
	BYTE							cbBankerTime;						//ׯ�Ҿ���
	DOUBLE							lCurrentBankerScore;				//ׯ�ҷ���
};

//����ׯ��
struct CMD_S_ApplyBanker
{
	word							wApplyUser;							//�������
};

//ȡ������
struct CMD_S_CancelBanker
{
	TCHAR							szCancelUser[32];					//ȡ�����
};

//�л�ׯ��
struct CMD_S_ChangeBanker
{
	word							wBankerUser;						//��ׯ���
	LONGLONG						lBankerScore;						//ׯ�ҽ��
};

//��Ϸ״̬
struct CMD_S_StatusFree
{
	//ȫ����Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	//�����Ϣ
	LONGLONG						lUserMaxScore;							//��ҽ��

	//ׯ����Ϣ
	word							wBankerUser;						//��ǰׯ��
	word							cbBankerTime;						//ׯ�Ҿ���
	LONGLONG						lBankerWinScore;					//ׯ�ҳɼ�
	LONGLONG						lBankerScore;						//ׯ�ҷ���
	bool							bEnableSysBanker;					//ϵͳ��ׯ

	//������Ϣ
	LONGLONG						lApplyBankerCondition;				//��������
	LONGLONG						lAreaLimitScore;					//��������
	int								CheckImage;

	TCHAR							szGameRoomName[32];			//�������� 
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//ȫ����ע
	LONGLONG						lAllJettonScore[AREA_COUNT+1];		//ȫ����ע

	//�����ע
	LONGLONG						lUserJettonScore[AREA_COUNT+1];		//������ע

	//��һ���
	LONGLONG						lUserMaxScore;						//�����ע							

	//������Ϣ
	LONGLONG						lApplyBankerCondition;				//��������
	LONGLONG						lAreaLimitScore;					//��������

	//�˿���Ϣ
	BYTE							cbTableCardArray[1][1];				//�����˿�

	//ׯ����Ϣ
	word							wBankerUser;						//��ǰׯ��
	word							cbBankerTime;						//ׯ�Ҿ���
	LONGLONG						lBankerWinScore;					//ׯ��Ӯ��
	LONGLONG						lBankerScore;						//ׯ�ҷ���
	bool							bEnableSysBanker;					//ϵͳ��ׯ

	//������Ϣ
	LONGLONG						lEndBankerScore;					//ׯ�ҳɼ�
	LONGLONG						lEndUserScore;						//��ҳɼ�
	LONGLONG						lEndUserReturnScore;				//���ػ���
	LONGLONG						lEndRevenue;						//��Ϸ˰��

	//ȫ����Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��
	BYTE							cbGameStatus;						//��Ϸ״̬
	int								CheckImage;
	TCHAR							szGameRoomName[32];			//�������� 
};

//��Ϸ����
struct CMD_S_GameFree
{
	BYTE							cbTimeLeave;						//ʣ��ʱ��
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	word							wBankerUser;						//ׯ��λ��
	LONGLONG						lBankerScore;						//ׯ�ҽ��
	LONGLONG						lUserMaxScore;						//�ҵĽ��
	BYTE							cbTimeLeave;						//ʣ��ʱ��	
	bool							bContiueCard;						//��������
	int								nChipRobotCount;					//�������� (��ע������)
};

//�û���ע
struct CMD_S_PlaceJetton
{
	word							wChairID;							//�û�λ��
	BYTE							cbJettonArea;						//��������
	LONGLONG						lJettonScore;						//��ע��Ŀ
	BYTE							cbAndroid;							//������
};

//��Ϸ����
struct CMD_S_GameEnd
{
	//�¾���Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	//�˿���Ϣ
	BYTE							cbTableCardArray[1][1];				//�����˿�
	BYTE							cbLeftCardCount;					//�˿���Ŀ

	BYTE							bcFirstCard;
 
	//ׯ����Ϣ
	LONGLONG						lBankerScore;						//ׯ�ҳɼ�
	LONGLONG						lBankerTotallScore;					//ׯ�ҳɼ�
	INT								nBankerTime;						//��ׯ����

	//��ҳɼ�
	LONGLONG						lUserScore;							//��ҳɼ�
	LONGLONG						lUserReturnScore;					//���ػ���

	//ȫ����Ϣ
	LONGLONG						lRevenue;							//��Ϸ˰��
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_PLACE_JETTON			1									//�û���ע
#define SUB_C_APPLY_BANKER			2									//����ׯ��
#define SUB_C_CANCEL_BANKER			3									//ȡ������
#define SUB_C_CONTINUE_CARD			4									//��������
#define SUB_C_CHECK_IMAGE			5									//��������
#define SUB_C_ADMIN_COMMDN			6									//ϵͳ����
#define SUB_C_GET_ACCOUNT			7									//��ȡ�ʺ�
#define SUB_C_CHECK_ACCOUNT			8									//��ȡ�ʺ�
#define SUB_S_SCORE_RESULT			9									//���ֽ��
#define SUB_S_ACCOUNT_RESULT		10									//�ʺŽ��
#define SUB_C_UPDATE_STORAGE                11//���¿��


//�û���ע
struct CMD_C_PlaceJetton
{
	BYTE							cbJettonArea;						//��������
	LONGLONG						lJettonScore;						//��ע��Ŀ
};

struct CMD_C_CheckImage
{
	int Index;
};

//////////////////////////////////////////////////////////////////////////

#define IDM_ADMIN_COMMDN	WM_USER+1000
#define IDM_GET_ACCOUNT		WM_USER+1001
#define IDM_CHEAK_ACCOUNT	WM_USER+1002
#define IDM_UPDATE_STORAGE			WM_USER+1003

//����������Ϣ
struct tagControlInfo
{
	BYTE cbControlArea;						//��������
};

//���������Ʒ���
#define	 S_CR_FAILURE				0		//ʧ��
#define  S_CR_UPDATE_SUCCES			1		//���³ɹ�
#define	 S_CR_SET_SUCCESS			2		//���óɹ�
#define  S_CR_CANCEL_SUCCESS		3		//ȡ���ɹ�
struct CMD_S_ControlReturns
{
	BYTE cbReturnsType;				//�ظ�����
	BYTE cbControlArea;				//��������
	BYTE cbControlTimes;			//���ƴ���
};


//�ͻ��˿�������
#define  C_CA_UPDATE				1		//����
#define	 C_CA_SET					2		//����
#define  C_CA_CANCELS				3		//ȡ��
struct CMD_C_ControlApplication
{
	BYTE cbControlAppType;			//��������
	BYTE cbControlArea;				//��������
	BYTE cbControlTimes;			//���ƴ���
};

struct CMD_C_CheakAccount
{
	TCHAR szUserAccount[32];
};

struct CMD_S_ScoreResult
{
	LONGLONG lUserJettonScore[AREA_COUNT+1];	//������ע
};

struct CMD_S_AccountResult
{
	TCHAR szAccount[100][32];					//�ʺ��ǳ�
};

//////////////////////////////////////////////////////////////////////////


#define RQ_REFRESH_STORAGE		1
#define RQ_SET_STORAGE			2

//���¿��
struct CMD_C_UpdateStorage
{
	BYTE                            cbReqType;						//��������
	LONGLONG						lStorage;						//�¿��ֵ
	LONGLONG						lStorageDeduct;					//���˥��
};
#pragma pack()
#endif
