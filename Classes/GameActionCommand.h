#ifndef __GAME_ACTION_COMMAND_H__
#define __GAME_ACTION_COMMAND_H__

#include "KeyCommand.h"
#include "cocos2d.h"
#include <functional>

USING_NS_CC;

/**
 * @brief 游戏动作命令基类
 * 
 * 用于处理游戏中的各种动作命令，如钓鱼、伐木、挖矿等
 * 支持条件检查（如体力值、位置等）和动作执行
 */
class GameActionCommand : public KeyCommand
{
public:
    /**
     * @brief 构造函数
     * 
     * @param actionName 动作名称（用于日志和调试）
     * @param actionFunction 实际执行的动作函数
     * @param conditionChecker 条件检查函数（可选），返回true时才执行动作
     * @param preActionCallback 动作前回调（可选），用于状态更新或UI反馈
     */
    GameActionCommand(
        const std::string& actionName,
        std::function<void()> actionFunction,
        std::function<bool()> conditionChecker = nullptr,
        std::function<void()> preActionCallback = nullptr
    );
    
    virtual ~GameActionCommand() = default;
    
    // 执行命令
    virtual void execute() override;
    
    // 检查是否可以执行
    virtual bool canExecute() const override;
    
protected:
    std::string actionName;
    std::function<void()> actionFunction;
    std::function<bool()> conditionChecker;
    std::function<void()> preActionCallback;
};

// 具体的游戏动作命令类在SceneInteractionCommand.h中定义
// 以避免重复定义和维护问题

#endif // __GAME_ACTION_COMMAND_H__