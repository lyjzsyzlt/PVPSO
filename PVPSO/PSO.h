#pragma once
#ifndef PSO_H
#define PSO_H

#include"structs.h"
#include"Particle.h"
#include"functions.h"
#include<fstream>
#include<set>
using namespace std;

class PSO
{
private:
	set<solution> gbest; // 所有粒子到目前的最好解
	int particleNum; //粒子的数量
	Particle *particle; //粒子数组
	
public:
	PSO();
	void getGbest();
	void update();
	set<solution> getSolution();
	~PSO();
};

#endif
