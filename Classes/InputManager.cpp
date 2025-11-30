#include "InputManager.h"
#include <algorithm>

// 静态成员初始化
InputManager* InputManager::instance = nullptr;

InputManager::InputManager() 
    : keyboardListener(nullptr)
    , activeContext("Default")
    , loggingEnabled(false)
{
}

InputManager::~InputManager() {
    cleanup();
}

InputManager* InputManager::getInstance() {
    if (instance == nullptr) {
        instance = new InputManager();
    }
    return instance;
}

void InputManager::destroyInstance() {
    if (instance != nullptr) {
        delete instance;
        instance = nullptr;
    }
}

void InputManager::initialize() {
    if (keyboardListener != nullptr) {
        logDebugInfo("InputManager already initialized");
        return;
    }
    
    // 创建键盘事件监听器
    keyboardListener = EventListenerKeyboard::create();
    
    // 设置按键按下回调
    keyboardListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event) {
        this->onKeyPressed(keyCode, event);
    };
    
    // 设置按键释放回调
    keyboardListener->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event* event) {
        this->onKeyReleased(keyCode, event);
    };
    
    // 将监听器添加到事件分发器中，设置为最高优先级
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(keyboardListener, 1);
    
    logDebugInfo("InputManager initialized successfully");
}

void InputManager::cleanup() {
    if (keyboardListener != nullptr) {
        Director::getInstance()->getEventDispatcher()->removeEventListener(keyboardListener);
        keyboardListener = nullptr;
    }
    
    clearAllBindings();
    keyStates.clear();
    
    logDebugInfo("InputManager cleaned up");
}

// ==================== 命令绑定管理 ====================

void InputManager::bindPressCommand(EventKeyboard::KeyCode key, std::shared_ptr<KeyCommand> command) {
    if (command == nullptr) {
        logDebugInfo("Warning: Trying to bind null command");
        return;
    }
    
    keyPressCommands[key].push_back(command);
    
    if (loggingEnabled) {
        logDebugInfo("Bound press command '" + command->getDescription() + "' to key " + std::to_string(static_cast<int>(key)));
    }
}

void InputManager::bindReleaseCommand(EventKeyboard::KeyCode key, std::shared_ptr<KeyCommand> command) {
    if (command == nullptr) {
        logDebugInfo("Warning: Trying to bind null command");
        return;
    }
    
    keyReleaseCommands[key].push_back(command);
    
    if (loggingEnabled) {
        logDebugInfo("Bound release command '" + command->getDescription() + "' to key " + std::to_string(static_cast<int>(key)));
    }
}

void InputManager::bindCommand(EventKeyboard::KeyCode key, std::shared_ptr<KeyCommand> command) {
    // 默认绑定到按键按下事件
    bindPressCommand(key, command);
}

void InputManager::unbindCommand(EventKeyboard::KeyCode key, std::shared_ptr<KeyCommand> command) {
    // 从按键按下命令中移除
    auto itPress = keyPressCommands.find(key);
    if (itPress != keyPressCommands.end()) {
        auto& commands = itPress->second;
        commands.erase(
            std::remove(commands.begin(), commands.end(), command),
            commands.end()
        );
        
        if (commands.empty()) {
            keyPressCommands.erase(itPress);
        }
    }
    
    // 从按键释放命令中移除
    auto itRelease = keyReleaseCommands.find(key);
    if (itRelease != keyReleaseCommands.end()) {
        auto& commands = itRelease->second;
        commands.erase(
            std::remove(commands.begin(), commands.end(), command),
            commands.end()
        );
        
        if (commands.empty()) {
            keyReleaseCommands.erase(itRelease);
        }
    }
    
    if (loggingEnabled) {
        logDebugInfo("Unbound command from key " + std::to_string(static_cast<int>(key)));
    }
}

void InputManager::clearBindings(EventKeyboard::KeyCode key) {
    auto itPress = keyPressCommands.find(key);
    if (itPress != keyPressCommands.end()) {
        keyPressCommands.erase(itPress);
    }
    
    auto itRelease = keyReleaseCommands.find(key);
    if (itRelease != keyReleaseCommands.end()) {
        keyReleaseCommands.erase(itRelease);
    }
    
    logDebugInfo("Cleared all bindings for key " + std::to_string(static_cast<int>(key)));
}

void InputManager::clearAllBindings() {
    keyPressCommands.clear();
    keyReleaseCommands.clear();
    logDebugInfo("Cleared all command bindings");
}

// ==================== 键盘事件处理 ====================

void InputManager::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
    // 更新按键状态
    keyStates[keyCode] = true;
    
    if (loggingEnabled) {
        logDebugInfo("Key pressed: " + std::to_string(static_cast<int>(keyCode)) + " in context: " + activeContext);
    }
    
    // 查找并执行绑定的按键按下命令
    auto it = keyPressCommands.find(keyCode);
    if (it != keyPressCommands.end()) {
        // 复制命令列表以防止在场景切换时出现迭代器失效问题
        auto commands = it->second;  // 创建副本
        auto currentScene = Director::getInstance()->getRunningScene();
        
        for (auto& command : commands) {
            if (command != nullptr && command->canExecute()) {
                if (loggingEnabled) {
                    logDebugInfo("Executing press command: " + command->getDescription());
                }
                try {
                    command->execute();
                    
                    // 检查场景是否在命令执行后发生了变化
                    if (Director::getInstance()->getRunningScene() != currentScene) {
                        // 场景已经切换，停止执行剩余命令以避免访问已释放的内存
                        if (loggingEnabled) {
                            logDebugInfo("Scene changed during command execution, stopping command loop");
                        }
                        break;
                    }
                    
                    // 调用命令执行回调（如果设置了）
                    if (commandExecutedCallback) {
                        commandExecutedCallback();
                    }
                } catch (const std::exception& e) {
                    logDebugInfo("Error executing command: " + std::string(e.what()));
                } catch (...) {
                    logDebugInfo("Unknown error executing command: " + command->getDescription());
                }
            }
        }
    }
}

void InputManager::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
    // 更新按键状态
    keyStates[keyCode] = false;
    
    if (loggingEnabled) {
        logDebugInfo("Key released: " + std::to_string(static_cast<int>(keyCode)));
    }
    
    // 查找并执行绑定的按键释放命令
    auto it = keyReleaseCommands.find(keyCode);
    if (it != keyReleaseCommands.end()) {
        // 复制命令列表以防止在场景切换时出现迭代器失效问题
        auto commands = it->second;  // 创建副本
        auto currentScene = Director::getInstance()->getRunningScene();
        
        for (auto& command : commands) {
            if (command != nullptr && command->canExecute()) {
                if (loggingEnabled) {
                    logDebugInfo("Executing release command: " + command->getDescription());
                }
                try {
                    command->execute();
                    
                    // 检查场景是否在命令执行后发生了变化
                    if (Director::getInstance()->getRunningScene() != currentScene) {
                        // 场景已经切换，停止执行剩余命令以避免访问已释放的内存
                        if (loggingEnabled) {
                            logDebugInfo("Scene changed during command execution, stopping command loop");
                        }
                        break;
                    }
                    
                    // 调用命令执行回调（如果设置了）
                    if (commandExecutedCallback) {
                        commandExecutedCallback();
                    }
                } catch (const std::exception& e) {
                    logDebugInfo("Error executing command: " + std::string(e.what()));
                } catch (...) {
                    logDebugInfo("Unknown error executing command: " + command->getDescription());
                }
            }
        }
    }
}

// ==================== 状态查询 ====================

bool InputManager::isKeyPressed(EventKeyboard::KeyCode key) const {
    auto it = keyStates.find(key);
    return (it != keyStates.end()) ? it->second : false;
}

size_t InputManager::getCommandCount(EventKeyboard::KeyCode key) const {
    size_t pressCount = 0;
    size_t releaseCount = 0;
    
    auto itPress = keyPressCommands.find(key);
    if (itPress != keyPressCommands.end()) {
        pressCount = itPress->second.size();
    }
    
    auto itRelease = keyReleaseCommands.find(key);
    if (itRelease != keyReleaseCommands.end()) {
        releaseCount = itRelease->second.size();
    }
    
    return pressCount + releaseCount;
}

// ==================== 上下文管理 ====================

void InputManager::setActiveContext(const std::string& context) {
    if (activeContext != context) {
        logDebugInfo("Context changed from '" + activeContext + "' to '" + context + "'");
        activeContext = context;
    }
}

std::string InputManager::getActiveContext() const {
    return activeContext;
}

// ==================== 调试和日志 ====================

void InputManager::enableKeyLogging(bool enabled) {
    loggingEnabled = enabled;
    logDebugInfo(enabled ? "Key logging enabled" : "Key logging disabled");
}

void InputManager::logDebugInfo(const std::string& message) const {
    if (loggingEnabled) {
        CCLOG("[InputManager] %s", message.c_str());
    }
}

void InputManager::setCommandExecutedCallback(std::function<void()> callback) {
    commandExecutedCallback = callback;
    logDebugInfo("Command executed callback set");
}