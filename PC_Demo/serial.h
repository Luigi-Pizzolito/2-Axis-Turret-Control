#ifndef ROBOT_CONTROL_H
#define ROBOT_CONTROL_H


#include <iostream> 
#include <string> 
#include<Windows.h>
#pragma comment(lib,"ws2_32.lib")


#define SERIAL_BAUDRATE		CBR_38400
#define SERIAL_BYTESIZE		8
#define SERIAL_PARITY			NOPARITY
#define SERIAL_STOPBITS		ONESTOPBIT
#define SERIAL_DTRCONTROL		DTR_CONTROL_ENABLE
#define SERIAL_RTSCONTROL		RTS_CONTROL_ENABLE




// serial protocol
struct serial {

public:
	serial(char *portName = "",
		DWORD baudRate = SERIAL_BAUDRATE,
		BYTE byteSize = SERIAL_BYTESIZE,
		BYTE parity = SERIAL_PARITY,
		BYTE stopBits = SERIAL_STOPBITS,
		DWORD dtrControl = SERIAL_DTRCONTROL,
		DWORD rtsControl = SERIAL_RTSCONTROL);

	~serial();
	int read(char *, const int size = 10);
	int write(const char *, const int size = 10);
	std::string getPortName();
	DWORD getBaudRate();
	void setSerial(char *, DWORD baudRate = SERIAL_BAUDRATE);
	void close();

private:
	std::string PortName;
	DWORD BaudRate;
	HANDLE hcom;
	std::string error;

};


#endif
