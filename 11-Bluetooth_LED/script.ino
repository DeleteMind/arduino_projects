#include <SoftwareSerial.h>

// Configuração do módulo HC-06
#define BT_RX 0  // Pino RX do Arduino conectado ao TX do HC-06
#define BT_TX 1  // Pino TX do Arduino conectado ao RX do HC-06
#define LED_PIN 4  // Pino do LED

SoftwareSerial bluetooth(BT_RX, BT_TX);  // RX, TX

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);  // Inicia com LED desligado
  
  // Inicia a comunicação serial com o computador (para debug)
  Serial.begin(9600);
  Serial.println("Sistema iniciado!");
  
  // Inicia a comunicação serial com o módulo Bluetooth
  bluetooth.begin(9600);
  bluetooth.println("Conectado ao Arduino! Envie '1' para ligar o LED ou '0' para desligar.");
}

void loop() {
  // Verifica se há dados disponíveis no Bluetooth
  if (bluetooth.available()) {
    char command = bluetooth.read();
    
    // Mostra o comando recebido no Serial Monitor (para debug)
    Serial.print("Comando recebido: ");
    Serial.println(command);
    
    // Processa o comando
    if (command == '1') {
      digitalWrite(LED_PIN, HIGH);
      bluetooth.println("LED ligado!");
      Serial.println("LED ligado");
    } 
    else if (command == '0') {
      digitalWrite(LED_PIN, LOW);
      bluetooth.println("LED desligado!");
      Serial.println("LED desligado");
    }
    else {
      bluetooth.println("Comando inválido! Envie '1' ou '0'.");
    }
  }
  
  // Pequeno delay para evitar sobrecarga
  delay(100);
}