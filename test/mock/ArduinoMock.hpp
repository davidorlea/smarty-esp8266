#pragma once

#include <gmock/gmock.h>

class ArduinoMock {
public:
    MOCK_METHOD0(millis, unsigned long());
};

extern ArduinoMock Arduino;
