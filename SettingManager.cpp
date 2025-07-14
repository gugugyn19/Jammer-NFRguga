#include "SettingManager.h"

SettingManager::SettingManager() {
    // O construtor pode inicializar com valores padrão caso a EEPROM esteja vazia.
    currentBrightness = 128;  // Valor médio
    currentScrollSpeed = 150; // Valor médio
}

void SettingManager::init() {
    EEPROM.begin(EEPROM_SIZE);
    loadSettings();
}

void SettingManager::loadSettings() {
    // Lê os valores salvos na EEPROM.
    currentBrightness = EEPROM.read(BRIGHTNESS_ADDR);
    currentScrollSpeed = EEPROM.read(SCROLL_SPEED_ADDR);

    // Validação simples: se a velocidade lida for 0 ou 255 (valor padrão da EEPROM vazia),
    // define um valor padrão razoável.
    if (currentScrollSpeed == 0 || currentScrollSpeed == 255) {
        currentScrollSpeed = 150;
    }
}

uint8_t SettingManager::getBrightness() {
    return currentBrightness;
}

uint8_t SettingManager::getMenuScrollSpeed() {
    return currentScrollSpeed;
}

void SettingManager::setBrightness(uint8_t brightness) {
    currentBrightness = brightness;
    EEPROM.write(BRIGHTNESS_ADDR, brightness);
    EEPROM.commit(); // Salva a alteração na EEPROM
}

void SettingManager::setMenuScrollSpeed(uint8_t speed) {
    currentScrollSpeed = speed;
    EEPROM.write(SCROLL_SPEED_ADDR, speed);
    EEPROM.commit(); // Salva a alteração na EEPROM
}