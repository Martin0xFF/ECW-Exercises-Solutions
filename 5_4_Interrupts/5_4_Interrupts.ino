// Interrupts stop the processor and forces it to enter a different code block when a signal is asserted
// to the interrupt pin

// modify the code so that it turns off the IR LED when the counter reaches 10
// you will also need to slightly modify the circuit

#define ledPin 13

unsigned int counter = 0;

void setup() {
  
    Serial.begin(115200);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);
    
    // Interrupts can only be attached to pin 2 and 3 on the arduino uno
    attachInterrupt(digitalPinToInterrupt(2), count, RISING);
}

void loop() {
    if (counter > 9){
        digitalWrite(ledPin, HIGH);
      }
}

void count(){
  counter = counter + 1;
  Serial.print("Counter: ");
  Serial.println(counter);
  }
