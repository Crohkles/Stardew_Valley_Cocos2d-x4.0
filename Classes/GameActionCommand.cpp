#include "GameActionCommand.h"
#include "Player.h"
#include "EnergySystem.h"
#include "FishingGame.h"
#include "AppDelegate.h"
#include "cocos2d.h"

USING_NS_CC;

// ======================== GameActionCommand 基类实现 ========================

GameActionCommand::GameActionCommand(
    const std::string& actionName,
    std::function<void()> actionFunction,
    std::function<bool()> conditionChecker,
    std::function<void()> preActionCallback
) : actionName(actionName),
    actionFunction(actionFunction),
    conditionChecker(conditionChecker),
    preActionCallback(preActionCallback)
{
}

void GameActionCommand::execute()
{
    if (!canExecute()) {
        CCLOG("GameActionCommand: Cannot execute %s - conditions not met", actionName.c_str());
        return;
    }
    
    CCLOG("GameActionCommand: Executing %s", actionName.c_str());
    
    // 执行前回调
    if (preActionCallback) {
        preActionCallback();
    }
    
    // 执行动作
    if (actionFunction) {
        actionFunction();
    }
}

bool GameActionCommand::canExecute() const
{
    if (conditionChecker) {
        return conditionChecker();
    }
    return true;  // 默认总是可以执行
}

// 具体的游戏动作命令类实现在SceneInteractionCommand.cpp中
// 以避免重复定义和维护问题