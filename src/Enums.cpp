#include "Enums.h"
#include "App/Utils.h"

FullName::FullName(const string &first_name, const string &last_name) : first{first_name}, last{last_name} {
    Utils::trimStr(first);
    Utils::trimStr(last);
}

FullName::FullName(const string &name) {
    int pos = (int)name.find_last_of(' ');
    if (pos == string::npos)
        pos = -1;

    first = name.substr(pos + 1, name.size());
    if (pos > 0)
        last = name.substr(0, pos);

    Utils::trimStr(first);
    Utils::trimStr(last);
}

string FullName::toStr() const {
    return last + " " + first;
}
