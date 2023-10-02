#define led_pin 8
#define sensor_pin A0

char command = ' ';
bool shouldReadSensor = false;
bool alarmMode = false;
bool lightControlMode = false;
bool send_one_value = false;
int lightThreshold = 850; 
unsigned long previousMillis = 0;


void setup()
{
    Serial.begin(9600);
    pinMode(led_pin, OUTPUT);
}

void loop()
{
    long current_time = millis();

    // Обработка команд
if (Serial.available() > 0)
{
    command = Serial.read();
    
    if (command == 'p')
    {
        shouldReadSensor = true;
    }
    else if (command == 's')
    {
        shouldReadSensor = false;
        send_one_value = true;
    }
    else if (command == 'a')
    {
        alarmMode = true;
        lightControlMode = false;
    }
    else if (command == 'l')
    {
        alarmMode = false;
        lightControlMode = true;
    }
    else if (command == 'r')
    {
        alarmMode = false;
        lightControlMode = false;
        shouldReadSensor = false;
        digitalWrite(led_pin, LOW);
    }
    else if (command == 'n')
    {
        alarmMode = false;
        lightControlMode = false;
        digitalWrite(led_pin, HIGH);
    }
}
    if (alarmMode)
    {
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= 1000) //интервал
        {
            previousMillis = currentMillis;
            digitalWrite(led_pin, !digitalRead(led_pin));
        }
    }
    else if (lightControlMode)
    {
        int lightValue = analogRead(sensor_pin);
        if (lightValue > lightThreshold)
        {
            digitalWrite(led_pin, LOW);
        }
        else
        {
            digitalWrite(led_pin, HIGH);
        }
    }

    if (shouldReadSensor)
    {
        int val = analogRead(sensor_pin);
        Serial.print("Sensor value: ");
        Serial.println(val);
    }

    if (send_one_value)
    {
        int val = analogRead(sensor_pin);
        Serial.print("Sensor value: ");
        Serial.println(val);
        send_one_value = false;
    }
}