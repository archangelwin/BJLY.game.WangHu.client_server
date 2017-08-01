#include "CarGameScence.h"

#include "CMD_RedNine100.h"

void CarGameScence::upSelfPlayerInfo()
{
	m_nUserScore = getSelfGamePlayer()->GetUserScore();
	m_nKeYongScore = m_nUserScore;

	setUserNickeName(utility::a_u8(getSelfGamePlayer()->GetNickName()));
	setUserScore(m_nUserScore);
	setUserKeYong(m_nUserScore);
	setUserResoult(0);
}

bool CarGameScence::OnEventSceneMessage( byte cbGameStatus, bool bLookonUser, void* data, int dataSize )
{
	switch (cbGameStatus)
	{
	case GAME_STATUS_FREE:			//����״̬
		{  
			onGameStatusFree(data,dataSize);
			return true;
		}
	case GS_PLACE_JETTON:			//��Ϸ״̬
	case GS_GAME_END:				//����״̬
		{
			onGameStatusPlay(data,dataSize);
			return true;
		}
	}
	return true;
}
