#include <Arduino.h>
#include "BluetoothSerial.h"

#define potPin1 25
#define potPin2 26
#define potPin3 27
#define potPin4 14
#define DOORrelay 16
#define POWERrelay 17
#define GREENledPin 18
#define REDledPin 19
//#define OVER_RIDE_PIN 0


           // IMPORTANT LIBRARY TO BE ADDED AS WE ARE READING AND WRITING TO EEPROM FOR POTENTIOMETER CALIBRATION


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
BluetoothSerial SerialBT;

const int oveRidPin = 0;
const int rst = 1;

int period = 500;
unsigned long time_now = 0;

int val1, val2, val3, val4;
boolean doorFlag = false;
const int thresh11 = 1500;
const int thresh12 = 1750;
const int thresh21 = 3600;
const int thresh22 = 4200;
const int thresh31 = 1050;
const int thresh32 = 1300;
const int thresh41 = 1900;
const int thresh42 = 2200;


// Handle received and sent messages
String message = "";
char incomingChar;

void overRide();
void checkOverRide();
void BLE_Override();

void setup()
{

  pinMode(GREENledPin, OUTPUT);          // declare the greeN ledPin as an OUTPUT
  digitalWrite(GREENledPin, LOW);        // drive greeN ledPin  LOW
  pinMode(DOORrelay, OUTPUT);            // declare the doorrelayPin as an OUTPUT
  digitalWrite(DOORrelay, LOW);          // drive DOOR relay pin  LOW
  pinMode(POWERrelay, OUTPUT);           // declare the power relayPin as an OUTPUT
  digitalWrite(POWERrelay, LOW);         // drive relay power pin  LOW
  pinMode(REDledPin, OUTPUT);            // declare the RED ledPin as an OUTPUT
  digitalWrite(REDledPin, HIGH);         // drive RED led pin  HIGH
  // pinMode(oveRidPin, INPUT_PULLUP);      // declare oveRidPin as an INPUT
  // digitalWrite(oveRidPin, HIGH);         // drive oveRidPin  LOW


  analogReadResolution(12);


  Serial.begin(115200);

  SerialBT.begin("MARSHALL"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop()
{

    if(millis() >= time_now + period){
        time_now += period;
  BLE_Override();
  digitalWrite(REDledPin, HIGH);
  
  val1 = analogRead(potPin1);
  val2 = analogRead(potPin2);
  val3 = analogRead(potPin3);
  val4 = analogRead(potPin4);
  

  Serial.print("Pot1");
  Serial.print("--");
  Serial.println(val1);


  // read the value from the OVERDRIVE GAIN
  Serial.print("Pot2");
  Serial.print("--");
  Serial.println(val2);


  // read the value from the OVERDRIVE VOLUME
  Serial.print("Pot3");
  Serial.print("--");
  Serial.println(val3);


  // read the value from the CONTOUR
  Serial.print("Pot4");
  Serial.print("--");
  Serial.println(val4);

   // CHECK FOR THE CORRECT KNOB POSITIONS and TRIGGER THE OUTPUT

  if ((val1 >= thresh11 &&  val1 <= thresh12) &&
      (val2 >= thresh21 &&  val2 <= thresh22) &&
      (val3 >= thresh31 &&  val3 <= thresh32) &&
      (val4 >= thresh41 &&  val4 <= thresh42))
  {
    Serial.println("Knobs are in correct position");
    {
      if (!doorFlag)
    {
      digitalWrite(REDledPin, LOW);       // turn the REDledPin OFF
      digitalWrite(GREENledPin, HIGH);    // turn the GREENledPin on
      digitalWrite(POWERrelay, HIGH);     // turn the RELAYPin on which power on the MUSIC PEDALS
      digitalWrite(DOORrelay, HIGH);      // turn the RELAYPin on which OPENS THE TRAP DOOR
      doorFlag = true;
    }
      Serial.println("Correct sequence");
    }

  }
  else
  {
  
          doorFlag = false;
    digitalWrite(DOORrelay, LOW);
    digitalWrite(POWERrelay, LOW);     // turn the RELAYPin on which power on the MUSIC PEDALS
    digitalWrite(GREENledPin, LOW);     // turn the GREENledPin off
    digitalWrite(REDledPin, HIGH);
    Serial.println("Wrong sequence");
    }

    }

}

void overRide()
{
  Serial.println("OVER RIDING PUZZLE");
  digitalWrite(DOORrelay, HIGH);  // turn the RELAYPin on which power on the MUSIC PEDALS
  digitalWrite(POWERrelay, HIGH); // turn the RELAYPin on which OPENS THE TRAP DOOR
  doorFlag = true;
  delay(3000);
  digitalWrite(DOORrelay, LOW);   // turn the RELAYPin on which OPENS THE TRAP DOOR
}

// void checkOverRide()
// {
//   if (!digitalRead(OVER_RIDE_PIN))
//   {
// Serial.println("OVER RIDE PRESSED");
// SerialBT.println("OVER RIDE PRESSED");
//   overRide();
//   }
// }

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

