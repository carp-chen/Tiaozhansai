/***********************************************************
ȫ����У������ս��������������Կ�ָ�����
J�㷨��������
�����޸�ʱ�䣺2022-4-14
************************************************************/
/***********************************************************
ע���㷨ʵ����16����Ϊ�������ݵ�Ԫ��u16��;
���ġ�����Ϊ2��u16���ɵ�����pt[2]��ct[2]��������֧ΪArr[0],��֧ΪArr[1];
������ԿΪ4��u16���ɵ�����seedkey[4]������seedkey[i]=K[i],0<=i<=3.
*************************************************************/
#include "Tiaozhansai2022.h"

int main(void)
{
	u16 seedkey[4];
	u16 pt[2], ct[2];

	pt[0] = 2;
	pt[1] = 1;
	// seedkey=0xfedcba9876543210
	seedkey[0] = 0x3210;
	seedkey[1] = 0x7654;
	seedkey[2] = 0xba98;
	seedkey[3] = 0xfedc;

	// int count = 100;
	// u16 Rk0;
	// printf("Rk0:%04x\n", Rk0);

	// Rk0 = func1(seedkey, count);
	// printf("func1-Rk0:0x%04x\n", Rk0);

	// Rk0 = func2(seedkey);
	// printf("func2-Rk0:0x%04x\n", Rk0);

	Enc(pt, ct, seedkey, 1);
	printf("ct = %.4x %.4x\n", ct[0], ct[1]);

	Enc(ct, pt, seedkey, 1);
	printf("pt = %.4x %.4x\n", pt[0], pt[1]);

	system("pause");
	return 0;
}
