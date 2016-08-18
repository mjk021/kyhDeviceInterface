#ifndef _MYRSA_H_
#define _MYRSA_H_

// ����polarsslû�еĺ���

int RSA_padding_add_PKCS1_type_2(unsigned char *to, int tlen, 
		const unsigned char *from, int flen);

int RSA_padding_add_PKCS1_type_1(unsigned char *to, int tlen,
	    const unsigned char *from, int flen);

int RSA_padding_check_PKCS1_type_2(unsigned char *to, int tlen,
		const unsigned char *from, int flen, int num);


#endif