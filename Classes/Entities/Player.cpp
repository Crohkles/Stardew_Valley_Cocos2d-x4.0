#include "Player.h"
#include "InputManager.h"
#include "PlayerMovementCommand.h"

USING_NS_CC;  // 使用 Cocos2d-x 命名空间

// Player 类的构造函数
Player::Player() : speed(10.0f),pic_path("character1 / player_down3.png") {}

// Player 类的析构函数
Player::~Player() {}

// Player 类的初始化函数
bool Player::init()
{
    if (!Sprite::init())  // 初始化父类 Sprite，检查是否成功
    {
        return false;  // 如果初始化失败，返回 false
    }

    // 加载角色的图片（玩家朝下的站立图片）
    this->initWithFile("character1/player_down3.png");

    // 注意：setupInputBindings() 需要在Player被addChild到场景后调用
    // 因为Command的canExecute()需要检查player->getParent() != nullptr

    // 每 0.05 秒调用一次 player1_move() 函数，控制玩家移动
    this->schedule([this](float dt) {
        this->player1_move();
        }, 0.05f, "player1_move");

    // 每 0.3 秒调用一次 player_change() 函数，控制玩家动画切换
    this->schedule([this](float dt) {
        this->player_change();
        }, 0.3f, "player_change");

    return true;  // 初始化成功，返回 true
}

// Player 类的静态创建方法
Player* Player::create()
{
    Player* player = new Player();  // 创建 Player 对象
    if (player && player->init())  // 如果创建并初始化成功
    {
        player->autorelease();  // 自动释放内存
        return player;  // 返回新创建的 Player 对象
    }
    CC_SAFE_DELETE(player);  // 如果创建失败，删除 player 对象
    return nullptr;  // 返回空指针
}

// 按键按下时的回调函数 (已废弃，现在使用Command系统)
// 此方法保留用于兼容性，但不再使用
void Player::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    // 不再处理键盘输入，所有输入由InputManager和Command系统处理
    // 此方法已废弃，但保留接口以确保兼容性
}

// 按键释放时的回调函数 (已废弃，现在使用Command系统)
// 此方法保留用于兼容性，但不再使用
void Player::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    // 不再处理键盘输入，所有输入由InputManager和Command系统处理
    // 按键释放的逻辑现在在各个StopMoveCommand中处理
    // 此方法已废弃，但保留接口以确保兼容性
}

// 玩家移动的逻辑
void Player::player1_move() {
    // 在移动前更新碰撞权限
    if (!collisionPoints.empty()) {
        updateMovementPermissions();
    }

    // 如果按下左箭头并且允许向左移动
    if (this->leftpressed && this->moveLeft) {
        if (this->look_state == 0) {
            this->look_state++;  // 如果玩家静止状态，更新状态
            return;
        }
        this->setPositionX(this->getPositionX() - speed);  // 向左移动
    }
    // 如果按下下箭头并且允许向下移动
    else if (this->downpressed && this->moveDown) {
        if (this->look_state == 0) {
            this->look_state++;  // 如果玩家静止状态，更新状态
            return;
        }
        this->setPositionY(this->getPositionY() - speed);  // 向下移动
    }
    // 如果按下上箭头并且允许向上移动
    else if (this->uppressed && this->moveUp) {
        if (this->look_state == 0) {
            this->look_state++;  // 如果玩家静止状态，更新状态
            return;
        }
        this->setPositionY(this->getPositionY() + speed);  // 向上移动
    }
    // 如果按下右箭头并且允许向右移动
    else if (this->rightpressed && this->moveRight) {
        if (this->look_state == 0) {
            this->look_state++;  // 如果玩家静止状态，更新状态
            return;
        }
        this->setPositionX(this->getPositionX() + speed);  // 向右移动
    }
}

// 玩家状态变化的逻辑（切换动画）
void Player::player_change() {

    // 如果按下左箭头并且允许向左移动
    if (this->leftpressed && this->moveLeft) {
        if (this->look_state % 2 == 1) {  // 如果是奇数帧，切换为第一个动画
            this->look_state++;
            this->setTexture("character1/player_left1.png");
        }
        else {  // 如果是偶数帧，切换为第二个动画
            this->look_state++;
            this->setTexture("character1/player_left2.png");
        }
    }
    // 如果按下下箭头并且允许向下移动
    else if (this->downpressed && this->moveDown) {
        if (this->look_state % 2 == 1) {  // 如果是奇数帧，切换为第一个动画
            this->look_state++;
            this->setTexture("character1/player_down1.png");
        }
        else {  // 如果是偶数帧，切换为第二个动画
            this->look_state++;
            this->setTexture("character1/player_down2.png");
        }
    }
    // 如果按下上箭头并且允许向上移动
    else if (this->uppressed && this->moveUp) {
        if (this->look_state % 2 == 1) {  // 如果是奇数帧，切换为第一个动画
            this->look_state++;
            this->setTexture("character1/player_up1.png");
        }
        else {  // 如果是偶数帧，切换为第二个动画
            this->look_state++;
            this->setTexture("character1/player_up2.png");
        }
    }
    // 如果按下右箭头并且允许向右移动
    else if (this->rightpressed && this->moveRight) {
        if (this->look_state % 2 == 1) {  // 如果是奇数帧，切换为第一个动画
            this->look_state++;
            this->setTexture("character1/player_right1.png");
        }
        else {  // 如果是偶数帧，切换为第二个动画
            this->look_state++;
            this->setTexture("character1/player_right2.png");
        }
    }
}

// 新增方法：设置Player的输入绑定
void Player::setupInputBindings() {
    auto inputManager = InputManager::getInstance();
    
    // 创建移动命令
    auto moveUpCommand = std::make_shared<MoveUpCommand>(this);
    auto moveDownCommand = std::make_shared<MoveDownCommand>(this);
    auto moveLeftCommand = std::make_shared<MoveLeftCommand>(this);
    auto moveRightCommand = std::make_shared<MoveRightCommand>(this);
    
    auto stopMoveUpCommand = std::make_shared<StopMoveUpCommand>(this);
    auto stopMoveDownCommand = std::make_shared<StopMoveDownCommand>(this);
    auto stopMoveLeftCommand = std::make_shared<StopMoveLeftCommand>(this);
    auto stopMoveRightCommand = std::make_shared<StopMoveRightCommand>(this);
    
    // 绑定按键按下事件
    inputManager->bindPressCommand(EventKeyboard::KeyCode::KEY_UP_ARROW, moveUpCommand);
    inputManager->bindPressCommand(EventKeyboard::KeyCode::KEY_DOWN_ARROW, moveDownCommand);
    inputManager->bindPressCommand(EventKeyboard::KeyCode::KEY_LEFT_ARROW, moveLeftCommand);
    inputManager->bindPressCommand(EventKeyboard::KeyCode::KEY_RIGHT_ARROW, moveRightCommand);
    
    // 绑定按键释放事件
    inputManager->bindReleaseCommand(EventKeyboard::KeyCode::KEY_UP_ARROW, stopMoveUpCommand);
    inputManager->bindReleaseCommand(EventKeyboard::KeyCode::KEY_DOWN_ARROW, stopMoveDownCommand);
    inputManager->bindReleaseCommand(EventKeyboard::KeyCode::KEY_LEFT_ARROW, stopMoveLeftCommand);
    inputManager->bindReleaseCommand(EventKeyboard::KeyCode::KEY_RIGHT_ARROW, stopMoveRightCommand);
    
    CCLOG("Player input bindings set up successfully");
}

// 新增：Command系统用的移动状态控制方法
void Player::startMove(int direction) {
    switch(direction) {
        case 0: // down
            if (!downpressed) {
                downpressed = true;
                CCLOG("Player started moving down");
            }
            break;
        case 1: // left  
            if (!leftpressed) {
                leftpressed = true;
                CCLOG("Player started moving left");
            }
            break;
        case 2: // right
            if (!rightpressed) {
                rightpressed = true;
                CCLOG("Player started moving right");
            }
            break;
        case 3: // up
            if (!uppressed) {
                uppressed = true;
                CCLOG("Player started moving up");
            }
            break;
    }
}

void Player::stopMove(int direction) {
    switch(direction) {
        case 0: // down
            if (downpressed) {
                downpressed = false;
                this->look_state = 0;
                this->setTexture("character1/player_down3.png");
                this->pic_path = "character1/player_down3.png";
                CCLOG("Player stopped moving down");
            }
            break;
        case 1: // left
            if (leftpressed) {
                leftpressed = false;
                this->look_state = 0;
                this->setTexture("character1/player_left3.png");
                this->pic_path = "character1/player_left3.png";
                CCLOG("Player stopped moving left");
            }
            break;
        case 2: // right
            if (rightpressed) {
                rightpressed = false;
                this->look_state = 0;
                this->setTexture("character1/player_right3.png");
                this->pic_path = "character1/player_right3.png";
                CCLOG("Player stopped moving right");
            }
            break;
        case 3: // up
            if (uppressed) {
                uppressed = false;
                this->look_state = 0;
                this->setTexture("character1/player_up3.png");
                this->pic_path = "character1/player_up3.png";
                CCLOG("Player stopped moving up");
            }
            break;
    }
}

bool Player::canMove(int direction) const {
    switch(direction) {
        case 0: return moveDown;   // down
        case 1: return moveLeft;   // left
        case 2: return moveRight;  // right
        case 3: return moveUp;     // up
        default: return false;
    }
}

// 碰撞上下文管理方法实现
void Player::setCollisionContext(const std::vector<Vec2>& collisionPoints) {
    this->collisionPoints = collisionPoints;
    updateMovementPermissions();
    CCLOG("Player collision context updated with %d collision points", (int)collisionPoints.size());
}

void Player::updateMovementPermissions() {
    if (collisionPoints.empty()) {
        // 如果没有碰撞点，允许所有方向移动
        moveLeft = moveRight = moveUp = moveDown = true;
        return;
    }

    Vec2 playerPos = this->getPosition();
    
    // 重置移动权限
    moveLeft = moveRight = moveUp = moveDown = true;
    
    // 检查每个方向的碰撞
    for (const auto& point : collisionPoints) {
        float distance;
        Vec2 tempPos;
        
        // 检查左移
        tempPos = playerPos;
        tempPos.x -= speed;
        distance = tempPos.distance(point);
        if (distance <= collisionRadius) {
            moveLeft = false;
        }
        
        // 检查右移
        tempPos = playerPos;
        tempPos.x += speed;
        distance = tempPos.distance(point);
        if (distance <= collisionRadius) {
            moveRight = false;
        }
        
        // 检查上移
        tempPos = playerPos;
        tempPos.y += speed;
        distance = tempPos.distance(point);
        if (distance <= collisionRadius) {
            moveUp = false;
        }
        
        // 检查下移
        tempPos = playerPos;
        tempPos.y -= speed;
        distance = tempPos.distance(point);
        if (distance <= collisionRadius) {
            moveDown = false;
        }
    }
}
