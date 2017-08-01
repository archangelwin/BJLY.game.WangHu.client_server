
#include "NNGameLogic.h"

//////////////////////////////////////////////////////////////////////////

FV_SINGLETON_STORAGE(NNGameLogic);
//�˿�����
BYTE NNGameLogic::m_cbCardListData[52]=
{
	//0x1C,0x1D,0x3B,0x3C,0x3D,0x09,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x0A,0x0B,0x0C,0x0D,	//���� A - K
	//0x3A,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,	//÷�� A - K
	//0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//���� A - K
	//0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39	//���� A - K
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//���� A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//÷�� A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//���� A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D	//���� A - K
};

//////////////////////////////////////////////////////////////////////////

//���캯��
NNGameLogic::NNGameLogic()
{
}

//��������
NNGameLogic::~NNGameLogic()
{
}

//��ȡ����
BYTE NNGameLogic::GetCardType(BYTE cbCardData[], BYTE cbCardCount)
{
	ASSERT(cbCardCount==MAX_COUNT);

	BYTE cbCardDataTemp[MAX_COUNT];
	for (int i=0;i<MAX_COUNT;i++)
	{
		cbCardDataTemp[i] = cbCardData[i];
	}

	//ը������
	BYTE bSameCount = 0;
	SortCardList(cbCardDataTemp,cbCardCount);
	BYTE bSecondValue = GetCardValue(cbCardDataTemp[MAX_COUNT/2]);
	for(BYTE i=0;i<cbCardCount;i++)
	{
		if(bSecondValue == GetCardValue(cbCardDataTemp[i]))
		{
			bSameCount++;
		}
	}
	if(bSameCount==4)return OX_FOUR_SAME;

	BYTE bKingCount=0,bTenCount=0;
	for(BYTE i=0;i<cbCardCount;i++)
	{
		if(GetCardValue(cbCardDataTemp[i])>10)
		{
			bKingCount++;
		}
		else if(GetCardValue(cbCardDataTemp[i])==10)
		{
			bTenCount++;
		}
	}
	if(bKingCount==MAX_COUNT) 
	{
		return OX_FIVEKING;
	}


	BYTE bTemp[MAX_COUNT];
	BYTE bSum=0;
	for (BYTE i=0;i<cbCardCount;i++)
	{
		bTemp[i]=GetCardLogicValue(cbCardDataTemp[i]);
		bSum+=bTemp[i];
	}
	if(bSum == 10)
	{
		return OX_FIVESMALL;
	}

	for (BYTE i=0;i<cbCardCount-1;i++)
	{
		for (BYTE j=i+1;j<cbCardCount;j++)
		{
			if((bSum-bTemp[i]-bTemp[j])%10==0)
			{
				return ((bTemp[i]+bTemp[j])>10)?(bTemp[i]+bTemp[j]-10):(bTemp[i]+bTemp[j]);
			}
		}
	}

	return OX_VALUE0;
}

//��ȡ����
BYTE NNGameLogic::GetTimes(BYTE cbCardData[], BYTE cbCardCount)
{
	if(cbCardCount!=MAX_COUNT)return 0;

	BYTE bTimes=GetCardType(cbCardData,MAX_COUNT);
	if(bTimes<7)return 1;
	else if(bTimes==7)return 1;
	else if(bTimes==8)return 2;
	else if(bTimes==9)return 2;
	else if(bTimes==10)return 3;
	else if(bTimes==OX_FOUR_SAME)return 4;
	else if(bTimes==OX_FIVEKING)return 5;
	else if(bTimes==OX_FIVESMALL)return 8;

	return 0;
}

//��ȡţţ
bool NNGameLogic::GetOxCard(BYTE cbCardData[], BYTE cbCardCount)
{
	ASSERT(cbCardCount==MAX_COUNT);

	//���ñ���
	BYTE bTemp[MAX_COUNT],bTempData[MAX_COUNT];
	memcpy(bTempData,cbCardData,sizeof(bTempData));
	BYTE bSum=0;
	for (BYTE i=0;i<cbCardCount;i++)
	{
		bTemp[i]=GetCardLogicValue(cbCardData[i]);
		bSum+=bTemp[i];
	}

	//����ţţ
	for (BYTE i=0;i<cbCardCount-1;i++)
	{
		for (BYTE j=i+1;j<cbCardCount;j++)
		{
			if((bSum-bTemp[i]-bTemp[j])%10==0)
			{
				BYTE bCount=0;
				for (BYTE k=0;k<cbCardCount;k++)
				{
					if(k!=i && k!=j)
					{
						cbCardData[bCount++] = bTempData[k];
					}
				}ASSERT(bCount==3);

				cbCardData[bCount++] = bTempData[i];
				cbCardData[bCount++] = bTempData[j];

				return true;
			}
		}
	}

	return false;
}

//��ȡ����
bool NNGameLogic::IsIntValue(BYTE cbCardData[], BYTE cbCardCount)
{
	BYTE sum=0;
	for(BYTE i=0;i<cbCardCount;i++)
	{
		sum+=GetCardLogicValue(cbCardData[i]);
	}
	ASSERT(sum>0);
	return (sum%10==0);
}

//�����˿�
void NNGameLogic::SortCardList(BYTE cbCardData[], BYTE cbCardCount)
{
	//ת����ֵ
	BYTE cbLogicValue[MAX_COUNT];
	for (BYTE i=0;i<cbCardCount;i++) cbLogicValue[i]=GetCardValue(cbCardData[i]);	

	//�������
	bool bSorted=true;
	BYTE cbTempData,bLast=cbCardCount-1;
	do
	{
		bSorted=true;
		for (BYTE i=0;i<bLast;i++)
		{
			if ((cbLogicValue[i]<cbLogicValue[i+1])||
				((cbLogicValue[i]==cbLogicValue[i+1])&&(cbCardData[i]<cbCardData[i+1])))
			{
				//����λ��
				cbTempData=cbCardData[i];
				cbCardData[i]=cbCardData[i+1];
				cbCardData[i+1]=cbTempData;
				cbTempData=cbLogicValue[i];
				cbLogicValue[i]=cbLogicValue[i+1];
				cbLogicValue[i+1]=cbTempData;
				bSorted=false;
			}	
		}
		bLast--;
	} while(bSorted==false);

	return;
}

//�����˿�
void NNGameLogic::RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{
	//CopyMemory(cbCardBuffer,m_cbCardListData,cbBufferCount);
	//����׼��
	BYTE cbCardData[CountArray(m_cbCardListData)];
	memcpy(cbCardData,m_cbCardListData,sizeof(m_cbCardListData));

	//�����˿�
	BYTE bRandCount=0,bPosition=0;
	do
	{
		bPosition=rand()%(CountArray(m_cbCardListData)-bRandCount);
		cbCardBuffer[bRandCount++]=cbCardData[bPosition];
		cbCardData[bPosition]=cbCardData[CountArray(m_cbCardListData)-bRandCount];
	} while (bRandCount<cbBufferCount);

	return;
}

//�߼���ֵ
BYTE NNGameLogic::GetCardLogicValue(BYTE cbCardData)
{
	//�˿�����
	BYTE bCardColor=GetCardColor(cbCardData);
	BYTE bCardValue=GetCardValue(cbCardData);

	//ת����ֵ
	return (bCardValue>10)?(10):bCardValue;
}

//�Ա��˿�
bool NNGameLogic::CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount,bool FirstOX,bool NextOX)
{
	if(FirstOX!=NextOX)return (FirstOX>NextOX);

	//�Ƚ�ţ��С
	if(FirstOX==TRUE)
	{
		//��ȡ����
		BYTE cbNextType=GetCardType(cbNextData,cbCardCount);
		BYTE cbFirstType=GetCardType(cbFirstData,cbCardCount);

		//�����ж�
		if (cbFirstType!=cbNextType) return (cbFirstType>cbNextType);

		switch(cbNextType)
		{
		case OX_FOUR_SAME:		//ը������	
			{
				//�����С
				BYTE bFirstTemp[MAX_COUNT],bNextTemp[MAX_COUNT];
				memcpy(bFirstTemp,cbFirstData,cbCardCount);
				memcpy(bNextTemp,cbNextData,cbCardCount);
				SortCardList(bFirstTemp,cbCardCount);
				SortCardList(bNextTemp,cbCardCount);

				return GetCardValue(bFirstTemp[MAX_COUNT/2])>GetCardValue(bNextTemp[MAX_COUNT/2]);

				break;
			}
		}
	}

	//�����С
	BYTE bFirstTemp[MAX_COUNT],bNextTemp[MAX_COUNT];
	memcpy(bFirstTemp,cbFirstData,cbCardCount);
	memcpy(bNextTemp,cbNextData,cbCardCount);
	SortCardList(bFirstTemp,cbCardCount);
	SortCardList(bNextTemp,cbCardCount);

	//�Ƚ���ֵ
	BYTE cbNextMaxValue=GetCardValue(bNextTemp[0]);
	BYTE cbFirstMaxValue=GetCardValue(bFirstTemp[0]);
	if(cbNextMaxValue!=cbFirstMaxValue)return cbFirstMaxValue>cbNextMaxValue;

	//�Ƚ���ɫ
	return GetCardColor(bFirstTemp[0]) > GetCardColor(bNextTemp[0]);

	return false;
}

//////////////////////////////////////////////////////////////////////////