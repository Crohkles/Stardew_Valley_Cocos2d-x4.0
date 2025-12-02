// InventoryUI.h  
#pragma once  
#include "cocos2d.h"  
#include "../Systems/Inventory.h"  
#include "../Core/AppDelegate.h"
#include "intimacyUI.h"
#include "SkillTreeUI.h"

class InventoryUI : public cocos2d::Layer, public GameStateObserver {
public:
    virtual bool init ( Inventory* inventory , std::string sceneName );
    
    virtual ~InventoryUI();

    static InventoryUI* create ( Inventory* inventory , std::string sceneName );

    void backgroundcreate ();

    void Itemblock ( Inventory* inventory );

    void updateDisplay (); // 更新显示内容  

    void Buttons_switching ();

    void updateCoordinate ( float &x , float &y );
    
    virtual void onInventoryStateChanged() override;
private:
    Inventory* _inventory; // 指向 Inventory 实例的指针  

    cocos2d::Label* _itemLabel;  // 显示物品信息的标签 

    cocos2d::Vector<cocos2d::Sprite*> _itemSlots; // 存储物品槽的 Sprite  

    int _selectedSlot; // 当前选中的槽位 

    bool isClick = false;  // 标志，表示是否被点击

    Sprite* currentItemSprite = nullptr; // 标识当前选择的物品

    std::string SceneName;
};
