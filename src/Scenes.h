#pragma once

#include <functional>

#include "App.h"

using std::function;

class Scenes {
private:
    void (Scenes::*current_scene)();
    std::shared_ptr<App> app;
    bool stop_token;

public:
    explicit Scenes(App &app) : app{&app}, stop_token{false} {
        current_scene = &Scenes::menu;
    }

    [[noreturn]] void run();

    void menu();
    void menuNoDefaultYear();
    void menuNoDefaultSemester();
    void menuWithDefaultSemester();

};