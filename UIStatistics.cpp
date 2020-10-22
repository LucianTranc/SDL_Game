#include "UIStatistics.h"
#include "ECS/Components.h"


UIStatistics::UIStatistics(Manager* gameManager) : manager(gameManager)
{
    
    textColour = {255, 255, 255, 255};

}

void UIStatistics::newStat(int xpos, int ypos, std::string labelTitle)
{
    auto& newLabel(manager->addEntity());

    newLabel.addComponent<UILabel>(xpos, ypos, "blank feild", "arial", textColour);

    newLabel.addGroup(Game::groupUILables);

    labels.emplace(labelTitle, newLabel);

}

void UIStatistics::updateStat(std::string labelid, std::string labelString, std::string dataString)
{
    std::stringstream datastream;

    datastream << labelString << ": " << dataString;

    auto& label = labels[labelid];

    label->getComponent<UILabel>().SetLabelText(datastream.str(), "arial");
}