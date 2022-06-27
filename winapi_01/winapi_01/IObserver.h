#pragma once
#ifndef _IOBSERVER_H
#define _IOBSERVER_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;


namespace spectral
{


class IObserver
{
public:
    virtual void updateCountThread(int p);
    virtual void updateCountThread(map<string, int> p);
};

class IObservable
{
public:
    virtual void addObserver(IObserver* o);
    virtual void removeObserver(IObserver* o);
    virtual void notify(pair<string, int> p);
    virtual void notify(map<string, int> p);
    virtual ~IObservable();
};


}


#endif