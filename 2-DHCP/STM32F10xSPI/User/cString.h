#ifndef CSTRING_H_
#define CSTRING_H_
#include <stm32f10x.h>
#ifndef CSTRING_C_//���û�ж���
#define CSTRING_Ex_ extern
#else
#define CSTRING_Ex_
#endif

void cStringFree(void);
void cStringRestore(void);
char *StrBetwString(char *Str,char *StrBegin,char *StrEnd);
int split(char *src,const char *separator,char **dest,int DestLen);


/**
* @brief  16����ת�ַ�����ʾ
* @param  pHex       Դ16����
* @param  pAscii     ת�����
* @param  nLen       ����
* @param  None       
* @retval None
* @example 
**/
void HexToAscii(unsigned char *pHex, unsigned char *pAscii, int nLen);
/**
* @brief  �����������
* @param  temp   Դ����
* @param  len    ��ǰ���������ݵĳ���    
* @param  k      Ҫ�����λ��
* @param  data   ���������     
* @retval None
* @example 
**/
void insert(char *temp, int len,int k, char data);


#endif

