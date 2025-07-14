/*
 * =================================================================================
 * nRFBox - Versão Refatorada com Arquitetura Orientada a Objetos
 * =================================================================================
 * Autor: Seu Nome
 * Data: [Data Atual]
 *
 * Descrição:
 * Este é o arquivo principal do projeto, atuando como um "maestro".
 * Sua responsabilidade é inicializar e coordenar os "gerenciadores" (Managers)
 * que encapsulam as funcionalidades de hardware e configurações.
 *
 * Arquitetura:
 * - SettingManager: Gerencia o salvamento e carregamento de configurações na EEPROM.
 * - DisplayManager: Controla tudo relacionado à tela OLED (desenho, brilho, etc.).
 * - NeoPixelManager: Comanda a fita de LEDs NeoPixel (cores, brilho, animações).
 *
 * O fluxo principal (loop) agora apenas lê a entrada do usuário (encoder e botão)
 * e delega as ações para os gerenciadores apropriados.
 * =================================================================================
 */

// Bibliotecas de Hardware
#include <Encoder.h>

// Nossas classes de gerenciamento (ordem não importa aqui)
#include "SettingManager.h"
#include "DisplayManager.h"
#include "NeoPixelManager.h"
#include "Encoder.h"

// --- Configurações de Hardware e Pinos ---
#define ENCODER_PIN_A 2
#define ENCODER_PIN_B 3
#define BUTTON_PIN    4

// --- Definições do Menu da Aplicação ---
const char *menuItems[] = {"Scan WiFi", "Brightness", "LEDs Off"};
const int MENU_ITEMS_COUNT = sizeof(menuItems) / sizeof(menuItems[0]);

// --- Instanciação dos Nossos Objetos Gerenciadores ---
// Em vez de vários objetos globais (U8G2, Adafruit_NeoPixel),
// agora temos apenas os nossos gerenciadores.
SettingManager  settings;
DisplayManager  display;
NeoPixelManager leds;
Encoder         encoder(ENCODER_PIN_A, ENCODER_PIN_B);

// --- Variáveis de Estado da Aplicação ---
// Estas variáveis controlam o estado atual da UI.
int  selectedItem = 0;
long lastEncoderPos = -999;


// =================================================================================
//   SETUP - Inicialização do Sistema
// =================================================================================
void setup() {
  Serial.begin(115200);
  
  // 1. Inicializa o gerenciador de configurações (carrega dados da EEPROM)
  settings.init();

  // 2. Inicializa o display, já com o brilho carregado das configurações
  display.init(settings.getBrightness());
  display.showActivityScreen("Booting..."); // Mostra uma tela de boot
  delay(1000);

  // 3. Inicializa os NeoPixels, também com o brilho correto
  leds.init(settings.getBrightness());
  leds.setColor(0, 0, 255); // Cor azul para indicar que ligou
  delay(1000);
  leds.clear();
  
  // Configura o pino do botão do encoder
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  Serial.println("nRFBox inicializado e pronto.");
}


// =================================================================================
//   LOOP - Ciclo Principal da Aplicação
// =================================================================================
void loop() {
  // --- Leitura de Entrada do Usuário ---
  
  // Lê o encoder e atualiza o item selecionado no menu
  long currentEncoderPos = encoder.read() / 4; // /4 para mais estabilidade
  if (currentEncoderPos != lastEncoderPos) {
    selectedItem += (currentEncoderPos - lastEncoderPos);
    
    // Garante que o item selecionado permaneça dentro dos limites do menu
    if (selectedItem < 0) {
      selectedItem = MENU_ITEMS_COUNT - 1;
    } else if (selectedItem >= MENU_ITEMS_COUNT) {
      selectedItem = 0;
    }
    
    lastEncoderPos = currentEncoderPos;
  }

  // Verifica se o botão foi pressionado
  if (digitalRead(BUTTON_PIN) == LOW) {
    delay(200); // Debounce simples para evitar múltiplos cliques
    handleMenuAction(selectedItem);
  }

  // --- Atualização da Exibição ---
  // Desenha o menu na tela a cada ciclo. O DisplayManager cuida de toda a complexidade.
  display.drawMenu(menuItems, MENU_ITEMS_COUNT, selectedItem);
}


// =================================================================================
//   FUNÇÕES AUXILIARES - Lógica da Aplicação
// =================================================================================

/**
 * @brief Executa a ação correspondente ao item de menu selecionado.
 * @param itemIndex O índice do item do menu que foi selecionado.
 */
void handleMenuAction(int itemIndex) {
  switch (itemIndex) {
    case 0: // Scan WiFi
      scanNetworks();
      break;
    case 1: // Brightness
      adjustBrightness();
      break;
    case 2: // LEDs Off
      leds.clear();
      break;
  }
}

/**
 * @brief Simula uma varredura de redes WiFi.
 */
void scanNetworks() {
  Serial.println("Ação: Scan WiFi");
  display.showActivityScreen("Scanning...");
  leds.setColor(255, 255, 0); // Amarelo para indicar atividade
  
  // (Aqui entraria a sua lógica real de scan)
  delay(3000); 

  leds.clear();
  display.clear(); // Limpa a tela para o menu ser redesenhado no próximo loop
}

/**
 * @brief Entra em um modo para ajustar o brilho do display e dos LEDs.
 */
void adjustBrightness() {
  Serial.println("Ação: Ajustar Brilho");
  uint8_t currentBrightness = settings.getBrightness();
  long lastBrightnessPos = encoder.read() / 4;

  while (true) {
    // Desenha a UI de ajuste de brilho
    display.clear();
    display.showActivityScreen("Set Brightness"); // Reutilizando a função para mostrar um título
    // (Poderíamos criar uma função drawBrightnessScreen no DisplayManager para algo mais elaborado)

    // Lê o encoder para ajustar o brilho em tempo real
    long currentBrightnessPos = encoder.read() / 4;
    if (currentBrightnessPos != lastBrightnessPos) {
      currentBrightness += (currentBrightnessPos - lastBrightnessPos);
      // Limita o valor entre 0 e 255
      currentBrightness = max(0, min(255, (int)currentBrightness));
      
      leds.setBrightness(currentBrightness);
      display.setBrightness(currentBrightness); // Feedback visual instantâneo
      
      // Demonstração no LED
      leds.setColor(255, 255, 255);
      
      lastBrightnessPos = currentBrightnessPos;
    }

    // Verifica se o botão foi pressionado para sair e salvar
    if (digitalRead(BUTTON_PIN) == LOW) {
      delay(200); // Debounce
      settings.saveBrightness(currentBrightness); // Salva a nova configuração
      Serial.print("Brilho salvo: ");
      Serial.println(currentBrightness);
      leds.clear();
      break; // Sai do loop de ajuste
    }
  }
}