#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <ArduinoJson.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;
const char* mqtt_client_id = "abrigo_001";

const char* topic_ocupacao = "abrigo/001/ocupacao";
const char* topic_ambiente = "abrigo/001/ambiente";
const char* topic_recursos = "abrigo/001/recursos";
const char* topic_emergencia = "abrigo/001/emergencia";

#define DHT_PIN 4
#define DHT_TYPE DHT22
#define PIR_PIN 2
#define ULTRASONIC_TRIG 5
#define ULTRASONIC_ECHO 18
#define LED_STATUS 19
#define BUZZER_PIN 21

DHT dht(DHT_PIN, DHT_TYPE);
WiFiClient espClient;
PubSubClient client(espClient);

int pessoas_detectadas = 0;
int ultima_deteccao = 0;
float nivel_agua = 100.0;
bool emergencia_ativa = false;

void setup() {
  Serial.begin(115200);
  
  pinMode(PIR_PIN, INPUT);
  pinMode(ULTRASONIC_TRIG, OUTPUT);
  pinMode(ULTRASONIC_ECHO, INPUT);
  pinMode(LED_STATUS, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  dht.begin();
  
  setup_wifi();
  
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  
  Serial.println("Sistema Abrigue-se IoT Iniciado!");
  digitalWrite(LED_STATUS, HIGH); // LED indica sistema ativo
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  // Ler sensores a cada 5 segundos
  static unsigned long ultimo_envio = 0;
  if (millis() - ultimo_envio > 5000) {
    
    // 1. SENSOR DE OCUPAÇÃO (PIR)
    ler_ocupacao();
    
    // 2. SENSOR AMBIENTE (DHT22)
    ler_ambiente();
    
    // 3. SENSOR DE RECURSOS (Ultrassônico simulando nível água)
    ler_recursos();
    
    // 4. VERIFICAR EMERGÊNCIAS
    verificar_emergencias();
    
    ultimo_envio = millis();
  }
  
  delay(1000);
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensagem recebida [");
  Serial.print(topic);
  Serial.print("]: ");
  
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);
  
  if (String(topic) == "abrigo/001/comando") {
    if (message == "ALARME_ON") {
      emergencia_ativa = true;
      digitalWrite(BUZZER_PIN, HIGH);
    } else if (message == "ALARME_OFF") {
      emergencia_ativa = false;
      digitalWrite(BUZZER_PIN, LOW);
    }
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conexão MQTT...");
    
    if (client.connect(mqtt_client_id)) {
      Serial.println("conectado");
      client.subscribe("abrigo/001/comando");
    } else {
      Serial.print("falhou, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}

void ler_ocupacao() {
  int movimento = digitalRead(PIR_PIN);
  
  if (movimento == HIGH && millis() - ultima_deteccao > 1000) {
    pessoas_detectadas++;
    ultima_deteccao = millis();
    Serial.println("🚶 PESSOA DETECTADA! Nova pessoa entrou no abrigo!");
    Serial.println("👥 Total atualizado: " + String(pessoas_detectadas) + "/50 pessoas");
  } else {
    // Apenas mostrar total atual quando não há detecção
    Serial.println("👥 Total de pessoas no abrigo: " + String(pessoas_detectadas) + "/50");
  }
  
  // Criar JSON para ocupação
  StaticJsonDocument<200> doc;
  doc["abrigo_id"] = "001";
  doc["pessoas_atual"] = pessoas_detectadas;
  doc["capacidade_maxima"] = 50;
  doc["porcentagem_ocupacao"] = (pessoas_detectadas * 100.0) / 50;
  doc["timestamp"] = millis();
  
  char buffer[256];
  serializeJson(doc, buffer);
  
  client.publish(topic_ocupacao, buffer);
  Serial.println("Dados de ocupação enviados");
}

void ler_ambiente() {
  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();
  
  if (isnan(temperatura) || isnan(umidade)) {
    Serial.println("Erro ao ler DHT22!");
    return;
  }
  
  // Criar JSON para ambiente
  StaticJsonDocument<200> doc;
  doc["abrigo_id"] = "001";
  doc["temperatura"] = temperatura;
  doc["umidade"] = umidade;
  doc["qualidade_ar"] = "BOA"; // Simulado
  doc["timestamp"] = millis();
  
  char buffer[256];
  serializeJson(doc, buffer);
  
  client.publish(topic_ambiente, buffer);
  Serial.println("Dados ambientais enviados - Temp: " + String(temperatura) + "°C, Umidade: " + String(umidade) + "%");
}

void ler_recursos() {
  // Simular medição de nível de água com ultrassônico
  digitalWrite(ULTRASONIC_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIG, LOW);
  
  long duration = pulseIn(ULTRASONIC_ECHO, HIGH);
  float distance = duration * 0.034 / 2;
  
  // Converter distância para porcentagem de água (simulação)
  nivel_agua = map(distance, 5, 20, 100, 0); // 5cm = cheio, 20cm = vazio
  if (nivel_agua > 100) nivel_agua = 100;
  if (nivel_agua < 0) nivel_agua = 0;
  
  // Criar JSON para recursos
  StaticJsonDocument<200> doc;
  doc["abrigo_id"] = "001";
  doc["nivel_agua"] = nivel_agua;
  doc["energia_bateria"] = 85; // Simulado
  doc["alimentos"] = 70; // Simulado
  doc["timestamp"] = millis();
  
  char buffer[256];
  serializeJson(doc, buffer);
  
  client.publish(topic_recursos, buffer);
  Serial.println("Dados de recursos enviados - Água: " + String(nivel_agua) + "%");
}

void verificar_emergencias() {
  bool emergencia_atual = false;
  String motivo_atual = "";
  
  // Verificar condições de emergência
  if (nivel_agua < 20) {
    emergencia_atual = true;
    motivo_atual = "AGUA_CRITICA";
  }
  
  if (pessoas_detectadas > 45) { // 90% da capacidade
    emergencia_atual = true;
    if (motivo_atual != "") motivo_atual += " + ";
    motivo_atual += "SUPERLOTACAO";
  }
  
  float temp = dht.readTemperature();
  if (!isnan(temp) && (temp > 35 || temp < 10)) {
    emergencia_atual = true;
    if (motivo_atual != "") motivo_atual += " + ";
    motivo_atual += "TEMPERATURA_EXTREMA";
  }
  
  if (emergencia_atual) {
    // Sempre avisar sobre emergências ativas
    Serial.println("🚨 EMERGÊNCIA ATIVA: " + motivo_atual);
    
    // Só tocar alarme na primeira detecção ou mudança de tipo
    static String ultimo_motivo = "";
    if (motivo_atual != ultimo_motivo) {
      digitalWrite(BUZZER_PIN, HIGH);
      delay(1000);
      digitalWrite(BUZZER_PIN, LOW);
      ultimo_motivo = motivo_atual;
    }
    
    // Enviar alerta de emergência
    StaticJsonDocument<200> doc;
    doc["abrigo_id"] = "001";
    doc["tipo_emergencia"] = motivo_atual;
    doc["nivel"] = "ALTO";
    doc["timestamp"] = millis();
    
    char buffer[256];
    serializeJson(doc, buffer);
    
    client.publish(topic_emergencia, buffer);
    emergencia_ativa = true;
  } else {
    if (emergencia_ativa) {
      Serial.println("✅ Todas as emergências foram resolvidas");
      emergencia_ativa = false;
    } else {
      Serial.println("✅ Nenhuma emergência detectada");
    }
  }
}