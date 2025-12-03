#include "EconomicSystem.h"  
#include "cocos2d.h"  

// 引用全局变量以保持兼容性
extern int GoldAmount;

EconomicSystem* EconomicSystem::instance = nullptr;

EconomicSystem::EconomicSystem() {
    // 初始化时同步全局变量
    this->goldAmount = GoldAmount;
}

EconomicSystem::~EconomicSystem() {
    // 析构时反向同步（保底）
    GoldAmount = this->goldAmount;
}

EconomicSystem* EconomicSystem::getInstance() {
    if (instance == nullptr) {
        instance = new EconomicSystem();
    }
    return instance;
}

void EconomicSystem::destroyInstance() {
    if (instance != nullptr) {
        delete instance;
        instance = nullptr;
    }
}

void EconomicSystem::addGold(int amount) {
    if (amount > 0) {
        goldAmount += amount;
        GoldAmount = goldAmount; // 实时同步
        CCLOG("EconomicSystem: Added %d gold. Total: %d", amount, goldAmount);
        // 通知观察者 (Timesystem, StoreUI 等)
        notifyEconomicStateChanged(goldAmount, amount);
    }
}

void EconomicSystem::subtractGold(int amount) {
    if (amount > 0 && amount <= goldAmount) {
        goldAmount -= amount;
        GoldAmount = goldAmount; // 实时同步
        CCLOG("EconomicSystem: Subtracted %d gold. Total: %d", amount, goldAmount);
        // 通知观察者
        notifyEconomicStateChanged(goldAmount, -amount);
    } else {
        CCLOG("EconomicSystem: Failed to subtract %d gold. Not enough money.", amount);
    }
}

int EconomicSystem::getGoldAmount() const {
    return goldAmount;
}