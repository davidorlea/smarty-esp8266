#include <gtest/gtest.h>
#include "Smarty/MqttSubscription.hpp"

TEST(SmartyMqttSubscriptionTest, testThatValidSimpleTopicIsAccepted) {
  bool result = SmartyMqttSubscription::isValidTopic("a/valid/topic");

  EXPECT_TRUE(result);
}

TEST(SmartyMqttSubscriptionTest, testThatValidSingleLevelTopicIsAccepted) {
  bool result1 = SmartyMqttSubscription::isValidTopic("a/valid/+/topic");
  bool result2 = SmartyMqttSubscription::isValidTopic("a/valid/topic/+");

  EXPECT_TRUE(result1);
  EXPECT_TRUE(result2);
}

TEST(SmartyMqttSubscriptionTest, testThatValidMultiLevelTopicIsAccepted) {
  bool result = SmartyMqttSubscription::isValidTopic("a/valid/topic/#");

  EXPECT_TRUE(result);
}

TEST(SmartyMqttSubscriptionTest, testThatInvalidSingleLevelTopicIsRejected) {
  bool result1 = SmartyMqttSubscription::isValidTopic("an/invalid+/topic");
  bool result2 = SmartyMqttSubscription::isValidTopic("an/+invalid/topic");

  EXPECT_FALSE(result1);
  EXPECT_FALSE(result2);
}

TEST(SmartyMqttSubscriptionTest, testThatInvalidMultiLevelTopicIsRejected) {
  bool result1 = SmartyMqttSubscription::isValidTopic("an/invalid/#/topic");
  bool result2 = SmartyMqttSubscription::isValidTopic("an/invalid#/topic");
  bool result3 = SmartyMqttSubscription::isValidTopic("an/#invalid/topic");

  EXPECT_FALSE(result1);
  EXPECT_FALSE(result2);
  EXPECT_FALSE(result3);
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

TEST(SmartyMqttSubscriptionTest, testThatCanHandleReturnsFalseForUnknownTopic) {
  SmartyMqttSubscription subscription("a/valid/topic");

  bool result = subscription.canHandle("a/different/topic");

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

TEST(SmartyMqttSubscriptionListTest, testThatListIsInitiallyEmpty) {
  std::vector<SmartyMqttSubscription*>* list = SmartyMqttSubscription::getList();

  EXPECT_EQ(list->size(), 0);
}


TEST(SmartyMqttSubscriptionListTest, testThatNewSubscriptionIsAdded) {
  std::vector<SmartyMqttSubscription*>* list = SmartyMqttSubscription::getList();

  SmartyMqttSubscription subscription("a/valid/topic");

  EXPECT_EQ(list->size(), 1);
}

TEST(SmartyMqttSubscriptionListTest, testThatSubscriptionIsRemovedOnDestruction) {
  std::vector<SmartyMqttSubscription*>* list = SmartyMqttSubscription::getList();

  {
    SmartyMqttSubscription subscription("a/valid/topic");
  }

  EXPECT_EQ(list->size(), 0);
}
