#include <LiquidCrystal.h>
#include <AccelStepper.h>
#include <GyverTimers.h>

#define motorPin1  8
#define motorPin2  9
#define motorPin3  10
#define motorPin4  11

int val = 0;
float motorSpeed = 0;
byte left[8] = {
  B00000,
  B00010,
  B00100,
  B01000,
  B11111,
  B01000,
  B00100,
  B00010};

byte right[8] = {
  B00000,
  B01000,
  B00100,
  B00010,
  B11111,
  B00010,
  B00100,
  B01000};
LiquidCrystal lcd(13, 12, 5, 4, 3, 2);
AccelStepper stepper_motor(8, motorPin1, motorPin3, motorPin2, motorPin4);

void setup()
{
  lcd.createChar(0, left);
  lcd.createChar(1, right);
  lcd.begin(16, 2);//объявляем "рабочее пространство" lcd: 16 сегментов, 2 строки
  Timer1.setFrequency(5); // частота 3 Герца
  Timer1.enableISR(); // вкл прерывания
}

void loop()
{
  stepper();
}

ISR(TIMER1_A) {  // обработчик прерывания
  lcd.clear();
  lcd.print("Val of ADC:");
  lcd.setCursor(0, 1);
  lcd.print("Speed:");
  lcd.setCursor(7, 1);
  lcd.print(abs (motorSpeed * 4 / 1000 * 4));
  val = analogRead(A0);
  lcd.setCursor(12, 0);
  lcd.print(val);
  lcd.setCursor(11, 1);
  lcd.print("r/m");
  rotation();
}

void stepper() {
  stepper_motor.setMaxSpeed(1000);// может делать ~60000 шагов в минуту (~15 оборотов в минуту)
  val = analogRead(A0);
  motorSpeed = map(val, 0, 1023, -1000, 1000);
  stepper_motor.setSpeed(motorSpeed);
  stepper_motor.runSpeed();
}

void rotation() {
  if (motorSpeed < 0)
  {
    lcd.setCursor(15, 1);
    lcd.write((uint8_t)1);
  }
  else {
    lcd.setCursor(15, 1);
    lcd.write((uint8_t)0);
  }
}
