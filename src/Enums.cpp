#include "Enums.h"
#include "App/Utils.h"

FullName::FullName(const string &first_name, const string &last_name) : first{first_name}, last{last_name} {
    Utils::trimStr(first);
    Utils::trimStr(last);
}

string FullName::toStr() const {
    return last + " " + first;
}
