//PCからのリクエストに対し前回の送信からの積算距離を送る
#include <LiquidCrystal.h>

LiquidCrystal lcd(7,8,9,10,11,12);

#define DI_WHEEL_LEFT_A 14
#define DI_WHEEL_LEFT_B 15
#define DI_WHEEL_RIGHT_A 17
#define DI_WHEEL_RIGHT_B 16
#define DI_BTN_ZERO_CLEAR 18

#define DO_LED_STAT 13
#define DO_LED_ERR 12

#define SWITCH_CHECK 3


#define tire 1770 //タイヤの周径
#define patarn 72  //光学式エンコーダーの分解能（分割数）
#define speed_frequency 5 //1秒当たりの速度更新回数

#define bet_wheel 535

#define pai 3.14159265359

int cnt_left, cnt_right;
int enc_left, enc_right;
int ret;

// int tm_serial_out;
#define TIMEOUT 1000

long now_time, before_time;
double left_dist,right_dist,dist,odo;  //片輪の移動距離
double wheel_speed_left,wheel_speed_right,wheel_speed;  //片輪の速度/中心の速度
double wheel_rotation;  //回転角
double rotation_radius; //回転半径

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

  lcd.begin(8, 2);           /* LCDの設定(8文字2行) */
  lcd.clear();                /* LCDのクリア */
  lcd.setCursor(0, 0);        /* 0列0行から表示する */
  lcd.print("START"); /* 文字列の表示 */
  delay(1000);
  lcd.clear();                /* LCDのクリア */
  
  pinMode(DO_LED_STAT, OUTPUT);  
  pinMode(DO_LED_ERR, OUTPUT);  

  pinMode(DI_WHEEL_LEFT_A, INPUT);     
  pinMode(DI_WHEEL_LEFT_B, INPUT);
  pinMode(DI_WHEEL_RIGHT_A, INPUT);     
  pinMode(DI_WHEEL_RIGHT_B, INPUT);

  pinMode(SWITCH_CHECK,INPUT);

  
 // pinMode(AI_ROTARY_ENCODER_L, INPUT);
 //pinMode(AI_ROTARY_ENCODER_R, INPUT);
  
  cnt_left = cnt_right = 0;
//  tm_serial_out = 0;
  
  wheelState &= digitalRead(DI_WHEEL_LEFT_A) | digitalRead(DI_WHEEL_LEFT_B)<<1;
  prevState = wheelState;
  
  Serial.begin(9600);
}

void loop(){
  wheelState = digitalRead(DI_WHEEL_LEFT_A) | digitalRead(DI_WHEEL_LEFT_B)<<1 | digitalRead(DI_WHEEL_RIGHT_A)<<2 | digitalRead(DI_WHEEL_RIGHT_B)<<3;
  //enc_left = analogRead(AI_ROTARY_ENCODER_L);
  //enc_right = analogRead(AI_ROTARY_ENCODER_R);
  
  prevLeft = prevState&ST_WHL_LEFT;
  prevRight = prevState&ST_WHL_RIGHT;
  
  // LEFT WHEEL COUNTER UPDATE
  switch(wheelState&ST_WHL_LEFT){
    case ST_WHL_LEFT:
      if(prevLeft==ST_WHL_LA){
        //Serial.println((char)0b01);
        cnt_left++;
      }else if(prevLeft==ST_WHL_LB){
        //Serial.println((char)0b00);
        cnt_left--;
      }
      break;
    case ST_WHL_LA:
      if(prevLeft==ST_WHL_NONE){
        //Serial.println((char)0b01);
        cnt_left++;
      }else if(prevLeft==ST_WHL_LEFT){
        //Serial.println((char)0b00);
        cnt_left--;
      }
      break;
    case ST_WHL_NONE:
      if(prevLeft==ST_WHL_LB){
        //Serial.println((char)0b01);
        cnt_left++;
      }else if(prevLeft==ST_WHL_LA){
        //Serial.println((char)0b00);
        cnt_left--;
      }
      break;
    case ST_WHL_LB:
      if(prevLeft==ST_WHL_LEFT){
        //Serial.println((char)0b01);
        cnt_left++;
      }else if(prevLeft==ST_WHL_NONE){
        //Serial.println((char)0b00);
        cnt_left--;
      }
      break;
  }
  
  switch(wheelState&ST_WHL_RIGHT){
    case ST_WHL_RIGHT:
      if(prevRight==ST_WHL_RA){
        //Serial.println((char)0b11);
        cnt_right++;
      }else if(prevRight==ST_WHL_RB){
        //Serial.println((char)0b10);
        cnt_right--;
      }
      break;
    case ST_WHL_RA:
      if(prevRight==ST_WHL_NONE){
        //Serial.println((char)0b11);
        cnt_right++;
      }else if(prevRight==ST_WHL_RIGHT){
        //Serial.println((char)0b10);
        cnt_right--;
      }
      break;
    case ST_WHL_NONE:
      if(prevRight==ST_WHL_RB){
        //Serial.println((char)0b11);
        cnt_right++;
      }else if(prevRight==ST_WHL_RA){
        //Serial.println((char)0b10);
        cnt_right--;
      }
      break;
    case ST_WHL_RB:
      if(prevRight==ST_WHL_RIGHT){
        //Serial.println((char)0b11);
        cnt_right++;
      }else if(prevRight==ST_WHL_NONE){
        //Serial.println((char)0b10);
        cnt_right--;
      }
      break;
  }
  prevState=wheelState;

// ret = Serial.read();

  if(Serial.read() > 0)
  {
    Serial.write(cnt_left);
    Serial.write(cnt_right);
    cnt_left = 0;
    cnt_right = 0;
  }

if(digitalRead(SWITCH_CHECK) == HIGH){
    
    lcd.setCursor(0,0);
    lcd.print(analogRead(0));
    lcd.setCursor(4,0);
    lcd.print(analogRead(1));
    lcd.setCursor(0,1);
    lcd.print(analogRead(2));
    lcd.setCursor(4,1);
    lcd.print(analogRead(3));
    delay(200);   
    lcd.clear();
  }


  // LED
  if (wheelState& (ST_WHL_LA|ST_WHL_RA)) {     
    digitalWrite(DO_LED_STAT, HIGH);  
    //Serial.println("a");
  }else{
    digitalWrite(DO_LED_STAT, LOW);
    //Serial.println("b");
  }
}
