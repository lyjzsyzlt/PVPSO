#include"stdafx.h"
#include"functions.h"
#include"structs.h"

#include<iostream>
#include<fstream>
#include<time.h>
#include<random>
#include<set>

using namespace std;

int batchNum;//批的个数
vector<batch> B;

int min(int a, int b, int c)
{
	int m;
	if (a < b)
		m = a;
	else
		m = b;
	if (c < m)
		m = c;
	return m;
}

bool cmp(const job &J1, const job &J2)
{
	return J1.JobPT[1] > J2.JobPT[1];//按照第一阶段的加工时间排序
}
bool cmp3(const batch &b1, const batch &b2)
{
	return b1.ct < b2.ct;//按照完成时间升序
}

bool cmp4(const batch &b1, const batch &b2)
{
	return b1.ct > b2.ct;//按照完成时间降序
}


// 初始化工件的信息
void initJob(vector<job> &J)
{
	J.clear();
	ofstream out1("jobInfo.txt", ios::ate);
	out1 << "";

	ofstream out("jobInfo.txt", ios::app);
	default_random_engine dre((unsigned)time(NULL));// 初始化随机数引擎
	uniform_int_distribution<int> uid(1, 10);// 使用给出的范围初始化分布类
	out << "工件id" << '\t' << "工件大小" << '\t';
	for (int i = 1; i < kmax + 1; i++)
	out << "第" << i << "阶段名义加工时间" << '\t';
	out << endl;

	for (int i = 1; i < JobNum + 1; i++)
	{
	job tempj;
	tempj.JobId = i;
	tempj.JobSize = uid(dre);
	out << tempj.JobId << '\t' << tempj.JobSize << '\t';

	uniform_int_distribution<int> pt(10, 50);
	for (int j = 1; j < kmax + 1; j++) {
	tempj.JobPT[j] = pt(dre);

	out << tempj.JobPT[j] << '\t';
	}
	out << endl;

	J.push_back(tempj);

	}
}

// 初始化批的基本信息
void initBatch(batch &B, int id)
{
	for (int i = 1; i < kmax + 1; i++)
	{
		B.BId = id;
		B.BP[i] = -1;
		B.freeSpace = C;
		B.JNum = 0;
		B.flag = 1;
		B.BC[0] = 0;
	}
}

// 计算各个批的名义加工时间
void getPbk(vector<job> J, vector<batch> &B)
{
	int size = B.size();
	for (int i = 1; i < size; i++) //第i个批
	{
		for (int p = 1; p < kmax + 1; p++)// 第p个阶段
		{
			int maxPT = -999999;
			for (int j = 0; j < B[i].JNum; j++)// 第j个工件
			{
				if (B[i].BJob[j].JobPT[p] > maxPT)
				{
					maxPT = B[i].BJob[j].JobPT[p];
				}
			}
			B[i].BP[p] = maxPT;
		}
	}
}

// 分批
void BFLPT(vector<job> J/*, vector<batch> &B*/)
{
	B.clear();
	sort(J.begin(), J.end(), cmp);//对工件按照加工时间非递增顺序排序
	int c = 0;
	for (vector<job>::iterator it = J.begin(); it != J.end(); it++)
	{
		if ((*it).JobSize >= 5)
			c++;
	}
	cout << "c=" << c << endl;
	//主要为了使B使从1开始编号
	batch tempB;
	initBatch(tempB, -1);
	tempB.freeSpace = -10;
	tempB.flag = 0;
	B.push_back(tempB);

	vector<job>::iterator it2 = J.begin();//工件向量的迭代器
	int initBid = 1;//批的初始编号

	for (it2; it2 != J.end(); it2++)//遍历所有的工件
	{
		int min = 9999999;//最小的的剩余空间
		int minBid = -1;//对应的批编号
		vector<batch>::iterator it = B.begin() + 1;//批向量的迭代器
		for (it; it != B.end(); it++)//为每个工件遍历一次当前的批集合
		{
			if ((*it).freeSpace >= (*it2).JobSize)//当前的这个批足够放下这个工件
			{
				if ((*it).freeSpace - (*it2).JobSize < min)//判断放入之后，是否使得这个批在整个集合中满足剩余空间最小
				{
					min = (*it).freeSpace - (*it2).JobSize;
					minBid = (*it).BId;
				}
			}
		}
		//重新建立一个批
		if (minBid == -1)
		{
			batch tempB;
			initBatch(tempB, initBid);
			tempB.freeSpace -= (*it2).JobSize;
			tempB.JNum++;
			tempB.BJob.push_back((*it2));
			initBid++;
			B.push_back(tempB);
		}
		// 将这个工件放入符合条件的批
		else
		{
			B[minBid].freeSpace -= (*it2).JobSize;
			B[minBid].JNum++;
			B[minBid].BJob.push_back((*it2));
		}
	}
	batchNum = B.size();
	getPbk(J, B);//补全各个批各个阶段的名义加工时间
}

// 输出各个批包含的工件
void displayBatch(/*vector<batch> B*/)//不能是const vector<batch> B
{
	int size = B.size();
	for (int i = 1; i<size; i++) {
		cout << "第" << i << "个批的编号：" << B[i].BId << endl;;
		cout << "第" << i << "个批所含工件数：" << B[i].JNum << endl;
		cout << "第" << i << "个批所包含的工件为(编号）:";

		for (int p = 0; p < B[i].JNum; p++) {
			cout << B[i].BJob[p].JobId << "  ";
		}
		cout << endl;

		for (int j = 1; j < kmax + 1; j++) {//k个阶段
			cout << "    第" << j << "阶段的名义加工时间:" << B[i].BP[j] << endl;
		}
	}
}


Machine **M;
int machineNum[kmax + 1];//存放每个阶段的机器数量

void initMachine()
{
	ifstream  in2;

	//初始化每个阶段机器的数量
	in2.open("machineNum.txt", ios::in | ios::binary);
	for (int i = 1; i<kmax + 1; i++)
		in2 >> machineNum[i];
	M = new Machine*[kmax + 1];
	for (int i = 0; i < kmax + 1; i++)
		M[i] = new Machine[10];
	M[0][0].PW = 1;

	default_random_engine dre((unsigned)time(NULL));// 初始化随机数引擎
	uniform_int_distribution<int> v1(1, 5);// 使用给出的范围初始化分布类
	uniform_int_distribution<int> pw(5, 10);
	ofstream out1("machineInfo.txt", ios::ate);
	out1 << "";
	ofstream out("machineInfo.txt", ios::app);
	out << "阶段" << '\t' << "机器号" << '\t' << "速度" << '\t' << "功率" << endl;

	for (int i = 1; i < kmax + 1; i++)
	{

		for (int j = 1; j < machineNum[i] + 1; j++)
		{
			out <<  i << '\t';
			out <<j<< '\t';
			M[i][j].V = v1(dre);
			M[i][j].PW = pw(dre);
			M[i][j].avt = 0;
			M[i][j].List.clear();

			out << M[i][j].V << '\t' << M[i][j].PW << endl;
		}
	}
}


set<solution> updateNDS(solution sol, set<solution> NDS)
{
	solution tempSol=sol;
	
	int flag = 0;
	set<solution>::iterator it = NDS.begin();
	for (; it != NDS.end(); )
	{
		if (tempSol.Cmax > (*it).Cmax&&tempSol.EPC > (*it).EPC) {
			flag = 1;
			break;
		}
		else if (tempSol.Cmax < (*it).Cmax&&tempSol.EPC < (*it).EPC) {
			it = NDS.erase(it);
		}
		else if (tempSol.Cmax == (*it).Cmax&&tempSol.EPC == (*it).EPC) {
			flag = 1;
			break;
		}
		else if (tempSol.Cmax == (*it).Cmax || (*it).EPC == tempSol.EPC) {
			if (tempSol.Cmax >(*it).Cmax || tempSol.EPC >(*it).EPC) {
				flag = 1;
				break;
			}
			else {
				it = NDS.erase(it);
			}
		}
		else
			it++;
	}
	if (flag == 0)
		NDS.insert(tempSol);

	return NDS;
}


int t;//当前迭代的代数