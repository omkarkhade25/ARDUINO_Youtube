
// Final code for the count down timer - start, pause, customizable timer 
// Library by Avishay Orpaz (tools menu--> manage libraries--> type 'tm1637'--> install library by Avishay Orpaz)
// Code edited by Omkar Khade


// Include Library
#include <Arduino.h>
#include <TM1637Display.h>

// Module connection pins (Digital Pins)
#define CLK 4
#define DIO 5

// Constants
const uint8_t OFF[] = {0, 0, 0, 0};
const unsigned int buzz_freq = 500;
short int buzz_pin = 9;

// Global Variables
long int count_time;                // 1000ms in 1sec, 60secs in 1min, 60mins in 1hr. So, 1000x60x60 = 3600000ms = 1hr
long int reset_value;               // copy this value into count_time when the button is pressed next time
unsigned long NewTime = 0;
unsigned long current_time = 0;
unsigned long previous_time = 0;
short int position_button = 6;            // Position button to Pin 6
short int increase_button = 3;            // Increase button to Pin 3
short int decrease_button = 7;            // Decrease button to Pin 7
short int start_button = 2;               // Start the timer button to Pin 8 (Start = pause button)
volatile short int start_pause = 0;
int seconds, minutes;
unsigned int left_value = 0;      
unsigned int right_value = 0;
int Num = 0;                              // For switching between left & right part of the display(Even = left, Odd = right)

TM1637Display display(CLK, DIO);

void setup(){

//Serial.begin(9600);
display.setBrightness(2);                 // Display brightness (0= min, 7= max)
display.clear();
pinMode(start_button, INPUT);
pinMode(increase_button, INPUT);
pinMode(decrease_button, INPUT);
pinMode(position_button, INPUT);
pinMode(buzz_pin, OUTPUT); 

/*seconds = (count_time % 60);                 // To display the countdown in mm:ss format
minutes = ((count_time / 60) % 60);          //separate CountTime in two parts

display.showNumberDecEx(seconds, 0, true, 2, 2);          
display.showNumberDecEx(minutes, 0b11100000, true, 2, 0);
delay(100);*/

// If start button changes from HIGH to LOW start/pause the timer
attachInterrupt(digitalPinToInterrupt(start_button), check_button, FALLING);    
}                                                                                

void loop(){

  if(start_pause == 1){

    delay(250);
    start_pause = 0;
    //Serial.println(left_value*60 + right_value);                   
    // Pass this value to countDown function to start the timer
    if(count_time == 0)
    {
      count_time = reset_value;
    }
    
    countDown();   // Function call   
  }

  if(digitalRead(position_button) == 0){
    
    Num++;
    Serial.println(Num);
    delay(200);
    
    // Sets the time for left part of the display
    while(Num % 2 == 0 && digitalRead(position_button) == 1 && start_pause == 0)    
    {
      
    if(digitalRead(increase_button) == 0){ 
      left_value++;
      delay(150);
    }
    
    if(digitalRead(decrease_button) == 0){  
      left_value--;
      delay(150);
    }
      display.showNumberDec(left_value, false, 2, 0);
      
 }    // End of 1st while loop
      display.clear();

    // Sets the time for right part of the display
    while(Num % 2 != 0 && digitalRead(position_button) == 1 && start_pause == 0)     
    {
      
    if(digitalRead(increase_button) == 0){  
      right_value++;
      delay(150);
      
    }
    if(digitalRead(decrease_button) == 0){  
      right_value--;
      delay(150);
      
    }
      display.showNumberDec(right_value, false, 2, 2);
      
}     // End of 2nd while loop
      display.clear();

      count_time = left_value*60 + right_value;       // This is for if you want to start the timer next time 
      reset_value = count_time;                       // with the previous set-time
}

}

void countDown(){
    delay(250);
    while(count_time > 0 && start_pause == 0)
      {
      current_time = millis();
              
    if(current_time - previous_time >= 1000)          // Update the display after every one second
    {
      NewTime = count_time - 1;                       // Calculate the time remaining 
    
      seconds = (NewTime % 60);                       // To display the countdown in mm:ss format
      minutes = ((NewTime / 60) % 60 );               //separate CountTime in two parts
   
      display.showNumberDecEx(seconds, 0, true, 2, 2); // Display the seconds in the last two places
    
      display.showNumberDecEx(minutes, 0b11100000, true, 2, 0); // Display the minutes in the first two places, with colon

      count_time = NewTime;                  // Update the time remaining
      previous_time = current_time;          // For one second time interval

      left_value = minutes;
      right_value = seconds;
    }    

    } //End of while loop
    
     if(count_time == 0) {
      // Count down is completed, now make the display OFF
      delay(200);
      start_pause = 0;
      Num = 0;
     for(int i = 0; i < 10; i++)           
      {
       // Turn ON the buzzer for 5 times and also Blink the display
       //tone(buzz_pin, buzz_freq);
       display.showNumberDec(0, true);
       delay(300);
       //noTone(buzz_pin);
       display.clear();
       delay(300);
      }
      
     }
     else {
      // Display the time stored when the Pause(Start) button is pressed
      display.showNumberDecEx(seconds, 0, true, 2, 2); 
      display.showNumberDecEx(minutes, 0b11100000, true, 2, 0); 
      start_pause = 0;
      delay(100);
      
     }
  }   //End of void countDown function
    
     
void check_button()
{
      start_pause = 1;
}















  












  
