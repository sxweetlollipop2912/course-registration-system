#pragma once

#include <string>

using std::string;

enum class UserType {
    Staff,
    Student,
    Unknown
};

enum class Gender {
    Male,
    Female,
    Other,
    Unknown
};

struct FullName {
    string first;
    string last;
    FullName() = default;
    FullName(const string &first_name, const string &last_name) : first{first_name}, last{last_name} {}
};