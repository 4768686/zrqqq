#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Carrot.h"
#include "Way.h"
#include "Classes/Monster/My_monster.h"
#define CREATE_FUNC(__TYPE__) \
static __TYPE__* create() \
{ \
    __TYPE__ *pRet = new(std::nothrow) __TYPE__(); \
    if (pRet && pRet->init()) \
    { \
        pRet->autorelease(); \
        return pRet; \
    } \
    else \
    { \
        delete pRet; \
        pRet = nullptr; \
        return nullptr; \
    } \
}
    
class Barrier : public cocos2d::CCSprite
{
public:
    //精灵是否还活着
    bool is_alive;
    virtual void update(float dt);
    //是否受伤
    void behurt(int monster_blood, int type);
    Sprite* choice;
    bool choosed;
    struct pos {
        int x;
        int y;
    };
    pos position[4] = { (0,0), (0,0), (0,0), (0,0) };
private:
    Sprite* bloodbox;
    Sprite* behit;
    Sprite* sprBlood;
    //总血量
    int hp_total;
    //设置生命值
    CC_SYNTHESIZE(float, _hp, HP);
    //设置金钱
    CC_SYNTHESIZE(float, _money, Money);
    int BLOOD_BAR;
    //是否死亡
    bool isDie();
    //创建血量条底图
    void create_Health_bar();
    //血量条根据血量改变
    void scheduleBlood();
    //删除当前结点
    void deletemonster(float a);
    //是否被选中
    void touched();
    //选中后产生头上红点
    void betouched();
    void deletebehit(float a);
    void initBarrier(int hp, int money, Texture2D* texture, Vec2 pos);
    void clear();
    CREATE_FUNC(Barrier);
};

extern cocos2d::Vector<Barrier*> BarrierContainer;