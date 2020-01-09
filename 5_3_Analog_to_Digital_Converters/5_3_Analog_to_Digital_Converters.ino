#define sensorPin A0    // select the input pin for the potentiometer
int sensorValue = 0;  // variable to store the value coming from the sensor

void setup() {
  //Start Serial communication on pin 0(Rx) and pin 1(Tx)
  Serial.begin(115200);
}

void loop() {
  // Read the Analog signal from pin A0, this number will be between 1023 and 0 (5v and 0V)
  // You can determine the resolution of the Analog to Digital converter by the number of bits
  // A good ADC will have a linear conversion
  
  sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);
  
  // If your while loop is sufficiently small, its good practice to have some delay, this helps to avoid race conditions
  delay(10);
  
}
