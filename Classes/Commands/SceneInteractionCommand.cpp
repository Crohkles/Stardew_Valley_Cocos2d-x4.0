#include "SceneInteractionCommand.h"
#include "../Entities/Player.h"
#include "../Scenes/farm.h"
#include "../Scenes/Myhouse.h"
#include "../Systems/FishingGame.h"
#include "../Core/AppDelegate.h"
#include "../Systems/EnergySystem.h"
#include "../Items/Generaltem.h"  // 引入flyweight物品系统

// ==================== SceneTransitionCommand 实现 ====================

SceneTransitionCommand::SceneTransitionCommand(Player* p, std::function<Scene*()> creator, const std::string& sceneName)
    : player(p), sceneCreator(creator), targetSceneName(sceneName) {
}

void SceneTransitionCommand::execute() {
    if (!sceneCreator) {
        CCLOG("Error: Scene creator is null");
        return;
    }
    
    CCLOG("Transitioning to scene: %s", targetSceneName.c_str());
    
    // 如果玩家存在，从当前场景中移除
    if (player) {
        player->removeFromParent();
    }
    
    // 创建新场景
    auto nextScene = sceneCreator();
    if (nextScene) {
        Director::getInstance()->replaceScene(nextScene);
    } else {
        CCLOG("Error: Failed to create target scene");
    }
}

void SceneTransitionCommand::undo() {
    // 场景切换通常不可撤销
}

bool SceneTransitionCommand::canExecute() const {
    return sceneCreator != nullptr;
}

std::string SceneTransitionCommand::getDescription() const {
    return "SceneTransition_" + targetSceneName;
}

// ==================== ConditionalSceneTransitionCommand 实现 ====================

ConditionalSceneTransitionCommand::ConditionalSceneTransitionCommand(
    Player* p, 
    std::function<Scene*()> creator, 
    std::function<bool()> condition,
    std::function<void()> preCallback,
    const std::string& sceneName)
    : player(p), sceneCreator(creator), conditionChecker(condition), 
      preTransitionCallback(preCallback), targetSceneName(sceneName) {
}

void ConditionalSceneTransitionCommand::execute() {
    if (!canExecute()) {
        return;
    }
    
    CCLOG("Conditional scene transition to: %s", targetSceneName.c_str());
    
    // 执行预切换回调
    if (preTransitionCallback) {
        preTransitionCallback();
    }
    
    // 如果玩家存在，从当前场景中移除
    if (player) {
        player->removeFromParent();
    }
    
    // 创建新场景
    auto nextScene = sceneCreator();
    if (nextScene) {
        Director::getInstance()->replaceScene(nextScene);
    } else {
        CCLOG("Error: Failed to create target scene");
    }
}

void ConditionalSceneTransitionCommand::undo() {
    // 场景切换通常不可撤销
}

bool ConditionalSceneTransitionCommand::canExecute() const {
    return sceneCreator != nullptr && conditionChecker != nullptr && conditionChecker();
}

std::string ConditionalSceneTransitionCommand::getDescription() const {
    return "ConditionalSceneTransition_" + targetSceneName;
}

// ==================== FarmingActionCommand 基类实现 ====================

FarmingActionCommand::FarmingActionCommand(farm* scene, const std::string& action)
    : farmScene(scene), actionType(action) {
}

bool FarmingActionCommand::canExecute() const {
    return farmScene != nullptr;
}

std::string FarmingActionCommand::getDescription() const {
    return "FarmingAction_" + actionType;
}

// ==================== PlantCommand 实现 ====================

PlantCommand::PlantCommand(farm* scene) : FarmingActionCommand(scene, "plant") {
}

void PlantCommand::execute() {
    if (!farmScene) {
        return;
    }
    
    CCLOG("Executing plant command");
    farmScene->performPlantAction();
}

void PlantCommand::undo() {
    // 种植操作通常不支持撤销
}

// ==================== HarvestCommand 实现 ====================

HarvestCommand::HarvestCommand(farm* scene) : FarmingActionCommand(scene, "harvest") {
}

void HarvestCommand::execute() {
    if (!farmScene) {
        return;
    }
    
    CCLOG("Executing harvest command");
    farmScene->performHarvestAction();
}

void HarvestCommand::undo() {
    // 收获操作通常不支持撤销
}

// ==================== WaterCommand 实现 ====================

WaterCommand::WaterCommand(farm* scene) : FarmingActionCommand(scene, "water") {
}

void WaterCommand::execute() {
    if (!farmScene) {
        return;
    }
    
    CCLOG("Executing water command");
    farmScene->performWaterAction();
}

void WaterCommand::undo() {
    // 浇水操作通常不支持撤销
}

// ==================== FishingCommand 实现 ====================

FishingCommand::FishingCommand(Scene* scene, Player* p)
    : currentScene(scene), player(p) {
}

void FishingCommand::execute() {
    if (!currentScene || !player) {
        return;
    }
    
    // 检查是否已经有钓鱼游戏在进行
    if (currentScene->getChildByName("FishingGameLayer")) {
        CCLOG("Fishing game already active");
        return;
    }
    
    // 检查体力是否足够
    if (strength < 10) {
        CCLOG("Not enough energy for fishing");
        return;
    }
    
    CCLOG("Starting fishing game");
    
    // 消耗体力
    strength -= 10;
    EnergySystem::getInstance()->setEnergy(strength);
    
    // 创建钓鱼游戏界面
    auto fishing_game = FishingGame::create(player->getPosition());
    if (fishing_game) {
        currentScene->addChild(fishing_game, 10, "FishingGameLayer");
    }
}

void FishingCommand::undo() {
    // 钓鱼操作通常不支持撤销
}

bool FishingCommand::canExecute() const {
    return currentScene != nullptr && player != nullptr && 
           strength >= 10 && 
           !currentScene->getChildByName("FishingGameLayer");
}

std::string FishingCommand::getDescription() const {
    return "Fishing";
}

// ==================== MiningCommand 实现 ====================

MiningCommand::MiningCommand(Scene* scene) : currentScene(scene) {
}

void MiningCommand::execute() {
    if (!currentScene) {
        return;
    }
    
    CCLOG("Executing mining command");
    
    // 获取玩家位置
    Vec2 playerPos = player1->getPosition();
    
    // 遍历矿物信息列表进行挖矿检测
    for (auto it = Ore_information.begin(); it != Ore_information.end(); ++it) {
        auto ore = *it;
        
        float distance = ore->position.distance(playerPos);
        
        // 检查距离、可用性和体力
        if (distance <= 75 && ore->available && strength >= 10) {
            
            strength -= 10;
            TimeUI->UpdateEnergy();
            
            skill_tree->AddExperience(mining_skill, 10);
            
            ore->available = false;
            ore->mining_day = season[Season] * 7 + day;
            
            // 根据矿物类型添加物品
            if (ore->GetName() == "Emerald") {
                if (GoldMaskfirst) {
                    inventory->AddItem(*GetGoldMask());
                    GoldMaskfirst = false;
                }
                inventory->AddItem(*GetEmerald());
            }
            else if (ore->GetName() == "Ruby") {
                inventory->AddItem(*GetRuby());
            }
            else {
                inventory->AddItem(*GetAmethyst());
            }
            
            // 角色挖矿动画
            if (player1->pic_path == "character1/player_right3.png") {
                player1->setTexture("character1/player_plant3.png");
                player1->setScale(3.9f);
                
                // 延迟0.3秒后切换到第二个图片
                player1->scheduleOnce([=](float dt) {
                    player1->setTexture("character1/player_plant4.png");
                    player1->setScale(4.1f);
                }, 0.15f, "change_image1_key");
                
                // 延迟0.6秒后切换到第三个图片并显示挖矿效果
                player1->scheduleOnce([=](float dt) {
                    player1->setTexture("character1/player_right3.png");
                    player1->setScale(2.3f);
                    auto earth = Sprite::create("Ore/earth.png");
                    currentScene->addChild(earth, 5);
                    earth->setPosition(ore->position);
                    earth->setScale(2.9f);
                    auto temp = Sprite::create(ore->mining_pic);
                    currentScene->addChild(temp, 6);
                    temp->setPosition(ore->position);
                    temp->setScale(2.7f);
                }, 0.35f, "change_image2_key");
            }
            else {
                player1->setTexture("character1/player_plant1.png");
                player1->setScale(3.9f);
                
                // 延迟0.3秒后切换到第二个图片
                player1->scheduleOnce([=](float dt) {
                    player1->setTexture("character1/player_plant2.png");
                    player1->setScale(4.1f);
                }, 0.15f, "change_image1_key");
                
                // 延迟0.6秒后切换到第三个图片并显示挖矿效果
                player1->scheduleOnce([=](float dt) {
                    player1->setTexture("character1/player_left3.png");
                    player1->setScale(2.3f);
                    auto earth = Sprite::create("Ore/earth.png");
                    currentScene->addChild(earth, 5);
                    earth->setPosition(ore->position);
                    earth->setScale(2.9f);
                    auto temp = Sprite::create(ore->mining_pic);
                    currentScene->addChild(temp, 6);
                    temp->setPosition(ore->position);
                    temp->setScale(2.7f);
                }, 0.35f, "change_image2_key");
            }
            
            break;  // 找到一个可挖的矿物后就退出循环
        }
    }
}

void MiningCommand::undo() {
    // 挖矿操作通常不支持撤销
}

bool MiningCommand::canExecute() const {
    return currentScene != nullptr;
}

std::string MiningCommand::getDescription() const {
    return "Mining";
}

// ==================== LoggingCommand 实现 ====================

LoggingCommand::LoggingCommand(Scene* scene) : currentScene(scene) {
}

void LoggingCommand::execute() {
    if (!currentScene) {
        return;
    }
    
    CCLOG("Executing logging command");
    
    // 获取玩家位置
    Vec2 playerPos = player1->getPosition();
    
    // 遍历树木信息列表进行伐木检测
    for (auto it = Tree_information.begin(); it != Tree_information.end(); ++it) {
        auto tree = *it;
        
        float distance = tree->position.distance(playerPos);
        
        // 检查距离、可用性和体力
        if ((distance <= 250 && tree->available) && strength >= 10) {
            
            strength -= 10;
            TimeUI->UpdateEnergy();
            
            // 根据技能等级减少砍伐次数
            if (skill_tree->GetSkillLevels()[foraging_skill] >= 5) {
                tree->removetimes -= 2;
            }
            else {
                tree->removetimes -= 1;
            }
            
            // 如果树被完全砍倒
            if (tree->removetimes <= 0) {
                inventory->AddItem(*GetWood());
                
                // 升级经验
                skill_tree->AddExperience(foraging_skill, 10);
                
                cocos2d::log("Cut Down");
                
                tree->available = false;
                tree->mining_day = season[Season] * 7 + day;
                
                // 角色砍伐动画（树倒下）
                if (player1->pic_path == "character1/player_right3.png") {
                    player1->setTexture("character1/player_plant3.png");
                    player1->setScale(3.5f);
                    
                    // 延迟0.3秒后切换到第二个图片
                    player1->scheduleOnce([=](float dt) {
                        player1->setTexture("character1/player_plant4.png");
                        player1->setScale(3.7f);
                    }, 0.15f, "change_image1_key");
                    
                    // 延迟0.6秒后切换到第三个图片并显示倒下的树
                    player1->scheduleOnce([=](float dt) {
                        player1->setTexture("character1/player_right3.png");
                        player1->setScale(2.3f);
                        auto temp = Sprite::create(tree->G_Cut_pic);
                        currentScene->addChild(temp, 5);
                        temp->setPosition(tree->position);
                        temp->setScale(3.1f);
                    }, 0.35f, "change_image2_key");
                }
                else {
                    player1->setTexture("character1/player_plant1.png");
                    player1->setScale(3.5f);
                    
                    // 延迟0.3秒后切换到第二个图片
                    player1->scheduleOnce([=](float dt) {
                        player1->setTexture("character1/player_plant2.png");
                        player1->setScale(3.7f);
                    }, 0.15f, "change_image1_key");
                    
                    // 延迟0.6秒后切换到第三个图片并显示倒下的树
                    player1->scheduleOnce([=](float dt) {
                        player1->setTexture("character1/player_left3.png");
                        player1->setScale(2.3f);
                        auto temp = Sprite::create(tree->G_Cut_pic);
                        currentScene->addChild(temp, 5);
                        temp->setPosition(tree->position);
                        temp->setScale(3.1f);
                    }, 0.35f, "change_image2_key");
                }
            }
            else {
                // 树没有完全砍倒，只显示砍伐动画
                if (player1->pic_path == "character1/player_right3.png") {
                    player1->setTexture("character1/player_plant3.png");
                    player1->setScale(3.5f);
                    
                    // 延迟0.3秒后切换到第二个图片
                    player1->scheduleOnce([=](float dt) {
                        player1->setTexture("character1/player_plant4.png");
                        player1->setScale(3.7f);
                    }, 0.15f, "change_image1_key");
                    
                    // 延迟0.6秒后恢复原状态
                    player1->scheduleOnce([=](float dt) {
                        player1->setTexture("character1/player_right3.png");
                        player1->setScale(2.3f);
                    }, 0.35f, "change_image2_key");
                }
                else {
                    player1->setTexture("character1/player_plant1.png");
                    player1->setScale(3.5f);
                    
                    // 延迟0.3秒后切换到第二个图片
                    player1->scheduleOnce([=](float dt) {
                        player1->setTexture("character1/player_plant2.png");
                        player1->setScale(3.7f);
                    }, 0.15f, "change_image1_key");
                    
                    // 延迟0.6秒后恢复原状态
                    player1->scheduleOnce([=](float dt) {
                        player1->setTexture("character1/player_left3.png");
                        player1->setScale(2.3f);
                    }, 0.35f, "change_image2_key");
                }
            }
            
            break;  // 找到一个可砍的树后就退出循环
        }
    }
}

void LoggingCommand::undo() {
    // 伐木操作通常不支持撤销
}

bool LoggingCommand::canExecute() const {
    return currentScene != nullptr;
}

std::string LoggingCommand::getDescription() const {
    return "Logging";
}

// ==================== SleepCommand 实现 ====================

SleepCommand::SleepCommand(Myhouse* scene) : myhouseScene(scene) {
}

void SleepCommand::execute() {
    if (!myhouseScene) {
        CCLOG("Error: Myhouse scene is null");
        return;
    }
    
    CCLOG("Executing sleep command");
    
    // 这里需要访问Myhouse的睡觉逻辑
    // 由于原来的睡觉逻辑比较复杂，暂时使用场景切换
    // 实际的睡觉逻辑需要在Myhouse类中实现一个public方法
    
    // 设置下一天标志
    extern bool IsNextDay;
    extern int day;
    extern std::string Season;
    
    IsNextDay = true;
    day++;
    
    if (day == 8) {
        if (Season == "Spring") {
            Season = "Summer";
        }
        else if (Season == "Summer") {
            Season = "Autumn";
        }
        else {
            Season = "Winter";
        }
        day = 1;
    }
    
    // 触发场景重置
    auto nextday = Myhouse::create();
    Director::getInstance()->replaceScene(TransitionFade::create(3.0f, nextday));
}

void SleepCommand::undo() {
    // 睡觉通常不可撤销
}

bool SleepCommand::canExecute() const {
    if (!myhouseScene) return false;
    
    // 检查是否在床区域内
    // 这需要获取玩家位置，暂时返回true
    return true;
}

std::string SleepCommand::getDescription() const {
    return "Sleep_Command";
}

// ==================== StoreInteractionCommand 实现 ====================

StoreInteractionCommand::StoreInteractionCommand(Scene* scene) : currentScene(scene) {
}

void StoreInteractionCommand::execute() {
    if (!currentScene) {
        return;
    }
    
    CCLOG("Executing store interaction command");
    // 这里需要实现商店交互逻辑
    // 原代码中主要是打开/关闭商店界面
}

void StoreInteractionCommand::undo() {
    // 商店交互操作通常不支持撤销
}

bool StoreInteractionCommand::canExecute() const {
    return currentScene != nullptr;
}

std::string StoreInteractionCommand::getDescription() const {
    return "StoreInteraction";
}