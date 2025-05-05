int red = 9;
int yellow = 8;
int green = 7;

void setup()
{
    // put your setup code here, to run once:
    pinMode(red, OUTPUT);
    pinMode(yellow, OUTPUT);
    pinMode(green, OUTPUT);
}

void loop()
{
    // put your main code here, to run repeatedly:
    digitalWrite(green, HIGH);
    digitalWrite(red, LOW);
    digitalWrite(yellow, LOW);
    delay(15000);

    digitalWrite(green, LOW);
    digitalWrite(yellow, HIGH);
    delay(4000);

    digitalWrite(yellow, LOW);
    digitalWrite(red, HIGH);
    delay(10000);
}
