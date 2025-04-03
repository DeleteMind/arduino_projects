// Definir o pino do relé
int relePin = 7;  // O pino digital do Arduino que vai controlar o relé

void setup() {
  // Configura o pino do relé como saída
  pinMode(relePin, OUTPUT);
  
  // Certificar-se de que o relé começa desligado
  digitalWrite(relePin, LOW); // Relé desligado no início
}

void loop() {
  // Ligar o relé (motor ligado)
  digitalWrite(relePin, LOW); // Aciona o relé
   // Motor desligado por 5 segundos
}
