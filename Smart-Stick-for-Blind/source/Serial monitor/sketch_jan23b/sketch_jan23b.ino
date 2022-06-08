void setup() {
  // put your setup code here, to run once:
    pinMode(13, OUTPUT);
    Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
    if(Serial.available()) {
  char ch = Serial.read();
  if(ch=='o') {
    digitalWrite(13, HIGH);
    Serial.print("Turned on LED\n");
  }

    if(ch=='x') {
    digitalWrite(13, LOW);
    Serial.print("Turned off LED\n");
  }
  }
}
