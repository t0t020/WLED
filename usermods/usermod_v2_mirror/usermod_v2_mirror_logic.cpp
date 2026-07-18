#include "usermod_v2_mirror_logic.h"

void MirrorLogic::setup() {
    if (radarPin >= 0) pinMode(radarPin, INPUT);
    if (buttonPin >= 0) pinMode(buttonPin, INPUT);

    initDone = true;

    buttonState = false;
    radarState = false;
    lastMeas = millis();
}

void MirrorLogic::loop() {
    if (!initDone) return;
    if (!initDone) return;

    unsigned long currentTime = millis();

    if(!invertRadarType) {
        radarState = digitalRead(radarPin);
    } else {
        radarState = !digitalRead(radarPin);
    }

    if(!invertButtonType) {
        buttonState = digitalRead(buttonPin);
    } else {
        buttonState = !digitalRead(buttonPin);
    }

    int8_t activePreset = presetOFF;
    

    // Preset Logic
    if (buttonState){
        activePreset = presetON;
    } else if (radarState) {
        activePreset = presetRadar;
        lastRadarTriggerTime = currentTime;
    } else {
        if (currentTime - lastRadarTriggerTime < RadarRunTimeMS) {
            activePreset = presetRadar;
        } else {
            activePreset = presetOFF;
        }
    }

    if (activePreset != lastPreset) {
        applyPreset(activePreset);
        lastPreset = activePreset;
    }
}

void MirrorLogic::addToConfig(JsonObject& root) {
    // Eingabefelder im Browser
    JsonObject top = root.createNestedObject("MirrorLogic");
    top["Radar_GPIO"] = radarPin;
    top["Button_GPIO"] = buttonPin;
    top["invert_Radar"] = invertRadarType;
    top["invert_Button"] = invertButtonType;
    top["Preset_OFF"] = presetOFF;
    top["Preset_ON"] = presetON;
    top["Preset_Radar"] = presetRadar;
    top["Motion_RunTime_MS"] = RadarRunTimeMS;
}

bool MirrorLogic::readFromConfig(JsonObject& root) {
    JsonObject top = root["MirrorLogic"];
    
    // Defaults wenn noch keine Einstellung vorhanden
    if (top.isNull()) {
        USERMOD_LOG_LN("Keine Einstellungen gefunden. Nutze Defaults.");
        return false; 
    }

    // Werte aus dem JSON extrahieren
    int8_t oldRadarPin = radarPin;
    int8_t oldButtonPin = buttonPin;

    radarPin = top["Radar_GPIO"] | radarPin;
    buttonPin = top["Button_GPIO"] | buttonPin;
    invertRadarType = top["invert_Radar"] | invertRadarType;
    invertButtonType = ["invert_Button"] | invertButtonType;
    presetOFF = top["Preset_OFF"] | presetOFF;
    presetON = top["Preset_ON"] | presetON;
    presetRadar = top["Preset_Radar"] | presetRadar;
    RadarRunTimeMS = top["Motion_RunTime_MS"] | RadarRunTimeMS;
}

    // Falls sich die Pins im laufenden Betrieb geändert haben, Pins neu initialisieren
    if (initDone && (radarPin != oldRadarPin || buttonPin != oldButtonPin)) {
        setup(); 
    }

    return true;
}

uint16_t MirrorLogic::getId() {
    return USERMOD_ID_EXAMPLE; // Oder deine spezifische ID
}

REGISTER_USERMOD(MirrorLogic);
