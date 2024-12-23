#ifndef __BOTTLE_TOWER_BULLET_H_
#define __BOTTLE_TOWER_BULLET_H_
#include "Classes\BulletClasses\Bullet.h"
#include "Classes\Monster\My_monster.h"
#include "Classes\Barrier\Barrier.h"

using namespace cocos2d;

class GreenTowerBullet : public Bullet {
public:
    virtual bool initGreenBullet(int grade);

    CREATE_FUNC(GreenTowerBullet);

    void shoot();

    void removeBullet();

    virtual void inputBulletAction(Point towerLoc, Point monsterLoc);

    void rotateSpriteToDirection();

    int speed;

    MoveBy* shootBy;

    Point src, dst;

    int myGrade;
};
#endif//__BOTTLE_TOWER_BULLET_H_