#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Smarty/Uptime.hpp"

using ::testing::Return;

class UptimeTest : public TestFixture {};

TEST_F(UptimeTest, testThatUpdateCallsMillisEachTime) {
  EXPECT_CALL(*TestFixture::arduinoMock, millis())
    .Times(5);

  SmartyUptime uptime;
  for (int i = 0; i < 5; i++) {
    uptime.update();
  }
}

TEST_F(UptimeTest, testThatUpdateIncreasesTimestamp) {
  EXPECT_CALL(*TestFixture::arduinoMock, millis())
    .WillOnce(Return(1000))
    .WillOnce(Return(2000));

  SmartyUptime uptime;
  uptime.update();
  uptime.update();

  EXPECT_EQ(uptime.getSeconds(), 2);
}

TEST_F(UptimeTest, testThatGetSecondsReturnsInSeconds) {
  EXPECT_CALL(*TestFixture::arduinoMock, millis())
    .WillOnce(Return(20000));

  SmartyUptime uptime;
  uptime.update();

  EXPECT_EQ(uptime.getSeconds(), 20);
}
