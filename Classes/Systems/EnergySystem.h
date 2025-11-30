#ifndef ENERGY_SYSTEM_H
#define ENERGY_SYSTEM_H

#include "../Core/GameStateObserver.h"

// 全局体力状态管理器
class EnergySystem : public GameStateSubject {
private:
    static EnergySystem* instance;
    int currentEnergy;
    int maxEnergy;
    
    EnergySystem() : currentEnergy(100), maxEnergy(100) {}
    
public:
    static EnergySystem* getInstance() {
        if (instance == nullptr) {
            instance = new EnergySystem();
        }
        return instance;
    }
    
    void setEnergy(int energy) {
        if (energy != currentEnergy) {
            currentEnergy = energy;
            notifyEnergyStateChanged(currentEnergy, maxEnergy);
        }
    }
    
    void addEnergy(int amount) {
        int newEnergy = currentEnergy + amount;
        if (newEnergy > maxEnergy) {
            newEnergy = maxEnergy;
        }
        if (newEnergy != currentEnergy) {
            currentEnergy = newEnergy;
            notifyEnergyStateChanged(currentEnergy, maxEnergy);
        }
    }
    
    void reduceEnergy(int amount) {
        int newEnergy = currentEnergy - amount;
        if (newEnergy < 0) {
            newEnergy = 0;
        }
        if (newEnergy != currentEnergy) {
            currentEnergy = newEnergy;
            notifyEnergyStateChanged(currentEnergy, maxEnergy);
        }
    }
    
    int getCurrentEnergy() const { return currentEnergy; }
    int getMaxEnergy() const { return maxEnergy; }
    
    void setMaxEnergy(int max) {
        maxEnergy = max;
        if (currentEnergy > maxEnergy) {
            currentEnergy = maxEnergy;
            notifyEnergyStateChanged(currentEnergy, maxEnergy);
        }
    }
};

#endif // ENERGY_SYSTEM_H