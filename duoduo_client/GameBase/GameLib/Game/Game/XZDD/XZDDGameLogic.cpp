#include "XZDDGameLogic.h"

//////////////////////////////////////////////////////////////////////////
FV_SINGLETON_STORAGE(XZDD::CGameLogic);
namespace XZDD
{
	//��̬����
	bool		CChiHuRight::m_bInit = false;
	dword		CChiHuRight::m_dwRightMask[MAX_RIGHT_COUNT];

	//���캯��
	CChiHuRight::CChiHuRight()
	{
		zeromemory( m_dwRight,sizeof(m_dwRight) );

		if( !m_bInit )
		{
			m_bInit = true;
			for( BYTE i = 0; i < CountArray(m_dwRightMask); i++ )
			{
				if( 0 == i )
					m_dwRightMask[i] = 0;
				else
					m_dwRightMask[i] = (dword(pow((float)2,(float)(i-1))))<<28;
			}
		}
	}

	//��ֵ������
	CChiHuRight & CChiHuRight::operator = ( dword dwRight )
	{
		dword dwOtherRight = 0;
		//��֤Ȩλ
		if( !IsValidRight( dwRight ) )
		{
			//��֤ȡ��Ȩλ
			ASSERT( IsValidRight( ~dwRight ) );
			if( !IsValidRight( ~dwRight ) ) return *this;
			dwRight = ~dwRight;
			dwOtherRight = MASK_CHI_HU_RIGHT;
		}

		for( BYTE i = 0; i < CountArray(m_dwRightMask); i++ )
		{
			if( (dwRight&m_dwRightMask[i]) || (i==0&&dwRight<0x10000000) )
				m_dwRight[i] = dwRight&MASK_CHI_HU_RIGHT;
			else m_dwRight[i] = dwOtherRight;
		}

		return *this;
	}

	//�����
	CChiHuRight & CChiHuRight::operator &= ( dword dwRight )
	{
		bool bNavigate = false;
		//��֤Ȩλ
		if( !IsValidRight( dwRight ) )
		{
			//��֤ȡ��Ȩλ
			ASSERT( IsValidRight( ~dwRight ) );
			if( !IsValidRight( ~dwRight ) ) return *this;
			//����Ȩλ
			dword dwHeadRight = (~dwRight)&0xF0000000;
			dword dwTailRight = dwRight&MASK_CHI_HU_RIGHT;
			dwRight = dwHeadRight|dwTailRight;
			bNavigate = true;
		}

		for( BYTE i = 0; i < CountArray(m_dwRightMask); i++ )
		{
			if( (dwRight&m_dwRightMask[i]) || (i==0&&dwRight<0x10000000) )
			{
				m_dwRight[i] &= (dwRight&MASK_CHI_HU_RIGHT);
			}
			else if( !bNavigate )
				m_dwRight[i] = 0;
		}

		return *this;
	}

	//�����
	CChiHuRight & CChiHuRight::operator |= ( dword dwRight )
	{
		//��֤Ȩλ
		if( !IsValidRight( dwRight ) ) return *this;

		for( BYTE i = 0; i < CountArray(m_dwRightMask); i++ )
		{
			if( (dwRight&m_dwRightMask[i]) || (i==0&&dwRight<0x10000000) )
				m_dwRight[i] |= (dwRight&MASK_CHI_HU_RIGHT);
		}

		return *this;
	}

	//��
	CChiHuRight CChiHuRight::operator & ( dword dwRight )
	{
		CChiHuRight chr = *this;
		return (chr &= dwRight);
	}

	//��
	CChiHuRight CChiHuRight::operator & ( dword dwRight ) const
	{
		CChiHuRight chr = *this;
		return (chr &= dwRight);
	}

	//��
	CChiHuRight CChiHuRight::operator | ( dword dwRight )
	{
		CChiHuRight chr = *this;
		return chr |= dwRight;
	}

	//��
	CChiHuRight CChiHuRight::operator | ( dword dwRight ) const
	{
		CChiHuRight chr = *this;
		return chr |= dwRight;
	}

	//�Ƿ�ȨλΪ��
	bool CChiHuRight::IsEmpty()
	{
		for( BYTE i = 0; i < CountArray(m_dwRight); i++ )
			if( m_dwRight[i] ) return false;
		return true;
	}

	//����ȨλΪ��
	void CChiHuRight::SetEmpty()
	{
		zeromemory( m_dwRight,sizeof(m_dwRight) );
		return;
	}

	//��ȡȨλ��ֵ
	BYTE CChiHuRight::GetRightData( dword dwRight[], BYTE cbMaxCount )
	{
		ASSERT( cbMaxCount >= CountArray(m_dwRight) );
		if( cbMaxCount < CountArray(m_dwRight) ) return 0;

		memcpy( dwRight,m_dwRight,sizeof(dword)*CountArray(m_dwRight) );
		return CountArray(m_dwRight);
	}

	//����Ȩλ��ֵ
	bool CChiHuRight::SetRightData( const dword dwRight[], BYTE cbRightCount )
	{
		ASSERT( cbRightCount <= CountArray(m_dwRight) );
		if( cbRightCount > CountArray(m_dwRight) ) return false;

		zeromemory( m_dwRight,sizeof(m_dwRight) );
		memcpy( m_dwRight,dwRight,sizeof(dword)*cbRightCount );

		return true;
	}

	//����λ�Ƿ���ȷ
	bool CChiHuRight::IsValidRight( dword dwRight )
	{
		dword dwRightHead = dwRight & 0xF0000000;
		for( BYTE i = 0; i < CountArray(m_dwRightMask); i++ )
			if( m_dwRightMask[i] == dwRightHead ) return true;
		return false;
	}

	//////////////////////////////////////////////////////////////////////////



	//////////////////////////////////////////////////////////////////////////
	//��̬����

	//�˿�����
	const BYTE CGameLogic::m_cbCardDataArray[MAX_REPERTORY]=
	{
		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
	};

	//////////////////////////////////////////////////////////////////////////

	//���캯��
	CGameLogic::CGameLogic()
	{
		m_cbMagicIndex = MAX_INDEX;
	}

	//��������
	CGameLogic::~CGameLogic()
	{
	}

	//�����˿�
	void CGameLogic::RandCardData(BYTE cbCardData[], BYTE cbMaxCount)
	{
		//����׼��
		BYTE cbCardDataTemp[CountArray(m_cbCardDataArray)];
		memcpy(cbCardDataTemp,m_cbCardDataArray,sizeof(m_cbCardDataArray));

		//�����˿�
		BYTE cbRandCount=0,cbPosition=0;
		do
		{
			cbPosition=rand()%(cbMaxCount-cbRandCount);
			cbCardData[cbRandCount++]=cbCardDataTemp[cbPosition];
			cbCardDataTemp[cbPosition]=cbCardDataTemp[cbMaxCount-cbRandCount];
		} while (cbRandCount<cbMaxCount);

		return;
	}

	//ɾ���˿�
	bool CGameLogic::RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard)
	{
		//Ч���˿�
		ASSERT(IsValidCard(cbRemoveCard));
		ASSERT(cbCardIndex[SwitchToCardIndex(cbRemoveCard)]>0);

		//ɾ���˿�
		BYTE cbRemoveIndex=SwitchToCardIndex(cbRemoveCard);
		if (cbCardIndex[cbRemoveIndex]>0)
		{
			cbCardIndex[cbRemoveIndex]--;
			return true;
		}

		//ʧ��Ч��
		ASSERT(FALSE);

		return false;
	}

	//ɾ���˿�
	bool CGameLogic::RemoveCard(BYTE cbCardIndex[MAX_INDEX], const BYTE cbRemoveCard[], BYTE cbRemoveCount)
	{
		//ɾ���˿�
		for (BYTE i=0;i<cbRemoveCount;i++)
		{
			//Ч���˿�
			ASSERT(IsValidCard(cbRemoveCard[i]));
			ASSERT(cbCardIndex[SwitchToCardIndex(cbRemoveCard[i])]>0);

			//ɾ���˿�
			BYTE cbRemoveIndex=SwitchToCardIndex(cbRemoveCard[i]);
			if (cbCardIndex[cbRemoveIndex]==0)
			{
				//�������
				ASSERT(FALSE);

				//��ԭɾ��
				for (BYTE j=0;j<i;j++) 
				{
					ASSERT(IsValidCard(cbRemoveCard[j]));
					cbCardIndex[SwitchToCardIndex(cbRemoveCard[j])]++;
				}

				return false;
			}
			else 
			{
				//ɾ���˿�
				--cbCardIndex[cbRemoveIndex];
			}
		}

		return true;
	}

	BYTE CGameLogic::RemoveValueCardAll(BYTE cbCardData[], BYTE cbCardCount,BYTE cbRemoveCard)
	{
		BYTE cbCardIndex[MAX_INDEX];			//�����˿�
		SwitchToCardIndex(cbCardData,cbCardCount,cbCardIndex);
		BYTE cbRemoveCardArray[MAX_INDEX];
		BYTE cbRemoveCout = cbCardIndex[SwitchToCardIndex(cbRemoveCard)];
		for (int i = 0;i<cbRemoveCout;i++)
		{
			cbRemoveCardArray[i] = cbRemoveCard;
		}
		RemoveValueCard(cbCardData,cbCardCount,cbRemoveCardArray,cbRemoveCout);
		return cbCardCount - cbRemoveCout;
	}
	bool CGameLogic::RemoveValueCardOne(BYTE cbCardData[], BYTE cbCardCount,BYTE cbRemoveCard)
	{
		BYTE cbRemoveCardArray[MAX_INDEX];
		cbRemoveCardArray[0] = cbRemoveCard;
		return RemoveValueCard(cbCardData,cbCardCount,cbRemoveCardArray,1);
	}
	//ɾ���˿�
	bool CGameLogic::RemoveValueCard(BYTE cbCardData[], BYTE cbCardCount, const BYTE cbRemoveCard[], BYTE cbRemoveCount)
	{
		//��������
		ASSERT(cbCardCount<=14);
		ASSERT(cbRemoveCount<=cbCardCount);

		//�������
		BYTE cbDeleteCount=0,cbTempCardData[14];
		if (cbCardCount>CountArray(cbTempCardData))
			return false;
		memcpy(cbTempCardData,cbCardData,cbCardCount*sizeof(cbCardData[0]));

		//�����˿�
		for (BYTE i=0;i<cbRemoveCount;i++)
		{
			for (BYTE j=0;j<cbCardCount;j++)
			{
				if (cbRemoveCard[i]==cbTempCardData[j])
				{
					cbDeleteCount++;
					cbTempCardData[j]=0;
					break;
				}
			}
		}

		//�ɹ��ж�
		if (cbDeleteCount!=cbRemoveCount) 
		{
			ASSERT(FALSE);
			return false;
		}

		//�����˿�
		BYTE cbCardPos=0;
		for (BYTE i=0;i<cbCardCount;i++)
		{
			if (cbTempCardData[i]!=0) 
				cbCardData[cbCardPos++]=cbTempCardData[i];
		}

		return true;
	}

	bool CGameLogic::AddCard(BYTE cbCardIndex[MAX_INDEX], const BYTE cbAddCard[], BYTE cbAddCount)
	{

		//����˿�
		for (BYTE i=0;i<cbAddCount;i++)
		{
			//Ч���˿�
			ASSERT(IsValidCard(cbAddCard[i]));
			ASSERT(cbCardIndex[SwitchToCardIndex(cbAddCard[i])]<4);

			//ɾ���˿�
			BYTE cbAddIndex=SwitchToCardIndex(cbAddCard[i]);
			if (cbCardIndex[cbAddIndex]<4)
			{
				//ɾ���˿�
				++cbCardIndex[cbAddIndex];
			}
			else 
			{
				//�������
				ASSERT(FALSE);
				//��ԭ���
				for (BYTE j=0;j<i;j++) 
				{
					ASSERT(IsValidCard(cbAddCard[j]));
					cbCardIndex[SwitchToCardIndex(cbAddCard[j])]--;
				}
				return false;
			}
		}
		return true;
	}

	bool CGameLogic::ReplaceCardData(BYTE cbCardData[], BYTE cbCardCount, const BYTE cbAddCardData[], BYTE cbAddCount)
	{
		BYTE cbCardPos=cbCardCount-cbAddCount;
		for (BYTE i=0;i<cbAddCount;i++)
		{
			cbCardData[cbCardPos]=cbAddCardData[i];
			cbCardPos++;
		}
		return true;
	}

	//��Ч�ж�
	bool CGameLogic::IsValidCard(BYTE cbCardData)
	{
		BYTE cbValue=(cbCardData&MASK_VALUE);
		BYTE cbColor=(cbCardData&MASK_COLOR)>>4;
		return (((cbValue>=1)&&(cbValue<=9)&&(cbColor<=2))||((cbValue>=1)&&(cbValue<=7)&&(cbColor==3)));
	}

	//�˿���Ŀ
	BYTE CGameLogic::GetCardCount(const BYTE cbCardIndex[MAX_INDEX])
	{
		//��Ŀͳ��
		BYTE cbCardCount=0;
		for (BYTE i=0;i<MAX_INDEX;i++) 
			cbCardCount+=cbCardIndex[i];

		return cbCardCount;
	}

	//��ȡ���
	BYTE CGameLogic::GetWeaveCard(BYTE cbWeaveKind, BYTE cbCenterCard, BYTE cbCardBuffer[4])
	{
		//����˿�
		switch (cbWeaveKind)
		{
		case WIK_LEFT:		//���Ʋ���
			{
				//���ñ���
				cbCardBuffer[0]=cbCenterCard;
				cbCardBuffer[1]=cbCenterCard+1;
				cbCardBuffer[2]=cbCenterCard+2;

				return 3;
			}
		case WIK_RIGHT:		//���Ʋ���
			{
				//���ñ���
				cbCardBuffer[0]=cbCenterCard;
				cbCardBuffer[1]=cbCenterCard-1;
				cbCardBuffer[2]=cbCenterCard-2;

				return 3;
			}
		case WIK_CENTER:	//���Ʋ���
			{
				//���ñ���
				cbCardBuffer[0]=cbCenterCard;
				cbCardBuffer[1]=cbCenterCard-1;
				cbCardBuffer[2]=cbCenterCard+1;

				return 3;
			}
		case WIK_PENG:		//���Ʋ���
			{
				//���ñ���
				cbCardBuffer[0]=cbCenterCard;
				cbCardBuffer[1]=cbCenterCard;
				cbCardBuffer[2]=cbCenterCard;

				return 3;
			}
		case WIK_GANG:		//���Ʋ���
			{
				//���ñ���
				cbCardBuffer[0]=cbCenterCard;
				cbCardBuffer[1]=cbCenterCard;
				cbCardBuffer[2]=cbCenterCard;
				cbCardBuffer[3]=cbCenterCard;

				return 4;
			}
		default:
			{
				ASSERT(FALSE);
			}
		}

		return 0;
	}

	//�����ȼ�
	BYTE CGameLogic::GetUserActionRank(BYTE cbUserAction)
	{
		//���Ƶȼ�
		if (cbUserAction&WIK_CHI_HU) { return 4; }

		//���Ƶȼ�
		if (cbUserAction&WIK_GANG) { return 3; }

		//���Ƶȼ�
		if (cbUserAction&WIK_PENG) { return 2; }

		//���Ƶȼ�
		if (cbUserAction&(WIK_RIGHT|WIK_CENTER|WIK_LEFT)) { return 1; }

		return 0;
	}

	//���Ƶȼ�
	WORD CGameLogic::GetChiHuActionRank(const CChiHuRight & ChiHuRight)
	{
		WORD wFanShu = 0;

		if( !(ChiHuRight&CHR_QING_LONG_QI_DUI).IsEmpty() )
			wFanShu = 32;
		else if( !(ChiHuRight&CHR_DI_HU).IsEmpty() )
			wFanShu = 32;
		else if( !(ChiHuRight&CHR_TIAN_HU).IsEmpty() )
			wFanShu = 32;
		else if( !(ChiHuRight&CHR_LONG_QI_DUI).IsEmpty() )
			wFanShu = 16;
		else if( !(ChiHuRight&CHR_QING_QI_DUI).IsEmpty() )
			wFanShu = 16;
		else if( !(ChiHuRight&CHR_QING_YAO_JIU).IsEmpty() )
			wFanShu = 16;
		else if( !(ChiHuRight&CHR_QING_DUI).IsEmpty() )
			wFanShu = 8;
		else if( !(ChiHuRight&CHR_JIANG_DUI).IsEmpty() )
			wFanShu = 8;
		else if( !(ChiHuRight&CHR_QING_YI_SE).IsEmpty() )
			wFanShu = 4;
		else if( !(ChiHuRight&CHR_DAI_YAO).IsEmpty() )
			wFanShu = 4;
		else if( !(ChiHuRight&CHR_QI_XIAO_DUI).IsEmpty() )
			wFanShu = 4;
		else if( !(ChiHuRight&CHR_DA_DUI_ZI).IsEmpty() )
			wFanShu = 2;
		else if( !(ChiHuRight&CHR_SHU_FAN).IsEmpty() )
			wFanShu = 1;
		ASSERT( wFanShu > 0 );

		//���ϻ�
		if( !(ChiHuRight&CHR_GANG_KAI).IsEmpty() )
			wFanShu *= 2;
		//������
		if( !(ChiHuRight&CHR_GANG_SHANG_PAO).IsEmpty() )
			wFanShu *= 2;


		return wFanShu;
	}

	//�����ж�
	BYTE CGameLogic::EstimateEatCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
	{
		//����Ч��
		ASSERT(IsValidCard(cbCurrentCard));

		//�����ж�
		if ( cbCurrentCard>=0x31 || IsMagicCard(cbCurrentCard) ) 
			return WIK_NULL;

		//��������
		BYTE cbExcursion[3]={0,1,2};
		BYTE cbItemKind[3]={WIK_LEFT,WIK_CENTER,WIK_RIGHT};

		//�����ж�
		BYTE cbEatKind=0,cbFirstIndex=0;
		BYTE cbCurrentIndex=SwitchToCardIndex(cbCurrentCard);
		for (BYTE i=0;i<CountArray(cbItemKind);i++)
		{
			BYTE cbValueIndex=cbCurrentIndex%9;
			if ((cbValueIndex>=cbExcursion[i])&&((cbValueIndex-cbExcursion[i])<=6))
			{
				//�����ж�
				cbFirstIndex=cbCurrentIndex-cbExcursion[i];

				//���Ʋ��ܰ���������
				if( m_cbMagicIndex != MAX_INDEX &&
					m_cbMagicIndex >= cbFirstIndex && m_cbMagicIndex <= cbFirstIndex+2 ) continue;

				if ((cbCurrentIndex!=cbFirstIndex)&&(cbCardIndex[cbFirstIndex]==0))
					continue;
				if ((cbCurrentIndex!=(cbFirstIndex+1))&&(cbCardIndex[cbFirstIndex+1]==0))
					continue;
				if ((cbCurrentIndex!=(cbFirstIndex+2))&&(cbCardIndex[cbFirstIndex+2]==0))
					continue;

				//��������
				cbEatKind|=cbItemKind[i];
			}
		}

		return cbEatKind;
	}

	//�����ж�
	BYTE CGameLogic::EstimatePengCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
	{
		//����Ч��
		ASSERT(IsValidCard(cbCurrentCard));

		//�����ж�
		if ( IsMagicCard(cbCurrentCard) ) 
			return WIK_NULL;

		//�����ж�
		return (cbCardIndex[SwitchToCardIndex(cbCurrentCard)]>=2)?WIK_PENG:WIK_NULL;
	}

	//�����ж�
	BYTE CGameLogic::EstimateGangCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
	{
		//����Ч��
		ASSERT(IsValidCard(cbCurrentCard));

		//�����ж�
		if ( IsMagicCard(cbCurrentCard) ) 
			return WIK_NULL;

		//�����ж�
		return (cbCardIndex[SwitchToCardIndex(cbCurrentCard)]==3)?WIK_GANG:WIK_NULL;
	}

	//���Ʒ���
	BYTE CGameLogic::AnalyseGangCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, tagGangCardResult & GangCardResult)
	{
		//���ñ���
		BYTE cbActionMask=WIK_NULL;
		zeromemory(&GangCardResult,sizeof(GangCardResult));

		//���ϸ���
		for (BYTE i=0;i<MAX_INDEX;i++)
		{
			if( i == m_cbMagicIndex ) continue;
			if (cbCardIndex[i]==4)
			{
				cbActionMask|=WIK_GANG;
				GangCardResult.cbCardData[GangCardResult.cbCardCount++]=SwitchToCardData(i);
			}
		}

		//��ϸ���
		for (BYTE i=0;i<cbWeaveCount;i++)
		{
			if (WeaveItem[i].cbWeaveKind==WIK_PENG)
			{
				if (cbCardIndex[SwitchToCardIndex(WeaveItem[i].cbCenterCard)]==1)
				{
					cbActionMask|=WIK_GANG;
					GangCardResult.cbCardData[GangCardResult.cbCardCount++]=WeaveItem[i].cbCenterCard;
				}
			}
		}

		return cbActionMask;
	}
	 
	BYTE CGameLogic::AnalyseGangCard(const  BYTE cbCardIndex[MAX_INDEX], const CMD_WeaveItem WeaveItem[], BYTE cbWeaveCount, tagGangCardResult & GangCardResult )
	{
		//���ñ���
		BYTE cbActionMask=WIK_NULL;
		zeromemory(&GangCardResult,sizeof(GangCardResult));

		//���ϸ���
		for (BYTE i=0;i<MAX_INDEX;i++)
		{
			if( i == m_cbMagicIndex ) continue;
			if (cbCardIndex[i]==4)
			{
				cbActionMask|=WIK_GANG;
				GangCardResult.cbCardData[GangCardResult.cbCardCount++]=SwitchToCardData(i);
			}
		}

		//��ϸ���
		for (BYTE i=0;i<cbWeaveCount;i++)
		{
			if (WeaveItem[i].cbWeaveKind==WIK_PENG)
			{
				if (cbCardIndex[SwitchToCardIndex(WeaveItem[i].cbCenterCard)]==1)
				{
					cbActionMask|=WIK_GANG;
					GangCardResult.cbCardData[GangCardResult.cbCardCount++]=WeaveItem[i].cbCenterCard;
				}
			}
		}

		return cbActionMask;
	}


	//�˿�ת��
	BYTE CGameLogic::SwitchToCardData(BYTE cbCardIndex)
	{
		ASSERT(cbCardIndex<34);
		return ((cbCardIndex/9)<<4)|(cbCardIndex%9+1);
	}

	//�˿�ת��
	BYTE CGameLogic::SwitchToCardIndex(BYTE cbCardData)
	{
		ASSERT(IsValidCard(cbCardData));
		return ((cbCardData&MASK_COLOR)>>4)*9+(cbCardData&MASK_VALUE)-1;
	}

	//�˿�ת��
	BYTE CGameLogic::SwitchToCardData(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[MAX_COUNT])
	{
		//ת���˿�
		BYTE cbPosition=0;
		//����
		if( m_cbMagicIndex != MAX_INDEX )
		{
			for( BYTE i = 0; i < cbCardIndex[m_cbMagicIndex]; i++ )
				cbCardData[cbPosition++] = SwitchToCardData(m_cbMagicIndex);
		}
		for (BYTE i=0;i<MAX_INDEX;i++)
		{
			if( i == m_cbMagicIndex ) continue;
			if (cbCardIndex[i]!=0)
			{
				for (BYTE j=0;j<cbCardIndex[i];j++)
				{
					ASSERT(cbPosition<MAX_COUNT);
					cbCardData[cbPosition++]=SwitchToCardData(i);
				}
			}
		}

		return cbPosition;
	}

	//�˿�ת��
	BYTE CGameLogic::SwitchToCardIndex(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbCardIndex[MAX_INDEX])
	{
		//���ñ���
		zeromemory(cbCardIndex,sizeof(BYTE)*MAX_INDEX);

		//ת���˿�
		for (BYTE i=0;i<cbCardCount;i++)
		{
			ASSERT(IsValidCard(cbCardData[i]));
			cbCardIndex[SwitchToCardIndex(cbCardData[i])]++;
		}

		return cbCardCount;
	}

	//����
	bool CGameLogic::IsMagicCard( BYTE cbCardData )
	{
		if( m_cbMagicIndex != MAX_INDEX )
			return SwitchToCardIndex(cbCardData) == m_cbMagicIndex;
		return false;
	}

	//����,������ֵ����
	bool CGameLogic::SortCardList( BYTE cbCardData[MAX_COUNT], BYTE cbCardCount )
	{
		//��Ŀ����
		if (cbCardCount==0||cbCardCount>MAX_COUNT) return false;

		//�������
		bool bSorted=true;
		BYTE cbSwitchData=0,cbLast=cbCardCount-1;
		do
		{
			bSorted=true;
			for (BYTE i=0;i<cbLast;i++)
			{
				if (cbCardData[i]>cbCardData[i+1])
				{
					//���ñ�־
					bSorted=false;

					//�˿�����
					cbSwitchData=cbCardData[i];
					cbCardData[i]=cbCardData[i+1];
					cbCardData[i+1]=cbSwitchData;
				}	
			}
			cbLast--;
		} while(bSorted==false);

		return true;
	}

	void CGameLogic::GetCardWithColor(BYTE cbCardData[MAX_COUNT],BYTE cbCardCount,std::vector<BYTE>& CardList1,std::vector<BYTE>& CardList2,std::vector<BYTE>& CardList3)
	{
		//��Ŀ����
		if (cbCardCount==0||cbCardCount>MAX_COUNT) return;
		for(int i=0;i<cbCardCount;i++)
		{
			BYTE cbColor=(((BYTE)cbCardData[i]&MASK_COLOR)>>4)+1;
			if (cbColor== 1 )
			{
				CardList1.push_back(cbCardData[i]);
			}
			else if (cbColor == 2 )
			{
				CardList2.push_back(cbCardData[i]);
			}
			else if (cbColor == 3 )
			{
				CardList3.push_back(cbCardData[i]);
			}
		}
	}

	/*
	// ������������
	*/

	//�����
	bool CGameLogic::IsPengPeng( const tagAnalyseItem *pAnalyseItem )
	{
		for( BYTE i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++ )
		{
			if( pAnalyseItem->cbWeaveKind[i]&(WIK_LEFT|WIK_CENTER|WIK_RIGHT) )
				return false;
		}
		return true;
	}

	//��һɫ��
	bool CGameLogic::IsQingYiSe(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbItemCount,const BYTE cbCurrentCard)
	{
		//�����ж�
		BYTE cbCardColor=0xFF;

		for (BYTE i=0;i<MAX_INDEX;i++)
		{
			if(i==m_cbMagicIndex) continue;
			if (cbCardIndex[i]!=0)
			{
				//��ɫ�ж�
				if (cbCardColor!=0xFF)
					return false;

				//���û�ɫ
				cbCardColor=(SwitchToCardData(i)&MASK_COLOR);

				//��������
				i=(i/9+1)*9-1;
			}
		}

		//�������ֻ������
		if( cbCardColor == 0xFF )
		{
			ASSERT( m_cbMagicIndex != MAX_INDEX && cbCardIndex[m_cbMagicIndex] > 0 );
			//������
			ASSERT( cbItemCount > 0 );
			cbCardColor = WeaveItem[0].cbCenterCard&MASK_COLOR;
		}

		if((cbCurrentCard&MASK_COLOR)!=cbCardColor && !IsMagicCard(cbCurrentCard) ) return false;

		//����ж�
		for (BYTE i=0;i<cbItemCount;i++)
		{
			BYTE cbCenterCard=WeaveItem[i].cbCenterCard;
			if ((cbCenterCard&MASK_COLOR)!=cbCardColor)	return false;
		}

		return true;
	}

	//��С����
	bool CGameLogic::IsQiXiaoDui(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount,const BYTE cbCurrentCard)
	{
		//����ж�
		if (cbWeaveCount!=0) return false;

		//������Ŀ
		BYTE cbReplaceCount = 0;

		//��ʱ����
		BYTE cbCardIndexTemp[MAX_INDEX];
		memcpy(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

		//��������
		BYTE cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);
		cbCardIndexTemp[cbCurrentIndex]++;

		//���㵥��
		for (BYTE i=0;i<MAX_INDEX;i++)
		{
			BYTE cbCardCount=cbCardIndexTemp[i];

			//���ƹ���
			if( i == m_cbMagicIndex ) continue;

			//����ͳ��
			if( cbCardCount == 1 || cbCardCount == 3 ) 	cbReplaceCount++;
		}

		//���Ʋ���
		if( m_cbMagicIndex != MAX_INDEX && cbReplaceCount > cbCardIndexTemp[m_cbMagicIndex] ||
			m_cbMagicIndex == MAX_INDEX && cbReplaceCount > 0 )
			return false;

		return true;

	}

	//����
	bool CGameLogic::IsDaiYao( const tagAnalyseItem *pAnalyseItem )
	{
		//�������
		BYTE cbCardValue = pAnalyseItem->cbCardEye&MASK_VALUE;
		if( cbCardValue != 1 && cbCardValue != 9 ) return false;

		for( BYTE i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++ )
		{
			if( pAnalyseItem->cbWeaveKind[i]&(WIK_LEFT|WIK_CENTER|WIK_RIGHT) )
			{
				BYTE j = 0;
				for(; j < 3; j++ )
				{
					cbCardValue = pAnalyseItem->cbCardData[i][j]&MASK_VALUE;
					if( cbCardValue == 1 || cbCardValue == 9 ) break;
				}
				if( j == 3 ) return false;
			}
			else
			{
				cbCardValue = pAnalyseItem->cbCenterCard[i]&MASK_VALUE;
				if( cbCardValue != 1 && cbCardValue != 9 ) return false;
			}
		}
		return true;
	}

	//����
	bool CGameLogic::IsJiangDui( const tagAnalyseItem *pAnalyseItem )
	{
		//�Ƿ�����
		if( !IsPengPeng(pAnalyseItem) ) return false;

		//�������
		BYTE cbCardValue = pAnalyseItem->cbCardEye&MASK_VALUE;
		if( cbCardValue != 2 && cbCardValue != 5 && cbCardValue != 8 ) return false;

		for( BYTE i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++ )
		{
			if( pAnalyseItem->cbWeaveKind[i]&(WIK_LEFT|WIK_CENTER|WIK_RIGHT) )
			{
				BYTE j = 0;
				for(; j < 3; j++ )
				{
					cbCardValue = pAnalyseItem->cbCardData[i][j]&MASK_VALUE;
					if( cbCardValue == 2 || cbCardValue == 5 || cbCardValue == 8 ) break;
				}
				if( j == 3 ) return false;
			}
			else
			{
				cbCardValue = pAnalyseItem->cbCenterCard[i]&MASK_VALUE;
				if( cbCardValue != 2 && cbCardValue != 5 && cbCardValue != 8 ) return false;
			}
		}
		return true;
	}

	//�Ƿ���
	bool CGameLogic::IsHuaZhu( const BYTE cbCardIndex[], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount )
	{
		BYTE cbColor[3] = { 0,0,0 };
		for( BYTE i = 0; i < MAX_INDEX; i++ )
		{
			if( cbCardIndex[i] > 0 )
			{
				BYTE cbCardColor = SwitchToCardData(i)&MASK_COLOR;
				cbColor[cbCardColor>>4]++;

				i = (i/9+1)*9-1;
			}
		}
		for( BYTE i = 0; i < cbWeaveCount; i++ )
		{
			BYTE cbCardColor = WeaveItem[i].cbCenterCard&MASK_COLOR;
			cbColor[cbCardColor>>4]++;
		}
		//ȱһ�žͲ��ǻ���
		for( BYTE i = 0; i < CountArray(cbColor); i++ )
			if( cbColor[i] == 0 ) return false;

		return true;
	}

	BYTE CGameLogic::GetCardColor(BYTE cbCardDat)
	{
		ASSERT(IsValidCard(cbCardDat));
		return ((cbCardDat&MASK_COLOR)>>4)+1;
	}

	BYTE CGameLogic::GetCardValue(BYTE cbCardDat)
	{
		ASSERT(IsValidCard(cbCardDat));
		return (cbCardDat&MASK_VALUE);
	}
	void CGameLogic::GetCardInfoList(BYTE* pHandCardData, int nHandCardCount,std::vector<Card_Info>& kCardInfoList )
	{
		for (int m=1;m<=CARD_COLOR_TIAO;m++)
		{	
			Card_Info kInfo;
			kInfo.nColor = m;
			kInfo.nLen = 0;
			for(int i=0;i<nHandCardCount;i++)
			{
				BYTE cbColor= GetCardColor(pHandCardData[i]);
				if (cbColor== kInfo.nColor )
				{
					kInfo.kCardList.push_back(pHandCardData[i]);
					kInfo.nLen++;
				}
			}
			kCardInfoList.push_back(kInfo);
		}
		std::sort(kCardInfoList.begin(),kCardInfoList.end());
	}
}

//////////////////////////////////////////////////////////////////////////
