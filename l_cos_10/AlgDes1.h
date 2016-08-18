/*  定义DES的实现函数
	编写： 陈建华
	版权所有  2001.5
  */

#ifndef _DES_H_2001_05_12
#define _DES_H_2001_05_12

/* DES算法实现
参数：src 输入，长度为8 字节
	  dst 输出，长度为8 字节
	  des_key为密钥，长度为8字节
	  op 选项，-1 解密, 1 加密  
*/
void DES(unsigned char *src,unsigned char *dst,unsigned char *des_key,int op) ;

/* 采用CBC模式进行加密计算。并且进行填充
参数：	[in-out] datalen 输入为待加密数据src长度，输出为加密后数据 dst长度。
		src待加密数据
		[out] dst加密后数据。dst 空间由调用函数负责管理。
		key为密钥
返回值：0 正确，其他错误
 */
int DesEncipher(int *datalen, unsigned char *src,unsigned char *dst,unsigned char *key);

/* 采用CBC模式进行解密计算。并且去掉填充字节
参数：	[in-out] datalen 输入为待解密数据src长度，输出为解密后数据 dst有效长度。
		src待解密数据
		[out] dst解密后数据。dst 空间由调用函数负责管理。
		key为密钥
返回值：0 正确，其他错误
 */
int DesDecipher(int *datalen, unsigned char *src,unsigned char *dst,unsigned char *key);

/* 采用CBC模式进行计算加密校验和。并且进行填充
 参数：	inital为进行计算加密校验和的初始值，为8个字节。
		slen为计算加密校验和数据src长度
		src待计算加密校验和数据
		[out] dst加密校验和。dst 空间由调用函数负责管理(不小于8)。
返回值：0 正确，其他错误
 */
int DesChecksum(unsigned char *inital,int slen, unsigned char *src,unsigned char *dst,unsigned char *key);

/* 采用金融IC卡规范进行加密计算。并且根据需要可能进行填充
参数：	[in-out] datalen 输入为待加密数据src长度(小于256)，输出为加密后数据 dst长度。
		src待加密数据,第一个字节指出数据长度,因此最多为255个数据。
		[out] dst加密后数据。dst 空间由调用函数负责管理。
		key为密钥
返回值：0 正确，其他错误
 */
int DesFinanceEncipher(int *datalen, unsigned char *src,unsigned char *dst,unsigned char *key);

/* 采用金融IC卡规范进行解密计算。并且去掉填充字节。
参数：	[in-out] datalen 输入为待解密数据src长度，输出为解密后数据 dst有效长度。
		src待解密数据
		[out] dst解密后数据。dst 空间由调用函数负责管理。
		key为密钥
返回值：0 正确，其他错误
 */
int DesFinanceDecipher(int *datalen, unsigned char *src,unsigned char *dst,unsigned char *key);


/***********************  以下定义 3DES 算法的实现   ******************************
3DES加解密和密码校验和，其中密码校验和采用《金融IC卡规范》。
************************************************************************************/
/* 3DES 组加解密
参数：src 输入，长度为8 字节
	  dst 输出，长度为8 字节
	  des_key为密钥，长度为16字节
	  op 选项，-1 解密, 1 加密  
*/
void TripleDES(unsigned char *src,unsigned char *dst,unsigned char *des_key,int op);
/* 3DES 采用CBC模式进行加密计算。并且进行填充
参数：	[in-out] datalen 输入为待加密数据src长度，输出为加密后数据 dst长度。
		src待加密数据
		[out] dst加密后数据。dst 空间由调用函数负责管理。
		key为密钥(16字节)
返回值：0 正确，其他错误
 */
int TripleDESEncipher(int *datalen, unsigned char *src,unsigned char *dst,unsigned char *key);
/* 3DES 采用CBC模式进行解密计算。并且去掉填充字节
参数：	[in-out] datalen 输入为待解密数据src长度，输出为解密后数据 dst有效长度。
		src待解密数据
		[out] dst解密后数据。dst 空间由调用函数负责管理。
		key为密钥(16字节)
返回值：0 正确，其他错误
 */
int TripleDESDecipher(int *datalen, unsigned char *src,unsigned char *dst,unsigned char *key);

/* 3DES采用计算加密校验和。并且进行填充(符合金融规范)
/* 3DES采用计算加密校验和。并且进行填充(符合金融规范)
 参数：	inital 为进行计算加密校验和的初始值，为8个字节。
		slen为计算加密校验和数据src长度
		src待计算加密校验和数据
		[out] dst加密校验和。dst 空间由调用函数负责管理。
		key为密钥(16字节)
返回值：0 正确，其他错误
 */
int TripleDESChecksum(unsigned char *inital, int slen, unsigned char *src,unsigned char *dst,unsigned char *key);

/* 3DES 采用金融IC卡规范进行加密计算。并且根据需要可能进行填充
参数：	[in-out] datalen 输入为待加密数据src长度，输出为加密后数据 dst长度。
		src待加密数据
		[out] dst加密后数据。dst 空间由调用函数负责管理。
		key为密钥(16字节)
返回值：0 正确，其他错误
 */
int TripleDESFinanceEncipher(int *datalen, unsigned char *src,unsigned char *dst,unsigned char *key);
/* 3DES 采用金融IC卡规范进行解密计算。并且去掉填充字节
参数：	[in-out] datalen 输入为待解密数据src长度，输出为解密后数据 dst有效长度。
		src待解密数据
		[out] dst解密后数据。dst 空间由调用函数负责管理。
		key为密钥(16字节)
返回值：0 正确，其他错误
 */
int TripleDESFinanceDecipher(int *datalen, unsigned char *src,unsigned char *dst,unsigned char *key);

#endif
