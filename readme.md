# Servo Control
![Project Image](https://github.com/Gangster45671/Gangster45671.github.io/raw/master/websocketcameraturret.jpg)
Code for controling a 2-axis servo turret from an Arduino's serial input.

## Table of Contents
- [Servo Control](#servo-control)
  - [Table of Contents](#table-of-contents)
  - [Usage](#usage)
    - [Examples](#examples)
      - [Setting Angles](#setting-angles)
      - [Reseting Turret](#reseting-turret)
    - [Using the turret with C++ on Windows](#using-the-turret-with-c-on-windows)
  - [Parameters](#parameters)

## Usage
To set the turret simply send a comma separeted list of the pitch and yaw angles, terminated by a `.`


### Examples

#### Setting Angles
Sending the following string over serial
> -30,50.

Sets the pitch (rotation around axis perpendicular to facing direction) to `-30°` and the yaw (rotation around the vertical axis) to `50°`.

#### Reseting Turret
Sending the following string over serial
> .

Resets the pitch (rotation around axis perpendicular to facing direction) to `0°` and the yaw (rotation around the vertical axis) to `0°`.

### Using the turret with C++ on Windows
To use the turret in Windows with C++, connect to the serial port and send your angles formated as an ASCII string. See PC_Demo for a demo.
```c++
#include "serial.h" //libraries
#include <string>

int main() {
  serial myserial("COM5"); //connect to COM port

  string str = "";    //new string
  str = to_string(pitch) + "," + to_string(yaw) + ".";    //  concatenate string and angles from int to string
  const char* c = str.c_str();    //convert strin to const char*

  myserial.write(c, str.size());   //send string over serial

  return 0;
}
```

## Parameters
| Parameter | Default Value | Explanation |
| --- | --- | --- |
| ease_amount | `20` | Delay in ms between every movement of 1°, smoothness/movement speed |
| BAUD_RATE | `115200` | Baud rate for serial communications |
| EOPmarker | `'.'` | End of serial packet marker |
| Servo_up_pin | `9` | PWM pin for top servo
| Servo_down_pin | `10` | PWM pin for bottom servo
| UP_ZERO | `(-18)` | Center point for top servo
| DOWN_ZERO | `0` | Center point for bottom servo
| UP_MIN | `(-90)` | Minimum angle value for top servo
| UP_MAX | `90` | Maximum angle value for top servo
| UP_MIN | `(-135)` | Minimum angle value for bottom servo
| UP_MAX | `135` | Maximum angle value for bottom servo
