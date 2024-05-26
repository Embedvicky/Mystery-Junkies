#include <Arduino.h>
#include "BluetoothSerial.h"

//#define OVER_RIDE_PIN 0
#define RELAY 16
#define BUZZER 17
#define ACTIVATE 1
#define DEACTIVATE 0
//#define GNDPIN 18
#define BUT1 25
#define BUT2 26
#define BUT3 27
#define BUT4 33

#define DELAY 50

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
BluetoothSerial SerialBT;

unsigned long lastKeyPressTime = 0;
unsigned long current_time = 0;
const unsigned int reset_time = 5000;           //How long before the counter resets


bool buttonState1 = 0;
bool buttonState2 = 0;
bool buttonState3 = 0;
bool buttonState4 = 0;

bool lastButtonState1 = 0;
bool lastButtonState2 = 0;
bool lastButtonState3 = 0;
bool lastButtonState4 = 0;

bool currentButtonState1 = 0;
bool currentButtonState2 = 0;
bool currentButtonState3 = 0;
bool currentButtonState4 = 0;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 40;



int givenSeq[] = {3, 3, 3, 4, 4, 2, 1, 1};       // Correct sequence , CHANGE HERE TO CHANGE THE SOLUTION
int keyPressed[sizeof(givenSeq)];                // input sequence
int i = 0;
uint8_t counter = 0;
uint8_t max_counter = (sizeof(givenSeq) / sizeof(givenSeq[0])); // to find the number of elements in the given sequence

// Handle received and sent messages
String message = "";
char incomingChar;

bool checkSequence();
void checkSwitch();
void checkTimeOut();
void openDoor();
void trapDoor(bool closed);
//void checkOverRide();
void wrongBeep();
void correctBeep();
void Beep();
void BLE_Override();

void setup() {
  // put your setup code here, to run once:
  pinMode(BUZZER, OUTPUT);
  pinMode(RELAY, OUTPUT);
  pinMode(BUT1, INPUT_PULLUP);
  pinMode(BUT2, INPUT_PULLUP);
  pinMode(BUT3, INPUT_PULLUP);
  pinMode(BUT4, INPUT_PULLUP);
  //pinMode(OVER_RIDE_PIN, INPUT_PULLUP);
  //pinMode(GNDPIN,OUTPUT);
 // digitalWrite(GNDPIN,LOW);

  digitalWrite(BUZZER, LOW);
  digitalWrite(RELAY, LOW);
  //delay(1000);
  currentButtonState1 = digitalRead(BUT1);
  currentButtonState2 = digitalRead(BUT2);
  currentButtonState3 = digitalRead(BUT3);
  currentButtonState4 = digitalRead(BUT4);
  Serial.begin(115200);

  SerialBT.begin("MUSIC_PEDAL"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop()
{
  // put your main code here, to run repeatedly:
  //checkOverRide();
  BLE_Override();
  currentButtonState1 = digitalRead(BUT1);
  currentButtonState2 = digitalRead(BUT2);
  currentButtonState3 = digitalRead(BUT3);
  currentButtonState4 = digitalRead(BUT4);
  if (counter == max_counter)
  {
    Serial.print("max:");
    Serial.println(max_counter);
    counter = 0;
    checkSequence();
    int keyPressed[sizeof(givenSeq)];
    Serial.println("Back Online");
  }
  checkSwitch();
  checkTimeOut();
}


bool checkSequence()
{
  // Method to evaluate the default code with input data
  int retVal = 1;
  for (int e = 0; e < 8; e++)
  {
    Serial.print(keyPressed[e]);
    Serial.print("\n");
    if (keyPressed[e] != givenSeq[e])
    {
      Serial.println("INCORRECT SEQUENCE DETECTED");
      wrongBeep();
      return 0;
    }
  }
  Serial.println("CORRECT SEQUENCE DETECTED");
  correctBeep();
  openDoor();
  return retVal;
}




void checkSwitch()
{
  if (currentButtonState1 != lastButtonState1) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (currentButtonState1 != buttonState1) {
       buttonState1 = currentButtonState1;
      if (buttonState1 == LOW) {
            keyPressed[counter] = 1;
            Beep();
            Serial.println("Pressed button 1-");
            counter++;
            delay(DELAY);
            lastKeyPressTime = millis();
      }
    }
  }
  lastButtonState1 = currentButtonState1;


    if (currentButtonState2 != lastButtonState2) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (currentButtonState2 != buttonState2) {
       buttonState2 = currentButtonState2;
      if (buttonState2 == LOW) {

            keyPressed[counter] = 2;
            Beep();
            Serial.println("Pressed button 2-");
            counter++;
            delay(DELAY);
            lastKeyPressTime = millis();
      }
    }
  }
  lastButtonState2 = currentButtonState2;

    if (currentButtonState3 != lastButtonState3) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (currentButtonState3 != buttonState3) {
       buttonState3 = currentButtonState3;
      if (buttonState3 == LOW) {
            keyPressed[counter] = 3;
            Beep();
            Serial.println("Pressed button 3-");
            counter++;
            delay(DELAY);
            lastKeyPressTime = millis();
      }
    }
  }
  lastButtonState3 = currentButtonState3;

    if (currentButtonState4 != lastButtonState4) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (currentButtonState4 != buttonState4) {
       buttonState4 = currentButtonState4;
      if (buttonState4 == LOW) {
            keyPressed[counter] = 4;
            Beep();
            Serial.println("Pressed button 4-");
            counter++;
            delay(DELAY);
            lastKeyPressTime = millis();
      }
    }
  }
  lastButtonState4 = currentButtonState4;
}


void checkTimeOut()
{
  current_time = millis();
  if (current_time - lastKeyPressTime > reset_time && counter != 0)                        //if no key is pressed for "reset_time" millisec, then go into condition
  {
    counter = 0;                                                                    //Reset counter so user can start from beginning
    Serial.println("No Response from user...Restart Sequence");
    lastKeyPressTime = current_time;                                                       //Reset previous time
    wrongBeep();
  }
}


void openDoor()
{
  trapDoor(DEACTIVATE);
  delay(3000);
  trapDoor(ACTIVATE);
}


void trapDoor(bool closed)
{
  if (closed)
    digitalWrite(RELAY, LOW);
  else
    digitalWrite(RELAY, HIGH);
  //Serial.println("trapDoor");
}
// void checkOverRide()
// {
//   if (!digitalRead(OVER_RIDE_PIN))
//   {
//     Serial.println("yallow");
//     openDoor();
//   }
// }
void wrongBeep()
{
  bool state = true;
  for (uint8_t i = 0; i < 10; i++)
  {
    digitalWrite(BUZZER, state);
    delay(100);
    state = !state;
  }
}


void correctBeep()
{
  digitalWrite(BUZZER, HIGH);
  delay(1000);
  digitalWrite(BUZZER, LOW);
}

void Beep()
{
  digitalWrite(BUZZER, HIGH);
  delay(100);
  digitalWrite(BUZZER, LOW);
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
    openDoor();
  }
}



