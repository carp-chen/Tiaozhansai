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
#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#define TOTALROUND 40 //ע�����㷨����������������Թ����������������֡��˴���40Ϊ�����ɸ���
#define u16 uint16_t
#define LCS(x, a) ((x) << a ^ (x) >> (16 - a)) //��16���ش�ѭ������aλ

void KeySchedual(u16 seedkey[], u16 roundkey[]) //��Կ��չ�㷨
{
	int idx;

#if (TOTALROUND > 8)
	for (idx = 0; idx < 4; idx++)
	{
		roundkey[2 * idx] = seedkey[idx];
		roundkey[2 * idx + 1] = ~seedkey[idx];
	}
	for (idx = 8; idx < TOTALROUND; idx++)
	{
		if ((idx & 1) == 0)
			roundkey[idx] = ~roundkey[idx - 8] ^ ((idx - 8) >> 1); //��1.10�İ汾�����޸�
		else
			roundkey[idx] = ~roundkey[idx - 1];
	}
#else
	for (idx = 0; idx < TOTALROUND; idx++)
	{
		if ((idx & 1) == 0)
			roundkey[idx] = seedkey[idx >> 1];
		else
			roundkey[idx] = ~roundkey[idx - 1];
	}
#endif
}

void RoundFun(u16 s[], u16 roundkey[], int round) //�ֺ���,sΪ����״̬�����s[0]��roundkeyΪ����Կ��roundΪ��ǰ�ֱ�ţ���0��ʼ�ǣ�
{
	u16 temp;

	temp = s[0] & roundkey[round] ^ s[1] ^ round;
	temp = temp << 2 & temp << 1 ^ temp;				// S
	temp = LCS(temp, 3) ^ LCS(temp, 9) ^ LCS(temp, 14); // P
	s[0] ^= temp;
	s[1] ^= temp & roundkey[round];
	temp = s[0];
	s[0] = s[1];
	s[1] = temp; //������֧  ////��1.10�İ汾�����޸�
}

void Enc(u16 pt[], u16 ct[], u16 seedkey[], int round) //���ܺ�����ptΪ���ģ�ctΪ���ģ�seedkeyΪ������Կ��roundΪ������
{
	u16 s[2];
	u16 roundkey[TOTALROUND];
	int r;
	FILE *fp;

	fp = fopen("TestVector.txt", "w");
	if (fp == NULL)
	{
		printf("�ļ���ʧ��\n");
		return;
	}

	if (round > TOTALROUND)
	{
		printf("error: ��Կ��չ����С���������������ͷ�ļ���15��ʹTOTALROUND��ֵ����������\n");
		return;
	}

	KeySchedual(seedkey, roundkey);
	// for (r = 0; r < round; r++)
	// 	printf("%.4x\n", roundkey[r]);

	s[0] = pt[0];
	s[1] = pt[1];
	for (r = 0; r < round; r++)
	{
		RoundFun(s, roundkey, r);
		// printf("%.4x %.4x\n", s[0], s[1]);
		fprintf(fp, "%2d, %.4x, %.4x %.4x\n", r, roundkey[r], s[0], s[1]);
	}
	ct[0] = s[1];
	ct[1] = s[0]; //���һ�ֲ�����  ////��1.10�İ汾�����޸�

	fclose(fp);
}

//第一轮方法1
u16 round1_func1(u16 seedkey[], int count)
{
	srand((unsigned)time(NULL));
	u16 pt[2] = {0x0000, 0x0000};
	u16 ct[2];
	int T_j[16] = {0};
	u16 Rk0 = 0;
	for (int i = 0; i < count; i++)
	{
		pt[0] = rand() % 65536;
		pt[1] = rand() % 65536;
		Enc(pt, ct, seedkey, 1);
		u16 T = pt[0] ^ pt[1] ^ ct[0] ^ ct[1];
		//若?的第j位为1，则可断定??0的第j位为0
		for (int j = 0; j < 16; j++)
		{
			if ((T >> j) & 1)
			{
			}
			else
			{
				T_j[j]++;
			}
		}
	}
	for (int i = 0; i < 16; i++)
	{
		//超过80%的T的第j位为0，则可断定??0的第j位为1
		if (T_j[i] > 0.8 * count)
		{
			Rk0 ^= 1 << i;
		}
	}
	return Rk0;
}

//第一轮方法2
u16 round1_func2(u16 seedkey[])
{

	srand((unsigned)time(NULL));
	u16 pt[2] = {0x0000, 0x0000};
	pt[0] = rand() % 65536;
	pt[1] = rand() % 65536;
	u16 ct[2];
	u16 ct_[2];
	u16 Rk0 = 0;

	for (int i = 0; i < 16; i++)
	{
		u16 e = 1 << i;
		u16 pt_[2] = {0x0000, 0x0000};
		pt_[0] = pt[0] ^ e;
		pt_[1] = pt[1];
		Enc(pt, ct, seedkey, 1);
		Enc(pt_, ct_, seedkey, 1);
		if ((ct[0] ^ ct_[0]) == e)
		{
		}
		else
		{
			Rk0 ^= 1 << i;
		}
	}
	return Rk0;
}

u16 round2_func(u16 seedkey[])
{
	u16 pt[2];
	u16 ct[2];

	u16 pt_[2];
	u16 ct_[2];

	u16 x = 0x2111;

	pt[0] = 0x0000;
	pt[1] = 0;
	pt_[0] = pt[0] ^ x;
	pt_[1] = 0;

	Enc(pt, ct, seedkey, 1);
	Enc(pt_, ct_, seedkey, 1);

	u16 y = ct[0] ^ ct_[0];
	printf("y:%04x\n", y);

	// u16 x = 0x2000;
	// u16 y = 0x2841;
	int Rk_num[65536] = {0};
	u16 ans = 0;
	int max = 0;
	for (int i = 0; i <= 100; i++)
	{
		pt[0] = 0;
		pt[1] = i;

		pt_[0] = pt[0] ^ x;
		pt_[1] = i;

		Enc(pt, ct, seedkey, 2);
		Enc(pt_, ct_, seedkey, 2);

		for (int j = 0; j < 65536; j++)
		{
			u16 ct2[2];
			ct2[0] = ct[0];
			ct2[1] = ct[1];
			u16 ct2_[2];
			ct2_[0] = ct_[0];
			ct2_[1] = ct_[1];
			u16 roundkey[] = {0x0000, j};
			RoundFun(ct2, roundkey, 1);
			RoundFun(ct2_, roundkey, 1);
			if ((ct2[0] ^ ct2_[0]) == y)
			{
				Rk_num[j]++;
			}
			if (Rk_num[j] > max)
			{
				max = Rk_num[j];
				ans = j;
			}
		}
	}
	return ans;
}

u16 round3_func(u16 seedkey[])
{

	u16 pt[2];
	u16 ct[2];

	u16 pt_[2];
	u16 ct_[2];

	u16 x = 0x2111;

	pt[0] = 0x0000;
	pt[1] = 0;
	pt_[0] = pt[0] ^ x;
	pt_[1] = 0;

	Enc(pt, ct, seedkey, 2);
	Enc(pt_, ct_, seedkey, 2);

	u16 y_0 = ct[0] ^ ct_[0];
	u16 y_1 = ct[1] ^ ct_[1];

	int Rk_num[65536] = {0};
	u16 ans = 0;
	int max = 0;
	for (int i = 0; i <= 1000; i++)
	{
		pt[0] = 0;
		pt[1] = i;

		pt_[0] = pt[0] ^ x;
		pt_[1] = i;

		Enc(pt, ct, seedkey, 3);
		Enc(pt_, ct_, seedkey, 3);

		for (int j = 0; j < 65536; j++)
		{
			u16 ct2[2];
			ct2[0] = ct[0];
			ct2[1] = ct[1];
			u16 ct2_[2];
			ct2_[0] = ct_[0];
			ct2_[1] = ct_[1];
			u16 roundkey[] = {0x0000, 0x0000, j};
			RoundFun(ct2, roundkey, 2);
			RoundFun(ct2_, roundkey, 2);
			if ((ct2[0] ^ ct2_[0]) == y_0 && (ct2[1] ^ ct2_[1]) == y_1)
			{
				Rk_num[j]++;
			}
			if (Rk_num[j] > max)
			{
				max = Rk_num[j];
				ans = j;
			}
		}
	}
	return ans;
}

u16 round4_func(u16 seedkey[])
{

	u16 pt[2];
	u16 ct[2];

	u16 pt_[2];
	u16 ct_[2];

	u16 x = 0x2111;

	pt[0] = 0x0000;
	pt[1] = 0;
	pt_[0] = pt[0] ^ x;
	pt_[1] = 0;

	Enc(pt, ct, seedkey, 2);
	Enc(pt_, ct_, seedkey, 2);

	u16 y_0 = ct[0] ^ ct_[0];
	u16 y_1 = ct[1] ^ ct_[1];

	int Rk_num[65536] = {0};
	u16 ans = 0;
	int max = 0;
	for (int i = 0; i <= 1000; i++)
	{
		pt[0] = 0;
		pt[1] = i;

		pt_[0] = pt[0] ^ x;
		pt_[1] = i;

		Enc(pt, ct, seedkey, 3);
		Enc(pt_, ct_, seedkey, 3);

		for (int j = 0; j < 65536; j++)
		{
			u16 ct2[2];
			ct2[0] = ct[0];
			ct2[1] = ct[1];
			u16 ct2_[2];
			ct2_[0] = ct_[0];
			ct2_[1] = ct_[1];
			u16 roundkey[] = {0x0000, 0x0000, j};
			RoundFun(ct2, roundkey, 2);
			RoundFun(ct2_, roundkey, 2);
			if ((ct2[0] ^ ct2_[0]) == y_0 && (ct2[1] ^ ct2_[1]) == y_1)
			{
				Rk_num[j]++;
			}
			if (Rk_num[j] > max)
			{
				max = Rk_num[j];
				ans = j;
			}
		}
	}
	return ans;
}