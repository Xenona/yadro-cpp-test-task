#include "./Utils/utils.cpp"
#include <cctype>
#include <gtest/gtest.h>

TEST(tryParsePositiveInt, tryParsePositiveInt)
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
  EXPECT_EQ(tryParsePositiveInt("10012313"), 1);
  EXPECT_EQ(tryParsePositiveInt("0001"), 1);
  EXPECT_EQ(tryParsePositiveInt("100"), 1);
}

TEST(hasNonNumericChars, hasNonNumericChars)
{
  EXPECT_EQ(hasNonNumericChars("A"), true);
  EXPECT_EQ(hasNonNumericChars("A434   efw32"), true);
  EXPECT_EQ(hasNonNumericChars("-12312"), true);
  EXPECT_EQ(hasNonNumericChars("123123"), false);
  EXPECT_EQ(hasNonNumericChars(""), false);
}

TEST(splitString, splitString)
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

TEST(checkTime, checkTime)
{
  EXPECT_EQ(checkTime("12:12"), 732);
  EXPECT_EQ(checkTime("00:00"), 0);
  
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
  EXPECT_EQ(checkTime("25:62"), -1);
}

TEST(checkClientName, checkClientName) {
  EXPECT_EQ(!!isalpha('a'), true);
  EXPECT_EQ(!!isupper('A'), true);

}
