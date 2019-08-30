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
  heapJson["total"] = _getTotalMemory();
  heapJson["totalUsed"] = _getTotalUsedMemory();
  heapJson["totalFree"] = _getTotalAvailableMemory();
  heapJson["largestFreeBlock"] = _getLargestAvailableBlock();
  heapJson["fragmentation"] = _getFragmentation();

  return rootJson;
}

size_t SmartyHardware::_getTotalMemory() {
  umm_info(nullptr, 0);
  return ummHeapInfo.totalBlocks * _blockSize;
}

size_t SmartyHardware::_getTotalUsedMemory() {
  umm_info(nullptr, 0);
  return ummHeapInfo.usedBlocks * _blockSize;
}

size_t SmartyHardware::_getTotalAvailableMemory() {
  umm_info(nullptr, 0);
  return ummHeapInfo.freeBlocks * _blockSize;
}

size_t SmartyHardware::_getLargestAvailableBlock() {
  umm_info(nullptr, 0);
  return ummHeapInfo.maxFreeContiguousBlocks * _blockSize;
}
