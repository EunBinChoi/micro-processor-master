
#include "pitches.h"
float val_fire = 0.0; // 화재 센서값이 입력
float val_gas = 0.0; // 가스 센서값이 입력
float val_gas1 = 0.0; // 가스 이전 값
float val_gas2 = 0.0;// 가수 이후 값


int fire_sensor = 1; // 불꽃감지
int ledpin = 13; // led
int pinSpeaker = 11; // 버저
int echoPin = 9;
int trigPin = 8;
int vibration = 10;
int gas_sensor = 6; // 가스 센서(가스 감지시 400정도로 올라감)
// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
//int noteDurations[] = {
//  4, 8, 8, 4, 4, 4, 4, 4
//};

//int meldoy = NOTE_C4;
int noteDurations = 4;
int period = 0;
float duration, distance;

void setup() {
  pinMode(ledpin, OUTPUT);
  pinMode(fire_sensor, INPUT);
  pinMode(gas_sensor, INPUT);
  pinMode(pinSpeaker, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(vibration, OUTPUT);

  Serial.begin(9600);
}

void loop() {

  delay(300);
  val_fire = analogRead(fire_sensor); // 센서값을 수신
  val_gas = analogRead(gas_sensor);

  //Serial.println("**************FIRE**************");
  Serial.println(val_fire); // 불꽃이 보이기 시작하면 200이전으로 내려감
  //  Serial.println("**************GAS**************");
  //  Serial.println(val_gas1);
  //  Serial.println(val_gas2);
 // Serial.println(val_gas);
  //  Serial.println("*****************************");

  // Serial.println();


  /*불꽃, 가스 감지 센서*/
  if (val_fire >= 1000) // 만약 값이 HIGH 일때(평소상태)
  {
  }
  
  else {
      period = 3;
      playSound(period);
  }

  /*초음파 진동센서*/
  calUltra();
  if (distance <= 50)
  {
    digitalWrite(vibration, HIGH);
    delay(1000);
    digitalWrite(vibration, LOW);

  }
  else {}

  Serial.print(distance);
  Serial.println("cm");
  // 수정한 값을 출력
  delay(1000);
}

//void playTone(long duration, int freq) { // 경보음 함수
//  duration *= 1000;
//  int period = (1.0 / freq) * 1000000;
//  long elapsed_time = 0;
//  while (elapsed_time < duration) {
//    digitalWrite(pinSpeaker, HIGH);
//    delayMicroseconds(period / 2);
//    digitalWrite(pinSpeaker, LOW);
//    delayMicroseconds(period / 2);
//    elapsed_time += (period);
//  }
//}

void playSound(int period) {
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < period; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1200 / noteDurations;
    tone(pinSpeaker, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(pinSpeaker);

  }
}

void calUltra() {
  // 초음파를 보낸다. 다 보내면 echo가 HIGH 상태로 대기하게 된다.
  digitalWrite(trigPin, HIGH);
  delay(10);
  digitalWrite(trigPin, LOW);

  // echoPin 이 HIGH를 유지한 시간을 저장 한다.
  duration = pulseIn(echoPin, HIGH);
  // HIGH 였을 때 시간(초음파가 보냈다가 다시 들어온 시간)을 가지고 거리를 계산 한다.
  distance = ((float)(340 * duration) / 10000) / 2;

}

