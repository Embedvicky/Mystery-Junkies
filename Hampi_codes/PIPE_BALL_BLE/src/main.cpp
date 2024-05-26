#include <Arduino.h>

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
BluetoothSerial SerialBT;

// Handle received and sent messages
String message = "";
char incomingChar;

int period = 100;
unsigned long time_now = 0;



const int ledPin_relay = 16;        // Relay pin 
const int knockSensor  = 25;       // sensoR connected to Analog Port A0
int val = 0;                       //define digital variable val 

const int THRESHOLD = 1500;          //Threshold value ( to set sensitivity)  CHANGE HERE TO CHANGE SENSITIVITY 

void overRide();
void BLE_Override();


void setup() 
{
   pinMode(ledPin_relay, OUTPUT);        // Declaring led_Relay as an out put port
   digitalWrite(ledPin_relay, LOW);      // Drive Led_Relay LOW
   Serial.begin(115200);
   SerialBT.begin("PIPE_BALL_BLE"); //Bluetooth device name
   Serial.println("The device started, now you can pair it with bluetooth!");
   Serial.println("Starting Puzzel .....");
}



void loop() 
{



      if(millis() >= time_now + period){
        time_now += period;
           BLE_Override();
           val = analogRead(knockSensor);

   if (val >= THRESHOLD)
   {
     digitalWrite(ledPin_relay,HIGH);
     delay(3000);
     digitalWrite(ledPin_relay,LOW);
     Serial.println("BALL HIT");
     Serial.println(val);
   } 
   Serial.println("NO BALL HIT");
   
      
    }
}
 
 
 
     
// External Override Game Master input
void overRide()
{
  Serial.println("OVER RIDING PUZZLE");
  digitalWrite(ledPin_relay, HIGH);
  delay(3000);
  digitalWrite(ledPin_relay, LOW); 
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
    overRide();
  }
}

