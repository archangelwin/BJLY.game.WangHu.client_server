#include "XZDDGameScence.h"
#include "Game/GameLib.h"

#include "CMD_XZDD.h"
#include "XZDDPlayer.h"
#include "XZDDGameLogic.h"

bool XZDDGameScence::OnEventSceneMessage(byte cbGameStatus, bool bLookonUser, void* data, int dataSize)
{
	if (cbGameStatus == GS_MJ_FREE)
	{
		OnFreeScence(data,dataSize);
	}
	if(cbGameStatus == GS_MJ_HUANPAI)
	{
		OnHuanPaiScence(data,dataSize);
	}
	if (cbGameStatus == GS_MJ_XUANQUE)
	{
		OnXuanQueScence(data,dataSize);
	}
	if (cbGameStatus == GS_MJ_PLAY)
	{
		OnPlayScence(data,dataSize);
	}
	return true;
}
void XZDDGameScence::OnFreeScence(void* data, int wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
	if (wDataSize!=sizeof(CMD_S_StatusFree)) return;

	CMD_S_StatusFree* pNetInfo = (CMD_S_StatusFree*)data;
	WidgetFun::setVisible(this,"FreeStateNode",m_pLocal->GetUserStatus() < US_READY);

	m_nGameState = XZDD_STATE_NULL;

}
void XZDDGameScence::OnEventUserStatus(GamePlayer * pPlayer)
{
	if (m_pLocal->GetUserStatus() >= US_READY )
	{
		WidgetFun::setVisible(this,"PrivateEndPlane",false);
	}
	WidgetFun::setVisible(this,"FreeStateNode",m_pLocal->GetUserStatus() < US_READY);
}
void XZDDGameScence::OnPlayScence(void* data, int wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
	if (wDataSize!=sizeof(CMD_S_StatusPlay)) return;

	CMD_S_StatusPlay* pNetInfo = (CMD_S_StatusPlay*)data;

	defaultState();

	m_iBankerUser=pNetInfo->wBankerUser;
	m_iCurrentUser = pNetInfo->wCurrentUser;


	for (int i = 0;i<MAX_PLAYER;i++)
	{
		XZDDPlayer* pPlayer = getPlayerByChairID(i);
		if (!pPlayer)
		{
			continue;
		}
		pPlayer->setWeaveItem(pNetInfo->WeaveItemArray[i],pNetInfo->cbWeaveCount[i]);
		pPlayer->setHandCard(pNetInfo->cbCardData,13 - pNetInfo->cbWeaveCount[i]*3);
		pPlayer->setQueColor(pNetInfo->nQueColor[i],true,true);
		for (BYTE m = 0;m<pNetInfo->cbDiscardCount[i];m++)
		{
			pPlayer->addHandOutCard(pNetInfo->cbDiscardCard[i][m]);
		}
	}

	m_pLocal->setHandCard(pNetInfo->cbCardData,pNetInfo->cbCardCount);

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i]->showHandCard();
		m_pPlayer[i]->startGameNoTime();
	}

	defaultPlayerActionState();
	if (pNetInfo->wOutCardUser != INVALID_WORD && pNetInfo->cbOutCardData)
	{
		XZDDPlayer* pPlayer = getPlayerByChairID(pNetInfo->wOutCardUser);
		pPlayer->showMJPoint();
	}

	if (pNetInfo->wCurrentUser != INVALID_WORD)
	{
		setCurrentPlayer(pNetInfo->wCurrentUser,pNetInfo->cbActionMask);

		XZDDPlayer* pPlayer = getPlayerByChairID(m_iCurrentUser);
		if (pPlayer != m_pLocal && pPlayer)
		{
			pPlayer->addNewInCard(0);
			m_pLocal->blackAllHandCard(true);
		}
		else if (pPlayer == m_pLocal&& pPlayer)
		{
			m_pLocal->blackHandCardByQueColor();
		}

	}
	else if (pNetInfo->cbActionMask != WIK_NULL)
	{
		setCurrentPlayer(m_pLocal->GetChairID(),pNetInfo->cbActionMask);
	}

	for (int i = 0;i<pNetInfo->cbWinCout;i++)
	{
		XZDDPlayer* pPlayer = getPlayerByChairID(pNetInfo->wWinOrder[i]);
		if (!pPlayer)
		{
			continue;
		}
		pPlayer->showHuPai(false);
		pPlayer->showStatusImagic("Hu");
	}
	m_nGameState = XZDD_STATE_PLAYING;

	WidgetFun::setVisible(this,"TimeNode",true);
	WidgetFun::setVisible(this,"LastCardNode",true);
	WidgetFun::setText(this,"LastCardCout",(int)pNetInfo->cbLeftCardCount);
}

void XZDDGameScence::OnHuanPaiScence(void* data, int dataSize)
{
	//Ч������
	ASSERT(dataSize==sizeof(CMD_S_StateHuanPai));
	if (dataSize!=sizeof(CMD_S_StateHuanPai)) return;

	CMD_S_StateHuanPai* pNetInfo = (CMD_S_StateHuanPai*)data;

	defaultState();
	m_iBankerUser=pNetInfo->wBankerUser;
	m_nLeftTime = pNetInfo->nLeftTime;

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		XZDDPlayer* pPlayer = getPlayerByChairID(i);
		pPlayer->setHandCard(pNetInfo->cbCardData,MAX_COUNT-1);

	}
	m_pLocal->setHandCard(pNetInfo->cbCardData,MAX_COUNT-1);

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i]->showHandCard();
		m_pPlayer[i]->startGameNoTime();
	}
	getPlayerByChairID(m_iBankerUser)->setZhuang();
	XZDDButtonAction_ShowCard(NULL,NULL);
	WidgetFun::setText(this,"LastCardCout",(int)pNetInfo->cbLeftCardCount);

	m_nGameState = XZDD_STATE_HUANPAI;
}
void XZDDGameScence::OnXuanQueScence(void* data, int dataSize)
{
	//Ч������
	ASSERT(dataSize==sizeof(CMD_S_StateXuanQue));
	if (dataSize!=sizeof(CMD_S_StateXuanQue)) return;

	CMD_S_StateXuanQue* pNetInfo = (CMD_S_StateXuanQue*)data;

	defaultState();
	m_iBankerUser=pNetInfo->wBankerUser;
	m_nLeftTime = pNetInfo->nLeftTime;

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		XZDDPlayer* pPlayer = getPlayerByChairID(i);
		pPlayer->setHandCard(pNetInfo->cbCardData,MAX_COUNT-1);

	}
	m_pLocal->setHandCard(pNetInfo->cbCardData,MAX_COUNT-1);

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i]->showHandCard();
		m_pPlayer[i]->startGameNoTime();
	}
	getPlayerByChairID(m_iBankerUser)->setZhuang();
	DoXuanQueNotice();
	WidgetFun::setText(this,"LastCardCout",(int)pNetInfo->cbLeftCardCount);

}