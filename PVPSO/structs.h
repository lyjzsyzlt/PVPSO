#pragma once
#ifndef structs_H
#define structs_H

#include"defines.h"
#include<list>
#include<vector>
using namespace std;

// 工件11
struct job
{
	int JobId;//工件编号
	int JobSize;//工件尺寸
	int JobPT[kmax + 1];//工件再不同阶段的处理时间,从1开始编号
};

// 批
struct batch
{
	int BId;//批编号，从1开始

	int BP[kmax + 1];//批再不同阶段名义处理时间,从1开始
	int BC[kmax + 1];//各个批在不同阶段的完成时间
	int JNum;//批中工件个数
	int freeSpace;//记录批的剩余空间
	vector<job> BJob;//批里面存放的工件
	float SI;//批的倾斜度
	int flag;//1表示该批还没有加入队列
	int ct;//当前阶段的完工时间
	int BS[kmax + 1];//批不同阶段的开始时间
	int MID[kmax + 1];//批每个阶段在哪个机器上
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
	int V;//各个阶段不同机器的速度
	int PW;//各个机器在不同阶段下的工作功率
	int avt;//在k阶段，每个机器的当前可用时间
	list<batch> List;//记录机器上加工的批
};
#endif // !structs_H
