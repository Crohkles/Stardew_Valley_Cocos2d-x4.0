#ifndef GAME_STATE_OBSERVER_H
#define GAME_STATE_OBSERVER_H

#include <vector>
#include <string>
#include <algorithm>
#include "../Items/Item.h"

// 观察者接口
class GameStateObserver {
public:
    virtual ~GameStateObserver() = default;
    
    // 经济状态变化通知
    virtual void onEconomicStateChanged(int newGoldAmount, int delta) {}
    
    // 背包状态变化通知
    virtual void onInventoryStateChanged() {}
    
    // 体力状态变化通知
    virtual void onEnergyStateChanged(int newEnergy, int maxEnergy) {}
    
    // NPC关系状态变化通知
    virtual void onRelationshipStateChanged(const std::string& npc, double newLevel) {}
    
    // 任务状态变化通知
    virtual void onTaskStateChanged(const std::string& taskName, int state) {}
};

// 被观察者基类
class GameStateSubject {
private:
    std::vector<GameStateObserver*> observers;
    
public:
    virtual ~GameStateSubject() {
        // 清空观察者列表，防止悬空指针
        observers.clear();
    }
    
    void addObserver(GameStateObserver* observer) {
        if (observer != nullptr) {
            observers.push_back(observer);
        }
    }
    
    void removeObserver(GameStateObserver* observer) {
        if (observer == nullptr) return;
        auto it = std::find(observers.begin(), observers.end(), observer);
        while (it != observers.end()) {
            observers.erase(it);
            it = std::find(observers.begin(), observers.end(), observer);
        }
    }
    
protected:
    void notifyEconomicStateChanged(int newGoldAmount, int delta) {
        // 创建观察者列表的副本以防止迭代过程中列表被修改
        auto observersCopy = observers;
        for (auto* observer : observersCopy) {
            if (observer != nullptr) {
                observer->onEconomicStateChanged(newGoldAmount, delta);
            }
        }
    }
    
    void notifyInventoryStateChanged() {
        // 创建观察者列表的副本以防止迭代过程中列表被修改
        auto observersCopy = observers;
        for (auto* observer : observersCopy) {
            if (observer != nullptr) {
                observer->onInventoryStateChanged();
            }
        }
    }
    
    void notifyEnergyStateChanged(int newEnergy, int maxEnergy) {
        // 创建观察者列表的副本以防止迭代过程中列表被修改
        auto observersCopy = observers;
        for (auto* observer : observersCopy) {
            if (observer != nullptr) {
                observer->onEnergyStateChanged(newEnergy, maxEnergy);
            }
        }
    }
    
    void notifyRelationshipStateChanged(const std::string& npc, double newLevel) {
        // 创建观察者列表的副本以防止迭代过程中列表被修改
        auto observersCopy = observers;
        for (auto* observer : observersCopy) {
            if (observer != nullptr) {
                observer->onRelationshipStateChanged(npc, newLevel);
            }
        }
    }
    
    void notifyTaskStateChanged(const std::string& taskName, int state) {
        // 创建观察者列表的副本以防止迭代过程中列表被修改
        auto observersCopy = observers;
        for (auto* observer : observersCopy) {
            if (observer != nullptr) {
                observer->onTaskStateChanged(taskName, state);
            }
        }
    }
};

#endif // GAME_STATE_OBSERVER_H