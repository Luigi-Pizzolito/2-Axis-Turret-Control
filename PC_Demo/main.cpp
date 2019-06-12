#include "serial.h"
#include <iostream>
#include <string>
using namespace std;

signed int pitch = 0;
signed int yaw = 0;

string port;

int main() {
	
	// ask for serial port and connect to arduino
	cout << "Enter COM Port (COMX): ";
	cin >> port;
	serial myserial(port,115200);
	
	while (true) {
		// ask for angles to move to
		cout << "\nEnter angles,  pitch(-90 - 90) and yaw(-135 - 135) : \n";
		cout << "Pitch: ";
		cin >> pitch;
		cout << "Yaw: ";
		cin >> yaw;

		//convert angles to serial message and send
		string str = "";
		str = to_string(pitch) + "," + to_string(yaw) + ".";
		const char* c = str.c_str();
		myserial.write(c, str.size());
		
		//display movement on console
		cout << "\nMoved to (" << pitch << ", " << yaw << ")\n";
		//cout << c;
		
	}

	return 0;
}