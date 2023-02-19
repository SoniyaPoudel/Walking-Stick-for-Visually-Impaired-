#include <TinyGPS.h>
#include <SoftwareSerial.h>
SoftwareSerial Gsm(7, 8);
char phone_no[] = "+9779869049197"; //replace with phone no. to get sms
 int buttonpin=12;
TinyGPS gps;  //Creates a new instance of the TinyGPS object
 
const int buttonPin = 2;     // the number of the pushbutton pin
const int buzzPin1 =  13; // the number of the LED pin
// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
// Define pins for ultrasonic and buzzer
int const trigPin = 5;
int const echoPin = 6;
int const buzzPin2 = 3;
int duration, distance;
bool newData = false;
unsigned long chars;
unsigned short sentences, failed;
void setup() {
  // put your setup code here, to run once:
// initialize the Buzz pin as an output:
  Serial.begin(9600);
 Gsm.begin(9600);  
 pinMode(buttonpin,INPUT);
 Serial.println("press button");
  pinMode(buzzPin1, OUTPUT);
  // initialize the button pin as an input:
  pinMode(buttonPin, INPUT);
  pinMode(trigPin, OUTPUT); // trig pin will have pulses output
pinMode(echoPin, INPUT); // echo pin should be input to get pulse width
pinMode(buzzPin2, OUTPUT); // buzz pin is output to control buzzering
Serial.begin(9600);
}
void loop() {
  // put your main code here, to run repeatedly:
// read the state of the button value:
  buttonState = digitalRead(buttonPin);

  // check if the button is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    // turn buzz on:
    digitalWrite(buzzPin1, HIGH);
  } else {
    // turn buzz off:
    digitalWrite(buzzPin1, LOW);
  }
  // Duration will be the input pulse width and distance will be the distance to the obstacle in centimeters

// Output pulse with 1ms width on trigPin
digitalWrite(trigPin, HIGH);
delay(100);
digitalWrite(trigPin, LOW);
// Measure the pulse input in echo pin
duration = pulseIn(echoPin, HIGH);
// Distance is half the duration devided by 29.1 (from datasheet)
distance = (duration/2) / 29.1;
// if distance less than 0.5 meter and more than 0 (0 or less means over range)
if (distance <= 50 && distance >= 0) {
// Buzz
digitalWrite(buzzPin2, HIGH);
// Prints the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.println(distance);
} else {
// Don't buzz
digitalWrite(buzzPin2, LOW);
// Prints the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.println(distance);
}
// Waiting 60 ms won't hurt any one
delay(60);

 if(digitalRead(buttonpin)==HIGH)
  {
   Serial.print("hi");
  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (Serial.available())
    {
      char c = Serial.read();
      Serial.print(c);
      if (gps.encode(c)) 
        newData = true;  
    }
  }
 
  if (newData)      //If newData is true
  {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);   
    Gsm.print("AT+CMGF=1\r"); 
    delay(400);
    Gsm.print("AT+CMGS=\"");
    Gsm.print(phone_no);
    Gsm.println("\"");
    
    delay(300);
    Gsm.print("I need help!!! http://maps.google.com/maps?q=loc:");
    
   // Gsm.print("Latitude = ");
    Gsm.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    //Gsm.print(" Longitude = ");
    Serial.print(",");
    Gsm.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    delay(200);
    Gsm.println((char)26); // End AT command with a ^Z, ASCII code 26
    delay(200);
    Gsm.println();
    delay(20000);
 
  }
  }
  
 
  Serial.println(failed);
}    

