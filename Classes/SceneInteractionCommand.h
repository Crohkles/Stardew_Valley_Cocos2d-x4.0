#ifndef __SCENE_INTERACTION_COMMAND_H__
#define __SCENE_INTERACTION_COMMAND_H__

#include "KeyCommand.h"
#include "cocos2d.h"
#include <functional>

// 前向声明
class Player;
class farm;
class FishingGame;

USING_NS_CC;

/**
 * @brief 场景切换命令
 * 
 * 处理ENTER键场景切换功能，支持通过函数对象创建目标场景
 */
class SceneTransitionCommand : public KeyCommand {
private:
    std::function<Scene*()> sceneCreator;   // 场景创建函数
    Player* player;                         // 玩家对象，用于场景切换前的清理
    std::string targetSceneName;            // 目标场景名称，用于调试
    
public:
    SceneTransitionCommand(Player* p, std::function<Scene*()> creator, const std::string& sceneName = "");
    virtual ~SceneTransitionCommand() = default;
    
    void execute() override;
    void undo() override;
    bool canExecute() const override;
    std::string getDescription() const override;
};

/**
 * @brief 农场操作命令基类
 * 
 * 为所有农场相关的操作提供公共基础
 */
class FarmingActionCommand : public KeyCommand {
protected:
    farm* farmScene;        // 农场场景引用
    std::string actionType; // 操作类型："plant", "harvest", "water"
    
public:
    FarmingActionCommand(farm* scene, const std::string& action);
    virtual ~FarmingActionCommand() = default;
    
    bool canExecute() const override;
    std::string getDescription() const override;
};

/**
 * @brief 种植命令
 * 
 * 处理P键种植作物功能
 */
class PlantCommand : public FarmingActionCommand {
public:
    PlantCommand(farm* scene);
    void execute() override;
    void undo() override;
};

/**
 * @brief 收获命令
 * 
 * 处理G键收获作物功能
 */
class HarvestCommand : public FarmingActionCommand {
public:
    HarvestCommand(farm* scene);
    void execute() override;
    void undo() override;
};

/**
 * @brief 浇水命令
 * 
 * 处理W键浇水功能
 */
class WaterCommand : public FarmingActionCommand {
public:
    WaterCommand(farm* scene);
    void execute() override;
    void undo() override;
};

/**
 * @brief 钓鱼命令
 * 
 * 处理H键钓鱼功能
 */
class FishingCommand : public KeyCommand {
private:
    Scene* currentScene;    // 当前场景
    Player* player;         // 玩家对象
    
public:
    FishingCommand(Scene* scene, Player* p);
    virtual ~FishingCommand() = default;
    
    void execute() override;
    void undo() override;
    bool canExecute() const override;
    std::string getDescription() const override;
};

/**
 * @brief 挖矿命令
 * 
 * 处理M键挖矿功能
 */
class MiningCommand : public KeyCommand {
private:
    Scene* currentScene;    // 当前场景
    
public:
    MiningCommand(Scene* scene);
    virtual ~MiningCommand() = default;
    
    void execute() override;
    void undo() override;
    bool canExecute() const override;
    std::string getDescription() const override;
};

/**
 * @brief 伐木命令
 * 
 * 处理L键伐木功能
 */
class LoggingCommand : public KeyCommand {
private:
    Scene* currentScene;    // 当前场景
    
public:
    LoggingCommand(Scene* scene);
    virtual ~LoggingCommand() = default;
    
    void execute() override;
    void undo() override;
    bool canExecute() const override;
    std::string getDescription() const override;
};

/**
 * @brief 条件场景切换命令
 * 
 * 只有在满足特定条件时才执行场景切换的命令
 * 用于处理需要在特定区域内才能触发的场景切换
 */
class ConditionalSceneTransitionCommand : public KeyCommand {
private:
    std::function<Scene*()> sceneCreator;           // 场景创建函数
    std::function<bool()> conditionChecker;         // 条件检查函数
    std::function<void()> preTransitionCallback;    // 场景切换前的回调函数
    Player* player;                                 // 玩家对象
    std::string targetSceneName;                    // 目标场景名称
    
public:
    ConditionalSceneTransitionCommand(
        Player* p, 
        std::function<Scene*()> creator, 
        std::function<bool()> condition,
        std::function<void()> preCallback = nullptr,
        const std::string& sceneName = ""
    );
    virtual ~ConditionalSceneTransitionCommand() = default;
    
    void execute() override;
    void undo() override;
    bool canExecute() const override;
    std::string getDescription() const override;
};

/**
 * @brief 睡觉命令
 * 
 * 处理Myhouse场景中的睡觉功能，包括时间推进和场景重置
 */
class SleepCommand : public KeyCommand {
private:
    class Myhouse* myhouseScene;    // Myhouse场景引用
    
public:
    SleepCommand(class Myhouse* scene);
    virtual ~SleepCommand() = default;
    
    void execute() override;
    void undo() override;
    bool canExecute() const override;
    std::string getDescription() const override;
};

/**
 * @brief 商店交互命令
 * 
 * 处理P键打开/关闭商店界面功能
 */
class StoreInteractionCommand : public KeyCommand {
private:
    Scene* currentScene;    // 当前场景
    
public:
    StoreInteractionCommand(Scene* scene);
    virtual ~StoreInteractionCommand() = default;
    
    void execute() override;
    void undo() override;
    bool canExecute() const override;
    std::string getDescription() const override;
};

#endif // __SCENE_INTERACTION_COMMAND_H__