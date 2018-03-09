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
	set<solution> gbest; // �������ӵ�Ŀǰ����ý�
	int particleNum; //���ӵ�����
	Particle *particle; //��������
	
public:
	PSO();
	void getGbest();
	void update();
	set<solution> getSolution();
	~PSO();
};

#endif
