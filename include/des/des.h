#ifndef __DES_FOR_FTSAFE_H__
#define __DES_FOR_FTSAFE_H__

#ifndef WIN32
#define WINAPI
#endif

void WINAPI arr_inttohex(unsigned long int num , unsigned char arr_hex[] , int arr_len) ;
	
unsigned long int WINAPI  arr_hextoint(unsigned char arr_hexin[], int arrlen) ;
	
unsigned char WINAPI  asc_to_hex(char   ic_cmd[], unsigned char resp[], int len );
	
bool WINAPI  hex_to_asc(unsigned char hex_in[],  char data_back[], int len);
	
int WINAPI  TripleDesEnc(unsigned char *outdata, unsigned char *indata, int datalen, const unsigned char *key);
	
int  WINAPI TripleDesDec(unsigned char *outdata, unsigned char *indata, int datalen, const unsigned char *key);
	
bool WINAPI  GetDriverKey(const unsigned char MasterKey[], unsigned char ASN[] ,unsigned char DriverKey[] )	;
	
void WINAPI  DesDec(unsigned char *outdata, unsigned char *indata, int datalen, const unsigned char *key);
	
void WINAPI   DesEnc(unsigned char *outdata, unsigned char *indata, int datalen, const unsigned char *key);
	
bool WINAPI  mac_des(const unsigned char *byKey, const unsigned char *initvalue, unsigned char *datain, unsigned char *MAC, int byLen);
	
bool  WINAPI mac_3des(const unsigned char byKey[], const unsigned char initvalue[], unsigned char datain[], unsigned char MAC[],int byLen);
	
bool WINAPI  arr_HexCompare(unsigned char hex_a[] , unsigned char hex_b[] , int arr_len);
	
void  WINAPI KeyXors(unsigned char *drivekey, unsigned char *key_out );

void WINAPI  TurnString(unsigned char *pIn, int iInLen, int iLen);
	
void WINAPI  arr_int2TyteArr( int num , unsigned char bArr[], int iArrLen, int iType, bool bFlag);
	
int  WINAPI arr_TyteArr2int(unsigned char bArr[], int iArrLen, int iType, bool bFlag);
	
int WINAPI  pad80(unsigned char *DataIn , int *iDataLen);

bool WINAPI AddPKCSPadding(unsigned char* pData, unsigned long block_size, unsigned long data_len, unsigned long total_len);

bool WINAPI StripPKCSPadding(unsigned char* pData, unsigned long block_size,unsigned long total_len, unsigned long*  pData_len);

void WINAPI Xor(unsigned char *src,unsigned char *dst,unsigned short Len);

void WINAPI CommDesEnc(unsigned char *src,unsigned char *dst,unsigned char *key,unsigned int Len);

void WINAPI CommDesEncRet(unsigned char *src,unsigned char *dst,unsigned char *key,unsigned int Len);

#endif //__DES_FOR_FTSAFE_H__
