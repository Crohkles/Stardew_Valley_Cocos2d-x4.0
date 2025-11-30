#include "AppDelegate.h"
#include "farm.h"
#include "Forest.h"
#include "Player.h"
#include "physics/CCPhysicsWorld.h"
#include "ui/CocosGUI.h"
#include "vector"
#include "InputManager.h"
#include "SceneInteractionCommand.h"
#include "UICommand.h"

USING_NS_CC;

Forest::Forest() {}

Forest::~Forest() {
    cleanupInputCommands();
}

bool Forest::init()
{

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    button = cocos2d::Sprite::create("CloseNormal.png");
    this->addChild(button, 11);

    // 设置计时器标签
    TimeUI = Timesystem::create ( "Forest" );
    this->addChild(TimeUI, 13);

    // 恢复种植
    AllInitialize_ore();

    if (Weather == "Rainy") {
        // 下雨
        createRainEffect();
    }

    // 设置背景图片
    auto background_real = Sprite::create("forest/forest.png");
    background_real->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(background_real, 1);
    background_real->setScale(2.3f);

    auto background = Sprite::create("forest/forest_path.png");
    this->addChild(background, 5);
    background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    background->setScale(2.3f);

    auto background_up = Sprite::create("forest/forest_up.png");
    background_up->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(background_up, 7);
    background_up->setScale(2.3f);


    Vec2 spritePosition = background->getPosition();   // 获取精灵的位置（中心点）
    Size spriteSize = background->getContentSize();    // 获取精灵的尺寸（宽度和高度）
  

    // 计算左下角的坐标
    Vec2 leftBottomPosition = Vec2(
        spritePosition.x - background->getScaleX() * spriteSize.width / 2,   // 中心点 x 坐标减去宽度的一半
        spritePosition.y - background->getScaleY() * spriteSize.height / 2   // 中心点 y 坐标减去高度的一半
    );
   

    Image img;
    if (img.initWithImageFile("forest/forest_path.png"))
    {
        int width = img.getWidth();
        int height = img.getHeight();

        // 获取像素数据
        unsigned char* data = img.getData();

        // 遍历所有像素，检查是否有内容（透明度大于0）
        for (int y = 0; y < height; y = y + 2)
        {
            for (int x = 0; x < width; x = x + 2)
            {
                // 获取当前像素的 RGBA 值
                int index = (y * width + x) * 4;    // 每个像素占用 4 个字节 (RGBA)
                unsigned char a = data[index + 3];  // 透明度

                // 如果透明度 (alpha) 大于 0，表示此像素有内容
                if (a > 0)
                {
                    float screenX = leftBottomPosition.x + x * background->getScaleX();
                    float screenY = leftBottomPosition.y + (height - y - 1) * background->getScaleY();  // 注意 Y 轴反向
                    nonTransparentPixels.push_back(Vec2(screenX, screenY));                             // 记录屏幕坐标
                }
            }
        }
    }

    // 初始化角色并将其添加到场景
    if (player1->getParent() == NULL) {
        this->addChild(player1, 6);
        // 在添加到场景后设置输入绑定
        player1->setupInputBindings();
        // 设置碰撞上下文
        player1->setCollisionContext(nonTransparentPixels);
        for (auto& pair : W_lastplace) {
            if (pair.second == true) { 
                player1->setPosition(pair.first.second);
                pair.second = false;
            }
        }
        player1->speed = 6.1f;
        //仅为方便测试
       /* player1->speed = 15.0f;*/
        player1->setScale(2.3f);
        player1->setAnchorPoint(Vec2(0.5f, 0.2f));
    }    

    
    // 启动人物的定时器
    player1->schedule([=](float dt) {
        player1->player1_move();
        }, 0.05f, "player1_move");

    player1->schedule([=](float dt) {
        player1->player_change();
        }, 0.3f, "player_change");


    // 计算背景精灵的缩放后范围
    float scaledWidth = background->getContentSize().width * background->getScaleX();
    float scaledHeight = background->getContentSize().height * background->getScaleY();

    // 构造 Follow 的边界 Rect
    auto followRect = cocos2d::Rect(leftBottomPosition.x, leftBottomPosition.y, scaledWidth, scaledHeight);

    // 创建 Follow 动作并限制玩家在背景范围内移动
    auto followAction = Follow::create(player1, followRect);
    this->runAction(followAction);

    // 定期更新玩家状态
    this->schedule([this](float dt) {
        this->checkPlayerPosition();  // 检查玩家是否接近轮廓点
        }, 0.01f, "check_position_key");

    auto listener = EventListenerMouse::create();
    listener->onMouseDown = [this](Event* event) {
       
        // 获取鼠标点击的位置
        auto mouseEvent = static_cast<EventMouse*>(event);
        Vec2 clickPos(mouseEvent->getCursorX(), mouseEvent->getCursorY());
        clickPos = this->convertToNodeSpace(clickPos);

        // 判断点击位置是否在精灵范围内
        if (button != nullptr && button->getBoundingBox().containsPoint(clickPos)) {
            Director::getInstance()->end();
        }
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, button);

    // 旧的键盘监听器已替换为Command Pattern

    // 设置Command Pattern输入绑定
    setupInputCommands();

    //界面下的背包显示
    miniBag = mini_bag::create ( inventory );
    miniBag->setScale ( 1.0f );
    Vec2 pos = miniBag->getPosition ();
    if (miniBag != NULL) {
        cocos2d::log ( "miniBagtest %f" , pos.x );
    }
    if (!this->getChildByName ( "mini_bag" )) {
        this->addChild ( miniBag , 10 , "mini_bag" );
    }


    // 更新物品栏
    schedule ( [=]( float deltaTime ) {
        if (inventory->is_updated == true) {
            miniBag->updateDisplay ();
            inventory->is_updated = false;
        }
        } , 0.1f , "item_update_key" );

    return true;
}


Forest* Forest::create()
{
    Forest* scene = new Forest();
    if (scene && scene->init())
    {
        scene->autorelease();
        return scene;
    }
    CC_SAFE_DELETE(scene);
    return nullptr;
}

void  Forest::AllInitialize_ore() {

    for (auto it = Tree_information.begin(); it != Tree_information.end(); /* no increment here */) {

        auto tree = *it;  // 解引用迭代器以访问 Crop 对象

      
        tree->recover_time = 2;

        if (!tree->available) {
            if (season[Season] * 7 + day - tree->mining_day >= tree->recover_time) {
                tree->available = true;
                tree->removetimes = 2;
            }
        }
        

        if (tree->available) {

            if (Season == "Autumn" || Season == "Winter") {
                auto temp = Sprite::create(tree->Y_Ini_pic);
                this->addChild(temp, 5);
                temp->setPosition(tree->position);
                temp->setScale(3.1f);
            }
            else {
                auto temp = Sprite::create(tree->initial_pic);
                this->addChild(temp, 5);
                temp->setPosition(tree->position);
                temp->setScale(3.1f);
            }
        }
        else {
            auto temp = Sprite::create(tree->G_Cut_pic);
            this->addChild(temp, 5);
            temp->setPosition(tree->position);
            temp->setScale(3.1f);
        }


        it++;


    }

}

// 检查玩家是否接近背景的轮廓点
void  Forest::checkPlayerPosition()
{

    // 获取玩家的位置
    Vec2 playerPos = player1->getPosition();
    
    // 更新计时器显示
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


        for (auto it = Crop_information.begin(); it != Crop_information.end();) {

            auto crop = *it;  // 解引用迭代器以访问 Crop 对象

            if (day == 1) {
                crop->watered = true;
            }
            if ((day - 1) % 3 == 1) {
                crop->watered = true;
            }

            // 判断前一天是否浇水
            if ((crop->watered == false) && (crop->GetPhase() != Phase::MATURE)) {
                // 判断是否已经进入枯萎状态
                if (crop->GetPhase() != Phase::SAPLESS) {
                    crop->ChangePhase(Phase::SAPLESS);
                    crop->ChangMatureNeeded(2); // 延迟两天收获
                    it++;
                }
                else {
                    // 删除元素并更新迭代器
                    it = Crop_information.erase(it);
                }

            }
            else {
                // 更新状态
                crop->UpdateGrowth();
                it++;
            }

        }

        for (auto& pair : F_lastplace) {
            if (pair.second) {
                pair.second = false;
            }
            if (pair.first.first == "myhouse") {  // 检查 bool 值是否为 true
                pair.second = true;
            }
        }

        for (auto& pair : W_lastplace) {
            if (pair.second) {
                pair.second = false;
            }
            if (pair.first.first == "farm") {
                pair.second = true;
            }
        }

        //恢复为能够生产产品
        for (auto livestock : livestocks) {
            livestock->SetCanProduce ( true );
        }

        IsSleep = false;
        frombed = true;
        remainingTime = 10800;
        player1->removeFromParent();
        auto nextday = Myhouse::create();
        Director::getInstance()->replaceScene(nextday);

    }

    // 更新标签位置
    float currentx = 0, currenty = 0;
    if (playerPos.x <= -600) {
        currentx = -600;
    }
    else if (playerPos.x >= 2197) {
        currentx = 2197;
    }
    else {
        currentx = playerPos.x;  
    }

    if (playerPos.y >= 2200) {
        currenty = 2200;
    }
    else if (playerPos.y <= -850) {
        currenty = -850;
    }
    else {
        currenty = playerPos.y;
    }

    TimeUI->setPosition(currentx, currenty);
    button->setPosition(currentx + 730, currenty - 590);
    miniBag->setPosition ( currentx , currenty );
    if (Weather == "Rainy") {
        emitter->setPositionY ( currenty + 350 );
    }
    // 伐木逻辑已移到LoggingCommand中处理

    // 场景切换逻辑已移到Command Pattern中处理

    // 碰撞检测逻辑已移到Player类的updateMovementPermissions方法中
    // 通过setCollisionContext设置碰撞点即可
    // 碰撞权限会在Player的player1_move()中自动更新
}

int Forest::getRegionNumber(Vec2 pos) {

    int region_number = 1;

    return region_number;
}

void Forest::createRainEffect() {

    emitter = ParticleRain::create();
    emitter->setDuration(ParticleSystem::DURATION_INFINITY);
    emitter->setScale(5.7f);
    emitter->setTotalParticles(100);
    emitter->setSpeed(250);

    addChild(emitter, 10);

    // 每帧更新粒子生命周期
    schedule([this](float dt) {
        updaterain(dt);
        }, "update_rain_key");

}

void Forest::updaterain(float deltaTime) {
    if (emitter) {
        // 随机生成一个生命周期（范围 1 到 1.5 秒之间）
        float newLife = cocos2d::rand_0_1() * 1.5f;

        // 设置新的生命周期
        emitter->setLife(newLife);

        emitter->setEmissionRate(emitter->getTotalParticles() / emitter->getLife() * 1.3);
    }
}

void Forest::setupInputCommands()
{
    auto inputManager = InputManager::getInstance();
    // inventory是全局变量，在AppDelegate.h中声明
    
    // 创建条件场景切换命令 - 前往农场
    auto farmCommand = std::make_shared<ConditionalSceneTransitionCommand>(
        player1,
        []() { return farm::create(); },
        [this]() { 
            Vec2 playerPos = player1->getPosition();
            return farm_area.containsPoint(playerPos); 
        },
        nullptr,
        "farm"
    );
    
    // 创建条件场景切换命令 - 前往小镇
    auto townCommand = std::make_shared<ConditionalSceneTransitionCommand>(
        player1,
        []() { return Town::create(); },
        [this]() { 
            Vec2 playerPos = player1->getPosition();
            return town_area.containsPoint(playerPos); 
        },
        nullptr,
        "town"
    );
    
    // 创建伐木动作命令
    auto loggingCommand = std::make_shared<LoggingCommand>(
        this
    );
    
    // 创建UI切换命令 - 背包
    auto inventoryCommand = std::make_shared<ToggleInventoryCommand>(
        this,
        inventory,
        "Forest"
    );
    
    // 绑定命令到按键
    inputManager->bindPressCommand(EventKeyboard::KeyCode::KEY_ENTER, farmCommand);
    inputManager->bindPressCommand(EventKeyboard::KeyCode::KEY_ENTER, townCommand);
    
    inputManager->bindPressCommand(EventKeyboard::KeyCode::KEY_KP_ENTER, farmCommand);
    inputManager->bindPressCommand(EventKeyboard::KeyCode::KEY_KP_ENTER, townCommand);
    
    inputManager->bindPressCommand(EventKeyboard::KeyCode::KEY_L, loggingCommand);
    inputManager->bindPressCommand(EventKeyboard::KeyCode::KEY_ESCAPE, inventoryCommand);
    
    // 保存绑定的命令，方便清理
    boundCommands.push_back(farmCommand);
    boundCommands.push_back(townCommand);
    boundCommands.push_back(loggingCommand);
    boundCommands.push_back(inventoryCommand);
}

void Forest::cleanupInputCommands()
{
    auto inputManager = InputManager::getInstance();
    
    // 清理绑定的命令 - 解绑各个按键的命令
    for (auto& command : boundCommands) {
        if (auto toggleCmd = std::dynamic_pointer_cast<ToggleInventoryCommand>(command)) {
            inputManager->unbindCommand(EventKeyboard::KeyCode::KEY_ESCAPE, command);
        } else if (auto loggingCmd = std::dynamic_pointer_cast<LoggingCommand>(command)) {
            inputManager->unbindCommand(EventKeyboard::KeyCode::KEY_L, command);
        } else {
            // 场景切换命令绑定在ENTER键上
            inputManager->unbindCommand(EventKeyboard::KeyCode::KEY_ENTER, command);
            inputManager->unbindCommand(EventKeyboard::KeyCode::KEY_KP_ENTER, command);
        }
    }
    
    // 清空命令列表
    boundCommands.clear();
}
