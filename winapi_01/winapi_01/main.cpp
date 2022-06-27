

#include "mainwin.h"
#include "VecThread.h"


using namespace spectral;


int main()
{
	setlocale(LC_ALL, "ru");

	CMutex* cmtx = new CMutex();

	MainWin* wm = new MainWin();

	VecThread* vt = new VecThread(wm, cmtx);

	wm->Run();


	return 0;
}











