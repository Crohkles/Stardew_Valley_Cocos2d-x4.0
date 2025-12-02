#include "ui/CocosGUI.h"  
#include "quitUI.h"
#include "../Commands/UICommand.h"
#include "../Factories/SceneBoundaryFactory.h"

USING_NS_CC;

static void problemLoading ( const char* filename )
{
    printf ( "Error while loading: %s\n" , filename );
    printf ( "Depending on how you compiled you might have to add 'Resources/' in front of filenames in CreateCharacterScene.cpp\n" );
}

void quitUI::updateCoordinate ( float& x , float& y ) {
    Vec2 position = player1->getPosition ();
	auto boundary = SceneBoundaryFactory::getBoundaryConfig ( SceneName );
    if (x <= boundary.left) {
        x = boundary.left;
    }
    else if (x >= boundary.right) {
        x = boundary.right;
    }
    else {
        x = position.x;
    }

    if (y >= boundary.upper) {
        y = boundary.upper;
    }
    else if (y <= boundary.lower) {
        y = boundary.lower;
    }
    else {
        y = position.y;
    }
}

void quitUI::backgroundcreate () {
    Vec2 position = player1->getPosition ();
    float currentx = position.x , currenty = position.y;
    updateCoordinate ( currentx , currenty );
    auto visibleSize = Director::getInstance ()->getVisibleSize ();
    // 创建一个半透明的黑色遮罩
    auto darkLayer = cocos2d::LayerColor::create ( cocos2d::Color4B ( 0 , 0 , 0 , 120 ) , 10 * visibleSize.width , 5 * visibleSize.height );  // 黑色，透明度为120
    darkLayer->setPosition ( Vec2 ( currentx , currenty ) - visibleSize );// 设置遮罩层的位置
    this->addChild ( darkLayer , 0 );
    //大框架
    auto quitFace = Sprite::create ( "UIresource/SkillTree/background.png" );
    if (quitFace == nullptr)
    {
        problemLoading ( "'background.png'" );
    }
    else
    {
        // 获取原始图片的宽高
        float originalWidth = quitFace->getContentSize ().width;
        float originalHeight = quitFace->getContentSize ().height;
        // 根据屏幕宽度和图片原始宽高计算比例
        float scaleX = visibleSize.width / originalWidth;
        float scaleY = visibleSize.height / originalHeight;
        // 选择最小的缩放比例，以保证图片完全显示在屏幕上且不变形
        float scale = std::min ( scaleX , scaleY );
        quitFace->setScale ( scale / 1.5 );
        quitFace->setPosition ( Vec2 ( currentx , currenty ) );

        this->addChild ( quitFace , 1 );
    }
}

void quitUI::Buttons_switching () {
    Vec2 position = player1->getPosition ();
    float currentx = position.x , currenty = position.y;
    updateCoordinate ( currentx , currenty );
    auto visibleSize = Director::getInstance ()->getVisibleSize ();
    //图标显示
    auto bagkey = Sprite::create ( "UIresource/beibao/bagkey.png" );
    auto Skillkey = Sprite::create ( "UIresource/beibao/Skillkey.png" );
    auto intimacykey = Sprite::create ( "UIresource/beibao/intimacykey.png" );
    auto quitkey = Sprite::create ( "UIresource/beibao/quit.png" );
    if (bagkey == nullptr)
    {
        problemLoading ( "'bagkey.png'" );
    }
    else
    {
        // 获取原始图片的宽高
        float originalWidth = bagkey->getContentSize ().width;
        float originalHeight = bagkey->getContentSize ().height;
        // 根据屏幕宽度和图片原始宽高计算比例
        float scaleX = visibleSize.width / originalWidth;
        float scaleY = visibleSize.height / originalHeight;
        // 选择最小的缩放比例，以保证图片完全显示在屏幕上且不变形
        float scale = std::min ( scaleX , scaleY );
        bagkey->setScale ( scale / 16.5 );
        bagkey->setPosition ( Vec2 ( currentx - visibleSize.width * 0.25 , currenty + visibleSize.height * 0.315 ) );//0.305是选中时位置
        Skillkey->setScale ( scale / 16.5 );
        Skillkey->setPosition ( Vec2 ( currentx - visibleSize.width * 0.19 , currenty + visibleSize.height * 0.315 ) );//0.315是未选中时位置
        intimacykey->setScale ( scale / 16.5 );
        intimacykey->setPosition ( Vec2 ( currentx - visibleSize.width * 0.13 , currenty + visibleSize.height * 0.315 ) );
        quitkey->setScale ( scale / 16.5 );
        quitkey->setPosition ( Vec2 ( currentx - visibleSize.width * 0.07 , currenty + visibleSize.height * 0.305 ) );
        this->addChild ( bagkey , 2 );
        this->addChild ( Skillkey , 2 );
        this->addChild ( intimacykey , 2 );
        this->addChild ( quitkey , 2 );
    }

    //动画以及切换Layer
    auto listener = EventListenerMouse::create ();
    listener->onMouseDown = [this , bagkey , Skillkey , intimacykey , quitkey]( EventMouse* event ) {
        Vec2 mousePos = Vec2 ( event->getCursorX () , event->getCursorY () );
        mousePos = this->convertToNodeSpace ( mousePos );
        //CCLOG ( "X:%f,Y:%f" , event->getCursorX () , event->getCursorY () );
        if (bagkey->getBoundingBox ().containsPoint ( mousePos )) {
            std::string nowScene = SceneName;
            auto inventoryUI = InventoryUI::create(inventory, nowScene);
            Director::getInstance()->getRunningScene()->addChild(inventoryUI, 20);
            ToggleInventoryCommand::updateState(ToggleInventoryCommand::UIState::Inventory, inventoryUI);
            this->removeFromParent();
        }
        else if (Skillkey->getBoundingBox ().containsPoint ( mousePos )) {
            std::string nowScene = SceneName;
            auto skillTreeUI = SkillTreeUI::create(nowScene);
            Director::getInstance()->getRunningScene()->addChild(skillTreeUI, 20);
            ToggleInventoryCommand::updateState(ToggleInventoryCommand::UIState::SkillTree, skillTreeUI);
            this->removeFromParent();
        }
        else if (intimacykey->getBoundingBox ().containsPoint ( mousePos )) {
            std::string nowScene = SceneName;
            auto intimacyUI = intimacyUI::create(nowScene);
            Director::getInstance()->getRunningScene()->addChild(intimacyUI, 20);
            ToggleInventoryCommand::updateState(ToggleInventoryCommand::UIState::Intimacy, intimacyUI);
            this->removeFromParent();
        }
        else if (quitkey->getBoundingBox ().containsPoint ( mousePos )) {
        }
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority ( listener , this );
}

void quitUI::quitAndsetting () {
    Vec2 position = player1->getPosition ();
    float currentx = position.x , currenty = position.y;
    updateCoordinate ( currentx , currenty );
    auto visibleSize = Director::getInstance ()->getVisibleSize ();
    auto quit = Sprite::create ( "UIresource/quit.png" );
    if (quit == nullptr)
    {
        problemLoading ( "'quit.png'" );
    }
    else
    {
        // 获取原始图片的宽高
        float originalWidth = quit->getContentSize ().width;
        float originalHeight = quit->getContentSize ().height;
        // 根据屏幕宽度和图片原始宽高计算比例
        float scaleX = visibleSize.width / originalWidth;
        float scaleY = visibleSize.height / originalHeight;
        // 选择最小的缩放比例，以保证图片完全显示在屏幕上且不变形
        float scale = std::min ( scaleX , scaleY );
        quit->setScale ( scale / 5.5 );
        quit->setPosition ( Vec2 ( currentx , currenty ) );

        this->addChild ( quit , 1 );

        auto listener = EventListenerMouse::create ();
        listener->onMouseMove = [this , quit , scale]( EventMouse* event ) {
            Vec2 mousePos = Vec2 ( event->getCursorX () , event->getCursorY () );
            mousePos = this->convertToNodeSpace ( mousePos );
            if (quit->getBoundingBox ().containsPoint ( mousePos )) {
                quit->setScale ( scale / 5.5 * 1.2f );
            }
            else
                quit->setScale ( scale / 5.5 );
            };
        listener->onMouseDown = [this , quit]( EventMouse* event ) {
            Vec2 mousePos = Vec2 ( event->getCursorX () , event->getCursorY () );
            mousePos = this->convertToNodeSpace ( mousePos );
            if (quit->getBoundingBox ().containsPoint ( mousePos )) {
                Director::getInstance ()->end ();
            }
            };
        _eventDispatcher->addEventListenerWithSceneGraphPriority ( listener , quit );
    }
}

quitUI::~quitUI() {
}

bool quitUI::init ( std::string sceneName ) {
    if (!Layer::init ()) {
        return false;
    }
    SceneName = sceneName;
    backgroundcreate ();
    quitAndsetting ();
    Buttons_switching ();
    return true;
}

quitUI* quitUI::create ( std::string sceneName ) {
    quitUI* ret = new quitUI ();
    if (ret && ret->init ( sceneName )) {
        ret->autorelease ();
        return ret;
    }
    CC_SAFE_DELETE ( ret );
    return nullptr;
}