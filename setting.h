#ifndef SETTING_H
#define SETTING_H

#include <Arduino.h> // Para usar tipos como uint8_t

// Esta é a estrutura que armazena todas as configurações persistentes
// que você deseja salvar na EEPROM.
struct AppSettings {
    uint8_t brightness; // Brilho para os LEDs e Display (0-255)
    // Você pode adicionar mais configurações aqui no futuro
    // int volume;
    // bool bluetoothEnabled;
};

#endif // SETTING_H
