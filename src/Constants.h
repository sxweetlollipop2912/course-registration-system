#pragma once

#include <string>

using std::string;

namespace PATH {
    const string ASSETS = "./assets/";
    const string FONTS = ASSETS + (const string) "fonts/";
    const string DEFAULT_FONT = FONTS + (const string) "arial.ttf";
    const string IMAGES = ASSETS + (const string) "images/";

    const string SAVES = "./saves/";
    const string DATABASE = SAVES + (const string) "database.txt";
    const string APP = SAVES + (const string) "app.txt";

    const string EXPORTS = "./exports/";
}

namespace CSV {
    const char SEPARATOR = ',';
}

namespace ACCOUNT {
    const string DEFAULT_PASS = "1234";

    /// For debugging
    const string ADMIN_USERNAME = "admin";
    const string ADMIN_PASS = "admin";
}

namespace GUI {
    const int windowWidth = 1200;
    const int windowHeight = 900;

    const int defaultMediumCharSize = 25;
    const int defaultSmallCharSize = 20;
}