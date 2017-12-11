
int FINGER1 = 4;
int FINGER2 = 5;
int FINGER3 = 6;
int FINGER4 = 7;

int BASE = 2;
void setup() {
  // put your setup code here, to run once:
    Serial.begin(38400);
    pinMode(FINGER1, INPUT);
    digitalWrite(FINGER1, HIGH);
    pinMode(FINGER2, INPUT);
    digitalWrite(FINGER2, HIGH);
    pinMode(FINGER3, INPUT);
    digitalWrite(FINGER3, HIGH);
    pinMode(FINGER4, INPUT);
    digitalWrite(FINGER4, HIGH);
    pinMode(BASE, OUTPUT);
    digitalWrite(BASE, LOW);


}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(digitalRead(FINGER1)*(-1)+1);
  Serial.print(digitalRead(FINGER2)*(-1)+1);
  Serial.print(digitalRead(FINGER3)*(-1)+1);
  Serial.print(digitalRead(FINGER4)*(-1)+1);
  Serial.print("\n");
  delay(11);
}
