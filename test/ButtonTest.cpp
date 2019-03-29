#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Smarty/Transducer/Button.hpp"

using ::testing::Return;

class ButtonTest : public TestFixture {};

TEST_F(ButtonTest, testThatSetupSetsPinToInputPullup) {
  EXPECT_CALL(*TestFixture::arduinoMock, pinMode(1, INPUT_PULLUP))
    .Times(1);

  SmartyButton button("button", 1, SmartyButton::Mode::PUSH);
  button.setup();
}

TEST_F(ButtonTest, testThatSetupReadsInitialState) {
  EXPECT_CALL(*TestFixture::arduinoMock, digitalRead(1))
    .Times(1);

  SmartyButton button("button", 1, SmartyButton::Mode::PUSH);
  button.setup();
}

TEST_F(ButtonTest, testThatStateReturnsCurrentState) {
  EXPECT_CALL(*TestFixture::arduinoMock, digitalRead(1))
    .Times(2)
    .WillOnce(Return(0))
    .WillOnce(Return(1));

  SmartyButton button("button", 1, SmartyButton::Mode::PUSH);

  EXPECT_EQ(button.state(), 0);
  EXPECT_EQ(button.state(), 1);
}

TEST_F(ButtonTest, testThatButtonChangeIsRecognizedWhenLongEnough) {
  bool called = false;
  EXPECT_CALL(*TestFixture::arduinoMock, digitalRead(1))
    .Times(3)
    .WillOnce(Return(0))
    .WillOnce(Return(1))
    .WillOnce(Return(1));
  EXPECT_CALL(*TestFixture::arduinoMock, millis())
    .Times(2)
    .WillOnce(Return(0))
    .WillOnce(Return(51));

  SmartyButton button("button", 1, SmartyButton::Mode::SWITCH);
  button.addStateCallback([&called](uint8_t state) {
    called = true;
  });

  button.setup();
  button.loop();
  button.loop();

  EXPECT_TRUE(called);
}

TEST_F(ButtonTest, testThatButtonChangeIsIgnoredWhenTooShort) {
  bool called = false;
  EXPECT_CALL(*TestFixture::arduinoMock, digitalRead(1))
    .Times(3)
    .WillOnce(Return(0))
    .WillOnce(Return(1))
    .WillOnce(Return(1));
  EXPECT_CALL(*TestFixture::arduinoMock, millis())
    .Times(2)
    .WillOnce(Return(0))
    .WillOnce(Return(49));

  SmartyButton button("button", 1, SmartyButton::Mode::SWITCH);
  button.addStateCallback([&called](uint8_t state) {
    called = true;
  });

  button.setup();
  button.loop();
  button.loop();

  EXPECT_FALSE(called);
}
