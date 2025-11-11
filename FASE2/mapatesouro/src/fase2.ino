#include <Arduino.h>
#include "DHT.h"

// --- MAPEAMENTO DOS PINOS ---
#define DHT_PIN 4
#define LDR_PIN 34
#define BTN_N_PIN 25
#define BTN_P_PIN 26
#define BTN_K_PIN 27
#define RELE_PIN 13
#define LED_STATUS_PIN 12

// --- CONFIGURAÇÃO DO SENSOR DHT ---
#define DHT_TYPE DHT22
DHT dht(DHT_PIN, DHT_TYPE);

// --- VARIÁVEIS GLOBAIS ---
unsigned long previousMillis = 0;
const long interval = 500;
int ledState = LOW;

void setup() {
  // CHECKPOINT 1
  Serial.begin(115200);
  delay(1000); // Tempo para o monitor serial conectar
  Serial.println("--- CHECKPOINT 1: Setup iniciado ---");

  // Inicia o sensor DHT
  //dht.begin();
  
  // CHECKPOINT 2
  Serial.println("--- CHECKPOINT 2: DHT.begin() executado ---");

  pinMode(LDR_PIN, INPUT);
  pinMode(BTN_N_PIN, INPUT);
  pinMode(BTN_P_PIN, INPUT);
  pinMode(BTN_K_PIN, INPUT);
  pinMode(RELE_PIN, OUTPUT);
  pinMode(LED_STATUS_PIN, OUTPUT);

  digitalWrite(RELE_PIN, LOW);
  digitalWrite(LED_STATUS_PIN, LOW);

  // CHECKPOINT 3
  Serial.println("--- CHECKPOINT 3: Setup completo. Entrando no loop... ---");
}

void loop() {
  // CHECKPOINT 4
  Serial.println("--- CHECKPOINT 4: Loop iniciado ---");

  float umidade = dht.readHumidity();
  
  if (isnan(umidade)) {
    Serial.println("!!! ERRO: Falha ao ler o sensor DHT. Verificando novamente...");
    delay(1000); // Espera um segundo antes de tentar de novo
    return;
  }

  int valorLDR = analogRead(LDR_PIN);
  bool n_pressionado = (digitalRead(BTN_N_PIN) == HIGH);
  bool p_pressionado = (digitalRead(BTN_P_PIN) == HIGH);
  bool k_pressionado = (digitalRead(BTN_K_PIN) == HIGH);
  float ph = (valorLDR / 4095.0) * 14.0;

  bool deveIrrigar = (umidade < 50.0 && ph > 6.0 && ph < 7.0 && n_pressionado);

  if (deveIrrigar) {
    digitalWrite(RELE_PIN, HIGH);
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      ledState = !ledState;
      digitalWrite(LED_STATUS_PIN, ledState);
    }
  } else {
    digitalWrite(RELE_PIN, LOW);
    digitalWrite(LED_STATUS_PIN, LOW);
    ledState = LOW;
  }

// --- SEÇÃO PRINT
Serial.print("DADOS: Umidade: " + String(umidade) + "%");
Serial.print(" | pH: " + String(ph));
Serial.print(" | N: " + String(n_pressionado));
Serial.print(" | P: " + String(p_pressionado));
Serial.print(" | K: " + String(k_pressionado));
Serial.print(" | Irrigando: " + String(deveIrrigar));
Serial.println(" --- Fim do Loop ---");

  delay(5000);
}