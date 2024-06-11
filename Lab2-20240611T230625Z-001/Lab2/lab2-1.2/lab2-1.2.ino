/*
  Blink

*/
int i = 0;
int a = 0;
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(47, OUTPUT);
  pinMode(48, OUTPUT);
  pinMode(49, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
    digitalWrite(47, HIGH);  // turn the SPEAKER
    delay(333);
    digitalWrite(47, LOW);

    digitalWrite(48, HIGH);
    delay(333);
    digitalWrite(48, LOW);

    digitalWrite(49, HIGH);
    delay(333);
    digitalWrite(49, LOW);
  }
