#include "NeoPixelManager.h"

// Usamos uma lista de inicializadores para construir o objeto 'strip'
// Esta é a maneira correta em C++ de inicializar objetos membros.
NeoPixelManager::NeoPixelManager(uint16_t numPixels, int8_t pin)
    : strip(numPixels, pin, NEO_GRB + NEO_KHZ800) {}

void NeoPixelManager::init() {
    strip.begin();           // Inicializa a biblioteca NeoPixel.
    strip.clear();           // Garante que todos os pixels comecem desligados.
    strip.show();            // Envia os dados para a fita.
}

void NeoPixelManager::setNeoPixelColour(uint32_t color) {
    strip.fill(color);
    strip.show();
}

void NeoPixelManager::scanEffect(uint32_t color, int scanDelay) {
    for (int i = 0; i < strip.numPixels(); i++) {
        strip.clear(); // Limpa o pixel anterior
        strip.setPixelColor(i, color);
        strip.show();
        delay(scanDelay);
    }
    clear(); // Apaga o último pixel ao final do efeito
}

uint32_t NeoPixelManager::Color(uint8_t r, uint8_t g, uint8_t b) {
    return strip.Color(r, g, b);
}

void NeoPixelManager::clear() {
    strip.clear();
    strip.show();
}