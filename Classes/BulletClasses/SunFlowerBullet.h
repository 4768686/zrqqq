#ifndef __SUN_FLOWER_BULLET_H_
#define __SUN_FLOWER_BULLET_H_
#include "Classes\BulletClasses\Bullet.h"
#include "Classes\Monster\My_monster.h"

using namespace cocos2d;

class SunFlowerBullet : public Bullet {
public:
    virtual bool initSunBullet(int grade);

    CREATE_FUNC(SunFlowerBullet);

    void spread();

    void checkCollision(float dt);

    virtual void inputBulletAction(Point towerLoc);

    void spreadSpriteToScope();

private:
    int speed;

    ScaleTo* spreadTo;

    Point src;

    int myGrade;
};
#endif