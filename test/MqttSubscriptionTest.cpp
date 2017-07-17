#include <gtest/gtest.h>
#include "Smarty/MqttSubscription.hpp"

TEST(SmartyMqttSubscriptionTest, testThatValidSimpleTopicIsAccepted) {
  bool result1 = SmartyMqttSubscription::isValidTopic("a/valid/topic");
  bool result2 = SmartyMqttSubscription::isValidTopic("a");
  bool result3 = SmartyMqttSubscription::isValidTopic("/");
  bool result4 = SmartyMqttSubscription::isValidTopic("");

  EXPECT_TRUE(result1);
  EXPECT_TRUE(result2);
  EXPECT_TRUE(result3);
  EXPECT_TRUE(result4);
}

TEST(SmartyMqttSubscriptionTest, testThatValidSingleLevelTopicIsAccepted) {
  bool result1 = SmartyMqttSubscription::isValidTopic("a/valid/+/topic/+");
  bool result2 = SmartyMqttSubscription::isValidTopic("a/valid/+/topic");
  bool result3 = SmartyMqttSubscription::isValidTopic("a/valid/topic/+");
  bool result4 = SmartyMqttSubscription::isValidTopic("+");

  EXPECT_TRUE(result1);
  EXPECT_TRUE(result2);
  EXPECT_TRUE(result3);
  EXPECT_TRUE(result4);
}

TEST(SmartyMqttSubscriptionTest, testThatValidMultiLevelTopicIsAccepted) {
  bool result1 = SmartyMqttSubscription::isValidTopic("a/valid/topic/#");
  bool result2 = SmartyMqttSubscription::isValidTopic("#");

  EXPECT_TRUE(result1);
  EXPECT_TRUE(result2);
}

TEST(SmartyMqttSubscriptionTest, testThatInvalidSingleLevelTopicIsRejected) {
  bool result1 = SmartyMqttSubscription::isValidTopic("an/invalid+/topic");
  bool result2 = SmartyMqttSubscription::isValidTopic("an/+invalid/topic");

  EXPECT_FALSE(result1);
  EXPECT_FALSE(result2);
}

TEST(SmartyMqttSubscriptionTest, testThatInvalidMultiLevelTopicIsRejected) {
  bool result1 = SmartyMqttSubscription::isValidTopic("an/invalid/topic/#/#");
  bool result2 = SmartyMqttSubscription::isValidTopic("an/invalid/#/topic");
  bool result3 = SmartyMqttSubscription::isValidTopic("an/invalid#/topic");
  bool result4 = SmartyMqttSubscription::isValidTopic("an/#invalid/topic");

  EXPECT_FALSE(result1);
  EXPECT_FALSE(result2);
  EXPECT_FALSE(result3);
  EXPECT_FALSE(result4);
}

TEST(SmartyMqttSubscriptionTest, testThatConstructionFailsWithInvalidTopic) {

  EXPECT_CALL(Serial, print("ERROR: Invalid MQTT subscription topic!"));
  EXPECT_CALL(Serial, println());
  EXPECT_DEATH({SmartyMqttSubscription subscription("a/valid/#/topic");}, "");
}

TEST(SmartyMqttSubscriptionTest, testThatCanHandleReturnsTrueForSubscriptionTopic) {
  SmartyMqttSubscription subscription("a/valid/topic");

  bool result = subscription.canHandle("a/valid/topic");

  EXPECT_TRUE(result);
}

TEST(SmartyMqttSubscriptionTest, testThatCanHandleReturnsFalseForMismatchingTopic) {
  SmartyMqttSubscription subscription("a/valid/topic");

  bool result = subscription.canHandle("a/mismatching/topic");

  EXPECT_FALSE(result);
}

TEST(SmartyMqttSubscriptionTest, testThatCanHandleReturnsTrueForMatchingSingleLevelTopic) {
  SmartyMqttSubscription subscription("a/+/valid/topic");

  bool result1 = subscription.canHandle("a/first/valid/topic");
  bool result2 = subscription.canHandle("a/second/valid/topic");

  EXPECT_TRUE(result1);
  EXPECT_TRUE(result2);
}

TEST(SmartyMqttSubscriptionTest, testThatCanHandleReturnsFalseForMismatchingSingleLevelTopic) {
  SmartyMqttSubscription subscription("a/+/valid/topic");

  bool result1 = subscription.canHandle("a/first/mismatching/topic");
  bool result2 = subscription.canHandle("a/second/mismatching/topic");

  EXPECT_FALSE(result1);
  EXPECT_FALSE(result2);
}

TEST(SmartyMqttSubscriptionTest, testThatCanHandleReturnsTrueForMatchingSuffixSingleLevelTopic) {
  SmartyMqttSubscription subscription("a/valid/topic/+");

  bool result = subscription.canHandle("a/valid/topic/first");

  EXPECT_TRUE(result);
}

TEST(SmartyMqttSubscriptionTest, testThatCanHandleReturnsFalseForMismatchingSuffixSingleLevelTopic) {
  SmartyMqttSubscription subscription("a/valid/topic/+");

  bool result1 = subscription.canHandle("a/valid/topic");
  bool result2 = subscription.canHandle("a/valid/topic/first/second");

  EXPECT_FALSE(result1);
  EXPECT_FALSE(result2);
}

TEST(SmartyMqttSubscriptionTest, testThatCanHandleReturnsTrueForMatchingMultiLevelTopic) {
  SmartyMqttSubscription subscription("a/valid/topic/#");

  bool result1 = subscription.canHandle("a/valid/topic");
  bool result2 = subscription.canHandle("a/valid/topic/first");
  bool result3 = subscription.canHandle("a/valid/topic/and/second");

  EXPECT_TRUE(result1);
  EXPECT_TRUE(result2);
  EXPECT_TRUE(result3);
}

TEST(SmartyMqttSubscriptionTest, testThatCanHandleReturnsFalseForMismatchingMultiLevelTopic) {
  SmartyMqttSubscription subscription("a/valid/topic/#");

  bool result = subscription.canHandle("a/first/valid/topic");

  EXPECT_FALSE(result);
}

TEST(SmartyMqttSubscriptionTest, testThatHandleCallsCallbackWithCorrectArguments) {
  const char* calledTopic;
  const char* calledMessage;

  SmartyMqttSubscription subscription("a/valid/topic");

  subscription.setCallback([&calledTopic, &calledMessage](const char* topic, const char* message) {
    calledTopic = topic;
    calledMessage = message;
  });

  subscription.handle("some/topic", "some message");

  EXPECT_STREQ(calledTopic, "some/topic");
  EXPECT_STREQ(calledMessage, "some message");
}
