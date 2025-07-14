/* ____________________________
   This software is licensed under the MIT License:
   https://github.com/cifertech/nrfbox
   ________________________________________ */

#ifndef CONFIG_H
#define CONFIG_H

// =================================================================
// 1. INCLUDES DE BIBLIOTECAS
// Todas as bibliotecas necessárias para o projeto.
// =================================================================
#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#include <Preferences.h>
#include <vector>
#include <string>
#include <SD.h>
#include <Update.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <WiFi.h>
#include <esp_wifi.h>

// Inclui o arquivo de configurações/funções auxiliares
#include "setting.h"


// =================================================================
// 2. DEFINIÇÕES DE CONSTANTES (PINOS, ETC.)
// Valores que não mudam durante a execução.
// =================================================================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Pinos dos Botões
#define BUTTON_UP_PIN       26 
#define BUTTON_SELECT_PIN   33
#define BUTTON_DOWN_PIN     32 
#define BTN_PIN_RIGHT       27
#define BTN_PIN_LEFT        25

// Pino do Cartão SD
#define SD_CS_PIN 5
#define FIRMWARE_FILE "/firmware.bin"

// Pinos do nRF24
#define NRF_CE_PIN_A    5  
#define NRF_CSN_PIN_A   17 
#define NRF_CE_PIN_B    16  
#define NRF_CSN_PIN_B   4  
#define NRF_CE_PIN_C    15  
#define NRF_CSN_PIN_C   2  


// =================================================================
// 3. DECLARAÇÕES EXTERNAS (extern)
// Avisa ao compilador que esses objetos existem e serão definidos em outro lugar (no .ino).
// =================================================================
extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;
extern Adafruit_NeoPixel pixels;
extern bool neoPixelActive;
extern uint8_t oledBrightness;


// =================================================================
// 4. DECLARAÇÕES DE FUNÇÕES (PROTÓTIPOS)
// Avisa ao compilador sobre as funções que existem nos diferentes módulos.
// =================================================================

// BLE-related namespaces
namespace BleJammer {
  void blejammerSetup();
  void blejammerLoop();
}

namespace BleScan {
  void blescanSetup();
  void blescanLoop();
}

namespace SourApple {
  void sourappleSetup();
  void sourappleLoop();
}

namespace Spoofer {
  void spooferSetup();
  void spooferLoop();
}

// nRF24-related namespaces
namespace Analyzer {
  void analyzerSetup();
  void analyzerLoop();
}

namespace ProtoKill {
  void blackoutSetup();
  void blackoutLoop();
}

namespace Scanner {
  void scannerSetup();
  void scannerLoop();
}

namespace Jammer {
  void jammerSetup();
  void jammerLoop();
}

// WiFi-related namespaces
namespace WifiScan {
  void wifiscanSetup();
  void wifiscanLoop();
}

namespace Deauther {
  void deautherSetup();
  void deautherLoop();
}

#endif // CONFIG_H