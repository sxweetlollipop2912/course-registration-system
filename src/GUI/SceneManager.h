#pragma once

#include "../App/Database.h"
#include "../App/List.h"

class SceneManager {
private:
    List<int> stack;
public:
    DataIter arg;

    SceneManager() = default;

    int top() const;
    int size() const;
    bool empty() const;
    void push(int);
    void pop();
    void clear();
};