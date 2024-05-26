/*                                                                  
                                                 THE RUINS OF HAMPI -- DANCER PUZZLE
 */

#include <Arduino.h>
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
BluetoothSerial SerialBT;

// Handle received and sent messages
String message = "";
char incomingChar;


const int Relay = 4;         // digital pin connected to RELAY
const int BUZZ = 13;

const int PB11 = 16; //define Button port1
const int PB12 = 17; //define Button port2
const int PB13 = 18; //define Button port3
const int PB14 = 19; //define Button port4
const int PB21 = 21; //define Button port5
const int PB22 = 22; //define Button port6
const int PB23 = 23; //define Button port7
const int PB24 = 25; //define Button port8
const int PB31 = 26; //define Button port9
const int PB32 = 27; //define Button port10
const int PB33 = 32; //define Button port11
const int PB34 = 33; //define Button port12

int dArray[] = {9, 2, 7, 12, 1};                  // CHANGE HERE TO CHANGE THE SEQUENCE
int inArray[5];
int data = 0;
int aCounter = 0;
int rst;
int strtTime = 0;
int curTime = 0;       // Stores the current time in seconds
int elapseTime = 5000; // Time the programs waits if no input is provided by user

boolean buttonPB11Active;
boolean buttonPB12Active;
boolean buttonPB13Active;
boolean buttonPB14Active;
boolean buttonPB21Active;
boolean buttonPB22Active;
boolean buttonPB23Active;
boolean buttonPB24Active;
boolean buttonPB31Active;
boolean buttonPB32Active;
boolean buttonPB33Active;
boolean buttonPB34Active;

void BLE_Override();
int evaluate();
void BUZZ_SHORT();
void BUZZ_INT();
void BUZZ_LNG();
void overRide();

void  setup()
{
  pinMode(Relay, OUTPUT);      //define "ledPin_ready" as a output port
  pinMode(BUZZ, OUTPUT);              //define BUZZ as buzzer output port
  pinMode(PB11, INPUT);               //define  PB11 as a input port
  pinMode(PB12, INPUT);               //define  PB12 as a input port
  pinMode(PB13, INPUT);               //define  PB13 as a input port
  pinMode(PB14, INPUT);               //define  PB14 as a input port
  pinMode(PB21, INPUT);               //define  PB21 as a input port
  pinMode(PB22, INPUT);               //define  PB22 as a input port
  pinMode(PB23, INPUT);               //define  PB23 as a input port
  pinMode(PB24, INPUT);               //define  PB24 as a input port
  pinMode(PB31, INPUT);               //define  PB31 as a input port
  pinMode(PB32, INPUT);               //define  PB32 as a input port
  pinMode(PB33, INPUT);               //define  PB33 as a input port
  pinMode(PB34, INPUT);               //define  PB34 as a input port
  Serial.begin(115200);

  SerialBT.begin("DANCER_BLE"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");

  Serial.println("Starting Puzzel .....");
  digitalWrite(PB11, LOW);
  digitalWrite(PB12, LOW);
  digitalWrite(PB13, LOW);
  digitalWrite(PB14, LOW);
  digitalWrite(PB21, LOW);
  digitalWrite(PB22, LOW);
  digitalWrite(PB23, LOW);
  digitalWrite(PB24, LOW);
  digitalWrite(PB31, LOW);
  digitalWrite(PB32, LOW);
  digitalWrite(PB33, LOW);
  digitalWrite(PB34, LOW);
  digitalWrite(Relay, LOW);
  digitalWrite(BUZZ, LOW);
  strtTime = millis();
}

void  loop()
{
  /*  Reset Button Pushed
      Game Master Control
  */

  curTime = millis();

  BLE_Override();

  if (digitalRead(PB11) == HIGH)
  {
    if (buttonPB11Active == false)
    {
      buttonPB11Active = true;
      data = 1;
      BUZZ_SHORT();
      Serial.println("BUTTON PB11 PRESSED");
      if (aCounter < 5)
      {
        inArray[aCounter] = data;
        aCounter ++;
      }
      strtTime = millis();
    }
  }


  else if (digitalRead(PB12) == HIGH)
  {
    if (buttonPB12Active == false)
    {
      buttonPB12Active = true;
      data = 2;
      BUZZ_SHORT();
      Serial.println("BUTTON PB12 PRESSED");
      if (aCounter < 5)
      {
        inArray[aCounter] = data;
        aCounter ++;
      }
      strtTime = millis();
    }
  }

  else if (digitalRead(PB13) == HIGH)
  {
    if (buttonPB13Active == false)
    {
      buttonPB13Active = true;
      data = 3;
      BUZZ_SHORT();
      Serial.println("BUTTON PB13 PRESSED");
      if (aCounter < 5)
      {
        inArray[aCounter] = data;
        aCounter ++;
      }
      strtTime = millis();
    }
  }

  else if (digitalRead(PB14) == HIGH)
  {
    if (buttonPB14Active == false)
    {
      buttonPB14Active = true;
      data = 4;
      BUZZ_SHORT();
      Serial.println("BUTTON PB14 PRESSED");
      if (aCounter < 5)
      {
        inArray[aCounter] = data;
        aCounter ++;
      }
      strtTime = millis();
    }
  }

  else if (digitalRead(PB21) == HIGH)
  {
    if (buttonPB21Active == false)
    {
      buttonPB21Active = true;
      data = 5;
     BUZZ_SHORT();
      Serial.println("BUTTON PB21 PRESSED");
      if (aCounter < 5)
      {
        inArray[aCounter] = data;
        aCounter ++;
      }
      strtTime = millis();
    }

  }

  else if (digitalRead(PB22) == HIGH)
  {
    if (buttonPB22Active == false)
    {
      buttonPB22Active = true;
      data = 6;
     BUZZ_SHORT();
      Serial.println("BUTTON PB22 PRESSED");
      if (aCounter < 5)
      {
        inArray[aCounter] = data;
        aCounter ++;
      }
      strtTime = millis();
    }
  }

  else if (digitalRead(PB23) == HIGH)
  {
    if (buttonPB23Active == false)
    {
      buttonPB23Active = true;
     BUZZ_SHORT();
      Serial.println("BUTTON PB23 PRESSED");
      data = 7;
      if (aCounter < 5) {
        inArray[aCounter] = data;
        aCounter ++;
      }
      strtTime = millis();
    }
  }
  else if (digitalRead(PB24) == HIGH)
  {
    if (buttonPB24Active == false)
    {
      buttonPB24Active = true;
      BUZZ_SHORT();
      Serial.println("BUTTON PB24 PRESSED");
      data = 8;
      if (aCounter < 5) {
        inArray[aCounter] = data;
        aCounter ++;
      }
      strtTime = millis();
    }
  }

  else if (digitalRead(PB31) == HIGH)
  {
    if (buttonPB31Active == false)
    {
      buttonPB31Active = true;
      BUZZ_SHORT();
      Serial.println("BUTTON PB31 PRESSED");
      data = 9;
      if (aCounter < 5) {
        inArray[aCounter] = data;
        aCounter ++;
      }
      strtTime = millis();
    }
  }

  else if (digitalRead(PB32) == HIGH)
  {
    if (buttonPB32Active == false)
    {
      buttonPB32Active = true;
      BUZZ_SHORT();
      Serial.println("BUTTON PB32 PRESSED");
      data = 10;
      if (aCounter < 5) {
        inArray[aCounter] = data;
        aCounter ++;
      }
      strtTime = millis();
    }
  }

  else if (digitalRead(PB33) == HIGH)
  {
    if (buttonPB33Active == false)
    {
      buttonPB33Active = true;
      BUZZ_SHORT();
      Serial.println("BUTTON PB33 PRESSED");
      data = 11;
      if (aCounter < 5) {
        inArray[aCounter] = data;
        aCounter ++;
      }
      strtTime = millis();
    }
  }

  else if (digitalRead(PB34) == HIGH)
  {
    if (buttonPB34Active == false)
    {
      buttonPB34Active = true;
     BUZZ_SHORT();
      Serial.println("BUTTON PB34 PRESSED");
      data = 12;
      if (aCounter < 5) {
        inArray[aCounter] = data;
        aCounter ++;
      }
      strtTime = millis();
    }
  }


  curTime = millis();
  //Time has elapsed or Read all values complete, evaluate and reset counter
  //
  if ((curTime - strtTime > elapseTime) || aCounter == 5)
  {
    if (aCounter != 5)
    {
      // The user has not input all the values but time has expired, rest the counter and show failed indicator
      Serial.println("INCORRECT SEQUENCE");
      BUZZ_INT(); 
      buttonPB11Active = false;
      buttonPB12Active = false;
      buttonPB13Active = false;
      buttonPB14Active = false;
      buttonPB21Active = false;
      buttonPB22Active = false;
      buttonPB23Active = false;
      buttonPB24Active = false;
      buttonPB31Active = false;
      buttonPB32Active = false;
      buttonPB33Active = false;
      buttonPB34Active = false;
      delay(50);
     
    }
    else
    {
      evaluate();
    }
    aCounter = 0;
  }
  delay(100);
}


// // Method to evaluate the default code with input data
int evaluate()
{
  int retVal = 1;
  for (int e = 0; e < 5; e++)
  {
    Serial.print(inArray[e]);
    Serial.print("\n");
    if (inArray[e] != dArray[e])
    {
      Serial.println("INCORRECT SEQUENCE");
      BUZZ_INT();
      buttonPB11Active = false;
      buttonPB12Active = false;
      buttonPB13Active = false;
      buttonPB14Active = false;
      buttonPB21Active = false;
      buttonPB22Active = false;
      buttonPB23Active = false;
      buttonPB24Active = false;
      buttonPB31Active = false;
      buttonPB32Active = false;
      buttonPB33Active = false;
      buttonPB34Active = false;
      return 0;
    }
  }
  digitalWrite(Relay, HIGH);              // Relay will turn ON forever and hence the TRAP door will OPEN once the correct sequence is detected
  Serial.println("CORRECT SEQUENCE");
  BUZZ_LNG();
  digitalWrite(Relay, HIGH);
  delay(3000);                                     // GREEN led  will be ON for 3 Sec if CORRECT Sequence is detected
  digitalWrite(Relay, LOW);
  return retVal;
}

void BUZZ_SHORT()
{
 digitalWrite(BUZZ, HIGH);
  delay(100);
  digitalWrite(BUZZ, LOW); 
}

void BUZZ_INT()
{
  for(int i=0;i<5;i++)
  {
   digitalWrite(BUZZ, HIGH);
   delay(100);
   digitalWrite(BUZZ, LOW); 
   delay(100);
  }
}


void BUZZ_LNG()
{
  digitalWrite(BUZZ, HIGH);
  delay(1000);
  digitalWrite(BUZZ, LOW); 
}



// External Override Game Master input
void overRide()
{
  Serial.println("OVER RIDING PUZZLE");
  digitalWrite(Relay, HIGH);
  delay(3000);
  digitalWrite(Relay, LOW);
  aCounter = 0;
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



