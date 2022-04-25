#include "SceneManager.h"

int SceneManager::top() const {
    return stack.back();
}

int SceneManager::size() const {
    return stack.size();
}

bool SceneManager::empty() const {
    return stack.empty();
}

void SceneManager::push(int i) {
    stack.push_back(i);
}

void SceneManager::pop() {
    stack.pop_back();
}

void SceneManager::clear() {
    stack.clear();
}