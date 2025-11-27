// Controle de LED RGB via Bluetooth HC-06
// LED RGB catodo comum
// App: Serial Bluetooth Terminal

// Pinos do LED RGB
const int pinR = 12;
const int pinG = 11;
const int pinB = 13;

// Variáveis de controle
char comando;
bool piscando = false;
unsigned long tempoAnterior = 0;
const int intervalo = 500; // intervalo do piscar (ms)

// Cor atual
int corR = 0, corG = 0, corB = 0;

// Índice para o modo "mudar de cor automaticamente"
int indiceCor = 0;  // controla qual cor será usada no modo automático

void setup() {
  Serial.begin(9600);

  pinMode(pinR, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(pinB, OUTPUT);

  apagarCor();

  Serial.println("======================================");
  Serial.println("  Sistema iniciado com sucesso");
  Serial.println(" Bluetooth HC-06 pronto");
  Serial.println("  LED RGB aguardando comandos");
  Serial.println("======================================");
}

void loop() {

  // Recebendo comandos do Bluetooth
  if (Serial.available()) {
    comando = Serial.read();

    Serial.print("Comando recebido: ");
    Serial.println(comando);

    switch (comando) {
      case 'R': definirCor(255, 0, 0, "VERMELHO"); break;
      case 'G': definirCor(0, 255, 0, "VERDE"); break;
      case 'B': definirCor(0, 0, 255, "AZUL"); break;
      case 'Y': definirCor(255, 255, 0, "AMARELO"); break;
      case 'C': definirCor(0, 255, 255, "CIANO"); break;
      case 'M': definirCor(255, 0, 255, "MAGENTA"); break;
      case 'W': definirCor(255, 255, 255, "BRANCO"); break;

      case 'O':
        piscando = false;
        apagarCor();
        Serial.println("LED DESLIGADO");
        break;

      case 'P':  // Iniciar piscar e mudar de cor automaticamente
        piscando = true;
        indiceCor = 0; // reinicia ciclo das cores
        Serial.println("MODO PISCAR + MUDAR DE COR ATIVADO");
        break;

      case 'S':  // Parar piscar
        piscando = false;
        aplicarCor();
        Serial.println("MODO PISCAR DESATIVADO (LED mantém última cor)");
        break;

      default:
        Serial.println("Comando inválido!");
    }
  }

  // Lógica do piscar + troca de cor automática
  if (piscando) {
    unsigned long agora = millis();
    if (agora - tempoAnterior >= intervalo) {
      tempoAnterior = agora;
      static bool ligado = false;
      ligado = !ligado;

      if (ligado) {
        // Quando for ligar, troca a cor
        mudarCorAutomaticamente();
        aplicarCor();
        Serial.print("LED ON — nova cor: ");
        Serial.print("R="); Serial.print(corR);
        Serial.print(" G="); Serial.print(corG);
        Serial.print(" B="); Serial.println(corB);
      } else {
        apagarCor();
        Serial.println("LED OFF (piscar)");
      }
    }
  }
}

// -------------------------------------------------------------
// FUNÇÕES AUXILIARES
// -------------------------------------------------------------

void definirCor(int r, int g, int b, String nomeCor) {
  corR = r;
  corG = g;
  corB = b;

  Serial.println("Alterando cor para: " + nomeCor);
  Serial.print("Valores RGB -> ");
  Serial.print("R="); Serial.print(r);
  Serial.print(" G="); Serial.print(g);
  Serial.print(" B="); Serial.println(b);

  aplicarCor();
}

void aplicarCor() {
  analogWrite(pinR, corR);
  analogWrite(pinG, corG);
  analogWrite(pinB, corB);
}

void apagarCor() {
  analogWrite(pinR, 0);
  analogWrite(pinG, 0);
  analogWrite(pinB, 0);
}

void mudarCorAutomaticamente() {
  // Ciclo de 6 cores, pode adicionar mais se quiser
  switch (indiceCor) {
    case 0: definirCor(255, 0, 0, "VERMELHO"); break;
    case 1: definirCor(0, 255, 0, "VERDE"); break;
    case 2: definirCor(0, 0, 255, "AZUL"); break;
    case 3: definirCor(255, 255, 0, "AMARELO"); break;
    case 4: definirCor(0, 255, 255, "CIANO"); break;
    case 5: definirCor(255, 0, 255, "MAGENTA"); break;
  }

  indiceCor++;
  if (indiceCor > 5) indiceCor = 0; // reinicia ciclo
}
