/* JoyStick module receiver code
 - CONNECTIONS: nRF24L01 Modules See:
   1 - GND
   2 - VCC 3.3V !!! NOT 5V
   3 - CE to Arduino pin 6
   4 - CSN to Arduino pin 8
   5 - SCK to Arduino pin 13
   6 - MOSI to Arduino pin 11
   7 - MISO to Arduino pin 12
   8 - UNUSED
 */
//-----( Import needed libraries )-----
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <printf.h>
#include <Servo.h>
/*-----( Declare Constants and Pin Numbers )-----*/
#define CE_PIN 9
#define CSN_PIN 10

// NOTE: the "LL" at the end of the constant is "LongLong" type
//const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe

//To do: 
const byte addresses[][6] = {"00001", "00002"};
#define light_Front  18     //LED Front Right   pin A4 for Arduino Uno
#define light_Back   19     //LED Back Right    pin A5 for Arduino Uno
#define horn_Buzz    7     


#define IN_11  3          // L298N #1 in 1 motor Front Right
#define IN_12  4          // L298N #1 in 2 motor Front Right
#define IN_13  5          // L298N #1 in 3 motor Back Right
#define IN_14  6          // L298N #1 in 4 motor Back Right

#define IN_21  7         // L298N #2 in 1 motor Front Left
#define IN_22  8         // L298N #2 in 2 motor Front Left
#define IN_23  11         // L298N #2 in 3 motor Back Left
#define IN_24  12         // L298N #2 in 4 motor Back Left

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

int wheel1;
int wheel1a;
int wheel2;
int wheel2a;

int state1 = 0;
int state2 = 0;
int state3 = 0;

/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio
/*-----( Declare Variables )-----*/
int joystick[7];  // 6 element array holding Joystick readings
int speedRight = 0;
int speedLeft = 0;
int  xAxis, yAxis, Turn;
// the four button variables from joystick
int buttonUp;
int buttonRight;
int buttonDown;
int buttonLeft;

int servo1Offset = 580;
int servo2Offset = 580;
int servo3Offset = 580;
int servo4Offset = 580;
int servo1_Offset;
int servo2_Offset;
int servo3_Offset;
int servo4_Offset;
double LT = 0;
int LTa = 0;
unsigned long currentMillis;
long previousMillis = 0;    // set up timers
long interval = 10;        // time constant for timer
int direction = 1;

void setup()
{
  printf_begin();
  pinMode(light_Front, OUTPUT);
  pinMode(light_Back, OUTPUT);
  pinMode(horn_Buzz, OUTPUT);
    
  
  pinMode(IN_11, OUTPUT);
  pinMode(IN_12, OUTPUT);
  pinMode(IN_13, OUTPUT);
  pinMode(IN_14, OUTPUT);
    
  pinMode(IN_21, OUTPUT);
  pinMode(IN_22, OUTPUT);
  pinMode(IN_23, OUTPUT);
  pinMode(IN_24, OUTPUT);

  Serial.begin(115200);
  Serial.println("Nrf24L01 Receiver Starting"); 
  
  radio.begin();
  //radio.openReadingPipe(1,pipe);
  radio.openWritingPipe(addresses[1]); // 00002
  radio.openReadingPipe(1, addresses[0]); // 00001
  radio.startListening();

    servo1.attach(34);
    servo2.attach(36);
    servo3.attach(32);
    servo4.attach(38);
    

    servo1.writeMicroseconds(servo1Offset);
    servo2.writeMicroseconds(servo2Offset);
    servo3.writeMicroseconds(servo3Offset);
    servo4.writeMicroseconds(servo4Offset);      
}

void loop()
{
  currentMillis = millis();
  if (currentMillis - previousMillis >= 100) {  // start timed event
          
    previousMillis = currentMillis;

    if ( radio.available() )
    {

        radio.read( joystick, sizeof(joystick) );
        xAxis = joystick[0];
        yAxis = joystick[1];
        double XT = (xAxis-528) ;
        double YT = (yAxis-514) ; 
        //XT*=528;
        //YT*=514;
        double angle = atan2(YT,XT)/(2*M_PI)*360;
        
        LT=(sqrt(XT*XT+YT*YT))/2;

        //reflecting across origin
        if (angle < 0) {
          angle += 180;
        }
        //fixing origin edge-case
        if(abs(XT)<5&&abs(YT)<5){
          angle=90;
        }
        
        Turn = (angle/180)*1023;
        
        // the four button variables from joystick array
        int buttonUp    = joystick[2];
        int buttonRight = joystick[3];
        int buttonDown  = joystick[4];
        int buttonLeft  = joystick[5];
        Serial.print("X = ");
        Serial.print(xAxis);
        Serial.print(" Y = ");  
        Serial.print(yAxis);
        Serial.print(" T = ");  
        Serial.print(Turn);

        Serial.print(" Up = ");
        Serial.print(joystick[2]);
        Serial.print(" Right = ");  
        Serial.print(joystick[3]);
        Serial.print(" Down = ");
        Serial.print(joystick[4]);
        Serial.print(" Left = ");  
        Serial.print(joystick[5]);
        Serial.print(" State = ");
        Serial.print(state1);
        
        if (state1 == 0){

          if (joystick[2] == 0 and state3 == 1){
            Serial.print(" state 1=1 and state2=1! ");
            state1 = 1;
            state2 = 1;
          }
          if (joystick[2] == 1){ 
            state3 = 1;
          }
        }
        else{
          if (state2 == 1){
            if (joystick[2] == 1){
              state2 = 0;
            }
          }
          else{
            if (joystick[2] == 0){
              Serial.print("waa!");
              state1 = 0;
              state3 = 0;
            }
          }
          }
        if (state1 == 0){
        servo1_Offset = servo1Offset + (Turn * 1.86);  //offset for 45: ~1970
        servo2_Offset = servo2Offset + (Turn * 1.86);  //offset for 45: ~1030
        servo3_Offset = servo3Offset + (Turn * 1.87);  //offset for 45: ~1087
        servo4_Offset = servo4Offset + (Turn * 1.87);  //offset for 45: ~1922
          
          servo1.writeMicroseconds(servo1_Offset);
          servo2.writeMicroseconds(servo2_Offset);
          servo3.writeMicroseconds(servo3_Offset);
          servo4.writeMicroseconds(servo4_Offset);
          
        }
        else if (state1 == 1){
          servo1.writeMicroseconds(1970);
          servo2.writeMicroseconds(1030);
          servo3.writeMicroseconds(1087);
          servo4.writeMicroseconds(1922);
          
        }


         
         
       // scale driving stick some more
         //(max resistor pos - rest pos) to rescale hypotenuse
         
         Serial.print(" Angle: ");
         Serial.print(angle);
         Serial.print(" LT: ");
         Serial.print(LT);
         if (LT > 255) {
           LT=255;
         }
               if (LT >=0&&yAxis>=514) {
                  LTa = abs(LT);
                  if(LTa>255){LTa=255;}
                  if (state1==0){
                  analogWrite(4, LTa);       // wheel 2
                  analogWrite(3, 0);
                  analogWrite(6, LTa);       // wheel 4
                  analogWrite(5, 0);
                  analogWrite(8, LTa);       // wheel 1
                  analogWrite(7, 0);
                  analogWrite(12, 0);       // wheel 3
                  analogWrite(11, LTa);
                

                  
                  
                }
                else {
                  LTa = abs(LT);
                                                      if(LTa>255){LTa=255;}
                  analogWrite(4, LTa);       // wheel 2
                  analogWrite(3, 0);
                  analogWrite(6, LTa);       // wheel 4
                  analogWrite(5, 0);
                  analogWrite(8, 0);       // wheel 1
                  analogWrite(7, LTa);
                  analogWrite(12, LTa);       // wheel 3
                  analogWrite(11, 0);

                }
               }
                else if (LT <=0||yAxis<514) {
                  LTa = abs(LT);
                  direction = -1;
                  if(LTa>255){LTa=255;}
                  if (state1==0){
                  analogWrite(4, 0);          // wheel 2
                  analogWrite(3, LTa);
                  analogWrite(6, 0);          // wheel 4
                  analogWrite(5, LTa);
                  analogWrite(8, 0);          // wheel 1
                  analogWrite(7, LTa);
                  analogWrite(12, LTa);       // wheel 3
                  analogWrite(11,0);
                 
                  
                  
                 
                }
                else {
                                  LTa = abs(LT);
                                    if(LTa>255){LTa=255;}
                  analogWrite(4, 0);       // wheel 2
                  analogWrite(3, LTa);
                  analogWrite(6, 0);       // wheel 4
                  analogWrite(5, LTa);
                  analogWrite(8, LTa);       // wheel 1
                  analogWrite(7, 0);
                  analogWrite(12, 0);       // wheel 3
                  analogWrite(11, LTa);

                }
               }  
               Serial.print(" LTA: ") ;                    
               Serial.println(LTa);
    }
    else {
            Serial.println("no data");
          }

      
  }

}
    
