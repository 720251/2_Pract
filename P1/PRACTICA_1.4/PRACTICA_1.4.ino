const int pinPot = 34;

#define LED_GPIO 12  /* GPIO16 */
#define PWM1_Ch    0
#define PWM1_Res   12
#define PWM1_Freq  5000


String escritura;

int ValorEscritura = 0;
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
  ledcAttachPin(LED_GPIO, PWM1_Ch);
  ledcSetup(PWM1_Ch, PWM1_Freq, PWM1_Res);
  Serial.println("Escriba actADC para mostrar la lectura actual");
  Serial.println("Escriba ADC(x) siendo x un numero del 0 al 9 para mostrar la lectura cada x segundos");
  Serial.println("Escriba PWM(x) siendo x un numero del 0 al 9 para modificar el ciclo de trabajo");
}

int leePot() {
  int resul = analogRead(pinPot);
  return resul;
}

void ADCx(int t) {
  if (interruptCounter > 0) {
    portENTER_CRITICAL(&timerMux);
    interruptCounter--;
    portEXIT_CRITICAL(&timerMux);
    //Serial.println(totalInterruptCounter);
    totalInterruptCounter++;
    if (totalInterruptCounter == t) {
      valADC = leePot();
      Serial.println(valADC);
      totalInterruptCounter = 0;
    }
  }
}

void PWMx(int x) {
  int y = map(x, 0, 9, 0, 4095);
  ledcWrite(PWM1_Ch, y);
}

void funcionEscritura() {
  if (ValorEscritura == 1) {
    if (escritura.substring(4, 5).toInt() != 0) {
      ADCx(escritura.substring(4, 5).toInt());
    }
  }
  if (ValorEscritura == 2) {
    PWMx(escritura.substring(4, 5).toInt());
  }
}

void loop() {
  funcionEscritura();
  if (Serial.available() > 0) {
    escritura = Serial.readStringUntil('\n');
    Serial.println(escritura);
    if (escritura == "actADC") {
      ValorEscritura = 0;
      Serial.print("Envio de la lectura del ADC actual: ");
      valADC = leePot();
      Serial.println(valADC);
      //String valADCstring = (String)valADC;
      // Serial.write(valADCstring);
    }
    else if (escritura.substring(0, 4) == "ADC(" && escritura.substring(5, 6) == ")") {
      Serial.print("Envio de lectura cada ");
      Serial.print(escritura.substring(4, 5));
      Serial.println(" segundos");
      ValorEscritura = 1;
      totalInterruptCounter = 0;
      interruptCounter = 0;
    }
    else if (escritura.substring(0, 4) == "PWM(" && escritura.substring(5, 6) == ")") {
      Serial.print("Duty cycle: ");
      Serial.println(escritura.substring(4, 5));
      ValorEscritura = 2;
    }
    else
      ValorEscritura = 0;
  }
}
