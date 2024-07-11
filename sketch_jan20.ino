
/*
* Garage Parking Sensor - Published By Bob Torrence
*/
#include <FastLED.h>
#include <QuickStats.h>
#include <stdio.h>

QuickStats stats; 

#define LED_PIN  3
#define NUM_LEDS 90
#define TRIG_PIN 9
#define ECHO_PIN 10

CRGB   leds[NUM_LEDS];
float duration;
float durationarray[15];
int distance;
float   stopdistance=30.0;   //parking position from sensor (CENTIMETERS)
float startdistance=350.0;   //distance from sensor to begin scan as car pulls in (CENTIMETERS) 
int increment=((startdistance-stopdistance)/15);

void setup() {
  pinMode(TRIG_PIN, OUTPUT); // Sets the TRIG_PIN as an Output
  pinMode(ECHO_PIN,   INPUT); // Sets the ECHO_PIN as an Input
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  Serial.begin(9600); // Starts the serial communication

}

void loop() {
  distance = calculateDistance();
  displayBar(distance);
}

int calculateDistance() {
  for (int i=0;i<=14;i++) {
    // Clears the TRIG_PIN
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    
    // Sets the TRIG_PIN on HIGH state for 10 microseconds
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Reads the ECHO_PIN, returns the sound wave travel time in microseconds
    durationarray[i] = pulseIn(ECHO_PIN, HIGH);
    distance= durationarray[i]*0.034/2;
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

  if (normDistance > 1.0) {
    displayAllGreen();
  } else if (normDistance > 0.1) {
    displayPartialYellow(normDistance);
  }
  
}

void displayPartialYellow(float normalizedDistance) {
  int ledsToShow = normalizedDistance * NUM_LEDS;

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



