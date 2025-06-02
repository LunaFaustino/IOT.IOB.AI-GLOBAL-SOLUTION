# Sistema Abrigue-se IoT

## 👥 Equipe

| Nome | RM | Turma |
|------|----|----|
| **Luna Faustino Lima** | 552473 | 2TDSPS |
| **Larissa Araújo Gama Alvarenga** | 96496 | 2TDSPS |
| **Larissa Lopes Oliveira** | 552628 | 2TDSPC |

### 📹 Demonstração

**Links importantes:**
- **🎥 Vídeo demonstração**: [https://www.youtube.com/watch?v=YuNUJ33ozRw](https://www.youtube.com/watch?v=YuNUJ33ozRw)
- **🔧 Simulador funcionando**: [https://wokwi.com/projects/432688021089960961](https://wokwi.com/projects/432688021089960961)
- **📊 Dashboard exemplo**: Disponível após configuração do Node-RED

## 🌍 Sobre a solução da Global Solution

Nossa solução é o aplicativo mobile **"Abrigue-se"**, desenvolvido para auxiliar na gestão de abrigos durante desastres naturais. Pelo aplicativo, administradores poderiam monitorar em tempo real a ocupação e recursos disponíveis. Como cidadão seria possível verificar a disponibilidade de abrigos próximos e solicitar uma rota segura até ele.

## 🏠 Sobre a entrega da matéria

O **Sistema Abrigue-se IoT** é uma solução tecnológica desenvolvida para monitoramento inteligente de abrigos durante eventos climáticos extremos. O sistema integra sensores IoT, comunicação MQTT e dashboards em tempo real para auxiliar na gestão eficiente de recursos e ocupação de abrigos de emergência.

### 🎯 Objetivos

- **Monitoramento em Tempo Real**: Ocupação, condições ambientais e recursos disponíveis
- **Alertas Proativos**: Notificações automáticas para situações críticas
- **Tomada de Decisão**: Dados precisos para coordenadores de emergência
- **Gestão de Recursos**: Controle de água, energia e suprimentos
- **Segurança**: Monitoramento de condições ambientais para bem-estar dos abrigados

### 📡 Fluxo de Dados

1. **Coleta**: Sensores coletam dados a cada 5 segundos
2. **Transmissão**: ESP32 envia via MQTT para broker público
3. **Processamento**: Node-RED recebe, processa e analisa dados
4. **Visualização**: Dashboard exibe informações em tempo real
5. **Alertas**: Sistema gera notificações automáticas para emergências

## 🔧 Hardware e Sensores

### 📟 Microcontrolador
- **ESP32 DevKit C**: WiFi integrado, processamento e conectividade

### 🌡️ Sensores Implementados

| Sensor | Função | Dados Coletados |
|--------|---------|-----------------|
| **DHT22** | Monitoramento Ambiental | Temperatura, Umidade |
| **PIR Motion** | Detecção de Ocupação | Presença/Movimento |
| **HC-SR04** | Nível de Recursos | Distância (simula água) |
| **LED** | Indicador de Status | Status do Sistema |
| **Buzzer** | Alarme Sonoro | Alertas de Emergência |

## 💻 Tecnologias Utilizadas

### 🖥️ Hardware
- **ESP32**: Microcontrolador principal
- **Wokwi**: Simulador de hardware online

### 📚 Bibliotecas e Frameworks
- **Arduino IDE**: Desenvolvimento do firmware
- **WiFi.h**: Conectividade wireless
- **PubSubClient.h**: Protocolo MQTT
- **DHT.h**: Sensor de temperatura/umidade
- **ArduinoJson.h**: Manipulação de dados JSON

### 🌐 Comunicação e Gateway
- **MQTT Protocol**: Comunicação assíncrona IoT
- **Node-RED**: Gateway e processamento de dados
- **test.mosquitto.org**: Broker MQTT público

### 📊 Visualização
- **Node-RED Dashboard**: Interface web em tempo real
- **Chart.js**: Gráficos e visualizações
- **UI Components**: Gauges, textos e alertas

## ⚙️ Instalação e Configuração

### 📋 Pré-requisitos

- **Node.js** (versão 14 ou superior)
- **Git** para clonagem do repositório
- **Navegador web** para acessar dashboards

### 🚀 Passo 1: Clone o Repositório

```bash
git clone https://github.com/LunaFaustino/IOT.IOB.AI-GLOBAL-SOLUTION.git
cd IOT.IOB.AI-GLOBAL-SOLUTION
```

### 🔧 Passo 2: Configure o Simulador Wokwi

**Opção A - Usar Projeto Público (Recomendado)**:
1. Acesse diretamente: **[Projeto Wokwi - Sistema Abrigue-se IoT](https://wokwi.com/projects/432688021089960961)**
2. Clique em **"Start Simulation"**
3. Observe o Serial Monitor funcionando

**Opção B - Criar do Zero**:
1. Acesse [Wokwi.com](https://wokwi.com)
2. Crie um novo projeto ESP32
3. Copie o código de `projeto wokwi/sketch.ino`
4. Importe o diagrama de `projeto wokwi/diagram.json`
5. Adicione as bibliotecas conforme `projeto wokwi/libraries.txt`:
   - DHT sensor library
   - PubSubClient
   - ArduinoJson

### 🌐 Passo 3: Instale Node-RED

```bash
# Instalar Node-RED globalmente
npm install -g node-red

# Executar Node-RED
node-red
```

### 📊 Passo 4: Configure Dashboard

1. Acesse `http://localhost:1880`
2. Instale o dashboard:
   - Menu → Manage Palette → Install
   - Buscar: `node-red-dashboard`
   - Install

3. Importe o fluxo:
   - Menu → Import
   - Cole o conteúdo de `flow_node-red.json`
   - Deploy

## ▶️ Como Executar

### 🔥 Execução Completa

1. **Inicie o Simulador Wokwi**:
   - Acesse: **[https://wokwi.com/projects/432688021089960961](https://wokwi.com/projects/432688021089960961)**
   - Clique em **"Start Simulation"**
   - Observe o Serial Monitor

2. **Inicie o Node-RED**:
   ```bash
   node-red
   ```

3. **Acesse o Dashboard**:
   - Navegue para `http://localhost:1880/ui`
   - Visualize dados em tempo real

## 🔄 Fluxos Node-RED

### 📥 Entrada de Dados (MQTT Input)

```javascript
// Tópicos MQTT configurados:
- abrigo/001/ocupacao    // Dados de pessoas no abrigo
- abrigo/001/ambiente    // Temperatura e umidade
- abrigo/001/recursos    // Níveis de água, energia, alimentos
- abrigo/001/emergencia  // Alertas críticos
```

### 📊 Saída de Dados (Dashboard)

| Componente | Tipo | Função |
|------------|------|---------|
| **Gauge Ocupação** | ui_gauge | Mostra % de ocupação (0-100%) |
| **Chart Ambiente** | ui_chart | Gráfico temporal temp/umidade |
| **Gauges Recursos** | ui_gauge | Níveis de água, energia, alimentos |
| **Text Alertas** | ui_text | Mensagens de status e emergência |