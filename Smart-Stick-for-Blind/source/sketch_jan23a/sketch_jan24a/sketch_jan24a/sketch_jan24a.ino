 */
#include "pitches.h"
int ledpin = 13;

int fire_sensor_pin = 1; // 불꽃감지
int val = 0;
int pinSpeaker = 11; // 버저

int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};


void setup() {
 pinMode(ledpin, OUTPUT);
 pinMode(fire_sensor_pin, INPUT);
 pinMode(pinSpeaker, OUTPUT);
 Serial.begin(9600);
}

void loop() {
 val = analogRead(fire_sensor_pin); // 센서값을 수신
 Serial.println(val);
 if (val >= 200) // 만약 값이 HIGH 일때(평소상태)
 {
 ///digitalWrite(ledpin, LOW); // 13번 핀(보드에 내장되어 있는 LED) 를 켭니다.
 playTone(0, 0); // 경보음 함수 호출
 //Serial.println("Beware of fire");
 //Serial.println(val);
 }  
 
 else  // 불꽃이 감지되면
 {
 playTone(333,160);
 delay(1000);
 //digitalWrite(ledpin, HIGH);
  // 경보음 함수 호출
 
 Serial.println("FIRE!!!!");
 //Serial.println(val);
 
 }
}

void playTone(long duration, int freq) { // 경보음 함수
// iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(8, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(8);
  }
}


