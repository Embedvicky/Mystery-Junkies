#include <Arduino.h>
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
BluetoothSerial SerialBT;

#define OVER_RIDE_PIN 16
#define KEY_E 17
#define KEY_B 18
#define KEY_G 19
#define KEY_Em 21
#define RELAY 2
#define ACTIVATE 1
#define DEACTIVATE 0
#define BUZZ 23

unsigned long prev_time = 0;
unsigned long current_time = 0;
const unsigned int reset_time = 5000; //How long before the counter resets when there is no keys played on xylo
bool e = false;
bool b = false;
bool g = false;
bool em = false;
uint8_t counter = 0;
uint8_t i = 0;
char seq[] = {'E', 'G', 'B', 'e', 'B', 'G'}; //Required Sequence
char current_key = 'z';
char prev_key = 'z';
uint8_t max_counter = sizeof(seq);

// Handle received and sent messages
String message = "";
char incomingChar;


// put function declarations here:

void BLE_Override();
void check_sequence();
void checkTimeOut();
void openDoor();
void trapDoor(bool closed);
void checkOverRide();
void beep();

void setup() 
{

  {
  Serial.begin(115200);
  SerialBT.begin("XYLOPHONE"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");

  //Assigning the pins as input with pullups so that the default state is HIGH and goes low when the mallet strikes
  pinMode(KEY_E, INPUT_PULLUP); //E
  pinMode(KEY_B, INPUT_PULLUP); //B
  pinMode(KEY_G, INPUT_PULLUP); //G
  pinMode(KEY_Em, INPUT_PULLUP); //Em
  pinMode(OVER_RIDE_PIN, INPUT_PULLUP);
  pinMode(RELAY, OUTPUT);
  pinMode(BUZZ,OUTPUT);
  digitalWrite(BUZZ,LOW);
  }
  
}

void loop() 
{

{ 
  BLE_Override();
 // checkOverRide();
  //Reading and storing data from the input pins
  e = digitalRead(KEY_E);
  b = digitalRead(KEY_B);
  g = digitalRead(KEY_G);
  em = digitalRead(KEY_Em);
  if (!e) //Check if Middle E is pressed
    current_key = 'E';
  else if (!b)
    current_key = 'B';
  else if (!g)
    current_key = 'G';
  else if (!em)
    current_key = 'e';

  if (current_key != prev_key && current_key != 'z')      //check if the current key pressed is same as the previous key and go in only if not
  {
    Serial.print(current_key);
    SerialBT.println(current_key);

    Serial.println("pressed");
    SerialBT.println("pressed");

    prev_key = current_key;                               //update the previous key press value
    check_sequence();                                     //check the current key with the sequence
    prev_time = millis();                                 //Reset the previous time value
  }

  if (counter == max_counter)                             //check if the current number of presses = number of presses in required sequence
  {
    counter = 0;                                          //Reset counter
    current_key = 'z';                                    //Reset current key
    prev_key = 'z';                                       //Reset prev key

    Serial.println("Sequence correct! Trigger Relay");
    SerialBT.println("Sequence correct! Trigger Relay");

    beep();
    delay(1000);
    openDoor();

  Serial.println("Back Online");
  SerialBT.println("Back Online");

  }
  checkTimeOut();                                         //check the interval between last press and current press
}
}

// put function definitions here:

void check_sequence()
{
  if (current_key == seq[counter])
  {
  Serial.println("Right!");
  SerialBT.println("Right!");

    counter++;                                            //Counter increases only if right key is played
  }
  else
  {
  Serial.println("Wrong key! Restart Sequence!");
  SerialBT.println("Wrong key! Restart Sequence!");

    counter = 0;                                          //Reset counter if the wrong key is pressed
  }
}

void checkTimeOut()
{
  current_time = millis();
  if (current_time - prev_time > reset_time && counter != 0)                        //if no key is pressed for "reset_time" millisec, then go into condition
  {
    counter = 0;                                                                    //Reset counter so user can start from beginning
    current_key = 'z';

  Serial.println("No Response from user...Restart Sequence");
  SerialBT.println("No Response from user...Restart Sequence");
    
    prev_time = current_time;                                                       //Reset previous time
  }
}

void openDoor()
{
  trapDoor(DEACTIVATE);
  delay(reset_time);
  trapDoor(ACTIVATE);
}

void trapDoor(bool closed)
{
  if (closed)
    digitalWrite(RELAY, LOW);
    
  else
    digitalWrite(RELAY, HIGH);
}

void checkOverRide()
{
  if (!digitalRead(OVER_RIDE_PIN))
  {
Serial.println("OVER RIDE PRESSED");
SerialBT.println("OVER RIDE PRESSED");
  openDoor();
  }
}

void beep()
{
  digitalWrite(BUZZ,HIGH);
  delay(1000);
  digitalWrite(BUZZ,LOW);
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
