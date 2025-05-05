#include <Wire.h>
#include <Servo.h>
#include "RTClib.h"

#define SERIAL Serial

#define Nivel_abaixo 40
#define Nivel_acima 50
#define THRESHOLD 90

#define ATTINY1_HIGH_ADDR 0x78
#define ATTINY2_LOW_ADDR 0x77
#define NUM_READINGS 3
#define MAX_SECTIONS 20



Servo meuServo;
RTC_DS3231 rtc;

unsigned char low_data[8] = {0};
unsigned char high_data[12] = {0};
float readings[NUM_READINGS] = {0};  
int relePin = 7; //Pino digital do Arduino que vaicontrolar o relé
int readIndex = 0;  
float total = 0;  
float average = 0;  


bool motorLigado = false;
bool primeiraExecucao = false;
bool segundaExecucao =false;
int ultimoDiaExecutado = -1;


void getHigh12SectionValue() {
    memset(high_data, 0, sizeof(high_data));
    Wire.requestFrom(ATTINY1_HIGH_ADDR, 12); //Pede os dados do dispositico (endereço, quantidade)
    while (Wire.available() < 12);

    for (int i = 0; i < 12; i++) {
        high_data[i] = Wire.read();
    }
    delay(10);
}

// Função para os 8 valores mais baixos

void getLow8SectionValue() {
    memset(low_data, 0, sizeof(low_data));
    Wire.requestFrom(ATTINY2_LOW_ADDR, 8);
    while (Wire.available() < 8);

    for (int i = 0; i < 8; i++) {
        low_data[i] = Wire.read(); 
    }
    delay(10);
}

void check() {
    int trig_section = 0;
    
    getLow8SectionValue();
    getHigh12SectionValue();

    // Contar secções ativas em vez de OR bitwise
    for (int i = 0; i < 8; i++) {
        if (low_data[i] > THRESHOLD) {
            trig_section++;
        }
    }
    for (int i = 0; i < 12; i++) {
        if (high_data[i] > THRESHOLD) {
            trig_section++;
        }
    }

    // Limitar ao máximo de secções disponíveis
    if (trig_section > MAX_SECTIONS) {
        trig_section = MAX_SECTIONS;
    }

    // Ajustar a percentagem com calibração dinâmica
    float water_level = (trig_section / (float)MAX_SECTIONS) * 100;  

    // Média móvel para suavizar leitura
    total = total - readings[readIndex];  
    readings[readIndex] = water_level;  
    total = total + readings[readIndex];  

    readIndex++;
    if (readIndex >= NUM_READINGS) readIndex = 0;  

    average = total / NUM_READINGS;  

    // Exibir resultados
    SERIAL.print("Water level (média): ");
    SERIAL.print(average, 1);
    SERIAL.println("%");
    
    SERIAL.println("*********************************************************");
}

void startMotor() {
  
  check();
  if (!motorLigado && average <= Nivel_abaixo && average > 0) {
    digitalWrite(relePin, HIGH);
    motorLigado = true;
    Serial.println("Motor Ligado");
  } else if (motorLigado && (average >= Nivel_acima || average == 0)) {
    digitalWrite(relePin, LOW);
    motorLigado = false;
    Serial.println("Motor Desligado");
  }
}

void setup() {
    SERIAL.begin(9600);
    //O sensor começa a ler
    Wire.begin();
    //Configura o pino do relé como saida
    pinMode(relePin, OUTPUT);
    digitalWrite(relePin, LOW); // Certifica-se que o relé começa desligado
    meuServo.attach(9);
    meuServo.write(0);
    pinMode(3, OUTPUT);
    pinMode(5, OUTPUT);
 
 
    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC");
        while (1);
    }

    if (rtc.lostPower()) {
        Serial.println("RTC lost power, setting time...");
        rtc.adjust(DateTime(2025, 4, 6, 14, 39, 0)); // Usa horário de compilação
    }
   

}

void moverServo() {
    if (!meuServo.attached()) {
        meuServo.attach(9);
    }
    meuServo.write(90);
    delay(500);
    meuServo.write(0);
    delay(500);
    meuServo.write(90);
    delay(500);
    meuServo.write(0);
    delay(500);
    meuServo.detach();
}

void loop() {
    startMotor();
    DateTime now = rtc.now();
    int horaAtual = now.hour();
    int minutoAtual = now.minute();
    int secondsAtual = now.second();
    int diaAtual = now.day();

    // Horários de acionamento (10:00 e 21:00)
    bool horario1 = (horaAtual == 10 && minutoAtual == 0);
    bool horario2 = (horaAtual == 21 && minutoAtual == 0);

    // Reinicia as execuções se o dia mudou
    if (diaAtual != ultimoDiaExecutado) {
        primeiraExecucao = false;
        segundaExecucao = false;
        digitalWrite(3, LOW);
        digitalWrite(5, LOW);
        ultimoDiaExecutado = diaAtual;
    }

    // Primeiro horário (10:00)
    if (horario1 && !primeiraExecucao) {
        moverServo();
        digitalWrite(3, HIGH);
        primeiraExecucao = true;
        ultimoDiaExecutado = diaAtual;
        Serial.println("Servo acionado às 10:00");
    }

    // Segundo horário (14:00)
    if (horario2 && !segundaExecucao) {
        moverServo();
        digitalWrite(5, HIGH);
        segundaExecucao = true;
        ultimoDiaExecutado = diaAtual;
        Serial.println("Servo acionado às 14:00");
    }

   Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
    delay(500);

    
}

