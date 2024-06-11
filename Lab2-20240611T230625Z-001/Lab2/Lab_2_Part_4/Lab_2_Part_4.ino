#define OP_DECODEMODE  8
#define OP_SCANLIMIT   10
#define OP_SHUTDOWN    11
#define OP_DISPLAYTEST 14
#define OP_INTENSITY   10

#define DDR3 DDRB
#define PORT3 PORTB

#define PIN_12 1 << 4; // pin 47
#define PIN_11 1 << 3; // pin 48
#define PIN_10 1 << 2; // pin 49

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


// #define DIN 12 // the pin location of DIN 
// #define CS 10 // the pin location of CS 
// #define CLK 11 // the pin location of CLK 

//Transfers 1 SPI command to LED Matrix for given row
//Input: row - row in LED matrix
//       data - bit representation of LEDs in a given row; 1 indicates ON, 0 indicates OFF
void spiTransfer(volatile byte row, volatile byte data);

byte spidata[2]; //spi shift register uses 16 bits, 8 for ctrl and 8 for data
int DIN = 12;
int CS =  10;
int CLK = 11;
// sets up the necessary output pins for the LED dot matrix
void setup() {
  Serial.begin(115200);

  pinMode(DIN, OUTPUT);

  pinMode(CS, OUTPUT);
  pinMode(CLK, OUTPUT);
  
  digitalWrite(CS, HIGH);
  //PORTB |= CS;
  // DDRB |= PB6; // initializes the DIN pin as an output
  // DDRB |= PB4; // initializes the CS pin as an output
  // DDRB |= PB5; // initializes the CLK pin as an output
  PORTB |= CS; // sets the CS pin to HIGH
  spiTransfer(OP_DISPLAYTEST,0);
  spiTransfer(OP_SCANLIMIT,7);
  spiTransfer(OP_DECODEMODE,0);
  spiTransfer(OP_SHUTDOWN,1);


  DDRH |= (1 << 3); 

  TCCR4A = 0;
  TCCR4A |= (1 <<  COM4A0);
  TCCR4B = 0;
  TCCR4B |= (1 << WGM42) | (1 << CS40);
  TCCR4C = 0;
}
int index = 0;
int j = 0;
// moves a dot around the LED matrix based on the position of the thumbstick
void loop() {
  int xVal = analogRead(A0); // fractional voltage reading from the A0 analog pin
  int yVal = analogRead(A1); // fractional voltage reading from the A1 analog pin

  int xPos = map(xVal, 0, 1023, 0, 7); // maps the xVal to a value between 0 and 7
  int yPos = map(yVal, 0, 1023, 0, 7); // maps the yVal to a value between 0 and 7
  // Serial.println(xPos);
  // Serial.println(yVal);
  spiTransfer(yPos, 1 << (7 - xPos)); // turns on the LED at the row and column corresponding to xPos and yPos
  delay(50); // pauses the loop function for 50 ms
  spiTransfer(yPos, 0b00000000); // turns off the LED that was previously turned on
  //     int j = 0;
  // int i = 0;
  // for (j = 0; j < 8; j++){ //for each row, set the LEDs
  //   spiTransfer(j, 0b10100101);
    
  // }
  // delay(500);
  // for (i = 0; i < 8; i++){ //for each row, clear the LEDs
  //   spiTransfer(i, 0b00000000);
  // }
  // delay(500);

  if(j % 3 == 0) {
      OCR4A =  8 * melody[index];
    index++;
    if(index == 53) {
      OCR4A = 0;
      index = 0;
    }
  }
  j++;

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


void spiTransfer(volatile byte opcode, volatile byte data){
  int offset = 0; //only 1 device
  int maxbytes = 2; //16 bits per SPI command
  
  for(int i = 0; i < maxbytes; i++) { //zero out spi data
    spidata[i] = (byte)0;
  }
  //load in spi data
  spidata[offset+1] = opcode+1;
  spidata[offset] = data;
   digitalWrite(CS, LOW); //
  //PORTB &= !CS;
  for(int i=maxbytes;i>0;i--)
    shiftOut(12,11,MSBFIRST,spidata[i-1]); //shift out 1 byte of data starting with leftmost bit
   digitalWrite(CS,HIGH);
  //PORTB |= CS;
}