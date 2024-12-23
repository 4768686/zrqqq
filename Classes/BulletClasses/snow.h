#ifndef __SNOW_FLOWER_BULLET_H_
#define __SNOW_FLOWER_BULLET_H_
#include "Classes\BulletClasses\Bullet.h"
#include "Classes\Monster\My_monster.h"

using namespace cocos2d;

class SnowFlowerBullet : public Bullet {
public:
    virtual bool initSnowBullet(int grade);

    CREATE_FUNC(SnowFlowerBullet);

    void spread();

    void checkCollision(float dt);

    virtual void inputBulletAction(Point towerLoc);

    void spreadSpriteToScope();

    int speed;

    CC_SYNTHESIZE(float, _freeze, Freeze);

    ScaleTo* spreadTo;

    Point src;

    int myGrade;
};
#endif