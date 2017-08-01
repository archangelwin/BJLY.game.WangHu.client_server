#include "LPGameScence.h"
#include "Game/GameLib.h"
#include "CMD_Land.h"

bool LPGameScence::OnEventSceneMessage(byte cbGameStatus, bool bLookonUser, void* data, int dataSize)
{
	switch (cbGameStatus)
	{
	case GAME_SCENE_BET:		//��ע״̬
		{
			OnBetScence(data,dataSize);
			return true;
		}
	case GAME_SCENE_PLAY:		//��Ϸ״̬
		{
			OnPlayScence(data,dataSize);
			return true;
		}
	}
	return true;
}
void LPGameScence::OnBetScence(void* data, int wDataSize)
{
	cocos2d::log("OnBetScence");
	//Ч������
	ASSERT(wDataSize == sizeof(CMD_S_StatusBet));
	if (wDataSize!=sizeof(CMD_S_StatusBet)) return;

	CMD_S_StatusBet* pStateBet = (CMD_S_StatusBet*)data;

	cleanYaChips(m_pYaChipList);
	cleanYaChips(m_kBetChipMap);
	showClock(pStateBet->cbTimeLeave);
	m_nGameState = GAME_SCENE_BET;
	
}
void LPGameScence::OnPlayScence(void* data, int wDataSize)
{
	cocos2d::log("OnPlayScence");
	//Ч������
	ASSERT(wDataSize == sizeof(CMD_S_StatusPlay));
	if (wDataSize!=sizeof(CMD_S_StatusPlay)) return;

	CMD_S_StatusPlay* pStatePlay = (CMD_S_StatusPlay*)data;
	m_nGameState = GAME_SCENE_PLAY;
//	showClock(pStatePlay->cbTimeLeave);
}