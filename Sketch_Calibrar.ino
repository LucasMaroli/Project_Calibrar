

// INCLUSÃO DE BIBLIOTECAS
#include <HX711.h>
#include "BluetoothSerial.h"
// DEFINIÇÕES
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
// DEFINIÇÕES DE PINOS
#define pinDT 27
#define pinSCK 26
#define pinBotao 35
#define nvl1 22
#define nvl2 18
#define nvl3 23
#define nvl4 21
#define nvl5 15
#define nvl6 2
#define nvl7 4
#define buttonTare 16
#define buttonRestart 19
// INSTANCIANDO OBJETOS
HX711 scale;
BluetoothSerial SerialBT;
// DECLARAÇÃO DE VARIÁVEIS
float medida = 0;
uint8_t k1 = 0;
uint8_t k2 = 0;
uint8_t k3 = 0;
bool buttonPressed = false;

void setup() {

  Serial.begin(57600);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  scale.begin(pinDT, pinSCK); // CONFIGURANDO OS PINOS DA BALANÇA
  scale.wait_ready(0);
	scale.wait_ready_retry(3, 0);
	scale.wait_ready_timeout(300, 0);
  scale.set_scale(32150.000); // VALOR DA ESCALA (KG)
  delay(2000);
  scale.tare(); // ZERANDO A BALANÇA PARA DESCONSIDERAR A MASSA DA ESTRUTURA

  pinMode(nvl1, OUTPUT); // DEFININDO OS PINOS COMO SAIDA
  pinMode(nvl2, OUTPUT);
  pinMode(nvl3, OUTPUT);
  pinMode(nvl4, OUTPUT);
  pinMode(nvl5, OUTPUT);
  pinMode(nvl6, OUTPUT);
  pinMode(nvl7, OUTPUT);
  pinMode(buttonTare, INPUT_PULLUP);
  pinMode(buttonRestart, INPUT_PULLUP);

  scale.power_up();

}

void loop() {

  if(digitalRead(buttonTare) == LOW) { // Realiza a Tara

    k1++;
    delay(100);

    if (k1 == 30) {

      Serial.println("Botão pressionado! Tara...");
      SerialBT.printf("Botão pressionado! Tara...");
      scale.tare();
      k1 = 0;

    }


  }

  if(digitalRead(buttonRestart) == LOW) { // Reinicia o programa

    k2++;
    delay(100);

    if (k2 == 20) {

      k2 = 0;
      SerialBT.printf("Botão pressionado! Reiniciando...");

      while(k3 < 3) {

      digitalWrite(nvl1, HIGH);
      digitalWrite(nvl2, HIGH);
      digitalWrite(nvl3, HIGH);
      digitalWrite(nvl4, HIGH);
      digitalWrite(nvl5, HIGH);
      digitalWrite(nvl6, HIGH);
      digitalWrite(nvl7, HIGH);
      delay(1000);
      k3++;

      }

      k3 = 0;
      ESP.restart(); // Reinicia o ESP32

    }

  }


  medida = scale.get_units(1); // SALVANDO NA VARIAVEL MEDIDA
  Serial.println(medida, 3); // ENVIANDO PARA MONITOR SERIAL A MEDIDA COM 3 CASAS DECIMAIS

  if (medida != 0) { // AVALIA SE O MONITOR RECEBEU ALGUMA MEDIDA

    digitalWrite(nvl1, LOW);
    digitalWrite(nvl2, LOW);
    digitalWrite(nvl3, LOW);
    digitalWrite(nvl4, LOW);
    digitalWrite(nvl5, LOW);
    digitalWrite(nvl6, LOW);
    digitalWrite(nvl7, LOW);

    if(medida < 1.000) {
    SerialBT.printf("Menos de 1 kgf!\n");
    }

    if(medida > 1.000 && medida < 2.000) {

    SerialBT.printf("Entre 1 kgf e 2 kgf!\n");
    digitalWrite(nvl1, HIGH);

    }

    if(medida > 2.000 && medida < 4.000) {

    SerialBT.printf("Entre 2 kgf e 4 kgf!\n");
    digitalWrite(nvl1, HIGH);
    digitalWrite(nvl2, HIGH);

    }

    if(medida > 4.000 && medida < 8.000) {

    SerialBT.printf("Entre 4 kgf e 8 kgf!\n");
    digitalWrite(nvl1, HIGH);
    digitalWrite(nvl2, HIGH);
    digitalWrite(nvl3, HIGH);

    }

    if(medida > 8.000 && medida < 12.000) {

    SerialBT.printf("Entre 8 kgf e 12 kgf!\n");
    digitalWrite(nvl1, HIGH);
    digitalWrite(nvl2, HIGH);
    digitalWrite(nvl3, HIGH);
    digitalWrite(nvl4, HIGH);

    }

    if(medida > 12.000 && medida < 16.000) {

    SerialBT.printf("Entre 12 kgf e 16 kgf!\n");
    digitalWrite(nvl1, HIGH);
    digitalWrite(nvl2, HIGH);
    digitalWrite(nvl3, HIGH);
    digitalWrite(nvl4, HIGH);
    digitalWrite(nvl5, HIGH);

    }

    if(medida > 16.000 && medida < 20.000) {

    SerialBT.printf("Entre 16 kgf e 20 kgf!\n");
    digitalWrite(nvl1, HIGH);
    digitalWrite(nvl2, HIGH);
    digitalWrite(nvl3, HIGH);
    digitalWrite(nvl4, HIGH);
    digitalWrite(nvl5, HIGH);
    digitalWrite(nvl6, HIGH);

    }

    if(medida > 20.000) {

    SerialBT.printf("Mais de 20 kgf!\n");
    digitalWrite(nvl1, HIGH);
    digitalWrite(nvl2, HIGH);
    digitalWrite(nvl3, HIGH);
    digitalWrite(nvl4, HIGH);
    digitalWrite(nvl5, HIGH);
    digitalWrite(nvl6, HIGH);
    digitalWrite(nvl7, HIGH);

    }

  }

}
