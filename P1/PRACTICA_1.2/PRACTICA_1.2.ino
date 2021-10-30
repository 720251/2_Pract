const int pinPot = 34;

int valADC = 0;
volatile int interruptCounter;
int totalInterruptCounter;
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);
}

void setup() {
  Serial.begin(115200);
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);
}

int leePot() {
  int resul = analogRead(pinPot);
  return resul;
}


void loop() {
  if (interruptCounter > 0) {
    portENTER_CRITICAL(&timerMux);
    interruptCounter--;
    portEXIT_CRITICAL(&timerMux);
    //Serial.println(totalInterruptCounter);
    totalInterruptCounter++;
    if (totalInterruptCounter == 10) {
      valADC = leePot();
      Serial.println(valADC);
      totalInterruptCounter = 0;
    }
  }

}
