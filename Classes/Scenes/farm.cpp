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
}

bool farm::init ()
{
    AudioEngine::pauseAll ();
    auto backgroundAudioID = AudioEngine::play2d ( "MUSIC/TheShire.mp3" , true );

    auto visibleSize = Director::getInstance ()->getVisibleSize ();
    Vec2 origin = Director::getInstance ()->getVisibleOrigin ();

    button = cocos2d::Sprite::create ( "CloseNormal.png" );
    this->addChild ( button , 11 );

    // ???????
    TimeUI = Timesystem::create ( "farm" );
    this->addChild ( TimeUI , 13 );

    // ????
    AllInitialize_crop ();

    if (Weather == "Rainy") {
        // ??
        createRainEffect ();
    }

    // ??????
    auto background_real = Sprite::create ( "farm/farm.png" );
    background_real->setPosition ( Vec2 ( visibleSize.width / 2 , visibleSize.height / 2 ) );
    this->addChild ( background_real , 1 );
    background_real->setScale ( 1.5f );

    auto background = Sprite::create ( "farm/farm_path.png" );
    this->addChild ( background , 5 );
    background->setPosition ( Vec2 ( visibleSize.width / 2 , visibleSize.height / 2 ) );
    background->setScale ( 1.5f );


    Vec2 spritePosition = background->getPosition ();   // ????????????
    Size spriteSize = background->getContentSize ();    // ??????????????


    // ????????
    Vec2 leftBottomPosition = Vec2 (
        spritePosition.x - background->getScaleX () * spriteSize.width / 2 ,   // ??? x ?????????
        spritePosition.y - background->getScaleY () * spriteSize.height / 2   // ??? y ?????????
    );


    Image img;
    if (img.initWithImageFile ( "farm/farm_path.png" ))
    {
        int width = img.getWidth ();
        int height = img.getHeight ();

        // ??????
        unsigned char* data = img.getData ();

        // ????????????????????0?
        for (int y = 0; y < height; y = y + 8)
        {
            for (int x = 0; x < width; x = x + 8)
            {
                // ??????? RGBA ?
                int index = (y * width + x) * 4;  // ?????? 4 ??? (RGBA)
                unsigned char a = data[index + 3];  // ???

                // ????? (alpha) ?? 0?????????
                if (a > 0)
                {
                    float screenX = leftBottomPosition.x + x * background->getScaleX ();
                    float screenY = leftBottomPosition.y + (height - y - 1) * background->getScaleY ();  // ?? Y ???
                    nonTransparentPixels.push_back ( Vec2 ( screenX , screenY ) );  // ??????
                }
            }
        }
    }

    // ?????????????
    if (player1->getParent () == NULL) {
        this->addChild ( player1 , 17 );
        // ?????????????
        player1->setupInputBindings();
        // ???????
        player1->setCollisionContext(nonTransparentPixels);
        for (auto& pair : F_lastplace) {
            if (pair.second == true) {  // ?? bool ???? true
                player1->setPosition ( pair.first.second );
                pair.second = false;
            }
        }
         player1->speed = 4.7f;
         //??????
       /* player1->speed = 20.0f;*/
        player1->setScale ( 1.5f );
        player1->setAnchorPoint ( Vec2 ( 0.5f , 0.2f ) );
    }

    // ????????
    player1->schedule ( [=]( float dt ) {
        player1->player1_move ();
        } , 0.05f , "player1_move" );

    player1->schedule ( [=]( float dt ) {
        player1->player_change ();
        } , 0.3f , "player_change" );


    // ????????????
    float scaledWidth = background->getContentSize ().width * background->getScaleX ();
    float scaledHeight = background->getContentSize ().height * background->getScaleY ();

    // ?? Follow ??? Rect
    auto followRect = cocos2d::Rect ( leftBottomPosition.x , leftBottomPosition.y , scaledWidth , scaledHeight );

    // ?? Follow ???????????????
    auto followAction = Follow::create ( player1 , followRect );
    this->runAction ( followAction );

    // ????????
    this->schedule ( [this]( float dt ) {
        this->checkPlayerPosition ();  // ???????????
        } , 0.01f , "check_position_key" );

    //???????????
    auto mailBox = Sprite::create ( "UIresource/xinxiang/xinxiang.png" );
    mailBox->setPosition ( Vec2 ( 260 , 1050 ) );
    mailBox->setScale ( 0.7f );
    this->addChild ( mailBox , 10 );

    //??????????
    Box = Sprite::create ( "UIresource/xiangzi/xiangzi.png" );
    Box->setPosition ( Vec2 ( 260 , 1150 ) );
    Box->setAnchorPoint ( Vec2 ( 0 , 0 ) );
    Box->setScale ( 0.7f );
    this->addChild ( Box , 10 );

    //???? ????
    auto cooking_pot = CookingPot::create ();
    cooking_pot->setPosition ( -50 , 1000 );
    cooking_pot->setAnchorPoint ( Vec2 ( 0 , 0 ) );
    cooking_pot->setScale ( 2.7f );
    this->addChild ( cooking_pot , 10 );
    

    //????
    float interactionRadius = 200.0f;

    auto listener = EventListenerMouse::create ();
    listener->onMouseDown = [this , mailBox , interactionRadius, cooking_pot]( Event* event ) {

        // ?????????
        auto mouseEvent = static_cast<EventMouse*>(event);
        Vec2 clickPos ( mouseEvent->getCursorX () , mouseEvent->getCursorY () );
        clickPos = this->convertToNodeSpace ( clickPos );

        if (Box->getBoundingBox ().containsPoint ( clickPos )) {

            // ???????  
            Vec2 playerPos = player1->getPosition ();

            Vec2 BoxPos = Box->getPosition ();

            // ?????NPC?????  
            float distance = playerPos.distance ( BoxPos );

            // ?????????????  
            if (distance <= interactionRadius) {
                if (miniBag->getSelectedSlot ()) {
                    GoldAmount += inventory->GetItemAt ( miniBag->getSelectedSlot () )->GetValue ();
                    inventory->RemoveItem ( miniBag->getSelectedSlot () );
                    inventory->is_updated = true;
                    miniBag->getSelectBack ();
                }
            }
        }

        //?????????
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

        // ????????????????
        if (button != nullptr && button->getBoundingBox ().containsPoint ( clickPos )) {
            Director::getInstance ()->end ();
        }
        if (mailBox->getBoundingBox ().containsPoint ( clickPos )) {
            // ???????  
            Vec2 playerPos = player1->getPosition ();

            // ????????????  
            float distance = playerPos.distance ( mailBox->getPosition () );
            // ?????????????  
            if (distance <= interactionRadius) {
                mailBoxUI* mailbox = mailBoxUI::create ();
                this->addChild ( mailbox , 20 );
            }
        }
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority ( listener , this );

    // ???????????Command Pattern
    
    // ??Command Pattern????
    setupInputCommands();

    //????????
    miniBag = mini_bag::create ( inventory );
    miniBag->setScale ( 1.0f );
    Vec2 pos = miniBag->getPosition ();
    if (miniBag != NULL) {
        cocos2d::log ( "miniBagtest %f" , pos.x );
    }
    if (!this->getChildByName ( "mini_bag" )) {
        this->addChild ( miniBag , 10 , "mini_bag" );
    }


    // ?????
    // ??Observer???????????????
    // ?????
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


    for (auto it = Crop_information.begin (); it != Crop_information.end (); /* no increment here */) {

        auto crop = *it;  // ????????? Crop ??
        int nums = crop->nums;

        cocos2d::log ( "set nums = %d" , nums );

        // ????
        std::string type = crop->GetName ();

        // ??????????????
        if (crop->GetPhase () == Phase::MATURE) {
            auto test = Sprite::create ( crop->mature_pic );
            this->addChild ( test , 15 - nums / 19 );
            test->setPosition ( 545 + ((nums % 19) - 2) * 48 , 910 - ((nums / 19) - 1) * 48 );
            test->setScale ( 1.9f );
        }
        else if (crop->GetPhase () == Phase::GROWING) {
            auto test = Sprite::create ( crop->growing_pic );
            this->addChild ( test , 15 - nums / 19 );
            test->setPosition ( 545 + ((nums % 19) - 2) * 48 , 910 - ((nums / 19) - 1) * 48 );
            test->setScale ( 2.1f );
        }
        else if (crop->GetPhase () == Phase::SAPLESS) {
            auto test = Sprite::create ( "crop/sapless.png" );
            this->addChild ( test , 15 - nums / 19 );
            test->setPosition ( 545 + ((nums % 19) - 2) * 48 , 910 - ((nums / 19) - 1) * 48 );
            test->setScale ( 2.1f );
        }
        else {
            auto test = Sprite::create ( crop->initial_pic );
            this->addChild ( test , 15 - nums / 19 );
            test->setPosition ( 545 + ((nums % 19) - 2) * 48 , 910 - ((nums / 19) - 1) * 48 );
            test->setScale ( 2.1f );
        }

        // ???????
        ++it;
    }


}

// ??????????????
void farm::checkPlayerPosition ()
{

    // ???????
    Vec2 playerPos = player1->getPosition ();
    //????????
    if (playerPos.y < 0) {
        miniBag->setLocalZOrder ( 0 );
    }
    else {
        miniBag->setLocalZOrder ( 11 );  
    }


    // ????????????  
    float distance = playerPos.distance ( Box->getPosition () );
    // ?????????????  
    if (distance <= 200.0f) {
        Box->setTexture ( "UIresource/xiangzi/Open.png" );
    }
    else {
        Box->setTexture ( "UIresource/xiangzi/xiangzi.png" );
    }

    // ???????
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

            auto crop = *it;  // ????????? Crop ??

            if (day == 1) {
                crop->watered = true;
            }
            if ((day - 1) % 3 == 1) {
                crop->watered = true;
            }

            // ?????????
            if ((crop->watered == false) && (crop->GetPhase () != Phase::MATURE)) {
                // ????????????
                if (crop->GetPhase () != Phase::SAPLESS) {
                    crop->ChangePhase ( Phase::SAPLESS );
                    crop->ChangMatureNeeded ( 2 ); // ??????
                    it++;
                }
                else {
                    // ??????????
                    it = Crop_information.erase ( it );
                }

            }
            else {
                // ????
                crop->UpdateGrowth ();
                it++;
            }

        }

        for (auto& pair : F_lastplace) {
            if (pair.first.first == "myhouse") {  // ?? bool ???? true
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
    // ??????
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

    // ????????
    if (plant_area.containsPoint ( playerPos )) {

        // ????????P?Command??
        // ????????W?Command??
        // ????????G?Command??

    }

    // ????????????ENTER?Command??

    // ????????????ENTER?Command??

    // ????????????ENTER?Command??

    // ????????????ENTER?Command??

    // ????????????ENTER?Command??

    // ?????????Player??updateMovementPermissions???
    // ??setCollisionContext???????
    // ??????Player?player1_move()?????
}

int farm::getRegionNumber ( Vec2 pos ) {

    // ?????????
    int left_bottom_x = 496;  // ???x??
    int left_bottom_y = 467;  // ???y??
    int width = 912;          // ????
    int height = 480;         // ????
    int block_size = 48;      // ?????

    // ?????????
    int rows = height / block_size;  // ??
    int cols = width / block_size;   // ??

    // ????????????
    int col = (pos.x - left_bottom_x) / block_size;
    int row = (left_bottom_y + height - pos.y) / block_size;
    // ????
    if (col < 0) {
        col = 0;
    }
    if (row < 0) {
        row = 0;
    }
    // ???????????
    int region_number = (row)*cols + col + 1;  // ???1??

    return region_number;
}

void farm::createRainEffect () {

    emitter = ParticleRain::create ();
    emitter->setDuration ( ParticleSystem::DURATION_INFINITY );
    emitter->setScale ( 5.7f );
    emitter->setTotalParticles ( 100 );
    emitter->setSpeed ( 250 );

    addChild ( emitter , 10 );

    // ??????????
    schedule ( [this]( float dt ) {
        updaterain ( dt );
        } , "update_rain_key" );

}


void farm::updaterain ( float deltaTime ) {
    if (emitter) {
        // ????????????? 1 ? 1.5 ????
        float newLife = cocos2d::rand_0_1 () * 1.5f;

        // ????????
        emitter->setLife ( newLife );

        emitter->setEmissionRate ( emitter->getTotalParticles () / emitter->getLife () * 1.3 );
    }
}

void farm::setupInputCommands() {
    auto inputManager = InputManager::getInstance();

    // ???????????
    auto plantCommand = std::make_shared<PlantCommand>(this);
    auto waterCommand = std::make_shared<WaterCommand>(this);
    auto harvestCommand = std::make_shared<HarvestCommand>(this);
    auto toggleInventoryCommand = std::make_shared<ToggleInventoryCommand>(
        this, inventory, "farm"
    );

    // ???????? - ??ConditionalSceneTransitionCommand
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
            // ??????
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
            // ??????
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
            // ??????
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
            // ??????
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

    // ??????? - ??bindPressCommand
    inputManager->bindPressCommand(EventKeyboard::KeyCode::KEY_P, plantCommand);
    inputManager->bindPressCommand(EventKeyboard::KeyCode::KEY_W, waterCommand);
    inputManager->bindPressCommand(EventKeyboard::KeyCode::KEY_G, harvestCommand);
    inputManager->bindPressCommand(EventKeyboard::KeyCode::KEY_ESCAPE, toggleInventoryCommand);
    
    // ENTER???????????
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

    // ??????????
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
    
    // ??????
    for (auto& command : boundCommands) {
        inputManager->unbindCommand(EventKeyboard::KeyCode::KEY_ESCAPE, command);
        inputManager->unbindCommand(EventKeyboard::KeyCode::KEY_P, command);
        inputManager->unbindCommand(EventKeyboard::KeyCode::KEY_W, command);
        inputManager->unbindCommand(EventKeyboard::KeyCode::KEY_G, command);
        inputManager->unbindCommand(EventKeyboard::KeyCode::KEY_ENTER, command);
        inputManager->unbindCommand(EventKeyboard::KeyCode::KEY_KP_ENTER, command);
    }
    
    // ??????
    boundCommands.clear();
}

void farm::performPlantAction() {
    Vec2 playerPos = player1->getPosition();
    
    // ??????????
    if (!plant_area.containsPoint(playerPos)) {
        return;
    }
    
    int nums = getRegionNumber(Vec2(playerPos.x + 10, playerPos.y - 10));
    bool Isplant = false;
    
    CCLOG("plant nums = %d", nums);
    
    // ??????????????
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
                // ???????????
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
                        // ???????????????
                        player1->setTexture("character1/player_plant3.png");
                        player1->setScale(2.5f);

                        // ??0.3??????????
                        player1->scheduleOnce([=](float dt) {
                            player1->setTexture("character1/player_plant4.png");  // ???player_plant2
                            player1->setScale(2.7f);
                        }, 0.15f, "change_image1_key");

                        // ??0.6??????????
                        player1->scheduleOnce([=](float dt) {
                            player1->setTexture("character1/player_right3.png"); // ???player_left3
                            player1->setScale(1.5f);
                            auto temp = Sprite::create(Crop_information.back()->initial_pic);
                            this->addChild(temp, 15 - nums / 19);
                            temp->setPosition(500 + ((nums % 19) - 1) * 48, 910 - ((nums / 19) - 1) * 48);
                            temp->setScale(2.1f);
                        }, 0.35f, "change_image2_key");
                    }
                    else {
                        // ???????????????
                        player1->setTexture("character1/player_plant1.png");
                        player1->setScale(2.5f);

                        // ??0.3??????????
                        player1->scheduleOnce([=](float dt) {
                            player1->setTexture("character1/player_plant2.png");  // ???player_plant2
                            player1->setScale(2.7f);
                        }, 0.15f, "change_image1_key");

                        // ??0.6??????????
                        player1->scheduleOnce([=](float dt) {
                            player1->setTexture("character1/player_left3.png"); // ???player_left3
                            player1->setScale(1.5f);
                            auto temp = Sprite::create(Crop_information.back()->initial_pic);
                            this->addChild(temp, 15 - nums / 19);
                            temp->setPosition(500 + ((nums % 19) - 1) * 48, 910 - ((nums / 19) - 1) * 48);
                            temp->setScale(2.1f);
                        }, 0.35f, "change_image2_key");
                    }
                }
            }
        }
    }
}

void farm::performWaterAction() {
    Vec2 playerPos = player1->getPosition();
    
    // ??????????
    if (!plant_area.containsPoint(playerPos)) {
        return;
    }
    
    int nums = getRegionNumber(Vec2(playerPos.x + 30, playerPos.y - 10));

    for (auto it : Crop_information) {
        if (it->nums == nums) {
            // ?????
            it->watered = true;

            if (player1->pic_path == "character1/player_left3.png") {
                // ???????????????
                player1->setTexture("character1/player_water4.png");
                player1->setScale(1.7f);

                // ??0.3??????????
                player1->scheduleOnce([=](float dt) {
                    player1->setTexture("character1/player_water3.png");  // ???player_water1
                    player1->setScale(1.7f);
                }, 0.15f, "change_image1_key");

                // ??0.6??????????
                player1->scheduleOnce([=](float dt) {
                    player1->setTexture("character1/player_left3.png"); // ???player_right3
                    player1->setScale(1.5f);
                    // ???????????
                    player1->moveDown = true;
                    player1->moveLeft = true;
                    player1->moveUp = true;
                    player1->moveRight = true;
                }, 0.35f, "change_image2_key");
            }
            else {
                // ???????????????
                player1->setTexture("character1/player_water2.png");
                player1->setScale(1.7f);

                // ??0.3??????????
                player1->scheduleOnce([=](float dt) {
                    player1->setTexture("character1/player_water1.png");  // ???player_water1
                    player1->setScale(1.7f);
                }, 0.15f, "change_image1_key");

                // ??0.6??????????
                player1->scheduleOnce([=](float dt) {
                    player1->setTexture("character1/player_right3.png"); // ???player_right3
                    player1->setScale(1.5f);
                    // ???????????
                    player1->moveDown = true;
                    player1->moveLeft = true;
                    player1->moveUp = true;
                    player1->moveRight = true;
                }, 0.35f, "change_image2_key");
            }
            break; // ????????????
        }
    }
}

void farm::performHarvestAction() {
    Vec2 playerPos = player1->getPosition();
    
    // ??????????
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

                // ????
                auto test = Sprite::create("farm/tile.png");
                this->addChild(test, 15 - nums / 19);
                test->setPosition(495 + ((nums % 19) - 1) * 48, 910 - ((nums / 19) - 1) * 48);
                test->setScaleX(2.5f);
                test->setScaleY(1.9f);

                // ??????????
                it = Crop_information.erase(it);  // erase ??????? 

                // ???????????????
                player1->setTexture("character1/player_plant1.png");
                player1->setScale(2.5f);

                // ??0.3??????????
                player1->scheduleOnce([=](float dt) {
                    player1->setTexture("character1/player_plant2.png");  // ???player_plant2
                    player1->setScale(2.7f);
                }, 0.15f, "change_image1_key");

                // ??0.6??????????
                player1->scheduleOnce([=](float dt) {
                    player1->setTexture("character1/player_left3.png"); // ???player_left3
                    player1->setScale(1.5f);
                }, 0.35f, "change_image2_key");
                
                return; // ???????
            }
            else {
                return; // ??????????
            }
        }
        else {
            ++it;  // ?????????
        }
    }
}







