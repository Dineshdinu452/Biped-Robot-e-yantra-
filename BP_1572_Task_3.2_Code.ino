#include <Arduino.h>

#define Mag         12                     // electromagnet pin

#define L1            8                      // motor pin
#define PWML          6                     // PWM motor pin  
#define L2            9                       // motor pin  

#define R1            10                       // motor pin
#define PWMR           5                       // PWM motor pin
#define R2            11                       // motor pin 


//Function for forward motion of motor 1
void motorForwardL(int PWM_val)  
{
    
    analogWrite(PWML, PWM_val);
    digitalWrite(L1, HIGH);
    digitalWrite(L2, LOW);
}
//Function for forward motion of motor 2
void motorForwardR(int PWM_val) 
{
    analogWrite(PWMR, PWM_val);
    digitalWrite(R1, HIGH);
    digitalWrite(R2, LOW);
}
//Function for backward motion of motor 1
void motorBackwardR(int PWM_val) 
{
    analogWrite(PWMR, PWM_val);
    digitalWrite(R1, LOW);
    digitalWrite(R2, HIGH);
}
//Function for backward motion of motor 2
void motorBackwardL(int PWM_val)  
{
    analogWrite(PWML, PWM_val);
    digitalWrite(L1, LOW);
    digitalWrite(L2, HIGH);
}

void setup() 
{
//intialing the serial communication in baud rate of 9600  
    Serial.begin(9600);

//initialization for electromagnet     
    pinMode(Mag, OUTPUT);
    digitalWrite(Mag,LOW);

//initialization for motor 1
    pinMode(L1, OUTPUT);
    pinMode(L2, OUTPUT);
    pinMode(PWML, OUTPUT);
    
//initialization for motor 2
    pinMode(R1, OUTPUT);
    pinMode(R2, OUTPUT);
    pinMode(PWMR, OUTPUT);
}

void loop() 
{
if(Serial.available()>=18)
{
  
 if(Serial.read()== 0x7E)
 {
  for(int i=1;i<12;i++)
  {
    byte discardByte = Serial.read(); 
  }
  int dig1 = Serial.read();    //reading the digital value
  Serial.println(dig1);
//reading the analog values
  int MSB1 = Serial.read();    
  int LSB1 = Serial.read();
  int yaxis = LSB1 +(MSB1 * 256);   //converting the two 8 bits into single 16 bits ("yaxis value")
  int MSB2 = Serial.read();
  int LSB2 = Serial.read();
  int xaxis = LSB2 +(MSB2 * 256);   //converting the two 8 bits into single 16 bits ("xaxis value")

//controlling the electromagnet
 if (dig1 == 0)
 {
  Serial.println("low");
  digitalWrite(Mag,LOW);
 }
 else
 {
  Serial.println("high");
  digitalWrite(Mag,HIGH);
 }
 
// X-axis used for forward and backward control
  if (xaxis < 620) 
  {
// Convert the declining X-axis readings for going forward from 620 to 0 into 0 to 255 value for the PWM signal for increasing the motor speed
   int motorSpeedA = map(xaxis, 620, 0, 0, 255);
   int motorSpeedB = map(xaxis, 620, 0, 0, 255);
//To avoid buzzing of motor 
    if (motorSpeedA < 70)
    {
    motorSpeedA = 0;
    }
    if (motorSpeedB < 70)
    {
    motorSpeedB = 0;
    }
// Set Motor A forward
    motorForwardR(motorSpeedA);
// Set Motor B forward
    motorForwardL(motorSpeedB);
  }
  else if (xaxis > 760) 
  {
// Convert the increasing X-axis readings for going backward from 760 to 1023 into 0 to 255 value for the PWM signal for increasing the motor speed
    int motorSpeedA = map(xaxis, 760, 1023, 0, 255);
    int motorSpeedB = map(xaxis, 760, 1023, 0, 255);
//To avoid buzzing of motor
    if (motorSpeedA < 70)
    {
    motorSpeedA = 0;
    }
    if (motorSpeedB < 70) 
    {
      motorSpeedB = 0;
    }
// Set Motor A backward
    motorBackwardR(motorSpeedA); 
// Set Motor B backward
    motorBackwardL(motorSpeedB); 
  }
// If joystick stays in middle the motors are not moving
  else 
  {
   int motorSpeedA = 0;
   int motorSpeedB = 0;
  }
  if (yaxis < 620) 
  {
// Convert the declining Y-axis readings for going backward from 620 to 0 into 0 to 255 value for the PWM signal for increasing the motor speed
    int motorSpeedA = map(yaxis, 620, 0, 0, 255);
    int motorSpeedB = map(yaxis, 620, 0, 0, 255);
//To avoid buzzing of motor
    if (motorSpeedA < 70)
    {
      motorSpeedA = 0;
    }
    if (motorSpeedB < 70) 
    {
      motorSpeedB = 0;
    }
//Set Motor A forward
    motorForwardR(motorSpeedA); 
// Set Motor B backward
    motorBackwardL(motorSpeedB);
    
  }
  else if (yaxis > 760) 
  {
// Convert the increasing Y-axis readings for going forward from 760 to 1023 into 0 to 255 value for the PWM signal for increasing the motor speed
    int motorSpeedA = map(yaxis, 760, 1023, 0, 255);
    int motorSpeedB = map(yaxis, 760, 1023, 0, 255);
//To avoid buzzing of motor
    if (motorSpeedA < 70)
    {
      motorSpeedA = 0;
    }
    if (motorSpeedB < 70) 
    {
      motorSpeedB = 0;
    }
// Set Motor A Backward
    motorBackwardR(motorSpeedA);
// Set Motor B Forward
    motorForwardL(motorSpeedB);     
  }
// If joystick stays in middle the motors are not moving  
  else 
  {
   int motorSpeedA = 0;
   int motorSpeedB = 0;
  }
  
}
} 
}
