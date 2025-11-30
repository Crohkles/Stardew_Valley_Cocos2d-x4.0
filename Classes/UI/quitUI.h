#pragma once
//quitUI.h
//退出界面
#include "cocos2d.h"  
#include "../Core/AppDelegate.h"
#include "../Core/InputManager.h"
#include "../Commands/UICommand.h"
#include <memory>

class quitUI : public cocos2d::Layer {
public:
    virtual bool init ( std::string sceneName );
    virtual ~quitUI();

    static quitUI* create ( std::string sceneName );

private:
    // Command Pattern??
    std::shared_ptr<CloseUICommand> escCloseCommand;
    
    void close ();

    void backgroundcreate ();

    void Buttons_switching ();

    void updateCoordinate ( float& x , float& y );

    void quitAndsetting ();

    std::string SceneName;
};