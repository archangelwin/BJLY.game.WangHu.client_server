#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "CMD_Ox.h"
#include "Game/Game/GameDefine.h"
#include "Game/FV/FvSingleton.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//��ֵ����
#define	LOGIC_MASK_COLOR			0xF0								//��ɫ����
#define	LOGIC_MASK_VALUE			0x0F								//��ֵ����

//�˿�����
#define OX_VALUE0					0									//�������
#define OX_FOUR_SAME				103									//��������
#define OX_FIVEKING					104									//�廨ţ
#define OX_FIVESMALL				105									//��Сţ

//////////////////////////////////////////////////////////////////////////

//�����ṹ
struct tagAnalyseResult
{
	BYTE 							cbFourCount;						//������Ŀ
	BYTE 							cbThreeCount;						//������Ŀ
	BYTE 							cbDoubleCount;						//������Ŀ
	BYTE							cbSignedCount;						//������Ŀ
	BYTE 							cbFourLogicVolue[1];				//�����б�
	BYTE 							cbThreeLogicVolue[1];				//�����б�
	BYTE 							cbDoubleLogicVolue[2];				//�����б�
	BYTE 							cbSignedLogicVolue[5];				//�����б�
	BYTE							cbFourCardData[MAX_COUNT];			//�����б�
	BYTE							cbThreeCardData[MAX_COUNT];			//�����б�
	BYTE							cbDoubleCardData[MAX_COUNT];		//�����б�
	BYTE							cbSignedCardData[MAX_COUNT];		//������Ŀ
};


//��Ϸ�߼���

//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼���
class NNGameLogic
	: public FvSingleton<NNGameLogic>
{
	//��������
private:
	static BYTE						m_cbCardListData[52];				//�˿˶���

	//��������
public:
	//���캯��
	NNGameLogic();
	//��������
	virtual ~NNGameLogic();

	//���ͺ���
public:
	//��ȡ����
	BYTE GetCardType(BYTE cbCardData[], BYTE cbCardCount);
	//��ȡ��ֵ
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData&LOGIC_MASK_VALUE; }
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE cbCardData) { return cbCardData&LOGIC_MASK_COLOR; }
	//��ȡ����
	BYTE GetTimes(BYTE cbCardData[], BYTE cbCardCount);
	//��ȡţţ
	bool GetOxCard(BYTE cbCardData[], BYTE cbCardCount);
	//��ȡ����
	bool IsIntValue(BYTE cbCardData[], BYTE cbCardCount);

	//���ƺ���
public:
	//�����˿�
	void SortCardList(BYTE cbCardData[], BYTE cbCardCount);
	//�����˿�
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);

	//���ܺ���
public:
	//�߼���ֵ
	BYTE GetCardLogicValue(BYTE cbCardData);
	//�Ա��˿�
	bool CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount,BOOL FirstOX,BOOL NextOX);
};

//////////////////////////////////////////////////////////////////////////

#endif

