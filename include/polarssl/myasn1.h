/**
 * \file myasn1.h
 *
 *  ԭ��polarssl��֧��asn1�ı��룬
 *  ��չ֧��asn1 (der) �������ӿڣ�
 */
#ifndef _MY_ASN1_H_
#define _MY_ASN1_H_
//Bit8-bit7
/*������ʾ TAG ���ͣ��������֣�
�ֱ���
universal(00)��
application(01)��
context-specific(10)��
private(11)��*/

#define ASN1_CLASS00_UNIVERSAL             0x00
#define ASN1_CLASS01_APPLICATION           0x40
#define ASN1_CLASS10_CONTEXT_SPECIFIC      0x80
#define ASN1_CLASS11_PRIVATE               0xC0

//bit6
//��ʾ�Ƿ�Ϊ�ṹ����(1λ�ṹ����)��0��������������Ǽ����͡�
#define ASN1_FLAG_CONSTRUCTED              0x20

/*
��CLASS = ASN1_CLASS_UNIVERSAL
bit5-bit1��ֵ��ʾ��ͬ��universal��ֵ
*/
//��ʼֵ����׼û�ж���
#define ASN1_TAG00_ZERO                    0x00
// [UNIVERSAL 1] BOOLEAN [������ֵ:false��true]
#define ASN1_TAG01_BOOLEAN                 0x01
// [UNIVERSAL 2] INTEGER [����ֵ]
#define ASN1_TAG02_INTEGER                 0x02
// [UNIVERSAL 3] BIT STRING [0λ���λ]
#define ASN1_TAG03_BIT_STRING              0x03
// [UNIVERSAL 4] OCTET STRING [0�ֽڻ���ֽ�]
#define ASN1_TAG04_OCTET_STRING            0x04
// [UNIVERSAL 5] NULL
#define ASN1_TAG05_NULL                    0x05
// [UNIVERSAL 6] OBJECT IDENTIFIER [��Ӧ��һ������Ķ��ر�ʶ����]
#define ASN1_TAG06_OID                     0x06
// [UNIVERSAL 7] OBJECT DESCRIPTOR  [һ������ļ��]
//#define ASN1_TAG_OBJECT_DESCRIPTOR       0x07
// [UNIVERSAL 8] EXTERNAL, INSTANCE OF [ASN.1û�ж������������]
//#define ASN1_TAG_EXTERNAL                0x08
// [UNIVERSAL 9] REAL [ʵ��ֵ]
//#define ASN1_TAG_REAL                    0x09
// [UNIVERSAL 10] ENUMERATED [��ֵ�б���Щ����ÿ�����ж��صı�ʶ������ΪASN.1�����������͵�һ����]
#define ASN1_TAG10_ENUMERATED              0x0A
//ȱ�� 11������
//......
// [UNIVERSAL 12] UTF8String
#define ASN1_TAG12_UTF8_STRING             0x0C
//[UNIVERSAL 13] RELATIVE-OID
//......
//ȱ�� 14������
//......
//ȱ�� 15������
//......
// [UNIVERSAL 16] SEQUENCE, SEQUENCE OF [�������У�SEQUENCE�����ÿ����ֵ�������ǲ�ͬ���͵ģ���SEQUENCE OF����0������������ͬ������]
#define ASN1_TAG16_SEQUENCE                0x10
// [UNIVERSAL 17] SET, SET OF [�������У�SET�����ÿ����ֵ�������ǲ�ͬ���͵ģ���SET OF����0������������ͬ������]
#define ASN1_TAG17_SET                     0x11
// [UNIVERSAL 18] NumericString [0��9�Լ��ո�]
#define ASN1_TAG18_NUMERIC_STRING          0x12
// [UNIVERSAL 19] PrintableString [A-Z��a-z��0-9���ո��Լ�����'()+,-./:=?]
#define ASN1_TAG19_PRINTABLE_STRING        0x13
// [UNIVERSAL 20] TeletexString, T61String
#define ASN1_TAG20_T61_STRING              0x14
// [UNIVERSAL 21] VideotexString
#define ASN1_TAG21_VIDEOTEXSTRING          0x15
// [UNIVERSAL 22] IA5String
#define ASN1_TAG22_IA5_STRING              0x16
// [UNIVERSAL 23] UTCTime [ͳһȫ��ʱ���ʽ]
#define ASN1_TAG23_UTC_TIME                0x17
// [UNIVERSAL 24] GeneralizedTime
#define ASN1_TAG24_GENERALIZED_TIME        0x18
// [UNIVERSAL 25] GraphicString
#define ASN1_TAG25_GRAPHICSTRING           0x19
// [UNIVERSAL 26] VisibleString, ISO646String
#define ASN1_TAG26_ISO64STRING             0x1A
#define ASN1_TAG26_VISIBLESTRING           0x1A/* alias */
// [UNIVERSAL 27] GeneralString
#define ASN1_TAG27_GENERALSTRING           0x1B
// [UNIVERSAL 28] UniversalString
#define ASN1_TAG28_UNIVERSAL_STRING        0x1C
// [UNIVERSAL 29] CHARACTER STRING
#define ASN1_TAG29_CHARACTER_STRING        0x1D
// [UNIVERSAL 30] BMPString
#define ASN1_TAG30_BMP_STRING              0x1E
// [UNIVERSAL 31]... reserved for future use
#define ASN1_TAG31_RESERVED                0x1F


/*
��CLASS = ASN1_CLASS10_CONTEXT_SPECIFIC
bit5-bit1��ֵ��ʾ��������
*/
//[0] �C- ��ʾ֤��İ汾
#define ASN1_TAG00_CERTVERSION              0x00
//[1] �C- issuerUniqueID,��ʾ֤�鷢���ߵ�Ψһid
#define ASN1_TAG01_ISSUER_ID                0x01
//[2] �C- subjectUniqueID,��ʾ֤�������Ψһid
#define ASN1_TAG02_SUBJECT_ID               0x02
//[3] �C- ��ʾ֤�����չ�ֶ�
#define ASN1_TAG03_CERTVERSION              0x03

#define ASN1_MASK_CLASS                     0xC0
#define ASN1_MASK_TAG                       0x1F
#define ASN1_MASK_CONSTRUCTED               0x20


#ifdef __cplusplus
extern "C" {
#endif
/*
 * ����asn1����
 * return obj lentgh
 * �˺�������Ҫ���ε��ã�����ָ��Ҳ����������ĵ�ַƫ��
 * �ɹ� ����object���ȣ�ʧ�ܷ���-1
 */
int myasn1_get_object(const unsigned char *p_der, //������� der����ָ��
					int der_length,//������󳤶ȣ���������˳��Ƚ���ʧ��
					unsigned char *pClass,
					unsigned char *pFlag,
					unsigned char *pTag,
					unsigned char **pp_value,
					int *pvalue_length
					);
/*
* ����asn1����
* return obj lentgh
 * �˺�����Ҫ���ε�������ȡ��ʵ����
 * �ɹ� ����der���볤�ȣ�ʧ�� ����-1
 */
int myasn1_put_object(const unsigned char *p_value, //��������ָ��
						int value_length,//������󳤶�
						unsigned char cClass,
						unsigned char cFlag,
						unsigned char cTag,
						unsigned char *p_der, //������� der����ָ��
						int *pder_length//�������der���ȣ����ΪNULL���� 0���Ƕ��ε���
						);

#ifdef _MY_ASN1_TEST_
/**
 * \brief          Checkup routine
 *
 * \return         0 if successful, or 1 if the test failed
 */
int asn1_self_test( int verbose );
#endif //#ifdef _MY_ASN1_TEST_
#ifdef __cplusplus
}
#endif

#endif /* _MY_ASN1_H_ */
