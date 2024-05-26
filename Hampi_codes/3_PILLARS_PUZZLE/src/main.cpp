#include <Arduino.h>
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
BluetoothSerial SerialBT;

// Handle received and sent messages
String message = "";
char incomingChar;

int Delay = 100;
unsigned long time_now = 0;

const int Relay = 16;

void overRide();
void BLE_Override();


void setup() {

   pinMode(Relay, OUTPUT);        // Declaring led_Relay as an out put port
   digitalWrite(Relay, LOW);      // Drive Led_Relay LOW
   Serial.begin(115200);
   SerialBT.begin("3_PILLARS_PUZZLE"); //Bluetooth device name
   Serial.println("The device started, now you can pair it with bluetooth!");
   Serial.println("Starting Puzzel .....");

}

void loop() {

        if(millis() >= time_now + Delay){
        time_now += Delay;
        }
  // put your main code here, to run repeatedly:
}

void overRide()
{
  Serial.println("OVER RIDING PUZZLE");
  digitalWrite(Relay, HIGH);
  delay(3000);
  digitalWrite(Relay, LOW); 
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