#define DDR3 DDRL 
#define PORT3 PORTL

#define PIN_47 PL4 // pin 47
#define PIN_48 PL2 // pin 48
#define PIN_49 PL3 // pin 49

void setup() {
  Serial.begin(115200);
  DDR3 |= PIN_47; // initializes pin 47 as an output
  DDR3 |= PIN_48; // initializes pin 48 as an output
  DDR3 |= PIN_49; // initializes pin 49 as an output

  DDRH |= (1 << 3); 

  TCCR4A = 0;
  TCCR4A |= (1 <<  COM4A0);
  TCCR4B = 0;
  TCCR4B |= (1 << WGM42) | (1 << CS40);
  TCCR4C = 0;
}

void loop() {
  taskC();
}

void taskC() {
  taskA(2);
  taskB(4);
  taskAAndB(10);
  delay(1000);
}

void taskAAndB(int n) {
  OCR4A = 0;
  int prevTime = millis();
  int currTime = millis();
  int sounds[] {19999, 31999, 9999, 0};
  int i = 0;
  while (currTime - prevTime < n * 1000) {
    PORT3 |= PIN_47;  // turns pin 47 on
    delay(333);       // pauses for 333 ms
    PORT3 &= !PIN_47; // turns pin 47 off

    PORT3 |= PIN_48;  // turns pin 48 on 
    delay(333);       // pauses for 333 ms
    PORT3 &= !PIN_48; // turns pin 48 off

    PORT3 |= PIN_49;  // turns pin 49 on 
    delay(333);       // pauses for 333 ms
    PORT3 &= !PIN_49; // turns pin 49 off
    currTime = millis();
        OCR4A = sounds[i];
    i++;
    if(i == 4) {
      i = 0;
    }
  }
}

void taskA(int n) {
  OCR4A = 0;
  int prevTime = millis();
  int currTime = millis();
  while (currTime - prevTime < n * 1000) {
    PORT3 |= PIN_47;  // turns pin 47 on
    delay(333);       // pauses for 333 ms
    PORT3 &= !PIN_47; // turns pin 47 off

    PORT3 |= PIN_48;  // turns pin 48 on 
    delay(333);       // pauses for 333 ms
    PORT3 &= !PIN_48; // turns pin 48 off

    PORT3 |= PIN_49;  // turns pin 49 on 
    delay(333);       // pauses for 333 ms
    PORT3 &= !PIN_49; // turns pin 49 off
    currTime = millis();
  }
}

void taskB(int n) {
  PORT3 &= !PIN_47;
  PORT3 &= !PIN_48;
  PORT3 &= !PIN_49;

  int prevTime = millis();
  int currTime = millis();
  while (currTime - prevTime < n * 1000) {
    OCR4A = 19999; //400
    delay (1000); // delay pauses for 1s

    OCR4A = 31999; //250
    delay (1000);

    OCR4A = 9999; //800
    delay (1000);

    OCR4A = 0;
    delay (1000);
    currTime = millis();
  }
}