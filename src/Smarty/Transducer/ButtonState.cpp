#include "ButtonState.hpp"

void SmartyButtonState::setButtonState(SmartyButtonState::State buttonState) {
  _buttonState = buttonState;
}

JsonObject& SmartyButtonState::toJson(JsonObject& rootJson) {
  JsonObject& stateJson = rootJson.createNestedObject("state");
  stateJson["button"] = (int) this->_buttonState;
  return stateJson;
}
