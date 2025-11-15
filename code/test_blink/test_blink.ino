void setup() {
  // setup pin 5 as a digital output pin
  pinMode (25, OUTPUT);
}

void loop() {
  digitalWrite (25, HIGH);	// turn on the LED
  delay(1000);	// wait for half a second or 500 milliseconds
  digitalWrite (25, LOW);	// turn off the LED
  delay(1000);	// wait for half a second or 500 milliseconds
}