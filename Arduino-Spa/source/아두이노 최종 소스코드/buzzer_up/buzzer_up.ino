
#include "Buzzer.h"
int melody[] = {NOTE_F5}; 
int melody2[] = {NOTE_C5}; 
//// 음계를 저장한 배열 선언
int noteDurations[] = {1}; // 음계가 울리는 시간을 저장하는 배열 선언
int noteDurations2[] = {1};
int temp_state = 0;
unsigned int g_unNoteDuration; // 실질적인 시간을 계산해 저장하는 변수 선언
int OnBeep = 1;
int fan = 44;
int vibration = 41;
int fanSpeed = 0;
int vibration_State = 0;

String inputString = "";
boolean stringComplete = false;
String temp_str = "";
char tcc ;
bool isStart = false ;
void calculateDuration(unsigned int duration) // 실질적인 시간을 계산하는 함수
{
   g_unNoteDuration = 1000/noteDurations[duration];
   /*이렇게 하는 이유는 보통 4분음표, 8분음표가 있는데, 4분 음표는 1/4만큼 소리낸다는 뜻이다. 만약 noteDurations[duration]가 4이면 1000/4 = 250(ms)는 4분 음표에 해당하는 만큼 소리를 낸다는 뜻이므로, 위와 같이 작성하는 것이다*/
}


void calculateDuration2(unsigned int duration) // 실질적인 시간을 계산하는 함수
{
   g_unNoteDuration = 1000/noteDurations2[duration];
   /*이렇게 하는 이유는 보통 4분음표, 8분음표가 있는데, 4분 음표는 1/4만큼 소리낸다는 뜻이다. 만약 noteDurations[duration]가 4이면 1000/4 = 250(ms)는 4분 음표에 해당하는 만큼 소리를 낸다는 뜻이므로, 위와 같이 작성하는 것이다*/

}
void soundNote (unsigned int note) // Buzzer를 울리게 하는 함수
{
   BuzzerContol(melody[note],g_unNoteDuration);
   delay(1000);
   BuzzerStop();
}

void soundNote2 (unsigned int note) // Buzzer를 울리게 하는 함수
{
   BuzzerContol(melody2[note],g_unNoteDuration);
   delay(1000);
   BuzzerStop();
}
void setup() {
   Serial2.begin(115200);   //up - up
 
   pinMode(vibration,OUTPUT);
   pinMode(fan,OUTPUT); // 기본값이 0
   digitalWrite(vibration,HIGH);
   //  바이브레이션 LOW->작동
   analogWrite(fan,0);
}
void serial2Event() {
   while (Serial2.available()) {

      char inChar = (char)Serial2.read();
      
      if((inChar == 'f' ||inChar == 'v'||inChar == 'b') && !isStart){
         // 어디가 시작인지 확인해주기 위해여 isStart 변수를 true로 설정
         // isStartrㅏ true가 되면 이제 시작임을 알림
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
   serial2Event();
   if (stringComplete) {
      switch(tcc){
        // fanSpeed 0~255
      case 'f': fanSpeed = inputString.toInt(); break;
      case 'v': vibration_State = inputString.toInt(); break;
      case 'b': temp_state = inputString.toInt(); if(temp_state == 0) OnBeep = 1; break;
      }
      inputString = "";
      stringComplete = false;
      isStart = false;
   }
   digitalWrite(vibration, !vibration_State);
   // 0->커지고 1이면 꺼진다(앱에서는 1이켜지고 0이 꺼짐이기때문에 !(반전)연산)
   analogWrite(fan,fanSpeed);

   if(temp_state == 1) // high 상태이면
   {
      int thisNote = 0;  // 
      if(OnBeep){
         OnBeep = 0;
         /* Buzzer를 설정 온도보다 높아지면 한번 울리게 하기 위해 만약 이 조건문이 실행되면 onBeep을 0으로 줘서 다음부터는 실행되지 않게 한다 */
         for(thisNote =  0; thisNote < 1; thisNote++){ // 반복문을 돌면서
            calculateDuration(thisNote); 
            // 음계가 울리는 시간을 계산하는 함수를 호출하여 실질적인 시간 설정 
            soundNote(thisNote);
            // 저장된 음계를 해당되는 시간만큼 경고음을 울리게 함 
         }
      }    
      temp_state = 0;
   }else if(temp_state == 2){
      int thisNote = 0; 
      //음계를 저장한 배열의 값을 읽어오기 위한 변수 선언(반복문을 돌기 위해 필요)
      /* Buzzer를 설정 온도보다 높아지면 한번 울리게 하기 위해 만약 이 조건문이 실행되면 onBeep을 0으로 줘서 다음부터는 실행되지 않게 한다 */
      if(OnBeep){
         OnBeep = 0;
         for(thisNote =  0; thisNote < 1; thisNote++){ // 반복문을 돌면서
            calculateDuration2(thisNote); 
            // 음계가 울리는 시간을 계산하는 함수를 호출하여 실질적인 시간 설정 
            soundNote2(thisNote);
            // 저장된 음계를 해당되는 시간만큼 경고음을 울리게 함 
         }
      }   
      temp_state = 0;
   }
}
