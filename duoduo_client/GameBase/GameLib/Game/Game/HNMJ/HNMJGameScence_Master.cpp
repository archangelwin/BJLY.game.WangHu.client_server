#include "HNMJGameScence.h"
#include "Game/GameLib.h"
#include "HNMJPlayer.h"
#include "HNMJGameLogic.h"

void HNMJGameScence::updateUserRight()
{
	// ���ư�ť 536870912
	if (!WidgetFun::getChildWidget(this,"Button_Master"))
	{
		utility::log("HNMJGameScence::updateUserRight()");
		return;
	}
	if( CUserRight::IsGameCheatUser(UserInfo::Instance().getUserRight()))
	{
		WidgetFun::setVisible(this,"Button_Master",true);
	}
	else
	{
		WidgetFun::setVisible(this,"Button_Master",false);
	}
}

void HNMJGameScence::initMaster()
{
	if (WidgetFun::getChildWidget(this,"GameMaterPlane"))
	{
		return;
	}
	WidgetScenceXMLparse kScence("GameHNMJ/xml/MasterGameScence.xml",this);

	cocos2d::Node* pCardNode = WidgetFun::getChildWidget(this,"MasterCardNode");

	cocos2d::Vec2 kAddPosX = utility::parsePoint(WidgetFun::getWidgetUserInfo(pCardNode,"HandAddPosX"));
	cocos2d::Vec2 kAddPosY = utility::parsePoint(WidgetFun::getWidgetUserInfo(pCardNode,"HandAddPosY"));
	std::string kImagic = WidgetFun::getWidgetUserInfo(pCardNode,"MingImagic");

	cocos2d::Vec2 startPos = cocos2d::Vec2(0,100);
	for (int i=0;i<MAX_INDEX;i++)
	{
		if (i%9 == 0)
		{
			startPos.y += kAddPosY.y;
			startPos.x = 0;
		}
		BYTE iCard = HNMJ::CGameLogic::Instance().SwitchToCardData(i);
		cocos2d::Node* pNode = WidgetManager::Instance().createWidget("NHMJ_MASTER_LEFTCARD",pCardNode);

		float fHuScale = utility::parseFloat(WidgetFun::getWidgetUserInfo(pNode,"HuScale"));
		pNode->setScale(fHuScale);

		BYTE cbValue=((BYTE)iCard&MASK_VALUE);
		BYTE cbColor=(((BYTE)iCard&MASK_COLOR)>>4)+1;
		WidgetFun::setButtonImagic(pNode,utility::toString(kImagic,(int)cbColor,(int)cbValue,".png"),true);
		WidgetFun::setWidgetUserInfo(pNode,"MasterCardData",utility::toString((int)iCard));
		WidgetFun::setWidgetUserInfo(pNode,"MasterCardindex",utility::toString((int)i));

		pNode->setPosition(startPos);
		startPos +=kAddPosX;

	}
}

void HNMJGameScence::showMater()
{
	initMaster();
	WidgetFun::setVisible(this,"GameMaterPlane",true);
	SendSocketData(SUB_C_MASTER_LEFTCARD,0,0);
	WidgetFun::setVisible(this,"MasterCheakCard",false);
}

void HNMJGameScence::defaultMaster(bool bRestZhaNiao)
{
	if (WidgetFun::getChildWidget(this,"GameMaterPlane"))
	{
		if (bRestZhaNiao)
		{
			for (int i = 1;i<7;i++)
			{
				WidgetFun::setChecked(this,utility::toString("MasterZhaNiao",i),false);
			}
		}
		WidgetFun::setVisible(this,"GameMaterPlane",false);
	}
	updateUserRight();
}

void HNMJGameScence::HNMJButton_Master(cocos2d::Ref*,WidgetUserInfo*)
{
	if (!IsInGame())
	{
		return;
	}
	showMater();
	for (int i=0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i]->setMingPai(true);
		m_pPlayer[i]->showHandCard();
	}
}

void HNMJGameScence::Button_MasterClose(cocos2d::Ref*,WidgetUserInfo*)
{
	if (!IsInGame())
	{
		return;
	}
	defaultMaster();
	for (int i=0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i]->setMingPai(false);
		m_pPlayer[i]->showHandCard();
	}
}

void HNMJGameScence::OnMasterHandCard( const void *pBuffer, WORD wDataSize )
{
	if (!IsInGame())
	{
		return;
	}
	datastream kStream(const_cast<void *>(pBuffer),wDataSize);
	MasterHandCard kMasterHandCard;
	kMasterHandCard.StreamValue(kStream,false);

	ASSERT(kMasterHandCard.kMasterHandCardList.size() == GAME_PLAYER);

	for (int i=0;i<GAME_PLAYER;i++)
	{
		if (m_pPlayer[i] == m_pLocal)
		{
			continue;
		}
		m_pPlayer[i]->setMaskHandCard(kMasterHandCard.kMasterHandCardList);	
	}
}

void HNMJGameScence::OnMasterLeftCard( const void *pBuffer, WORD wDataSize )
{
	if (!IsInGame())
	{
		return;
	}
	//Ч������
	ASSERT(wDataSize==sizeof(MasterLeftCard));
	if (wDataSize!=sizeof(MasterLeftCard)) 
		return;
	
	MasterLeftCard* pMasterLeft =(MasterLeftCard*)pBuffer; 

	cocos2d::Node* pCardNode = WidgetFun::getChildWidget(this,"MasterCardNode");
	for (int i =0;i<MAX_INDEX;i++)
	{
		cocos2d::Node* pNode = pCardNode->getChildren().at(i);
		int nCardIndex = utility::parseInt(WidgetFun::getWidgetUserInfo(pNode,"MasterCardindex"));
		if (nCardIndex == i)
		{
			WidgetFun::setText(pNode,"LeftNum",(int)pMasterLeft->kMasterLeftIndex[i]);
		}
		WidgetFun::setButtonEnabled(pNode,pMasterLeft->kMasterLeftIndex[i]>0);
	}

	setMasterCheakCard(pMasterLeft->kMasterCheakCard);
	
}
void HNMJGameScence::NHMJ_MASTER_LEFTCARD(cocos2d::Ref*,WidgetUserInfo* pUseInfo)
{
	if (!IsInGame())
	{
		return;
	}
	BYTE iCard = utility::parseInt(WidgetFun::getUserInfoValue(pUseInfo,"MasterCardData"));
	setMasterCheakCard(iCard);

	MaterCheckCard kMaterCheckCard;
	kMaterCheckCard.cbCheakCard = iCard;

	SendSocketData(SUB_C_MASTER_CHEAKCARD,&kMaterCheckCard,sizeof(kMaterCheckCard));
}
void HNMJGameScence::MasterZhaNiao(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	BYTE iNiaoCout = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"NiaoCout"));
	BYTE iLastCout = INVALID_BYTE;
	for (int i = 1;i<7;i++)
	{
		if (WidgetFun::isChecked(this,utility::toString("MasterZhaNiao",i)))
		{
			iLastCout = i;
		}
	}
	if (iLastCout == INVALID_BYTE)
	{
		iNiaoCout = INVALID_BYTE;
	}
	for (int i = 1;i<7;i++)
	{
		WidgetFun::setChecked(this,utility::toString("MasterZhaNiao",i),i == iNiaoCout);
	}
	MaterNiaoCout kMaterNiaoCout;
	kMaterNiaoCout.cbNiaoCout = iNiaoCout;

	SendSocketData(SUB_C_MASTER_ZHANIAO,&kMaterNiaoCout,sizeof(kMaterNiaoCout));
}

void HNMJGameScence::setMasterCheakCard(BYTE cbCard)
{
	cocos2d::Node* pCardNode = WidgetFun::getChildWidget(this,"MasterCheakCard");
	if (cbCard<0)
	{
		pCardNode->setVisible(false);
		return;
	}
	pCardNode->setVisible(true);
	std::string kImagic = WidgetFun::getWidgetUserInfo(pCardNode,"MingImagic");
	m_pLocal->setCardImagic(pCardNode,cbCard,kImagic);
}