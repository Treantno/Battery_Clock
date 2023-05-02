#include <Arduino.h>
#include <TimeLib.h>

#include <MAX72XX.h>
#include <MAX72XX.hpp>

int latchPin = 0;
int numDevices = 2;

MAX72XX dispChain(latchPin, numDevices);
MAX72XXDisplay  Disp_min(&dispChain, 8, 2);
MAX72XXDisplay  Disp_sec(&dispChain, 10, 2);
MAX72XXDisplay  Disp2(&dispChain, 4, 4);

unsigned long startTime; // to store the start time in milliseconds
unsigned long startTime_zero; // to store the start time in milliseconds
int restart;
const int countdownTime = (60 * 60 * 1000); // 1 hour in milliseconds
const int countdownTime_zero = (60 * 1000); // 60 seconds in milliseconds
//const int countdownTime = (10 * 1000); // 10 seconds in milliseconds
//const int countdownTime_zero = (10 * 1000); // 10 seconds in milliseconds

void setup() {
  Serial.begin(115200);
  dispChain.setIntensity(4); // set the intensity for all the devices
  startTime = millis(); // record the start time
  restart = 0;
}

int reversedigits(int number)
{
    int reversed=0,multiply=0;

    if(number <= 9)
      multiply=10;
    
    while (number > 0) { // loop until all digits have been reversed
      int digit = number % 10; // extract the last digit
      reversed = reversed * 10 + digit; // add the digit to the reversed number
      number /= 10; // remove the last digit from the original number
    }

    if(multiply)
      reversed = reversed * 10;

    return reversed;
}

void loop() {
  unsigned long elapsedTime = millis() - startTime; // calculate the elapsed time
  long remainingTime = countdownTime - elapsedTime; // calculate the remaining time
  unsigned long elapsedTime_zero = millis() - startTime_zero; // calculate the elapsed time for bombactive
  long remainingTime_zero = countdownTime_zero - elapsedTime_zero; // calculate the remaining time for bombactive
  int cnt2 = random(0,9999);
  
  if (remainingTime > 0) // if time remains
  {
    int half_second = remainingTime % 1000;
    int seconds = remainingTime / 1000 % 60;
    int minutes = remainingTime / (1000 * 60) % 60;

    Disp_min.writeNumber(reversedigits(minutes), MAX72XX::Character::ZERO);
    Disp_sec.writeNumber(reversedigits(seconds), MAX72XX::Character::ZERO);

    if(half_second >= 500)
      Disp_min.setDecimalPoint(1);
    else
      Disp_min.setDecimalPoint(5);
  }
  else if(restart == 0)
  {
    startTime_zero = millis();
    restart = 1;
    dispChain.setIntensity(15); // set the intensity for all the devices

  }
  else
  {
     int half_second = elapsedTime % 1000;

    if(half_second >= 500)
      dispChain.shutdown(0); // Blink display
    else
      dispChain.shutdown(1); // Blink display

    if(remainingTime_zero <= 0)
    {
      startTime = millis(); // record the start time to restart
      dispChain.shutdown(0); // Blink display
      restart = 0;
      dispChain.setIntensity(4); // set the intensity for all the devices
    }
     
    cnt2=0;
  }

  Disp2.writeNumber(cnt2, MAX72XX::Character::ZERO);
 
  delay(100);
}