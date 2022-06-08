#include <LiquidCrystal.h> // LCD 쉴드, 이 쉴드를 전용으로 만든 헤더파일(LiquidCrystal.h)을 포함시켜줌
#include <Servo.h> // 서보모터를 돌기 위한 헤더파일

#include <String.h>
#include "Buzzer.h"
#include "TimerOne.h"
#include "TimerThree.h"

Servo myservo;  // create servo object to control a servo
Servo myservo2;  // create servo object to control a servo

int sensorPin = A2;    // select the input pin for the potentiometer
int sensorValue = 0;  // ADC 값
float sensorVoltage = 0.0; // 센서에 걸리는 전압
float sensorResistor = 0.0; // 센서의 저항
float sensorTemp = 0.0; // 센서에서 측정된 온도
float tempMiddle = 0.0;
float range = 30;   //10%
String str_temp = "";

LiquidCrystal lcd(8,9,4,5,6,7); 
// LCD 쉴드 객체 생성(LCD 출력에 사용할 핀들을 설정해준다)

int led = 23; // LED 출력을 담당하는 핀을 2번으로 설정(빨간불)
int led2 = 22; // 노란불

int pos1 = 0;
int pos2 = 0;
int mt_state1 = 0; //(Hot)servo 모터 상태 저장( 0: 고정 , 1 : 열림 , 2: 닫힘)
int mt_state2 = 0; //(Cool)
bool onBeep= 1;   //
bool bInput = 1;  //
bool bStart = 1;  //

#define btnRIGHT  0
#define btnUP     1 
#define btnDOWN   2 
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5   
#define btnEncodeOK  6 

const int Encoder_A =  3;            // Incremental Encoder singal A is PD3 
const int Encoder_B =  2;            // Incremental Encoder singal B is PD2 

unsigned int minTemp = 20; //10 ~ 30
unsigned int maxTemp = 40; //30 ~ 50

int state = 0;  // 0: 온도 출력 , 1: min Temp. 설정, 2: max Temp. 설정
int adc_key_in  = 0; //조이스틱 값 저장
int lcd_key     = 0;  //조이스틱의 판별된 값

int angle = 180; //servo motor의 최대 각도

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
String temp_str = "";
boolean isStart = false; //패킷 시작
char tcc ; //모드 저장
int read_LCD_buttons()  //조이스틱의 값을 읽어서 판별된 정보를 반환하는 함수
{  
   adc_key_in = analogRead(0); //조이스틱이 A0번에 연결되어 있음      

   if (adc_key_in > 1000) return btnNONE; 
   if (adc_key_in < 50)   return btnLEFT;   
   if (adc_key_in < 150)  return btnUP;   
   if (adc_key_in < 250)  return btnRIGHT;   
   if (adc_key_in < 450)  return btnSELECT;   
   if (adc_key_in < 700)  return btnDOWN;     
   if (adc_key_in < 850)  return btnEncodeOK;

   return btnNONE;  // when all others fail, return this... 
}   

void Encoder_san() //엔코더 값을 읽는 함수
{     
   if(digitalRead(Encoder_B)) //1이면 시계방향
   {
      if(state == 1){       //state가 1이면 minTemp설정
         minTemp++;  //10 ~ 30
         if(minTemp > 30) minTemp = 30; 
      }else if(state == 2){ //state가 2이면 maxTemp 설정
         maxTemp++;  //30 ~ 50
         if(maxTemp > 50) maxTemp = 50;
      }
   }
   else                       //0이면 반시계 방향
   {  
      if(state == 1){
         minTemp--;
         if(minTemp < 10) minTemp = 10; 
      }else if(state == 2){
         maxTemp--;
         if(maxTemp < 30) maxTemp = 30;
      }
   }     
}

/*경고음이 반복해서 들리지 않고 설정한 온도보다 높아지는 순간에만 울리기 위해 설정한 변수이다. 만약 onBeep이 1이면 경고음을 내고, 0이면 경고음을 내지않기 위한 것을 설정해주는 변수*/
void setup() {
   myservo.attach(27);  // attaches the servo on pin 11 to the servo object
   myservo2.attach(26);  // attaches the  servo on pin 11 to the servo object
   lcd.begin(16, 2);// LCD의 cols의 길이가 16, rows의 길이가 2
   pinMode(10, OUTPUT); // 10번 핀으로 LCD 출력을 담당
   digitalWrite(10, HIGH); // 10번 핀(pin)에 HIGH(5V)를 줄지, LOW(0V)를 줄지 입력
   // lcd에 함수의 인수 안에 든 문자열을 출력
   pinMode(led, OUTPUT);  // 3번 핀으로 LED 출력을 담당
   pinMode(led2, OUTPUT);  // 3번 핀으로 LED 출력을 담당
   digitalWrite(led,HIGH);   //sink방식으로 연결되어 있기 때문에 High로 설정
   digitalWrite(led2,HIGH);  // 5v - LED - Arduino(digital)            

   Serial2.begin(115200);   //mainup - buzzerup
   Serial1.begin(9600);     //smart phone - mainup
   
   myservo.write(0);  //초기 각도를 0도로 설정
   myservo2.write(0);

   Timer1.initialize(20000); //20000us -> 20ms -> 50Hz
   Timer1.attachInterrupt(callback1); 
   // 0.02초

   Timer3.initialize(500000); //us -> 0.5초
   Timer3.attachInterrupt(callback3); 
  // 현재하던 일을 중단시키고 callback 함수 실행
  
   
   pinMode(Encoder_A, INPUT); 
   pinMode(Encoder_B, INPUT); 
   digitalWrite(Encoder_A, 1);
   digitalWrite(Encoder_B, 1);
   //========================================
   attachInterrupt(1, Encoder_san, FALLING);      
   // 회전하면서 값을 설정하는 순간 Encoder_san 함수를 불러 min 10~30 // max 20~40 사이의 숫자로 설정하도록 한다
  // 회전할 때 Encoder_san
  
   inputString.reserve(200); // 통신용 데이터저장공간 200byte
}
void callback1(){    // 서브모터
   myservo.write(pos1);              // tell servo to go to  position in variable 'pos']
   myservo2.write(pos2);     
   if(mt_state1 == 1){ //여는거         
      if(pos1 < angle) pos1++; 
   }else if(mt_state1 == 2){ //잠구는거
      if(pos1 > 0) pos1--; 
   }
   if(mt_state2 == 1){
      if(pos2 < angle) pos2++; 
   }else if(mt_state2 == 2){ //잠구는거
      if(pos2 > 0) pos2--; 
   }
}
void callback3(){ // 온도 값 읽어들임
   TempHumiSensorRead();
}
void TempHumiSensorRead(){ 
   static float tmp = 0; // 이전 온도를 저장해주는 변수

   sensorValue = analogRead(sensorPin);
   sensorVoltage = 5.0*sensorValue / 1023.0;
   sensorResistor = 10.4*sensorVoltage / (5-sensorVoltage);
   sensorTemp = (706.6 * (pow((sensorResistor*1000),-0.1541))-146);

   if(bStart){ //처음 시작할때, tmp와 sensorTemp를 같게 시작
      tmp = sensorTemp; // 이전온도 저장
      bStart = 0; // 처음에만 이전온도를 sensorTemp로 설정
      
     }

   if(sensorTemp < -10 ){ //값이 갑자기 급변하는경우 (노이즈) 이전 값으로 대체
      sensorTemp = tmp; // 이전값으로 대체
   } 
   
   tempMiddle = (maxTemp - minTemp)/2.0; // 중간값이 몇인지 저장
   if(state == 1){ // minTemp설정 화면
      lcd.setCursor(0,0); // LCD에 커서를 (0,0)으로 설정
      lcd.clear();
      lcd.print("Input min Temp.:"); // 그 부분에 “Temperature High!”를 출력
      lcd.setCursor(0,1);
      lcd.print(minTemp); 
   }
   else if(state == 2) // maxTemp설정 화면
   {
      lcd.setCursor(0,0); // LCD에 커서를 (0,0)으로 설정
      lcd.clear();
      lcd.print("Input max Temp.:"); // 그 부분에 “Temperature High!”를 출력
      lcd.setCursor(0,1);
      lcd.print(maxTemp); 
   }
   
   else{ // 온도 출력 화면
      if((sensorTemp > maxTemp) && (mt_state1%2 == 0) && (pos1 == 0)){ // 만약 온도 값이 40도가 넘어가면
         digitalWrite(led, LOW); // LED에 HIGH(5V)를 줘서 빨간불이 들어올 수 있게 함
         lcd.setCursor(0,0); // LCD에 커서를 (0,0)으로 설정
         lcd.clear();
         lcd.print("High!!"); // 그 부분에 “Temperature High!”를 출력
         lcd.setCursor(0,1); // LCD에 커서를 (0,1)으로 설정
         lcd.print(sensorTemp); // 변수 g_fTempC의 값을 lcd에 출력해줌
         if(onBeep)   {  //1번만 소리를 재생시킴
            Serial2.print("b1$");
            onBeep = 0;
         }
         if(pos1 == 0) mt_state1 = 1;  //닫혀있을 때만 개방(움직이는 도중에는 구동이 안되게)
      }
      else if((sensorTemp < minTemp) && (mt_state2%2 == 0) && (pos2 == 0)){
         digitalWrite(led2, LOW); // LED에 HIGH(5V)를 줘서 빨간불이 들어올 수 있게 함
         lcd.setCursor(0,0); // LCD에 커서를 (0,0)으로 설정
         lcd.clear();
         lcd.print("Low!!"); // 그 부분에 “Temperature High!”를 출력
         lcd.setCursor(0,1); // LCD에 커서를 (0,1)으로 설정
         lcd.print(sensorTemp); // 변수 g_fTempC의 값을 lcd에 출력해줌
         if(onBeep)   {
            Serial2.print("b2$"); // 그냥 숫자만 보내면 buzzer up에서 fan인지 버저인지 구분해주기 위해서
            // 앞에 문자를 붙임
            // buzzer 의 2단계 + $(마지막의미)
            
            onBeep = 0;
         }
         if(pos2 == 0) mt_state2 = 1;
      }  
      else{
//여기까지
         digitalWrite(led, HIGH); // LED이 출력되지 않게 함
         digitalWrite(led2, HIGH); // LED이 출력되지 않게 함
         lcd.setCursor(0,0); // LCD에 커서를 (0,0)으로 설정
         lcd.clear();
         lcd.print("Temperature : "); // 그 부분에 "Normal Temperature"를 출력
         lcd.setCursor(0,1);// LCD에 커서를 (0,1)으로 설정
         lcd.print(sensorTemp); // 변수 g_fTempC의 값을 lcd에 출력해줌         
         Serial2.print("b0$");
         onBeep = 1; //
         if((sensorTemp <= (minTemp + tempMiddle + tempMiddle*(100.0-range)/100.0)) && mt_state1 == 1 && pos1 >= angle){ //열려 있는 상태에서 끝까지 열었으면, 닫음 
            mt_state1 = 2; //닫아라 // 뜨거운 -> 차가운
         //min = 20 ,middle = 5. max = 10;
         }
         if((sensorTemp >= (minTemp + tempMiddle - tempMiddle*(100.0-range)/100.0)) && mt_state2 == 1 && pos2 >= angle){ //열려 있는 상태에서 끝까지 열었으면, 닫음 
            mt_state2 = 2;
         }
      }
      str_temp = String(sensorTemp);
      Serial1.println("c"+str_temp);
      tmp = sensorTemp;  //이전 값 저장
   }
}


void serial1Event() {// smartphone에서 mainup로 들어오는 정보를 처리해주는 함수
   while (Serial1.available()) { // Serial1(smartphone - mainup)에 데이터가 있으면
      char inChar = (char)Serial1.read(); // 그 데이터를 한바이트씩 읽어들임(char로 형변환)
      if((inChar == 'f' ||inChar == 'v'||inChar == 'm'||inChar == 'M') && !isStart){
        isStart = true;
         tcc = inChar; // 모드 저장
      }
      else if(isStart && inChar != '$'){
         inputString += inChar; // 그 다음부터는 숫자값->이것으로 inpusString 값 저장
      }
      if (isStart && inChar == '$') {
         stringComplete = true;
      }      
   }
}
void loop() {
   serial1Event();
   if (stringComplete) { //문자열이 끝났으면(엔터까지)
      switch(tcc){
      case 'm': minTemp = inputString.toInt(); break;
      case 'M': maxTemp = inputString.toInt(); break;
      case 'f': Serial2.print("f"+inputString+"$"); break;
      case 'v': Serial2.print("v"+inputString+"$"); break;
      // fan과 vibration은 버저마프에 있기떄문에 버저마프로 전달      
     }
      inputString = "";
      stringComplete = false;
      isStart = false;
   }

   lcd_key = read_LCD_buttons();  //조이스틱에서 값을 읽음
   switch (lcd_key)               // depending on which button was pushed, we perform an action  
   {    
   case btnRIGHT:  
     break;    
   case btnLEFT:     
      break;  
   case btnUP: 
        break; 
   case btnDOWN:     
       break; 
   case btnSELECT:    
      {    
         if(bInput ){   // 눌렀다 땠을 때를 판별하기위한 변수 
            state++; //state를 다음으로 변경
            state %= 3;   //0, 1, 2만 반복하기 위해
            bInput = 0;  //state의 중복 증가를 방지하기 위하여 bInput을 0으로 저장
            // 쭉 누르고 있을 때 계속 state가 바뀌기 때문에 그것을 방지해주기 위한것
         }
         break;    
      } 
   case btnEncodeOK:   
        break;  
   case btnNONE:   
      {     
         bInput = 1;  //땠을때, bInput을 1로 해줌으로써, 새로운 state 입력을 받을 수 있게 함
         break;     
      } 

   } 
   delay(20); 
}
