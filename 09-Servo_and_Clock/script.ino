

#include <Servo.h>
#include"RTClib.h"

Servo meuServo;
RTC_DS3231 rtc;

bool jaExecutouHoje = false; // verifica se já foi executado e que  a execução do servo só ocorra uma vez
int ultimoDiaExecutado = -1; //Armazena o ultimo dia em que o servo foi executado


void setup() {
  Serial.begin(9600); //inicia a comunicação serial
  meuServo.attach(9);
 
if (!rtc.begin()) { // verifica se o módulo rtc foi encontrado
  Serial.println("Couldn't find rtc!");
  Serial.flush();
  while(1) delay(10); // se não for encontrado, entra num loop infinito
}

if(rtc.lostPower()) { // se perder energia e desligar
  Serial.print("RTC lost power, let's set the time");
  rtc.adjust(DateTime(2025, 4, 5, 6, 0, 58, 0));
 }
}

void ServoClock() {
 DateTime agora = rtc.now(); //DateTime obtém as horas atuais do rtc e guarda na variável agora

  int horaAtual = agora.hour();
  int minutoAtual = agora.minute();
  int diaAtual = agora.day();  // para saber se já mudou o dia

  int horaAlvo = 10;
  int minutoAlvo = 0;

  // Se for a hora e minuto certos, e ainda não executou hoje
  if (horaAtual == horaAlvo && minutoAtual == minutoAlvo && !jaExecutouHoje) {
    meuServo.write(90);
    delay(1000);
    meuServo.write(0);
    delay(1000);

    meuServo.detach(); // Desliga o RTC ao pin 9

    jaExecutouHoje = true;
    ultimoDiaExecutado = diaAtual;
  }

  // Se já passou para outro dia, volta a permitir a execução
  if (diaAtual != ultimoDiaExecutado) {
    jaExecutouHoje = false;
    meuServo.attach(9);
  }

  delay(60000);

}

void loop() {
  ServoClock();
}