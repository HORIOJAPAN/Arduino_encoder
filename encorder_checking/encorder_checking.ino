//光学式エンコーダーの元のプログラム
//左右の値を積算していき表示する

#define DI_WHEEL_LEFT_A 2
#define DI_WHEEL_LEFT_B 3
#define DI_WHEEL_RIGHT_A 4
#define DI_WHEEL_RIGHT_B 5
#define DI_BTN_ZERO_CLEAR 6

#define DO_LED_STAT 13
#define DO_LED_ERR 12

#define AI_ROTARY_ENCODER_L A0
#define AI_ROTARY_ENCODER_R A1

long cnt_left, cnt_right, left_before, right_before;
int enc_left, enc_right;
// int tm_serial_out;
#define TIMEOUT 1000

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
  pinMode(DO_LED_STAT, OUTPUT);  
  pinMode(DO_LED_ERR, OUTPUT);  

  pinMode(DI_WHEEL_LEFT_A, INPUT);     
  pinMode(DI_WHEEL_LEFT_B, INPUT);
  pinMode(DI_WHEEL_RIGHT_A, INPUT);     
  pinMode(DI_WHEEL_RIGHT_B, INPUT);
  
  pinMode(AI_ROTARY_ENCODER_L, INPUT);
  pinMode(AI_ROTARY_ENCODER_R, INPUT);
  
  cnt_left = cnt_right = 0;
  left_before = right_before = 0;
//  tm_serial_out = 0;
  
  wheelState &= digitalRead(DI_WHEEL_LEFT_A) | digitalRead(DI_WHEEL_LEFT_B)<<1;
  prevState = wheelState;
  
  Serial.begin(9600);
}

void loop(){
  wheelState = digitalRead(DI_WHEEL_LEFT_A) | digitalRead(DI_WHEEL_LEFT_B)<<1 | digitalRead(DI_WHEEL_RIGHT_A)<<2 | digitalRead(DI_WHEEL_RIGHT_B)<<3;
  enc_left = analogRead(AI_ROTARY_ENCODER_L);
  enc_right = analogRead(AI_ROTARY_ENCODER_R);
  
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
    //Serial.println("a");
  }else{
    digitalWrite(DO_LED_STAT, LOW);
    //Serial.println("b");
  }
  
  if(wheelState& (ST_WHL_LB|ST_WHL_RB)){
    digitalWrite(DO_LED_ERR, HIGH);
   // Serial.println("c");
  }else{
   digitalWrite(DO_LED_ERR, LOW);
   //Serial.println("d");
  } 
  // Serial Out
//  tm_serial_out++;
//  if(tm_serial_out>TIMEOUT){
//    tm_serial_out=0;
if (cnt_left != left_before || cnt_right != right_before ){
    strData = String(cnt_left);
    strData += ",";
    strData += String(cnt_right);
//    strData += ",";
//    strData += String(enc_left);
//    strData += ",";
//    strData += String(enc_right);
    Serial.println(strData);
    left_before = cnt_left;
    right_before = cnt_right;
  }
}


