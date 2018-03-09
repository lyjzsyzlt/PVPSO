#include "stdafx.h"
#include "PSO.h"

extern int t;

PSO::PSO()
{
	particleNum = Num;

	particle = new Particle[particleNum+1];
	// ��ʼÿ�����ӵ�ƫ�������������У�pbest
	for (int i = 1; i < particleNum + 1; i++)
	{
		particle[i].initParticle();
		particle[i].updateBest();
	}
	// �������Ⱥ��gbest
	getGbest();

	for (int i = 1; i < particleNum + 1; i++)
		particle[i].gbest = gbest;

}

void PSO::getGbest()
{

	for (int i = 1; i < particleNum + 1; i++)
	{
		for (set<solution>::iterator it = particle[i].pbest.begin(); it != particle[i].pbest.end(); it++)
		{
			gbest = updateNDS((*it), gbest);
		}
	}
}

void PSO::update()
{
	ofstream solX("solX.txt", ios::app);

	for (int i = 1; i < particleNum + 1; i++)
	{
		solX << "��" << i << "������" << "**************************************" << endl;
		particle[i].updateVX(); 
		particle[i].getFitness();
		particle[i].updateBest();
	}
	getGbest();
	for (int i = 1; i < particleNum + 1; i++)
		particle[i].gbest = gbest;

	cout << "gBest.size==" << gbest.size() << endl;

	// ���ѡ��10���������³�ʼ��
	if (t > 80&&rand()%RAND_MAX/float(RAND_MAX)<0.2)
	{
		for (int i = 1; i <= 10; i++)
		{
			int select = rand() % particleNum + 1;
			particle[select].initParticle();
			particle[select].updateBest();
		}
		getGbest();
	}
}

set<solution> PSO::getSolution()
{
	return gbest;
}


PSO::~PSO()
{
}
