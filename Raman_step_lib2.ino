#include <ezButton.h>
#include <AccelStepper.h>

#define LIMIT_SWITCH_PIN  6
#define MOTOR_STEP_PIN    7  
#define MOTOR_DIR_PIN     2
#define MOTOR_INTERFACE_TYPE 1 
#define PHOTORESISTOR_PIN A0

// ls == limit switch
// pos == position
// pd == photodiode

ezButton limitSwitch(LIMIT_SWITCH_PIN);
AccelStepper stepper(MOTOR_INTERFACE_TYPE, MOTOR_STEP_PIN, MOTOR_DIR_PIN);

bool LS_flag = false;
bool Raman_flag = false;
float posDegree = 0.00;
int posStep = 0;
int pdMeasure = 0;

void setup() {
  Serial.begin(115200);
  limitSwitch.setDebounceTime(50);
  pinMode(PHOTORESISTOR_PIN,INPUT);
}

void loop() {
  limitSwitch.loop();
  int lsCount = limitSwitch.getCount() % 3;

  pdMeasure=analogRead(A0);
  
  if(lsCount == 0 && !Raman_flag){
    stepper.setMaxSpeed(1000);
    stepper.setSpeed(500);
    stepper.runSpeed();
    stepper.setCurrentPosition(0);
  }
  
  if(lsCount == 1 && !Raman_flag){
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
       Serial.println(pdMeasure);
    }
  }

  if(limitSwitch.isPressed()){
    Raman_flag = false;
    posDegree = 0.00;
    posStep = 0;
  }

  if(lsCount == 2 && !Raman_flag){
    stepper.moveTo(-400);
    stepper.setSpeed(400);
    stepper.runSpeedToPosition();
  }
}
