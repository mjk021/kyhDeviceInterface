/*  ����DES��ʵ�ֺ���
	��д�� �½���
	��Ȩ����  2001.5
  */

#ifndef _DES_H_2001_05_12
#define _DES_H_2001_05_12

/* DES�㷨ʵ��
������src ���룬����Ϊ8 �ֽ�
	  dst ���������Ϊ8 �ֽ�
	  des_keyΪ��Կ������Ϊ8�ֽ�
	  op ѡ�-1 ����, 1 ����  
*/
void DES(unsigned char *src,unsigned char *dst,unsigned char *des_key,int op) ;

/* ����CBCģʽ���м��ܼ��㡣���ҽ������
������	[in-out] datalen ����Ϊ����������src���ȣ����Ϊ���ܺ����� dst���ȡ�
		src����������
		[out] dst���ܺ����ݡ�dst �ռ��ɵ��ú����������
		keyΪ��Կ
����ֵ��0 ��ȷ����������
 */
int DesEncipher(int *datalen, unsigned char *src,unsigned char *dst,unsigned char *key);

/* ����CBCģʽ���н��ܼ��㡣����ȥ������ֽ�
������	[in-out] datalen ����Ϊ����������src���ȣ����Ϊ���ܺ����� dst��Ч���ȡ�
		src����������
		[out] dst���ܺ����ݡ�dst �ռ��ɵ��ú����������
		keyΪ��Կ
����ֵ��0 ��ȷ����������
 */
int DesDecipher(int *datalen, unsigned char *src,unsigned char *dst,unsigned char *key);

/* ����CBCģʽ���м������У��͡����ҽ������
 ������	initalΪ���м������У��͵ĳ�ʼֵ��Ϊ8���ֽڡ�
		slenΪ�������У�������src����
		src���������У�������
		[out] dst����У��͡�dst �ռ��ɵ��ú����������(��С��8)��
����ֵ��0 ��ȷ����������
 */
int DesChecksum(unsigned char *inital,int slen, unsigned char *src,unsigned char *dst,unsigned char *key);

/* ���ý���IC���淶���м��ܼ��㡣���Ҹ�����Ҫ���ܽ������
������	[in-out] datalen ����Ϊ����������src����(С��256)�����Ϊ���ܺ����� dst���ȡ�
		src����������,��һ���ֽ�ָ�����ݳ���,������Ϊ255�����ݡ�
		[out] dst���ܺ����ݡ�dst �ռ��ɵ��ú����������
		keyΪ��Կ
����ֵ��0 ��ȷ����������
 */
int DesFinanceEncipher(int *datalen, unsigned char *src,unsigned char *dst,unsigned char *key);

/* ���ý���IC���淶���н��ܼ��㡣����ȥ������ֽڡ�
������	[in-out] datalen ����Ϊ����������src���ȣ����Ϊ���ܺ����� dst��Ч���ȡ�
		src����������
		[out] dst���ܺ����ݡ�dst �ռ��ɵ��ú����������
		keyΪ��Կ
����ֵ��0 ��ȷ����������
 */
int DesFinanceDecipher(int *datalen, unsigned char *src,unsigned char *dst,unsigned char *key);


/***********************  ���¶��� 3DES �㷨��ʵ��   ******************************
3DES�ӽ��ܺ�����У��ͣ���������У��Ͳ��á�����IC���淶����
************************************************************************************/
/* 3DES ��ӽ���
������src ���룬����Ϊ8 �ֽ�
	  dst ���������Ϊ8 �ֽ�
	  des_keyΪ��Կ������Ϊ16�ֽ�
	  op ѡ�-1 ����, 1 ����  
*/
void TripleDES(unsigned char *src,unsigned char *dst,unsigned char *des_key,int op);
/* 3DES ����CBCģʽ���м��ܼ��㡣���ҽ������
������	[in-out] datalen ����Ϊ����������src���ȣ����Ϊ���ܺ����� dst���ȡ�
		src����������
		[out] dst���ܺ����ݡ�dst �ռ��ɵ��ú����������
		keyΪ��Կ(16�ֽ�)
����ֵ��0 ��ȷ����������
 */
int TripleDESEncipher(int *datalen, unsigned char *src,unsigned char *dst,unsigned char *key);
/* 3DES ����CBCģʽ���н��ܼ��㡣����ȥ������ֽ�
������	[in-out] datalen ����Ϊ����������src���ȣ����Ϊ���ܺ����� dst��Ч���ȡ�
		src����������
		[out] dst���ܺ����ݡ�dst �ռ��ɵ��ú����������
		keyΪ��Կ(16�ֽ�)
����ֵ��0 ��ȷ����������
 */
int TripleDESDecipher(int *datalen, unsigned char *src,unsigned char *dst,unsigned char *key);

/* 3DES���ü������У��͡����ҽ������(���Ͻ��ڹ淶)
/* 3DES���ü������У��͡����ҽ������(���Ͻ��ڹ淶)
 ������	inital Ϊ���м������У��͵ĳ�ʼֵ��Ϊ8���ֽڡ�
		slenΪ�������У�������src����
		src���������У�������
		[out] dst����У��͡�dst �ռ��ɵ��ú����������
		keyΪ��Կ(16�ֽ�)
����ֵ��0 ��ȷ����������
 */
int TripleDESChecksum(unsigned char *inital, int slen, unsigned char *src,unsigned char *dst,unsigned char *key);

/* 3DES ���ý���IC���淶���м��ܼ��㡣���Ҹ�����Ҫ���ܽ������
������	[in-out] datalen ����Ϊ����������src���ȣ����Ϊ���ܺ����� dst���ȡ�
		src����������
		[out] dst���ܺ����ݡ�dst �ռ��ɵ��ú����������
		keyΪ��Կ(16�ֽ�)
����ֵ��0 ��ȷ����������
 */
int TripleDESFinanceEncipher(int *datalen, unsigned char *src,unsigned char *dst,unsigned char *key);
/* 3DES ���ý���IC���淶���н��ܼ��㡣����ȥ������ֽ�
������	[in-out] datalen ����Ϊ����������src���ȣ����Ϊ���ܺ����� dst��Ч���ȡ�
		src����������
		[out] dst���ܺ����ݡ�dst �ռ��ɵ��ú����������
		keyΪ��Կ(16�ֽ�)
����ֵ��0 ��ȷ����������
 */
int TripleDESFinanceDecipher(int *datalen, unsigned char *src,unsigned char *dst,unsigned char *key);

#endif
