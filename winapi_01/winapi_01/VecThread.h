#pragma once
#ifndef _VECTHREAD_H
#define _VECTHREAD_H

#include <Windows.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <vector>

#include "mainwin.h"
#include "IObserver.h"

using namespace std;

namespace spectral
{

class CMutex
{
public:
	mutex* cmtx;
public: 
	CMutex();
	~CMutex();
};

class CEquation : private CMutex
{
private:
	CMutex* cmtx;
public:
	CEquation(CMutex* mtx);
	~CEquation();
public:
	void Equation(int x);
};

class VecThread : public IObserver, private CMutex
{
private:
	IObservable* mainwin;
	CMutex* cmtx;
	int countThread;
public:
	VecThread(IObservable* obj, CMutex* mtx);
	~VecThread();
public:
	void updateCountThread(int p) override;
	void updateCountThread(map<string, int> p) override;
	void CreateThread(int countThread);
	void CreateThread(int countThread,int max,int min,int step);
};

}

#endif