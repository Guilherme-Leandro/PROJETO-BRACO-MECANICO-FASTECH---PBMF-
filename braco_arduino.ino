// Definição dos pinos de controle
#define CLK 44
#define CW  36
#define EN  30

// AJUSTE DE MOVIMENTO (Modo Passo Cheio: 200 passos = 1 volta)
// Para dar exatamente 2 voltas completas: 2 x 200 = 400 passos.
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