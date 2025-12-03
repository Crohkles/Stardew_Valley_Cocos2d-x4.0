#include "UICommand.h"
#include "../UI/InventoryUI.h"
#include "../UI/mini_bag.h"
#include "../Items/Food.h"
#include "../Core/AppDelegate.h"
#include "../Systems/EnergySystem.h"
#include "../Systems/GameInteractionFacade.h"
#include "cocos2d.h"
#include <memory>
#include <algorithm>

USING_NS_CC;

// ==================== ToggleInventoryCommand 实现 ====================

// 静态成员初始化
ToggleInventoryCommand::UIState ToggleInventoryCommand::currentState = ToggleInventoryCommand::UIState::None;
cocos2d::Node* ToggleInventoryCommand::currentUI = nullptr;

ToggleInventoryCommand::ToggleInventoryCommand(cocos2d::Node* parent, Inventory* inv, const std::string& scene)
    : parentNode(parent), inventory(inv), sceneType(scene) {
}

void ToggleInventoryCommand::execute() {
    if (!inventory) {
        return;
    }
    
    // 获取当前运行的场景，避免使用可能失效的parentNode指针
    auto currentScene = Director::getInstance()->getRunningScene();
    if (!currentScene) {
        CCLOG("Warning: No running scene found for inventory toggle");
        return;
    }
    
    // 检查当前currentUI是否仍然有效
    if (currentUI) {
        // 使用安全的方法检查UI是否仍在场景中
        bool isValid = false;
        try {
            // 检查引用计数，如果对象已被销毁，这里可能会出现问题
            if (currentScene && currentUI->getReferenceCount() > 0) {
                auto parent = currentUI->getParent();
                if (parent == currentScene) {
                    isValid = true;
                }
            }
        } catch (...) {
            // 如果访问出现异常，说明对象已经无效
            isValid = false;
        }
        
        if (!isValid) {
            // UI已经无效，重置状态
            CCLOG("UI is invalid, resetting state");
            currentState = UIState::None;
            currentUI = nullptr;
        }
    }
    
    // 如果当前状态为None，则打开背包界面
    if (currentState == UIState::None) {
        CCLOG("Opening inventory for scene: %s", sceneType.c_str());
        
        auto inventoryUI = InventoryUI::create(inventory, sceneType);
        if (inventoryUI) {
            currentScene->addChild(inventoryUI, 20);  // 将 InventoryUI 添加到当前场景上层
            updateState(UIState::Inventory, inventoryUI);
        }
    }
    // 如果状态不为None，则统一关闭当前打开的UI
    else {
        CCLOG("Closing current UI, state: %d", static_cast<int>(currentState));
        
        if (currentUI && currentScene) {
            currentScene->removeChild(currentUI, true);  // 从当前场景中移除 UI
        }
        resetState();
    }
}

void ToggleInventoryCommand::undo() {
    // 背包切换通常不需要撤销功能
    // 如果需要，可以实现反向操作
}

bool ToggleInventoryCommand::canExecute() const {
    return parentNode != nullptr && inventory != nullptr;
}

std::string ToggleInventoryCommand::getDescription() const {
    return "ToggleInventory_" + sceneType;
}

void ToggleInventoryCommand::resetState() {
    currentState = UIState::None;
    currentUI = nullptr;
}

void ToggleInventoryCommand::updateState(UIState newState, cocos2d::Node* ui) {
    currentState = newState;
    currentUI = ui;
    CCLOG("UI State updated to: %d", static_cast<int>(newState));
}

void ToggleInventoryCommand::onUIDestroyed(cocos2d::Node* ui) {
    // 当UI被外部销毁时，重置静态指针
    if (currentUI == ui) {
        CCLOG("UI destroyed externally, resetting state");
        resetState();
    }
}

// ==================== CloseUICommand 实现 ====================

CloseUICommand::CloseUICommand(cocos2d::Node* ui, cocos2d::Node* parent)
    : uiNode(ui), parentNode(parent) {
}

void CloseUICommand::execute() {
    if (uiNode && parentNode) {
        // 关闭UI时，同时重置整个UI系统状态
        // 这确保ESC键能正确关闭所有相关UI
        ToggleInventoryCommand::resetState();
        
        parentNode->removeChild(uiNode, true);
        uiNode = nullptr;  // 防止重复操作
        
        CCLOG("UI closed via ESC key, inventory system state reset");
    }
}

void CloseUICommand::undo() {
    // UI关闭操作通常不需要撤销功能
}

bool CloseUICommand::canExecute() const {
    return uiNode != nullptr && parentNode != nullptr;
}

std::string CloseUICommand::getDescription() const {
    return "CloseUI";
}

// ==================== ConsumeFoodCommand 实现 ====================

ConsumeFoodCommand::ConsumeFoodCommand(class mini_bag* bag)
    : miniBag(bag) {
}

void ConsumeFoodCommand::execute() {
    if (!miniBag) {
        CCLOG("ConsumeFoodCommand: mini_bag is null");
        return;
    }
    
    // 获取选中的食物
    auto selected_item = std::dynamic_pointer_cast<Food>(miniBag->getSelectedItem());
    if (selected_item != nullptr) {
        // 使用外观类统一处理食物消费逻辑
        auto facade = GameInteractionFacade::getInstance();
        auto result = facade->consumeFood(selected_item, miniBag);
        
        if (result.success) {
            CCLOG("ConsumeFoodCommand: %s", result.message.c_str());
        } else {
            CCLOG("ConsumeFoodCommand: Failed - %s", result.message.c_str());
        }
    } else {
        CCLOG("ConsumeFoodCommand: No food item selected");
    }
}

void ConsumeFoodCommand::undo() {
    // 食物消费通常不支持撤销
}

bool ConsumeFoodCommand::canExecute() const {
    if (!miniBag) {
        return false;
    }
    
    // 检查是否有选中的食物
    auto selected_item = std::dynamic_pointer_cast<Food>(miniBag->getSelectedItem());
    return selected_item != nullptr;
}

std::string ConsumeFoodCommand::getDescription() const {
    return "ConsumeFood";
}