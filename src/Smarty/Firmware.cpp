#include "Firmware.hpp"

JsonObject& SmartyFirmware::toJson(JsonObject& rootJson) {
  JsonObject& firmwareJson = rootJson.createNestedObject("firmware");
  firmwareJson["version"] = version;
  firmwareJson["buildTime"] = buildTime;
  JsonObject& arduinoJson = firmwareJson.createNestedObject("arduino");
  arduinoJson["sdkVersion"] = ESP.getSdkVersion();
  arduinoJson["coreVersion"] = ESP.getCoreVersion();
  arduinoJson["fullVersion"] = ESP.getFullVersion();
  return firmwareJson;
}
