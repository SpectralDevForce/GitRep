
#include "IObserver.h"

using namespace std;


namespace spectral
{

void IObserver::updateCountThread(int p) {};
void IObserver::updateCountThread(map<string, int> p) {};


void IObservable::addObserver(IObserver* o) {};
void IObservable::removeObserver(IObserver* o) {};
void IObservable::notify(pair<string, int> p) {};
void IObservable::notify(map<string, int> p) {};
IObservable::~IObservable() {}

}


