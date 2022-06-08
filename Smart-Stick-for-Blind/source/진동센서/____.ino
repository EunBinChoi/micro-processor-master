/*
 * Vibrate sketch
 * Vibrate for one second every minute 
 * 1분마다 1초씩 진동 발생
 *
 */
const int motorPin  = 3;  // vibration motor transistor is connected to pin 3 // 핀3에 진동모터 트랜지스터 연결
void setup()
{
  pinMode(motorPin, OUTPUT);
}
void loop()
{
  digitalWrite(motorPin,  HIGH); //vibrate                    // 진동 Start
  delay(1000); // delay one second                               // 1초동안 대기
  digitalWrite(motorPin,  LOW);   // stop vibrating      // 진동 Stop
  delay(59000); // wait 59 seconds.                             // 59초동안 대기
}


