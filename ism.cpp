/* ____________________________
   This software is licensed under the MIT License:
   https://github.com/cifertech/nrfbox
   ________________________________________ */

// Inclusões explícitas de dependências
#include <SPI.h>
#include "config.h" // Continua necessário para os ponteiros de função e u8g2
#include "setting.h"  // Para as definições de pinos

//================================================================================
// Módulo Analyzer
//================================================================================
namespace Analyzer {

  // Constantes para os registradores do NRF24 (mais seguro que #define)
  constexpr uint8_t NRF24_CONFIG   = 0x00;
  constexpr uint8_t NRF24_EN_AA    = 0x01;
  constexpr uint8_t NRF24_RF_CH    = 0x05;
  constexpr uint8_t NRF24_RF_SETUP = 0x06;
  constexpr uint8_t NRF24_RPD      = 0x09;
  
  // Constante para o tamanho do buffer de valores
  constexpr int NUM_CHANNELS = 128;

  // Struct para encapsular o estado do Analyzer
  struct State {
    uint8_t values[NUM_CHANNELS];
    int channel = 0;
  };

  State state; // Instância da struct de estado

  // Função auxiliar para ler um registrador do NRF24
  uint8_t readRegister(uint8_t reg) {
    digitalWrite(NRF_CSN, LOW);
    SPI.transfer(reg);
    uint8_t value = SPI.transfer(0x00);
    digitalWrite(NRF_CSN, HIGH);
    return value;
  }

  // Função auxiliar para definir o canal
  void setChannel(uint8_t ch) {
    digitalWrite(NRF_CSN, LOW);
    SPI.transfer(NRF24_RF_CH | 0x20); // Comando de escrita no registrador RF_CH
    SPI.transfer(ch);
    digitalWrite(NRF_CSN, HIGH);
  }

  void analyzerSetup() {
    // Configuração inicial do NRF24 para modo de recepção
    digitalWrite(NRF_CSN, LOW);
    SPI.transfer(NRF24_CONFIG | 0x20); // Comando de escrita
    SPI.transfer(0x0F); // RX, 2-byte CRC
    digitalWrite(NRF_CSN, HIGH);
    delay(5);

    digitalWrite(NRF_CE, HIGH); // Habilita a recepção
  }

  void analyzerLoop() {
    setChannel(state.channel);
    delayMicroseconds(150); // Aguarda o PLL estabilizar

    // Lê o RPD (Received Power Detector) e armazena
    // O bit 0 do registrador RPD indica se a potência recebida é > -64dBm
    if (readRegister(NRF24_RPD) & 1) {
      state.values[state.channel] = 1;
    } else {
      state.values[state.channel] = 0;
    }

    // Avança para o próximo canal
    state.channel++;
    if (state.channel >= NUM_CHANNELS) {
      state.channel = 0;
    }

    // Desenha o gráfico no display
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_profont10_tf);
    u8g2.drawStr(0, 8, "Analyzer");
    for (int i = 0; i < NUM_CHANNELS; i++) {
      if (state.values[i] == 1) {
        u8g2.drawPixel(i, 63); // Desenha um ponto se o canal estiver ocupado
      }
    }
    u8g2.sendBuffer();
  }
}

//================================================================================
// Módulo Jammer
//================================================================================
namespace Jammer {

  // Struct para encapsular o estado do Jammer
  struct State {
    bool jamming = false;
    int current_option = 0;
    int current_channel = 0;
    unsigned long lastDebounceTimeBT1 = 0;
    unsigned long lastDebounceTimeBT2 = 0;
    unsigned long lastDebounceTimeBT3 = 0;
    unsigned long lastDebounceTimeBT4 = 0;
  };
  
  State state; // Instância da struct de estado

  const char* jammer_options[] = {"Start", "Channel"};
  const int num_options = 2;

  // Função para transmitir um payload em um canal específico
  void jammer() {
    digitalWrite(NRF_CE, LOW); // Modo Standby-I para configurar

    // Configura para modo de transmissão
    digitalWrite(NRF_CSN, LOW);
    SPI.transfer(0x20); // Comando de escrita no registrador CONFIG
    SPI.transfer(0x0E); // TX, 2-byte CRC
    digitalWrite(NRF_CSN, HIGH);

    // Define o canal
    setChannel(state.current_channel);

    // Envia o payload
    digitalWrite(NRF_CSN, LOW);
    SPI.transfer(0xA0); // Comando W_TX_PAYLOAD
    for(int i = 0; i < 32; i++) {
      SPI.transfer(0xFF); // Payload de interferência
    }
    digitalWrite(NRF_CSN, HIGH);

    // Pulsa CE para transmitir
    digitalWrite(NRF_CE, HIGH);
    delayMicroseconds(20);
    digitalWrite(NRF_CE, LOW);
  }

  // Funções de callback para os botões
  void navigateUp() { if (!state.jamming) state.current_option = (state.current_option - 1 + num_options) % num_options; }
  void navigateDown() { if (!state.jamming) state.current_option = (state.current_option + 1) % num_options; }
  void changeOptionLeft() { if (state.current_option == 1) state.current_channel = (state.current_channel - 1 + 128) % 128; }
  void changeOptionRight() {
    if (state.current_option == 0) {
      state.jamming = !state.jamming;
    } else if (state.current_option == 1) {
      state.current_channel = (state.current_channel + 1) % 128;
    }
  }

  void updateDisplay() {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_profont10_tf);

    // Desenha o título
    char title[15];
    sprintf(title, "Jammer [%s]", state.jamming ? "ON" : "OFF");
    u8g2.drawStr(0, 8, title);

    // Desenha as opções do menu
    for (int i = 0; i < num_options; i++) {
        if (i == state.current_option) {
            u8g2.drawBox(0, 12 + i * 10, 60, 10);
            u8g2.setDrawColor(0); // Inverte a cor para o texto
        }
        u8g2.drawStr(2, 20 + i * 10, jammer_options[i]);
        u8g2.setDrawColor(1); // Restaura a cor padrão
    }
    
    // Mostra o valor do canal
    char channel_str[15];
    sprintf(channel_str, "CH: %d", state.current_channel);
    u8g2.drawStr(70, 30, channel_str);

    u8g2.sendBuffer();
  }

  void jammerSetup() {
    pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
    pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
    pinMode(BTN_PIN_LEFT, INPUT_PULLUP);
    pinMode(BTN_PIN_RIGHT, INPUT_PULLUP);
    
    u8g2.begin();
    updateDisplay();
  }

  void jammerLoop() {
    handleButtonPress(BUTTON_UP_PIN, state.lastDebounceTimeBT1, navigateUp);
    handleButtonPress(BTN_PIN_RIGHT, state.lastDebounceTimeBT2, changeOptionRight);
    handleButtonPress(BUTTON_DOWN_PIN, state.lastDebounceTimeBT3, navigateDown);
    handleButtonPress(BTN_PIN_LEFT, state.lastDebounceTimeBT4, changeOptionLeft);

    updateDisplay();

    if (state.jamming) {
      jammer();
    }

    delay(50); // Pequeno delay para estabilidade
  }
}