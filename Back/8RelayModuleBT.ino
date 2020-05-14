
/*
  |\/|   | _  |
  |  |(|(|(/_ |)\/
                /
                  ___     _   _ _               _
                 / _ \___| |_(_) |_/\   /\_   _| | ___ __ _ _ __
                / /_)/ _ \ __| | __\ \ / / | | | |/ __/ _` | '_ \
               / ___/  __/ |_| | |_ \ V /| |_| | | (_| (_| | | | |
               \/    \___|\__|_|\__| \_/  \__,_|_|\___\__,_|_| |_|
*/

// Import Library
#include <SoftwareSerial.h>     // Create a virtual serial

// Define Bluetooth module
SoftwareSerial BTSerial(13, 4); // RX | TX  = > Bluetooth-TX=13 BT-RX=4

// Define push button inputs
const int pushButton[] = {A0, A1, A2, A3, A4, A5, 2, 3};
// Output pins where 8 relays will be connected
const int relayPin[] = {5, 6, 7, 8, 9, 10, 11, 12};
// Define name for 8 relays
String relayNames[] = {"CH1=", "CH2=", "CH3=", "CH4=", "CH5=", "CH6=", "CH7=", "CH8="};
// Status of each buttons
int pushed[] = {0, 0, 0, 0, 0, 0, 0, 0};
// Initial status of relay
int relayStatus[] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
// Define variable for bluetooth message
int messageBt;
// Define variable for time elapsed
int elapsedTime;

void setup()
{
  // Initialize serial monitor
  //Serial.begin(9600);
  // Initialize elapsed time to 0
  elapsedTime = 0;
  // Initialize Bluetooth
  BTSerial.begin(9600);
  // Setup iterator for all buttons and relay
  for (int i = 0; i < 8; i++)
  {
    // Define all buttons input pins
    pinMode(pushButton[i], INPUT_PULLUP);
    // Define all relay output pins
    pinMode(relayPin[i], OUTPUT);
    // initial relay status to be OFF
    digitalWrite(relayPin[i], HIGH);
  }
}

void loop()
{
  for (int i = 0; i < 8; i++)
  {
    // Read value of each button
    int  val = digitalRead(pushButton[i]);
    // Enter if button is pushed and relayStatus is LOW
    if (val == HIGH && relayStatus[i] == LOW)
    {
      // Inverted pushed status
      pushed[i] = 1 - pushed[i];
      // informs the front APP that the status has changed
      BTSerial.println(relayNames[i] + pushed[i]);
    }
    // relayStatus take val value
    relayStatus[i] = val;
    // Enter if pushed value is HIGH
    if (pushed[i] == HIGH)
    {
      // Turn ON relay
      digitalWrite(relayPin[i], LOW);
    }
    // Enter if pushed value is LOW
    else
    {
      // Turn OFF relay
      digitalWrite(relayPin[i], HIGH);
    }
  }
  //delay(100);

  while (BTSerial.available())
  {
    // Uncomment for debug
    //Serial.print("Recu sur bluetooth: ");
    // Receiving Bluetooth String and cast into int
    messageBt = BTSerial.readString().toInt();
    messageBt--;
    // Uncomment to see messageBt in Serial
    // Serial.println(messageBt);
    // Use 8 for get status of each relay
    if (messageBt == 8)
    {
      state();
    }
    // Use 0 to 7 to change each 8 relay status
    if (messageBt >= 0 && messageBt < 8)
    {
      // Inverted pushed status
      pushed[messageBt] = 1 - pushed[messageBt];
      // Informs the front APP of action
      BTSerial.println(relayNames[messageBt] + pushed[messageBt]);
    }
  }
}

// Methode to Informs front APP of the 8 relays states
void state()
{
  // For each 8 relay
  for (int i = 0; i < 8; i++)
  {
    // If pushed value is HIGH
    if (pushed[i] == HIGH)
    {
      // Informs front APP relay status is HIGH
      BTSerial.println(relayNames[i] + "1");
    }
    // If pushed value is LOW
    else
    {
      // Informs front APP relay status is LOW
      BTSerial.println(relayNames[i] + "0");
    }
    // Wait 100ms
    delay(100);
  }
}
