#include "GameInteractionFacade.h"
#include "../Core/AppDelegate.h"
#include "../Systems/Timesystem.h"
#include "cocos2d.h"

USING_NS_CC;

// 访问全局变量
extern int strength;
extern int GoldAmount;
extern Inventory* inventory;
extern Timesystem* TimeUI;

// 静态成员初始化
GameInteractionFacade* GameInteractionFacade::instance = nullptr;

GameInteractionFacade::GameInteractionFacade() 
    : energySystem(nullptr)
    , economicSystem(nullptr)
    , taskManager(nullptr)
    , npcRelationship(nullptr)
    , playerInventory(nullptr)
    , skillTree(nullptr)
    , player(nullptr)
{
}

GameInteractionFacade* GameInteractionFacade::getInstance() {
    if (instance == nullptr) {
        instance = new GameInteractionFacade();
    }
    return instance;
}

void GameInteractionFacade::destroyInstance() {
    if (instance != nullptr) {
        delete instance;
        instance = nullptr;
    }
}

void GameInteractionFacade::initialize(EconomicSystem* economic, TaskManagement* task, 
                                     NpcRelationship* npcRel, Inventory* inventoryRef,
                                     SkillTree* skills, Player* playerRef) {
    economicSystem = economic;
    taskManager = task;
    npcRelationship = npcRel;
    playerInventory = inventoryRef;
    skillTree = skills;
    player = playerRef;
    
    // 获取能量系统单例
    energySystem = EnergySystem::getInstance();
    
    CCLOG("GameInteractionFacade initialized with all subsystems");
}

bool GameInteractionFacade::isInitialized() const {
    return energySystem != nullptr && 
           playerInventory != nullptr && 
           player != nullptr;
}

GameInteractionFacade::FoodConsumptionResult 
GameInteractionFacade::consumeFood(std::shared_ptr<Food> food, mini_bag* miniBag) {
    FoodConsumptionResult result;
    
    // 基础验证
    if (!validateFoodConsumption(food, miniBag)) {
        result.message = "Invalid food consumption parameters";
        return result;
    }
    
    // 获取食物信息
    int energyGain = food->GetEnergy();
    result.itemName = food->GetName();
    
    // 计算体力恢复（最大值100）
    int oldEnergy = getCurrentEnergy();
    int newEnergy = std::min(100, oldEnergy + energyGain);
    result.energyGained = newEnergy - oldEnergy;
    result.currentEnergy = newEnergy;
    
    // 更新体力系统
    if (energySystem) {
        energySystem->setEnergy(newEnergy);
    }
    
    // 从背包中移除食物
    bool removedFromInventory = false;
    if (playerInventory) {
        // 尝试从主背包移除
        if (playerInventory->RemoveItem(*food) != -1) {
            removedFromInventory = true;
        }
    }
    
    if (removedFromInventory) {
        // 重置mini_bag的选择状态
        if (miniBag) {
            miniBag->getSelectBack();
        }
        
        // 更新UI显示
        updateEnergySystemUI();
        
        result.success = true;
        result.message = "Consumed " + result.itemName + ", gained " + 
                        std::to_string(result.energyGained) + " energy";
        
        CCLOG("GameFacade: Successfully consumed %s, energy: %d -> %d", 
              result.itemName.c_str(), oldEnergy, newEnergy);
    } else {
        result.message = "Failed to remove food from inventory";
        CCLOG("GameFacade: Failed to consume %s - could not remove from inventory", 
              result.itemName.c_str());
    }
    
    return result;
}

GameInteractionFacade::FoodConsumptionResult 
GameInteractionFacade::consumeFoodFromSlot(int selectedSlot, mini_bag* miniBag) {
    FoodConsumptionResult result;
    
    if (!miniBag || selectedSlot <= 0) {
        result.message = "Invalid slot or mini_bag reference";
        return result;
    }
    
    // 获取选中的物品
    auto selectedItem = miniBag->getSelectedItem();
    if (!selectedItem) {
        result.message = "No item selected in mini bag";
        return result;
    }
    
    // 检查是否为食物
    auto food = std::dynamic_pointer_cast<Food>(selectedItem);
    if (!food) {
        result.message = "Selected item is not food";
        return result;
    }
    
    // 调用主要的食物消费逻辑
    return consumeFood(food, miniBag);
}

bool GameInteractionFacade::validateFoodConsumption(std::shared_ptr<Food> food, mini_bag* miniBag) {
    if (!food) {
        CCLOG("GameFacade: Food object is null");
        return false;
    }
    
    if (!miniBag) {
        CCLOG("GameFacade: mini_bag reference is null");
        return false;
    }
    
    if (!isInitialized()) {
        CCLOG("GameFacade: Facade not properly initialized");
        return false;
    }
    
    return true;
}

void GameInteractionFacade::updateEnergySystemUI() {
    // UI更新现在通过Observer模式自动处理
}

bool GameInteractionFacade::hasEnoughEnergy(int requiredEnergy) {
    int current = getCurrentEnergy();
    return current >= requiredEnergy;
}

void GameInteractionFacade::consumeEnergy(int amount) {
    if (amount <= 0) return;
    
    int current = getCurrentEnergy();
    int newEnergy = std::max(0, current - amount);
    
    if (energySystem) {
        energySystem->setEnergy(newEnergy);
    }
    
    // 同步全局变量
    strength = newEnergy;
    
    // 更新UI
    updateEnergySystemUI();
    
    CCLOG("GameFacade: Consumed %d energy, current: %d", amount, newEnergy);
}

int GameInteractionFacade::getCurrentEnergy() const {
    if (energySystem) {
        return energySystem->getCurrentEnergy();
    }
    
    // 回退到全局变量
    return strength;
}

// ==================== 物品出售交互实现 ====================

GameInteractionFacade::ItemSaleResult 
GameInteractionFacade::sellItem(std::shared_ptr<Item> item, int quantity, mini_bag* miniBag) {
    ItemSaleResult result;
    
    if (!validateItemSale(item, quantity, miniBag)) {
        result.message = "Invalid parameters";
        return result;
    }
    
    result.itemName = item->GetName();
    result.quantity = quantity;
    int unitValue = item->GetValue();
    result.goldEarned = unitValue * quantity;
    
    // 1. 从背包移除物品
    bool removedFromInventory = false;
    if (playerInventory) {
        int removeResult = playerInventory->RemoveItem(*item, quantity);
        if (removeResult != -1) {
            removedFromInventory = true;
        }
    }
    
    if (removedFromInventory) {
        // 2. 使用单例增加金币
        // 这会自动触发 onEconomicStateChanged 通知
        EconomicSystem::getInstance()->addGold(result.goldEarned);
        
        result.currentGold = EconomicSystem::getInstance()->getGoldAmount();
        
        // 3. 重置 UI 状态
        if (miniBag) {
            miniBag->getSelectBack();
        }
        
        // 注意：无需手动 updateUI，观察者模式会自动处理
        
        result.success = true;
        result.message = "Sold " + std::to_string(quantity) + "x " + result.itemName;
        CCLOG("Facade: %s", result.message.c_str());
    } else {
        result.message = "Failed to remove items";
    }
    
    return result;
}

GameInteractionFacade::ItemSaleResult 
GameInteractionFacade::sellItemFromSlot(int selectedSlot, int quantity, mini_bag* miniBag) {
    ItemSaleResult result;
    
    if (!miniBag || selectedSlot <= 0) {
        result.message = "Invalid slot or mini_bag reference";
        return result;
    }
    
    // 获取指定槽位的物品
    auto item = playerInventory ? playerInventory->GetItemAt(selectedSlot) : nullptr;
    if (!item) {
        result.message = "No item in slot " + std::to_string(selectedSlot);
        return result;
    }
    
    // 获取该槽位的实际数量
    int availableQuantity = playerInventory ? playerInventory->GetItemCountAt(selectedSlot) : 0;
    if (availableQuantity == 0) {
        result.message = "No items available in slot " + std::to_string(selectedSlot);
        return result;
    }
    
    // 调整出售数量（不能超过可用数量）
    int actualQuantity = std::min(quantity, availableQuantity);
    
    // 调用主要的出售逻辑
    return sellItem(item, actualQuantity, miniBag);
}

GameInteractionFacade::ItemSaleResult 
GameInteractionFacade::sellSelectedItem(mini_bag* miniBag) {
    ItemSaleResult result;
    
    if (!miniBag) {
        result.message = "mini_bag reference is null";
        return result;
    }
    
    // 获取选中的槽位
    int selectedSlot = miniBag->getSelectedSlot();
    if (selectedSlot <= 0) {
        result.message = "No slot selected";
        return result;
    }
    
    // 获取该槽位的全部数量
    int quantity = playerInventory ? playerInventory->GetItemCountAt(selectedSlot) : 0;
    if (quantity <= 0) {
        result.message = "No items in selected slot";
        return result;
    }
    
    // 出售选中槽位的所有物品
    return sellItemFromSlot(selectedSlot, quantity, miniBag);
}

bool GameInteractionFacade::validateItemSale(std::shared_ptr<Item> item, int quantity, mini_bag* miniBag) {
    if (!item) {
        CCLOG("GameFacade: Item object is null");
        return false;
    }
    
    if (quantity <= 0) {
        CCLOG("GameFacade: Invalid quantity: %d", quantity);
        return false;
    }
    
    if (!miniBag) {
        CCLOG("GameFacade: mini_bag reference is null");
        return false;
    }
    
    if (!isInitialized()) {
        CCLOG("GameFacade: Facade not properly initialized");
        return false;
    }
    
    return true;
}

void GameInteractionFacade::updateEconomicSystemUI() {
    // 留空，由观察者模式处理
}
