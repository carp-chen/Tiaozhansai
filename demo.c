/***********************************************************
全国高校密码挑战赛：分组密码密钥恢复问题
J算法测试向量
代码修改时间：2022-4-14
************************************************************/
/***********************************************************
注：算法实现以16比特为基本数据单元（u16）;
明文、密文为2个u16构成的数组pt[2]，ct[2]，其中左支为Arr[0],右支为Arr[1];
种子密钥为4个u16构成的数组seedkey[4]，其中seedkey[i]=K[i],0<=i<=3.
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
