#include "GameBase.h"

#include "Game/GameLib.h"
#include "ScenceManagerBase.h"
#include "GameManagerBase.h"


//������Ϸ
bool GameBase::SetupGameClient()
{
	return true;
}
//������Ϸ
void GameBase::ResetGameClient()
{
	return;
}
//�ر���Ϸ
void GameBase::CloseGameClient(bool isExit)
{
	if (isExit)
	{
		GameManagerBase::InstanceBase().index = 1;
		ExitGame();
	}else{
		GameManagerBase::InstanceBase().index = 0;
	}
	
	return;
}