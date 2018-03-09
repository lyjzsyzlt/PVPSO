#pragma once
#ifndef functions_H
#define functions_H
#include"structs.h"

#include<iostream>
#include<set>
using namespace std;

int min(int a, int b, int c);
bool cmp(const job &J1, const job &J2);
bool cmp3(const batch &b1, const batch &b2);
bool cmp4(const batch &b1, const batch &b2);
void initJob(vector<job> &J);
void initBatch(batch &B, int id);
void getPbk(vector<job> J, vector<batch> &B);
void BFLPT(vector<job> J/*, vector<batch> &B*/);
void displayBatch(/*vector<batch> B*/);
void initMachine();

set<solution> updateNDS(solution sol, set<solution> NDS);
#endif // !functions_H
