#include <gtest/gtest.h>
#include "Smarty/MqttSubscription.hpp"

class SmartyMqttSubscriptionTest : public TestFixture {};

TEST_F(SmartyMqttSubscriptionTest, testThatValidSimpleTopicIsAccepted) {
  bool result1 = SmartyMqttSubscription::isValidTopic("a/valid/topic");
  bool result2 = SmartyMqttSubscription::isValidTopic("a");
  bool result3 = SmartyMqttSubscription::isValidTopic("/");
  bool result4 = SmartyMqttSubscription::isValidTopic("");

  EXPECT_TRUE(result1);
  EXPECT_TRUE(result2);
  EXPECT_TRUE(result3);
  EXPECT_TRUE(result4);
}

TEST_F(SmartyMqttSubscriptionTest, testThatValidSingleLevelTopicIsAccepted) {
  bool result1 = SmartyMqttSubscription::isValidTopic("a/valid/+/topic/+");
  bool result2 = SmartyMqttSubscription::isValidTopic("a/valid/+/topic");
  bool result3 = SmartyMqttSubscription::isValidTopic("a/valid/topic/+");
  bool result4 = SmartyMqttSubscription::isValidTopic("+");

  EXPECT_TRUE(result1);
  EXPECT_TRUE(result2);
  EXPECT_TRUE(result3);
  EXPECT_TRUE(result4);
}

TEST_F(SmartyMqttSubscriptionTest, testThatValidMultiLevelTopicIsAccepted) {
  bool result1 = SmartyMqttSubscription::isValidTopic("a/valid/topic/#");
  bool result2 = SmartyMqttSubscription::isValidTopic("#");

  EXPECT_TRUE(result1);
  EXPECT_TRUE(result2);
}

TEST_F(SmartyMqttSubscriptionTest, testThatInvalidSingleLevelTopicIsRejected) {
  bool result1 = SmartyMqttSubscription::isValidTopic("an/invalid+/topic");
  bool result2 = SmartyMqttSubscription::isValidTopic("an/+invalid/topic");

  EXPECT_FALSE(result1);
  EXPECT_FALSE(result2);
}

TEST_F(SmartyMqttSubscriptionTest, testThatInvalidMultiLevelTopicIsRejected) {
  bool result1 = SmartyMqttSubscription::isValidTopic("an/invalid/topic/#/#");
  bool result2 = SmartyMqttSubscription::isValidTopic("an/invalid/#/topic");
  bool result3 = SmartyMqttSubscription::isValidTopic("an/invalid#/topic");
  bool result4 = SmartyMqttSubscription::isValidTopic("an/#invalid/topic");

  EXPECT_FALSE(result1);
  EXPECT_FALSE(result2);
  EXPECT_FALSE(result3);
  EXPECT_FALSE(result4);
}

TEST_F(SmartyMqttSubscriptionTest, testThatConstructionFailsWithInvalidTopic) {

  EXPECT_CALL(Serial, print("ERROR: Invalid MQTT subscription topic!"));
  EXPECT_CALL(Serial, println());
  EXPECT_DEATH({SmartyMqttSubscription subscription("a/valid/#/topic");}, "");
}

TEST_F(SmartyMqttSubscriptionTest, testThatCanHandleReturnsTrueForSubscriptionTopic) {
  SmartyMqttSubscription subscription("a/valid/topic");

  bool result = subscription.canHandle("a/valid/topic");

  EXPECT_TRUE(result);
}

TEST_F(SmartyMqttSubscriptionTest, testThatCanHandleReturnsFalseForMismatchingTopic) {
  SmartyMqttSubscription subscription("a/valid/topic");

  bool result = subscription.canHandle("a/mismatching/topic");

  EXPECT_FALSE(result);
}

TEST_F(SmartyMqttSubscriptionTest, testThatCanHandleReturnsTrueForMatchingSingleLevelTopic) {
  SmartyMqttSubscription subscription("a/+/valid/topic");

  bool result1 = subscription.canHandle("a/first/valid/topic");
  bool result2 = subscription.canHandle("a/second/valid/topic");

  EXPECT_TRUE(result1);
  EXPECT_TRUE(result2);
}

TEST_F(SmartyMqttSubscriptionTest, testThatCanHandleReturnsFalseForMismatchingSingleLevelTopic) {
  SmartyMqttSubscription subscription("a/+/valid/topic");

  bool result1 = subscription.canHandle("a/first/mismatching/topic");
  bool result2 = subscription.canHandle("a/second/mismatching/topic");

  EXPECT_FALSE(result1);
  EXPECT_FALSE(result2);
}

TEST_F(SmartyMqttSubscriptionTest, testThatCanHandleReturnsTrueForMatchingSuffixSingleLevelTopic) {
  SmartyMqttSubscription subscription("a/valid/topic/+");

  bool result = subscription.canHandle("a/valid/topic/first");

  EXPECT_TRUE(result);
}

TEST_F(SmartyMqttSubscriptionTest, testThatCanHandleReturnsFalseForMismatchingSuffixSingleLevelTopic) {
  SmartyMqttSubscription subscription("a/valid/topic/+");

  bool result1 = subscription.canHandle("a/valid/topic");
  bool result2 = subscription.canHandle("a/valid/topic/first/second");

  EXPECT_FALSE(result1);
  EXPECT_FALSE(result2);
}

TEST_F(SmartyMqttSubscriptionTest, testThatCanHandleReturnsTrueForMatchingMultiLevelTopic) {
  SmartyMqttSubscription subscription("a/valid/topic/#");

  bool result1 = subscription.canHandle("a/valid/topic");
  bool result2 = subscription.canHandle("a/valid/topic/first");
  bool result3 = subscription.canHandle("a/valid/topic/and/second");

  EXPECT_TRUE(result1);
  EXPECT_TRUE(result2);
  EXPECT_TRUE(result3);
}

TEST_F(SmartyMqttSubscriptionTest, testThatCanHandleReturnsFalseForMismatchingMultiLevelTopic) {
  SmartyMqttSubscription subscription("a/valid/topic/#");

  bool result = subscription.canHandle("a/first/valid/topic");

  EXPECT_FALSE(result);
}

TEST_F(SmartyMqttSubscriptionTest, testThatHandleCallsCallbackWithCorrectArguments) { // NOLINT(cert-err58-cpp)
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
