#define DDR3 DDRL
#define PORT3 PORTL

#define PIN_47 PL4 // pin 47
#define PIN_48 PL2 // pin 48
#define PIN_49 PL3 // pin 49

#define NOTE_c 3830 // 261 Hz
#define NOTE_d 3400 // 294 Hz
#define NOTE_e 3038 // 329 Hz
#define NOTE_f 2864 // 349 Hz
#define NOTE_g 2550 // 392 Hz
#define NOTE_a 2272 // 440 Hz
#define NOTE_b 2028 // 493 Hz
#define NOTE_C 1912 // 523 Hz
#define NOTE_R 0
int melody[] = {NOTE_e, NOTE_R, NOTE_d, NOTE_R, NOTE_c, NOTE_R, NOTE_d, NOTE_R, NOTE_e, NOTE_R, NOTE_e, NOTE_R, NOTE_e, NOTE_R, NOTE_d, NOTE_R, 
NOTE_d, NOTE_R, NOTE_d, NOTE_R, NOTE_e, NOTE_R, NOTE_g, NOTE_R, NOTE_g, NOTE_R, NOTE_e, NOTE_R, NOTE_d, NOTE_R, NOTE_c, NOTE_R, NOTE_d, NOTE_R, NOTE_e,
NOTE_R, NOTE_e, NOTE_R, NOTE_e, NOTE_R, NOTE_e, NOTE_R, NOTE_d, NOTE_R, NOTE_d, NOTE_R, NOTE_e, NOTE_R, NOTE_d, NOTE_R, NOTE_c, NOTE_R, NOTE_c};

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
  taskB(9);
  taskAAndB(9);
  delay(1000);
}


void taskAAndB(int n) {
  OCR4A = 0;
  int prevTime = millis();
  int currTime = millis();
  int i = 0;
  int j = 0;
  while (currTime - prevTime < n * 1000) {
    currTime = millis();
    OCR4A = 8 * melody[i];
    delay(166);

    if(j == 0) {
      PORT3 &= !PIN_49; // turns pin 49 off
      PORT3 |= PIN_47;  // turns pin 47 on
    } else if(j == 1) {
      PORT3 &= !PIN_47; // turns pin 47 off
      PORT3 |= PIN_48;  // turns pin 48 on 
    } else if(j == 2) {    
      PORT3 &= !PIN_48; // turns pin 48 off
      PORT3 |= PIN_49;  // turns pin 49 on 
    } else {

    }
    i++;
    if(i % 2 == 0) {
      j++;
    }
    if(j == 3) {
      j = 0;
    }
  }
  if(i == 53) {
      OCR4A = 0;
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
  //PORT3 &= !PIN_47;
  //PORT3 &= !PIN_48;
  //PORT3 &= !PIN_49;

  int prevTime = millis();
  int currTime = millis();

  int index = 0;

  while (currTime - prevTime < n * 1000) {
    OCR4A =  8 * melody[index];
    delay(166);
    index++;
    currTime = millis();
    if(index == 53) {
      OCR4A = 0;
    }
  }
}
