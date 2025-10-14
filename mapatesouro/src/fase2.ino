#include <Arduino.h>
#include <DHT.h>

// ==================== DEFINIÇÃO DE PINOS ====================
#define DHT_PIN 4          // Sensor DHT22 (umidade do solo simulada)
#define LDR_PIN 34         // Sensor LDR (pH simulado - analógico)
#define BTN_N_PIN 25       // Botão Nitrogênio (true/false)
#define BTN_P_PIN 26       // Botão Fósforo (true/false)
#define BTN_K_PIN 27       // Botão Potássio (true/false)
#define LED_PIN 12         // LED de status
#define RELAY_PIN 13       // Relé da bomba d'água

// ==================== CONFIGURAÇÃO DO DHT22 ====================
#define DHT_TYPE DHT22
DHT dht(DHT_PIN, DHT_TYPE);

// ==================== PARÂMETROS DE IRRIGAÇÃO ====================
// Cultura escolhida: TOMATE
// Referências ideais para tomate:
// - pH: 6.0 a 6.8 (ligeiramente ácido)
// - Umidade do solo: 60-80%
// - NPK: Necessita de níveis adequados (botões pressionados = nutriente OK)

// Limites de umidade do solo
const float UMIDADE_MIN = 60.0;  // Abaixo disso, precisa irrigar
const float UMIDADE_MAX = 80.0;  // Acima disso, não precisa irrigar

// Escala de pH expandida (0-100) para facilitar manipulação do LDR
// LDR retorna 0-4095 (ADC 12 bits), mapeamos para 0-100 para melhor controle
// pH ideal tomate: 60-68 na escala 0-100 (equivalente a 6.0-6.8 real)
const float PH_MIN = 40.0;        // pH muito baixo (< 4.0)
const float PH_IDEAL_MIN = 60.0;  // pH 6.0 (ideal mínimo)
const float PH_IDEAL_MAX = 68.0;  // pH 6.8 (ideal máximo)
const float PH_MAX = 85.0;        // pH muito alto (> 8.5)

// Influência dos nutrientes NPK no pH (conforme requisito do trabalho)
// Cada nutriente presente altera ligeiramente o pH medido
const float AJUSTE_PH_POR_NUTRIENTE = 2.0; // Cada botão pressionado ajusta +2 no pH

// Flags de estado
bool bombaLigada = false;
unsigned long ultimaLeitura = 0;
const unsigned long INTERVALO_LEITURA = 3000; // Ler sensores a cada 3 segundos

// Controle de LED piscante
unsigned long ultimoPisca = 0;
const unsigned long INTERVALO_PISCA = 500; // Piscar a cada 500ms
bool ledState = false;

// Previsão de chuva (API OpenWeather - opcional)
float previsaoChuva = 0.0; // mm de chuva prevista

// ==================== FUNÇÕES AUXILIARES ====================

void piscarLED() {
  unsigned long tempoAtual = millis();
  
  if (bombaLigada) {
    // Se bomba ligada, LED pisca
    if (tempoAtual - ultimoPisca >= INTERVALO_PISCA) {
      ultimoPisca = tempoAtual;
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState ? HIGH : LOW);
    }
  } else {
    // Se bomba desligada, LED apagado
    if (ledState) {
      digitalWrite(LED_PIN, LOW);
      ledState = false;
    }
  }
}

float calcularPHComNutrientes(int leituraLDR, bool N, bool P, bool K) {
  // Mapear LDR (0-4095) para escala de pH (0-100)
  float phBase = map(leituraLDR, 0, 4095, 0, 100);
  
  // Aplicar influência dos nutrientes NPK no pH
  // Conforme requisito: "mexer nos botões altera o pH"
  int nutrientesAtivos = (N ? 1 : 0) + (P ? 1 : 0) + (K ? 1 : 0);
  float ajustePH = nutrientesAtivos * AJUSTE_PH_POR_NUTRIENTE;
  
  float phFinal = phBase + ajustePH;
  
  // Limitar entre 0-100
  if (phFinal > 100) phFinal = 100;
  if (phFinal < 0) phFinal = 0;
  
  return phFinal;
}

void setup() {
  Serial.begin(115200);
  
  // Configurar pinos
  pinMode(BTN_N_PIN, INPUT);
  pinMode(BTN_P_PIN, INPUT);
  pinMode(BTN_K_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LDR_PIN, INPUT);
  
  // Inicializar DHT
  dht.begin();
  
  // Estado inicial: bomba desligada
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  
  Serial.println("===========================================");
  Serial.println("  SISTEMA DE IRRIGAÇÃO INTELIGENTE");
  Serial.println("  FarmTech Solutions - Cultura: TOMATE");
  Serial.println("===========================================");
  Serial.println("\nREQUISITOS IMPLEMENTADOS:");
  Serial.println("✓ 3 Botões NPK (true/false - pressionado/solto)");
  Serial.println("✓ LDR como sensor de pH (escala 0-100)");
  Serial.println("✓ DHT22 como umidade do solo");
  Serial.println("✓ Relação NPK ↔ pH implementada");
  Serial.println("✓ Sistema de irrigação automatizado");
  Serial.println("\nCOMANDOS:");
  Serial.println("  CHUVA:X - Previsão de chuva em mm");
  Serial.println("===========================================\n");
  
  // Teste inicial do LED
  Serial.println(">>> Testando LED...");
  for (int i = 0; i < 6; i++) {
    digitalWrite(LED_PIN, i % 2);
    delay(200);
  }
  Serial.println(">>> LED OK!\n");
  
  Serial.println(">>> Aguardando estabilização do DHT22...");
  delay(2000);
  
  // Teste de leitura do DHT22
  float testTemp = dht.readTemperature();
  float testHum = dht.readHumidity();
  
  if (isnan(testTemp) || isnan(testHum)) {
    Serial.println("⚠️  ATENÇÃO: DHT22 não respondeu!");
    Serial.println("    Verifique as conexões");
  } else {
    Serial.println("✓ DHT22 funcionando!");
    Serial.print("  Temperatura: ");
    Serial.print(testTemp);
    Serial.println("°C");
    Serial.print("  Umidade: ");
    Serial.print(testHum);
    Serial.println("%");
  }
  Serial.println();
}

void loop() {
  unsigned long tempoAtual = millis();
  
  // Atualizar LED continuamente
  piscarLED();
  
  // Verificar entrada serial para previsão de chuva (opcional)
  if (Serial.available() > 0) {
    String entrada = Serial.readStringUntil('\n');
    entrada.trim();
    
    if (entrada.startsWith("CHUVA:")) {
      previsaoChuva = entrada.substring(6).toFloat();
      Serial.print(">>> Previsão de chuva atualizada: ");
      Serial.print(previsaoChuva);
      Serial.println(" mm\n");
    }
  }
  
  // Realizar leitura periódica dos sensores
  if (tempoAtual - ultimaLeitura >= INTERVALO_LEITURA) {
    ultimaLeitura = tempoAtual;
    realizarLeituraESistema();
  }
}

void realizarLeituraESistema() {
  Serial.println("\n========== LEITURA DOS SENSORES ==========");
  
  // ========== LEITURA DOS BOTÕES NPK (TRUE/FALSE) ==========
  bool nitrogenio = digitalRead(BTN_N_PIN);   // TRUE = pressionado, FALSE = solto
  bool fosforo = digitalRead(BTN_P_PIN);
  bool potassio = digitalRead(BTN_K_PIN);
  
  Serial.println("--- Nutrientes NPK (Botões) ---");
  Serial.print("Nitrogênio (N): ");
  Serial.print(nitrogenio ? "TRUE " : "FALSE");
  Serial.println(nitrogenio ? " [Pressionado ✓]" : " [Solto ✗]");
  
  Serial.print("Fósforo (P):    ");
  Serial.print(fosforo ? "TRUE " : "FALSE");
  Serial.println(fosforo ? " [Pressionado ✓]" : " [Solto ✗]");
  
  Serial.print("Potássio (K):   ");
  Serial.print(potassio ? "TRUE " : "FALSE");
  Serial.println(potassio ? " [Pressionado ✓]" : " [Solto ✗]");
  
  // ========== LEITURA DO pH (LDR) COM INFLUÊNCIA DO NPK ==========
  int leituraLDR = analogRead(LDR_PIN);
  float phSemNutrientes = map(leituraLDR, 0, 4095, 0, 100);
  float phComNutrientes = calcularPHComNutrientes(leituraLDR, nitrogenio, fosforo, potassio);
  
  Serial.println("\n--- pH do Solo (LDR) ---");
  Serial.print("Leitura LDR (ADC): ");
  Serial.println(leituraLDR);
  Serial.print("pH Base (sem NPK): ");
  Serial.println(phSemNutrientes, 1);
  Serial.print("pH Final (com NPK): ");
  Serial.print(phComNutrientes, 1);
  Serial.print(" (Escala 0-100)");
  
  String statusPh;
  if (phComNutrientes < PH_MIN) {
    statusPh = " - MUITO ÁCIDO! ⚠️";
  } else if (phComNutrientes < PH_IDEAL_MIN) {
    statusPh = " - Ácido";
  } else if (phComNutrientes <= PH_IDEAL_MAX) {
    statusPh = " - IDEAL ✓";
  } else if (phComNutrientes <= PH_MAX) {
    statusPh = " - Alcalino";
  } else {
    statusPh = " - MUITO ALCALINO! ⚠️";
  }
  Serial.println(statusPh);
  
  int nutrientesAtivos = (nitrogenio ? 1 : 0) + (fosforo ? 1 : 0) + (potassio ? 1 : 0);
  if (nutrientesAtivos > 0) {
    Serial.print("⚗️  Ajuste pH por NPK: +");
    Serial.print(nutrientesAtivos * AJUSTE_PH_POR_NUTRIENTE, 1);
    Serial.println(" (nutrientes alteram o pH)");
  }
  
  // ========== LEITURA DA UMIDADE DO SOLO (DHT22) ==========
  float umidade = dht.readHumidity();
  float temperatura = dht.readTemperature();
  
  // Tentar ler novamente se falhar
  if (isnan(umidade) || isnan(temperatura)) {
    delay(100);
    umidade = dht.readHumidity();
    temperatura = dht.readTemperature();
  }
  
  if (isnan(umidade) || isnan(temperatura)) {
    Serial.println("\n⚠️  ERRO: Falha na leitura do DHT22!");
    Serial.println("    Usando valores padrão para teste:");
    umidade = 55.0;
    temperatura = 24.0;
  }
  
  Serial.println("\n--- Umidade do Solo (DHT22) ---");
  Serial.print("Umidade: ");
  Serial.print(umidade, 1);
  Serial.println("%");
  Serial.print("Temperatura: ");
  Serial.print(temperatura, 1);
  Serial.println("°C");
  
  // ========== ANÁLISE E DECISÃO DE IRRIGAÇÃO ==========
  Serial.println("\n========== ANÁLISE DO SISTEMA ==========");
  
  bool necessitaIrrigacao = false;
  String motivo = "";
  
  // CONDIÇÃO 1: Verificar previsão de chuva
  if (previsaoChuva > 5.0) {
    Serial.print("☔ Previsão de chuva: ");
    Serial.print(previsaoChuva);
    Serial.println(" mm - Irrigação SUSPENSA");
    necessitaIrrigacao = false;
    motivo = "Chuva prevista";
  }
  // CONDIÇÃO 2: Verificar umidade do solo
  else if (umidade < UMIDADE_MIN) {
    Serial.print("⚠️  SOLO SECO! Umidade ");
    Serial.print(umidade, 1);
    Serial.print("% < ");
    Serial.print(UMIDADE_MIN, 1);
    Serial.println("%");
    necessitaIrrigacao = true;
    motivo = "Umidade baixa";
  }
  else if (umidade > UMIDADE_MAX) {
    Serial.print("✓ Solo úmido (");
    Serial.print(umidade, 1);
    Serial.println("%)");
    necessitaIrrigacao = false;
    motivo = "Solo já úmido";
  }
  else {
    Serial.print("✓ Umidade adequada (");
    Serial.print(umidade, 1);
    Serial.println("%)");
    necessitaIrrigacao = false;
    motivo = "Umidade adequada";
  }
  
  // CONDIÇÃO 3: Verificar pH (bloqueia se crítico)
  if (phComNutrientes < PH_MIN || phComNutrientes > PH_MAX) {
    Serial.println("⚠️  pH CRÍTICO! Correção necessária antes de irrigar");
    necessitaIrrigacao = false;
    motivo = "pH fora da faixa segura";
  }
  
  // CONDIÇÃO 4: Verificar NPK (informativo)
  Serial.print("Nutrientes adequados: ");
  Serial.print(nutrientesAtivos);
  Serial.println("/3");
  
  if (nutrientesAtivos < 2 && necessitaIrrigacao) {
    Serial.println("⚠️  Nutrientes insuficientes! Considere adubação");
  }
  
  // ========== CONTROLAR BOMBA D'ÁGUA E LED ==========
  Serial.println("\n========== DECISÃO FINAL ==========");
  
  if (necessitaIrrigacao && !bombaLigada) {
    // LIGAR a bomba
    digitalWrite(RELAY_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);
    bombaLigada = true;
    ledState = true;
    ultimoPisca = millis();
    Serial.println("💧 BOMBA LIGADA! (LED piscando)");
    Serial.print("Motivo: ");
    Serial.println(motivo);
  }
  else if (!necessitaIrrigacao && bombaLigada) {
    // DESLIGAR a bomba
    digitalWrite(RELAY_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
    bombaLigada = false;
    ledState = false;
    Serial.println("🛑 BOMBA DESLIGADA! (LED apagado)");
    Serial.print("Motivo: ");
    Serial.println(motivo);
  }
  else if (bombaLigada) {
    Serial.println("💧 Bomba continua LIGADA (LED piscando)");
  }
  else {
    Serial.println("✓ Bomba permanece DESLIGADA");
  }
  
  Serial.println("===========================================\n");
}