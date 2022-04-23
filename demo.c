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

	pt[0] = 0x2002;
	pt[1] = 3;
	// seedkey=0xfedcba9876543210
	seedkey[0] = 0x3210;
	seedkey[1] = 0x7654;
	seedkey[2] = 0xba98;
	seedkey[3] = 0xfedc;

	// int count = 100;
	// u16 Rk0;
	// printf("Rk0:%04x\n", Rk0);

	// Rk0 = round1_func1(seedkey, count);
	// printf("func1-Rk0:0x%04x\n", Rk0);

	// Rk0 = round1_func2(seedkey);
	// printf("func2-Rk0:0x%04x\n", Rk0);

	Enc(pt, ct, seedkey, 2);
	printf("ct = %.4x %.4x\n", ct[0], ct[1]);

	u16 rounkey[] = {0x0000, 0xcdef};

	RoundFun(ct, rounkey, 1);
	printf("pt = %.4x %.4x\n", ct[0], ct[1]);

	// u16 x = 0xce3b ^ 0xe67a;
	// printf("x:%04x\n", x);

	u16 Rk1 = round2_func(seedkey);
	printf("Rk1:%04x\n", Rk1);
	// system("pause");
	return 0;
}
