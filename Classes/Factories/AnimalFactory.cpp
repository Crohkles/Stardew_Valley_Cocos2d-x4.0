#include "AnimalFactory.h"
#include "../Entities/Chicken.h"
#include "../Entities/Cow.h"
#include "../Entities/Sheep.h"
#include "cocos2d.h"

USING_NS_CC;

AnimalFactory* AnimalFactory::s_instance = nullptr;

AnimalFactory* AnimalFactory::getInstance() {
    if (s_instance == nullptr) {
        s_instance = new AnimalFactory();
    }
    return s_instance;
}

AnimalFactory::AnimalFactory() {
    initializeDefaultAnimals();
}

void AnimalFactory::initializeDefaultAnimals() {
    // Register default animal types with their creation functions
    // This replaces the hardcoded if-else chain in StoreUI
    
    registerAnimal("AnimalChicken", [](const cocos2d::Rect& area) -> Livestock* {
        return Chicken::create(area);
    });
    
    registerAnimal("AnimalCow", [](const cocos2d::Rect& area) -> Livestock* {
        return Cow::create(area);
    });
    
    registerAnimal("AnimalSheep", [](const cocos2d::Rect& area) -> Livestock* {
        return Sheep::create(area);
    });
    
    CCLOG("AnimalFactory: Initialized with %d animal types", static_cast<int>(m_animalCreators.size()));
}

Livestock* AnimalFactory::createAnimal(const std::string& animalType, const cocos2d::Rect& area) {
    auto it = m_animalCreators.find(animalType);
    if (it != m_animalCreators.end()) {
        Livestock* animal = it->second(area);
        if (animal) {
            CCLOG("AnimalFactory: Successfully created %s", animalType.c_str());
            return animal;
        } else {
            CCLOG("AnimalFactory: Failed to create %s - creation function returned null", animalType.c_str());
        }
    } else {
        CCLOG("AnimalFactory: Unknown animal type: %s", animalType.c_str());
    }
    return nullptr;
}

void AnimalFactory::registerAnimal(const std::string& animalType, CreateFunction createFunc) {
    if (createFunc) {
        m_animalCreators[animalType] = createFunc;
        CCLOG("AnimalFactory: Registered animal type: %s", animalType.c_str());
    } else {
        CCLOG("AnimalFactory: Warning - tried to register %s with null creation function", animalType.c_str());
    }
}

bool AnimalFactory::isAnimalTypeRegistered(const std::string& animalType) const {
    return m_animalCreators.find(animalType) != m_animalCreators.end();
}

std::vector<std::string> AnimalFactory::getRegisteredTypes() const {
    std::vector<std::string> types;
    types.reserve(m_animalCreators.size());
    
    for (const auto& pair : m_animalCreators) {
        types.push_back(pair.first);
    }
    
    return types;
}