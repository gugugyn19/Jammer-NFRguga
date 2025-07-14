#ifndef NEOPIXEL_MANAGER_H
#define NEOPIXEL_MANAGER_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class NeoPixelManager {
public:
    /**
     * @brief Construtor da classe NeoPixelManager.
     * @param numPixels O número de LEDs na fita.
     * @param pin O pino do microcontrolador ao qual o pino de dados do NeoPixel está conectado.
     */
    NeoPixelManager(uint16_t numPixels, int8_t pin);

    /**
     * @brief Inicializa a fita NeoPixel. Deve ser chamado no setup().
     */
    void init();

    /**
     * @brief Define uma cor sólida para todos os LEDs da fita.
     * @param color A cor no formato de 32 bits (use strip.Color(R, G, B)).
     */
    void setNeoPixelColour(uint32_t color);

    /**
     * @brief Executa um efeito de "scan", acendendo um LED por vez.
     * @param color A cor a ser usada no efeito.
     * @param scanDelay O atraso em milissegundos entre cada passo do LED.
     */
    void scanEffect(uint32_t color, int scanDelay);
    
    /**
     * @brief Função utilitária para obter uma cor a partir de valores R, G, B.
     * @return A cor no formato de 32 bits.
     */
    uint32_t Color(uint8_t r, uint8_t g, uint8_t b);

    /**
     * @brief Apaga todos os LEDs (define a cor para preto).
     */
    void clear();

private:
    // O objeto da Adafruit_NeoPixel agora é um membro privado.
    Adafruit_NeoPixel strip;
};

#endif // NEOPIXEL_MANAGER_H