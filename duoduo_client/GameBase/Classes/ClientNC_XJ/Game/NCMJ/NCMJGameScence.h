#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetDefine.h"
#include "Game/Game/GameBase.h"
#include "Game/Game/GameDefine.h"
#include "Game/Script/TimeNode.h"


class NCMJPlayer;
struct CMD_S_GameEnd;
struct  CMD_WeaveItem;
struct  HNMJGameRecord;

class NCMJGameScence
	:public GameBase
	,public TimeNode
	,public FvSingleton<NCMJGameScence> 
{
public:
	const static int KIND_ID = 320;
	const static int VERSION_SERVER	= PROCESS_VERSION(6,0,3);				//����汾
	const static int VERSION_CLIENT	= PROCESS_VERSION(6,0,3);	
	const static int MAX_PLAYER = 4;

	enum 
	{
		NCMJ_STATE_NULL,
		NCMJ_STATE_READY,
		NCMJ_STATE_XIAO_HU,
		NCMJ_STATE_PLAYING,
	};
public:
	NCMJGameScence();
	~NCMJGameScence();
public:
	virtual bool init();
	virtual bool IsInGame();

	void initPrivate();
public:
	void EnterScence();
	void HideAll();
	void defaultState();
	void defaultPlayerActionState();
	void showSaiZi(unsigned int iValue);
	std::string getStringHuRight(dword kValue);
	std::string getStringGang(int nChairID,CMD_WeaveItem* pWeave,int iWeaveCout);
	void setCurrentPlayer(int iCurrentPlayer,int iUserAction);
	void showJieSuanInfo(CMD_S_GameEnd* pGameEnd);
	void setJieSuanNiaoCard(cocos2d::Node* pNode,BYTE* pNiaoCard,BYTE cbCardNum);
public:
	void initButton();
	void NCMJButton_Ready(cocos2d::Ref*,WidgetUserInfo*);
	void NCMJButton_GuoAction(cocos2d::Ref*,WidgetUserInfo*);
	void NCMJButton_GangAction(cocos2d::Ref*,WidgetUserInfo*);
	void NCMJButton_PengAction(cocos2d::Ref*,WidgetUserInfo*);
	void NCMJButton_ChiAction(cocos2d::Ref*,WidgetUserInfo*);
	void NCMJButton_HuAction(cocos2d::Ref*,WidgetUserInfo*);
	void NCMJButton_XiaoHuAction(cocos2d::Ref*,WidgetUserInfo*);
	void NCMJButtonAction_ShowNext(cocos2d::Ref*,WidgetUserInfo*);
	void NCMJButtonAction_ShowCard(cocos2d::Ref*,WidgetUserInfo*);

	void NCMJButton_JieSuanShare(cocos2d::Ref*,WidgetUserInfo*);
	void NCMJButton_JieSuanStart(cocos2d::Ref*,WidgetUserInfo*);

	void NCMJButton_TalkBegin(cocos2d::Ref*,WidgetUserInfo*);
	void endButtonTalk();
	void NCMJButton_TalkEnd(cocos2d::Ref*,WidgetUserInfo*);
private:
	virtual bool OnEventSceneMessage(byte cbGameStatus, bool bLookonUser, void* data, int dataSize);
	void OnFreeScence(void* data, int dataSize);
	void OnPlayScence(void* data, int dataSize);
public:
	NCMJPlayer* getPlayerByChairID(int iChairID);
	virtual GamePlayer* CreatePlayer(IClientUserItem * pIClientUserItem);
	virtual void DeletePlayer(GamePlayer* pPlayer);
	virtual void upSelfPlayerInfo();
	virtual void OnGFGameClose(int iExitCode);
	virtual void OnEventUserStatus(GamePlayer * pPlayer);

	//˽�˳�
public:
	void defaultPrivateState();
	virtual void OnSocketSubPrivateRoomInfo(CMD_GF_Private_Room_Info* pNetInfo);
	virtual void OnSocketSubPrivateEnd(CMD_GF_Private_End_Info* pNetInfo);
	virtual void OnSocketSubPrivateDismissInfo(CMD_GF_Private_Dismiss_Info* pNetInfo);
	void XZDDButton_WeiXinFriend(cocos2d::Ref*,WidgetUserInfo*);
	void XZDDButton_WeiXinImagic(cocos2d::Ref*,WidgetUserInfo*);
	void ButtonPlayerHeadClick(cocos2d::Ref*,WidgetUserInfo* pUserInfo);
public:
	void initNet();

	void OnSubGameStart(const void * pBuffer, word wDataSize);
	//�û�����
	void OnSubOutCard(const void * pBuffer, WORD wDataSize);
	//������Ϣ
	void OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//������ʾ
	void OnSubOperateNotify(const void * pBuffer, WORD wDataSize);
	//�������
	void OnSubOperateResult(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	void OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//�û��й�
	void OnSubTrustee(const void * pBuffer,WORD wDataSize);
	//�Ժ���Ϣ
	void OnSubUserChiHu( const void *pBuffer,WORD wDataSize );
	//�ܵ÷�
	void OnSubGangScore( const void *pBuffer, WORD wDataSize );
public:
	void Command_PlaceBet(int iArea,int iBetScore);
	void SendOutCard(cocos2d::Node* pCard);
public:
	void initTouch();
	bool ccTouchBegan(cocos2d::Vec2 kPos);
	void ccTouchMoved(cocos2d::Vec2 kPos);
	void ccTouchEnded(cocos2d::Vec2 kPos);

public:
	void setGameState(int nState);
	int getGameState();
	void setPlayCount(int nCount);
	int getPlayCount();
	void setPrivateInfo(CMD_GF_Private_Room_Info* pNetInfo);

public:
	void initRecord();
	void defaultRecordState();
	bool StartRecord(datastream kDataStream);
	void NextRecordAction();
	virtual void onGPAccountInfoHttpIP(dword dwUserID, std::string strIP,std::string strHttp);

	void Button_GameRecordPlay(cocos2d::Ref*,WidgetUserInfo*);
	void Button_GameRecordPase(cocos2d::Ref*,WidgetUserInfo*);
	void Button_GameRecordLeft(cocos2d::Ref*,WidgetUserInfo*);
	void Button_GameRecordRight(cocos2d::Ref*,WidgetUserInfo*);
	void Button_GameRecordExit(cocos2d::Ref*,WidgetUserInfo*);
protected:
	int							m_iBankerUser;						//ׯ���û�
	int							m_iCurrentUser;						//��ǰ�û�
	int							m_iUserAction;						//��Ҷ���

	NCMJPlayer*					m_pLocal;
	NCMJPlayer*					m_pPlayer[MAX_PLAYER];

	cocos2d::Node*				m_pTouchCardNode;
	cocos2d::Vec2				    m_kTouchSrcPos;

	int                         m_nGameState;
	int                         m_nPlayCount;

	word							m_wRecordSelfChairID;
	int							m_iActRecordIdex;
	int							m_RecordTimeID;
	float						m_RecordTime;
	HNMJGameRecord*				m_pGameRecord;
};