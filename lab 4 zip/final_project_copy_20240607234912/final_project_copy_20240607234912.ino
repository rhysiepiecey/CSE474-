#include <Arduino_FreeRTOS.h>
/* Include the standard Arduino SPI library */
#include <SPI.h>
/* Include the RFID library */
#include <RFID.h>
#define trigPin 6  
#define   echoPin 5
#define buzzer 2
float new_delay; 

/* Define the DIO used for the SDA (SS) and RST (reset) pins. */
#define SDA_DIO 9
#define RESET_DIO 8
/* Create an instance of the RFID library */
RFID RC522(SDA_DIO, RESET_DIO); 

// define two tasks for Blink & AnalogRead
void TaskBlink( void *pvParameters );
void TaskAnalogRead( void *pvParameters );
void TaskRFID( void *pvParameters );
void TaskSensor ( void *pvParameters);

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
    TaskRFID
    ,  "TaskRFID"
    ,  128  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL );
    xTaskCreate(
    TaskSensor
    ,  "Sensor"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  4  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
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


 void setupBlink() {
  /* Enable the SPI interface */
  SPI.begin(); 
  /* Initialise the RFID reader */
  RC522.init();
  }   

  void TaskRFID(void *pvParameters) {


    pinMode(41, OUTPUT);
    
    digitalWrite(41, LOW);    // turn the LED off by making the voltage LOW
    vTaskDelay( 100 / portTICK_PERIOD_MS ); // wait for one second
    digitalWrite(41, HIGH);   // turn the LED on (HIGH is the voltage level)
    vTaskDelay( 250 / portTICK_PERIOD_MS ); // wait for one second


    
  
}

 void TaskSensor(void *pvParameters) { 
    pinMode(trigPin, OUTPUT); 
    pinMode(echoPin, INPUT);
    pinMode(buzzer,OUTPUT);
  int rfid = getRFID();
  if(rfid == 127) {
    vTaskSuspend(NULL);
  }
  long duration,   distance;
  digitalWrite(trigPin, LOW);                     
   digitalWrite(trigPin, HIGH);          
  digitalWrite(trigPin,   LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
   new_delay= (distance * 2) + 30;
  if (distance < 50 && distance > 1)
  {
   digitalWrite(buzzer,HIGH);
   vTaskDelay(new_delay/portTICK_PERIOD_MS);
    digitalWrite(buzzer,LOW);
 
  }
  else
  {
    digitalWrite(buzzer,LOW);

   }
  
    vTaskDelay(1/portTICK_PERIOD_MS);
 }

  int getRFID() {
        setupBlink();
        int RFID = 0;
      /* Has a card been detected? */
    if (RC522.isCard())
  {
    /* If so then get its serial number */
    RC522.readCardSerial();
    Serial.println("Card detected:");
    // for(int i=0;i<5;i++)
    // // {
    // Serial.print(RC522.serNum[i],DEC);
    for(int i=0;i<5;i++) {
    RFID = RC522.serNum[i];
    }
    return RFID;
    //Serial.print(RC522.serNum[i],HEX); //to print card detail in Hexa Decimal format
    // }
    
    Serial.println();
    Serial.println();
  }
  vTaskDelay( 100 / portTICK_PERIOD_MS ); // wait for one second
  }
