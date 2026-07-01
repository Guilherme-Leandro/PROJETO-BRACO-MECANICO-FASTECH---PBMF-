# 🤖 Documentação Oficial do Projeto: Motor de Passo (NEMA 14 + TB6560)

Esta página contém o histórico final, o esquema elétrico validado, o código-fonte definitivo e a explicação minuciosa linha por linha do sistema do braço robótico.

---

## 🔌 1. Esquema de Hardware e Conexões

A fiação foi montada seguindo o método de **Cátodo Comum** (GND compartilhado), isolando os sinais lógicos do Arduino da potência da fonte.

### 📶 Sinais Lógicos (Arduino Mega ➔ Driver)

- **Pino 44 (Digital)** ➔ Conectado ao borne **CLK+** (Pulsos de passo)
- **Pino 36 (Digital)** ➔ Conectado ao borne **CW+** (Sentido de rotação)
- **Pino 30 (Digital)** ➔ Conectado ao borne **EN+** (Habilitação do driver)
- **GND (Arduino)** ➔ Conectado a uma emenda que une **CLK-**, **CW-** e **EN-**

### ⚡ Potência e Motor (Fonte/Motor ➔ Driver)

- **Bornes +24V e GND:** Conectados diretamente na fonte de alimentação externa.
- **Bornes A+, A-, B+, B-:** Conectados aos 4 fios das bobinas do motor NEMA 14 (identificados na ordem correta de continuidade).

---

## 🎛️ 2. Configuração Física das Chaves (DIP Switches)

### 💻 Chaves do Arduino (Comunicação USB)

Para carregar o código via IDE sem erros de comunicação, as chaves centrais da placa integrada devem estar no modo **USB ➔ MCU**:

- **Chaves 3 e 4:** **ON**
- **Chaves 1, 2, 5, 6, 7 e 8:** **OFF**

### 🔴 Chaves Vermelhas do Driver (Modo de Passo)

O driver está operando fisicamente no modo de **16 Micropassos (Microstepping)**. Isso significa que o motor se move de forma suave e precisa, precisando de **3200 micro-passos** para completar 1 volta inteira ($360^\circ$):

- `S1: ON` | `S2: ON` | `S3: OFF` | `S4: OFF` | `S5: ON` | `S6: ON`

---

## 💻 3. Código-Fonte Validado (Firmware)

Este código implementa uma interface interativa via **Monitor Serial (9600 baud)**.

```cpp
// Definição dos pinos de controle
#define CLK 44
#define CW  36
#define EN  30

// AJUSTE DE MOVIMENTO (Modo 16 Micropassos: 3200 passos = 1 volta)
// Para dar exatamente 2 voltas completas: 2 x 3200 = 6400 passos.
const int passosDesejados = 6400;

// Tempo em microssegundos entre os pulsos. Menor = Mais Rápido.
const int tempoPulso = 400;

void exibirMenu();

void setup() {
  pinMode(CLK, OUTPUT);
  pinMode(CW,  OUTPUT);
  pinMode(EN,  OUTPUT);

  digitalWrite(EN, LOW);  // Liga o driver
  digitalWrite(CW, LOW);  // Sentido inicial horário

  Serial.begin(9600);
  exibirMenu();
}

void loop() {
  if (Serial.available() > 0) {
    char cmd = Serial.read();

    if (cmd == '\n' || cmd == '\r' || cmd == ' ') return;

    Serial.print("\nComando recebido: ");
    Serial.println(cmd);

    // [ G ] - Girar sentido Horário
    if (cmd == 'G' || cmd == 'g') {
      digitalWrite(CW, LOW);
      Serial.println(">> Executando: Girando 2 voltas (HORÁRIO)...");

      for (int i = 0; i < passosDesejados; i++) {
        digitalWrite(CLK, HIGH);
        delayMicroseconds(tempoPulso);
        digitalWrite(CLK, LOW);
        delayMicroseconds(tempoPulso);
      }
      Serial.println(">> Movimento concluído!");
      exibirMenu();
    }

    // [ A ] - Girar sentido Anti-Horário
    else if (cmd == 'A' || cmd == 'a') {
      digitalWrite(CW, HIGH);
      Serial.println(">> Executando: Girando 2 voltas (ANTI-HORÁRIO)...");

      for (int i = 0; i < passosDesejados; i++) {
        digitalWrite(CLK, HIGH);
        delayMicroseconds(tempoPulso);
        digitalWrite(CLK, LOW);
        delayMicroseconds(tempoPulso);
      }
      Serial.println(">> Movimento concluído!");
      exibirMenu();
    }

    // [ P ] - Status de Retenção
    else if (cmd == 'P' || cmd == 'p') {
      Serial.println(">> Status: O motor está parado e energizado (travado).");
      exibirMenu();
    }

    // Comando Inválido
    else {
      Serial.println(">> Erro: Comando não reconhecido.");
      exibirMenu();
    }
  }
}

void exibirMenu() {
  Serial.println("\n------------------------------------------------");
  Serial.println("       PAINEL DE CONTROLE DO MOTOR DE PASSO     ");
  Serial.println("------------------------------------------------");
  Serial.println(" Digite uma das letras abaixo e pressione ENTER:");
  Serial.println("  [ G ] -> Girar no sentido Horário (2 voltas)");
  Serial.println("  [ A ] -> Girar no sentido Anti-horário (2 voltas)");
  Serial.println("  [ P ] -> Verificar status de retenção do motor");
  Serial.println("------------------------------------------------");
}
```
---
# CÓDIGO EXPLICADO EM DETALHES
# 💻 Documentação Completa do Código: Linha por Linha

Este guia traz o código-fonte definitivo e explica o funcionamento de absolutamente cada linha de forma simples, focada em quem não domina a linguagem C++.

---

## 📜 1. Código-Fonte Completo e Validado

Este é o programa que está rodando no seu Arduino Mega. Ele cria um painel interativo no Monitor Serial e controla o driver e o motor:

```cpp
// Definição dos pinos de controle
#define CLK 44
#define CW  36
#define EN  30

// AJUSTE DE MOVIMENTO (Modo Passo Cheio: 200 passos = 1 volta)
// Para dar exatamente 2 voltas completas: 2 x 200 = 400 passos.
const int passosDesejados = 400;

// Tempo em microssegundos entre os pulsos. Menor = Mais Rápido.
const int tempoPulso = 400;

void exibirMenu();

void setup() {
  pinMode(CLK, OUTPUT);
  pinMode(CW,  OUTPUT);
  pinMode(EN,  OUTPUT);

  digitalWrite(EN, LOW);  // Liga o driver
  digitalWrite(CW, LOW);  // Sentido inicial horário

  Serial.begin(9600);
  exibirMenu();
}

void loop() {
  if (Serial.available() > 0) {
    char cmd = Serial.read();

    if (cmd == '\n' || cmd == '\r' || cmd == ' ') return;

    Serial.print("\nComando recebido: ");
    Serial.println(cmd);

    // [ G ] - Girar sentido Horário
    if (cmd == 'G' || cmd == 'g') {
      digitalWrite(CW, LOW);
      Serial.println(">> Executando: Girando 2 voltas (HORÁRIO)...");

      for (int i = 0; i < passosDesejados; i++) {
        digitalWrite(CLK, HIGH);
        delayMicroseconds(tempoPulso);
        digitalWrite(CLK, LOW);
        delayMicroseconds(tempoPulso);
      }
      Serial.println(">> Movimento concluído!");
      exibirMenu();
    }

    // [ A ] - Girar sentido Anti-Horário
    else if (cmd == 'A' || cmd == 'a') {
      digitalWrite(CW, HIGH);
      Serial.println(">> Executando: Girando 2 voltas (ANTI-HORÁRIO)...");

      for (int i = 0; i < passosDesejados; i++) {
        digitalWrite(CLK, HIGH);
        delayMicroseconds(tempoPulso);
        digitalWrite(CLK, LOW);
        delayMicroseconds(tempoPulso);
      }
      Serial.println(">> Movimento concluído!");
      exibirMenu();
    }

    // [ P ] - Status de Retenção
    else if (cmd == 'P' || cmd == 'p') {
      Serial.println(">> Status: O motor está parado e energizado (travado).");
      exibirMenu();
    }

    // Comando Inválido
    else {
      Serial.println(">> Erro: Comando não reconhecido.");
      exibirMenu();
    }
  }
}

void exibirMenu() {
  Serial.println("\n------------------------------------------------");
  Serial.println("       PAINEL DE CONTROLE DO MOTOR DE PASSO     ");
  Serial.println("------------------------------------------------");
  Serial.println(" Digite uma das letras abaixo e pressione ENTER:");
  Serial.println("  [ G ] -> Girar no sentido Horário (2 voltas)");
  Serial.println("  [ A ] -> Girar no sentido Anti-horário (2 voltas)");
  Serial.println("  [ P ] -> Verificar status de retenção do motor");
  Serial.println("------------------------------------------------");
}
```

## 📖 2. Explicação Detalhada: Linha por Linha

Em C++, tudo o que começa com `//` é apenas um **comentário** descritivo que o Arduino ignora ao rodar. Serve apenas para nós lermos.

### 📍 Parte A: As Definições (Batismo dos Pinos)

```arduino
#define CLK 44  
#define CW  36  
#define EN  30
```

- **`#define CLK 44`**: Batiza o pino digital 44 da placa com o nome `CLK`. Cada pulso enviado por aqui faz o motor dar um passo.
- **`#define CW 36`**: Batiza o pino 36 como `CW` (*Clockwise*). Ele controla se o motor vai girar para um lado ou para o outro.
- **`#define EN 30`**: Batiza o pino 30 como `EN` (*Enable*). Ele liga ou desliga a energia que vai para o motor.

### ⚙️ Parte B: Variáveis de Configuração

```arduino
const int passosDesejados = 400; 
const int tempoPulso = 400;
```

- **`const int passosDesejados = 400;`**: Configura o limite de passos. Como o nosso driver precisa de 200 passos para dar 1 volta, colocamos `400` para darmos **2 voltas completas**.
- **`const int tempoPulso = 400;`**: Controla a velocidade. É o tempo de espera em microssegundos. **Girar mais rápido = Diminuir esse número. Girar mais lento = Aumentar esse número.**

### 🛠️ Parte C: O bloco `setup()` (A Inicialização)

Roda **apenas uma vez** logo que o Arduino recebe energia:

- **`pinMode(..., OUTPUT);`**: Configura os três pinos como saídas de energia. Avisa ao Arduino que ele vai injetar tensão ali em direção ao driver.
- **`digitalWrite(EN, LOW);`**: Coloca o pino EN em 0 Volts (`LOW`). No driver TB6560, colocar 0 Volts neste pino é o comando que "LIGA" e trava a força do motor.
- **`digitalWrite(CW, LOW);`**: Define o sentido de rotação inicial como horário.
- **`Serial.begin(9600);`**: Abre o canal de comunicação USB entre o Arduino e o Monitor Serial do computador a uma velocidade de 9600 bits por segundo.
- **`exibirMenu();`**: Executa a função que desenha o menu de texto na tela do computador.

### 🔄 Parte D: O bloco `loop()` (O Monitoramento Contínuo)

Fica repetindo em ciclo infinito, milhares de vezes por segundo:

- **`if (Serial.available() > 0)`**: Uma condição que pergunta: *"O usuário digitou alguma tecla e clicou em Enviar?"*. Se sim, o programa avança para ler.
- **`char cmd = Serial.read();`**: Cria uma variável de texto chamada `cmd` e guarda dentro dela a letra que você digitou no teclado.
- **`if (cmd == '\n' || cmd == '\r' || cmd == ' ') return;`**: Um filtro essencial. O Monitor Serial costuma enviar "caracteres fantasmas" de quebra de linha junto com os comandos. Essa linha diz para o Arduino ignorar esses enters invisíveis ou espaços e voltar ao início do loop.

### 🟢 Parte E: A Execução dos Comandos

O programa usa a lógica de condições `if` (Se) e `else if` (Ou Se) para tomar decisões baseadas na letra que foi armazenada em `cmd`:

#### 1. Se você digitar 'G' ou 'g' (Sentido Horário)

- Executa `digitalWrite(CW, LOW);` para garantir que o driver está configurado para o sentido horário.
- Entra no laço de repetição `for (int i = 0; i < passosDesejados; i++)`. Esse laço cria um contador automático `i` que começa em 0 e vai subindo de 1 em 1 até bater no número 400 (`passosDesejados`). Ele vai repetir tudo o que está dentro dele exatamente 400 vezes.
- **O pulso físico:** Dentro do laço, o pino `CLK` vai para `HIGH` (5 Volts), aguarda os 400 microssegundos do `tempoPulso`, vai para `LOW` (0 Volts) e aguarda mais 400 microssegundos. Esse ato de ligar e desligar o pino repetido 400 vezes força o driver a mover o motor por exatamente 2 voltas cravadas.

#### 2. Se você digitar 'A' ou 'a' (Sentido Anti-Horário)

- Faz exatamente a mesma repetição de 400 pulsos do comando anterior. A única diferença crucial é que ele executa `digitalWrite(CW, HIGH);` (5 Volts) no pino de direção. Essa inversão de tensão avisa o chip do driver para inverter o magnetismo interno e rodar o motor para o sentido anti-horário.

#### 3. Se você digitar 'P' ou uma tecla inválida

- Se for **'P'**, ele imprime na tela que o ciclo terminou e o motor está em modo de retenção (travado magneticamente para o braço não desabar).
- Se for qualquer outra letra, cai no bloco `else`, que exibe uma mensagem de comando inválido e redesenha o menu na tela para uma nova tentativa.
