//This program is used on a metal bender.
//The bender has to rotate 2 full turns and a quater turn, then goes back
// half a turn to reset.
// After the second turn it has to enable an electromagnet that moves a lever
// used to set a blockage on the bender at 25% of the turn.
// The electromagnet gets released on the half turn reset.
// If not half turn reset is received after 9 seconds it will release the magnet.


#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <TimerOne.h>

LiquidCrystal_I2C lcd(0x3f, 16, 2); // set the LCD address to 0x27 or 0x20

#define POWER_ON 0
#define WAIT1 1
#define TRIG1 2
#define WAIT2 3
#define TRIG2 4
#define WAIT3 5
#define TRIG3 6

int state = POWER_ON;
int timeout = 9;
int total=0;
int twohms = 0;
int LCDFlag = 0;
char LCDLine[17];


void updateLCD(){
    sprintf(LCDLine,"T:%d   Total=%d",timeout, total);
    lcd.setCursor(0,0);
    lcd.print(LCDLine);
    lcd.setCursor(0,1);
    switch(state){
      case POWER_ON:
        lcd.print(" [ ]->[ ]->[ ]  ");
      break;
      case WAIT1:
        lcd.print(" [/]->[ ]->[ ]  ");
      break;
      case TRIG1:
        lcd.print(" [*]->[ ]->[ ]  ");
      break;
      case WAIT2:
        lcd.print(" [ ]->[/]->[ ]  ");
      break;
      case TRIG2:
        lcd.print(" [ ]->[*]->[ ]  ");
      break;
      case WAIT3:
        lcd.print(" [ ]->[ ]->[/]  ");
      break;
      case TRIG3:
        lcd.print(" [ ]->[ ]->[*]  ");
      break;
    }
}

void setup()
{
  lcd.init();                      
  lcd.init();                      
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Soylent Green   ");
  lcd.setCursor(0, 1);
  lcd.print("V1.0            ");
  pinMode(12, INPUT_PULLUP);
  pinMode(11, OUTPUT);
  pinMode(13, OUTPUT);
  delay(2000);
  Timer1.initialize(200000);  // 150 ms
  Timer1.attachInterrupt(ISR_Timeout);
  updateLCD();
}



void ISR_Timeout()
{
  if (state == TRIG2){
    twohms++;
    if (twohms>=5){
      LCDFlag=1;
      twohms=0;
      timeout--;
      if (timeout<=0){
        state = POWER_ON;
        timeout=9;
        digitalWrite(11,LOW);

      }
    }
  } else {
    twohms=0;
    timeout=9;
  }
  
}


void loop()
{
  if (LCDFlag==1){
    LCDFlag=0;
    updateLCD();
  }
  
  int pinValue = digitalRead(12);
  if (pinValue == HIGH){
    if (state == TRIG3) state=0;
    // Move state
    state++;
    updateLCD();
    if (state!=TRIG2){
      digitalWrite(11,LOW);
    } else {
      digitalWrite(11,HIGH);
    }
    while(pinValue == HIGH){
      pinValue = digitalRead(12);
    }
    state++;
    if (state == TRIG3){
      total++;
    }
    updateLCD();
    if (state!=TRIG2){
      digitalWrite(11,LOW);
    } else {
      digitalWrite(11,HIGH);
    }
  }
}
