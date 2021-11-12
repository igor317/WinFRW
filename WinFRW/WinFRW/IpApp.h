#ifndef IPAPP_H
#define IPAPP_H

#include "IpApplication.h"
#include "IpDrawing.h"
#include "../IpWHierarchy.h"

class IpApp : public IpApplication
{
private:
	IpWForm* m_Form;
	IpWLabel* m_LBL;
	IpWScroll* m_Scroll;
	void TestFunc();
	void Test2();
public:
	IpApp();
	bool Init();
	bool Shutdown();
};

#endif // !IPAPP_H