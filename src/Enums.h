#pragma once

#include <string>

using std::string;

enum class DataType {
    Account,
    SchoolYear,
    Class,
    Other
};

enum class UserType {
    Staff = 0,
    Student = 1,
    Unknown = 2
};
/// Usage: userTypeStr[(int)UserType::Unknown]
const string userTypeStr[] = { "Staff","Student","Unknown" };

enum class Gender {
    Male = 0,
    Female = 1,
    Other = 2,
    Unknown = 3
};
/// Usage: genderStr[(int)Gender::Unknown]
const string genderStr[] = { "Male","Female","Other","Unknown" };

struct FullName {
    string first;
    string last;
    FullName() = default;
    FullName(const string &first_name, const string &last_name);

    string toStr() const;
};