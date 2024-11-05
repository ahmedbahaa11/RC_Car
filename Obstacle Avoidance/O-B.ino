#include <SoftwareSerial.h>
SoftwareSerial BT_Serial(2, 3); // RX, TX



#define enA 10//Enable1 L298 Pin enA 
#define in1 9 //Motor1  L298 Pin in1 
#define in2 8 //Motor1  L298 Pin in1 
#define in3 7 //Motor2  L298 Pin in1 
#define in4 6 //Motor2  L298 Pin in1 
#define enB 5 //Enable2 L298 Pin enB 

#define servo A4



#define echo A2    //Echo pin
#define trigger A3 //Trigger pin

int distance_L, distance_F = 30, distance_R;
long distance;
int set = 15 ;


int Speed = 255;  



void setup(){ // put your setup code here, to run once



pinMode(echo, INPUT );// declare ultrasonic sensor Echo pin as input
pinMode(trigger, OUTPUT); // declare ultrasonic sensor Trigger pin as Output  

pinMode(enA, OUTPUT); // declare as output for L298 Pin enA 
pinMode(in1, OUTPUT); // declare as output for L298 Pin in1 
pinMode(in2, OUTPUT); // declare as output for L298 Pin in2 
pinMode(in3, OUTPUT); // declare as output for L298 Pin in3   
pinMode(in4, OUTPUT); // declare as output for L298 Pin in4 
pinMode(enB, OUTPUT); // declare as output for L298 Pin enB 



Serial.begin(9600); // start serial communication at 9600bps
BT_Serial.begin(9600); 

pinMode(servo, OUTPUT);

/*
 for (int angle = 80; angle <= 160; angle += 10)  {
   servoPulse(servo, angle);  }
 for (int angle = 160; angle >= 0; angle -= 10)  {
   servoPulse(servo, angle);  }

 for (int angle = 0 ; angle >= 80; angle -= 10)  {
   servoPulse(servo, angle); } 

delay(500);
*/
Check_side();

}


void loop(){  

 

analogWrite(enA, Speed); // Write The Duty Cycle 0 to 255 Enable Pin A for Motor1 Speed 
analogWrite(enB, Speed); // Write The Duty Cycle 0 to 255 Enable Pin B for Motor2 Speed 

   
//===============================================================================
//                          Obstacle Avoiding Control
//===============================================================================     
 distance_F = Ultrasonic_read();
 Serial.print("S=");Serial.println(distance_F);
  if (distance_F > set){forword();}
    else{Check_side();}


//delay(10);

}

void servoPulse (int pin, int angle){
int pwm = (angle*11) + 500;      // Convert angle to microseconds
 digitalWrite(pin, HIGH);
 delayMicroseconds(pwm);
 digitalWrite(pin, LOW);
 delay(20);                   // Refresh cycle of servo
}


//**********************Ultrasonic_read****************************
long Ultrasonic_read(){
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  distance = pulseIn (echo, HIGH);
  return distance / 29 / 2;
}

void compareDistance(){
       if ( (distance_L > distance_R) && (distance_L > distance_F) && (distance_L > set ) ){
  turnLeft();
  delay(500);
  }
  else if ( (distance_R > distance_L ) && (distance_R > distance_F) && (distance_R > set ) ){
  turnRight();
  delay(500);
  }
  else if ( (distance_F > distance_L ) && (distance_F > distance_R) && (distance_F > set ) ){
  forword();
  }
 else if ( (distance_F < set ) && (distance_R < set ) && (distance_L < set ) ){
  backword();
  delay(500);
  Check_side();
  }
  /*
  else {
  backword();
  delay(500);
  Check_side();
  }
  */
}

void Check_side(){
    Stop();
    delay(100);
 for (int angle = 80; angle <= 170; angle += 15)  {
   servoPulse(servo, angle);  }
    delay(300);
    distance_L = Ultrasonic_read();
    delay(100);
  for (int angle = 170; angle >= 0; angle -= 15)  {
   servoPulse(servo, angle);  }
    delay(300);
    distance_R = Ultrasonic_read();
    delay(100);
 for (int angle = 0; angle <= 80; angle += 15)  {
   servoPulse(servo, angle);  }
   delay(300);
   distance_F = Ultrasonic_read();
    delay(100);
    compareDistance();
}

void forword(){  //forword
digitalWrite(in1, LOW); //Right Motor forword Pin 
digitalWrite(in2, HIGH);  //Right Motor backword Pin 
digitalWrite(in3, HIGH);  //Left Motor backword Pin 
digitalWrite(in4, LOW); //Left Motor forword Pin 
}

void backword(){ //backword
digitalWrite(in1, HIGH);  //Right Motor forword Pin 
digitalWrite(in2, LOW); //Right Motor backword Pin 
digitalWrite(in3, LOW); //Left Motor backword Pin 
digitalWrite(in4, HIGH);  //Left Motor forword Pin 
}

void turnRight(){ //turnRight
digitalWrite(in1, LOW);  //Right Motor forword Pin 
digitalWrite(in2, HIGH); //Right Motor backword Pin  
digitalWrite(in3, LOW);  //Left Motor backword Pin 
digitalWrite(in4, HIGH); //Left Motor forword Pin 
}

void turnLeft(){ //turnLeft
digitalWrite(in1, HIGH); //Right Motor forword Pin 
digitalWrite(in2, LOW);  //Right Motor backword Pin 
digitalWrite(in3, HIGH); //Left Motor backword Pin 
digitalWrite(in4, LOW);  //Left Motor forword Pin 
}

void Stop(){ //stop
digitalWrite(in1, LOW); //Right Motor forword Pin 
digitalWrite(in2, LOW); //Right Motor backword Pin 
digitalWrite(in3, LOW); //Left Motor backword Pin 
digitalWrite(in4, LOW); //Left Motor forword Pin 
}
