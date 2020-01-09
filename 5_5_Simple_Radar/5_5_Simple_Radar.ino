#define TRIGGER_PIN  7
#define ECHO_PIN     8
#define SERVO_PIN    3
#define MAX_DISTANCE 200
#define BUFFERSIZE 6
#include <NewPing.h>
#include <Servo.h>     

// We use two libraries here to illustrate that you need not always write things from scratch

Servo zRotate;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
     
void setup() {
    zRotate.attach(SERVO_PIN);
    zRotate.write(0);
    Serial.begin(115200);
    }

     //angle, distance
void loop() {
    int mAngle = 0;
    int xAngle = 75;
    int distance = 0;
    for (int angle=mAngle; angle<xAngle; angle= angle + 5){
      distance = noFilterMoveRead(angle);
      Serial.print(angle);
      Serial.print(",");
      Serial.print(distance);  
      Serial.print("\n");
      delay(50);
    }

    for (int angle=xAngle; angle>mAngle ; angle= angle - 5){
      distance = noFilterMoveRead(angle);
      Serial.print(angle);
      Serial.print(",");
      Serial.print(distance);  
      Serial.print("\n");
      delay(50);
    }
    
  }

int noFilterMoveRead(int angle){
  int reading;
  zRotate.write(angle);
  
  //allow for some time to settle  
  delay(10);
  reading = sonar.ping();
  return reading;  
  }
