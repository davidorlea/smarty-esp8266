#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Smarty/Transducer/BinaryActuator.hpp"

using ::testing::Return;

class BinaryActuatorTest : public TestFixture {};

TEST_F(BinaryActuatorTest, testThatSetupSetsPinToOutput) {
  EXPECT_CALL(*TestFixture::arduinoMock, pinMode(1, OUTPUT))
    .Times(1);

  SmartyBinaryActuator relay("relay", 1, SmartyBinaryActuator::Wiring::REGULAR);
  relay.setup();
}

TEST_F(BinaryActuatorTest, testThatSetupSetsInitialStateWithRegularWiring) {
  EXPECT_CALL(*TestFixture::arduinoMock, digitalWrite(1, LOW))
    .Times(1);

  SmartyBinaryActuator relay("relay", 1, SmartyBinaryActuator::Wiring::REGULAR);
  relay.setup();
}

TEST_F(BinaryActuatorTest, testThatSetupSetsInitialStateWithInverseWiring) {
  EXPECT_CALL(*TestFixture::arduinoMock, digitalWrite(1, HIGH))
    .Times(1);

  SmartyBinaryActuator relay("relay", 1, SmartyBinaryActuator::Wiring::INVERSE);
  relay.setup();
}

TEST_F(BinaryActuatorTest, testThatStateReturnsCurrentStateWithRegularWiring) {
  EXPECT_CALL(*TestFixture::arduinoMock, digitalRead(1))
    .Times(2)
    .WillOnce(Return(LOW))
    .WillOnce(Return(HIGH));

  SmartyBinaryActuator relay("relay", 1, SmartyBinaryActuator::Wiring::REGULAR);

  EXPECT_EQ(relay.state(), (uint8_t) SmartyBinaryActuator::State::OFF);
  EXPECT_EQ(relay.state(), (uint8_t) SmartyBinaryActuator::State::ON);
}

TEST_F(BinaryActuatorTest, testThatStateReturnsStateWithInverseWiring) {
  EXPECT_CALL(*TestFixture::arduinoMock, digitalRead(1))
    .Times(2)
    .WillOnce(Return(HIGH))
    .WillOnce(Return(LOW));

  SmartyBinaryActuator relay("relay", 1, SmartyBinaryActuator::Wiring::INVERSE);

  EXPECT_EQ(relay.state(), (uint8_t) SmartyBinaryActuator::State::OFF);
  EXPECT_EQ(relay.state(), (uint8_t) SmartyBinaryActuator::State::ON);
}

TEST_F(BinaryActuatorTest, testThatActivationSetsStateWithRegularWiring) {
  EXPECT_CALL(*TestFixture::arduinoMock, digitalWrite(1, HIGH))
    .Times(1);

  SmartyBinaryActuator relay("relay", 1, SmartyBinaryActuator::Wiring::REGULAR);
  relay.activate();
}

TEST_F(BinaryActuatorTest, testThatActivationSetsStateWithInverseWiring) {
  EXPECT_CALL(*TestFixture::arduinoMock, digitalWrite(1, LOW))
    .Times(1);

  SmartyBinaryActuator relay("relay", 1, SmartyBinaryActuator::Wiring::INVERSE);
  relay.activate();
}

TEST_F(BinaryActuatorTest, testThatActivationReturnsTrueWhenStateIsChanged) {
  EXPECT_CALL(*TestFixture::arduinoMock, digitalRead(1))
    .Times(1)
    .WillOnce(Return(LOW));
  EXPECT_CALL(*TestFixture::arduinoMock, digitalWrite(1, HIGH))
    .Times(1);

  SmartyBinaryActuator relay("relay", 1, SmartyBinaryActuator::Wiring::REGULAR);

  EXPECT_TRUE(relay.activate());
}

TEST_F(BinaryActuatorTest, testThatActivationReturnsFalseWhenStateIsNotChanged) {
  EXPECT_CALL(*TestFixture::arduinoMock, digitalRead(1))
    .Times(1)
    .WillOnce(Return(HIGH));
  EXPECT_CALL(*TestFixture::arduinoMock, digitalWrite(1, HIGH))
    .Times(1);

  SmartyBinaryActuator relay("relay", 1, SmartyBinaryActuator::Wiring::REGULAR);

  EXPECT_FALSE(relay.activate());
}

TEST_F(BinaryActuatorTest, testThatDeactivationSetsStateWithRegularWiring) {
  EXPECT_CALL(*TestFixture::arduinoMock, digitalWrite(1, LOW))
    .Times(1);

  SmartyBinaryActuator relay("relay", 1, SmartyBinaryActuator::Wiring::REGULAR);
  relay.deactivate();
}

TEST_F(BinaryActuatorTest, testThatDeactivationSetsStateWithInverseWiring) {
  EXPECT_CALL(*TestFixture::arduinoMock, digitalWrite(1, HIGH))
    .Times(1);

  SmartyBinaryActuator relay("relay", 1, SmartyBinaryActuator::Wiring::INVERSE);
  relay.deactivate();
}

TEST_F(BinaryActuatorTest, testThatDeactivationReturnsTrueWhenStateIsChanged) {
  EXPECT_CALL(*TestFixture::arduinoMock, digitalRead(1))
    .Times(1)
    .WillOnce(Return(HIGH));
  EXPECT_CALL(*TestFixture::arduinoMock, digitalWrite(1, LOW))
    .Times(1);

  SmartyBinaryActuator relay("relay", 1, SmartyBinaryActuator::Wiring::REGULAR);

  EXPECT_TRUE(relay.deactivate());
}

TEST_F(BinaryActuatorTest, testThatDeactivationReturnsFalseWhenStateIsNotChanged) {
  EXPECT_CALL(*TestFixture::arduinoMock, digitalRead(1))
    .Times(1)
    .WillOnce(Return(LOW));
  EXPECT_CALL(*TestFixture::arduinoMock, digitalWrite(1, LOW))
    .Times(1);

  SmartyBinaryActuator relay("relay", 1, SmartyBinaryActuator::Wiring::REGULAR);

  EXPECT_FALSE(relay.deactivate());
}

TEST_F(BinaryActuatorTest, testThatToggleSetsActivationStateWithRegularWiring) {
  EXPECT_CALL(*TestFixture::arduinoMock, digitalRead(1))
    .Times(2)
    .WillRepeatedly(Return(LOW));
  EXPECT_CALL(*TestFixture::arduinoMock, digitalWrite(1, HIGH))
    .Times(1);

  SmartyBinaryActuator relay("relay", 1, SmartyBinaryActuator::Wiring::REGULAR);
  relay.toggle();
}

TEST_F(BinaryActuatorTest, testThatToggleSetsActivationStateWithInverseWiring) {
  EXPECT_CALL(*TestFixture::arduinoMock, digitalRead(1))
    .Times(2)
    .WillRepeatedly(Return(HIGH));
  EXPECT_CALL(*TestFixture::arduinoMock, digitalWrite(1, LOW))
    .Times(1);

  SmartyBinaryActuator relay("relay", 1, SmartyBinaryActuator::Wiring::INVERSE);
  relay.toggle();
}

TEST_F(BinaryActuatorTest, testThatToggleSetsDeactivationStateWithRegularWiring) {
  EXPECT_CALL(*TestFixture::arduinoMock, digitalRead(1))
    .Times(2)
    .WillRepeatedly(Return(HIGH));
  EXPECT_CALL(*TestFixture::arduinoMock, digitalWrite(1, LOW))
    .Times(1);

  SmartyBinaryActuator relay("relay", 1, SmartyBinaryActuator::Wiring::REGULAR);
  relay.toggle();
}

TEST_F(BinaryActuatorTest, testThatToggleSetsDeactivationStateWithInverseWiring) {
  EXPECT_CALL(*TestFixture::arduinoMock, digitalRead(1))
    .Times(2)
    .WillRepeatedly(Return(LOW));
  EXPECT_CALL(*TestFixture::arduinoMock, digitalWrite(1, HIGH))
    .Times(1);

  SmartyBinaryActuator relay("relay", 1, SmartyBinaryActuator::Wiring::INVERSE);
  relay.toggle();
}
