#ifndef __CARROT_H_
#define __CARROT_H_
#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

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

struct filesName {
    std::string fileName = { "\0" };
};
class Carrot :public Sprite
{
public:
    bool initCarrot();                          // 初始化萝卜
    void setBitenAttack(float a = 1.0);
    void beBiten();
    void setLocation(Point mapDest);            // 设定位置
    void runTwistAnimation();
    void set_beBiten(bool set) { _beBiten = set; }
    void beTouched();                           // 是否点击
    virtual void update(float dt);              // 更新状态
    bool getIsAlive() { return isAlive; }
    bool upgrade();

    float scale;
    Point CarrotPos;    
    filesName bitenTectures[10];
    Sprite* CarrotBloodBox;
    Sprite* BloodNum;
    bool _beBiten;
    bool isAlive;
    float bitenDamage;
    float myGrade;

    CC_SYNTHESIZE(float, _hp, HP);
    void createBloodBox();
    void changeBloodBox(); 
    void changeCarrotAppearance();
    bool isDie();
    CREATE_FUNC(Carrot);
};

extern Vector<Carrot*> myCarrot;
#endif // __CARROT_H__