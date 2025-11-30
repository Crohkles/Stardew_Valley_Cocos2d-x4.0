//intimacyUI.h
//人物亲密度界面
#pragma once
#include "cocos2d.h"  
#include "NpcRelationship.h"  
#include "AppDelegate.h"
#include "InventoryUI.h"
#include <NPC.h>
#include "SkillTreeUI.h"
#include "InputManager.h"
#include "UICommand.h"
#include <memory>

class intimacyUI : public cocos2d::Layer {
public:
    virtual bool init ( std::string sceneName );
    virtual ~intimacyUI();

    static intimacyUI* create ( std::string sceneName );

    void backgroundcreate ();

    void Buttons_switching ();

    void close ();

    void updateCoordinate ( float& x , float& y );

    void characterInfo ( const string& name , const string& status , Vec2 Pos_photo);

    void intimacyDisplay ( const string& name , Vec2 Pos );

private:  
    std::string SceneName;
    
    // Command Pattern相关
    std::shared_ptr<CloseUICommand> escCloseCommand;

    NpcRelationship* NPC_RELATIONSHIP;
};
                            