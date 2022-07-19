#include <Servo.h>
#include <LiquidCrystal.h>
#define minDelay 6
#define maxDelay 250

Servo servo; 
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

uint8_t temp_cel[8] =
{
B11100,
B10100,
B11100,
B00000,
B00000,
B00000,
B00000
}; // strelochka

float speed;
int pos = 0;    
int val = 0;
int del, volt, motorPosition;
bool clockwise; 

void setup() {
  volt = analogRead(A0);
  Delay_Val();
  Speed_Val();
  motorPosition = 0;
  clockwise = false;
  
  lcd.createChar(1, temp_cel);
  lcd.begin(16, 2);
  outputSpeed();
  lcd.setCursor(0, 1);
  lcd.print("sec/60 deg");
  lcd.setCursor(10, 1);
  lcd.write((uint8_t)1);
  
  servo.attach(13);
  
  noInterrupts();
  
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2 = 130;
  TCCR2B |= (1 << CS20)|(1 << CS22); 
  TIMSK2 |= (1 << TOIE2);
  
  interrupts();
  
  pinMode(A0, INPUT);
  servo.attach(13);  
}

void loop() {
  rotation();
  delay(del);
}

ISR(TIMER2_OVF_vect)
{ 
  TCNT2 = 130;
  if(VoltChanged())
  {
    Delay_Val();
    Speed_Val();
    outputSpeed();
    lcd.setCursor(0, 1);
    lcd.print("sec/60 deg");
    lcd.setCursor(10, 1);
    lcd.write((uint8_t)1);
  }
}
  
bool VoltChanged(){
  int currentVolt = analogRead(A0);
  if(currentVolt == volt) return false;
  volt = currentVolt;
  return true;
}

void Delay_Val(){
  del = map(volt, 0, 1023, maxDelay, minDelay);
}

void Speed_Val(){
  speed = (float)60*del/1000;
}

void outputSpeed(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Speed:");
  lcd.setCursor(7, 0);
  lcd.print(speed);
}

void rotation(){
  if(clockwise){
    servo.write(--motorPosition);
    if(motorPosition == 0) clockwise = false;
  }
  else {
    servo.write(++motorPosition);
    if(motorPosition == 180) clockwise = true;
  }
}
