#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetDefine.h"
#include "Platform/PFKernel/CGPInsureMission.h"
#include "Platform/PFKernel/CGPPasswordMission.h"

class DBBankScence
	:public cocos2d::Node
	,public IGPInsureMissionSink
	,public IGPPasswordMissionSink
	,public FvSingleton<DBBankScence> 
{
public:
	DBBankScence();
	~DBBankScence();
public:
	void HideAll();
	void upPlayerInfo();
	void Button_Bank(cocos2d::Ref*,WidgetUserInfo*);
	void Button_SetBankPasswordAction(cocos2d::Ref*,WidgetUserInfo*);
	void Button_SaveGoldAction(cocos2d::Ref*,WidgetUserInfo*);
	void Button_TakeGoldAction(cocos2d::Ref*,WidgetUserInfo*);
	void Button_ChangeBankPasswordAction(cocos2d::Ref*,WidgetUserInfo*);
public:
	virtual bool init();
public:
	virtual void onInsureInfo();													//��������													
	virtual void onInsureSuccess(int type, const char* szDescription);					//���гɹ�
	virtual void onInsureFailure(int type, const char* szDescription);					//ʧ��
	virtual bool onInsureTransferConfirm(const char* szMessage);		//ת��
	virtual void onInsureEnableResult(int type, const char* szMessage);				//��ͨ

	virtual void onGPPasswordSuccess(int type, const char* szDescription);
	virtual void onGPPasswordFailure(int type, const char* szDescription);
private:
	CGPInsureMission m_kInsureMission;
	CGPPasswordMission m_kPasswordMission;
};