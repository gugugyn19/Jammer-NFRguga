#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>
#include <U8g2lib.h>

// Definindo as dimensões da tela aqui para que o DisplayManager as conheça
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

class DisplayManager {
public:
    /**
     * @brief Construtor da classe DisplayManager.
     *        Inicializa o objeto u8g2 com a rotação e pino de reset corretos.
     */
    DisplayManager();

    /**
     * @brief Inicializa a comunicação com o display e define o brilho inicial.
     * @param initialBrightness O valor do brilho (0-255) a ser aplicado no início.
     */
    void init(uint8_t initialBrightness);

    /**
     * @brief Define o brilho (contraste) do display OLED.
     * @param brightness O valor do brilho a ser definido (0-255).
     */
    void setBrightness(uint8_t brightness);

    /**
     * @brief Desenha o menu principal na tela.
     * @param menuItems Array de strings com os nomes dos itens do menu.
     * @param totalItems O número total de itens no menu.
     * @param selectedItem O índice do item atualmente selecionado.
     */
    void drawMenu(const char* menuItems[], int totalItems, int selectedItem);
    
    /**
     * @brief Mostra uma tela de "atividade" para indicar que uma função está em execução.
     * @param activityName O nome da atividade/função em execução (ex: "Scanning...").
     */
    void showActivityScreen(const char* activityName);

    /**
     * @brief Limpa o buffer do display.
     */
    void clear();

private:
    // O objeto da biblioteca U8G2 é agora um membro privado da classe.
    // Ninguém fora desta classe pode acessá-lo diretamente.
    U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;
};

#endif // DISPLAY_MANAGER_H