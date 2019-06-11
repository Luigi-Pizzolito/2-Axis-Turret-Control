#include "serial.h"
#include <iostream>
#include <string> // for string and to_string() 
using namespace std;

int pitch = 0;
int yaw = 0;

int main() {
	string port;
	cout << "Enter COM Port (COMX): ";
	cin >> port;
	serial myserial(port);
	
	while (true) {
		cout << "\nEnter angles,  pitch(-90 - 90) and yaw(-135 - 135) : \n";
		cout << "Pitch: ";
		cin >> pitch;
		cout << "Yaw: ";
		cin >> yaw;

		string str = "";
		str = to_string(pitch) + "," + to_string(yaw) + ".";
		const char* c = str.c_str();

		// cout << c;
		myserial.write(c, sizeof(c));
		cout << "\nMoved to (" << pitch << ", " << yaw << ")\n";
	}

	return 0;
}