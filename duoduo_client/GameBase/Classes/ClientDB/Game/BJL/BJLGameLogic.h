
#pragma once

#include "Game/GameLib.h"
#include "Game/Game/GameDefine.h"

//��Ϸ�߼�
class BJLGameLogic:public FvSingleton<BJLGameLogic>
{
public:

	//��ֵ����
	const static int	LOGIC_MASK_COLOR			=0xF0;							//��ɫ����
	const static int	LOGIC_MASK_VALUE			=0x0F;							//��ֵ����
	const static int	LOGIC_GAME_PLAYER				=100;							
	const static int	LOGIC_AREA_MAX					=8;								
	const static int	LOGIC_AREA_XIAN				=0;								
	const static int	LOGIC_AREA_ZHUANG				=2;					    
	//��������
private:
	static const BYTE				m_cbCardListData[52*8];				//�˿˶���

	//��������
public:
	//���캯��
	BJLGameLogic();
	//��������
	virtual ~BJLGameLogic();

	//���ͺ���
public:
	//��ȡ��ֵ
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData&LOGIC_MASK_VALUE; }
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE cbCardData) { return (cbCardData&LOGIC_MASK_COLOR)/LOGIC_MASK_VALUE; }

	//���ƺ���
public:
	//�����˿�
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);

	//�߼�����
public:
	//��ȡ�Ƶ�
	BYTE GetCardPip(BYTE cbCardData);
	//��ȡ�Ƶ�
	BYTE GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount);

	BYTE GetAreaBetMax(int nArea,LONGLONG AllPlayBet[LOGIC_GAME_PLAYER][LOGIC_AREA_MAX],SCORE& nMaxBetNum);


};

