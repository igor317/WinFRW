#ifndef IPSERIAL_H
#define IPSERIAL_H

class IpSerial
{
private:
	static const int ReadBufferSize = 4096;
	static const int WriteBufferSize = 2048;
	char m_PortName[20];

	unsigned long m_RSize;
	char m_PortWriteBuffer[WriteBufferSize];
	unsigned long m_WSize;
	HANDLE m_hSerial;
	bool m_IsOpened;
	bool m_SyncMode;
	int m_BaudRate;
	DWORD m_Flags;
	DCB m_DCB;

	int counter;

	bool ReadData();
public:
	static std::vector<IpSerial*> m_SerialPorts;
	char m_PortReadBuffer[ReadBufferSize];
	IpEventFunc* DataRecieved;

	struct Flags
	{
		enum EFlags
		{
			Read = GENERIC_READ,
			Write = GENERIC_WRITE,
			Standart = Read | Write,
		};
	};

	IpSerial();
	~IpSerial();
	void Free();

	bool Open(const char* PortName, DWORD Flags = IpSerial::Flags::Standart, bool SyncMode = true, int BaudRate = 9600);
	bool Close();
	bool Write(const char* Data);
	bool Read();
	bool IsOpen();
	//char* GetData();
};

#endif // !IPSERIAL_H
