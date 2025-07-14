#include "DisplayManager.h"

// O construtor usa uma lista de inicialização para configurar o objeto u8g2.
// Isso é mais eficiente do que atribuir valores dentro das chaves {}.
DisplayManager::DisplayManager() : u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE) {
  // O corpo do construtor pode ficar vazio se toda a inicialização
  // for feita na lista de inicialização.
}

void DisplayManager::init(uint8_t initialBrightness) {
  u8g2.begin();
  setBrightness(initialBrightness);
}

void DisplayManager::setBrightness(uint8_t brightness) {
  // A biblioteca U8g2 usa o termo "setContrast" para o brilho do display OLED.
  u8g2.setContrast(brightness);
}

void DisplayManager::clear() {
    u8g2.clearBuffer();
    u8g2.sendBuffer();
}

void DisplayManager::showActivityScreen(const char* activityName) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_7x13B_tr); // Usando uma fonte um pouco maior para destaque

    // Calcula a posição X para centralizar o texto
    u8g2_uint_t textWidth = u8g2.getStrWidth(activityName);
    u8g2_uint_t x = (SCREEN_WIDTH - textWidth) / 2;

    // Calcula a posição Y para centralizar o texto
    u8g2_uint_t y = (SCREEN_HEIGHT / 2) + 4; // Um pequeno ajuste para centralizar verticalmente

    u8g2.drawStr(x, y, activityName);
    u8g2.sendBuffer();
}

void DisplayManager::drawMenu(const char* menuItems[], int totalItems, int selectedItem) {
  // 1. Limpa a memória (buffer) do display antes de desenhar algo novo.
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tf); // Define a fonte para o menu

  for (int i = 0; i < totalItems; i++) {
    int yPosition = 12 + i * 13; // Calcula a posição Y para cada item

    // 2. Verifica se o item atual é o que está selecionado
    if (i == selectedItem) {
      // Desenha uma caixa preenchida para destacar o item selecionado
      u8g2.drawBox(0, yPosition - 9, SCREEN_WIDTH, 12); 
      // Define a cor de desenho para "transparente" (ou cor de fundo),
      // para que o texto apareça sobre a caixa preenchida.
      u8g2.setDrawColor(0); 
    }

    // 3. Desenha o texto do item do menu
    u8g2.drawStr(2, yPosition, menuItems[i]);

    // 4. Restaura a cor de desenho padrão para o próximo item
    u8g2.setDrawColor(1);
  }

  // 5. Envia o conteúdo do buffer para a tela do display físico.
  u8g2.sendBuffer();
}