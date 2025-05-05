#include <Servo.h>
#include "RTClib.h"

Servo meuServo;
RTC_DS3231 rtc;

bool primeiraExecucao = false;
bool segundaExecucao = false;
int ultimoDiaExecutado = -1;

void setup() {
    Serial.begin(9600);
    meuServo.attach(9);
    meuServo.write(0);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);

    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC");
        while (1);
    }

    if (rtc.lostPower()) {
        Serial.println("RTC lost power, setting time...");
        rtc.adjust(DateTime(2025, 4, 6, 9, 59, 50)); // Usa horário de compilação
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
    DateTime now = rtc.now();
    int horaAtual = now.hour();
    int minutoAtual = now.minute();
    int secondsAtual = now.second();
    int diaAtual = now.day();

    // Horários de acionamento (10:00 e 14:00)
    bool horario1 = (horaAtual == 10 && minutoAtual == 0);
    bool horario2 = (horaAtual == 10 && minutoAtual == 1);

    // Reinicia as execuções se o dia mudou
    if (diaAtual != ultimoDiaExecutado) {
        primeiraExecucao = false;
        segundaExecucao = false;
        digitalWrite(4, LOW);
        digitalWrite(4, LOW);
        ultimoDiaExecutado = diaAtual;
    }

    // Primeiro horário (10:00)
    if (horario1 && !primeiraExecucao) {
        moverServo();
        digitalWrite(4, HIGH);
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



    delay(1000); // Pequeno delay para reduzir uso da CPU
}