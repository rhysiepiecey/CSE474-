#include <Arduino_FreeRTOS.h>

////////////////////////////////////////////////
// APPROVED FOR ECE 474   Spring 2021
//
//  NOTE: modify analogRead() on line 113 according
//   to your setup.
////////////////////////////////////////////////
#define E 659 // Hz
#define C 523 // Hz
#define G 784 // Hz
#define g 392 // Hz
#define R 0   // Rest, play no sound
int melody[] = {E, R, E, R, R, E, R, R, C, R, E, R, R, G, R, R, R, R, R, g, R}; //mario
int noteDurations[] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
// define two tasks for Blink & AnalogRead
void TaskBlink( void *pvParameters );
void TaskAnalogRead( void *pvParameters );

// the setup function runs once when you press reset or power the board
void setup() {
  
  // initialize serial communication at 9600 bits per second:
  Serial.begin(19200);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
  } 

  // Now set up two tasks to run independently.
  xTaskCreate(
    TaskBlink
    ,  "Blink"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

  xTaskCreate(
    TaskAnalogRead
    ,  "AnalogRead"
    ,  128  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL );

  xTaskCreate(
    TaskBlink_OffBoard
    ,  "Blink_Off"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  3  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );  

  xTaskCreate(
    TaskPlaySong_Mario
    ,  "Mario"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  3  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );  
  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
  //  (note how the above comment is WRONG!!!)
  vTaskStartScheduler();


}

void loop()
{
  // Empty. Things are done in Tasks.
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskBlink(void *pvParameters)  // This is a task.
{
 // (void) pvParameters;  // allocate stack space for params

/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, LEONARDO, MEGA, and ZERO 
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN takes care 
  of use the correct LED pin whatever is the board used.
  
  The MICRO does not have a LED_BUILTIN available. For the MICRO board please substitute
  the LED_BUILTIN definition with either LED_BUILTIN_RX or LED_BUILTIN_TX.
  e.g. pinMode(LED_BUILTIN_RX, OUTPUT); etc.
  
  If you want to know what pin the on-board LED is connected to on your Arduino model, check
  the Technical Specs of your board  at https://www.arduino.cc/en/Main/Products
  
  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
  
  modified 2 Sep 2016
  by Arturo Guadalupi
*/

  // initialize digital LED_BUILTIN on pin 13 as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  for (;;) // A Task shall never return or exit.
  {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    vTaskDelay( 250 / portTICK_PERIOD_MS ); // wait for one second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    vTaskDelay( 100 / portTICK_PERIOD_MS ); // wait for one second
  }
}

void TaskAnalogRead(void *pvParameters)  // This is a task.
{
 // (void) pvParameters;
  
/*
  AnalogReadSerial
  Reads an analog input on pin 0, prints the result to the serial monitor.
  Graphical representation is available using serial plotter (Tools > Serial Plotter menu)
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.
*/
  for (;;)
  {
    // read the input on analog pin 0:
    int sensorValueX = analogRead(A0);  /// modify for your input pin!
    int sensorValueY = analogRead(A1);
    // print out the value you read:
    Serial.print("X Value: ");
    Serial.println(sensorValueX);
    Serial.print("Y Value: ");
    Serial.println(sensorValueY);
    vTaskDelay(500/portTICK_PERIOD_MS);  // 0.5 sec in between reads for stability
  }
}

void TaskBlink_OffBoard(void *pvParameters)  // This is a task.
{


  // initialize digital LED_BUILTIN on pin 13 as an output.
  pinMode(12, OUTPUT);

  for (;;) // A Task shall never return or exit.
  {
    //fix timer
    digitalWrite(12, HIGH);   // turn the LED on (HIGH is the voltage level)
    vTaskDelay( 200 / portTICK_PERIOD_MS); // wait for one second
    digitalWrite(12, LOW);    // turn the LED off by making the voltage LOW
    vTaskDelay( 100 / portTICK_PERIOD_MS ); // wait for one second
  }
}


void TaskPlaySong_Mario(void *pvParameters)  // This is a task.
{


  // initialize digital LED_BUILTIN on pin 13 as an output.
  pinMode(9, OUTPUT);
  TCCR1A = (1 << WGM10) | (1 << WGM11) | (1 << COM1A0);  // Fast PWM, toggle OC1A on compare match
  TCCR1B = (1 << WGM12) | (1 << WGM13) | (1 << CS10);   // Fast PWM, no prescaler

  for (;;) // A Task shall never return or exit.
  {
    //fix timer
      for (int thisNote = 0; thisNote < sizeof(melody) / sizeof(melody[0]); thisNote++) {
    // Get the duration of the current note
    int noteDuration = noteDurations[thisNote];
    
    if (melody[thisNote] == R) {
      // Rest, play no sound
      noTone();
      delay(noteDuration);
    } else {
      // Play the tone by setting the frequency
      playTone(melody[thisNote]);
      vTaskDelay(noteDuration / portTICK_PERIOD_MS);
    }
    // Pause between notes to create a delay
    delay(noteDuration * 1.30);
  }
  
  // Sleep for 4 seconds before playing the song again
  delay(4000);
  }

}
void playTone(int frequency) {
  // Calculate the timer value for the given frequency
  int timerValue = (16000000 / (2 * frequency)) - 1;
  OCR1A = timerValue;  // Set the output compare register value
}

void noTone() {
  // Disable the timer output
  TCCR1A &= ~(1 << COM1A0);
}