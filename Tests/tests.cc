#include "../Utils/utils.h"
#include "../Utils/utils.cpp"
#include <cctype>
#include <gtest/gtest.h>

TEST(tryParsePositiveInt, error)
{
  EXPECT_EQ(tryParsePositiveInt("sefse"), 0);
  EXPECT_EQ(tryParsePositiveInt("-0001"), 0);
  EXPECT_EQ(tryParsePositiveInt("0efef001"), 0);
  EXPECT_EQ(tryParsePositiveInt("0euu001"), 0);
  EXPECT_EQ(tryParsePositiveInt("-0euu001"), 0);
  EXPECT_EQ(tryParsePositiveInt("-0ui"), 0);
  EXPECT_EQ(tryParsePositiveInt("-012313"), 0);
  EXPECT_EQ(tryParsePositiveInt("2310237129871892"), 0);
  EXPECT_EQ(tryParsePositiveInt(""), 0);
}

TEST(tryParsePositiveInt, success)
{
  EXPECT_EQ(tryParsePositiveInt("10012313"), 1);
  EXPECT_EQ(tryParsePositiveInt("0001"), 1);
  EXPECT_EQ(tryParsePositiveInt("100"), 1);
  EXPECT_EQ(tryParsePositiveInt("1"), 1);
}

TEST(hasNonNumericChars, true)
{
  EXPECT_EQ(hasNonNumericChars("A"), true);
  EXPECT_EQ(hasNonNumericChars("A434   efw32"), true);
  EXPECT_EQ(hasNonNumericChars("-12312"), true);
}

TEST(hasNonNumericChars, false)
{
  EXPECT_EQ(hasNonNumericChars("123123"), false);
  EXPECT_EQ(hasNonNumericChars(""), false);
}

TEST(splitString, different_stuff)
{
  vector<string> res = {"12", "12"};
  EXPECT_EQ(splitString("12:12", ":"), res);
  res = {"1212"};
  EXPECT_EQ(splitString("1212", ":"), res);
  res = {"a"};
  EXPECT_EQ(splitString("a", ":"), res);
  res = {"12", "12"};
  EXPECT_EQ(splitString("12:12:", ":"), res);
  res = {"", "12", "12"};
  EXPECT_EQ(splitString(":12:12:", ":"), res);
}

TEST(checkTime, valid)
{
  EXPECT_EQ(checkTime("12:12"), 732);
  EXPECT_EQ(checkTime("10:59"), 659);
  EXPECT_EQ(checkTime("00:00"), 0);
}

TEST(checkTime, invalid)
{
  EXPECT_EQ(checkTime("a:a"), -1);
  EXPECT_EQ(checkTime("a:"), -1);
  EXPECT_EQ(checkTime(":a"), -1);
  EXPECT_EQ(checkTime("01:a"), -1);
  EXPECT_EQ(checkTime("01:a"), -1);
  EXPECT_EQ(checkTime("1:a"), -1);
  EXPECT_EQ(checkTime(""), -1);
  EXPECT_EQ(checkTime(":"), -1);
  EXPECT_EQ(checkTime("12:"), -1);
  EXPECT_EQ(checkTime("-2:2"), -1);
  EXPECT_EQ(checkTime("25:02"), -1);
  EXPECT_EQ(checkTime("23:62"), -1);
  EXPECT_EQ(checkTime("25:62"), -1);
}

TEST(checkClientName, true)
{
  string s = "sefsf";
  EXPECT_EQ(checkClientName(s), true);
  s = "sefsf_1234";
  EXPECT_EQ(checkClientName(s), true);
  s = "1234";
  EXPECT_EQ(checkClientName(s), true);
  s = "___";
  EXPECT_EQ(checkClientName(s), true);
  s = "hii";
  EXPECT_EQ(checkClientName(s), true);
  s = "123_";
  EXPECT_EQ(checkClientName(s), true);
  s = "srgrd_____";
  EXPECT_EQ(checkClientName(s), true);
}

TEST(checkClientName, false)
{
  string s = "";
  EXPECT_EQ(checkClientName(s), false);
  s = "And";
  EXPECT_EQ(checkClientName(s), false);
  s = "sesefseJ";
  EXPECT_EQ(checkClientName(s), false);
  s = "sfsef+";
  EXPECT_EQ(checkClientName(s), false);
  s = "fksehfskef1234567====";
  EXPECT_EQ(checkClientName(s), false);
    s = "sesef*-/ssefsf";
  EXPECT_EQ(checkClientName(s), false);
    s = "====";
  EXPECT_EQ(checkClientName(s), false);
    s = "123AAAA";
  EXPECT_EQ(checkClientName(s), false);

}


TEST(getTimeFromMins, eh) {

  string time = "10:59";
  int t = checkTime(time);  
  string time1 = "00:00";
  int t1 = checkTime(time1);
  string time2 = "23:59";
  int t2 = checkTime(time2);
  string time3 = "14:14";
  int t3 = checkTime(time3);
  string time4 = "18:41";
  int t4 = checkTime(time4);
  string time5 = "03:09";
  int t5 = checkTime(time5);
  string time6 = "10:00";
  int t6 = checkTime(time6);
  string time7 = "11:39";
  int t7 = checkTime(time7);
  string time8 = "17:57";
  int t8 = checkTime(time8);
  string time9 = "01:01";
  int t9 = checkTime(time9);


  EXPECT_EQ(getTimeFromMins(t), time);
  EXPECT_EQ(getTimeFromMins(t1), time1);
  EXPECT_EQ(getTimeFromMins(t2), time2);
  EXPECT_EQ(getTimeFromMins(t3), time3);
  EXPECT_EQ(getTimeFromMins(t4), time4);
  EXPECT_EQ(getTimeFromMins(t5), time5);
  EXPECT_EQ(getTimeFromMins(t6), time6);
  EXPECT_EQ(getTimeFromMins(t7), time7);
  EXPECT_EQ(getTimeFromMins(t8), time8);
  EXPECT_EQ(getTimeFromMins(t9), time9);
}
