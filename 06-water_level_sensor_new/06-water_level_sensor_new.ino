
#include <Wire.h>

#define SERIAL Serial

#define THRESHOLD 90   // Ajuste para sensibilidade
#define ATTINY1_HIGH_ADDR 0x78
#define ATTINY2_LOW_ADDR 0x77
#define NUM_READINGS 5  // Média móvel para suavizar leituras
#define MAX_SECTIONS 20  // Número máximo de secções do sensor

unsigned char low_data[8] = {0};
unsigned char high_data[12] = {0};
float readings[NUM_READINGS] = {0};  
int readIndex = 0;  
float total = 0;  
float average = 0;  

// Função para os 12 valores mais altos 

void getHigh12SectionValue() {
    memset(high_data, 0, sizeof(high_data));
    Wire.requestFrom(ATTINY1_HIGH_ADDR, 12); //Pede os dados do dispositico (endereço, quantidade)
    while (Wire.available() < 12);

    for (int i = 0; i < 12; i++) {
        high_data[i] = Wire.read();
    }
    delay(10);
}

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

void setup() {
    SERIAL.begin(115200);
    Wire.begin();
}

void loop() {
    check();
    delay(2000);
}
