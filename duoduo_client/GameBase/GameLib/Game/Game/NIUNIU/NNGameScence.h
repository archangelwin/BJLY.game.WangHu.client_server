#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetDefine.h"
#include "Game/Game/GameBase.h"
#include "Game/Game/GameDefine.h"
#include "Game/Script/TimeNode.h"


class NNPlayer;
struct CMD_S_GameStart;

class NNGameScence
	:public GameBase
	,public TimeNode
	,public FvSingleton<NNGameScence> 
{
public:
	const static int KIND_ID = 27;
	const static int VERSION_SERVER	= PROCESS_VERSION(6,0,3);				//����汾
	const static int VERSION_CLIENT	= PROCESS_VERSION(6,0,3);	
	const static int MAX_PLAYER = 5;

	enum 
	{
		GAME_OPTION_RULE_SHOW_WIN_LOSE1,
	};
public:
	NNGameScence();
	~NNGameScence();
public:
	virtual bool init();
	virtual bool IsInGame();

	void initPrivate();
public:
	void EnterScence();
	void HideAll();
	void defaultState();

	bool isSelfBanker();
	bool isSelfInGame();
public:
	void initButton();
	void NNButton_Ready(cocos2d::Ref*,WidgetUserInfo*);
	void NNButton_QiangZhuang(cocos2d::Ref*,WidgetUserInfo*);
	void NNButton_BuQiang(cocos2d::Ref*,WidgetUserInfo*);
	void NNButton_Bet(cocos2d::Ref*,WidgetUserInfo*);
	void NNButton_YouNiu(cocos2d::Ref*,WidgetUserInfo*);
	void NNButton_MeiYou(cocos2d::Ref*,WidgetUserInfo*);

	void NNButton_SendCardEnd(cocos2d::Ref*,WidgetUserInfo*);
	void NNButton_CheakZhuangEnd(cocos2d::Ref*,WidgetUserInfo*);

	void HNMJButton_TalkBegin(cocos2d::Ref*,WidgetUserInfo*);
	void endButtonTalk();
	void HNMJButton_TalkEnd(cocos2d::Ref*,WidgetUserInfo*);
private:
	virtual bool OnEventSceneMessage(byte cbGameStatus, bool bLookonUser, void* data, int dataSize);
	void OnFreeScence(void* data, int dataSize);
	void OnPlayScence(void* data, int dataSize);
	void OnCallBankerScence(void* data, int dataSize);
	void OnBetScoreScence(void* data, int dataSize);

	//��Ϣ����
protected:
	void initNet();
	//����
	void OnSubAllCard(void * pBuffer, int wDataSize);
	//�û���ׯ
	void OnSubCallBanker(void * pBuffer, int wDataSize);
	//��Ϸ��ʼ
	void OnSubGameStart(void * pBuffer, int wDataSize);
	//�û���ע
	void OnSubAddScore(void * pBuffer, int wDataSize);
	//������Ϣ
	void OnSubSendCard(void * pBuffer, int wDataSize);
	//�û�̯��
	void OnSubOpenCard(void * pBuffer, int wDataSize);
	//�û�ǿ��
	void OnSubPlayerExit(void * pBuffer, int wDataSize);
	//��Ϸ����
	void OnSubGameEnd(void * pBuffer, int wDataSize);
	//����
	void OnSubAdminControl(void * pBuffer, int wDataSize);
public:
	void initTouch();
	bool ccTouchBegan(cocos2d::Vec2 kPos);
	void ccTouchMoved(cocos2d::Vec2 kPos);
	void ccTouchEnded(cocos2d::Vec2 kPos);
public:
	NNPlayer* getPlayerByChairID(int iChairID);
	virtual GamePlayer* CreatePlayer(IClientUserItem * pIClientUserItem);
	virtual void DeletePlayer(GamePlayer* pPlayer);
	virtual void upSelfPlayerInfo();
	virtual void OnGFGameClose(int iExitCode);
	virtual void OnEventUserStatus(GamePlayer * pPlayer);
	virtual void LocalPlayerEnter();

	//˽�˳�
public:
	void defaultPrivateState();
	void setPrivateInfo(CMD_GF_Private_Room_Info* pNetInfo);
	virtual void OnSocketSubPrivateRoomInfo(CMD_GF_Private_Room_Info* pNetInfo);
	virtual void OnSocketSubPrivateEnd(CMD_GF_Private_End_Info* pNetInfo);
	virtual void OnSocketSubPrivateDismissInfo(CMD_GF_Private_Dismiss_Info* pNetInfo);
	void XZDDButton_WeiXinFriend(cocos2d::Ref*,WidgetUserInfo*);
	void XZDDButton_WeiXinImagic(cocos2d::Ref*,WidgetUserInfo*);
	void ButtonPlayerHeadClick(cocos2d::Ref*,WidgetUserInfo* pUserInfo);
public:
	void showBoxNumNode(bool bShow);
	void updateUpCardList(cocos2d::Node* pCard);
	void updateBoxNum();
	void checkZhuang(CMD_S_GameStart * pNetInfo);
	void showJieSuanInfo(LONGLONG lScore);
protected:
	NNPlayer*					m_pLocal;
	NNPlayer*					m_pPlayer[MAX_PLAYER];

	cocos2d::Node*				m_pTouchCardNode;

	word						m_wRecordSelfChairID;
	int							m_iActRecordIdex;
	int							m_RecordTimeID;
	float						m_RecordTime;

	BYTE						m_cbPlayStatus[MAX_PLAYER];			//��Ϸ״̬
	BYTE						m_cbBankerChairID;

	std::vector<cocos2d::Node*>     m_kUpCardList;
};