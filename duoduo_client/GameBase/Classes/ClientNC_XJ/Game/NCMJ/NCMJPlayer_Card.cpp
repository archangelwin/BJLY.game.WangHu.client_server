#include "NCMJPlayer.h"
#include "NCMJGameLogic.h"
#include "NCMJSoundFun.h"

void NCMJPlayer::setHandCard(BYTE* pCardData,int iCout)
{
	memcpy(m_kHandCardData,pCardData,sizeof(m_kHandCardData));
	m_kHandCardCout = iCout;
}
void NCMJPlayer::setWeaveItem(CMD_WeaveItem* pWeave,int iCout)
{
	memcpy(m_kWeaveItemArray,pWeave,sizeof(m_kWeaveItemArray));
	m_kWeaveCount = iCout;
}
void NCMJPlayer::setOperateResoult(CMD_S_OperateResult* pNetInfo)
{
	bool cbPublicCard = false;
	word wOperateUser=pNetInfo->wOperateUser;
	BYTE cbOperateCard=pNetInfo->cbOperateCard;


	if (pNetInfo->cbOperateCode == WIK_PENG)
	{
		runAniPeng();
	}	
	if (pNetInfo->cbOperateCode == WIK_LEFT||pNetInfo->cbOperateCode == WIK_CENTER||pNetInfo->cbOperateCode == WIK_RIGHT)
	{
		runAniChi();
	}
	else if(pNetInfo->cbOperateCode == WIK_GANG && pNetInfo->wProvideUser==wOperateUser )
	{
		runAniAnGang();
	}	
	else if(pNetInfo->cbOperateCode == WIK_GANG && pNetInfo->wProvideUser!=wOperateUser )
	{
		runAniMingGang();
	}


	if ((pNetInfo->cbOperateCode&WIK_GANG)!=0)
	{
		//����˿�
		BYTE cbWeaveIndex=0xFF;
		for (BYTE i=0;i<m_kWeaveCount;i++)
		{
			BYTE cbWeaveKind=m_kWeaveItemArray[i].cbWeaveKind;
			BYTE cbCenterCard=m_kWeaveItemArray[i].cbCenterCard;
			if ((cbCenterCard==pNetInfo->cbOperateCard)&&(cbWeaveKind==WIK_PENG))
			{
				cbWeaveIndex=i;
				m_kWeaveItemArray[cbWeaveIndex].cbPublicCard=true;
				m_kWeaveItemArray[cbWeaveIndex].cbWeaveKind=pNetInfo->cbOperateCode;
				m_kWeaveItemArray[cbWeaveIndex].wProvideUser=pNetInfo->wProvideUser;
				break;
			}
		}

		//����˿�
		if (cbWeaveIndex==0xFF)
		{
			//�����ж�
			cbPublicCard=(pNetInfo->wProvideUser==wOperateUser)?FALSE:TRUE;

			//�����˿�
			cbWeaveIndex=m_kWeaveCount++;
			m_kWeaveItemArray[cbWeaveIndex].cbPublicCard=cbPublicCard;
			m_kWeaveItemArray[cbWeaveIndex].cbCenterCard=cbOperateCard;
			m_kWeaveItemArray[cbWeaveIndex].cbWeaveKind=pNetInfo->cbOperateCode;
			m_kWeaveItemArray[cbWeaveIndex].wProvideUser=pNetInfo->wProvideUser;
		}

		//�˿�����
		if (m_iIdex == 0 || m_bGameRecord)
		{
			 NCMJ::CGameLogic::Instance().RemoveValueCardAll(m_kHandCardData,m_kHandCardCout,pNetInfo->cbOperateCard);
		}
		m_kHandCardCout = MAX_COUNT - m_kWeaveCount*3-1;
	}
	else if (pNetInfo->cbOperateCode!=WIK_NULL)
	{
		//�������
		BYTE cbWeaveIndex=m_kWeaveCount++;
		m_kWeaveItemArray[cbWeaveIndex].cbPublicCard=true;
		m_kWeaveItemArray[cbWeaveIndex].cbCenterCard=cbOperateCard;
		m_kWeaveItemArray[cbWeaveIndex].cbWeaveKind=pNetInfo->cbOperateCode;
		m_kWeaveItemArray[cbWeaveIndex].wProvideUser=pNetInfo->wProvideUser;

		//��Ͻ���
		//ɾ���˿�
		if (m_iIdex == 0 || m_bGameRecord)
		{
			BYTE cbWeaveCard[4]={cbOperateCard,cbOperateCard,cbOperateCard,cbOperateCard};
			if (pNetInfo->cbOperateCode == WIK_LEFT)
			{
				cbWeaveCard[0] = cbOperateCard+1;
				cbWeaveCard[1] = cbOperateCard+2;
			}
			if (pNetInfo->cbOperateCode == WIK_CENTER)
			{
				cbWeaveCard[0] = cbOperateCard-1;
				cbWeaveCard[1] = cbOperateCard+1;
			}
			if (pNetInfo->cbOperateCode == WIK_RIGHT)
			{
				cbWeaveCard[0] = cbOperateCard-1;
				cbWeaveCard[1] = cbOperateCard-2;
			}
			 NCMJ::CGameLogic::Instance().RemoveValueCard(m_kHandCardData,m_kHandCardCout,cbWeaveCard,2);
			m_kHandCardCout -= 2;
		}
		else
		{
			m_kHandCardCout = MAX_COUNT - m_kWeaveCount*3;
		}
	}
	showHandCard();
}

void NCMJPlayer::addNewInCard(int iCard)
{
	m_kHandCardCout++;
	m_kHandCardData[MAX_COUNT-1-m_kWeaveCount*3] = iCard;
}

void NCMJPlayer::sendOutCard(int iCard)
{
	m_pHandOutCard = addHandOutCard(iCard);
	m_pHandOutCard->setVisible(false);
	WidgetFun::runWidgetAction(m_pHandOutCard,"OutAni");//�ȴ�2����ʾ

	setActOutCard(iCard);
}

void NCMJPlayer::setActOutCard(int iCard,bool bDel)
{
	cocos2d::Node* pNode = WidgetFun::getChildWidget(m_pSeatNode,"BigOutNode");
	pNode->setVisible(iCard >= 0);

	if (iCard < 0)
	{
		return;
	}
	std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");
	setCardImagic(pNode,iCard,kImagic);

	if (m_pHandOutCard != NULL)
	{
		//���ƶ���
		cocos2d::Vec2 kEndPos = m_pHandOutCard->getPosition();
		pNode->setAnchorPoint(m_pHandOutCard->getAnchorPoint());
		WidgetFun::setWidgetUserInfo(pNode,"OutHandPos",utility::toString(kEndPos));
		WidgetFun::runWidgetAction(pNode,"Start");

	}

	NCMJSoundFun::playCardByGender(GetGender(),iCard);
	if (bDel)
	{
		if (m_iIdex == 0 || m_bGameRecord)
		{
			NCMJ::CGameLogic::Instance().RemoveValueCardOne(m_kHandCardData,m_kHandCardCout,iCard);
			m_kHandCardCout --;
			NCMJ::CGameLogic::Instance().SortCardList(m_kHandCardData,m_kHandCardCout);
		}
		else
		{
			m_kHandCardCout -- ;
		}
	}

	showHandCard();

}
cocos2d::Node* NCMJPlayer::addHandOutCard(int iCard)
{
	int iIdex = m_iIdex;
	cocos2d::Node* pCardNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode2");
	cocos2d::Vec2 kStartPos = WidgetFun::getChildWidget(m_pSeatNode,"OutPosNode")->getPosition();
	int iAddOder = utility::parseInt(WidgetFun::getWidgetUserInfo(
		m_pSeatNode,"OutPosNode","AddOder"));
	cocos2d::Vec2 kOutAddPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(
		m_pSeatNode,"OutPosNode","OutAddPos"));
	cocos2d::Vec2 kNextAddPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(
		m_pSeatNode,"OutPosNode","NextAddPos"));
	cocos2d::Vec2 kPos;

	int nLength = m_kOutCardList.size();
	if (nLength >= 10)
	{
		kPos = kOutAddPos*(nLength-10)+kStartPos;
		kPos += kNextAddPos;
	}
	else
	{
		kPos = kOutAddPos*nLength+kStartPos;
	}
	std::string kHandSkin = utility::toString("HNMJ_OUT_",iIdex);

	cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kHandSkin,pCardNode);
	pNode->setZOrder(iAddOder*nLength);
	pNode->setPosition(kPos);
	std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");
	setCardImagic(pNode,iCard,kImagic);

	m_kOutCardList.push_back(OutCardInfo(pNode,iCard));
	return pNode;
}
void NCMJPlayer::showHandCard()
{
	showHandCard(m_kWeaveItemArray,m_kWeaveCount,m_kHandCardData,m_kHandCardCout);
}
void NCMJPlayer::showHandCard(CMD_WeaveItem* pWeave,int iWeaveCout,BYTE* pHandCard,int iHandCout)
{
	bool bHaveNewIn = (iWeaveCout*3+iHandCout) == MAX_COUNT;
	int iIdex = m_iIdex;
	cocos2d::Node* pCardNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode1");
	pCardNode->removeAllChildren();
	cocos2d::Vec2 kStartPos = WidgetFun::getChildWidget(m_pSeatNode,"HandPosNode")->getPosition();
	int iAddOder = utility::parseInt(WidgetFun::getWidgetUserInfo(
		m_pSeatNode,"HandPosNode","AddOder"));
	cocos2d::Vec2 kHandAddPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(
		m_pSeatNode,"HandPosNode","HandAddPos"));
	cocos2d::Vec2 kNewInAddPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(
		m_pSeatNode,"HandPosNode","NewInAddPos"));
	std::string kPengSkin = utility::toString("HNMJ_PENG_",iIdex);
	std::string kGangSkin = utility::toString("HNMJ_GANG_",iIdex);
	std::string kHandSkin = utility::toString("HNMJ_HAND_",iIdex);
	int iOder = 0;
	for (int i = 0;i<iWeaveCout;i++)
	{
		CMD_WeaveItem* pTemp = pWeave+i;
		int iCardValue = pTemp->cbCenterCard;
		if (pTemp->cbPublicCard == 0)
		{
			iCardValue = -1;
		}

		if (pTemp->cbWeaveKind == WIK_PENG)
		{
			cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kPengSkin,pCardNode);
			pNode->setZOrder(iOder);
			pNode->setPosition(kStartPos);
			kStartPos += kHandAddPos*3;
			std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card0"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card1"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card2"),iCardValue,kImagic);
		}
		if (pTemp->cbWeaveKind == WIK_GANG)
		{
			cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kGangSkin,pCardNode);
			pNode->setZOrder(iOder);
			pNode->setPosition(kStartPos);
			kStartPos += kHandAddPos*3;
			std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card0"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card1"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card2"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card3"),pTemp->cbCenterCard,kImagic);
		}
		if (pTemp->cbWeaveKind == WIK_LEFT)
		{
			cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kPengSkin,pCardNode);
			pNode->setZOrder(iOder);
			pNode->setPosition(kStartPos);
			kStartPos += kHandAddPos*3;
			std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card0"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card1"),iCardValue+1,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card2"),iCardValue+2,kImagic);
		}
		if (pTemp->cbWeaveKind == WIK_CENTER)
		{
			cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kPengSkin,pCardNode);
			pNode->setZOrder(iOder);
			pNode->setPosition(kStartPos);
			kStartPos += kHandAddPos*3;
			std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card0"),iCardValue-1,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card1"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card2"),iCardValue+1,kImagic);
		}
		if (pTemp->cbWeaveKind == WIK_RIGHT)
		{
			cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kPengSkin,pCardNode);
			pNode->setZOrder(iOder);
			pNode->setPosition(kStartPos);
			kStartPos += kHandAddPos*3;
			std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card0"),iCardValue-2,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card1"),iCardValue-1,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card2"),iCardValue,kImagic);
		}
		iOder+=iAddOder;
	}
	for (int i = 0;i<iHandCout;i++)
	{
		BYTE* pTemp = pHandCard+i;
		cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kHandSkin,pCardNode);
		pNode->setTag(1);
		pNode->setZOrder(iOder);
		pNode->setPosition(kStartPos);
		if (bHaveNewIn && i == (iHandCout - 2))
		{
			kStartPos += kNewInAddPos;
		}
		else
		{
			kStartPos += kHandAddPos;
		}
		int iCardValue = *pTemp;
		if (m_iIdex == 0 || m_bGameRecord)
		{
			std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");
			setCardImagic(pNode,iCardValue,kImagic);
		}
		iOder+=iAddOder;
	}
}
void NCMJPlayer::setCardImagic(cocos2d::Node* pNode,int kValue,std::string kImagicFront)
{
	if (kValue > 0)
	{
		BYTE cbValue=((BYTE)kValue&MASK_VALUE);
		BYTE cbColor=(((BYTE)kValue&MASK_COLOR)>>4)+1;
		WidgetFun::setImagic(pNode,utility::toString(kImagicFront,(int)cbColor,(int)cbValue,".png"),false);
	}
	else
	{
		WidgetFun::setImagic(pNode,utility::toString(kImagicFront,0,0,".png"),false);
	}
}
cocos2d::Node* NCMJPlayer::getTouchCardNode(cocos2d::Vec2 kTouchPos)
{
	cocos2d::Node* pNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode1");
	for (int i = 0;i<pNode->getChildrenCount();i++)
	{
		cocos2d::Sprite* pSprite = dynamic_cast<cocos2d::Sprite*>(pNode->getChildren().at(i));
		if (pSprite == NULL || pSprite->getTag() != 1)
		{
			continue;
		}
		if (utility::IsSpriteInTouch(kTouchPos,pSprite))
		{
			return pSprite;
		}
	}
	return NULL;
}

BYTE NCMJPlayer::getTouchCardVlaue(cocos2d::Node* pNode)
{
	int iIdex = 0;
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode1");
	for (int i = 0;i<pRootNode->getChildrenCount();i++)
	{
		cocos2d::Sprite* pSprite = dynamic_cast<cocos2d::Sprite*>(pRootNode->getChildren().at(i));
		if (pSprite == NULL || pSprite->getTag() != 1)
		{
			continue;
		}
		if (pSprite == pNode)
		{
			return m_kHandCardData[iIdex];
		}
		iIdex++;
	}
	return NULL;
}
void NCMJPlayer::showHuPai(bool bZiMo)
{
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode1");
	for (int i = 0;i<pRootNode->getChildrenCount();i++)
	{
		cocos2d::Sprite* pSprite = dynamic_cast<cocos2d::Sprite*>(pRootNode->getChildren().at(i));
		if (pSprite == NULL || pSprite->getTag() != 1)
		{
			continue;
		}
		float fHuScale = utility::parseFloat(WidgetFun::getWidgetUserInfo(pSprite,"HuScale"));
		pSprite->setScale(fHuScale);
		std::string kImagic = WidgetFun::getWidgetUserInfo(pSprite,"MingImagic");
		if (i == (pRootNode->getChildrenCount()-1) && !bZiMo)
		{
			setCardImagic(pSprite,m_kHandCardData[m_kHandCardCout-1],kImagic);
		}
		else
		{
			setCardImagic(pSprite,0,kImagic);
		}
	}
}
void NCMJPlayer::removeHandOutCard( BYTE cbCardData )
{
	int nSize = m_kOutCardList.size();
	if (nSize <=0 )
	{
		return;
	}
	int i = nSize -1;
	if (m_kOutCardList[i].pCardNode != NULL && m_kOutCardList[i].nCardDarta == cbCardData)
	{
		m_kOutCardList[i].pCardNode->removeFromParent();
		m_kOutCardList[i].pCardNode = NULL;
		m_kOutCardList.pop_back();
	}
}

void NCMJPlayer::showCard()
{
	showCard(m_kHandCardData,m_kHandCardCout);
}
void NCMJPlayer::showCard(BYTE* cbCardData,BYTE cbCardCount)
{
	BYTE cbIdex = 0;
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode1");
	for (int i = 0;i<pRootNode->getChildrenCount();i++)
	{
		cocos2d::Sprite* pSprite = dynamic_cast<cocos2d::Sprite*>(pRootNode->getChildren().at(i));
		if (pSprite == NULL || pSprite->getTag() != 1)
		{
			continue;
		}
		float fHuScale = utility::parseFloat(WidgetFun::getWidgetUserInfo(pSprite,"HuScale"));
		pSprite->setScale(fHuScale);
		std::string kImagic = WidgetFun::getWidgetUserInfo(pSprite,"MingImagic");
		if (cbIdex < cbCardCount)
		{
			setCardImagic(pSprite,cbCardData[cbIdex],kImagic);
		}
		else
		{
			setCardImagic(pSprite,0,kImagic);
		}
		cbIdex++;
	}
}

void NCMJPlayer::showJieSuanCard(cocos2d::Node* pCardNode,CMD_WeaveItem* pWeave,int iWeaveCout,BYTE* pHandCard,int iHandCout)
{
	ASSERT(pCardNode);
	pCardNode->removeAllChildren();

	int iIdex = 0;

	cocos2d::Vec2 kStartPos = pCardNode->getPosition();
	int iAddOder = utility::parseInt(WidgetFun::getWidgetUserInfo(pCardNode,"AddOder"));
	cocos2d::Vec2 kHandAddPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(pCardNode,"JieSuanAddPos"));
	cocos2d::Vec2 kJieSuanWeaveGap = utility::parsePoint(WidgetFun::getWidgetUserInfo(pCardNode,"JieSuanWeaveGap"));

	std::string kPengSkin = utility::toString("HNMJ_PENG_",iIdex);
	std::string kGangSkin = utility::toString("HNMJ_GANG_",iIdex);
	std::string kHandSkin = utility::toString("HNMJ_HAND_",iIdex);
	int iOder = 0;
	
	for (int i = 0;i<iWeaveCout;i++)
	{
		CMD_WeaveItem* pTemp = pWeave+i;
		int iCardValue = pTemp->cbCenterCard;
		if (pTemp->cbPublicCard == 0)
		{
			iCardValue = -1;
		}

		if (pTemp->cbWeaveKind == WIK_PENG)
		{
			cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kPengSkin,pCardNode);
			pNode->setZOrder(iOder);
			pNode->setPosition(kStartPos);
			kStartPos += kHandAddPos*3 + kJieSuanWeaveGap;
			std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card0"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card1"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card2"),iCardValue,kImagic);
		}
		if (pTemp->cbWeaveKind == WIK_GANG)
		{
			cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kGangSkin,pCardNode);
			pNode->setZOrder(iOder);
			pNode->setPosition(kStartPos);
			kStartPos += kHandAddPos*3+kJieSuanWeaveGap;
			std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card0"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card1"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card2"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card3"),pTemp->cbCenterCard,kImagic);
		}
		if (pTemp->cbWeaveKind == WIK_LEFT)
		{
			cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kPengSkin,pCardNode);
			pNode->setZOrder(iOder);
			pNode->setPosition(kStartPos);
			kStartPos += kHandAddPos*3+kJieSuanWeaveGap;
			std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card0"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card1"),iCardValue+1,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card2"),iCardValue+2,kImagic);
		}
		if (pTemp->cbWeaveKind == WIK_CENTER)
		{
			cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kPengSkin,pCardNode);
			pNode->setZOrder(iOder);
			pNode->setPosition(kStartPos);
			kStartPos += kHandAddPos*3+kJieSuanWeaveGap;
			std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card0"),iCardValue-1,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card1"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card2"),iCardValue+1,kImagic);
		}
		if (pTemp->cbWeaveKind == WIK_RIGHT)
		{
			cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kPengSkin,pCardNode);
			pNode->setZOrder(iOder);
			pNode->setPosition(kStartPos);
			kStartPos += kHandAddPos*3+kJieSuanWeaveGap;
			std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card0"),iCardValue-2,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card1"),iCardValue-1,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card2"),iCardValue,kImagic);
		}
		iOder+=iAddOder;
	}

	BYTE cbChiHuCard = 0;

	for (int i = 0;i<iHandCout;i++)
	{
		BYTE* pTemp = pHandCard+i;
		int iCardValue = *pTemp;
		if (iCardValue == getChiHuCard())
		{
			cbChiHuCard = getChiHuCard();
			setChiHuCard(0);
			continue;
		}

		cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kHandSkin,pCardNode);
		pNode->setTag(1);
		pNode->setZOrder(iOder);
		pNode->setPosition(kStartPos);
		{
			kStartPos += kHandAddPos;
		}
		if (iIdex == 0)
		{
			std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"MingImagic");
			setCardImagic(pNode,iCardValue,kImagic);
		}
		iOder+=iAddOder;
	}

	if (cbChiHuCard != 0)
	{
		cocos2d::Vec2 kChiHuPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(pCardNode,"JieSuanChiHuPos"));
		cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kHandSkin,pCardNode);
		pNode->setZOrder(iOder);
		pNode->setPosition(kStartPos+kChiHuPos);

		std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"MingImagic");
		setCardImagic(pNode,cbChiHuCard,kImagic);
	}
}

void NCMJPlayer::setChiHuCard(BYTE cbCard)
{
	m_cbChiHuCard = cbCard;
}
BYTE NCMJPlayer::getChiHuCard()
{
	return m_cbChiHuCard;
}
BYTE NCMJPlayer::getGangCard(BYTE currentCard)
{
	ASSERT(m_iIdex == 0);

	BYTE cardIndex[MAX_INDEX]={0};
	NCMJ::CGameLogic::Instance().SwitchToCardIndex(m_kHandCardData,m_kHandCardCout,cardIndex);
	bool bValue = NCMJ::CGameLogic::Instance().EstimateGangCard(cardIndex,currentCard);
	if (bValue)
	{
		return currentCard;
	}
	NCMJ::tagGangCardResult kRes;
	NCMJ::CGameLogic::Instance().AnalyseGangCard(cardIndex,m_kWeaveItemArray,m_kWeaveCount,kRes);

	if (kRes.cbCardCount>0)
	{
		return kRes.cbCardData[kRes.cbCardCount-1];
	}
	ASSERT(false);
	return 0;
}
