void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available())
  {
// 아두이노 선이 통신과 업로드하는데 다 쓰임
// C++과 비슷
    long value1 = Serial.parseInt();
    long value2 = Serial.parseInt();

    Serial.print("value 1 : ");
    Serial.println(value1);
    
    Serial.print("value 2 : ");
    Serial.println(value2);

    Serial.print("value1 + value2 :");
    Serial.println(value1+value2);
    
  }
}
