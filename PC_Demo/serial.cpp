#include "serial.h"
#include <sstream>

using namespace std;

// init serial
serial::serial(string portName,
	DWORD baudRate,
	BYTE byteSize,
	BYTE parity,
	BYTE stopBits,
	DWORD dtrControl,
	DWORD rtsControl) {

	// save serial port message
	PortName = string(portName);
	BaudRate = baudRate;
	if (PortName == "")
		return;

	// create serial file
	hcom = CreateFileA(
		PortName.c_str(),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_NO_BUFFERING,
		NULL);

	if (hcom == INVALID_HANDLE_VALUE) {
		error = string(portName) + "can't open!";
		cerr << portName << "can't open" << endl;
	}
	else {

		// set bufLen
		SetupComm(hcom, 64, 64);

		// refresh driver
		PurgeComm(hcom, PURGE_TXABORT | PURGE_RXABORT
			| PURGE_TXCLEAR | PURGE_RXCLEAR);

		// set timeout
		COMMTIMEOUTS commTimeouts;
		commTimeouts.ReadIntervalTimeout = MAXDWORD;
		commTimeouts.ReadTotalTimeoutMultiplier = 0;
		commTimeouts.ReadTotalTimeoutConstant = 0;
		commTimeouts.WriteTotalTimeoutMultiplier = 0;
		commTimeouts.WriteTotalTimeoutConstant = 0;

		if (!SetCommTimeouts(hcom, &commTimeouts)) {
			CloseHandle(hcom);
			cout << "timeout error!" << endl;
			return;
		}
		// Set communication parameters
		DCB dcb;
		ZeroMemory(&dcb, sizeof(dcb));
		dcb.DCBlength = sizeof(dcb);
		GetCommState(hcom, &dcb);

		dcb.BaudRate = baudRate;
		dcb.ByteSize = byteSize;
		dcb.Parity = parity;
		dcb.StopBits = stopBits;
		dcb.fInX = dcb.fOutX = FALSE;
		dcb.fOutxDsrFlow = dcb.fOutxCtsFlow = FALSE;
		dcb.fDtrControl = dtrControl;
		dcb.fRtsControl = rtsControl;

		if (!SetCommState(hcom, &dcb))
		{
			cout << portName << " state error" << endl;
			CloseHandle(hcom);
			return;
		}
		cout << "serial is good!" << endl;
	}
}

// serial read
int serial::read(char *serialOut, const int size) {
	DWORD ret;
	DWORD dwEvtMask;
	bool fSuccess = SetCommMask(hcom, EV_RXCHAR);

	if (!fSuccess)
	{
		// Handle the error. 
		printf("SetCommMask failed with error %d.\n", GetLastError());
		//return;
	}
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	double prems = sys.wMilliseconds;
	while (!WaitCommEvent(hcom, &dwEvtMask, NULL)) {
		GetLocalTime(&sys);
		if (abs(prems - sys.wMilliseconds) > 10) {
			cerr << "read timeout " << endl;
			break;
		}

	};
	Sleep(5);

	if (dwEvtMask & EV_RXCHAR) {

		bool result = ReadFile(hcom, serialOut, size, &ret, NULL);
		cout << "I have recive " << ret << " chars" << endl;
		if (!result)
			cout << "read error!" << endl;
	}

	//cout << serialOut << endl;

	return size;
}

// serial write

int serial::write(const char *serialIn, const int size) {
	DWORD ret;
	bool result = WriteFile(hcom, serialIn, size, &ret, NULL);
	if (!result)
		cout << "write error!" << endl;
	return size;
}

// close serial port

void serial::close() {
	PurgeComm(hcom, PURGE_TXABORT | PURGE_RXABORT
		| PURGE_TXCLEAR | PURGE_RXCLEAR);
	CloseHandle(hcom);
}
// get port name
string serial::getPortName() {
	return PortName;
}

// get the baudRate
DWORD serial::getBaudRate() {
	return BaudRate;
}
// reset the serial
void serial::setSerial(char *portName, DWORD baudRate) {
	close();
	serial(portName, baudRate);
}

serial::~serial() {

	close();
}
