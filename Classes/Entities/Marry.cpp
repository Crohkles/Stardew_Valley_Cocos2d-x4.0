#include "ui/CocosGUI.h"  
#include "Marry.h"
#include "../Factories/SceneBoundaryFactory.h"

USING_NS_CC;

static void problemLoading ( const char* filename )
{
    printf ( "Error while loading: %s\n" , filename );
    printf ( "Depending on how you compiled you might have to add 'Resources/' in front of filenames in CreateCharacterScene.cpp\n" );
}

void Marry::updateCoordinate ( float& x , float& y ) {
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
    CCLOG ( "%f %f" , x , y );
}

void Marry::backgroundcreate () {
    Vec2 position = player1->getPosition ();
    float currentx = position.x , currenty = position.y;
    updateCoordinate ( currentx , currenty );
    auto visibleSize = Director::getInstance ()->getVisibleSize ();
    // 创建一个半透明的黑色遮罩
    auto darkLayer = cocos2d::LayerColor::create ( cocos2d::Color4B ( 0 , 0 , 0 , 120 ) , 10 * visibleSize.width , 5 * visibleSize.height );  // 黑色，透明度为120
    darkLayer->setPosition ( Vec2 ( currentx , currenty ) - visibleSize );// 设置遮罩层的位置
    this->addChild ( darkLayer , 0 );
    //大框架
    auto daily_record = Sprite::create ( "UIresource/SkillTree/background.png" );
    if (daily_record == nullptr)
    {
        problemLoading ( "'background.png'" );
    }
    else
    {
        // 获取原始图片的宽高
        float originalWidth = daily_record->getContentSize ().width;
        float originalHeight = daily_record->getContentSize ().height;
        // 根据屏幕宽度和图片原始宽高计算比例
        float scaleX = visibleSize.width / originalWidth;
        float scaleY = visibleSize.height / originalHeight;
        // 选择最小的缩放比例，以保证图片完全显示在屏幕上且不变形
        float scale = std::min ( scaleX , scaleY );
        daily_record->setScale ( scale / 1.5 );
        daily_record->setPosition ( Vec2 ( currentx , currenty ) );

        this->addChild ( daily_record , 1 );
    }
}

void Marry::ifmarry () {
    Vec2 position = player1->getPosition ();
    float currentx = position.x , currenty = position.y;
    updateCoordinate ( currentx , currenty );
    auto visibleSize = Director::getInstance ()->getVisibleSize ();
    std::string marryLabel = NpcName + ":will you marry me,\n" + protagonistName + "! ! ! ! ! ! ! !";
    auto ismarry = Label::createWithSystemFont ( marryLabel , "fonts/Comic Sans MS.ttf" , 80);
    ismarry->setPosition ( Vec2 ( currentx , currenty + visibleSize.height * 0.17 ) );
    this->addChild ( ismarry , 3 );
}

//对话选中框添加
void Marry::SelectedAnswer () {
    Vec2 position = player1->getPosition ();
    float currentx = position.x , currenty = position.y;
    updateCoordinate ( currentx , currenty );
    auto visibleSize = Director::getInstance ()->getVisibleSize ();
    auto Selectedbox1 = Sprite::create ( "npc/xuanzhongkuang.png" );
    auto Selectedbox2 = Sprite::create ( "npc/xuanzhongkuang.png" );
    if (Selectedbox1 == nullptr)
    {
        problemLoading ( "'npc/xuanzhongkuang.png'" );
    }
    else
    {
        Selectedbox1->setScale ( 1.2f , 1.5f  );
        Selectedbox1->setPosition ( Vec2 ( currentx - visibleSize.width * 0.05 , currenty - visibleSize.height * 0.04 ) );
        Selectedbox2->setScale ( 1.2f , 1.5f );
        Selectedbox2->setPosition ( Vec2 ( currentx - visibleSize.width * 0.05 , currenty - visibleSize.height * 0.174 ) );

        this->addChild ( Selectedbox1 , 0 );
        this->addChild ( Selectedbox2 , 0 );

        // 兩个回答
        auto Yes = Label::createWithSystemFont ( "YES!!I DO! ! BABY" , "fonts/Comic Sans MS.ttf" , 60);
        Yes->setTextColor ( cocos2d::Color4B::BLACK );
        Yes->setPosition ( Vec2 ( currentx - visibleSize.width * 0.1 , currenty - visibleSize.height * 0.04 ) );
        this->addChild ( Yes , 2 );

        auto NO = Label::createWithSystemFont ( "NO! ! I DONT LIKE U" ,"fonts/Comic Sans MS.ttf" , 60);
        NO->setTextColor ( cocos2d::Color4B::BLACK );
        NO->setPosition ( Vec2 ( currentx - visibleSize.width * 0.08 , currenty - visibleSize.height * 0.174 ) );
        this->addChild ( NO , 2 );


        // 鼠标移动事件  
        auto listener = EventListenerMouse::create ();
        listener->onMouseMove = [=]( EventMouse* event ) {
            Vec2 mousePosition = Vec2 ( event->getCursorX () , event->getCursorY () );
            mousePosition = this->convertToNodeSpace ( mousePosition );
            // 检查每个 Selectedbox  
            if (Selectedbox1->getBoundingBox ().containsPoint ( mousePosition )) {
                Selectedbox1->setLocalZOrder ( 2 ); // 显示在上层  
            }
            else {
                Selectedbox1->setLocalZOrder ( 0 ); // 隐藏在下层  
            }
            if (Selectedbox2->getBoundingBox ().containsPoint ( mousePosition )) {
                Selectedbox2->setLocalZOrder ( 2 );
            }
            else {
                Selectedbox2->setLocalZOrder ( 0 );
            }
            };
        
        listener->onMouseDown = [this , Selectedbox1 , Selectedbox2]( EventMouse* event ) {
            Vec2 mousePosition = Vec2 ( event->getCursorX () , event->getCursorY () );
            mousePosition = this->convertToNodeSpace ( mousePosition );

            // 检查每个 Selectedbox  
            if (Selectedbox1 && Selectedbox1->getBoundingBox ().containsPoint ( mousePosition )) {
                npc_relationship->increaseRelationship ( "player" , NpcName , 40 );
                this->removeFromParent ();
                AudioEngine::pauseAll ();
                auto backgroundAudioID = AudioEngine::play2d ( "MUSIC/AthousandYears.mp3" , true );
                return; // 提前返回，避免执行后续选择框检查  
            }
            if (Selectedbox2 && Selectedbox2->getBoundingBox ().containsPoint ( mousePosition )) {
                    npc_relationship->increaseRelationship ( "player" , NpcName , -80 );
                this->removeFromParent ();
                return; // 提前返回  
            }
            };

        _eventDispatcher->addEventListenerWithSceneGraphPriority ( listener , this );
    }
}

bool Marry::init ( std::string sceneName , std::string npcName ) {
    if (!Layer::init ()) {
        return false;
    }
    SceneName = sceneName;
    NpcName = npcName;
    backgroundcreate ();
    ifmarry ();
    SelectedAnswer ();
    return true;
}

Marry* Marry::create ( std::string sceneName , std::string npcName ) {
    Marry* ret = new Marry ();
    if (ret && ret->init ( sceneName ,npcName )) {
        ret->autorelease ();
        return ret;
    }
    CC_SAFE_DELETE ( ret );
    return nullptr;
}

