// Des.cpp : Defines the entry point for the console application.
//
#include "AlgDes1.h"
//#include "stdlib.h"
//#include "stdio.h"
//#include <memory.h>
#include <string.h>

#define CRYPTO_CHECKSUM_LENGTH 4
static unsigned char Shift[16]=
{
    1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1
};
static unsigned char PC_1[56]=
{
    56, 48, 40, 32, 24, 16,  8,  0, 57, 49, 41, 33, 25, 17,
     9,  1, 58, 50, 42, 34, 26, 18, 10,  2, 59, 51, 43, 35,
    62, 54, 46, 38, 30, 22, 14,  6, 61, 53, 45, 37, 29, 21,
    13,  5, 60, 52, 44, 36, 28, 20, 12,  4, 27, 19, 11,  3
};
static unsigned char PC_2[48]=
{
    13, 16, 10, 23,  0,  4,  2, 27, 14,  5, 20,  9,
    22, 18, 11,  3, 25,  7, 15,  6, 26, 19, 12,  1,
    40, 51, 30, 36, 46, 54, 29, 39, 50, 44, 32, 47,
    43, 48, 38, 55, 33, 52, 45, 41, 49, 35, 28, 31
};
static unsigned char IIP[64]=
{
    57, 49, 41, 33, 25, 17,  9,  1, 59, 51, 43, 35, 27, 19, 11,  3,
    61, 53, 45, 37, 29, 21, 13,  5, 63, 55, 47, 39, 31, 23, 15,  7,
    56, 48, 40, 32, 24, 16,  8,  0, 58, 50, 42, 34, 26, 18, 10,  2,
    60, 52, 44, 36, 28, 20, 12,  4, 62, 54, 46, 38, 30, 22, 14,  6
};
static unsigned char IIP_1[64]=
{
    39,  7, 47, 15, 55, 23, 63, 31, 38,  6, 46, 14, 54, 22, 62, 30,
    37,  5, 45, 13, 53, 21, 61, 29, 36,  4, 44, 12, 52, 20, 60, 28,
    35,  3, 43, 11, 51, 19, 59, 27, 34,  2, 42, 10, 50, 18, 58, 26,
    33,  1, 41,  9, 49, 17, 57, 25, 32,  0, 40,  8, 48, 16, 56, 24
};
static unsigned char E[48]=
{
     63, 32, 33, 34, 35, 36, 35, 36, 37, 38, 39, 40,
     39, 40, 41, 42, 43, 44, 43, 44, 45, 46, 47, 48,
     47, 48, 49, 50, 51, 52, 51, 52, 53, 54, 55, 56,
     55, 56, 57, 58, 59, 60, 59, 60, 61, 62, 63, 32
};
static unsigned char P[32]=
{
     15,  6, 19, 20, 28, 11, 27, 16,
      0, 14, 22, 25,  4, 17, 30,  9,
      1,  7, 23, 13, 31, 26,  2,  8,
     18, 12, 29,  5, 21, 10,  3, 24
};
static unsigned char S[8][64]=
{
    {
	 14,  4,  3, 15,  2, 13,  5,  3, 13, 14,  6,  9, 11,  2,  0,  5,
	  4,  1, 10, 12, 15,  6,  9, 10,  1,  8, 12,  7,  8, 11,  7,  0,
	  0, 15, 10,  5, 14,  4,  9, 10,  7,  8, 12,  3, 13,  1,  3,  6,
	 15, 12,  6, 11,  2,  9,  5,  0,  4,  2, 11, 14,  1,  7,  8, 13
    },
    {
	 15,  0,  9,  5,  6, 10, 12,  9,  8,  7,  2, 12,  3, 13,  5,  2,
	  1, 14,  7,  8, 11,  4,  0,  3, 14, 11, 13,  6,  4,  1, 10, 15,
	  3, 13, 12, 11, 15,  3,  6,  0,  4, 10,  1,  7,  8,  4, 11, 14,
	 13,  8,  0,  6,  2, 15,  9,  5,  7,  1, 10, 12, 14,  2,  5,  9
    },
    {
	 10, 13,  1, 11,  6,  8, 11,  5,  9,  4, 12,  2, 15,  3,  2, 14,
	  0,  6, 13,  1,  3, 15,  4, 10, 14,  9,  7, 12,  5,  0,  8,  7,
	 13,  1,  2,  4,  3,  6, 12, 11,  0, 13,  5, 14,  6,  8, 15,  2,
	  7, 10,  8, 15,  4,  9, 11,  5,  9,  0, 14,  3, 10,  7,  1, 12
    },
    {
	  7, 10,  1, 15,  0, 12, 11,  5, 14,  9,  8,  3,  9,  7,  4,  8,
	 13,  6,  2,  1,  6, 11, 12,  2,  3,  0,  5, 14, 10, 13, 15,  4,
	 13,  3,  4,  9,  6, 10,  1, 12, 11,  0,  2,  5,  0, 13, 14,  2,
	  8, 15,  7,  4, 15,  1, 10,  7,  5,  6, 12, 11,  3,  8,  9, 14
    },
    {
	  2,  4,  8, 15,  7, 10, 13,  6,  4,  1,  3, 12, 11,  7, 14,  0,
	 12,  2,  5,  9, 10, 13,  0,  3,  1, 11, 15,  5,  6,  8,  9, 14,
	 14, 11,  5,  6,  4,  1,  3, 10,  2, 12, 15,  0, 13,  2,  8,  5,
	 11,  8,  0, 15,  7, 14,  9,  4, 12,  7, 10,  9,  1, 13,  6,  3
    },
    {
	 12,  9,  0,  7,  9,  2, 14,  1, 10, 15,  3,  4,  6, 12,  5, 11,
	  1, 14, 13,  0,  2,  8,  7, 13, 15,  5,  4, 10,  8,  3, 11,  6,
	 10,  4,  6, 11,  7,  9,  0,  6,  4,  2, 13,  1,  9, 15,  3,  8,
	 15,  3,  1, 14, 12,  5, 11,  0,  2, 12, 14,  7,  5, 10,  8, 13
    },
    {
	  4,  1,  3, 10, 15, 12,  5,  0,  2, 11,  9,  6,  8,  7,  6,  9,
	 11,  4, 12, 15,  0,  3, 10,  5, 14, 13,  7,  8, 13, 14,  1,  2,
	 13,  6, 14,  9,  4,  1,  2, 14, 11, 13,  5,  0,  1, 10,  8,  3,
	  0, 11,  3,  5,  9,  4, 15,  2,  7,  8, 12, 15, 10,  7,  6, 12
    },
    {
	 13,  7, 10,  0,  6,  9,  5, 15,  8,  4,  3, 10, 11, 14, 12,  5,
	  2, 11,  9,  6, 15, 12,  0,  3,  4,  1, 14, 13,  1,  2,  7,  8,
	  1,  2, 12, 15, 10,  4,  0,  3, 13, 14,  6,  9,  7,  8,  9,  6,
	 15,  1,  5, 12,  3, 10, 14,  5,  8,  7, 11,  0,  4, 13,  2, 11
    }
};
static unsigned char bitmask[]={ 0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01 };
static unsigned char KEY[64];
static unsigned char CDD[56];                     /* 56 bit C0 & D0       */
static unsigned char LR0[64],LR1[64];
static unsigned char SUBKEY[16][48];             /* 48 bit sub_key *16   */
static unsigned char bit48[48];
static unsigned char bit32[32];
unsigned char deskey[9];

int getbit(unsigned char *buf,int i)
{
	return ((*(unsigned char*)((unsigned char*)buf+(i>>3))) & bitmask[i&0x07])!=0;
}

void setbit(unsigned char *buf,int i)
{
    *((unsigned char *)buf+(i>>3)) |= bitmask[i&0x07];
}

void getsubkey()
{
    int i,j,s1,s2;
    for (i=0; i<56; i++)
	CDD[i]=KEY[PC_1[i]];
    for (i=0; i<16; i++)   
	{
		s1=CDD[0]; s2=CDD[1];
		for (j=0; j<28-Shift[i]; j++)
			CDD[j]=CDD[j+Shift[i]];
		if (Shift[i]==2)	
		{
			CDD[26]=s1;
			CDD[27]=s2;
		}
		else
			CDD[27]=s1;
		s1=CDD[28]; s2=CDD[29];
		for (j=28; j<56-Shift[i]; j++)
			CDD[j]=CDD[j+Shift[i]];
		if (Shift[i]==2)        
		{
			CDD[54]=s1;
			CDD[55]=s2;
		}
		else
			CDD[55]=s1;
		for (j=0; j<48; j++)
			SUBKEY[i][j]=CDD[PC_2[j]];
	}
}

void DES_setkey(unsigned char *key)
{
    int i;
    for (i=0; i<64; i++)
	KEY[i]=getbit(key,i);
    getsubkey();
}

/*src 明文,dst 密文, op -1 解密, 1 加密  */
void DES(unsigned char *src,unsigned char *dst,unsigned char *des_key,int op) 
{
    int i,j,k,r;
    unsigned char v4,v6;

    for (i=0;i<8;i++) deskey[i]=des_key[i];
    DES_setkey(deskey);
    if (op==1) r=0;
    else r=15;

    for (i=0; i<64; i++)
	LR0[i]=getbit(src,IIP[i]);

    for (i=0; i<16; i++)    
	{
		memcpy(LR1,LR0+32,32);
		memset(LR1+32,0,32);

		for (j=0; j<48; j++)
			bit48[j]=(SUBKEY[r][j]!=LR0[E[j]]);

		for (j=0; j<8; j++)     
		{                          /* S box */
			v6=0; v4=(j<<2)+(j<<1);
			for (k=5; k>=0; k--)
			v6=(v6<<1)+bit48[v4+k];
			v4=S[j][v6];
			v6=(j<<2)+3;
			for (k=0; k<4; k++)
			bit32[v6-k]=v4&1,v4>>=1;
		}
		for (j=0; j<32; j++)
			LR1[j+32]=(LR0[j]!=bit32[P[j]]);
		memcpy(LR0,LR1,64);
		r+=op;
	}
    memcpy(LR1,LR0+32,32);
    memcpy(LR1+32,LR0,32);
    memset(dst,0,8);
    for (i=0; i<64; i++)
	if (LR1[IIP_1[i]]) setbit(dst,i);
}

/* 采用CBC模式进行加密计算。并且进行填充
参数：	[in-out] datalen 输入为待加密数据src长度，输出为加密后数据 dst长度。
		src待加密数据
		[out] dst加密后数据。dst 空间由调用函数负责管理。
		key为密钥
返回值：0 正确，其他错误
 */
int DesEncipher(int *datalen, unsigned char *src,unsigned char *dst,unsigned char *key)
{
	unsigned char srcbuf[8];
	unsigned char dstbuf[8];

	int nleft;
	int i;

	memset(dstbuf,0,8);	
	nleft=*datalen;
	while(nleft>=8)
	{
		for(i=0;i<8;i++)
			srcbuf[i]=dstbuf[i]^src[*datalen-nleft+i];
		DES(srcbuf,dstbuf,key,1) ;	//加密
		memcpy(dst+*datalen-nleft,dstbuf,8);
		nleft-=8;
	}
	//最后分组
	for(i=0;i<nleft;i++)
		srcbuf[i]=dstbuf[i]^src[*datalen-nleft+i];
	//填充
	srcbuf[i]=dstbuf[i]^0x80;
	i++;
	for(;i<8;i++)
		srcbuf[i]=dstbuf[i]^0x0;
	DES(srcbuf,dstbuf,key,1) ;
	memcpy(dst+*datalen-nleft,dstbuf,8);
	*datalen=*datalen+8-nleft;
	return 0;
}
/* 采用CBC模式进行解密计算。并且去掉填充字节
参数：	[in-out] datalen 输入为待解密数据src长度，输出为解密后数据 dst有效长度。
		src待解密数据
		[out] dst解密后数据。dst 空间由调用函数负责管理。
		key为密钥
返回值：0 正确，其他错误
 */
int DesDecipher(int *datalen, unsigned char *src,unsigned char *dst,unsigned char *key)
{
	int nleft;
	int i;
	unsigned char srcbuf[8];
	unsigned char dstbuf[8];
	unsigned char tempbuf[8];

	memset(srcbuf,0,8);	
	nleft=*datalen;
	while(nleft>=8)
	{
		memcpy(tempbuf,src+*datalen-nleft,8);
		DES(tempbuf,dstbuf,key,-1) ;	//解密
		for(i=0;i<8;i++)
			dst[*datalen-nleft+i]=dstbuf[i]^srcbuf[i];
		memcpy(srcbuf,tempbuf,8);
		nleft-=8;
	}
	//去掉填充字节。
	for(i=1;i<9;i++)
		if(dst[*datalen-i]==0x80)break;
	if(i==9)
		return -110;
	//dst有效数据的实际长度为 *datalen-i .
	*datalen=*datalen-i;
	return 0;
}
/* 采用CBC模式进行计算加密校验和。并且进行填充
 参数：	inital为进行计算加密校验和的初始值，为8个字节。
		slen为计算加密校验和数据src长度
		src待计算加密校验和数据
		[out] dst加密校验和。dst 空间由调用函数负责管理(不小于8)。
返回值：0 正确，其他错误
 */
int DesChecksum(unsigned char *inital,int slen, unsigned char *src,unsigned char *dst,unsigned char *key)
{
	unsigned char buf[8];
	unsigned char dstbuf[8];
	int nleft;
	int i;
	memcpy(dstbuf,inital,8);	
	nleft=slen;
	while(nleft>=8)
	{
		for(i=0;i<8;i++)
			buf[i]=dstbuf[i]^src[slen-nleft+i];
		DES(buf,dstbuf,key,1) ;	//加密
		nleft-=8;
	}
	//最后分组
	for(i=0;i<nleft;i++)
		buf[i]=dstbuf[i]^src[slen-nleft+i];
	//填充
	buf[i++]=dstbuf[i]^0x80;
	for(;i<8;i++)
		buf[i]=dstbuf[i]^0x0;
	DES(buf,dstbuf,key,1) ;
	memcpy(dst,dstbuf,CRYPTO_CHECKSUM_LENGTH);
	return 0;
}
/* 采用金融IC卡规范进行加密计算。并且根据需要可能进行填充
参数：	[in-out] datalen 输入为待加密数据src长度(小于256)，输出为加密后数据 dst长度。
		src待加密数据,第一个字节指出数据长度,因此最多为255个数据。
		[out] dst加密后数据。dst 空间由调用函数负责管理。
		key为密钥
返回值：0 正确，其他错误
 */
int DesFinanceEncipher(int *datalen, unsigned char *src,unsigned char *dst,unsigned char *key)
{
	unsigned char srcbuf[8];
	int nleft;
	int i;

	nleft=*datalen;
	while(nleft>=8)
	{
		DES(src+*datalen-nleft,dst+*datalen-nleft,key,1) ;	//加密
		nleft-=8;
	}
	if(nleft>0)
	{
		//最后分组
		for(i=0;i<nleft;i++)
			srcbuf[i]=src[*datalen-nleft+i];
		//填充
		srcbuf[i++]=0x80;
		for(;i<8;i++)
			srcbuf[i]=0x0;
		DES(srcbuf,dst+*datalen-nleft,key,1) ;
		*datalen=*datalen+8-nleft;
	}
	return 0;
}
/* 采用金融IC卡规范进行解密计算。并且去掉填充字节。
参数：	[in-out] datalen 输入为待解密数据src长度，输出为解密后数据 dst有效长度。
		src待解密数据
		[out] dst解密后数据。dst 空间由调用函数负责管理。
		key为密钥
返回值：0 正确，其他错误
 */
int DesFinanceDecipher(int *datalen, unsigned char *src,unsigned char *dst,unsigned char *key)
{
	int nleft;
	int i;

	nleft=*datalen;
	while(nleft>=8)
	{
		DES(src+*datalen-nleft,dst+*datalen-nleft,key,-1) ;	//解密
		nleft-=8;
	}
	if(dst[0]==*datalen-1)  //无填充字节
		return 0;

	//去掉填充字节。
	for(i=1;i<9;i++)
		if(dst[*datalen-i]==0x80)break;
	if(i==9)
		return -110;
	//dst有效数据的实际长度为 *datalen-i .
	*datalen=*datalen-i;
	if(dst[0]!=*datalen-1) return -110;
	return 0;
}


/***********************  以下定义 3DES 算法的实现   ******************************
3DES加解密和密码校验和采用《金融IC卡规范》。
************************************************************************************/
/* 3DES 组加解密
参数：src 输入，长度为8 字节
	  dst 输出，长度为8 字节
	  des_key为密钥，长度为16字节
	  op 选项，-1 解密, 1 加密  
*/
void TripleDES(unsigned char *src,unsigned char *dst,unsigned char *des_key,int op) 
{
	unsigned char temp[8];
	if(op==1)
	{
		DES(src,dst,des_key,1);
		DES(dst,temp,des_key+8,-1);
		DES(temp,dst,des_key,1);
	}
	else
	{
		DES(src,dst,des_key,-1);
		DES(dst,temp,des_key+8,1);
		DES(temp,dst,des_key,-1);
	}
}
/* 3DES 采用CBC模式进行加密计算。并且进行填充
参数：	[in-out] datalen 输入为待加密数据src长度，输出为加密后数据 dst长度。
		src待加密数据
		[out] dst加密后数据。dst 空间由调用函数负责管理。
		key为密钥(16字节)
返回值：0 正确，其他错误
 */
int TripleDESEncipher(int *datalen, unsigned char *src,unsigned char *dst,unsigned char *key)
{
	unsigned char srcbuf[8];
	unsigned char dstbuf[8];
	int nleft;
	int i;

	memset(dstbuf,0,8);	
	nleft=*datalen;
	while(nleft>=8)
	{
		for(i=0;i<8;i++)
			srcbuf[i]=dstbuf[i]^src[*datalen-nleft+i];
		TripleDES(srcbuf,dstbuf,key,1) ;	//加密
		memcpy(dst+*datalen-nleft,dstbuf,8);
		nleft-=8;
	}
	//最后分组
	for(i=0;i<nleft;i++)
		srcbuf[i]=dstbuf[i]^src[*datalen-nleft+i];
	//填充
	srcbuf[i]=dstbuf[i]^0x80;
	i++;
	for(;i<8;i++)
		srcbuf[i]=dstbuf[i]^0x0;
	TripleDES(srcbuf,dstbuf,key,1) ;
	memcpy(dst+*datalen-nleft,dstbuf,8);
	if(*datalen<8) *datalen=8;
	else if(nleft==0) *datalen=*datalen+8;
	else *datalen=*datalen+8-nleft;
	return 0;
}
/* 3DES 采用CBC模式进行解密计算。并且去掉填充字节
参数：	[in-out] datalen 输入为待解密数据src长度，输出为解密后数据 dst有效长度。
		src待解密数据
		[out] dst解密后数据。dst 空间由调用函数负责管理。
		key为密钥(16字节)
返回值：0 正确，其他错误
 */
int TripleDESDecipher(int *datalen, unsigned char *src,unsigned char *dst,unsigned char *key)
{
	int nleft;
	int i;
	unsigned char srcbuf[8];
	unsigned char dstbuf[8];
	unsigned char tempbuf[8];

	memset(srcbuf,0,8);	
	nleft=*datalen;
	while(nleft>=8)
	{
		memcpy(tempbuf,src+*datalen-nleft,8);
		TripleDES(tempbuf,dstbuf,key,-1) ;	//解密
		for(i=0;i<8;i++)
			dst[*datalen-nleft+i]=dstbuf[i]^srcbuf[i];
		memcpy(srcbuf,tempbuf,8);
		nleft-=8;
	}
	i = 0;
	//去掉填充字节。

//hffan 2013-08-20 14:29:30
//	for(i=1;i<9;i++)
//		if(dst[*datalen-i]==0x80)break;
//	if(i==9)
//		return -110;

	//dst有效数据的实际长度为 *datalen-i .
	*datalen=*datalen-i;
	return 0;
}
/* 3DES采用计算加密校验和。并且进行填充(符合金融规范)
 参数：	inital 为进行计算加密校验和的初始值，为8个字节。
		slen为计算加密校验和数据src长度
		src待计算加密校验和数据
		[out] dst加密校验和。dst 空间由调用函数负责管理。
		key为密钥(16字节)
返回值：0 正确，其他错误
 */
int TripleDESChecksum(unsigned char *inital, int slen, unsigned char *src,unsigned char *dst,unsigned char *key)
{
	unsigned char buf[8];
	unsigned char dstbuf[8];
	int nleft;
	int i;
	memcpy(dstbuf,inital,8);	
	nleft=slen;
	while(nleft>=8)
	{
		for(i=0;i<8;i++)
			buf[i]=dstbuf[i]^src[slen-nleft+i];
		DES(buf,dstbuf,key,1) ;	//加密
		nleft-=8;
	}
	//最后分组
	for(i=0;i<nleft;i++)
		buf[i]=dstbuf[i]^src[slen-nleft+i];
	//填充
	buf[i++]=dstbuf[i]^0x80;
	for(;i<8;i++)
		buf[i]=dstbuf[i]^0x0;
	TripleDES(buf,dstbuf,key,1) ;  //仅在最后一组采用3DES
	memcpy(dst,dstbuf,CRYPTO_CHECKSUM_LENGTH);
	return 0;
}

/* 3DES 采用金融IC卡规范进行加密计算。并且根据需要可能进行填充
参数：	[in-out] datalen 输入为待加密数据src长度，输出为加密后数据 dst长度。
		src待加密数据,第一个字节指出数据长度,因此最多为255个数据。
		[out] dst加密后数据。dst 空间由调用函数负责管理。
		key为密钥(16字节)
返回值：0 正确，其他错误
 */
int TripleDESFinanceEncipher(int *datalen, unsigned char *src,unsigned char *dst,unsigned char *key)
{
	unsigned char srcbuf[8];
	int nleft;
	int i;

	nleft=*datalen;
	while(nleft>=8)
	{
		TripleDES(src+*datalen-nleft,dst+*datalen-nleft,key,1) ;	//加密
		nleft-=8;
	}
	if(nleft>0)
	{
		//最后分组
		for(i=0;i<nleft;i++)
			srcbuf[i]=src[*datalen-nleft+i];
		//填充
		srcbuf[i++]=0x80;
		for(;i<8;i++)
			srcbuf[i]=0x0;
		TripleDES(srcbuf,dst+*datalen-nleft,key,1) ;
		*datalen=*datalen+8-nleft;
	}
	return 0;
}
/* 3DES 采用金融IC卡规范进行解密计算。并且去掉填充字节
参数：	[in-out] datalen 输入为待解密数据src长度，输出为解密后数据 dst有效长度。
		src待解密数据
		[out] dst解密后数据。dst 空间由调用函数负责管理。
		key为密钥(16字节)
返回值：0 正确，其他错误
 */
int TripleDESFinanceDecipher(int *datalen, unsigned char *src,unsigned char *dst,unsigned char *key)
{
	int nleft;
	int i;
	
	nleft=*datalen;
	while(nleft>=8)
	{
		TripleDES(src+*datalen-nleft,dst+*datalen-nleft,key,-1) ;	//解密
		nleft-=8;
	}
	if(dst[0]==*datalen-1)  //无填充字节
		return 0;

	//去掉填充字节。
	for(i=1;i<9;i++)
		if(dst[*datalen-i]==0x80)break;
	if(i==9)
		return -110;
	//dst有效数据的实际长度为 *datalen-i .
	*datalen=*datalen-i;
	if(dst[0]!=*datalen-1) return -110;
	return 0;
}
