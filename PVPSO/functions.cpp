#include"stdafx.h"
#include"functions.h"
#include"structs.h"

#include<iostream>
#include<fstream>
#include<time.h>
#include<random>
#include<set>

using namespace std;

int batchNum;//���ĸ���
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
	return J1.JobPT[1] > J2.JobPT[1];//���յ�һ�׶εļӹ�ʱ������
}
bool cmp3(const batch &b1, const batch &b2)
{
	return b1.ct < b2.ct;//�������ʱ������
}

bool cmp4(const batch &b1, const batch &b2)
{
	return b1.ct > b2.ct;//�������ʱ�併��
}


// ��ʼ����������Ϣ
void initJob(vector<job> &J)
{
	J.clear();
	ofstream out1("jobInfo.txt", ios::ate);
	out1 << "";

	ofstream out("jobInfo.txt", ios::app);
	default_random_engine dre((unsigned)time(NULL));// ��ʼ�����������
	uniform_int_distribution<int> uid(1, 10);// ʹ�ø����ķ�Χ��ʼ���ֲ���
	out << "����id" << '\t' << "������С" << '\t';
	for (int i = 1; i < kmax + 1; i++)
	out << "��" << i << "�׶�����ӹ�ʱ��" << '\t';
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

// ��ʼ�����Ļ�����Ϣ
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

// ���������������ӹ�ʱ��
void getPbk(vector<job> J, vector<batch> &B)
{
	int size = B.size();
	for (int i = 1; i < size; i++) //��i����
	{
		for (int p = 1; p < kmax + 1; p++)// ��p���׶�
		{
			int maxPT = -999999;
			for (int j = 0; j < B[i].JNum; j++)// ��j������
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

// ����
void BFLPT(vector<job> J/*, vector<batch> &B*/)
{
	B.clear();
	sort(J.begin(), J.end(), cmp);//�Թ������ռӹ�ʱ��ǵ���˳������
	int c = 0;
	for (vector<job>::iterator it = J.begin(); it != J.end(); it++)
	{
		if ((*it).JobSize >= 5)
			c++;
	}
	cout << "c=" << c << endl;
	//��ҪΪ��ʹBʹ��1��ʼ���
	batch tempB;
	initBatch(tempB, -1);
	tempB.freeSpace = -10;
	tempB.flag = 0;
	B.push_back(tempB);

	vector<job>::iterator it2 = J.begin();//���������ĵ�����
	int initBid = 1;//���ĳ�ʼ���

	for (it2; it2 != J.end(); it2++)//�������еĹ���
	{
		int min = 9999999;//��С�ĵ�ʣ��ռ�
		int minBid = -1;//��Ӧ�������
		vector<batch>::iterator it = B.begin() + 1;//�������ĵ�����
		for (it; it != B.end(); it++)//Ϊÿ����������һ�ε�ǰ��������
		{
			if ((*it).freeSpace >= (*it2).JobSize)//��ǰ��������㹻�����������
			{
				if ((*it).freeSpace - (*it2).JobSize < min)//�жϷ���֮���Ƿ�ʹ�����������������������ʣ��ռ���С
				{
					min = (*it).freeSpace - (*it2).JobSize;
					minBid = (*it).BId;
				}
			}
		}
		//���½���һ����
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
		// ������������������������
		else
		{
			B[minBid].freeSpace -= (*it2).JobSize;
			B[minBid].JNum++;
			B[minBid].BJob.push_back((*it2));
		}
	}
	batchNum = B.size();
	getPbk(J, B);//��ȫ�����������׶ε�����ӹ�ʱ��
}

// ��������������Ĺ���
void displayBatch(/*vector<batch> B*/)//������const vector<batch> B
{
	int size = B.size();
	for (int i = 1; i<size; i++) {
		cout << "��" << i << "�����ı�ţ�" << B[i].BId << endl;;
		cout << "��" << i << "����������������" << B[i].JNum << endl;
		cout << "��" << i << "�����������Ĺ���Ϊ(��ţ�:";

		for (int p = 0; p < B[i].JNum; p++) {
			cout << B[i].BJob[p].JobId << "  ";
		}
		cout << endl;

		for (int j = 1; j < kmax + 1; j++) {//k���׶�
			cout << "    ��" << j << "�׶ε�����ӹ�ʱ��:" << B[i].BP[j] << endl;
		}
	}
}


Machine **M;
int machineNum[kmax + 1];//���ÿ���׶εĻ�������

void initMachine()
{
	ifstream  in2;

	//��ʼ��ÿ���׶λ���������
	in2.open("machineNum.txt", ios::in | ios::binary);
	for (int i = 1; i<kmax + 1; i++)
		in2 >> machineNum[i];
	M = new Machine*[kmax + 1];
	for (int i = 0; i < kmax + 1; i++)
		M[i] = new Machine[10];
	M[0][0].PW = 1;

	default_random_engine dre((unsigned)time(NULL));// ��ʼ�����������
	uniform_int_distribution<int> v1(1, 5);// ʹ�ø����ķ�Χ��ʼ���ֲ���
	uniform_int_distribution<int> pw(5, 10);
	ofstream out1("machineInfo.txt", ios::ate);
	out1 << "";
	ofstream out("machineInfo.txt", ios::app);
	out << "�׶�" << '\t' << "������" << '\t' << "�ٶ�" << '\t' << "����" << endl;

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


int t;//��ǰ�����Ĵ���