#include "../Core/AppDelegate.h"
#include "Myhouse.h"
#include "Barn.h"
#include "farm.h"
#include "Cave.h"
#include "../Entities/Crop.h"
#include "../Items/CookingPot.h"
#include "../Entities/Player.h"
#include "../Core/BasicInformation.h" 
#include "physics/CCPhysicsWorld.h"
#include "ui/CocosGUI.h"
#include "vector"
#include "../UI/mailBoxUI.h"
#include "../Core/InputManager.h"
#include "../Commands/SceneInteractionCommand.h"
#include "../Commands/UICommand.h"
#include "../Systems/GameInteractionFacade.h"
#include "Myhouse.h"
#include "Barn.h"
#include "Cave.h"
#include "Forest.h"
#include "HelloWorldScene.h"
#include "Town.h"
#include "Beach.h"
#include "Forest.h"

USING_NS_CC;

farm::farm () {}

farm::~farm () {
    cleanupInputCommands();
    
    // 清理作物精灵池
    CropSpritePool::getInstance()->clearAllCrops();
}

bool farm::init ()
{
    AudioEngine::pauseAll ();
    auto backgroundAudioID = AudioEngine::play2d ( "MUSIC/TheShire.mp3" , true );

    auto visibleSize = Director::getInstance ()->getVisibleSize ();
    Vec2 origin = Director::getInstance ()->getVisibleOrigin ();

    button = cocos2d::Sprite::create ( "CloseNormal.png" );
    this->addChild ( button , 11 );

    // 创建并添加时间显示 UI（农场场景）
    TimeUI = Timesystem::create ( "farm" );
    this->addChild ( TimeUI , 13 );

    // 初始化所有作物
    AllInitialize_crop ();

    if (Weather == "Rainy") {
        // 雨天时创建雨滴特效
        createRainEffect ();
    }

    // 创建农场实际背景
    auto background_real = Sprite::create ( "farm/farm.png" );
    background_real->setPosition ( Vec2 ( visibleSize.width / 2 , visibleSize.height / 2 ) );
    this->addChild ( background_real , 1 );
    background_real->setScale ( 1.5f );

    auto background = Sprite::create ( "farm/farm_path.png" );
    this->addChild ( background , 5 );
    background->setPosition ( Vec2 ( visibleSize.width / 2 , visibleSize.height / 2 ) );
    background->setScale ( 1.5f );


    Vec2 spritePosition = background->getPosition ();   // 背景精灵的世界坐标
    Size spriteSize = background->getContentSize ();    // 背景图片的原始宽高

    // 计算背景左下角在世界坐标中的位置
    Vec2 leftBottomPosition = Vec2 (
        spritePosition.x - background->getScaleX () * spriteSize.width / 2 ,   // 左下角 x 坐标
        spritePosition.y - background->getScaleY () * spriteSize.height / 2   // 左下角 y 坐标
    );


    Image img;
    if (img.initWithImageFile ( "farm/farm_path.png" ))
    {
        int width = img.getWidth ();
        int height = img.getHeight ();

        // 获取像素数据
        unsigned char* data = img.getData ();

        // 以 8 像素为步长遍历整张图片，查找 alpha 不为 0 的像素
        for (int y = 0; y < height; y = y + 8)
        {
            for (int x = 0; x < width; x = x + 8)
            {
                // 每个像素由 4 个字节组成（RGBA）
                int index = (y * width + x) * 4;  // 每个像素占用 4 个字节 (RGBA)
                unsigned char a = data[index + 3];  // alpha 通道

                // 如果 alpha（不透明度）大于 0，则认为是有效碰撞点
                if (a > 0)
                {
                    float screenX = leftBottomPosition.x + x * background->getScaleX ();
                    float screenY = leftBottomPosition.y + (height - y - 1) * background->getScaleY ();  // 转换为世界坐标中的 Y 值
                    nonTransparentPixels.push_back ( Vec2 ( screenX , screenY ) );  // 记录非透明像素的世界坐标
                }
            }
        }
    }

    // 如果玩家尚未被添加到场景中，则在此场景中创建玩家
    if (player1->getParent () == NULL) {
        this->addChild ( player1 , 17 );
        // 初始化玩家的输入绑定
        player1->setupInputBindings();
        // 设置用于碰撞检测的非透明像素集合
        player1->setCollisionContext(nonTransparentPixels);
        for (auto& pair : F_lastplace) {
            if (pair.second == true) {  // 如果该场景被标记为上次停留位置
                player1->setPosition ( pair.first.second );
                pair.second = false;
            }
        }
         player1->speed = 4.7f;
         // 调试时可临时提高玩家速度
       /* player1->speed = 20.0f;*/
        player1->setScale ( 1.5f );
        player1->setAnchorPoint ( Vec2 ( 0.5f , 0.2f ) );
    }

    // 定时更新玩家移动
    player1->schedule ( [=]( float dt ) {
        player1->player1_move ();
        } , 0.05f , "player1_move" );

    player1->schedule ( [=]( float dt ) {
        player1->player_change ();
        } , 0.3f , "player_change" );


    // 计算缩放后的背景宽高
    float scaledWidth = background->getContentSize ().width * background->getScaleX ();
    float scaledHeight = background->getContentSize ().height * background->getScaleY ();

    // 设置 Follow 相机可移动的矩形区域
    auto followRect = cocos2d::Rect ( leftBottomPosition.x , leftBottomPosition.y , scaledWidth , scaledHeight );

    // 创建相机跟随动作，使场景在指定范围内跟随玩家
    auto followAction = Follow::create ( player1 , followRect );
    this->runAction ( followAction );

    // 定时检查玩家位置（用于时间 UI、箱子开关、睡觉等逻辑）
    this->schedule ( [this]( float dt ) {
        this->checkPlayerPosition ();  // 检查玩家位置并更新相关状态
        } , 0.01f , "check_position_key" );

    // 创建邮箱精灵
    auto mailBox = Sprite::create ( "UIresource/xinxiang/xinxiang.png" );
    mailBox->setPosition ( Vec2 ( 260 , 1050 ) );
    mailBox->setScale ( 0.7f );
    this->addChild ( mailBox , 10 );

    // 创建收货箱（出货箱）精灵
    Box = Sprite::create ( "UIresource/xiangzi/xiangzi.png" );
    Box->setPosition ( Vec2 ( 260 , 1150 ) );
    Box->setAnchorPoint ( Vec2 ( 0 , 0 ) );
    Box->setScale ( 0.7f );
    this->addChild ( Box , 10 );

    // 创建烹饪锅对象
    auto cooking_pot = CookingPot::create ();
    cooking_pot->setPosition ( -50 , 1000 );
    cooking_pot->setAnchorPoint ( Vec2 ( 0 , 0 ) );
    cooking_pot->setScale ( 2.7f );
    this->addChild ( cooking_pot , 10 );
    

    // 玩家与交互物体之间的交互半径
    float interactionRadius = 200.0f;

    auto listener = EventListenerMouse::create ();
    listener->onMouseDown = [this , mailBox , interactionRadius, cooking_pot]( Event* event ) {

        // 将原始事件转换为鼠标事件
        auto mouseEvent = static_cast<EventMouse*>(event);
        Vec2 clickPos ( mouseEvent->getCursorX () , mouseEvent->getCursorY () );
        clickPos = this->convertToNodeSpace ( clickPos );

        if (Box->getBoundingBox ().containsPoint ( clickPos )) {

            // 获取玩家当前位置  
            Vec2 playerPos = player1->getPosition ();

            Vec2 BoxPos = Box->getPosition ();

            // 计算玩家与箱子之间的距离  
            float distance = playerPos.distance ( BoxPos );

            // 如果玩家在交互半径内，则允许进行出售操作  
            if (distance <= interactionRadius) {
                if (miniBag->getSelectedSlot ()) {
                    // 使用外观类统一处理物品出售逻辑
                    auto facade = GameInteractionFacade::getInstance();
                    auto result = facade->sellSelectedItem(miniBag);
                    
                    if (result.success) {
                        CCLOG("Farm Box Sale: %s", result.message.c_str());
                    } else {
                        CCLOG("Farm Box Sale Failed: %s", result.message.c_str());
                    }
                }
            }
        }

        // 点击烹饪锅时的交互逻辑
        if (cooking_pot->getBoundingBox ().containsPoint ( clickPos )) {
            auto selected_item = miniBag->getSelectedItem ();
            if (selected_item != nullptr && std::dynamic_pointer_cast<Food>(selected_item) != nullptr) {
                cooking_pot->AddIngredient ( *selected_item );
            }
            else if (selected_item == nullptr) {
                auto id = cooking_pot->GetDishesId ();
                auto dishes = cooking_pot->GetDishes ( id );
                if (dishes != nullptr) {
                    auto size = cooking_pot->GetPotSize ();
                    bool can_get_dishes = true;
                    for (int i = 0; i < size; i++) {
                        auto ingredient = cooking_pot->GetPotBack ();
                        if (inventory->RemoveItem ( *ingredient ) == -1) {
                            can_get_dishes = false;
                            break;
                        }
                    }
                    if (can_get_dishes) {
                        inventory->AddItem ( *dishes );
                    }
                }
                cooking_pot->ClearPot ();
            }
            cooking_pot->DisplayPotInCCLOG ();
        }

        // 处理与关闭按钮、邮箱的点击交互
        if (button != nullptr && button->getBoundingBox ().containsPoint ( clickPos )) {
            Director::getInstance ()->end ();
        }
        if (mailBox->getBoundingBox ().containsPoint ( clickPos )) {
            // 获取玩家当前位置  
            Vec2 playerPos = player1->getPosition ();

            // 计算玩家与邮箱之间的距离  
            float distance = playerPos.distance ( mailBox->getPosition () );
            // 如果在交互半径内则打开邮箱 UI  
            if (distance <= interactionRadius) {
                mailBoxUI* mailbox = mailBoxUI::create ();
                this->addChild ( mailbox , 20 );
            }
        }
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority ( listener , this );

    // 将农场场景的交互逻辑接入 Command Pattern
    
    // 设置农场场景中用到的所有 Command Pattern 绑定
    setupInputCommands();

    // 创建并添加小背包 UI
    miniBag = mini_bag::create ( inventory );
    miniBag->setScale ( 1.0f );
    Vec2 pos = miniBag->getPosition ();
    if (miniBag != NULL) {
        cocos2d::log ( "miniBagtest %f" , pos.x );
    }
    if (!this->getChildByName ( "mini_bag" )) {
        this->addChild ( miniBag , 10 , "mini_bag" );
    }


    // 通过 Observer 模式：当背包内容发生变化时自动刷新小背包显示
    if (inventory && miniBag) {
        inventory->addObserver(miniBag);
    }

    return true;
}


farm* farm::create ()
{
    farm* scene = new farm ();
    if (scene && scene->init ())
    {
        scene->autorelease ();
        return scene;
    }
    CC_SAFE_DELETE ( scene );
    return nullptr;
}

void farm::AllInitialize_crop () {
    auto cropPool = CropSpritePool::getInstance();
    
    CCLOG("Farm: Initializing crops using sprite pool...");

    for (auto it = Crop_information.begin (); it != Crop_information.end (); ++it) {
        auto crop = *it;  // 解引用迭代器以访问 Crop 对象
        int nums = crop->nums;

        cocos2d::log ( "set nums = %d" , nums );

        // 作物类型
        std::string type = crop->GetName ();
        std::string imagePath;
        float scale = 2.1f;

        // 根据作物阶段确定图片路径和缩放
        if (crop->GetPhase () == Phase::MATURE) {
            imagePath = crop->mature_pic;
            scale = 1.9f;
        }
        else if (crop->GetPhase () == Phase::GROWING) {
            imagePath = crop->growing_pic;
        }
        else if (crop->GetPhase () == Phase::SAPLESS) {
            imagePath = "crop/sapless.png";
        }
        else {
            imagePath = crop->initial_pic;
        }

        // 使用对象池获取Sprite
        auto sprite = cropPool->getCropSprite(nums, imagePath);
        if (sprite) {
            this->addChild ( sprite , 15 - nums / 19 );
            sprite->setPosition ( 545 + ((nums % 19) - 2) * 48 , 910 - ((nums / 19) - 1) * 48 );
            sprite->setScale ( scale );
        }
        else {
            CCLOG("Farm: Failed to create sprite for crop at nums %d", nums);
        }
    }
    
    CCLOG("Farm: Crop initialization complete. Active crops: %zu, Recycled sprites: %zu", 
    cropPool->getActiveCropCount(), cropPool->getRecycledSpriteCount());
}

// 检查玩家位置并根据结果更新 UI、箱子状态和天数等
void farm::checkPlayerPosition ()
{

    // 获取玩家当前位置
    Vec2 playerPos = player1->getPosition ();
    // 当玩家在屏幕下方区域时，将小背包置于更低层级，避免遮挡角色
    if (playerPos.y < 0) {
        miniBag->setLocalZOrder ( 0 );
    }
    else {
        miniBag->setLocalZOrder ( 11 );  
    }


    // 计算玩家与收货箱的距离  
    float distance = playerPos.distance ( Box->getPosition () );
    // 若在一定范围内，则显示为打开箱子  
    if (distance <= 200.0f) {
        Box->setTexture ( "UIresource/xiangzi/Open.png" );
    }
    else {
        Box->setTexture ( "UIresource/xiangzi/xiangzi.png" );
    }

    // 剩余时间递增，达到阈值后进入新的一天
    remainingTime++;
    if (remainingTime == 43200 || strength == 0) {

        day++;

        IsNextDay = true;

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

        if (day % 3 == 1) {
            Weather = "Rainy";
        }
        else {
            Weather = "Sunny";
        }

        if ((Season == "Spring") && (day == 1)) {
            Festival = "Fishing Day";
        }
        else {
            Festival = "Noraml Day";
        }


        for (auto it = Crop_information.begin (); it != Crop_information.end ();) {

            auto crop = *it;  // 解引用迭代器以访问 Crop 对象

            if (day == 1) {
                crop->watered = true;
            }
            if ((day - 1) % 3 == 1) {
                crop->watered = true;
            }

            // 根据浇水与否更新作物的生长状态
            if ((crop->watered == false) && (crop->GetPhase () != Phase::MATURE)) {
                // 未浇水且未成熟，则作物进入枯萎或退化逻辑
                if (crop->GetPhase () != Phase::SAPLESS) {
                    crop->ChangePhase ( Phase::SAPLESS );
                    crop->ChangMatureNeeded ( 2 ); // 增加成熟所需天数
                    it++;
                }
                else {
                    // 已经处于枯萎阶段，则直接将其从列表中移除
                    it = Crop_information.erase ( it );
                }

            }
            else {
                // 正常生长流程
                crop->UpdateGrowth ();
                it++;
            }

        }

        for (auto& pair : F_lastplace) {
            if (pair.first.first == "myhouse") {  // 标记从农场回到房子的状态
                pair.second = true;
            }
        }

        IsSleep = false;
        frombed = true;
        remainingTime = 10800;
        player1->removeFromParent ();
        auto nextday = Myhouse::create ();
        Director::getInstance ()->replaceScene ( nextday );

    }
    // 计算时间 UI、关闭按钮、小背包的位置，使其跟随玩家
    float currentx = 0 , currenty = 0;
    if (playerPos.x <= 637) {
        currentx = 637;
    }
    else if (playerPos.x >= 960) {
        currentx = 960;
    }
    else {
        currentx = playerPos.x;
    }

    if (playerPos.y >= 777) {
        currenty = 777;
    }
    else if (playerPos.y <= 500) {
        currenty = 500;
    }
    else {
        currenty = playerPos.y;
    }

    TimeUI->setPosition ( currentx , currenty );
    button->setPosition ( currentx + 730 , currenty - 590 );
    miniBag->setPosition ( currentx , currenty );

    // 若玩家在种地区域内，则可以响应种地相关的按键（P/W/G）
    if (plant_area.containsPoint ( playerPos )) {

        // 这里可以为种地（P）、浇水（W）、收获（G）等 Command 添加提示或调试信息

    }

    // ENTER 对应的具体行为由 Command Pattern（setupInputCommands）统一管理
    // Player 的移动权限与碰撞检测由 setCollisionContext / player1_move() 等函数配合完成
}

int farm::getRegionNumber ( Vec2 pos ) {

    // 根据每日循环和浇水情况更新作物状态
int left_bottom_x = 496;  // 左下角 x 坐标
int left_bottom_y = 467;  // 左下角 y 坐标
int width = 912;          // 区域宽度
int height = 480;         // 区域高度
int block_size = 48;      // 每个小格的边长

// 计算行列数量
int rows = height / block_size;  // 行数
int cols = width / block_size;   // 列数

// 根据坐标换算所属行列
    int col = (pos.x - left_bottom_x) / block_size;
    int row = (left_bottom_y + height - pos.y) / block_size;
// 防止下标越界
    if (col < 0) {
        col = 0;
    }
    if (row < 0) {
        row = 0;
    }
// 计算区域编号（从 1 开始）
int region_number = (row)*cols + col + 1;  // 区域号从 1 开始

    return region_number;
}

void farm::createRainEffect () {

    emitter = ParticleRain::create ();
    emitter->setDuration ( ParticleSystem::DURATION_INFINITY );
    emitter->setScale ( 5.7f );
    emitter->setTotalParticles ( 100 );
    emitter->setSpeed ( 250 );

    addChild ( emitter , 10 );

    // 定时更新雨滴属性
    schedule ( [this]( float dt ) {
        updaterain ( dt );
        } , "update_rain_key" );

}


void farm::updaterain ( float deltaTime ) {
    if (emitter) {
        // 将雨滴生命周期设置为 0 ~ 1.5 之间的随机值
        float newLife = cocos2d::rand_0_1 () * 1.5f;

        // 根据新的生命周期更新发射速率
        emitter->setLife ( newLife );

        emitter->setEmissionRate ( emitter->getTotalParticles () / emitter->getLife () * 1.3 );
    }
}

void farm::setupInputCommands() {
    auto inputManager = InputManager::getInstance();

    // 创建并注册种植、浇水、收获和背包开关命令
    auto plantCommand = std::make_shared<PlantCommand>(this);
    auto waterCommand = std::make_shared<WaterCommand>(this);
    auto harvestCommand = std::make_shared<HarvestCommand>(this);
    auto toggleInventoryCommand = std::make_shared<ToggleInventoryCommand>(
        this, inventory, "farm"
    );

    // 退出农场命令 - 使用 ConditionalSceneTransitionCommand 封装场景切换
    auto exitFarmCommand = std::make_shared<ConditionalSceneTransitionCommand>(
        player1, 
        []() -> cocos2d::Scene* { return HelloWorld::create(); },
        [this]() -> bool { 
            Vec2 playerPos = player1->getPosition();
            return Out_Farm.containsPoint(playerPos);
        },
        nullptr,
        "farm_to_world"
    );

    auto enterHouseCommand = std::make_shared<ConditionalSceneTransitionCommand>(
        player1,
        [this]() -> cocos2d::Scene* { 
            // 进入房子前，记录玩家上一次在农场的位置
            for (auto& pair : F_lastplace) {
                if (pair.first.first == "myhouse") {
                    pair.second = true;
                }
            }
            player1->removeFromParent();
            return Myhouse::create();
        },
        [this]() -> bool {
            Vec2 playerPos = player1->getPosition();
            return myhouse_area.containsPoint(playerPos);
        },
        nullptr,
        "farm_to_house"
    );

    auto enterBarnCommand = std::make_shared<ConditionalSceneTransitionCommand>(
        player1,
        [this]() -> cocos2d::Scene* {
            // 进入牛棚前，记录玩家上一次在农场的位置
            for (auto& pair : F_lastplace) {
                if (pair.first.first == "barn") {
                    pair.second = true;
                }
            }
            player1->removeFromParent();
            return Barn::create();
        },
        [this]() -> bool {
            Vec2 playerPos = player1->getPosition();
            return barn_area.containsPoint(playerPos);
        },
        nullptr,
        "farm_to_barn"
    );

    auto enterCaveCommand = std::make_shared<ConditionalSceneTransitionCommand>(
        player1,
        [this]() -> cocos2d::Scene* {
            // 进入矿洞前，记录玩家上一次在农场的位置
            for (auto& pair : F_lastplace) {
                if (pair.first.first == "cave") {
                    pair.second = true;
                }
            }
            player1->removeFromParent();
            return Cave::create();
        },
        [this]() -> bool {
            Vec2 playerPos = player1->getPosition();
            return cave_area.containsPoint(playerPos);
        },
        nullptr,
        "farm_to_cave"
    );

    auto enterForestCommand = std::make_shared<ConditionalSceneTransitionCommand>(
        player1,
        [this]() -> cocos2d::Scene* {
            // 进入森林前，记录玩家上一次在农场的位置
            for (auto& pair : F_lastplace) {
                if (pair.first.first == "forest") {
                    pair.second = true;
                }
            }
            player1->removeFromParent();
            return Forest::create();
        },
        [this]() -> bool {
            Vec2 playerPos = player1->getPosition();
            return forest_area.containsPoint(playerPos);
        },
        nullptr,
        "farm_to_forest"
    );

    // 将命令与按键进行绑定（Command Pattern 的核心用法）
    inputManager->bindPressCommand(EventKeyboard::KeyCode::KEY_P, plantCommand);
    inputManager->bindPressCommand(EventKeyboard::KeyCode::KEY_W, waterCommand);
    inputManager->bindPressCommand(EventKeyboard::KeyCode::KEY_G, harvestCommand);
    inputManager->bindPressCommand(EventKeyboard::KeyCode::KEY_ESCAPE, toggleInventoryCommand);
    
    // ENTER / 小键盘 ENTER 被绑定为多个场景切换命令（由条件判断实际生效的命令）
    inputManager->bindPressCommand(EventKeyboard::KeyCode::KEY_ENTER, exitFarmCommand);
    inputManager->bindPressCommand(EventKeyboard::KeyCode::KEY_KP_ENTER, exitFarmCommand);
    inputManager->bindPressCommand(EventKeyboard::KeyCode::KEY_ENTER, enterHouseCommand);
    inputManager->bindPressCommand(EventKeyboard::KeyCode::KEY_KP_ENTER, enterHouseCommand);
    inputManager->bindPressCommand(EventKeyboard::KeyCode::KEY_ENTER, enterBarnCommand);
    inputManager->bindPressCommand(EventKeyboard::KeyCode::KEY_KP_ENTER, enterBarnCommand);
    inputManager->bindPressCommand(EventKeyboard::KeyCode::KEY_ENTER, enterCaveCommand);
    inputManager->bindPressCommand(EventKeyboard::KeyCode::KEY_KP_ENTER, enterCaveCommand);
    inputManager->bindPressCommand(EventKeyboard::KeyCode::KEY_ENTER, enterForestCommand);
    inputManager->bindPressCommand(EventKeyboard::KeyCode::KEY_KP_ENTER, enterForestCommand);

    // 记录已绑定的命令，便于之后统一解绑
    boundCommands.push_back(plantCommand);
    boundCommands.push_back(waterCommand);
    boundCommands.push_back(harvestCommand);
    boundCommands.push_back(toggleInventoryCommand);
    boundCommands.push_back(exitFarmCommand);
    boundCommands.push_back(enterHouseCommand);
    boundCommands.push_back(enterBarnCommand);
    boundCommands.push_back(enterCaveCommand);
    boundCommands.push_back(enterForestCommand);
}

void farm::cleanupInputCommands() {
    auto inputManager = InputManager::getInstance();
    
    // 解绑所有已绑定的按键命令
    for (auto& command : boundCommands) {
        inputManager->unbindCommand(EventKeyboard::KeyCode::KEY_ESCAPE, command);
        inputManager->unbindCommand(EventKeyboard::KeyCode::KEY_P, command);
        inputManager->unbindCommand(EventKeyboard::KeyCode::KEY_W, command);
        inputManager->unbindCommand(EventKeyboard::KeyCode::KEY_G, command);
        inputManager->unbindCommand(EventKeyboard::KeyCode::KEY_ENTER, command);
        inputManager->unbindCommand(EventKeyboard::KeyCode::KEY_KP_ENTER, command);
    }
    
    // 清空命令列表
    boundCommands.clear();
}

void farm::performPlantAction() {
    Vec2 playerPos = player1->getPosition();
    
    // 只有当玩家处于可种植区域内时才允许种植
    if (!plant_area.containsPoint(playerPos)) {
        return;
    }
    
    int nums = getRegionNumber(Vec2(playerPos.x + 10, playerPos.y - 10));
    bool Isplant = false;
    
    CCLOG("plant nums = %d", nums);
    
    // 若当前位置已有作物，则不再重复种植
    for (auto it = Crop_information.begin(); it != Crop_information.end(); ++it) {
        if ((*it)->nums == nums) {
            Isplant = true;
            break;
        }
    }
    
    if (Isplant == false) {
        auto temp = miniBag->getSelectedItem();
        if (temp != nullptr) {
            CCLOG("copy item");
            std::string TypeName = temp->GetName();
            auto point = cropbasicinformation.find(TypeName);
            if (point != cropbasicinformation.end()) {
                CCLOG("find crop");
                // 满足季节与体力条件时才允许种植
                if (((cropbasicinformation[TypeName].GetSeason() == Season) || (cropbasicinformation[TypeName].GetSeason() == "All")) && strength >= 10) {
                    inventory->RemoveItem(*temp);

                    if (nums == 13 && RainBowfirst) {
                        auto rainbowItem = GetRainBow();
                        if (rainbowItem != nullptr) {
                            inventory->AddItem(*rainbowItem);
                        }
                        RainBowfirst = false;
                    }

                    strength -= 10;
                    TimeUI->UpdateEnergy();

                    Crop_information.push_back(cropbasicinformation[TypeName].GetCropCopy());
                    Crop_information.back()->plant_day = season[Season] * 7 + day;
                    Crop_information.back()->nums = nums;

                    if (player1->pic_path == "character1/player_right3.png") {
                        // 播放向右种地的动作序列
                        player1->setTexture("character1/player_plant3.png");
                        player1->setScale(2.5f);

                        // 0.15 秒后切换到种地动作的第二帧
                        player1->scheduleOnce([=](float dt) {
                            player1->setTexture("character1/player_plant4.png");  // 对应种地动作的第二帧
                            player1->setScale(2.7f);
                        }, 0.15f, "change_image1_key");

                        // 再过 0.2 秒后恢复站立贴图，并生成作物精灵
                        player1->scheduleOnce([=](float dt) {
                            player1->setTexture("character1/player_right3.png"); // 恢复为面向右侧的站立贴图
                            player1->setScale(1.5f);
                            
                            // 使用对象池创建作物精灵
                            auto cropPool = CropSpritePool::getInstance();
                            auto sprite = cropPool->getCropSprite(nums, Crop_information.back()->initial_pic);
                            if (sprite) {
                                this->addChild(sprite, 15 - nums / 19);
                                sprite->setPosition(500 + ((nums % 19) - 1) * 48, 910 - ((nums / 19) - 1) * 48);
                                sprite->setScale(2.1f);
                            }
                        }, 0.35f, "change_image2_key");
                    }
                    else {
                        // 播放向左种地的动作序列
                        player1->setTexture("character1/player_plant1.png");
                        player1->setScale(2.5f);

                        // 0.15 秒后切换到种地动作的第二帧
                        player1->scheduleOnce([=](float dt) {
                            player1->setTexture("character1/player_plant2.png");  // 对应向左种地动作的第二帧
                            player1->setScale(2.7f);
                        }, 0.15f, "change_image1_key");

                        // 再过 0.2 秒后恢复站立贴图，并生成作物精灵
                        player1->scheduleOnce([=](float dt) {
                            player1->setTexture("character1/player_left3.png"); // 恢复为面向左侧的站立贴图
                            player1->setScale(1.5f);
                            
                            // 使用对象池创建作物精灵
                            auto cropPool = CropSpritePool::getInstance();
                            auto sprite = cropPool->getCropSprite(nums, Crop_information.back()->initial_pic);
                            if (sprite) {
                                this->addChild(sprite, 15 - nums / 19);
                                sprite->setPosition(500 + ((nums % 19) - 1) * 48, 910 - ((nums / 19) - 1) * 48);
                                sprite->setScale(2.1f);
                            }
                        }, 0.35f, "change_image2_key");
                    }
                }
            }
        }
    }
}

void farm::performWaterAction() {
    Vec2 playerPos = player1->getPosition();
    
    // 只有当玩家位于可浇水区域内时才允许浇水
    if (!plant_area.containsPoint(playerPos)) {
        return;
    }
    
    int nums = getRegionNumber(Vec2(playerPos.x + 30, playerPos.y - 10));

    for (auto it : Crop_information) {
        if (it->nums == nums) {
            // 标记当前作物已被浇水
            it->watered = true;

            if (player1->pic_path == "character1/player_left3.png") {
                // 向左浇水动作序列
                player1->setTexture("character1/player_water4.png");
                player1->setScale(1.7f);

                // 0.15 秒后切换到浇水动作的第二帧
                player1->scheduleOnce([=](float dt) {
                    player1->setTexture("character1/player_water3.png");  // 向左浇水动作的第二帧
                    player1->setScale(1.7f);
                }, 0.15f, "change_image1_key");

                // 再过 0.2 秒后恢复为站立贴图，并恢复移动权限
                player1->scheduleOnce([=](float dt) {
                    player1->setTexture("character1/player_left3.png"); // 恢复为面向左侧的站立贴图
                    player1->setScale(1.5f);
                    // 恢复四个方向的移动权限
                    player1->moveDown = true;
                    player1->moveLeft = true;
                    player1->moveUp = true;
                    player1->moveRight = true;
                }, 0.35f, "change_image2_key");
            }
            else {
                // 右侧浇水动作序列
                player1->setTexture("character1/player_water2.png");
                player1->setScale(1.7f);

                // 0.15 秒后切换到浇水动作的第二帧
                player1->scheduleOnce([=](float dt) {
                    player1->setTexture("character1/player_water1.png");  // 向右浇水动作的第二帧
                    player1->setScale(1.7f);
                }, 0.15f, "change_image1_key");

                // 再过 0.2 秒后恢复为站立贴图，并恢复移动权限
                player1->scheduleOnce([=](float dt) {
                    player1->setTexture("character1/player_right3.png"); // 恢复为面向右侧的站立贴图
                    player1->setScale(1.5f);
                    // 恢复四个方向的移动权限
                    player1->moveDown = true;
                    player1->moveLeft = true;
                    player1->moveUp = true;
                    player1->moveRight = true;
                }, 0.35f, "change_image2_key");
            }
            break; // 找到对应作物后立即退出循环
        }
    }
}

void farm::performHarvestAction() {
    Vec2 playerPos = player1->getPosition();
    
    // 只有当玩家位于可收获区域内时才允许收获
    if (!plant_area.containsPoint(playerPos)) {
        return;
    }
    
    int nums = getRegionNumber(Vec2(playerPos.x + 10, playerPos.y - 10));

    for (auto it = Crop_information.begin(); it != Crop_information.end(); /* no increment here */) {
        if ((*it)->nums == nums) {
            if ((*it)->GetPhase() == Phase::MATURE && strength >= 10) {

                skill_tree->AddExperience(farming_skill, 10);

                auto find_temp = (*it);

                if (find_temp->GetName() == "potato") {
                    inventory->AddItem(potato);
                }

                strength -= 10;
                TimeUI->UpdateEnergy();

                // 使用对象池移除作物精灵并替换为土地瓦片
                auto cropPool = CropSpritePool::getInstance();
                cropPool->removeCropSprite(nums);
                
                // 创建土地瓦片精灵
                auto tileSprite = cropPool->getCropSprite(nums, "farm/tile.png");
                if (tileSprite) {
                    this->addChild(tileSprite, 15 - nums / 19);
                    tileSprite->setPosition(495 + ((nums % 19) - 1) * 48, 910 - ((nums / 19) - 1) * 48);
                    tileSprite->setScaleX(2.5f);
                    tileSprite->setScaleY(1.9f);
                }

                // 删除作物数据对象
                it = Crop_information.erase(it);  // erase 返回下一个迭代器 

                // 收获动作：播放收割动画
                player1->setTexture("character1/player_plant1.png");
                player1->setScale(2.5f);

                // 0.15 秒后切换到收割动作的第二帧
                player1->scheduleOnce([=](float dt) {
                    player1->setTexture("character1/player_plant2.png");  // 收割动作的第二帧
                    player1->setScale(2.7f);
                }, 0.15f, "change_image1_key");

                // 再过 0.2 秒后恢复为站立贴图
                player1->scheduleOnce([=](float dt) {
                    player1->setTexture("character1/player_left3.png"); // 恢复为面向左侧的站立贴图
                    player1->setScale(1.5f);
                }, 0.35f, "change_image2_key");
                
                return; // 收获成功后直接返回
            }
            else {
                return; // 不满足收获条件（未成熟或体力不足）时直接返回
            }
        }
        else {
            ++it;  // 未命中当前作物，继续遍历下一个
        }
    }
}







