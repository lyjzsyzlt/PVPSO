#include "stdafx.h"
#include "Particle.h"

extern int batchNum;
extern Machine **M;
extern int machineNum[kmax + 1];//���ÿ���׶εĻ�������
extern vector<batch> B;
extern int t;

Particle::Particle()
{
}

void Particle::initParticle()
{
	// �������ƫ������
	Vmk = rand() % RAND_MAX / float(RAND_MAX);
	Vepc = 1 - Vmk;

	X.clear();
	V.clear();

	X.push_back(-1); // ʹ��X�Ǵ�1��ʼ���
	V.push_back(-1); //ʹ��V�Ǵ�1��ʼ���

	for (int i = 1; i < batchNum; i++)
	{
		float x = rand() % RAND_MAX / float(RAND_MAX);
		X.push_back(x);
		float v = rand() % RAND_MAX / float(RAND_MAX);
		V.push_back(v);
	}

	sol.X = X;
	sol.V = V;
	// ��ȡX��Ӧ��������
	vector<float> XTemp = X;
	sort(XTemp.begin() + 1, XTemp.end());// ��X��С��������
	int flag[1000];
	memset(flag, 0, sizeof(flag));
	sol.batchSeq.clear();

	sol.batchSeq.push_back(-1);
	for (int i = 1; i < batchNum ; i++)
	{
		for (int j = 1; j < batchNum; j++)
		{
			if (fabs(X[i] - XTemp[j])<= 1.0e-6 && (flag[j] == 0))
			{
				sol.batchSeq.push_back(j);
				flag[j] = 1;
				break;
			}
				
		}
	}
	out.open("LS_Process.txt", ios_base::out | ios_base::app);
	// ����Cmax��EPC
	getFitness();

}

void Particle::Right_Shift(vector<batch> tempB)
{
	int newEPC = 0;
	for (int k = kmax; k >= 1; k--)
	{
		vector<batch> tempBB = tempB;
		vector<int> D;
		D.clear();
		D.push_back(-1);
		int size = tempBB.size();
		for (int i = 1; i < size; i++)
			tempBB[i].ct = tempBB[i].BC[k];

		sort(tempBB.begin() + 1, tempBB.end(), cmp4);
		for (int i = 1; i < size; i++)
			D.push_back(tempBB[i].BId);

		for (int h = 1; h < size; h++)
		{
			int tmin = tempB[D[h]].BS[k];
			int STnext;
			int ST;//��ST(k+1)
			list<batch>::iterator it = M[k][tempB[D[h]].MID[k]].List.begin();
			for (; it != M[k][tempB[D[h]].MID[k]].List.end(); it++)
			{
				if ((*it).BId == tempB[D[h]].BId)
					break;
			}
				
			it++;
			if (it == M[k][tempB[D[h]].MID[k]].List.end())
				STnext = 9999999;
			else
				STnext = (*it).BS[k];
			if (k == kmax)
				ST = 9999999;
			else
				ST = tempB[D[h]].BS[k + 1];
			int tmax = min(sol.Cmax, STnext, ST) - ceil((float)tempB[D[h]].BP[k] / M[k][tempB[D[h]].MID[k]].V);
			//cout << "min(sol.Cmax, STnext, ST)" << sol.Cmax<<"   "<< STnext<<"    "<<ST<< min(sol.Cmax, STnext, ST) << endl;
			//cout << "�ӹ�ʱ��=" << ceil((float)tempB[D[h]].BP[k] / M[k][tempB[D[h]].MID[k]].V) << endl;
			//Ѱ����С��t
			int minEPC = 99999999;
			int t0;
			//cout << "tmin=" << tmin << "  tmax=" << tmax << endl;
			for (int t = tmin; t <= tmax; t++)
			{
				int tempEPC = getEPC1(t, ceil((float)tempB[D[h]].BP[k] / M[k][tempB[D[h]].MID[k]].V), k, tempB[D[h]].MID[k]);
				if (tempEPC <= minEPC)
				{
					minEPC = tempEPC;
					t0 = t;
				}

			}
			//cout << "t0=" << t0 << endl;
			tempB[D[h]].BS[k] = t0;//���¿�ʼ�ӹ�ʱ��
			tempB[D[h]].BC[k] = t0 + ceil((float)tempB[D[h]].BP[k] / M[k][tempB[D[h]].MID[k]].V);//�������ʱ��

																								 //���»�������
			it = M[k][tempB[D[h]].MID[k]].List.begin();
			for (; it != M[k][tempB[D[h]].MID[k]].List.end(); it++)
				if ((*it).BId == tempB[D[h]].BId)
				{
					(*it) = tempB[D[h]];
				}
		}
	}
	//����Ľ�������
	out << endl;
	out << "----------Right_Shift֮��-------------��" << endl;

	for (int k = 1; k < kmax + 1; k++)
	{
		for (int i = 1; i < machineNum[k] + 1; i++)
		{
			out << "��" << k << "�׶Σ���" << i << "̨������" << endl;
			list<batch>::iterator it = M[k][i].List.begin();
			for (; it != M[k][i].List.end(); it++)
				out << '\t' << "����" << (*it).BId << '\t' << "��ʼʱ�䣺" << (*it).BS[k] << '\t' << "���ʱ�䣺" << (*it).BC[k] << '\t' << "������" << (*it).MID[k] << endl;
		}
	}
	//����Ľ����EPC
	for (int k = 1; k < kmax + 1; k++)
	{
		for (int i = 1; i < machineNum[k] + 1; i++)
		{
			if (M[k][i].List.empty())
				newEPC += getEPC1(0, sol.Cmax, 0, 0);
			else
			{
				list<batch>::iterator it = M[k][i].List.begin();
				list<batch>::iterator it1;
				for (; it != M[k][i].List.end(); it++)
				{
					if (it == M[k][i].List.begin())
					{
						newEPC += getEPC1(0, (*it).BS[k], 0, 0);
						newEPC += getEPC1((*it).BS[k], ceil((float)(*it).BP[k] / M[k][i].V), k, i);
					}
					else
					{
						it1 = (--it);
						it++;
						newEPC += getEPC1((*it1).BC[k], (*it).BS[k] - (*it1).BC[k], 0, 0);
						newEPC += getEPC1((*it).BS[k], ceil((float)(*it).BP[k] / M[k][i].V), k, i);
					}
				}
				newEPC += getEPC1(M[k][i].List.back().BC[k], sol.Cmax - M[k][i].List.back().BC[k], 0, 0);
			}
		}
	}
	out << "newEPC=" << newEPC << '\t' << "Cmax=" << sol.Cmax << endl;
	sol.EPC = newEPC;
	out << "=================================================" << endl << endl;
}

int Particle::getEPC1(int t, int detaT, int k, int i)
{
	int n = (t + 3) / 24;
	if (t >= 24 * n - 3 && t < 24 * n + 7)
	{
		if (t + detaT >= 24 * n + 7)
			return (24 * n + 7 - t) * 5 * M[k][i].PW + getEPC1(24 * n + 7, detaT - (24 * n + 7 - t), k, i);
		else
			return detaT * 5 * M[k][i].PW;
	}
	if (t >= 24 * n + 7 && t < 24 * n + 11)
	{
		if (t + detaT >= 24 * n + 11)
			return (24 * n + 11 - t) * 8 * M[k][i].PW + getEPC1(24 * n + 11, detaT - (24 * n + 11 - t), k, i);
		else
			return detaT * 8 * M[k][i].PW;
	}
	if (t >= 24 * n + 11 && t < 24 * n + 17)
	{
		if (t + detaT >= 24 * n + 17)
			return (24 * n + 17 - t) * 10 * M[k][i].PW + getEPC1(24 * n + 17, detaT - (24 * n + 17 - t), k, i);
		else
			return detaT * 10 * M[k][i].PW;
	}
	if (t >= 24 * n + 17 && t < 24 * n + 21)
	{
		if (t + detaT >= 24 * n + 21)
			return (24 * n + 21 - t) * 8 * M[k][i].PW + getEPC1(24 * n + 21, detaT - (24 * n + 21 - t), k, i);
		else
			return detaT * 8 * M[k][i].PW;
	}
}

void Particle::getFitness()
{
	vector<int> SOL;//���ÿһ�׶ε������У�����ĸ���ǰ��ģ�
	SOL = sol.batchSeq;
	vector<batch> tempB = B;
	int EPC=0;
	int Cmax=0;

	//�����ֻ���ϲ����Ļ�����Ϣ
	for (int i = 1; i < kmax + 1; i++)
	{
		for (int j = 1; j < machineNum[i] + 1; j++)
		{
			M[i][j].avt = 0;
			M[i][j].List.clear();
		}
	}

	out << "---------------------------------------" << endl;

	int size = SOL.size();
	for (int i = 1; i < size; i++)
		out << SOL[i] << " ";
	out << endl;

	//����ÿ���׶�
	for (int k = 1; k < kmax + 1; k++)
	{
		//����ÿһ������ʹ��ÿ�����ڵ�ǰ�׶η����ں��ʵĻ�����
		for (int h = 1; h < size; h++)
		{
			float min = float(RAND_MAX);//����ʽ�ӵ���Сֵ
			int selectMachine = -1;//����ѡ��Ļ���
			float max1 = -RAND_MAX;
			float max2 = -RAND_MAX;

			int CT = tempB[SOL[h]].BC[k - 1];//��һ�׶ε����ʱ��

			for (int i = 1; i < machineNum[k] + 1; i++)
			{
				int temp1;
				
				temp1 = max(CT, M[k][i].avt) + ceil((float)tempB[SOL[h]].BP[k] / M[k][i].V) - CT;
				if (temp1 > max1)
					max1 = temp1;

				float temp2 = getEPC1(max(CT, M[k][i].avt), ceil((float)tempB[SOL[h]].BP[k] / M[k][i].V), k, i);
				if (temp2 > max2)
					max2 = temp2;
			}

			for (int i = 1; i < machineNum[k] + 1; i++)
			{
				float temp = Vmk * (max(CT, M[k][i].avt) + ceil((float)tempB[SOL[h]].BP[k] / M[k][i].V) - CT) / max1 + Vepc * getEPC1(max(CT, M[k][i].avt), ceil((float)tempB[SOL[h]].BP[k] / M[k][i].V), k, i) / max2;
				if (temp < min)
				{
					min = temp;
					selectMachine = i;
				}
			}

			//����������
			if (M[k][selectMachine].List.empty())
				EPC += getEPC1(0, max(CT, M[k][selectMachine].avt), 0, 0);
			else
				EPC += getEPC1(M[k][selectMachine].List.back().BC[k], max(CT, M[k][selectMachine].avt) - M[k][selectMachine].List.back().BC[k], 0, 0);
			//���㹤�����
			EPC += getEPC1(max(CT, M[k][selectMachine].avt), ceil(float(tempB[SOL[h]].BP[k]) / M[k][selectMachine].V), k, selectMachine);

			tempB[SOL[h]].BC[k] = max(CT, M[k][selectMachine].avt) + ceil(float(tempB[SOL[h]].BP[k]) / M[k][selectMachine].V);//��������������ʱ��
			tempB[SOL[h]].ct = tempB[SOL[h]].BC[k];//���ڵ�ǰ�׶ε��깤ʱ�䣬Ϊ�˺��������
			tempB[SOL[h]].BS[k] = max(CT, M[k][selectMachine].avt);//�������Ŀ�ʼʱ��
			tempB[SOL[h]].MID[k] = selectMachine;
			//����ѡ�����̨�����Ŀ���ʱ��
			M[k][selectMachine].avt = tempB[SOL[h]].BC[k];

			//�������������Ӧ�Ļ���������
			M[k][selectMachine].List.push_back(tempB[SOL[h]]);
		}//��ǰ�׶Σ����е������Ѿ��������

		 //�����е����������ʱ���������
		vector<batch>tempBB = tempB;
		sort(tempBB.begin() + 1, tempBB.end(), cmp3);

		Cmax = tempBB.back().BC[k];
		//��ȡ��һ���׶ε�������
		for (int j = 1; j < size; j++)
			SOL[j] = tempBB[j].BId;
	}//���н׶ζ��Ѿ����

	 //ǰ�������ʱ������û����ĳ̨�������һ������ɺ󣬻�Ҫ������ã���Ϊ��ʱ����֪��Cmax��
	for (int k = 1; k < kmax + 1; k++)
	{
		for (int i = 1; i < machineNum[k] + 1; i++)
		{
			//������û���κ�һ������һֱ���ڴ���״̬
			if (M[k][i].List.empty())
				EPC += getEPC1(0, Cmax, 0, 0);
			else
				EPC += getEPC1(M[k][i].List.back().BC[k], Cmax - M[k][i].List.back().BC[k], 0, 0);
		}
	}
	

	//���ÿ���׶�ÿ̨�����ϵ������
	for (int k = 1; k < kmax + 1; k++)
	{
		for (int i = 1; i < machineNum[k] + 1; i++)
		{
			out << "��" << k << "�׶Σ���" << i << "̨������" << endl;
			list<batch>::iterator it = M[k][i].List.begin();
			for (; it != M[k][i].List.end(); it++)
				out << '\t' << "����" << (*it).BId << '\t' << "��ʼʱ�䣺" << (*it).BS[k] << '\t' << "���ʱ�䣺" << (*it).BC[k] << '\t' << "������" << (*it).MID[k] << endl;
		}
	}
	//����ܵĵ�ѣ�Cmax
	out << "EPC=" << EPC << '\t' << "Cmax=" << Cmax << endl;
	sol.Cmax = Cmax;
	sol.EPC = EPC;

	//Right_Shift�Ľ�
	Right_Shift(tempB);

}


void Particle::updateBest()
{
	pbest = updateNDS(sol,pbest);
}

void Particle::updateVX()
{
	int pSelect = rand() % pbest.size();
	int gSelect = rand() % gbest.size();
	set<solution>::iterator pit = pbest.begin();
	set<solution>::iterator git = gbest.begin();

	for (int i = 0; pit != pbest.end(); pit++, i++)
		if (i == pSelect)
			break;

	for (int i = 0; git != gbest.end(); git++, i++)
		if (i == gSelect)
			break;

	for (int i = 1; i < batchNum; i++)
	{
		V[i] = (Wmax-(Wmax-Wmin)*t/Tmax) * V[i] + c1 * (rand() % RAND_MAX/ float(RAND_MAX))*((*pit).X[i] - X[i]) + c2 * (rand() % RAND_MAX / float(RAND_MAX))*((*git).X[i] - X[i]);
		X[i] = X[i] + V[i];

		if (V[i] < -50)
			V[i] = -rand() % RAND_MAX / float(RAND_MAX);
		else if (V[i] > 50)
			V[i] = rand() % RAND_MAX / float(RAND_MAX);

		if (X[i] < -30)
			X[i] = -10*(rand()%RAND_MAX/float(RAND_MAX));
		else if (X[i] > 30)
			X[i] = 10*(rand() % RAND_MAX / float(RAND_MAX));
	}



	sol.X = X;
	sol.V = V;
	vector<float> Xtemp = X;
	sort(Xtemp.begin() + 1, Xtemp.end());
	
	ofstream solX("solX.txt", ios::app);

	sol.batchSeq.clear();
	sol.batchSeq.push_back(-1);
	int flag[1000];
	memset(flag, 0, sizeof(flag));
	for (int i = 1; i < batchNum; i++)
	{
		solX << X[i] << "  ";
		for (int j = 1; j < batchNum; j++)
		{
			if (fabs(sol.X[i] - Xtemp[j]) < 1.0e-6&&flag[j] == 0)
			{
				sol.batchSeq.push_back(j);
				flag[j] = 1;
				break;
			}	
		}
	}

	//����
	if (rand() % RAND_MAX / float(RAND_MAX) < 0.2)
	{
		int s1 = rand() % (batchNum - 1) + 1;
		int s2 = rand() % (batchNum - 1) + 1;

		if (s1 > s2)
			swap(s1, s2);

		vector<int> batchSeq = sol.batchSeq;
		sol.batchSeq.clear();
		sol.batchSeq.push_back(-1);
		for (int i = s2; i < batchNum; i++)
		{
			sol.batchSeq.push_back(batchSeq[i]) ;
		}
		
		for (int i = 1; i < s1; i++)
			sol.batchSeq.push_back(batchSeq[i]);
		
		for (int i = s1; i < s2; i++)
			sol.batchSeq.push_back(batchSeq[i]);
	}
	solX << endl;
}

Particle::~Particle()
{
}
