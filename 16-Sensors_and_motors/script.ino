//Sensores

long duration, duration_2;
long distance, distance_2;

const int minDistance = 2; // 2 cm
const int maxDistance = 13; // 20 cm

// Primeiro Sensor
const int trigPin = 11; // saida do Sinal
const int echoPin = 10;

//Segundo Sensor
const int trigPin_2 = 6;
const int echoPin_2 = 5;

// Motor
const int ENA = 3;  // PWM
const int IN1 = 2;
const int IN2 = 4;

// Estado do motor
bool irParaSensor2 = true;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin_2, OUTPUT);
  pinMode(echoPin_2, INPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  analogWrite(ENA, 125);  // Velocidade 100% (0-255)
  Serial.begin(9600);
}

void loop() {

  // Sensor 1
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  // Sensor 2
  digitalWrite(trigPin_2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin_2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_2, LOW);

  duration_2 = pulseIn(echoPin_2, HIGH);
  distance_2 = duration_2 * 0.034 / 2;



  Serial.print("Sensor 1: ");
  Serial.println(distance);
  Serial.print("Sensor 2: ");
  Serial.println(distance_2);

  bool obj1 = distance >= minDistance && distance <= maxDistance;
  bool obj2 = distance_2 >= minDistance && distance_2 <= maxDistance;

  if (irParaSensor2) {
    if (obj2) {
      pararMotor();
      delay(500); // Para 2 segundos
      //inverter direção
      irParaSensor2 = false;
    } else {
      girarParaDireita(); // ou para o sensor 2
    }
  } else {
    if (obj1) {
      pararMotor();
      delay(500);
      // inverter direção
      irParaSensor2 = true; 
    } else {
      girarParaEsquerda(); // ou para o sensor 1
    }
  }

  delay(1000);
}

// Funçoes do Motor
   
void girarParaDireita() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
}

void girarParaEsquerda() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
}

void pararMotor() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

