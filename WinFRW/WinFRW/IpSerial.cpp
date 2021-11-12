#include "GlobalCore.h"


std::vector<IpSerial*> IpSerial::m_SerialPorts;

bool IpSerial::ReadData()
{
	unsigned long r;
	char buff;
	if (!ReadFile(m_hSerial, &buff, 1, &r, 0))
		return false;
	m_PortReadBuffer[counter++] = buff;
	if (counter == ReadBufferSize)
		counter = 0;

	return true;

}

IpSerial::IpSerial()
{
	DataRecieved = NULL;
	Free();
	m_SerialPorts.push_back(this);
}

IpSerial::~IpSerial()
{
	Close();
	Free();
	for (int i = 0; i < m_SerialPorts.size(); ++i)
	{
		if (m_SerialPorts[i] == this)
		{
			m_SerialPorts.erase(m_SerialPorts.begin() + i);
			break;
		}
	}
}

void IpSerial::Free()
{
	m_IsOpened = false;
	m_SyncMode = false;
	m_hSerial = INVALID_HANDLE_VALUE;
	counter = 0;
}

bool IpSerial::Open(const char* PortName, DWORD Flags, bool SyncMode, int BaudRate)
{
	strcpy(m_PortName, PortName);
	m_SyncMode = SyncMode;
	m_BaudRate = BaudRate;
	m_Flags = Flags;

	Close();
	m_hSerial = CreateFile(m_PortName, m_Flags, NULL, NULL, OPEN_EXISTING, (m_SyncMode) ? 0 : FILE_FLAG_OVERLAPPED, 0);
	if (m_hSerial == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{

		}
		Close();
		return false;
	}
	m_DCB = { 0 };
	m_DCB.DCBlength = sizeof(m_DCB);
	if (!GetCommState(m_hSerial, &m_DCB))
	{
		Close();
		return false;
	}
	m_DCB.BaudRate = m_BaudRate;
	m_DCB.ByteSize = 8;
	m_DCB.StopBits = ONESTOPBIT;
	m_DCB.Parity = NOPARITY;
	/*m_DCB.fAbortOnError = true;
	m_DCB.fDtrControl = DTR_CONTROL_DISABLE;
	m_DCB.fRtsControl = RTS_CONTROL_DISABLE;
	m_DCB.fBinary = true;
	m_DCB.fParity = false;
	m_DCB.fInX = false;
	m_DCB.fOutX = false;
	m_DCB.XonChar = 0;
	m_DCB.XoffChar = (unsigned char)0xff;
	m_DCB.fErrorChar = false;
	m_DCB.fNull = false;
	m_DCB.fOutxCtsFlow = false;
	m_DCB.fOutxDsrFlow = false;
	m_DCB.XonLim = 128;
	m_DCB.XoffLim = 128;*/
	if (!SetCommState(m_hSerial, &m_DCB))
	{
		Close();
		return false;
	}
	m_RSize = sizeof(m_PortReadBuffer);
	m_WSize = sizeof(m_PortWriteBuffer);
	m_IsOpened = true;
}

bool IpSerial::Close()
{
	if (!m_IsOpened)
		return false;
	m_IsOpened = false;
	return CloseHandle(m_hSerial);
}

bool IpSerial::Write(const char* Data)
{
	if (!m_IsOpened)
		return false;
	unsigned long lg = strlen(Data);
	unsigned long bytesWritten;
	return 	WriteFile(m_hSerial, Data, lg, &bytesWritten, NULL);
}

bool IpSerial::Read()
{
	if (!m_IsOpened)
		return false;
	return ReadData();
}

bool IpSerial::IsOpen()
{
	return m_IsOpened;
}

/*char* IpSerial::GetData()
{
	char c[50];
	strcpy(c, m_PortReadBuffer);
	return c;
	return NULL;
}*/