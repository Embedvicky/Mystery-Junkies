#include <Arduino.h>
#include "BluetoothSerial.h"

const int relay = 18; 
const int BUZZ = 22;

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
BluetoothSerial SerialBT;

// Handle received and sent messages
String message = "";
char incomingChar;

const int pB1 = 27;             //define PAUSE Button port
const int pB2 = 26;             //define STOP/EJECT Button port
const int pB3 = 25;             //define FFWD Button port
const int pB4 = 33;             //define REWIND Button port
const int pB5 = 32;             //define PLAY Button port
const int pB6 = 21;             //define RECORD Button port

int dArray[] = {5, 3, 5, 4, 3, 1}; // SEQ : PLAY- FF- PLAY- REWIND- FF - PAUSE 
int inArray[6];
int data = 0;
int aCounter = 0;
int strtTime = 0;
int curTime = 0;                 // Stores the current time in seconds
int elapseTime = 5000;           // Time the programs waits if no input is provided by user

void BLE_Override();
int evaluate();
void BUZZ_SHORT();
void BUZZ_INT();
void BUZZ_LNG();
void overRide();
void waitLoopForUserInput(int sNum);

void setup() {
  pinMode(relay, OUTPUT);            
  pinMode(BUZZ, OUTPUT); 
  pinMode(pB1, INPUT_PULLUP);               //define  pB11 as a input port
  pinMode(pB2, INPUT_PULLUP);               //define  pB12 as a input port
  pinMode(pB3, INPUT_PULLUP);               //define  pB13 as a input port
  pinMode(pB4, INPUT_PULLUP);               //define  pB14 as a input port
  pinMode(pB5, INPUT_PULLUP);               //define  pB21 as a input port
  pinMode(pB6, INPUT_PULLUP);               //define  pB22 as a input port

  Serial.begin(115200);
  SerialBT.begin("VHS_PLAYER"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");

  Serial.println("Starting Puzzel .....");
  digitalWrite(pB1, HIGH);
  digitalWrite(pB2, HIGH);
  digitalWrite(pB3, HIGH);
  digitalWrite(pB4, HIGH);
  digitalWrite(pB5, HIGH);
  digitalWrite(pB6, HIGH);

  digitalWrite(relay, LOW);
  digitalWrite(BUZZ, LOW);
  strtTime = millis();

}

void loop() {
  // put your main code here, to run repeatedly:
   
   
   curTime = millis();
   BLE_Override();


    if (digitalRead(pB1) == LOW)
  {
    waitLoopForUserInput(pB1);
    data = 1;
    BUZZ_SHORT();
    Serial.println("BUTTON PAUSE PRESSED");
    if (aCounter < 6)
    {
      inArray[aCounter] = data;
      aCounter ++;
    }
    strtTime = millis();
  }

  else if (digitalRead(pB2) == LOW)
  {
    waitLoopForUserInput(pB2);
    data = 2;
    BUZZ_SHORT();
    Serial.println("BUTTON STOP/EJECT PRESSED");
    if (aCounter < 6)
    {
     inArray[aCounter] = data;
     aCounter ++;
    }
    strtTime = millis();
  }


  else if (digitalRead(pB3) == LOW)
  {
    waitLoopForUserInput(pB3);
    data = 3;
    BUZZ_SHORT();
    Serial.println("BUTTON FFWD PRESSED");
    if (aCounter < 6)
    {
    inArray[aCounter] = data;
    aCounter ++;
    }
    strtTime = millis();
  }


   else if (digitalRead(pB4) == LOW)
  {
    waitLoopForUserInput(pB4);
    data = 4;
    BUZZ_SHORT();
    Serial.println("BUTTON REWIND PRESSED");
    if (aCounter < 6)
    {
      inArray[aCounter] = data;
      aCounter ++;
    }
    strtTime = millis();
  }


  else if (digitalRead(pB5) == LOW)
  {
     waitLoopForUserInput(pB5);
     data = 5;
     BUZZ_SHORT();
     Serial.println("BUTTON PLAY PRESSED");
     if (aCounter < 6)
     {
      inArray[aCounter] = data;
      aCounter ++;
     }
     strtTime = millis();
   }



   else if (digitalRead(pB6) == LOW)
   {
     waitLoopForUserInput(pB6);
     data = 6;
     BUZZ_SHORT();
     Serial.println("BUTTON RECORD PRESSED");
     if (aCounter < 6)
     {
       inArray[aCounter] = data;
       aCounter ++;
     }
     strtTime = millis();   
    }

curTime = millis();
//Time has elapsed or Read all values complete, evaluate and reset counter
if ((curTime - strtTime > elapseTime) || aCounter == 6)
{
  if (aCounter != 6)
  {
    // The user has not input all the values but time has expired, rest the counter and show failed indicator
    Serial.println("INCORRECT SEQUENCE");
  }
  else
  {
    evaluate();
  }
  aCounter = 0;
}
delay(500);
}



// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}

// // Method to evaluate the default code with input data
int evaluate()
{
  int retVal = 1;
  for (int e = 0; e < 6; e++)
  {
    Serial.print(inArray[e]);
    Serial.print("\n");
    if (inArray[e] != dArray[e])
    {
      Serial.println("INCORRECT SEQUENCE");
      BUZZ_INT();
      return 0;
    }
  }
  digitalWrite(relay, HIGH);              // Relay will turn ON forever and hence the TRAP door will OPEN once the correct sequence is detected
  Serial.println("CORRECT SEQUENCE");
  BUZZ_LNG();
  digitalWrite(relay, HIGH);
  delay(3000);                                     // GREEN led  will be ON for 3 Sec if CORRECT Sequence is detected
  digitalWrite(relay, LOW);
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
   delay(200);
   digitalWrite(BUZZ, LOW); 
   delay(200);
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
  digitalWrite(relay, HIGH);
  delay(3000);
  digitalWrite(relay, LOW);
  aCounter = 0;
}



// Method to overcome the problem of long button press
void waitLoopForUserInput(int sNum)
{
  do {
    // Will loop till the user releases the button
  }
  while (digitalRead(sNum) == HIGH);
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