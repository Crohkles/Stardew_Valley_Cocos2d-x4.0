#ifndef __UI_COMMAND_H__
#define __UI_COMMAND_H__

#include "KeyCommand.h"
#include "cocos2d.h"

// 前向声明，避免循环包含
class InventoryUI;
class Inventory;

USING_NS_CC;

/**
 * @brief 切换背包界面命令
 * 
 * 处理ESC键打开/关闭背包界面的功能
 * 使用静态变量跟踪当前界面状态，避免重复创建
 */
class ToggleInventoryCommand : public KeyCommand {
public:
    enum class UIState {
        None,
        Inventory,
        SkillTree,
        Intimacy,
        Quit
    };

private:
    cocos2d::Node* parentNode;           // 父节点，用于添加/移除UI
    Inventory* inventory;       // 背包数据
    std::string sceneType;      // 场景类型，用于创建对应的背包UI
    
    static UIState currentState;
    static cocos2d::Node* currentUI;
    
public:
    ToggleInventoryCommand(cocos2d::Node* parent, Inventory* inv, const std::string& scene);
    virtual ~ToggleInventoryCommand() = default;
    
    void execute() override;
    void undo() override;
    bool canExecute() const override;
    std::string getDescription() const override;
    
    // 静态方法用于重置状态
    static void resetState();
    
    // 更新状态和当前UI指针
    static void updateState(UIState newState, cocos2d::Node* ui);

    // 当UI被销毁时调用，用于重置静态指针（如果被销毁的UI是当前记录的UI）
    static void onUIDestroyed(cocos2d::Node* ui);
};

/**
 * @brief 关闭UI界面命令
 * 
 * 通用的关闭UI界面命令，适用于各种UI界面的ESC键关闭功能
 */
class CloseUICommand : public KeyCommand {
private:
    cocos2d::Node* uiNode;               // 要关闭的UI节点
    cocos2d::Node* parentNode;           // 父节点，用于移除UI
    
public:
    CloseUICommand(cocos2d::Node* ui, cocos2d::Node* parent);
    virtual ~CloseUICommand() = default;
    
    void execute() override;
    void undo() override;
    bool canExecute() const override;
    std::string getDescription() const override;
};

/**
 * @brief 消费食物命令
 * 
 * 处理mini_bag中的E键消费食物恢复体力功能
 */
class ConsumeFoodCommand : public KeyCommand {
private:
    class mini_bag* miniBag;    // mini_bag实例
    
public:
    ConsumeFoodCommand(class mini_bag* bag);
    virtual ~ConsumeFoodCommand() = default;
    
    void execute() override;
    void undo() override;
    bool canExecute() const override;
    std::string getDescription() const override;
};

#endif // __UI_COMMAND_H__