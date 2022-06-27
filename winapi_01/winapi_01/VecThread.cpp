
#include "VecThread.h"
#include "writereader.h"

namespace spectral
{

CEquation::CEquation(CMutex* mtx) : cmtx(mtx) {};
CEquation::~CEquation() {};

void CEquation::Equation(int x)
{
    cmtx->cmtx->lock();

    int y = 0;

    HINSTANCE load;
    load = LoadLibrary(L"makedll.dll");
    typedef int (*equation)(int);
    equation Equation;
    Equation = (equation)GetProcAddress(load, "Equation");
    y = Equation(x);
    FreeLibrary(load);

    for (int i = 0; i < 1000000; i++)
    {
        //y = x * 2;
    }

    WriteReader wr("data.txt", y);

    this_thread::sleep_for(chrono::milliseconds(500));
    cout << "ID потока = " << this_thread::get_id() << " y = " << y << endl;

    cmtx->cmtx->unlock();
}

///////////////////////////////////////////////////////
VecThread::VecThread(IObservable* obj, CMutex* mtx) : mainwin(obj), cmtx(mtx)
{
    printf("Create VecThread\n");
    obj->addObserver(this);
}

VecThread::~VecThread(){}

void VecThread::updateCountThread(int p)
{
    countThread = p;
    cout << "VecThread: countThread = " << countThread << endl;
    CreateThread(p);
}
void VecThread::updateCountThread(map<string, int> p)
{
    auto it1 = p.find("CountThread");
    auto it2 = p.find("max");
    auto it3 = p.find("min");
    auto it4 = p.find("step");

    //if (it1->first == "CountThread") {}
    CreateThread(it1->second, it2->second, it3->second, it4->second);
}

void VecThread::CreateThread(int countThread)
{

}
void VecThread::CreateThread(int countThread,int max,int min,int step)
{
    cout << "CreateThread = " << countThread << endl;
    vector<thread*> th_vec;
    vector<CEquation> th_eq;

    thread* th = new thread();

    cmtx->cmtx->lock();
    for (int i = 0; i < countThread; ++i)
    {
        int res = min + step * i;
        cout << "x = " << res << endl;

        CEquation* c = new CEquation(cmtx);

        th = new thread(&CEquation::Equation,c,res);
        th->detach();
    }
    cmtx->cmtx->unlock();
}

///////////////////////////////
CMutex::CMutex() { cmtx = new mutex; }
CMutex::~CMutex() {}
}
