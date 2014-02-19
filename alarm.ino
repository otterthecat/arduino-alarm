/*

   The circuit:
    * +V connection of the PING))) attached to +5V
    * GND connection of the PING))) attached to ground
    * SIG connection of the PING))) attached to digital pin 7
    http://www.arduino.cc/en/Tutorial/Ping

    This sketch based on work by David A. Mellis & Tom Igoe
 */

// include the SoftwareSerial library so we can use it to talk to the Emic 2 module
#include <SoftwareSerial.h>


// Set pin for output of Ping:
const int pingPin = 7;
// Serial input (connects to Emic 2 SOUT)
#define rxPin 2
// Serial output (connects to Emic 2 SIN)
#define txPin 3

// set up a new serial port
SoftwareSerial emicSerial =  SoftwareSerial(rxPin, txPin);

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  // set the data rate for the Emic port
  emicSerial.begin(9600);

    // define pin modes
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);

    /*
    When the Emic 2 powers on, it takes about 3 seconds for it to successfully
    intialize. It then sends a ":" character to indicate it's ready to accept
    commands. If the Emic 2 is already initialized, a CR will also cause it
    to send a ":"
  */
  emicSerial.print('\n');             // Send a CR in case the system is already up
  while (emicSerial.read() != ':');   // When the Emic 2 has initialized and is ready, it will send a single ':' character, so wait here until we receive it
  delay(10);                          // Short delay
  emicSerial.flush();                 // Flush the receive buffer

  // set volume level for emic(18 max)
  emicSerial.print("V16\n");
  // set voice for emic
  emicSerial.print("N2\n");
}

void loop()
{
  // establish variables for duration of the ping,
  // and the distance result in inches and centimeters:
  long duration, inches, cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);

  // convert the time into a distance
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);

  if(7 > inches){

    // Speak some text
    // "S" tells the emicSerial to output speech
    emicSerial.print('S');
    // Send the desired string to convert to speech
    emicSerial.print("Intruder alert! You are " + String(inches) + "inches too close!");
    emicSerial.print('\n');
    // Wait here until the Emic 2 responds with a ":" indicating it's ready to accept the next command
    while (emicSerial.read() != ':');

    delay(2000);
  };
}

long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}