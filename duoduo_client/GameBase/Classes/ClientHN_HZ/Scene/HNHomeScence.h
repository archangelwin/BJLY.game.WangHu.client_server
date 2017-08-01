#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetDefine.h"
#include "Platform/PFKernel/CGPLoginMission.h"

class HNHomeScence
	:public cocos2d::Node
	,public IServerListDataSink
	,public FvSingleton<HNHomeScence> 
{
public:
	HNHomeScence();
	~HNHomeScence();
public:
	virtual bool init();
public:
	//���֪ͨ
	virtual void OnGameItemFinish();
	//���֪ͨ
	virtual void OnGameKindFinish(uint16 wKindID);
	//����֪ͨ
	virtual void OnGameItemUpdateFinish();

	//����֪ͨ
public:
	//����֪ͨ
	virtual void OnGameItemInsert(CGameListItem * pGameListItem);
	//����֪ͨ
	virtual void OnGameItemUpdate(CGameListItem * pGameListItem);
	//ɾ��֪ͨ
	virtual void OnGameItemDelete(CGameListItem * pGameListItem);

	virtual void onGPNoticeResult(tagGameMatch&	pGameMatchInfo,bool bSucess,const char* pStr );
public:
	void EnterScence();
	void upPlayerInfo();
	void LogonSucess();
	void upGameSever();

	void upJoinNumTxt();
public:
	void init_Button();
	void Button_GameRoom(cocos2d::Ref*,WidgetUserInfo*);
	void Button_BackHome(cocos2d::Ref*,WidgetUserInfo*);
	void Button_HomeHead(cocos2d::Ref*,WidgetUserInfo*);
	void Button_HomeExit(cocos2d::Ref*,WidgetUserInfo*);

	void HZButton_BuyFangKa(cocos2d::Ref*,WidgetUserInfo*);
	void HZButton_Fuli(cocos2d::Ref*,WidgetUserInfo*);
	void HZButton_zhanji(cocos2d::Ref*,WidgetUserInfo*);
	void HZButton_fankui(cocos2d::Ref*,WidgetUserInfo*);
	void HZButton_wanfa(cocos2d::Ref*,WidgetUserInfo*);
	void HZButton_FenQu(cocos2d::Ref*,WidgetUserInfo*);
	void HZButton_HongZhongCreate(cocos2d::Ref*,WidgetUserInfo*);
	void HZButton_HongZhongJoin(cocos2d::Ref*,WidgetUserInfo*);
	
private:
};