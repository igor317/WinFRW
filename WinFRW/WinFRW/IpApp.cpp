#include "IpApp.h"

void IpApp::TestFunc()
{

}

void IpApp::Test2()
{
	std::string val = std::to_string(m_Scroll->GetCurrentValue());
	m_LBL->SetName(val);
}

void Exit()
{
	exit(0);
}

IpApp::IpApp() : IpApplication()
{
	m_Form = new IpWForm("WinForm", 50, 50, 500, 500,(UINT)ClassStyles::STANDART,(DWORD)WinStyles::STANDART,NULL,fnStandartProc,NULL);
	IpWHierarchy* m_H = new IpWHierarchy(20, 20, 300, 300, m_Form);
	//m_Scroll = new IpWScroll(5, 5, 20, 250, ScrollStyles::Vertical, m_H);
	//m_LBL = new IpWLabel("123", 50, 10, 80, 60, m_H, (DWORD)LabelStyles::Center);
	//m_Scroll->Scroll = CreateIpEventHandler(IpApp, IpApp::Test2);

	//m_Scroll->SetRangeValues(200, 500);
}

bool IpApp::Init()
{
	return true;
}

bool IpApp::Shutdown()
{
	return true;
}