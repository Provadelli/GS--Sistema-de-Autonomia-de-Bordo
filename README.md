# 🚀 GS - Sistema de Autonomia de Bordo

## 📖 Descrição da Missão

Em missões espaciais, períodos de perda de comunicação com a Terra são comuns devido à distância, interferências ou à passagem da nave por regiões sem cobertura de sinal. Durante esses momentos, o sistema embarcado precisa operar de forma totalmente autônoma, monitorando condições críticas e tomando decisões sem intervenção humana.

O objetivo deste projeto é simular um Sistema de Autonomia de Bordo utilizando Arduino e sensores virtuais no Tinkercad para monitorar variáveis ambientais e executar ações automáticas sempre que parâmetros críticos forem detectados.

O sistema atua como uma camada de segurança da nave, garantindo a continuidade da missão mesmo durante blackouts de comunicação, seguindo os princípios de Edge Computing para sistemas de missão crítica.

---

## 🛠 Componentes Utilizados

- Arduino Uno R3
- Sensor de Temperatura TMP36
- LED Verde
- LED Amarelo
- LED Vermelho
- Buzzer Piezoelétrico
- Resistores de 220Ω
- Protoboard
- Cabos Jumper

---

## 🔌 Esquema de Ligação

### Sensor TMP36

| TMP36 | Arduino |
|--------|----------|
| VCC | 5V |
| VOUT | A0 |
| GND | GND |

### LEDs

| LED | Pino Arduino |
|------|------|
| Verde | D2 |
| Amarelo | D3 |
| Vermelho | D4 |

> Todos os LEDs utilizam resistor de 220Ω em série.

### Buzzer

| Buzzer | Arduino |
|---------|----------|
| Positivo (+) | D8 |
| Negativo (-) | GND |

---

## ⚙️ Funcionamento do Sistema

O sistema monitora continuamente a temperatura interna da nave e classifica o estado operacional em três níveis:

### 🟢 Operação Normal
- LED verde aceso.
- Sistema funcionando normalmente.

### 🟡 Estado de Atenção
- LED amarelo aceso.
- Temperatura acima do limite seguro.

### 🔴 Estado Crítico
- LED vermelho aceso.
- Buzzer ativado.
- Simulação de procedimento autônomo de emergência.

As informações também são exibidas no Monitor Serial para acompanhamento em tempo real.

---

## ▶️ Instruções de Execução

### No Tinkercad

1. Acesse o link do projeto.
2. Clique em **Iniciar Simulação**.
3. Ajuste a temperatura do sensor TMP36.
4. Observe o comportamento dos LEDs e do buzzer.
5. Abra o Monitor Serial para visualizar os dados gerados.

### Na Arduino IDE

1. Copie o código deste repositório.
2. Abra a Arduino IDE.
3. Selecione a placa Arduino Uno.
4. Monte o circuito conforme o esquema de ligação.
5. Faça o upload do código.
6. Abra o Monitor Serial para acompanhar os dados do sistema.

---

## 💻 Código-Fonte Completo

O código-fonte completo está disponível neste repositório:

```bash
/src
 └── main.ino
```

Principais funcionalidades implementadas:

- Leitura contínua do sensor TMP36;
- Conversão de tensão para temperatura;
- Classificação dos níveis de operação;
- Acionamento automático dos LEDs;
- Acionamento do buzzer em situações críticas;
- Exibição de informações no Monitor Serial.

---

## 📊 Fluxograma de Funcionamento

```text
Inicialização
      │
      ▼
Leitura do TMP36
      │
      ▼
Conversão para Temperatura
      │
      ▼
Verificação dos Limites
      │
 ┌────┼────┐
 │    │    │
 ▼    ▼    ▼
Normal Atenção Crítico
 │     │      │
 ▼     ▼      ▼
LED   LED    LED +
Verde Amarelo Vermelho + Buzzer
```

---

## 🌐 Link do Projeto

🔗 Tinkercad:

https://www.tinkercad.com/things/9bgYErn8MZc-gs-sistema-de-autonomia-de-bordo

---

## 👨‍🚀 Objetivo Acadêmico

Projeto desenvolvido para a Global Solution da FIAP na disciplina de Sistemas de Missão Crítica e Autonomia de Bordo.

A proposta consiste em demonstrar conceitos de monitoramento embarcado, Edge Computing e tomada de decisão local, simulando o comportamento de sistemas utilizados em missões espaciais reais, onde a autonomia operacional é essencial para o sucesso da missão.
