#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <Wire.h>  // Comes with Arduino IDE
#define CE_PIN 9
#define CSN_PIN 10
unsigned long previousMillis = 0;
const long interval = 20;

int but1;
int but2;
int but3;
int but4;
int but5;

int axis1;
int axis2;
int axis3;

int joystick[7];
String count;

struct SEND_DATA_STRUCTURE{
//struct __attribute__((__packed__)) SEND_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to send
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO

    int16_t menuDown;      
    int16_t Select; 
    int16_t menuUp;  
    int16_t toggleBottom;  
    int16_t toggleTop; 
    int16_t mode;  
    int16_t RFB;
    int16_t LLR;
    int16_t LT;
};

struct RECEIVE_DATA_STRUCTURE_REMOTE{
  //put your variable definitions here for the data you want to receive
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  int16_t mode;
  int16_t count;
};

int remoteFlag = 0;

SEND_DATA_STRUCTURE mydata_send;
RECEIVE_DATA_STRUCTURE_REMOTE mydata_remote;
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio
const byte addresses[][6] = {"00001", "00002"};


void setup() {  



  pinMode(38, INPUT_PULLUP);
  pinMode(40, INPUT_PULLUP);
  pinMode(42, INPUT_PULLUP);
  pinMode(44, INPUT_PULLUP);
  pinMode(46, INPUT_PULLUP);

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);

  radio.begin();
  Serial.println("Nrf24L01 Tx Starting"); 
  //radio.openWritingPipe(pipe);
  radio.openWritingPipe(addresses[0]); // 00002
  radio.openReadingPipe(1, addresses[1]); // 00001
  radio.stopListening();
  Serial.begin(115200);

  
    
}

void loop() {

     unsigned long currentMillis = millis();
         if (remoteFlag == 0 && currentMillis - previousMillis >= 5) { 

              but1 =  digitalRead(38);
              but2 =  digitalRead(40);
              but3 =  digitalRead(42);
              but4 =  digitalRead(44);
              but5 =  digitalRead(46);                     
              
              if (but1 == 0) {
                mydata_send.menuDown = 1;
              }
              else {
                mydata_send.menuDown = 0;
              }
              
              if (but2 == 0) {
                mydata_send.Select = 1;
              }
              else {
                mydata_send.Select = 0;
              }
              
              if (but3 == 0) {
                mydata_send.menuUp = 1;
              }
              else {
                mydata_send.menuUp = 0;
              }
              
              if (but4 == 0) {
                mydata_send.toggleBottom = 1;
              }
              else {
                mydata_send.toggleBottom = 0;
              }
              
              if (but5 == 0) {
                mydata_send.toggleTop = 1;
              }
              else {
                mydata_send.toggleTop = 0;
              }
              
              axis1 = analogRead(A0);
              axis2 = analogRead(A1);
              axis3 = analogRead(A2);

              
              mydata_send.RFB = axis1;
              mydata_send.LLR = axis2;
              mydata_send.LT = axis3;                       
                joystick[0] = analogRead(axis1);
  joystick[1] = analogRead(axis2);
  joystick[2] = digitalRead(but1);
  joystick[3] = digitalRead(but2);
  joystick[4] = digitalRead(but3);
  joystick[5] = digitalRead(but4);
  joystick[6] = analogRead(axis3);
  radio.stopListening();
  radio.write( joystick, sizeof(joystick) );
  delay(20);
        Serial.print("X = ");
      Serial.print(joystick[0]);
      Serial.print(" Y = ");  
      Serial.print(joystick[1]);
      Serial.print(" T = ");  
      Serial.println(joystick[6]);


/*
              //delay(5);
              //radio.startListening();              
              //radio.read(&mydata_remote, sizeof(RECEIVE_DATA_STRUCTURE_REMOTE));

              count = String(mydata_remote.count);


              /*
              
              lcd.setCursor(0,3);
              lcd.print(count);
    
              if (mydata_remote.mode == 0) {
                lcd.setCursor(0,0);
                lcd.print("Mode 0 - Safe       ");
                lcd.setCursor(0,1);
                lcd.print("                    ");
              }
              else if (mydata_remote.mode == 1) {
                lcd.setCursor(0,0);
                lcd.print("Mode 1 - Kin Test   ");
                lcd.setCursor(0,1);
                lcd.print("                    ");
              }
              else if (mydata_remote.mode == 2) {
                lcd.setCursor(0,0);
                lcd.print("Mode 2 -            ");
                lcd.setCursor(0,1);
                lcd.print("                    ");
              }
              else if (mydata_remote.mode == 3) {
                lcd.setCursor(0,0);
                lcd.print("Mode 3 -            ");
                lcd.setCursor(0,1);
                lcd.print("                    ");
              }
              else if (mydata_remote.mode == 4) {
                lcd.setCursor(0,0);
                lcd.print("Mode 4 -            ");
                lcd.setCursor(0,1);
                lcd.print("                    ");
              }
              else if (mydata_remote.mode == 5) {
                lcd.setCursor(0,0);
                lcd.print("Mode 5 -            ");
                lcd.setCursor(0,1);
                lcd.print("                    ");
              }
              

              radio.stopListening();
              radio.write(&mydata_send, sizeof(SEND_DATA_STRUCTURE));
              Serial.print(mydata_send.RFB);
              Serial.print(',');
              Serial.print(mydata_send.LLR);
              Serial.print(',');
              Serial.print(mydata_send.LT);
              Serial.println();
              
              */
              previousMillis = currentMillis;  

         }        
        


      



  }  // end of main loop