#include "iostream"

#include "RegistrationSession.h"
#include "Utils.h"

bool RegistrationSession::isOngoing() const {
    return !hasEnded() && !hasNotStarted();
}

bool RegistrationSession::hasInit() const {
    auto st_tmp = start, en_tmp = end;
    return mktime(&st_tmp) != -1 && mktime(&en_tmp) != -1;
}

bool RegistrationSession::hasEnded() const {
    auto now = Utils::now();
    auto en_tmp = end;
    return mktime(&en_tmp) <= mktime(&now);
}

bool RegistrationSession::hasNotStarted() const {
    auto now = Utils::now();
    auto st_tmp = start;
    return mktime(&st_tmp) > mktime(&now);
}
