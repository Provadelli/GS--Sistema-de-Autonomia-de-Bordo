/**
 * @file MissionCritical_Tinkercad_TMP36.ino
 * @brief Sistema de Missão Crítica adaptado especificamente para o ecossistema Tinkercad.
 * @author Senior Software Engineer
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// --- Definições de Hardware ---
#define POT_BATTERY_PIN A0
#define TMP_SENSOR_PIN   A1

#define LED_SYS_OK   4
#define LED_HEATER   5
#define LED_SHEDDING 6
#define BUZZER_PIN   7

// --- Constantes de Operação (Ajustadas para o ambiente do Tinkercad) ---
const float TEMP_MIN_SAFE = 30.0;  // Elevado para 30°C para o LED acender no start (TMP36 inicia em 25°C)
const float TEMP_MAX_SAFE = 45.0;  
const int VOLT_CRITICAL   = 400;   // Limiar de corte de carga secundária

// --- Controle de Tempo (millis) ---
const unsigned long INTERVAL_SENSOR_READ = 500;  
const unsigned long INTERVAL_TELEMETRY   = 500;  
const unsigned long INTERVAL_HEARTBEAT   = 1000; 

// --- Variáveis de Estado Global ---
float lastValidTemperature = 25.0; 
int lastValidVoltageADC = 512;
bool sensorFailureState = false;

unsigned long prevMillisSensor = 0;
unsigned long prevMillisTelemetry = 0;
unsigned long prevMillisHeartbeat = 0;

bool heartbeatState = false;

// Configuração do LCD I2C do Tinkercad: Endereço ALTERADO para 0x20
LiquidCrystal_I2C lcd(0x20, 16, 2);

void setup() {
  // Configuração das Portas de Saída e Entrada
  pinMode(LED_SYS_OK, OUTPUT);
  pinMode(LED_HEATER, OUTPUT);
  pinMode(LED_SHEDDING, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  pinMode(TMP_SENSOR_PIN, INPUT);
  pinMode(POT_BATTERY_PIN, INPUT);

  // Inicializa barramento I2C
  Wire.begin();
  
  // Inicialização do LCD padrão Tinkercad
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TINKER-OS START");
  
  // Aguarda 1 segundo de forma não-bloqueante para estabilização de tensões
  unsigned long bootTime = millis();
  while(millis() - bootTime < 1000) {
    digitalWrite(LED_SYS_OK, HIGH);
  }
  lcd.clear();
}

void loop() {
  unsigned long currentMillis = millis();

  // -------------------------------------------------------------------------
  // TAREFA 1: ENGENHARIA DE LEITURA DO TMP36
  // -------------------------------------------------------------------------
  if (currentMillis - prevMillisSensor >= INTERVAL_SENSOR_READ) {
    prevMillisSensor = currentMillis;
    
    int rawTMP = analogRead(TMP_SENSOR_PIN);
    
    // Failsafe contra desconexão física no simulador (0 ou 1023)
    if (rawTMP <= 2 || rawTMP >= 1021) {
      sensorFailureState = true;
    } else {
      sensorFailureState = false;
      
      // Conversão matemática precisa para o TMP36 em barramento de 5V
      float voltage = rawTMP * (5.0 / 1023.0);
      float calculatedTemp = (voltage - 0.5) * 100.0;
      
      // Filtro de validação de limites do sensor
      if (calculatedTemp >= -40.0 && calculatedTemp <= 125.0) {
        lastValidTemperature = calculatedTemp;
      } else {
        sensorFailureState = true;
      }
    }

    // Leitura da telemetria da bateria
    int currentVoltADC = analogRead(POT_BATTERY_PIN);
    if (currentVoltADC >= 0 && currentVoltADC <= 1023) {
      lastValidVoltageADC = currentVoltADC;
    }
    
    // Executa a inteligência artificial de bordo
    processAutonomousDecisions();
  }

  // -------------------------------------------------------------------------
  // TAREFA 2: ATUALIZAÇÃO DO DISPLAY LOCAL
  // -------------------------------------------------------------------------
  if (currentMillis - prevMillisTelemetry >= INTERVAL_TELEMETRY) {
    prevMillisTelemetry = currentMillis;
    updateLocalTelemetry();
  }

  // -------------------------------------------------------------------------
  // TAREFA 3: SINAL DE VIDA DO PROCESSADOR (Heartbeat)
  // -------------------------------------------------------------------------
  if (currentMillis - prevMillisHeartbeat >= INTERVAL_HEARTBEAT) {
    prevMillisHeartbeat = currentMillis;
    if (!sensorFailureState) {
      heartbeatState = !heartbeatState;
      digitalWrite(LED_SYS_OK, heartbeatState);
    } else {
      digitalWrite(LED_SYS_OK, LOW); // Apaga em caso de falha de hardware
    }
  }
}

/**
 * @brief Gerenciador de Atuadores e Sistemas de Sobrevivência
 */
void processAutonomousDecisions() {
  // Controle de Temperatura (Aquecedor da Bateria)
  if (lastValidTemperature < TEMP_MIN_SAFE) {
    digitalWrite(LED_HEATER, HIGH); // Ativa o aquecedor (LED Vermelho)
  } else {
    digitalWrite(LED_HEATER, LOW);  // Desliga o aquecedor
  }

  // Controle Elétrico (Load Shedding)
  if (lastValidVoltageADC < VOLT_CRITICAL) {
    digitalWrite(LED_SHEDDING, HIGH); // Ativa corte de carga (LED Amarelo)
  } else {
    digitalWrite(LED_SHEDDING, LOW);  
  }

  // Alarme de Perigo iminente
  if (lastValidTemperature >= TEMP_MAX_SAFE || sensorFailureState || lastValidVoltageADC < (VOLT_CRITICAL - 100)) {
    tone(BUZZER_PIN, 2200, 100); 
  } else {
    noTone(BUZZER_PIN);
  }
}

/**
 * @brief Envio de dados tratados para o barramento do LCD
 */
void updateLocalTelemetry() {
  // Linha 1 do LCD
  lcd.setCursor(0, 0);
  if (sensorFailureState) {
    lcd.print("ERR_TMP T:---.-C");
  } else {
    lcd.print("OK_SYS  T:");
    lcd.print(lastValidTemperature, 1);
    lcd.print("C  "); 
  }

  // Linha 2 do LCD
  lcd.setCursor(0, 1);
  lcd.print("BAT:");
  int batPercentage = map(lastValidVoltageADC, 0, 1023, 0, 100);
  lcd.print(batPercentage);
  lcd.print("% ");

  // Estado operacional em string estática
  if (digitalRead(LED_SHEDDING)) {
    lcd.print("[SHED]");
  } else if (digitalRead(LED_HEATER)) {
    lcd.print("[HEAT]");
  } else {
    lcd.print("[NOM] ");
  }
}