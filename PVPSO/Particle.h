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
	vector<float> X;// 粒子现在的位置，随机产生
	vector<float> V;//粒子当前的速度
	solution sol; // 粒子当前的解
	set<solution> pbest;// 该粒子到目前为止最好的解
	set<solution> gbest; // 所有粒子到目前的最好解
	float Vmk; // 最大完工时间偏好
	float Vepc; // 电力成本偏好
	ofstream out;

	Particle();
	void initParticle();// 初始化粒子的相关参数
	void Right_Shift(vector<batch> tempB);
	int getEPC1(int t, int detaT, int k, int i);
	void getFitness(); // 计算每个粒子的适应度
	void updateBest(); // 更新粒子的pbest
	void updateVX(); // 更新粒子的速度和位置
	~Particle();
};

#endif // !Particle_H