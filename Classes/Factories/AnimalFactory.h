#pragma once
#include "cocos2d.h"
#include "../Entities/Livestock.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <functional>

/**
 * @brief Abstract Factory for creating Livestock animals
 * 
 * This factory pattern eliminates the if-else chain in StoreUI
 * and provides a clean way to add new animal types without
 * modifying existing code.
 */
class AnimalFactory {
public:
    // Factory function type
    using CreateFunction = std::function<Livestock*(const cocos2d::Rect&)>;
    
    /**
     * @brief Get the singleton instance of AnimalFactory
     * @return AnimalFactory instance
     */
    static AnimalFactory* getInstance();
    
    /**
     * @brief Create an animal by type name
     * @param animalType The type string (e.g., "AnimalChicken", "AnimalCow", "AnimalSheep")
     * @param area The area rectangle for the animal
     * @return Livestock pointer or nullptr if type not found
     */
    Livestock* createAnimal(const std::string& animalType, const cocos2d::Rect& area);
    
    /**
     * @brief Register a new animal type with its creation function
     * @param animalType The type string to register
     * @param createFunc The function to create this type of animal
     */
    void registerAnimal(const std::string& animalType, CreateFunction createFunc);
    
    /**
     * @brief Check if an animal type is registered
     * @param animalType The type string to check
     * @return true if registered, false otherwise
     */
    bool isAnimalTypeRegistered(const std::string& animalType) const;
    
    /**
     * @brief Get all registered animal types
     * @return Vector of registered type names
     */
    std::vector<std::string> getRegisteredTypes() const;
    
private:
    AnimalFactory();
    ~AnimalFactory() = default;
    
    // Prevent copying
    AnimalFactory(const AnimalFactory&) = delete;
    AnimalFactory& operator=(const AnimalFactory&) = delete;
    
    /**
     * @brief Initialize default animal types
     */
    void initializeDefaultAnimals();
    
    static AnimalFactory* s_instance;
    std::unordered_map<std::string, CreateFunction> m_animalCreators;
};

// Convenience macros for registering animals
#define REGISTER_ANIMAL(type, className) \
    AnimalFactory::getInstance()->registerAnimal(#type, [](const cocos2d::Rect& area) -> Livestock* { \
        return className::create(area); \
    })

#define REGISTER_ANIMAL_WITH_NAME(name, className) \
    AnimalFactory::getInstance()->registerAnimal(name, [](const cocos2d::Rect& area) -> Livestock* { \
        return className::create(area); \
    })