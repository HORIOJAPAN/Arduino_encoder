//速度計測プログラム
//wheel_speedにタイヤの速度（km/h）で記録
//表示についてはまだ未設定

#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

#define DI_WHEEL_LEFT_A 2
#define DI_WHEEL_LEFT_B 3
#define DI_WHEEL_RIGHT_A 4
#define DI_WHEEL_RIGHT_B 5
#define DI_BTN_ZERO_CLEAR 6

#define DO_LED_STAT 13
#define DO_LED_ERR 12

#define AI_ROTARY_ENCODER_L A0
#define AI_ROTARY_ENCODER_R A1

#define tire 1770 //タイヤの周径
#define patarn 72   //光学式エンコーダーの分解能（分割数）
#define speed_frequency 10  //1秒当たりの速度更新回数


String strData1;
String strData2;

long cnt_left, cnt_right;

//int enc_left, enc_right;
// int tm_serial_out;


long now_time, before_time;
double wheel_speed_left,wheel_speed_right;


String strData;

int wheelState, prevState, prevLeft, prevRight;
#define ST_WHL_NONE 0x0
#define ST_WHL_LA 0x01
#define ST_WHL_LB 0x02
#define ST_WHL_LEFT 0x03
#define ST_WHL_RA 0x04
#define ST_WHL_RB 0x08
#define ST_WHL_RIGHT 0x0c


void setup() {

  lcd.begin(8, 2);           /* LCDの設定(16文字2行) */
  lcd.clear();                /* LCDのクリア */
  lcd.setCursor(0, 0);        /* 0列0行から表示する */
  lcd.print("CHECK"); /* 文字列の表示 */
  delay(1000);
  lcd.clear();                /* LCDのクリア */
  
  pinMode(DO_LED_STAT, OUTPUT);  
  pinMode(DO_LED_ERR, OUTPUT);  

  pinMode(DI_WHEEL_LEFT_A, INPUT);     
  pinMode(DI_WHEEL_LEFT_B, INPUT);
  pinMode(DI_WHEEL_RIGHT_A, INPUT);     
  pinMode(DI_WHEEL_RIGHT_B, INPUT);
  
  pinMode(AI_ROTARY_ENCODER_L, INPUT);
  pinMode(AI_ROTARY_ENCODER_R, INPUT);
  
  cnt_left = cnt_right = 0;
//  tm_serial_out = 0;

  now_time = before_time = 0;
  
  wheelState &= digitalRead(DI_WHEEL_LEFT_A) | digitalRead(DI_WHEEL_LEFT_B)<<1;
  prevState = wheelState;
  
  Serial.begin(9600);
}

void loop(){
  wheelState = digitalRead(DI_WHEEL_LEFT_A) | digitalRead(DI_WHEEL_LEFT_B)<<1 | digitalRead(DI_WHEEL_RIGHT_A)<<2 | digitalRead(DI_WHEEL_RIGHT_B)<<3;
//  enc_left = analogRead(AI_ROTARY_ENCODER_L);
//  enc_right = analogRead(AI_ROTARY_ENCODER_R);
  now_time=millis();
  
  prevLeft = prevState&ST_WHL_LEFT;
  prevRight = prevState&ST_WHL_RIGHT;
  
  // LEFT WHEEL COUNTER UPDATE
  switch(wheelState&ST_WHL_LEFT){
    case ST_WHL_LEFT:
      if(prevLeft==ST_WHL_LA){
        cnt_left++;
      }else if(prevLeft==ST_WHL_LB){
        cnt_left--;
      }
      break;
    case ST_WHL_LA:
      if(prevLeft==ST_WHL_NONE){
        cnt_left++;
      }else if(prevLeft==ST_WHL_LEFT){
        cnt_left--;
      }
      break;
    case ST_WHL_NONE:
      if(prevLeft==ST_WHL_LB){
        cnt_left++;
      }else if(prevLeft==ST_WHL_LA){
        cnt_left--;
      }
      break;
    case ST_WHL_LB:
      if(prevLeft==ST_WHL_LEFT){
        cnt_left++;
      }else if(prevLeft==ST_WHL_NONE){
        cnt_left--;
      }
      break;
  }
  
  switch(wheelState&ST_WHL_RIGHT){
    case ST_WHL_RIGHT:
      if(prevRight==ST_WHL_RA){
        cnt_right++;
      }else if(prevRight==ST_WHL_RB){
        cnt_right--;
      }
      break;
    case ST_WHL_RA:
      if(prevRight==ST_WHL_NONE){
        cnt_right++;
      }else if(prevRight==ST_WHL_RIGHT){
        cnt_right--;
      }
      break;
    case ST_WHL_NONE:
      if(prevRight==ST_WHL_RB){
        cnt_right++;
      }else if(prevRight==ST_WHL_RA){
        cnt_right--;
      }
      break;
    case ST_WHL_RB:
      if(prevRight==ST_WHL_RIGHT){
        cnt_right++;
      }else if(prevRight==ST_WHL_NONE){
        cnt_right--;
      }
      break;
  }
  prevState=wheelState;

  // LED
  if (wheelState& (ST_WHL_LA|ST_WHL_RA)) {     
    digitalWrite(DO_LED_STAT, HIGH);  
  }else{
    digitalWrite(DO_LED_STAT, LOW);
  }
  
  if(wheelState& (ST_WHL_LB|ST_WHL_RB)){
    digitalWrite(DO_LED_ERR, HIGH);
  }else{
   digitalWrite(DO_LED_ERR, LOW);
 } 

//速度計算・出力
  if((now_time - before_time) >= (1000/speed_frequency))
  {
    wheel_speed_left = (((cnt_left/patarn) * tire)/((now_time - before_time)/1000)) * 3600 / 1000000;
    wheel_speed_right = (((cnt_left/patarn) * tire)/((now_time - before_time)/1000)) * 3600 / 1000000;

    strData1 = String("L:");
    strData1 += String(wheel_speed_left,1);
    strData1 += String(":");
    strData1 += String(cnt_left);
    strData2 = String("R:");
    strData2 += String(wheel_speed_right,1);
    strData2 += String(":");
    strData2 += String(cnt_right);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(strData1);
    lcd.setCursor(0,1);
    lcd.print(strData2);
    //Serial.println(wheel_speed,2);
    before_time = now_time;
    cnt_left = 0;
    cnt_right = 0;  
  }
}
  


