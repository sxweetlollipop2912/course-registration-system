#pragma once

#include<iostream>
#include <SFML/Graphics.hpp>
#include <stack>
#include "../Constants.h"
#include "Interaction.h"
#include "../App/App.h"

using namespace GUI;
using namespace std;

void checkAccount(int dummy);
void login_scene(sf::RenderWindow& window, App& app);