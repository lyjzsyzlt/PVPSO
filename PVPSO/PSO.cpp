#include "stdafx.h"
#include "PSO.h"

extern int t;

PSO::PSO()
{
	particleNum = Num;

	particle = new Particle[particleNum+1];
	// 初始每个粒子的偏好向量，批序列，pbest
	for (int i = 1; i < particleNum + 1; i++)
	{
		particle[i].initParticle();
		particle[i].updateBest();
	}
	// 获得粒子群的gbest
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
		solX << "第" << i << "个粒子" << "**************************************" << endl;
		particle[i].updateVX(); 
		particle[i].getFitness();
		particle[i].updateBest();
	}
	getGbest();
	for (int i = 1; i < particleNum + 1; i++)
		particle[i].gbest = gbest;

	cout << "gBest.size==" << gbest.size() << endl;

	// 随机选择10个粒子重新初始化
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
