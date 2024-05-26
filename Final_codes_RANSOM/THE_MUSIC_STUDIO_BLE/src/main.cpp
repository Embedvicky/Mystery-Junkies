#include <Arduino.h>
#include "BluetoothSerial.h"

#define relay 16
#define relay_disp 17
#define ledGREEN 18
#define ledRED 19
#define BUZZ 25
#define ADC 26
int adc = 0;

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
BluetoothSerial SerialBT;

// Handle received and sent messages
String message = "";
char incomingChar;

void activate();
void BLE_Override();

void setup() 
{
pinMode(relay,OUTPUT);
digitalWrite(relay,LOW);
pinMode(relay_disp,OUTPUT);
digitalWrite(relay_disp,LOW);

pinMode(ledGREEN,OUTPUT);
digitalWrite(ledGREEN,LOW);
pinMode(ledRED,OUTPUT);
digitalWrite(ledRED,HIGH);
pinMode(BUZZ,OUTPUT);
digitalWrite(BUZZ,LOW);

Serial.begin(115200);
SerialBT.begin("THE MUSIC STUDIO"); //Bluetooth device name
Serial.println("The device started, now you can pair it with bluetooth!");

}

void loop() 
{
  BLE_Override();
  adc = analogRead(ADC);                // ANALOG INPUT A0 PORT
  Serial.println(adc);
  delay(100);
  if(adc < 2000)                       // CHANGE THE VALUE HERE TO SENSITIVITY OF THE HANDS (100 BEING VERY SENSITIVE)
  {
    Serial.println("ON");
    activate();
    delay(100);
  }
  else
  {
     digitalWrite(ledRED,HIGH);
     digitalWrite(ledGREEN,LOW);
     Serial.println("OFF");
     digitalWrite(relay,LOW);
     delay(100);
  }
}

void activate()
{
    digitalWrite(ledRED,LOW);
    digitalWrite(ledGREEN,HIGH);
    digitalWrite(relay,LOW);
    for (int i = 0; i<15; i++)
    {
    digitalWrite(BUZZ,HIGH);
    delay(200);
    Serial.println("buzz on");
    }
    digitalWrite(BUZZ,LOW);
    digitalWrite(relay_disp,HIGH);
    delay(6000);
    digitalWrite(relay,HIGH);
    delay(30000);
    digitalWrite(relay_disp,LOW);
    
 }

 void BLE_Override()
{
// Read received messages (LED control command)
  if (SerialBT.available()){
    char incomingChar = SerialBT.read();
    if (incomingChar != '\n'){
      message += String(incomingChar);
    }
    else{
      message = "";
    }
  //Serial.println(incomingChar);  
  }
  // Check received message and control output accordingly
  if (message =="BLE")
  {
    Serial.println("BLE OVER RIDE");
    SerialBT.println("BLE OVER RIDE");
    activate();
  }
}