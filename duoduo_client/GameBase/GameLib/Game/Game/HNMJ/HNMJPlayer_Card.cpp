#include "HNMJPlayer.h"
#include "HNMJGameLogic.h"
#include "HNMJSoundFun.h"

void HNMJPlayer::setHandCard(BYTE* pCardData,int iCout)
{
	memcpy(m_kHandCardData,pCardData,sizeof(m_kHandCardData));
	m_kHandCardCout = iCout;
	HNMJ::CGameLogic::Instance().SortCardList(m_kHandCardData,m_kHandCardCout);
}
void HNMJPlayer::setWeaveItem(CMD_WeaveItem* pWeave,int iCout)
{
	memcpy(m_kWeaveItemArray,pWeave,sizeof(m_kWeaveItemArray));
	m_kWeaveCount = iCout;
}
void HNMJPlayer::setOperateResoult(CMD_S_OperateResult* pNetInfo)
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
	else if(pNetInfo->cbOperateCode == WIK_GANG || pNetInfo->cbOperateCode == WIK_CS_GANG)
	{
		runAniAnGang();
	}	
	else if(pNetInfo->cbOperateCode == WIK_BU_ZHANG)
	{
		runAniBu();
	}


	if ((pNetInfo->cbOperateCode&WIK_GANG)!=0 
		|| (pNetInfo->cbOperateCode&WIK_CS_GANG)!=0
		||(pNetInfo->cbOperateCode&WIK_BU_ZHANG)!=0)
	{
		//����˿�
		BYTE cbWeaveIndex=0xFF;
		for (BYTE i=0;i<m_kWeaveCount;i++)
		{
			DWORD cbWeaveKind=m_kWeaveItemArray[i].cbWeaveKind;
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
//		if (m_iIdex == 0 || m_bGameRecord|| m_bMingPai )
		{
			 HNMJ::CGameLogic::Instance().RemoveValueCardAll(m_kHandCardData,m_kHandCardCout,pNetInfo->cbOperateCard);
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
//		if (m_iIdex == 0 || m_bGameRecord || m_bMingPai)
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
			 HNMJ::CGameLogic::Instance().RemoveValueCard(m_kHandCardData,m_kHandCardCout,cbWeaveCard,2);
//			m_kHandCardCout -= 2;
		}
//		else
		{
			m_kHandCardCout = MAX_COUNT - m_kWeaveCount*3;
		}
	}
	showHandCard();
}

void HNMJPlayer::addNewInCard(int iCard)
{
	m_kHandCardData[m_kHandCardCout] = iCard;
	m_kHandCardCout++;
}
BYTE HNMJPlayer::getNewInCard()
{
	return m_kHandCardData[MAX_COUNT-1-m_kWeaveCount*3];
}

void HNMJPlayer::sendOutCard(int iCard)
{
	m_pHandOutCard = addHandOutCard(iCard);
	m_pHandOutCard->setVisible(false);
	WidgetFun::runWidgetAction(m_pHandOutCard,"OutAni");//�ȴ�2����ʾ

	setActOutCard(iCard);
}
void HNMJPlayer::showMJPoint()
{
	if (m_pHandOutCard)
	{
		WidgetFun::setPos(m_pSeatNode,"MJPoint",m_pHandOutCard->getPosition());
		WidgetFun::runWidgetAction(m_pSeatNode,"MJPoint","Start");
	}
}
void HNMJPlayer::setActOutCard(int iCard,bool bDel)
{
	cocos2d::Node* pNode = WidgetFun::getChildWidget(m_pSeatNode,"BigOutNode");
	pNode->setVisible(iCard >= 0);

	if (iCard < 0)
	{
		WidgetFun::runWidgetAction(m_pSeatNode,"MJPoint","End");
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
		showMJPoint();
	}

	HNMJSoundFun::playCardByGender(GetGender(),iCard);
	if (bDel)
	{
//		if (m_iIdex == 0 || m_bGameRecord || m_bMingPai)
		{
			HNMJ::CGameLogic::Instance().RemoveValueCardOne(m_kHandCardData,m_kHandCardCout,iCard);
			m_kHandCardCout --;
			HNMJ::CGameLogic::Instance().SortCardList(m_kHandCardData,m_kHandCardCout);
		}
// 		else
// 		{
// 			m_kHandCardCout -- ;
// 		}
	}

	showHandCard();

}
cocos2d::Node* HNMJPlayer::addHandOutCard(int iCard)
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
	pNode->setLocalZOrder(iAddOder*nLength);
	pNode->setPosition(kPos);
	std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");
	setCardImagic(pNode,iCard,kImagic);
	m_pHandOutCard = pNode;
	m_kOutCardList.push_back(OutCardInfo(pNode,iCard));
	return pNode;
}
void HNMJPlayer::showHandCard()
{
	showHandCard(m_kWeaveItemArray,m_kWeaveCount,m_kHandCardData,m_kHandCardCout);
	setHasCSGang();
}
void HNMJPlayer::showHandCard(CMD_WeaveItem* pWeave,int iWeaveCout,BYTE* pHandCard,int iHandCout)
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

	cocos2d::Vec2 kWeaveAddPos =  kHandAddPos*3*0.8f;

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
			pNode->setLocalZOrder(iOder);
			pNode->setPosition(kStartPos);
			kStartPos += kWeaveAddPos;
			std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card0"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card1"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card2"),iCardValue,kImagic);
		}
		if (pTemp->cbWeaveKind == WIK_GANG || pTemp->cbWeaveKind == WIK_CS_GANG||pTemp->cbWeaveKind == WIK_BU_ZHANG)
		{
			cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kGangSkin,pCardNode);
			pNode->setLocalZOrder(iOder);
			pNode->setPosition(kStartPos);
			kStartPos += kWeaveAddPos;
			std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card0"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card1"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card2"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card3"),pTemp->cbCenterCard,kImagic);
		}
		if (pTemp->cbWeaveKind == WIK_LEFT)
		{
			cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kPengSkin,pCardNode);
			pNode->setLocalZOrder(iOder);
			pNode->setPosition(kStartPos);
			kStartPos += kWeaveAddPos;
			std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card0"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card1"),iCardValue+1,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card2"),iCardValue+2,kImagic);
		}
		if (pTemp->cbWeaveKind == WIK_CENTER)
		{
			cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kPengSkin,pCardNode);
			pNode->setLocalZOrder(iOder);
			pNode->setPosition(kStartPos);
			kStartPos += kWeaveAddPos;
			std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card0"),iCardValue-1,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card1"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card2"),iCardValue+1,kImagic);
		}
		if (pTemp->cbWeaveKind == WIK_RIGHT)
		{
			cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kPengSkin,pCardNode);
			pNode->setLocalZOrder(iOder);
			pNode->setPosition(kStartPos);
			kStartPos += kWeaveAddPos;
			std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card0"),iCardValue-2,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card1"),iCardValue-1,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card2"),iCardValue,kImagic);
		}
		iOder+=iAddOder;
	}
	for (int i = 0;i<iHandCout;i++)
	{
		BYTE cbTemp = *(pHandCard+i);
		cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kHandSkin,pCardNode);
		pNode->setTag(1);
		pNode->setLocalZOrder(iOder);
		pNode->setPosition(kStartPos);
		if (bHaveNewIn && i == (iHandCout - 2))
		{
			kStartPos += kNewInAddPos;
		}
		else
		{
			kStartPos += kHandAddPos;
		}
		int iCardValue = cbTemp;

		if (m_iIdex == 0)
		{
			std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");

			setCardImagic(pNode,iCardValue,kImagic);
		}
		else if (m_bMingPai|| m_bGameRecord)
		{
			std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"MingImagic");
			float fHuScale = utility::parseFloat(WidgetFun::getWidgetUserInfo(pNode,"HuScale"));
			pNode->setScale(fHuScale);
			setCardImagic(pNode,iCardValue,kImagic);
		}
		iOder+=iAddOder;
	}
}
void HNMJPlayer::setCardImagic(cocos2d::Node* pNode,int kValue,std::string kImagicFront)
{
	if (kValue > 0)
	{
		BYTE cbValue= HNMJ::CGameLogic::Instance().GetCardValue(kValue);
		BYTE cbColor=HNMJ::CGameLogic::Instance().GetCardColor(kValue);
		WidgetFun::setImagic(pNode,utility::toString(kImagicFront,(int)cbColor,(int)cbValue,".png"),false);
	}
	else
	{
		WidgetFun::setImagic(pNode,utility::toString(kImagicFront,0,0,".png"),false);
	}
}
cocos2d::Node* HNMJPlayer::getTouchCardNode(cocos2d::Vec2 kTouchPos)
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

BYTE HNMJPlayer::getTouchCardVlaue(cocos2d::Node* pNode)
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
void HNMJPlayer::showHuPai(bool bZiMo)
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
void HNMJPlayer::removeHandOutCard( BYTE cbCardData )
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

void HNMJPlayer::showCard()
{
	showCard(m_kHandCardData,m_kHandCardCout);
}
void HNMJPlayer::showCard(BYTE* cbCardData,BYTE cbCardCount)
{
	BYTE cbIdex = 0;
	BYTE cbChiHuCard = 0;

	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode1");
	for (int i = 0;i<pRootNode->getChildrenCount();i++)
	{
		cocos2d::Sprite* pSprite = dynamic_cast<cocos2d::Sprite*>(pRootNode->getChildren().at(i));
		if (pSprite == NULL || pSprite->getTag() != 1)
		{
			continue;
		}
		BYTE iCardValue = *(cbCardData+cbIdex);
		if (iCardValue == getChiHuCard() && cbChiHuCard == 0)
		{
			cbChiHuCard = getChiHuCard();
		}
		if (cbChiHuCard != 0 && cbIdex==cbCardCount-1)
		{
			iCardValue = cbChiHuCard;
		}
		else if(cbChiHuCard != 0)
		{
			iCardValue = *(cbCardData+cbIdex+1);
		}
		float fHuScale = utility::parseFloat(WidgetFun::getWidgetUserInfo(pSprite,"HuScale"));
		pSprite->setScale(fHuScale);
		std::string kImagic = WidgetFun::getWidgetUserInfo(pSprite,"MingImagic");
		if (cbIdex < cbCardCount)
		{
			setCardImagic(pSprite,iCardValue,kImagic);
		}
		else
		{
			setCardImagic(pSprite,0,kImagic);
		}
		cbIdex++;
	}
}

void HNMJPlayer::showJieSuanCard(cocos2d::Node* pCardNode,CMD_WeaveItem* pWeave,int iWeaveCout,BYTE* pHandCard,int iHandCout
								 ,BYTE* pChiHuCard)
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
			pNode->setLocalZOrder(iOder);
			pNode->setPosition(kStartPos);
			kStartPos += kHandAddPos*3 + kJieSuanWeaveGap;
			std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card0"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card1"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card2"),iCardValue,kImagic);
		}
		if (pTemp->cbWeaveKind == WIK_GANG||pTemp->cbWeaveKind == WIK_CS_GANG||pTemp->cbWeaveKind == WIK_BU_ZHANG)
		{
			cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kGangSkin,pCardNode);
			pNode->setLocalZOrder(iOder);
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
			pNode->setLocalZOrder(iOder);
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
			pNode->setLocalZOrder(iOder);
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
			pNode->setLocalZOrder(iOder);
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
		if (iCardValue == getChiHuCard() && cbChiHuCard == 0)
		{
			cbChiHuCard = getChiHuCard();
			continue;
		}

		cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kHandSkin,pCardNode);
		pNode->setTag(1);
		pNode->setLocalZOrder(iOder);
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
		pNode->setLocalZOrder(iOder);
		pNode->setPosition(kStartPos+kChiHuPos);

		std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"MingImagic");
		setCardImagic(pNode,cbChiHuCard,kImagic);
	}
}

BYTE HNMJPlayer::getChiHuCard()
{
	return m_cbChiHuCard;
}

bool HNMJPlayer::isGangTing(int iAction)
{
	BYTE cbCardIndex[MAX_INDEX]={0};
	HNMJ::CGameLogic::Instance().SwitchToCardIndex(m_kHandCardData,m_kHandCardCout,cbCardIndex);
	return HNMJ::CGameLogic::Instance().isGangTingCS(cbCardIndex,m_kWeaveItemArray,m_kWeaveCount,iAction);
}

void HNMJPlayer::showTingCard()
{
	if (m_iIdex != 0)return;

	std::vector<BYTE> kTingCardList;
	BYTE cbCardIndex[MAX_INDEX]={0};

	HNMJ::CGameLogic::Instance().SwitchToCardIndex(m_kHandCardData,m_kHandCardCout,cbCardIndex);
	HNMJ::CGameLogic::Instance().getTingCardHZ(cbCardIndex,m_kWeaveItemArray,m_kWeaveCount,kTingCardList);

	int nLength = kTingCardList.size();
	cocos2d::Node* pNode = WidgetFun::getChildWidget(m_pSeatNode,"TingNode");
	if (nLength>0)
	{
		pNode->setVisible(true);
		showTingCard(pNode,kTingCardList);
	}
	else
	{
		pNode->setVisible(false);
	}

}
void HNMJPlayer::showTingCard(cocos2d::Node* pNode,const std::vector<BYTE>& kTingCardList )
{
	ASSERT(pNode);
	cocos2d::Node* pTingCardNode = WidgetFun::getChildWidget(pNode,"TingCardNode");
	cocos2d::Vec2 kTingAddPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(pNode,"TingAddPos"));
	cocos2d::Vec2 kStartPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(pNode,"TimgStartPos"));
	cocos2d::Size kCellSize = utility::parseSize(WidgetFun::getWidgetUserInfo(pNode,"CellSize"));
	std::string kTingCardSkin = "HNMJ_HAND_0";

	pTingCardNode->removeAllChildren();
	for (int i = 0;i<(int)kTingCardList.size();i++)
	{
		cocos2d::Node* pCard = WidgetManager::Instance().createWidget(kTingCardSkin,pTingCardNode);
		pCard->setTag(1);
		pCard->setLocalZOrder(i);
		pCard->setPosition(kStartPos);
		kStartPos += kTingAddPos;
		
		int iCardValue = kTingCardList[i];
		std::string kImagic = WidgetFun::getWidgetUserInfo(pCard,"Imagic");
		setCardImagic(pCard,iCardValue,kImagic);
	}
	WidgetFun::set9Size(pNode,"TingBg",cocos2d::Size(kCellSize.width*(kTingCardList.size()+2),kCellSize.height));
}


void HNMJPlayer::showCSGangCard(int nCard1,int nCard2,bool bOut)
{
	ASSERT(nCard1>0&& nCard2>0);
	if (nCard1<=0|| nCard2<=0)
	{
		return;
	}
	WidgetFun::setVisible(m_pSeatNode,"CSGangCardBg",true);
	cocos2d::Node* pCard1 = WidgetFun::getChildWidget(m_pSeatNode,"CSGangCard1");
	cocos2d::Node* pCard2 = WidgetFun::getChildWidget(m_pSeatNode,"CSGangCard2");

	std::string kImagic = WidgetFun::getWidgetUserInfo(pCard1,"Imagic");
	setCardImagic(pCard1,nCard1,kImagic);
	setCardImagic(pCard2,nCard2,kImagic);

	if (bOut)
	{
		cocos2d::Node*	pHandOutCard1 = addHandOutCard(nCard1);
		cocos2d::Node*	pHandOutCard2 = addHandOutCard(nCard2);
		pHandOutCard1->setVisible(false);
		pHandOutCard2->setVisible(false);
		WidgetFun::runWidgetAction(pHandOutCard1,"OutAni");//�ȴ�2����ʾ
		WidgetFun::runWidgetAction(pHandOutCard2,"OutAni");//�ȴ�2����ʾ

		WidgetFun::runWidgetAction(m_pSeatNode,"CSGangCardBg","HideAni");
	}
}

void HNMJPlayer::blackAllHandCard( bool bBlack )
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
		if (bBlack)
		{
			pSprite->setColor(cocos2d::Color3B::YELLOW);
		}
		else
		{
			pSprite->setColor(cocos2d::Color3B::WHITE);
		}
		iIdex++;
	}
}
