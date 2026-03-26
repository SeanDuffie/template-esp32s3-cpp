#pragma once

#include <ArduinoOTA.h>
#include <HTTPUpdate.h>
#include <HTTPClient.h>
#include <Arduino.h>

void setup_ota();
void perform_ota_update();