

// Inicializa o array estático de ponteiros para as instâncias do Encoder
#if defined(ESP32)
    Encoder* Encoder::_instances[XCHAL_NUM_INTERRUPTS] = { nullptr };
#else
    Encoder* Encoder::_instances[16] = { nullptr };
#endif

// Construtor
Encoder::Encoder(uint8_t pinA, uint8_t pinB) {
    _pinA = pinA;
    _pinB = pinB;
    _position = 0;
    _state = 0;

    pinMode(_pinA, INPUT_PULLUP);
    pinMode(_pinB, INPUT_PULLUP);

    // Lê o estado inicial
    if (digitalRead(_pinA)) _state |= 1;
    if (digitalRead(_pinB)) _state |= 2;
    
    // Converte o número do pino para um número de interrupção
    int8_t interruptA = digitalPinToInterrupt(_pinA);
    int8_t interruptB = digitalPinToInterrupt(_pinB);

    if (interruptA != NOT_AN_INTERRUPT) {
        // Armazena o ponteiro 'this' no array estático na posição da interrupção
        _instances[interruptA] = this;
        // Anexa a função ISR estática
        attachInterrupt(interruptA, _isrPinA, CHANGE);
    }
    
    if (interruptB != NOT_AN_INTERRUPT) {
        _instances[interruptB] = this;
        attachInterrupt(interruptB, _isrPinB, CHANGE);
    }
}

// Função de atualização chamada pelas ISRs estáticas
void IRAM_ATTR Encoder::_update() {
    uint8_t s = _state & 3;
    if (digitalRead(_pinA)) s |= 4;
    if (digitalRead(_pinB)) s |= 8;
    
    switch (s) {
        case 0: case 5: case 10: case 15:
            break;
        case 1: case 7: case 8: case 14:
            _position++; break;
        case 2: case 4: case 11: case 13:
            _position--; break;
        case 3: case 12:
            _position += 2; break;
        default:
            _position -= 2; break;
    }
    _state = (s >> 2);
}

// ISR Estática para o pino A
void IRAM_ATTR Encoder::_isrPinA() {
    // Como não sabemos qual pino gerou a interrupção no ESP32,
    // precisamos chamar o update de todas as instâncias.
    // Uma abordagem mais otimizada seria verificar o pino, mas isso é mais complexo.
    // Para a maioria dos casos com poucos encoders, esta abordagem funciona bem.
    for (uint8_t i = 0; i < (sizeof(_instances) / sizeof(_instances[0])); ++i) {
        if (_instances[i] != nullptr) {
            _instances[i]->_update();
        }
    }
}

// ISR Estática para o pino B
void IRAM_ATTR Encoder::_isrPinB() {
    // Chama a mesma lógica da ISR do pino A
    _isrPinA();
}

long Encoder::read() {
    // Desabilita interrupções temporariamente para ler a variável de forma atômica
    noInterrupts();
    long ret = _position;
    interrupts();
    return ret;
}

void Encoder::write(long newPosition) {
    // Desabilita interrupções temporariamente para escrever na variável de forma atômica
    noInterrupts();
    _position = newPosition;
    interrupts();
}