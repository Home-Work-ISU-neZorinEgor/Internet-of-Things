int led1 = 7;
int led2 = 8;
int led3 = 11;
int led4 = 12;

void setup(){
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
}

void loop(){

        digitalWrite(led1, LOW);//4
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, LOW);
    delay(5); // Задержка в 5 миллисекунд

        digitalWrite(led1, LOW); //3
    digitalWrite(led2, HIGH);
    digitalWrite(led3, LOW);
    digitalWrite(led4, HIGH);
    delay(5); // Задержка в 5 миллисекунд

        digitalWrite(led1, HIGH); //1
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, HIGH);
    delay(5); // Задержка в 5 миллисекунд




}