#include "iostream"

#include "RegistrationSession.h"
#include "Utils.h"

bool RegistrationSession::isOngoing() {
    auto now = Utils::now();
    return mktime(&start) <= mktime(&now) && mktime(&end) > mktime(&now);
}
