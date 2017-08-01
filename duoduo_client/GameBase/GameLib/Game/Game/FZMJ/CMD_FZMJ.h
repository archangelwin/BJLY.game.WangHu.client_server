#ifndef CMD_SPARROW_HEAD_FILE
#define CMD_SPARROW_HEAD_FILE

#include "Game/Game/GameDefine.h"
#pragma pack(1)

//�������
#define GAME_PLAYER					4									//��Ϸ����

#define GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_MATCH|GAME_GENRE_GOLD)	//��Ϸ����

//��Ϸ״̬
#define GS_MJ_FREE					GAME_STATUS_FREE					//����״̬
#define GS_MJ_PLAY					(GAME_STATUS_PLAY+1)				//��Ϸ״̬
#define GS_MJ_XIAOHU				(GAME_STATUS_PLAY+2)							//С��״̬

#define TIME_START_GAME				30									//��ʼ��ʱ��
#define TIME_OPERATE_CARD			15									//������ʱ��


#define GAME_TYPE_ZZ				0
#define GAME_TYPE_CS				1

#define GAME_TYPE_ZZ_ZIMOHU			1		//ֻ����ģ��
#define GAME_TYPE_ZZ_QIDUI			2		//�ɺ��߶�
#define GAME_TYPE_ZZ_QIANGGANGHU	3		//�����ܺ�
#define GAME_TYPE_ZZ_ZHANIAO2		4		//����2��
#define GAME_TYPE_ZZ_ZHANIAO4		5		//����4��
#define GAME_TYPE_ZZ_ZHANIAO6		6		//����6��
#define GAME_TYPE_ZZ_HONGZHONG		7		//�����淨
#define GAME_TYPE_CS_ZHUANGXIANFEN	8		//ׯ�з�
#define GAME_TYPE_ZZ_ZHANIAO3		9		//����3��
#define GAME_TYPE_ZZ_ZHANIAO5		10		//����5��
#define GAME_TYPE_ZZ_258			11		//258���� ��ɳ�齫
#define GAME_TYPE_ZZ_HONGZHONG_GZ	12		//������������2��

#define GAME_TYPE_ZZ_LIUJU_4CARD	13		//ʣ��4�Ż�ׯ

#define MAX_GAME_RULE				8		//������

//////////////////////////////////////////////////////////////////////////

//�������
struct CMD_WeaveItem
{
	DWORD							cbWeaveKind;						//�������
	BYTE							cbCenterCard;						//�����˿�
	BYTE							cbPublicCard;						//������־
	WORD							wProvideUser;						//��Ӧ�û�
};

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_OUT_CARD				101									//��������
#define SUB_S_SEND_CARD				102									//�����˿�
#define SUB_S_SEND_CARD_CSGANG		103									//�����˿�
#define SUB_S_OPERATE_NOTIFY		104									//������ʾ
#define SUB_S_OPERATE_RESULT		105									//��������
#define SUB_S_GAME_END				106									//��Ϸ����
#define SUB_S_TRUSTEE				107									//�û��й�
#define SUB_S_CHI_HU				108									//
#define SUB_S_GANG_SCORE			110									//
#define SUB_S_OUT_CARD_CSGANG		111									//
#define SUB_S_XIAO_HU				112									//С��

#define SUB_S_MASTER_HANDCARD		120									//
#define SUB_S_MASTER_LEFTCARD		121									//ʣ���ƶ�

#define ZI_PAI_COUNT	7												//����ȫ��

#define MASK_CHI_HU_RIGHT			0x0fffffff							//���ȨλDWORD����			

//��������
#define MAX_WEAVE					4									//������
#define MAX_INDEX					34									//�������
#define MAX_COUNT					14									//�����Ŀ
#define MAX_REPERTORY				136									//�����

#define MAX_NIAO_CARD				10									//���������
//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONGLONG						lCellScore;							//�������
	WORD							wBankerUser;						//ׯ���û�
	bool							bTrustee[GAME_PLAYER];				//�Ƿ��й�
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//��Ϸ����
	LONGLONG						lCellScore;									//��Ԫ����
	WORD							wBankerUser;								//ׯ���û�
	WORD							wCurrentUser;								//��ǰ�û�

	//״̬����
	BYTE							cbActionCard;								//�����˿�
	DWORD							cbActionMask;								//��������
	BYTE							cbLeftCardCount;							//ʣ����Ŀ
	bool							bTrustee[GAME_PLAYER];						//�Ƿ��й�
	WORD							wWinOrder[GAME_PLAYER];						//

	//������Ϣ
	WORD							wOutCardUser;								//�����û�
	BYTE							cbOutCardData;								//�����˿�
	BYTE							cbDiscardCount[GAME_PLAYER];				//������Ŀ
	BYTE							cbDiscardCard[GAME_PLAYER][60];				//������¼

	//�˿�����
	BYTE							cbCardCount;								//�˿���Ŀ
	BYTE							cbCardData[MAX_COUNT];						//�˿��б�
	BYTE							cbSendCardData;								//�����˿�

	//����˿�
	BYTE							cbWeaveCount[GAME_PLAYER];					//�����Ŀ
	CMD_WeaveItem					WeaveItemArray[GAME_PLAYER][MAX_WEAVE];		//����˿�

	bool                           bHasCSGang[GAME_PLAYER];
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	QYLONG						lSiceCount;									//���ӵ���
	WORD							wBankerUser;								//ׯ���û�
	WORD							wCurrentUser;								//��ǰ�û�
	DWORD							cbUserAction;								//�û�����
	BYTE							cbCardData[MAX_COUNT*GAME_PLAYER];			//�˿��б�
	BYTE							cbLeftCardCount;							//
	BYTE                         cbXiaoHuTag;                             //С����� 0 ûС�� 1 ��С����
};

//��������
struct CMD_S_OutCard
{
	WORD							wOutCardUser;						//�����û�
	BYTE							cbOutCardData;						//�����˿�
};

//��������
struct CMD_S_OutCard_CSGang
{
	WORD							wOutCardUser;						//�����û�
	BYTE							cbOutCardData1;						//�����˿�
	BYTE							cbOutCardData2;						//�����˿�
};
//�����˿�
struct CMD_S_SendCard
{
	BYTE							cbCardData;							//�˿�����
	DWORD							cbActionMask;						//��������
	WORD							wCurrentUser;						//��ǰ�û�
	bool							bTail;								//ĩβ����
};
//�����˿�
struct CMD_S_SendCard_CSGang
{
	BYTE							cbCardData1;						//�˿�����
	BYTE							cbCardData2;						//�˿�����
	DWORD							cbActionMask;						//��������
	WORD							wCurrentUser;						//��ǰ�û�
};

//������ʾ
struct CMD_S_OperateNotify
{
	WORD							wResumeUser;						//��ԭ�û�
	DWORD							cbActionMask;						//��������
	BYTE							cbActionCard;						//�����˿�
};

//��������
struct CMD_S_OperateResult
{
	WORD							wOperateUser;						//�����û�
	WORD							wProvideUser;						//��Ӧ�û�
	DWORD							cbOperateCode;						//��������
	BYTE							cbOperateCard;						//�����˿�
};

//��Ϸ����
struct CMD_S_GameEnd
{
	BYTE							cbCardCount[GAME_PLAYER];			//
	BYTE							cbCardData[GAME_PLAYER][MAX_COUNT];	//
	//������Ϣ
	WORD							wProvideUser[GAME_PLAYER];			//��Ӧ�û�
	dword							dwChiHuRight[GAME_PLAYER];			//��������
	dword							dwStartHuRight[GAME_PLAYER];			//���ֺ�������
	LONGLONG						lStartHuScore[GAME_PLAYER];			//���ֺ��Ʒ���

	//������Ϣ
	LONGLONG						lGameScore[GAME_PLAYER];			//��Ϸ����
	int								lGameTax[GAME_PLAYER];				//

	WORD							wWinOrder[GAME_PLAYER];				//��������

	LONGLONG						lGangScore[GAME_PLAYER];			//��ϸ�÷�
	BYTE							cbGenCount[GAME_PLAYER];			//
	WORD							wLostFanShu[GAME_PLAYER][GAME_PLAYER];
	WORD							wLeftUser;							//

	//����˿�
	BYTE							cbWeaveCount[GAME_PLAYER];					//�����Ŀ
	CMD_WeaveItem					WeaveItemArray[GAME_PLAYER][MAX_WEAVE];		//����˿�

	BYTE							cbCardDataNiao[MAX_NIAO_CARD];	// ����
	BYTE							cbNiaoCount;	//���Ƹ���
	BYTE							cbNiaoPick;	//�������
};

//�û��й�
struct CMD_S_Trustee
{
	bool							bTrustee;							//�Ƿ��й�
	WORD							wChairID;							//�й��û�
};

//
struct CMD_S_ChiHu
{
	WORD							wChiHuUser;							//
	WORD							wProviderUser;						//
	BYTE							cbChiHuCard;						//
	BYTE							cbCardCount;						//
	LONGLONG						lGameScore;							//
	BYTE							cbWinOrder;							//
};
struct CMD_S_XiaoHu
{
	WORD							wXiaoHuUser;							//
	DWORD                        dwXiaoCode;

	BYTE							cbCardCount;
	BYTE							cbCardData[MAX_COUNT];
};

//
struct CMD_S_GangScore
{
	WORD							wChairId;							//
	BYTE							cbXiaYu;							//
	LONGLONG						lGangScore[GAME_PLAYER];			//
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_OUT_CARD				1									//��������
#define SUB_C_OPERATE_CARD			3									//�����˿�
#define SUB_C_TRUSTEE				4									//�û��й�
#define SUB_C_XIAOHU				5									//С��
#define SUB_C_MASTER_LEFTCARD		6									//ʣ���ƶ�
#define SUB_C_MASTER_CHEAKCARD		7									//ѡ�����
#define SUB_C_MASTER_ZHANIAO		8									//����
#define SUB_C_OUT_CARD_CSGANG		9									//��ɳ�ܴ��

#define CARD_COLOR_NULL			0
#define CARD_COLOR_TONG			1
#define CARD_COLOR_WAN			2
#define CARD_COLOR_TIAO			3
//��������
struct CMD_C_OutCard
{
	BYTE							cbCardData;							//�˿�����
};
//��������
struct CMD_C_OutCard_CSGang
{
	BYTE							cbCardData1;							//�˿�����
	BYTE							cbCardData2;							//�˿�����
};
//��������
struct CMD_C_OperateCard
{
	DWORD							cbOperateCode;						//��������
	BYTE							cbOperateCard;						//�����˿�
};

//�û��й�
struct CMD_C_Trustee
{
	bool							bTrustee;							//�Ƿ��й�	
};

//����С��
struct CMD_C_XiaoHu
{
	DWORD						cbOperateCode;						//��������
	BYTE							cbOperateCard;						//�����˿�
};


//////////////////////////////////////////////////////////////////////////

struct  FZMJGameRecordPlayer
{
	DWORD dwUserID;
	std::string kHead;
	std::string kNickName;
	std::vector<BYTE> cbCardData;
	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(dwUserID);
		Stream_VALUE(kHead);
		Stream_VALUE(kNickName);
		Stream_VALUE(cbCardData);
	}
};

struct  FZMJGameRecordOperateResult
{
	enum Type
	{
		TYPE_NULL,
		TYPE_OperateResult,
		TYPE_SendCard,
		TYPE_OutCard,
		TYPE_ChiHu,
	};
	FZMJGameRecordOperateResult()
	{
		cbActionType = 0;
		wOperateUser = 0;
		wProvideUser = 0;
		cbOperateCode = 0;
		cbOperateCard = 0;
	}
	BYTE							cbActionType;
	WORD							wOperateUser;						//�����û�
	WORD							wProvideUser;						//��Ӧ�û�
	DWORD							cbOperateCode;						//��������
	BYTE							cbOperateCard;						//�����˿�
	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(cbActionType);
		Stream_VALUE(wOperateUser);
		Stream_VALUE(wProvideUser);
		Stream_VALUE(cbOperateCode);
		Stream_VALUE(cbOperateCard);
	}
};
struct  FZMJGameRecord
{
	DWORD dwKindID;
	DWORD dwVersion;
	std::vector<FZMJGameRecordPlayer> kPlayers;
	std::vector<FZMJGameRecordOperateResult> kAction;
	void StreamValue(datastream& kData,bool bSend)
	{
		StructVecotrMember(FZMJGameRecordPlayer,kPlayers);
		StructVecotrMember(FZMJGameRecordOperateResult,kAction);
		Stream_VALUE(dwKindID);
		Stream_VALUE(dwVersion);
	}

};

struct MasterHandCardInfo
{
	int nChairId;
	std::vector<BYTE>    kMasterHandCard;

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(nChairId);
		Stream_VALUE(kMasterHandCard);
	}
};

struct MasterHandCard
{
	std::vector<MasterHandCardInfo>    kMasterHandCardList;
	void StreamValue(datastream& kData,bool bSend)
	{
		StructVecotrMember(MasterHandCardInfo,kMasterHandCardList);
	}
};
struct MaterCheckCard
{
	BYTE							cbCheakCard;						//��������
};

struct MaterNiaoCout
{
	BYTE							cbNiaoCout;							//��������
};

struct MasterLeftCard
{
	BYTE      kMasterLeftIndex[MAX_INDEX];
	BYTE      kMasterCheakCard;
};

#pragma pack()
//////////////////////////////////////////////////////////////////////////

#endif
