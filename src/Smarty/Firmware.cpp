#include "Firmware.hpp"

JsonObject& SmartyFirmware::toJson(JsonObject& rootJson) {
  JsonObject& firmwareJson = rootJson.createNestedObject("firmware");
  firmwareJson["version"] = version;
  firmwareJson["buildTime"] = buildTime;
  return firmwareJson;
}
