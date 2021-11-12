#include "IpApp.h"

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	IpApp app;
	return app.Run();
}