#include <Servo.h>
#include "RTClib.h"

Servo meuServo;
RTC_DS3231 rtc;

bool jaExecutouHoje = false;
int ultimoDiaExecutado = -1;

void setup() {
  Serial.begin(9600);
  meuServo.attach(9);
  meuServo.write(0);

   if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    // Adjust the time for your timezone (e.g. UTC+1)
    rtc.adjust(DateTime(2025, 4, 6, 9, 59, 50)); //set hour
  }
  
}

void loop() {
  DateTime now = rtc.now();


  // Data e hora atual (simulado para teste)
  int horaAtual = now.hour();
  int minutoAtual = now.minute();
  int diaAtual = now.day();  // para saber se já mudou o dia

  int horaAlvo = 10;
  int minutoAlvo = 0;

  // Se for a hora e minuto certos, e ainda não executou hoje
  if (horaAtual == horaAlvo && minutoAtual == minutoAlvo && !jaExecutouHoje) {
    meuServo.write(90);     // Mover para 90°
    delay(500);            // Esperar 1 segundo
    meuServo.write(0);      // Mover de volta para 0°
    delay(500);            // Esperar 1 segundo para o servo descansar

    meuServo.detach();
    
    jaExecutouHoje = true;
    ultimoDiaExecutado = diaAtual;
  }

  // Se já passou para outro dia, volta a permitir a execução
  if (diaAtual != ultimoDiaExecutado) {
    jaExecutouHoje = false;
    meuServo.attach(9);
  }

   Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  delay(1000); // Espera 1 segundo antes de verificar novamente
}