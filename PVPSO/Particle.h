#pragma once
#ifndef Particle_H
#define Particle_H

#include"structs.h"
#include"functions.h"

#include<algorithm>
#include<vector>
#include<fstream>
#include<set>
using namespace std;

class Particle
{
public:
	vector<float> X;// �������ڵ�λ�ã��������
	vector<float> V;//���ӵ�ǰ���ٶ�
	solution sol; // ���ӵ�ǰ�Ľ�
	set<solution> pbest;// �����ӵ�ĿǰΪֹ��õĽ�
	set<solution> gbest; // �������ӵ�Ŀǰ����ý�
	float Vmk; // ����깤ʱ��ƫ��
	float Vepc; // �����ɱ�ƫ��
	ofstream out;

	Particle();
	void initParticle();// ��ʼ�����ӵ���ز���
	void Right_Shift(vector<batch> tempB);
	int getEPC1(int t, int detaT, int k, int i);
	void getFitness(); // ����ÿ�����ӵ���Ӧ��
	void updateBest(); // �������ӵ�pbest
	void updateVX(); // �������ӵ��ٶȺ�λ��
	~Particle();
};

#endif // !Particle_H