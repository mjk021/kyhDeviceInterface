/**
 * \file myasn1.h
 *
 *  原来polarssl不支持asn1的编码，
 *  扩展支持asn1 (der) 编码解码接口，
 */
#ifndef _MY_ASN1_H_
#define _MY_ASN1_H_
//Bit8-bit7
/*用来标示 TAG 类型，共有四种，
分别是
universal(00)、
application(01)、
context-specific(10)和
private(11)。*/

#define ASN1_CLASS00_UNIVERSAL             0x00
#define ASN1_CLASS01_APPLICATION           0x40
#define ASN1_CLASS10_CONTEXT_SPECIFIC      0x80
#define ASN1_CLASS11_PRIVATE               0xC0

//bit6
//表示是否为结构类型(1位结构类型)；0则表明编码类型是简单类型。
#define ASN1_FLAG_CONSTRUCTED              0x20

/*
当CLASS = ASN1_CLASS_UNIVERSAL
bit5-bit1的值表示不同的universal的值
*/
//初始值，标准没有定义
#define ASN1_TAG00_ZERO                    0x00
// [UNIVERSAL 1] BOOLEAN [有两个值:false或true]
#define ASN1_TAG01_BOOLEAN                 0x01
// [UNIVERSAL 2] INTEGER [整型值]
#define ASN1_TAG02_INTEGER                 0x02
// [UNIVERSAL 3] BIT STRING [0位或多位]
#define ASN1_TAG03_BIT_STRING              0x03
// [UNIVERSAL 4] OCTET STRING [0字节或多字节]
#define ASN1_TAG04_OCTET_STRING            0x04
// [UNIVERSAL 5] NULL
#define ASN1_TAG05_NULL                    0x05
// [UNIVERSAL 6] OBJECT IDENTIFIER [相应于一个对象的独特标识数字]
#define ASN1_TAG06_OID                     0x06
// [UNIVERSAL 7] OBJECT DESCRIPTOR  [一个对象的简称]
//#define ASN1_TAG_OBJECT_DESCRIPTOR       0x07
// [UNIVERSAL 8] EXTERNAL, INSTANCE OF [ASN.1没有定义的数据类型]
//#define ASN1_TAG_EXTERNAL                0x08
// [UNIVERSAL 9] REAL [实数值]
//#define ASN1_TAG_REAL                    0x09
// [UNIVERSAL 10] ENUMERATED [数值列表，这些数据每个都有独特的标识符，作为ASN.1定义数据类型的一部分]
#define ASN1_TAG10_ENUMERATED              0x0A
//缺少 11的资料
//......
// [UNIVERSAL 12] UTF8String
#define ASN1_TAG12_UTF8_STRING             0x0C
//[UNIVERSAL 13] RELATIVE-OID
//......
//缺少 14的资料
//......
//缺少 15的资料
//......
// [UNIVERSAL 16] SEQUENCE, SEQUENCE OF [有序数列，SEQUENCE里面的每个数值都可以是不同类型的，而SEQUENCE OF里是0个或多个类型相同的数据]
#define ASN1_TAG16_SEQUENCE                0x10
// [UNIVERSAL 17] SET, SET OF [无序数列，SET里面的每个数值都可以是不同类型的，而SET OF里是0个或多个类型相同的数据]
#define ASN1_TAG17_SET                     0x11
// [UNIVERSAL 18] NumericString [0－9以及空格]
#define ASN1_TAG18_NUMERIC_STRING          0x12
// [UNIVERSAL 19] PrintableString [A-Z、a-z、0-9、空格以及符号'()+,-./:=?]
#define ASN1_TAG19_PRINTABLE_STRING        0x13
// [UNIVERSAL 20] TeletexString, T61String
#define ASN1_TAG20_T61_STRING              0x14
// [UNIVERSAL 21] VideotexString
#define ASN1_TAG21_VIDEOTEXSTRING          0x15
// [UNIVERSAL 22] IA5String
#define ASN1_TAG22_IA5_STRING              0x16
// [UNIVERSAL 23] UTCTime [统一全球时间格式]
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
当CLASS = ASN1_CLASS10_CONTEXT_SPECIFIC
bit5-bit1的值表示特殊内容
*/
//[0] –- 表示证书的版本
#define ASN1_TAG00_CERTVERSION              0x00
//[1] –- issuerUniqueID,表示证书发行者的唯一id
#define ASN1_TAG01_ISSUER_ID                0x01
//[2] –- subjectUniqueID,表示证书主体的唯一id
#define ASN1_TAG02_SUBJECT_ID               0x02
//[3] –- 表示证书的扩展字段
#define ASN1_TAG03_CERTVERSION              0x03

#define ASN1_MASK_CLASS                     0xC0
#define ASN1_MASK_TAG                       0x1F
#define ASN1_MASK_CONSTRUCTED               0x20


#ifdef __cplusplus
extern "C" {
#endif
/*
 * 解码asn1对象
 * return obj lentgh
 * 此函数不需要二次调用，返回指针也是输入参数的地址偏移
 * 成功 返回object长度，失败返回-1
 */
int myasn1_get_object(const unsigned char *p_der, //输入参数 der数据指针
					int der_length,//数据最大长度，如果超过此长度解析失败
					unsigned char *pClass,
					unsigned char *pFlag,
					unsigned char *pTag,
					unsigned char **pp_value,
					int *pvalue_length
					);
/*
* 编码asn1对象
* return obj lentgh
 * 此函数需要二次调用来获取真实长度
 * 成功 返回der编码长度，失败 返回-1
 */
int myasn1_put_object(const unsigned char *p_value, //输入数据指针
						int value_length,//数据最大长度
						unsigned char cClass,
						unsigned char cFlag,
						unsigned char cTag,
						unsigned char *p_der, //输出参数 der数据指针
						int *pder_length//输入输出der长度，如果为NULL，或 0就是二次调用
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
