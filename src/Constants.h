#pragma once

#include <string>

using std::string;

namespace PATH {
    const string ASSETS = "./assets/";
    const string FONTS = ASSETS + (const string)"fonts/";
    const string DEFAULT_FONT = FONTS + (const string)"arial.ttf";
}

namespace ACCOUNT {
    const string DEFAULT_PASS = "1234";

    /// For debugging
    const string ADMIN_USERNAME = "admin";
    const string ADMIN_PASS = "admin";
}

namespace GUI {
    const int WINDOW_WIDTH = 1200;
    const int WINDOW_HEIGHT = 900;

    const int DEFAULT_CHAR_SIZE = 25;
}