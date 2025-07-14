#include "Encoder.h"

// Inicializa o array estático de instâncias.
// Usamos a mesma diretiva de compilação condicional do .h
#if defined(ESP32)
    Encoder* Encoder::_instances[XCHAL_NUM_INTERRUPTS] = { nullptr };
#else
    Encoder* Encoder::_instances[CORE_NUM_INTERRUPT] = { nullptr };
#endif

// Tabela de transição para decodificação rápida
const int8_t KNOBDIR[] = {
  0, -1,  1,  0,
  1,  0,  0, -1,
  -1,  0,  0,  1,
  0,  1, -1,  0
};

Encoder::Encoder(uint8_t pinA, uint8_t pinB) {
    _pinA = pinA;
    _pinB = pinB;
    _position = 0;

    pinMode(_pinA, INPUT_PULLUP);
    pinMode(_pinB, INPUT_PULLUP);
    
    // Lê o estado inicial
    _state = (digitalRead(_pinA) << 1) | digitalRead(_pinB);

    // Registra esta instância do encoder no array estático
    // para que a ISR possa encontrá-la
    _instances[digitalPinToInterrupt(_pinA)] = this;
    _instances[digitalPinToInterrupt(_pinB)] = this;

    // Anexa as interrupções usando funções estáticas
    attachInterrupt(digitalPinToInterrupt(_pinA), _isrPinA, CHANGE);
    attachInterrupt(digitalPinToInterrupt(_pinB), _isrPinB, CHANGE);
}

long Encoder::read() {
    return _position;
}

void Encoder::write(long newPosition) {
    noInterrupts(); // Desliga interrupções para escrita segura
    _position = newPosition;
    interrupts();   // Liga interrupções novamente
}

// ISR estática para o pino A
void IRAM_ATTR Encoder::_isrPinA() {
    // Descobre qual instância chamou a ISR e chama sua função _update
    for (uint8_t i = 0; i < sizeof(_instances) / sizeof(_instances[0]); ++i) {
        if (_instances[i]) {
            _instances[i]->_update();
        }
    }
}

// ISR estática para o pino B (pode chamar a mesma função de pino A)
void IRAM_ATTR Encoder::_isrPinB() {
    _isrPinA(); // Reutiliza a mesma lógica
}

// Função de membro que faz a atualização real. Chamada pelas ISRs.
void IRAM_ATTR Encoder::_update() {
    _state = (_state << 2) | (digitalRead(_pinA) << 1) | digitalRead(_pinB);
    _position += KNOBDIR[_state & 0x0F];
}