#include "InputTestScene.h"
#include "../Core/InputManager.h"
#include "../Commands/PlayerMovementCommand.h"
#include "../Commands/UICommand.h"
#include "../Entities/Player.h"
#include "../Core/AppDelegate.h"

USING_NS_CC;

// 静态成员初始化
int InputTestScene::commandExecuteCount = 0;

Scene* InputTestScene::createScene()
{
    return InputTestScene::create();
}

bool InputTestScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 创建背景
    auto background = DrawNode::create();
    background->drawSolidRect(Vec2::ZERO, visibleSize, Color4F::GRAY);
    this->addChild(background, -1);

    // 创建标题标签
    auto titleLabel = Label::createWithTTF("Input System Test Scene", "fonts/Marker Felt.ttf", 24);
    if (titleLabel != nullptr)
    {
        titleLabel->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height - titleLabel->getContentSize().height));
        this->addChild(titleLabel, 1);
    }

    // 创建状态显示标签
    statusLabel = Label::createWithTTF("Status: System initialized", "fonts/Marker Felt.ttf", 18);
    if (statusLabel != nullptr)
    {
        statusLabel->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height - 100));
        this->addChild(statusLabel, 1);
    }

    // 创建指令说明标签
    auto instructionLabel = Label::createWithTTF(
        "Controls:\n"
        "Arrow Keys - Move Player\n"
        "ESC - Toggle Inventory\n"
        "E - Consume Food (if selected)\n"
        "Commands executed: 0", 
        "fonts/Marker Felt.ttf", 14);
    if (instructionLabel != nullptr)
    {
        instructionLabel->setPosition(Vec2(origin.x + 150, origin.y + visibleSize.height - 200));
        this->addChild(instructionLabel, 1);
    }

    // 创建测试用的Player
    testPlayer = Player::create();
    if (testPlayer != nullptr)
    {
        testPlayer->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
        testPlayer->setScale(2.0f);  // 放大以便观察
        this->addChild(testPlayer, 2);
        
        // 重要：必须在addChild之后调用setupInputBindings，确保Player有parent
        testPlayer->setupInputBindings();
        
        // 将testPlayer设置为全局player1，这样其他系统可以访问
        player1 = testPlayer;
        
        CCLOG("Test player created and input bindings set up");
    }

    // 创建测试用的背包
    testInventory = new Inventory();
    inventory = testInventory;  // 设置全局背包

    // 设置场景的输入绑定
    setupInputBindings();

    // 每秒更新一次状态显示
    this->schedule([this](float dt) {
        if (statusLabel) {
            std::string status = "Status: Active | Commands executed: " + 
                               std::to_string(getCommandCount()) +
                               "\nPlayer position: (" + 
                               std::to_string((int)testPlayer->getPositionX()) + ", " +
                               std::to_string((int)testPlayer->getPositionY()) + ")";
            statusLabel->setString(status);
        }
    }, 1.0f, "update_status");

    return true;
}

void InputTestScene::setupInputBindings()
{
    auto inputManager = InputManager::getInstance();
    
    // 设置上下文
    inputManager->setActiveContext("InputTestScene");
    
    // 设置命令执行回调，用于统计命令执行次数
    inputManager->setCommandExecutedCallback([]() {
        InputTestScene::incrementCommandCount();
    });
    
    // 创建背包切换命令
    auto inventoryCommand = std::make_shared<ToggleInventoryCommand>(this, testInventory, "TestScene");
    inputManager->bindPressCommand(EventKeyboard::KeyCode::KEY_ESCAPE, inventoryCommand);
    
    // 创建一个简单的测试命令来验证系统工作
    class TestCommand : public KeyCommand {
    public:
        void execute() override {
            CCLOG("Test command executed! Total: %d", InputTestScene::getCommandCount());
        }
        void undo() override {}
        std::string getDescription() const override { return "TestCommand"; }
    };
    
    auto testCommand = std::make_shared<TestCommand>();
    inputManager->bindPressCommand(EventKeyboard::KeyCode::KEY_SPACE, testCommand);
    
    CCLOG("InputTestScene input bindings set up");
}

// 静态方法实现
void InputTestScene::incrementCommandCount()
{
    commandExecuteCount++;
}

int InputTestScene::getCommandCount()
{
    return commandExecuteCount;
}

void InputTestScene::resetCommandCount()
{
    commandExecuteCount = 0;
}