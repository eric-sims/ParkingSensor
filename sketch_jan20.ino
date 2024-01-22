
/*
* Garage Parking Sensor - Published By Bob Torrence
* forked and recreated by Eric Sims
*/
#include   <FastLED.h>
#include <QuickStats.h>
#include <stdio.h>

QuickStats stats; //initialize an instance   of this class
// defining the pins
#define LED_PIN     7
#define NUM_LEDS     90
const int trigPin = 9;
const int echoPin = 10;
// defining variables
CRGB   leds[NUM_LEDS];
float duration;
float durationarray[15];
int distance;
float   stopdistance=20.0;   //parking position from sensor (CENTIMETERS)
float startdistance=100.0;   //distance from sensor to begin scan as car pulls in(CENTIMETERS) 
int increment=((startdistance-stopdistance)/15);

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin,   INPUT); // Sets the echoPin as an Input
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  Serial.begin(9600); // Starts the serial communication

}

void loop() {
  distance = calculateDistance();
  // Prints the distance on the Serial   Monitor
  Serial.print("Distance: ");
  Serial.println(distance);

  displayBar(distance);
}

int calculateDistance() {
  for (int i=0;i<=14;i++) {
    // Clears the trigPin
    digitalWrite(trigPin,   LOW);
    delayMicroseconds(2);
    
    // Sets the trigPin on HIGH state for 10 micro   seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin,   LOW);

    // Reads the echoPin, returns the sound wave travel time in microseconds
    durationarray[i] = pulseIn(echoPin, HIGH);
    distance= durationarray[i]*0.034/2;
    // Serial.print(distance);
    // Serial.print("   ");
  }

  duration = (stats.median(durationarray,15));
  // Calculating the   distance
  return duration*0.034/2;
}

void displayBar(int distance) {
  if (isPastStopDistance(distance)) {
    displayAllRed();
    return;
  }

  float normDistance = normalizeDistance(distance);

  if (normDistance < 0.99 && normDistance > 0.1) {
    displayPartialYellow(normDistance);
  } else if (normDistance > 1.0) {
    displayAllGreen();
  }
  
}

void displayPartialYellow(float normalizedDistance) {
  int ledsToShow = normalizedDistance * NUM_LEDS;
  Serial.println(ledsToShow);

  for (int i = 0; i < ledsToShow; i++) {
    yellowPixel(i);
  }

  for (int i = ledsToShow; i < NUM_LEDS; i++) {
    blackPixel(i);
  }

  FastLED.show();
  delay(50);
}

void displayAllRed() {
  for (int i = 0; i < NUM_LEDS; i++) {
    redPixel(i);
  }
  FastLED.show();
  delay(50);
}

void displayAllGreen() {
  for (int i = 0; i < NUM_LEDS; i++) {
    greenPixel(i);
  }
  FastLED.show();
  delay(50);
}

void blackPixel(int pixelNum) {
  leds[pixelNum] = CRGB(0,0,0);
}

void greenPixel(int pixelNum) {
  leds[pixelNum] = CRGB(0,255,0);
}

void yellowPixel(int pixelNum) {
  leds[pixelNum] = CRGB(255,255,0);
}

void redPixel(int pixelNum) {
  leds[pixelNum] = CRGB(255,0,0);
}

bool isPastStopDistance(int distance) {
  return distance <= stopdistance;
}

float normalizeDistance(int distance) {
  return distance/startdistance;
}



