# PC Demos
## Serial Control
To use this simply run the program, supply the COM port number and then supply pitch and yaw values; they will be sent over serial.

## Websocket Control
### Websocket Server
To run the server simply compile and run the program passing the commandline options <ip_adress> <port> <serial_com>
The program will open serial communication when a websocket request is recieved and pass incoming strings to serial.
To compile the program `Boost` and `Beast` must be installed.
### Websocket Client
Before running the client, edit line 17 of d-pad.html so that `var wsUri = "ws://<ip_adress>:<port>";` matches that of your server.
Then simply open d-pad.html in any web browser to run the client.
The javascript functions for controlling the camera turret are `setTur([ <pitch>, <yaw> ])` and `movTur([ pitch, yaw ])`. One modifies the turrets current position while the other sets the angles to absolutes.
The angle range is limited in hardware so no software range limitations are needed.
