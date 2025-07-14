#ifndef SETTING_MANAGER_H
#define SETTING_MANAGER_H

#include <Arduino.h>
#include <EEPROM.h>

// Define um tamanho para a EEPROM. 16 bytes é mais que suficiente para nossas configurações.
#define EEPROM_SIZE 16

class SettingManager {
public:
    /**
     * @brief Construtor da classe SettingManager.
     */
    SettingManager();

    /**
     * @brief Inicializa o SettingManager. Deve ser chamado no setup().
     *        Começa a comunicação com a EEPROM e carrega as configurações salvas.
     */
    void init();

    /**
     * @brief Obtém o valor de brilho atual.
     * @return O valor do brilho (0-255).
     */
    uint8_t getBrightness();

    /**
     * @brief Obtém a velocidade de rolagem do menu atual.
     * @return O valor da velocidade de rolagem (em milissegundos).
     */
    uint8_t getMenuScrollSpeed();

    /**
     * @brief Define e salva um novo valor de brilho.
     * @param brightness O novo valor de brilho (0-255) a ser salvo na EEPROM.
     */
    void setBrightness(uint8_t brightness);

    /**
     * @brief Define e salva uma nova velocidade de rolagem do menu.
     * @param speed O novo valor de velocidade (em ms) a ser salvo na EEPROM.
     */
    void setMenuScrollSpeed(uint8_t speed);

private:
    // Endereços na memória EEPROM para cada configuração.
    const int BRIGHTNESS_ADDR = 0;
    const int SCROLL_SPEED_ADDR = 1;

    // Variáveis para manter os valores atuais em RAM para acesso rápido.
    uint8_t currentBrightness;
    uint8_t currentScrollSpeed;

    /**
     * @brief Carrega as configurações da EEPROM para as variáveis da classe.
     */
    void loadSettings();
};

#endif // SETTING_MANAGER_H