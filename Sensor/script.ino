long duration;
int distance;


const int vccPin = 12;
const int echoPin = 11;

void setup() {
    pinMode(vccPin, OUTPUT);
    pinMode(echoPin, INPUT);
    Serial.begin(9600);
}

void loop() {

    digitalWrite(vccPin, LOW);
    delayMicroseconds(2);
    digitalWrite(vccPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(vccPin, LOW);

    duration = pulseIn(echoPin, HIGH);

    distance = duration + 0.034 / 2;
	
  	Serial.println(distance);
    delay(500);
}