#pragma once
#include "Game.h"
#include "ECS/ECS.h"
#include <sstream>
#include <cmath>
#include <map>
#include <string>


class UIStatistics
{
public:

    UIStatistics(Manager* gameManager);
    ~UIStatistics();


    SDL_Color textColour;

    void newStat(int xpos, int ypos, std::string labelTitle);
    void updateStat(std::string labelid, std::string labelString, std::string dataString);


private:

    Manager* manager;
    std::map<std::string, Entity*> labels;

};