#pragma once
#include <initializer_list>
#include <set>
#include <algorithm>
#include <iterator>
#include <iostream>
#include "CppUnitTest.h"

#define WIDE(x) L ## x

template <class K, class T>
std::wostream& operator <<(std::wostream& os, const std::pair<K, T>& v)
{
  return os << "{" << v.first << ", " << v.second << "}";
}

template <class K, class T>
bool operator <(const std::pair<K, T>& a, const std::pair<K, T>& b)
{
  return std::tie(a) < std::tie(b);
}

template <class T>
std::string join(T container, const char *delim = ", ")
{
  std::stringstream ss;
  for (auto it = std::begin(container); it != std::end(container); ++it)
  {
    if (it != std::begin(container)) ss << delim;
    ss << *it;
  }
  return ss.str();
}

template <class T>
std::wstring wjoin(T container, const wchar_t *delim = L", ")
{
  std::wstringstream ss;
  for (auto it = std::begin(container); it != std::end(container); ++it)
  {
    if (it != std::begin(container)) ss << delim;
    ss << *it;
  }
  return ss.str();
}

template <class T>
void AssertSetEquals(std::initializer_list<typename T::value_type> expected, const T& actual, const wchar_t* message = nullptr, const Microsoft::VisualStudio::CppUnitTestFramework::__LineInfo* pLineInfo = nullptr)
{
  using namespace Microsoft::VisualStudio::CppUnitTestFramework;

  std::set<typename T::value_type> expected_set(expected.begin(), expected.end());
  std::set<typename T::value_type> actual_set(actual.begin(), actual.end());
  std::set<typename T::value_type> extra_actual, extra_expected;

  std::set_difference(expected_set.begin(), expected_set.end(),
    actual_set.begin(), actual_set.end(),
    std::inserter(extra_expected, extra_expected.end()));
  std::set_difference(actual_set.begin(), actual_set.end(),
    expected_set.begin(), expected_set.end(),
    std::inserter(extra_actual, extra_actual.end()));

  if (!extra_expected.empty() || !extra_actual.empty())
  {
    std::wstringstream ss;
    if (!extra_expected.empty())
      ss << L"\nNot Found: {" << wjoin(extra_expected) << L"}";
    if (!extra_actual.empty())
      ss << L"\nUnexpected: {" << wjoin(extra_actual) << L"}";
    if (message != nullptr) ss << L"\n" << message;
    Assert::Fail(ss.str().c_str(), pLineInfo);
  }
}
