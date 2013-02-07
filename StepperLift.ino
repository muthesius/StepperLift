// QuadStepper Test Sketch for quick and easy testing

// Use with Serial Console @9600 Baud
// s: one turn up
// d: one turn down
// 1-4: stepsize 1,2,4,8,16
// +: upwards permanent
// -: downwards permanent
// #: halt

const int torque = 0;
static int stepSize = 16;
static bool run = false;

// bound detection pins:
const int highFuse = 2;
const int lowFuse = 3;

void disableAll() {
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(10,HIGH);
      run = false;

}

void intSteppers() {
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  disableAll();
}


void enableAll() {
  digitalWrite(6,LOW);
  digitalWrite(7,LOW);
  digitalWrite(9,LOW);
  digitalWrite(10,LOW);
      run = true;

}

void stepAll(int speed){
  digitalWrite(5,HIGH);
  delayMicroseconds(speed);
  digitalWrite(5,LOW);
  delayMicroseconds(speed);
}

#define STEPMIN 800
// (STEPMIN + (_torque * 260)) / 4;
void stepTurns(float turns) {
  if (!run) return;
  int ss = (stepSize==0?1:stepSize);
  int steptime = (STEPMIN + (torque * 260)) / ss;
  int numSteps = int(200*turns) * (ss/16) * ss;
  for (int i=0; i<numSteps; i++) {
    stepAll(steptime);
  }
}

static int direction = 0;
void setDirection(int dir) {
  direction = dir;
  if (dir > 0) {
    enableAll();
    digitalWrite(11,LOW);
  }
  else if (dir < 0) {
    enableAll();
    digitalWrite(11,HIGH);
  }
  else if(dir==0) {
    disableAll();
  }
}

boolean atUpperEnd(){
  return direction > 0 && digitalRead(highFuse) == HIGH;
}

boolean atLowerEnd(){
  return direction < 0 && digitalRead(lowFuse) == HIGH;
}


void setup() {
  Serial.begin(9600);
  
  pinMode(highFuse,INPUT);
  pinMode(lowFuse,INPUT);

  intSteppers();
  
  pinMode(11,OUTPUT);
  setDirection(0);
  
  pinMode(5,OUTPUT);
}

void loop() {
  if(Serial.available()) {
    char cmd = Serial.read();
    switch (cmd) {
      case '+':
        setDirection(1);
        break;
      case '-':
        setDirection(-1);
        break;
      case '#':
        setDirection(0);
        break;
      case 's':
        setDirection(1);
        stepTurns(1);
        setDirection(0);
        break;
      case 'd':
        setDirection(-1);
        stepTurns(1);
        setDirection(0);
        break;
      case '1':
        stepSize = 2;
        break;
      case '2':
        stepSize = 4;
        break;
      case '3':
        stepSize = 8;
        break;
      case '4':
        stepSize = 16;
        break;
    }
  }

  if( atUpperEnd() || atLowerEnd() ) {
    // Stop the movement
    setDirection(0);
  }

  stepTurns(1);
  
}


