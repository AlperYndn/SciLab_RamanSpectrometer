#include <ezButton.h>
#include <AccelStepper.h>


int A=0;

ezButton limitSwitch(6);
AccelStepper stepper(1, 7, 2);
bool LS_flag = false;
bool Raman_flag = false;
float posDegree = 0.00;
int posStep;

void setup() {
  Serial.begin(115200);
  limitSwitch.setDebounceTime(50);
  pinMode(A0,INPUT);
}

void loop() {
  limitSwitch.loop();
  unsigned long count = limitSwitch.getCount();
  int myCount = count % 3;

  A=analogRead(A0);
  
  if(myCount == 0 && !Raman_flag){
    stepper.setMaxSpeed(1000);
    stepper.setSpeed(500);
    stepper.runSpeed();
    stepper.setCurrentPosition(0);
  }
  
  if(myCount == 1 && !Raman_flag){
    stepper.moveTo(-960);
    stepper.setSpeed(400);
    stepper.runSpeedToPosition();
  }
  
  if(stepper.currentPosition() == -960){
    Raman_flag = true;
  }

  if(Raman_flag == true){
    stepper.setSpeed(56);
    stepper.runSpeed();
    if(stepper.isRunning()){
      posStep = stepper.currentPosition() + 960;
      posDegree = posStep * 0.1125;
       Serial.print(posStep);
       Serial.print("|");
       Serial.print(posDegree);
       Serial.print("|");
       Serial.println(A);
    }
  }

  if(limitSwitch.isPressed()){
    Raman_flag = false;
    posDegree = 0.00;
    posStep = 0;
  }

  if(myCount == 2 && !Raman_flag){
    stepper.moveTo(-400);
    stepper.setSpeed(400);
    stepper.runSpeedToPosition();
  }
}
