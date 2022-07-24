#include <SoftwareSerial.h>

SoftwareSerial sim808(12,13);    

String SDT="+84845967877";

const int mainLedPin = 13;      
const int speakerPin = 3;      
const int threshold = 50;     

int cnt=0;
int phase=0;
int openFlag= 0;
int gsmReady = 0;



void call()
{
  at("AT", 1000);
  at("ATD"+SDT+";", 15000);
  at("ATH", 1000);
}
void sendSMS() {
  // send the message
  Serial.println("Sending SMS...");               //Show this message on serial monitor
  at("AT",1000);
  at("AT+CMGF=1",1000);
  at("AT+CSCS=\"GSM\"",1000);
  at("AT+CMGS=\"" + SDT+"\"",2000);
  at("Door opened!",1000);
  sim808.write(26);     
  Serial.println("Text Sent.");
  delay(5000);
}
void playTone(int tone, int duration) {
    for (long i = 0; i < duration * 1000L; i += tone * 2) {
        digitalWrite(speakerPin, HIGH);
        delayMicroseconds(tone);
        digitalWrite(speakerPin, LOW);
        delayMicroseconds(tone);
    }
}

void playNote(char note, int duration) {
    char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
    int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };

    for (int i = 0; i < 8; i++) {
        if (names[i] == note) {
            playTone(tones[i], duration);
        }
    }
}
void setup() {
  pinMode(mainLedPin, OUTPUT);
  pinMode(speakerPin, OUTPUT);
  sim808.begin(9600);
  Serial.begin(9600);
  delay(1000);
  Serial.println("Set up done!");
}

void loop() 
{
    int analogvaule = analogRead(A0);
    Serial.print("cnt is: ");
    Serial.print( cnt );
    Serial.print( "\n" );

    cnt++;
    
    //Read the value of light sensor
    if ( analogvaule > threshold )
    {
      Serial.println("Status: Door opened!!!");
      if ( openFlag == 0 )
      {
        //call();
        sendSMS();
        playNote('g', 200 );
        openFlag = 1;
      }
      else
      {
        playNote('a', 200 );
      }
    }
    else
    {
      openFlag = 0;
    }
    if ( phase == 0 )
    {
        phase = 1;
        digitalWrite(mainLedPin, HIGH);
    }
    else
    {
        phase = 0;
        digitalWrite(mainLedPin, LOW );
    }
    delay( 200 );
}

void at(String _atcm,unsigned long _dl){
  sim808.print(_atcm+"\r\n");
  delay(_dl);
}
