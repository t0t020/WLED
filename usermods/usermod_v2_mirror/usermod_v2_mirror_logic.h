#pragma once
#include "../../wled00/wled.h"

class MirrorLogic : public Usermod {
private:
    // Variables
    bool buttonState = false;
    bool radarState = false;
    bool lastButtonState = false;
    bool lastRadarState = false;
    bool initDone = false;

    int8_t radarPin = 12;   // GPIO12 (D6)
    int8_t buttonPin = 16;  // GPIO16 (D0)
    bool invertRadarType = false;
    bool invertButtonType = false;

    // Presets
    int8_t presetOFF = 1;
    int8_t presetON = 2;
    int8_t presetRadar = 3;

    int8_t lastPreset = 0;


    unsigned long lastRadarTriggerTime = 0;
    unsigned long RadarRunTimeMS = 30000;

public:
    // Die Core-Methoden von WLED (nur deklariert!)
    void setup() override;
    void loop() override;

    void addToConfig(JsonObject& root) override;
    bool readFromConfig(JsonObject& root) override;

    uint16_t getId() override {
        return USERMOD_ID_EXAMPLE; 
    }
};
