#include <gtest/gtest.h>
#include "Smarty/MqttPublication.hpp"

TEST(SmartyMqttPublicationTest, testThatValidSimpleTopicIsAccepted) {
  bool result1 = SmartyMqttPublication::isValidTopic("a/valid/topic");
  bool result2 = SmartyMqttPublication::isValidTopic("a");
  bool result3 = SmartyMqttPublication::isValidTopic("/");
  bool result4 = SmartyMqttPublication::isValidTopic("");

  EXPECT_TRUE(result1);
  EXPECT_TRUE(result2);
  EXPECT_TRUE(result3);
  EXPECT_TRUE(result4);
}

TEST(SmartyMqttPublicationTest, testThatSingleLevelTopicIsRejected) {
  bool result1 = SmartyMqttPublication::isValidTopic("an/invalid/topic/+");
  bool result2 = SmartyMqttPublication::isValidTopic("an/invalid/+/topic");
  bool result3 = SmartyMqttPublication::isValidTopic("an/invalid+/topic");
  bool result4 = SmartyMqttPublication::isValidTopic("an/+invalid/topic");
  bool result5 = SmartyMqttPublication::isValidTopic("+");

  EXPECT_FALSE(result1);
  EXPECT_FALSE(result2);
  EXPECT_FALSE(result3);
  EXPECT_FALSE(result4);
  EXPECT_FALSE(result5);
}

TEST(SmartyMqttPublicationTest, testThatMultiLevelTopicIsRejected) {
  bool result1 = SmartyMqttPublication::isValidTopic("an/invalid/topic/#");
  bool result2 = SmartyMqttPublication::isValidTopic("an/invalid/#/topic");
  bool result3 = SmartyMqttPublication::isValidTopic("an/invalid#/topic");
  bool result4 = SmartyMqttPublication::isValidTopic("an/#invalid/topic");
  bool result5 = SmartyMqttPublication::isValidTopic("#");

  EXPECT_FALSE(result1);
  EXPECT_FALSE(result2);
  EXPECT_FALSE(result3);
  EXPECT_FALSE(result4);
  EXPECT_FALSE(result5);
}

TEST(SmartyMqttPublicationTest, testThatConstructionFailsWithInvalidTopic) {

  EXPECT_CALL(Serial, print("ERROR: Invalid MQTT publication topic!"));
  EXPECT_CALL(Serial, println());
  EXPECT_DEATH({SmartyMqttPublication publication("an/invalid/+/topic");}, "");
}

TEST(SmartyMqttPublicationTest, testThatGetRetainInitiallyReturnsFalse) {
  SmartyMqttPublication publication("a/valid/topic");

  bool result = publication.getRetain();

  EXPECT_FALSE(result);
}

TEST(SmartyMqttPublicationTest, testThatGetRetainReturnsTrueWhenSet) {
  SmartyMqttPublication publication("a/valid/topic");
  publication.setRetain(true);

  bool result = publication.getRetain();

  EXPECT_TRUE(result);
}

TEST(SmartyMqttPublicationTest, testThatIsReadyInitiallyReturnsFalse) {
  SmartyMqttPublication publication("a/valid/topic");

  bool result = publication.isReady();

  EXPECT_FALSE(result);
}

TEST(SmartyMqttPublicationTest, testThatIsReadyReturnsFalseWhenMessageIsMissing) {
  SmartyMqttPublication publication("a/valid/topic");
  publication.ready();

  bool result = publication.isReady();

  EXPECT_FALSE(result);
}

TEST(SmartyMqttPublicationTest, testThatIsReadyReturnsFalseWhenReadyIsMissing) {
  SmartyMqttPublication publication("a/valid/topic");
  publication.setMessage("some message");

  bool result = publication.isReady();

  EXPECT_FALSE(result);
}

TEST(SmartyMqttPublicationTest, testThatIsReadyReturnsTrueWhenReadyAndTopicAndMessageAreSet) {
  SmartyMqttPublication publication("a/valid/topic");
  publication.setMessage("some message");
  publication.ready();

  bool result = publication.isReady();

  EXPECT_TRUE(result);
}

TEST(SmartyMqttPublicationTest, testThatIsReadyReturnsFalseWhenReadyIsRevoked) {
  SmartyMqttPublication publication("a/valid/topic");
  publication.setMessage("some message");
  publication.ready();
  publication.ready(false);

  bool result = publication.isReady();

  EXPECT_FALSE(result);
}


TEST(SmartyMqttPublicationListTest, testThatListIsInitiallyEmpty) {
  std::vector<SmartyMqttPublication*>* list = SmartyMqttPublication::getList();

  EXPECT_EQ(list->size(), 0);
}


TEST(SmartyMqttPublicationListTest, testThatPublicationIsAddedOnConstruction) {
  std::vector<SmartyMqttPublication*>* list = SmartyMqttPublication::getList();

  SmartyMqttPublication publication("a/valid/topic");

  EXPECT_EQ(list->size(), 1);
}

TEST(SmartyMqttPublicationListTest, testThatPublicationIsRemovedOnDestruction) {
  std::vector<SmartyMqttPublication*>* list = SmartyMqttPublication::getList();

  {
    SmartyMqttPublication publication("a/valid/topic");
  }

  EXPECT_EQ(list->size(), 0);
}
