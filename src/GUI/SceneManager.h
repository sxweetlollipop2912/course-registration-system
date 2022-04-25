#pragma once

#include "../App/Database.h"
#include "../App/List.h"
#include <SFML/Graphics.hpp>


enum class SceneType {
    Scene0,
    Scene1,
    Scene2,
    Scene3,
    Scene4,
    Scene5,
    Scene6,
    Scene7,
    Scene8,
    Scene9,
    Scene10,
    Scene11,
    SceneModifyReg,
    SceneAddCourse
};

class SceneManager {
private:
    List<SceneType> stack;
public:
    DataIter arg;
    bool refresh = false;

    SceneManager() = default;

    SceneType top() const;
    int size() const;
    bool empty() const;
    void push(SceneType);
    void pop();
    void clear();

    void interact(sf::Event event);
};