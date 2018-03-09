#pragma once
#ifndef structs_H
#define structs_H

#include"defines.h"
#include<list>
#include<vector>
using namespace std;

// ����11
struct job
{
	int JobId;//�������
	int JobSize;//�����ߴ�
	int JobPT[kmax + 1];//�����ٲ�ͬ�׶εĴ���ʱ��,��1��ʼ���
};

// ��
struct batch
{
	int BId;//����ţ���1��ʼ

	int BP[kmax + 1];//���ٲ�ͬ�׶����崦��ʱ��,��1��ʼ
	int BC[kmax + 1];//�������ڲ�ͬ�׶ε����ʱ��
	int JNum;//���й�������
	int freeSpace;//��¼����ʣ��ռ�
	vector<job> BJob;//�������ŵĹ���
	float SI;//������б��
	int flag;//1��ʾ������û�м������
	int ct;//��ǰ�׶ε��깤ʱ��
	int BS[kmax + 1];//����ͬ�׶εĿ�ʼʱ��
	int MID[kmax + 1];//��ÿ���׶����ĸ�������
};

struct solution
{
	vector<int> batchSeq;
	float Vmk;
	float Vepc;
	int Cmax;
	int EPC;
	vector<float> X;
	vector<float> V;
	bool operator<(const struct solution & right)const
	{
		if (this->batchSeq == right.batchSeq&&Vmk == right.Vmk&&Vepc == right.Vepc)
			return false;
		else
		{
			if (Cmax != right.Cmax)
				return Cmax > right.Cmax;
			else
				return EPC > right.EPC;
		}
	}
};
struct Machine
{
	int V;//�����׶β�ͬ�������ٶ�
	int PW;//���������ڲ�ͬ�׶��µĹ�������
	int avt;//��k�׶Σ�ÿ�������ĵ�ǰ����ʱ��
	list<batch> List;//��¼�����ϼӹ�����
};
#endif // !structs_H
