#ifndef __FAN_H__
#define __FAN_H__

#include "Classes\BulletClasses\Bullet.h"
#include "Classes\Monster\My_monster.h"
#include "Classes\Barrier\Barrier.h"

using namespace cocos2d;

class fanBullet : public Bullet
{
public:
    virtual bool initfanBullet(int grade);

    CREATE_FUNC(fanBullet);

    void shoot();

    virtual void inputBulletAction(Point towerLoc, Point monsterLoc);

    void rotateSpriteToDirection();

    void fanBullet::checkCollision(float dt);

    int speed;

    MoveBy* shootBy;

    Point src, dst;

    int myGrade;
};

#endif // __FAN_TOWER_BULLET_H__

