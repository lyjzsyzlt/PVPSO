// PVPSO.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include"defines.h"
#include"PSO.h"
#include"functions.h"

#include<iostream>
#include<time.h>
#include<random>
#include<set>

using namespace std;

extern int t;

int main()
{
	srand((unsigned)time(NULL));
	vector<job> J;
	initJob(J);
	BFLPT(J);
	displayBatch();
	initMachine();
	PSO pso;

	ofstream solX("solX.txt", ios::app);

	for ( t = 1; t <= Tmax; t++)
	{
		solX << "第" << t << "代" << endl << endl;
		cout << "第" << t << "代：";
		pso.update();
	}

	ofstream NDS;
	NDS.open("NDS.txt", ios::app);
	set<solution> gbest = pso.getSolution();

	// 将gbest的内容写入文件
	NDS << "gBest.size==" << gbest.size() << endl;
	for (set<solution>::iterator it = gbest.begin(); it != gbest.end(); it++)
	{
		NDS << (*it).Cmax << '\t' << (*it).EPC << endl;
	}
	
	set<solution>::iterator it1 = gbest.begin();
	set<solution>::iterator it2 = gbest.end();
	it2--;
	int xmk = (*it1).Cmax + sigma * ((*it1).Cmax - (*it2).Cmax);
	int xepc = (*it2).EPC + sigma * ((*it2).EPC-(*it1).EPC);

	cout << "maxmk=" << (*it1).Cmax << '\t' << "minmk=" << (*it2).Cmax << endl;
	cout << "maxepc=" << (*it2).EPC << '\t' << "minepc=" << (*it1).EPC << endl;
	int sum = 0;
	for (it1; it1 != gbest.end(); it1++) 
	{
		sum += abs(((*it1).Cmax - xmk)*((*it1).EPC - xepc));
	}
	cout << "sum=" << sum << endl;
    return 0;
}

