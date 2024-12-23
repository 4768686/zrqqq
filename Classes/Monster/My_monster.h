#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Carrot.h"
#include "Classes\Barrier\Barrier.h"
#include "Way.h"
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

class Monster : public cocos2d::CCSprite
{
public:
    int mapNum;
    //精灵是否还活着
    bool isAlive;
    virtual void update(float dt);
    void initMonsterType1(int a);
    void initMonsterType2(int a);
    void initMonsterType3(int a);
    void initMonsterboss(int a);
    //是否受伤，子弹每次攻击时调用该函数
    void beHurt(int monster_blood, int type);
    bool choosed;
    bool beShoot = 0;
    Sprite* choice;
    Sprite* bloodBox;
    Sprite* beHit;
    Sprite* sprBlood;
    Sprite* freeze;
    //总血量
    int hp_total;
    int freeze_total;
    //生命值
    CC_SYNTHESIZE(float, _hp, HP);
    //行走速度
    CC_SYNTHESIZE(float, _walkSpeed, WalkSpeed);
    //伤害值
    CC_SYNTHESIZE(float, _damage, Damage);
    //金钱
    CC_SYNTHESIZE(float, _money, Money);
    //冰冻
    CC_SYNTHESIZE(float, _freeze, Freeze);
    int BLOOD_BAR_m;
    //精灵序号，为每种精灵编一个序号
    int _spriteIndex;
    //GameMediator* m;
    //当前精灵的位置
    cocos2d::Point myPosition;
    //当前走到第几个结点了
    int waypoint;
    //开始路点
    cocos2d::Point beginningWaypoint;
    //结束路点
    cocos2d::Point destinationWaypoint;
    //是否死亡
    bool isDie();
    //是否被冰冻
    bool isFreezed();
    //创建血量条底图
    void create_Health_bar();
    //血量条根据血量改变
    void scheduleBlood();
    //过时后取消冰冻效果
    void unFreezed(float a);
    //怪物移动
    void walk();
    //判断怪物是否到了下一个结点
    bool judge_dest();
    //删除当前怪物结点
    void deletemonster(float a);
    //怪物是否被选中
    void touched();
    //选中后产生头上红点
    void betouched();
    void deletebehit(float a);
    CREATE_FUNC(Monster);
};

extern cocos2d::Vector<Monster*> monsterContainer;
extern int monsternum;
extern int die_monsternum;
extern bool choose_a_object;