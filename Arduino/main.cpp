/*
 Servo Control

 Recieves strings via serial with the format "<pitch_angle>,<yaw_angle>."
 Sends value to 2-axis servo turret.

 Originally written by Amy

 modified 11 June 2019
 by Luigi Pizzolito
 */

//************************************** SERIAL VARIABLES **************************************
const char EOPmarker = '.';     //This is the end of packet marker
char serialbuf[32];             //This gives the incoming serial some room. Change it if you want a longer incoming.
#include <string.h>             // we'll need this for subString
#define MAX_STRING_LEN 20       // like 3 lines above, change as needed.
#define BAUD_RATE 115200        //Baud rate for serial communications
//**********************************************************************************************

//************************************** SERVO VARIABLES ***************************************
#include <Servo.h>                      //Default Servo Library
#define ease_amount         20         //easing for smooth motion, higher=rougher.
#define Servo_up_pin        9           //PWM pin for top servo
#define Servo_down_pin      10          //PWM pin for bottom servo
#define START_TIME          500         //Servo Settings
#define END_TIME            25000
#define UP_ZERO             (-18)       //Center point for top servo
#define DOWN_ZERO           (65)        //Center point for bottom servo
#define UP_MIN              (-90)       //Minimum and Maximum angle values for top servo
#define UP_MAX              (90)        
#define DOWN_MIN            (-135)      //Minimum and Maximum angle values for bottom servo
#define DOWN_MAX            (135)       
int angel_convert(int);
bool myServo_up_control(uint8_t *, size_t);
bool myServo_down_control(uint8_t *, size_t);
Servo myServo_up;
Servo myServo_down;
//**********************************************************************************************

void setup()
{
    //---------------------------------------- INITIALIZE PREHIPHERALS ----------------------------------------
    Serial.begin(BAUD_RATE);                                           //Begin Serial communications
    myServo_up.attach(Servo_up_pin, START_TIME, END_TIME);          //Begin Servos
    myServo_down.attach(Servo_down_pin, START_TIME, END_TIME);
    myServo_up.write(angel_convert(UP_ZERO));                       //Center Turret
    myServo_down.write(angel_convert(DOWN_ZERO));
}

void loop()
{
    //---------------------------------------- READ SERIAL ----------------------------------------
    if (Serial.available() > 0)
    {                                //makes sure something is ready to be read
        static int bufpos = 0;       //starts the buffer back at the first position in the incoming serial.read
        char inchar = Serial.read(); //assigns one byte (as serial.read()'s only input one byte at a time
        if (inchar != EOPmarker)
        {                               //if the incoming character is not the byte that is the incoming package ender
            serialbuf[bufpos] = inchar; //the buffer position in the array get assigned to the current read
            bufpos++;                   //once that has happend the buffer advances, doing this over and over again until the end of package marker is read.
        }
        else
        {                          //once the end of package marker has been read
            serialbuf[bufpos] = 0; //restart the buff
            bufpos = 0;            //restart the position of the buff

            //---------------------------------------- SERIAL RECIEVED ----------------------------------------
            int x = atoi(subStr(serialbuf, ",", 1)); //split and parse varables from serial buffer
            int y = atoi(subStr(serialbuf, ",", 2));

            //---------------------------------------- ACT ON SERIAL DATA ----------------------------------------
            Serial.print("Pitch: ");       //Print recieved Pitch and Yaw for debug.
            Serial.print(x);
            Serial.print("\t-\tYaw: ");
            Serial.println(y);

            myServo_up_control(x);        //Move Turret pitch
            myServo_down_control(y);      //Move Turret yaw
        }
    }
}

//================================= STRING SPLITTING =================================
char *subStr(char *input_string, char *separator, int segment_number)
{
    char *act, *sub, *ptr;
    static char copy[MAX_STRING_LEN];
    int i;
    strcpy(copy, input_string);
    for (i = 1, act = copy; i <= segment_number; i++, act = NULL)
    {
        sub = strtok_r(act, separator, &ptr);
        if (sub == NULL)
            break;
    }
    return sub;
}
//===================================================================================

//================================= SERVO FUNCTIONS =================================
//scale angle to within range
inline int angel_convert(int A)
{
    return map(A, DOWN_MIN, DOWN_MAX, 0, 180);
}

//control top servo
bool myServo_up_control(int angel)
{
    //Make sure angle is within range, otherwise truncate
    if (angel > UP_MAX)
        angel = UP_MAX;
    if (angel < UP_MIN)
        angel = UP_MIN;

    angel = angel_convert(angel + UP_ZERO); //convert angle to servo value
    while (myServo_up.read() != angel) {    //ease in value for smooth motion
      if (myServo_up.read() > angel) myServo_up.write(myServo_up.read()-1); 
      if (myServo_up.read() < angel) myServo_up.write(myServo_up.read()+1); 
      if (myServo_up.read() == 180 || myServo_up.read() == 0) break; //exit if at servo's limits
      delay(ease_amount);
    }
    myServo_up.write(angel);                //write value and move servo
}

//control bottom servo
bool myServo_down_control(int angel)
{
    //Make sure angle is within range, otherwise truncate
    if (angel > DOWN_MAX)
        angel = DOWN_MAX;
    if (angel < DOWN_MIN)
        angel = DOWN_MIN;

    angel = angel_convert(angel + DOWN_ZERO); //convert angle to servo value
    while (myServo_down.read() != angel) {    //ease in value for smooth motion
      if (myServo_down.read() > angel) myServo_down.write(myServo_down.read()-1); 
      if (myServo_down.read() < angel) myServo_down.write(myServo_down.read()+1);
      if (myServo_down.read() == 180 || myServo_down.read() == 0) break; //exit if at servo's limits
      delay(ease_amount);
    }
    myServo_down.write(angel);                //write value and move servo
}
//===================================================================================
