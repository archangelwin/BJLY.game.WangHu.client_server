#include "HNHomeScence.h"
#include "HNScenceManager.h"
#include "GameLib/Game/GameLib.h"


//���֪ͨ
void HNHomeScence::OnGameItemFinish()
{
}
//���֪ͨ
void HNHomeScence::OnGameKindFinish(uint16 wKindID)
{

}
//����֪ͨ
void HNHomeScence::OnGameItemUpdateFinish()
{

}
//����֪ͨ
void HNHomeScence::OnGameItemInsert(CGameListItem * pGameListItem)
{
}
//����֪ͨ
void HNHomeScence::OnGameItemUpdate(CGameListItem * pGameListItem)
{

}
//ɾ��֪ͨ
void HNHomeScence::OnGameItemDelete(CGameListItem * pGameListItem)
{

}
void HNHomeScence::onGPNoticeResult(tagGameMatch&	pGameMatchInfo,bool bSucess,const char* pStr )
{
	NoticeMsg::Instance().ShowTopMsg(utility::a_u8(pStr));
}