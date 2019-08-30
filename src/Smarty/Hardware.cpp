#include "Hardware.hpp"

JsonObject& SmartyHardware::toJson(JsonObject& rootJson) {
  char chipId[6 + 1];
  snprintf(chipId, sizeof(chipId), "%06x", ESP.getChipId());
  char flashChipId[6 + 1];
  snprintf(flashChipId, sizeof(flashChipId), "%06x", ESP.getFlashChipId());

  JsonObject& chipJson = rootJson.createNestedObject("chip");
  chipJson["id"] =  chipId;
  chipJson["speed"] = ESP.getCpuFreqMHz() * 1000000L;
  JsonObject& flashJson = rootJson.createNestedObject("flash");
  flashJson["id"] = flashChipId;
  flashJson["realSize"] = ESP.getFlashChipRealSize();
  flashJson["sdkSize"] = ESP.getFlashChipSize();
  flashJson["speed"] = ESP.getFlashChipSpeed();
  JsonObject& heapJson = rootJson.createNestedObject("heap");
  heapJson["free"] = ESP.getFreeHeap();

  return rootJson;
}
