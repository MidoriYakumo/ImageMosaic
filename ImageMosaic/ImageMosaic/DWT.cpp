#include "StdAfx.h"
#include "DWT.h"

CDWT::CDWT(void)
: m_bTwice(false)
, m_bOnce(false)
{
}

CDWT::~CDWT(void)
{
}

void CDWT::WvltTransTwice(CImage &pImage,float* &I)
{
	//��ȡͼ������Բ���
	BYTE* pArray = (BYTE*)pImage.GetBits(); //�������
	int pitch = pImage.GetPitch();
	int bitCount = pImage.GetBPP() / 8;  //ÿ����λ��
	int height = pImage.GetHeight();
	int width = pImage.GetWidth();

	//ͼ�������������������	
	int x,y;
	unsigned char tempR, tempG, tempB;
	float fTempBufforDisp;
	short MaxPixVal,MinPixVal,Diff;
	short **spOriginData, **spTransData0, **spTransData1;
	//����ͼ��С���任���õ����ݿռ�
	spOriginData = new short* [height];
	spTransData0 = new short* [height];
	spTransData1 = new short* [height];

	for(int i = 0; i < height; i ++)
	{
		spOriginData[i] = new short [width];
		spTransData0[i] = new short [width];
		spTransData1[i] = new short [width];
	}

	//���豸�����л�ȡԭʼͼ������
	for(y = 0; y < height; y++)
	{
		for( x = 0; x < width; x++)
		{
			tempR = char((*(pArray + pitch * y + bitCount * x)));
			tempG = char((*(pArray + pitch * y + bitCount * x + 1)));
			tempB = char((*(pArray + pitch * y + bitCount * x + 2)));
			spOriginData[y][x] = short(0.299f * tempR + 0.587f * tempG + 0.114f * tempB);
		}
	}
	//���ͼ�������С���任
	DWT_TwoLayers(spOriginData,spTransData0,spTransData1,height,height/2,width,width/2,2,1.414f);
	MaxPixVal=spTransData1[0][0];
	MinPixVal=spTransData1[0][0];
	//����õ�ͼ��С��ϵ���ļ���ֵ�뼫Сֵ
	for( y=0; y<height/4; y++)
	{
		for( x=0; x<width/4; x++)
		{
			if(MaxPixVal<spTransData1[y][x])
			{
				MaxPixVal=spTransData1[y][x];
			}
			if(MinPixVal>spTransData1[y][x])
			{
				MinPixVal=spTransData1[y][x];
			}
		}
	}
	//������С��ϵ���ļ�ֵ��
	Diff=MaxPixVal-MinPixVal;
	//С��ϵ����������󣬷�����ʾ������
	for(y=0; y<height / 4; y++)
	{
		for(x=0; x<width / 4; x++)
		{
			//��ΪС���任���С��ϵ���п��ܳ���255�������࣬��ô�ͽ�
			//С��ϵ���ķ�Χӳ�䵽0~255�����ڣ��Ժ�������ƵĴ���Ŀ�Ķ���һ����
			fTempBufforDisp=spTransData1[y][x];
			fTempBufforDisp-=MinPixVal;
			fTempBufforDisp*=255;
			fTempBufforDisp/=Diff;
			I[y*(width/4) + x] = fTempBufforDisp;
		}
	}

	//ɾ����ʱ�����ݿռ�
	for(int a = 0; a < height; a ++)
	{
		delete[]spOriginData[a];
		spOriginData[a] = NULL;
		delete[]spTransData0[a];
		spTransData0[a] = NULL;
		delete[]spTransData1[a];
		spTransData1[a] = NULL;
	}
	delete[]spOriginData;
	spOriginData = NULL;
	delete[]spTransData0;
	spTransData0 = NULL;
	delete[]spTransData1;
	spTransData1 = NULL;
}
void CDWT::DWT_TwoLayers(short**& spOriginData, short**& spTransData0, short**& spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius)
{
	//����ѭ���������С���任
	for(int i=1; i<=layer; i++)
	{
		DWT_Once(spOriginData,spTransData0,spTransData1,nHeight,nHeight_H,nWidth,nWidth_H,i,fRadius);
		nHeight   = nHeight/2;		
		nWidth    = nWidth/2;
		nHeight_H = nHeight/2;	
		nWidth_H  = nWidth/2;
	}

}
void CDWT::WvltTransOnce(CImage &pImage,float* &I)
{
	//��ȡͼ������Բ��� 
	BYTE* pArray = (BYTE*)pImage.GetBits(); //�������
	int pitch = pImage.GetPitch();
	int bitCount = pImage.GetBPP() / 8;  //ÿ����λ��
	int height = pImage.GetHeight();
	int width = pImage.GetWidth();

	//ͼ�������������������	
	int x,y;
	unsigned char tempR, tempG, tempB;
	float fTempBufforDisp;
	short MaxPixVal,MinPixVal,Diff;
	short **spOriginData, **spTransData0, **spTransData1;
	//����ͼ��С���任���õ����ݿռ�
	spOriginData = new short* [height];
	spTransData0 = new short* [height];
	spTransData1 = new short* [height];

	for(int i = 0; i < height; i ++)
	{
		spOriginData[i] = new short [width];
		spTransData0[i] = new short [width];
		spTransData1[i] = new short [width];
	}

	//���豸�����л�ȡԭʼͼ������
	for(y = 0; y < height; y++)
	{
		for( x = 0; x < width; x++)
		{
			tempR = char((*(pArray + pitch * y + bitCount * x)));
			tempG = char((*(pArray + pitch * y + bitCount * x + 1)));
			tempB = char((*(pArray + pitch * y + bitCount * x + 2)));
			spOriginData[y][x] = short(0.299f * tempR + 0.587f * tempG + 0.114f * tempB);
		}
	}
	//���һ��ͼ��С���任
	DWT_Once(spOriginData,spTransData0,spTransData1,height,height/2,width,width/2,1,1.414f);
	MaxPixVal = spTransData1[0][0];
	MinPixVal = spTransData1[0][0];
	for( y = 0; y < height/2; y++)
	{
		for( x = 0; x < width/2; x++)
		{
			if(MaxPixVal < spTransData1[y][x])
			{
				MaxPixVal = spTransData1[y][x];
			}
			if(MinPixVal > spTransData1[y][x])
			{
				MinPixVal = spTransData1[y][x];
			}
		}
	}
	Diff = MaxPixVal - MinPixVal;
	for(y = 0; y < height / 2; y++)
	{
		for(x = 0; x < width / 2; x++)
		{
			//��ΪС���任���С��ϵ���п��ܳ���255�������࣬��ô�ͽ�
			//С��ϵ���ķ�Χӳ�䵽0~255�����ڣ��Ժ�������ƵĴ���Ŀ�Ķ���һ����
			fTempBufforDisp = spTransData1[y][x];
			fTempBufforDisp -= MinPixVal;
			fTempBufforDisp *= 255;
			fTempBufforDisp /= Diff;
			I[y*(width/2) + x] = fTempBufforDisp;
		}
	}
	//ɾ����ʱ�����ݿռ�
	for(int a = 0; a < height; a ++)
	{
		delete[]spOriginData[a];
		spOriginData[a] = NULL;
		delete[]spTransData0[a];
		spTransData0[a] = NULL;
		delete[]spTransData1[a];
		spTransData1[a] = NULL;
	}
	delete[]spOriginData;
	spOriginData = NULL;
	delete[]spTransData0;
	spTransData0 = NULL;
	delete[]spTransData1;
	spTransData1 = NULL;
}
void CDWT::DWT_Once(short**& spOriginData, short**& spTransData0, short**& spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius)
{
	int Trans_W,				//ͼ��ɨ���߿��ƣ�������
		Trans_H,				//ͼ��ɨ���߿��ƣ�������
		Trans_M,				//ͼ�����ĺ�����
		Trans_N;				//ͼ������������
	short Trans_Coeff0;			//С���任ϵ��
    signed short Trans_Coeff1;
	fRadius = 1.414f;				//�任�˲�ϵ��
	//��ģ����ɱ任ϵ���ĸ�ֵ����
	//�б任,��һ�Σ�layer=1ʱ��ʱnHeight��Ϊԭʼͼ��ĸ߶�ֵ
    for(Trans_H = 0; Trans_H < nHeight; Trans_H++)            
	{
		if(layer == 1)
		{
			 //layer=1ʱ��nWidth_HΪԭʼͼ����ֵ��һ��
			for(Trans_N = 0; Trans_N < nWidth_H; Trans_N++)          
			{
				Trans_W = Trans_N<<1;
	            if (fRadius == 2)
				{
					spTransData0[Trans_H][Trans_N] = (spOriginData[Trans_H][Trans_W]);
                    spTransData0[Trans_H][nWidth_H + Trans_N] = (spOriginData[Trans_H][Trans_W + 1]);
				}
	            else
				{
                    spTransData0[Trans_H][Trans_N] = (spOriginData[Trans_H][Trans_W]- 128);		
                    spTransData0[Trans_H][nWidth_H + Trans_N] = (spOriginData[Trans_H][Trans_W + 1] - 128);	
				}
	   		}
		}
		//���任��������1,���������Ƶ��С��ϵ��
		if(layer > 1)
		{
			for(Trans_N = 0; Trans_N < nWidth_H; Trans_N++)
			{
				Trans_W = Trans_N<<1;
				spTransData0[Trans_H][Trans_N] = spTransData1[Trans_H][Trans_W];
				spTransData0[Trans_H][nWidth_H + Trans_N] = spTransData1[Trans_H][Trans_W + 1];
			}
		}
	}
	for(Trans_H = 0; Trans_H < nHeight; Trans_H++)
	{
		for(Trans_N = 0; Trans_N < nWidth_H - 1; Trans_N++)
		{
			//��ż��ֵ�͵�һ��
			Trans_Coeff1 = ((spTransData0[Trans_H][Trans_N] + spTransData0[Trans_H][Trans_N + 1])>>1);	
			//�߼��ǲ�������ֵ��1
			Trans_Coeff1 = ~Trans_Coeff1 + 1;	
			//ϵ��Ԥ��
			spTransData0[Trans_H][nWidth_H + Trans_N] = spTransData0[Trans_H][nWidth_H + Trans_N] + Trans_Coeff1;	
		}
		//���һ��żϵ���ı߽紦��
		Trans_Coeff1 = ((spTransData0[Trans_H][nWidth_H - 1] + spTransData0[Trans_H][nWidth_H - 2])>>1);
		Trans_Coeff1 = ~Trans_Coeff1 + 1;
		spTransData0[Trans_H][nWidth - 1] = spTransData0[Trans_H][nWidth - 1] + Trans_Coeff1;
		//���һ����ϵ���ı߽紦��
		Trans_Coeff0 = ((spTransData0[Trans_H][nWidth_H] + spTransData0[Trans_H][nWidth_H + 1])>>2);
		spTransData0[Trans_H][0] = spTransData0[Trans_H][0] + Trans_Coeff0;
		//�����������������ı任
		for(Trans_N = 1; Trans_N < nWidth_H; Trans_N++)
		{
			Trans_Coeff0 = ((spTransData0[Trans_H][nWidth_H + Trans_N] + spTransData0[Trans_H][nWidth_H + Trans_N - 1])>>2);
			spTransData0[Trans_H][Trans_N] = spTransData0[Trans_H][Trans_N] + Trans_Coeff0;
		}

	}//ˮƽ����ı任����
	//��ֱ����ı任��ʼ������Դδˮƽ�任���С��ϵ��
	for(Trans_M = 0; Trans_M < nHeight; Trans_M++)
	{
		for(Trans_N = 0; Trans_N < nWidth_H; Trans_N++)
		{
			spTransData0[Trans_M][Trans_N] = short(spTransData0[Trans_M][Trans_N] * fRadius);
			spTransData0[Trans_M][Trans_N + nWidth_H] = short(spTransData0[Trans_M][Trans_N + nWidth_H] / fRadius);
		}
	}
	//���������������spTransData0�У�spTransData0�е�������Ȼ��ż����
	for(Trans_N = 0; Trans_N < nWidth_H; Trans_N++)
	{
		//�б任
		for(Trans_M = 0; Trans_M < nHeight_H; Trans_M++)
		{
			Trans_H = Trans_M<<1;
			//Ƶ��LL����
			spTransData1[Trans_M][Trans_N] = spTransData0[Trans_H][Trans_N];
			//Ƶ��HL����
			spTransData1[nHeight_H + Trans_M][Trans_N] = spTransData0[Trans_H + 1][Trans_N];
			//Ƶ��LH����
			spTransData1[Trans_M][nWidth_H + Trans_N] = spTransData0[Trans_H][nWidth_H + Trans_N];	
			//Ƶ��HH����
			spTransData1[nHeight_H + Trans_M][nWidth_H + Trans_N] = spTransData0[Trans_H + 1][nWidth_H + Trans_N];
		}
		//��һ��������������ϵ��
		for(Trans_M = 0; Trans_M < nHeight_H - 1; Trans_M++)
		{
			//��ֱ����ı任 
			Trans_Coeff1 = ((spTransData1[Trans_M][Trans_N]+spTransData1[Trans_M+1][Trans_N])>>1);
			Trans_Coeff1=~Trans_Coeff1+1;
			spTransData1[nHeight_H+Trans_M][Trans_N] = spTransData1[nHeight_H+Trans_M][Trans_N]+Trans_Coeff1;
			Trans_Coeff1 = ((spTransData1[Trans_M][nWidth_H+Trans_N]+spTransData1[Trans_M+1][nWidth_H+Trans_N])>>1);
			Trans_Coeff1=~Trans_Coeff1+1;
			spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N] = spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N]+Trans_Coeff1;
		}
		Trans_Coeff1 = ((spTransData1[nHeight_H-1][Trans_N]+spTransData1[nHeight_H-2][Trans_N])>>1);
		Trans_Coeff1=~Trans_Coeff1+1;
		spTransData1[nHeight-1][Trans_N] = spTransData1[nHeight-1][Trans_N]+Trans_Coeff1;
		Trans_Coeff1 = ((spTransData1[nHeight_H-1][nWidth_H+Trans_N]+spTransData1[nHeight_H-2][nWidth_H+Trans_N])>>1);
		Trans_Coeff1=~Trans_Coeff1+1;
		//�߽紦��
		spTransData1[nHeight-1][nWidth_H+Trans_N] = spTransData1[nHeight-1][nWidth_H+Trans_N]+Trans_Coeff1;

		Trans_Coeff0 = ((spTransData1[nHeight_H][Trans_N]+spTransData1[nHeight_H+1][Trans_N])>>2);
		spTransData1[0][Trans_N] = spTransData1[0][Trans_N]+Trans_Coeff0;
		Trans_Coeff0 = ((spTransData1[nHeight_H][nWidth_H+Trans_N]+spTransData1[nHeight_H+1][nWidth_H+Trans_N])>>2);
		//�߽紦��
		spTransData1[0][nWidth_H+Trans_N] = spTransData1[0][nWidth_H+Trans_N]+Trans_Coeff0;
		//��һ������ż������ϵ��
		for(Trans_M = 1; Trans_M < nHeight_H; Trans_M++)
		{
			Trans_Coeff0 = ((spTransData1[nHeight_H+Trans_M][Trans_N]+spTransData1[nHeight_H+Trans_M-1][Trans_N])>>2);
			spTransData1[Trans_M][Trans_N] = spTransData1[Trans_M][Trans_N]+Trans_Coeff0;
			Trans_Coeff0 = ((spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N]+spTransData1[nHeight_H+Trans_M-1][nWidth_H+Trans_N])>>2);
			spTransData1[Trans_M][nWidth_H+Trans_N] = spTransData1[Trans_M][nWidth_H+Trans_N]+Trans_Coeff0;
		}
	}
	//���С��ϵ����LLƵ����ϵ�����з�ֵ��ǿ����������ƵƵ����ϵ�����������ֵ
	for(Trans_N = 0; Trans_N < nWidth; Trans_N++)
	{
		for(Trans_M = 0; Trans_M < nHeight_H; Trans_M++)
		{
			spTransData1[Trans_M][Trans_N] = short(spTransData1[Trans_M][Trans_N] * fRadius);
			spTransData1[Trans_M+nHeight_H][Trans_N] = short(spTransData1[Trans_M+nHeight_H][Trans_N] / fRadius);
		}
	}
}
