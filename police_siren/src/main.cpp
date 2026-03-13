// *.｡+ﾟ✧🚓Police Siren🚓✧ﾟ+｡.*

#include <BluetoothSerial.h>
BluetoothSerial SerialBT;

//Pentru gestionarea starilor
enum SirenState { STOPPED, PATTERN1, PATTERN2 };
SirenState currentState = STOPPED;


void setup()
{
    //Pinii corespunzatori ledurilor
    pinMode(18, OUTPUT);
    pinMode(22, OUTPUT);
    Serial.begin(115200);

    //Initializam Bluetooth Serial
    SerialBT.begin("esp911"); //Redenumim esp-ul dupa numele echipei

    //Setup Buzzer
    ledcSetup(0, 2000, 8); //canal 0, frecventa 2000Hz, rezolutie pe 8 biti
    ledcAttachPin(16, 0); //atasat la pinul 16, canalul 0 PWM
}

void Pattern1()
{
    //Frecventa creste treptat
    for (int freq = 600; freq <= 1200; freq += 5)
    {
        ledcWriteTone(0, freq);
        delay(10);
    }
    //Aprindem si stingem ledurile consecutiv
    digitalWrite(18, HIGH);
    delay(40);
    digitalWrite(18, LOW);
    digitalWrite(22, HIGH);
    delay(40);
    digitalWrite(22, LOW);
    //Frecventa scade treptat
    for (int freq = 1200; freq >= 600; freq -= 5)
    {
        ledcWriteTone(0, freq);
        delay(10);
    }
    //Aprindem si stingem ledurile consecutiv
    digitalWrite(18, HIGH);
    delay(40);
    digitalWrite(18, LOW);
    digitalWrite(22, HIGH);
    delay(40);
    digitalWrite(22, LOW);
}

void Pattern2()
{
    //Tonuri alternante de 800Hz si 1000Hz
    for (int i = 0; i < 3; i++) //se repeta de 3 ori
    {
        ledcWriteTone(0, 800); //800Hz
        digitalWrite(18, HIGH);
        delay(100);
        digitalWrite(18, LOW);
        delay(100);

        ledcWriteTone(0, 1000); //1000Hz
        digitalWrite(22, HIGH);
        delay(100);
        digitalWrite(22, LOW);
        delay(100);
    }
}

void loop()
{
    // Check if data is available on Bluetooth
    if (SerialBT.available()) {
        char incomingChar = SerialBT.read();

        //Valoarea introdusa este 1 => modificam starea => Pattern1 ruleaza continuu
        if (incomingChar == '1')
        {
            currentState = PATTERN1;
        }
        //Valoarea introdusa este 2 => modificam starea => Pattern2 ruleaza continuu
        if (incomingChar == '2')
        {
            currentState = PATTERN2;
        }

        //Valoarea introdusa este 0 => modificam starea => Sirena se opreste
        if (incomingChar == '0') {
            currentState = STOPPED;
            ledcWriteTone(0, 0);
            digitalWrite(18, LOW);
            digitalWrite(22, LOW);
        }
    }

    //In functie de starea setata executam codul patternului corespunzator
    if (currentState == PATTERN1)
    {
        Pattern1();
    }
    else if (currentState == PATTERN2)
    {
        Pattern2();
    }
}