#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

#include "cocos2d.h"
#include "../Commands/KeyCommand.h"
#include <map>
#include <vector>
#include <memory>
#include <string>

USING_NS_CC;

/**
 * @brief 输入管理器 - 单例模式
 * 
 * 负责统一管理所有键盘输入，使用Command Pattern处理各种键盘操作。
 * 解决了原有系统中键盘处理逻辑分散、耦合度高的问题。
 */
class InputManager {
private:
    static InputManager* instance;
    
    // 键盘命令映射：支持一个按键绑定多个命令
    std::map<EventKeyboard::KeyCode, std::vector<std::shared_ptr<KeyCommand>>> keyPressCommands;
    std::map<EventKeyboard::KeyCode, std::vector<std::shared_ptr<KeyCommand>>> keyReleaseCommands;
    
    // 按键状态管理：统一管理所有按键的按下/释放状态
    std::map<EventKeyboard::KeyCode, bool> keyStates;
    
    // Cocos2d-x键盘事件监听器
    EventListenerKeyboard* keyboardListener;
    
    // 当前活动的上下文（用于处理不同场景下按键行为不同的情况）
    std::string activeContext;
    
    // 调试和日志开关
    bool loggingEnabled;
    
    // 命令执行回调（用于测试和统计）
    std::function<void()> commandExecutedCallback;
    
    // 私有构造函数（单例模式）
    InputManager();
    
    // 禁用拷贝构造和赋值操作
    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;

public:
    /**
     * @brief 获取InputManager单例实例
     */
    static InputManager* getInstance();
    
    /**
     * @brief 销毁单例实例
     */
    static void destroyInstance();
    
    /**
     * @brief 初始化输入管理器
     * 
     * 创建键盘事件监听器，设置回调函数
     */
    void initialize();
    
    /**
     * @brief 清理资源
     * 
     * 移除事件监听器，清空命令绑定
     */
    void cleanup();
    
    // ==================== 命令绑定管理 ====================
    
    /**
     * @brief 绑定按键按下时的命令
     * 
     * @param key 键盘按键
     * @param command 要执行的命令
     */
    void bindPressCommand(EventKeyboard::KeyCode key, std::shared_ptr<KeyCommand> command);
    
    /**
     * @brief 绑定按键释放时的命令
     * 
     * @param key 键盘按键
     * @param command 要执行的命令
     */
    void bindReleaseCommand(EventKeyboard::KeyCode key, std::shared_ptr<KeyCommand> command);
    
    /**
     * @brief 绑定按键到命令（兼容旧接口，默认为按下事件）
     * 
     * @param key 键盘按键
     * @param command 要执行的命令
     */
    void bindCommand(EventKeyboard::KeyCode key, std::shared_ptr<KeyCommand> command);
    
    /**
     * @brief 解绑特定按键的特定命令
     * 
     * @param key 键盘按键
     * @param command 要解绑的命令
     */
    void unbindCommand(EventKeyboard::KeyCode key, std::shared_ptr<KeyCommand> command);
    
    /**
     * @brief 清除特定按键的所有命令绑定
     * 
     * @param key 键盘按键
     */
    void clearBindings(EventKeyboard::KeyCode key);
    
    /**
     * @brief 清除所有命令绑定
     */
    void clearAllBindings();
    
    // ==================== 键盘事件处理 ====================
    
    /**
     * @brief 按键按下事件处理
     * 
     * @param keyCode 按键代码
     * @param event 事件对象
     */
    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    
    /**
     * @brief 按键释放事件处理
     * 
     * @param keyCode 按键代码
     * @param event 事件对象
     */
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
    
    // ==================== 状态查询 ====================
    
    /**
     * @brief 查询按键是否处于按下状态
     * 
     * @param key 键盘按键
     * @return true 如果按键当前被按下
     */
    bool isKeyPressed(EventKeyboard::KeyCode key) const;
    
    /**
     * @brief 获取指定按键绑定的命令数量
     * 
     * @param key 键盘按键
     * @return 绑定的命令数量
     */
    size_t getCommandCount(EventKeyboard::KeyCode key) const;
    
    // ==================== 上下文管理 ====================
    
    /**
     * @brief 设置当前活动的上下文
     * 
     * 用于在不同场景/状态下实现不同的按键行为
     * 例如："GamePlaying", "MenuOpen", "DialogOpen"等
     * 
     * @param context 上下文名称
     */
    void setActiveContext(const std::string& context);
    
    /**
     * @brief 获取当前活动的上下文
     * 
     * @return 当前上下文名称
     */
    std::string getActiveContext() const;
    
    // ==================== 调试和日志 ====================
    
    /**
     * @brief 启用/禁用键盘输入日志
     * 
     * @param enabled 是否启用日志
     */
    void enableKeyLogging(bool enabled);
    
    /**
     * @brief 设置命令执行回调（用于测试和统计）
     * 
     * @param callback 命令执行时调用的回调函数
     */
    void setCommandExecutedCallback(std::function<void()> callback);
    
    /**
     * @brief 记录调试信息
     * 
     * @param message 日志消息
     */
    void logDebugInfo(const std::string& message) const;
    
    /**
     * @brief 析构函数
     */
    ~InputManager();
};

#endif // __INPUT_MANAGER_H__