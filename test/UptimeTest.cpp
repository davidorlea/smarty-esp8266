#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Smarty/Uptime.hpp"
using ::testing::Return;

TEST(UptimeTest, testThatUpdateCallsMillisEachTime) {
  EXPECT_CALL(Arduino, millis())
    .Times(5);

  SmartyUptime uptime;
  for (int i = 0; i < 5; i++) {
    uptime.update();
  }
}

TEST(UptimeTest, testThatUpdateIncreasesTimestamp) {
  EXPECT_CALL(Arduino, millis())
    .WillOnce(Return(1000))
    .WillOnce(Return(2000));

  SmartyUptime uptime;
  uptime.update();
  uptime.update();

  EXPECT_EQ(uptime.getSeconds(), 2);
}

TEST(UptimeTest, testThatGetSecondsReturnsInSeconds) {
  EXPECT_CALL(Arduino, millis())
    .WillOnce(Return(20000));

  SmartyUptime uptime;
  uptime.update();

  EXPECT_EQ(uptime.getSeconds(), 20);
}
