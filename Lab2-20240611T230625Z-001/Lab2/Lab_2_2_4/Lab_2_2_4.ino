/* University of Washington
* Lab 2: Digital I/O and timing of outputs
* Rhys SHah
*
*/


void setup() {
  // TCCR4B |= (1 << WGM42) | (1 << CS41) | (1 << CS40); // CTC Mode
  // TCCR4A |= (1 << COM4A0) | (1 << COM4A1);
  DDRH |= (1 << 3); 

  TCCR4A = 0;
  TCCR4A |= (1 <<  COM4A0);
  TCCR4B = 0;
  TCCR4B |= (1 << WGM42) | (1 << CS40);
  
}

void loop() {
  OCR4A = 19999; //400
  delay (1000); // delay pauses for 1s

  OCR4A = 32000; //250
  delay (1000);

  OCR4A = 9999; //800
  delay (1000);

  OCR4A = 0;
  delay (1000);
} // top = clock speed / (desired freq * prescaler)
