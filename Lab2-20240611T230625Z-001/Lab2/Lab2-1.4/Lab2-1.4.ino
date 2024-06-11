

void setup() {
  // put your setup code here, to run once:
  DDRL |= (1<<4);
  DDRL |= (1<<5);
  DDRL |= (1<<7);
}

void loop() {
PORTL |= PL2; // set bit 5 to “1”
delay(333);
PORTL &= !PL2; // clear bit 5
delay(333);

PORTL |= PL1; // set bit 5 to “1”
delay(333);
PORTL &= !PL1; 

PORTL |= PL4; // set bit 5 to “1”
delay(333);
PORTL &= !PL4; 
}
