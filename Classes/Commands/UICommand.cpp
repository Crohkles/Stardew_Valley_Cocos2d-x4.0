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
bool ToggleInventoryCommand::isInventoryOpen = false;
InventoryUI* ToggleInventoryCommand::currentInventoryUI = nullptr;

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
    
    // 检查当前currentInventoryUI是否仍然有效（可能在场景切换后失效）
    if (currentInventoryUI && !currentInventoryUI->isRunning()) {
        // 如果currentInventoryUI已经不在运行中，说明它在场景切换时被销毁了
        CCLOG("Resetting inventory state after scene transition");
        isInventoryOpen = false;
        currentInventoryUI = nullptr;
    }
    
    // 如果当前没有打开背包界面，则打开它
    if (!isInventoryOpen || currentInventoryUI == nullptr) {
        isInventoryOpen = true;
        CCLOG("Opening inventory for scene: %s", sceneType.c_str());
        
        currentInventoryUI = InventoryUI::create(inventory, sceneType);
        if (currentInventoryUI) {
            currentScene->addChild(currentInventoryUI, 20);  // 将 InventoryUI 添加到当前场景上层
        }
    }
    // 如果已经打开背包界面，则关闭它
    else {
        isInventoryOpen = false;
        CCLOG("Closing inventory");
        
        if (currentInventoryUI && currentScene) {
            currentScene->removeChild(currentInventoryUI, true);  // 从当前场景中移除 InventoryUI
        }
        currentInventoryUI = nullptr;  // 重置指针
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
    isInventoryOpen = false;
    currentInventoryUI = nullptr;
}

// ==================== CloseUICommand 实现 ====================

CloseUICommand::CloseUICommand(cocos2d::Node* ui, cocos2d::Node* parent)
    : uiNode(ui), parentNode(parent) {
}

void CloseUICommand::execute() {
    if (uiNode && parentNode) {
        parentNode->removeChild(uiNode, true);
        uiNode = nullptr;  // 防止重复操作
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