#include "Classes\Tower\BottleTower.h"
#include"Classes\Monster\My_monster.h"
#include"Classes\BulletClasses\BottleTowerBullet.h"



BottleTower::BottleTower()
{

}

BottleTower* BottleTower::create(const std::string& towerImage)
{
    BottleTower* tower = new BottleTower();
    if (tower && tower->init(towerImage)){
        tower->autorelease();
        return tower;
    }
    else{
        delete tower;
        return nullptr;
    }
}


bool BottleTower::init(const std::string& towerImage)
{
    if (!Tower::init(towerImage))
        return false;

    // 注册 update 函数
    schedule([=](float dt) {
        this->update(dt);
        }, "update_key");

    return true;
}

void BottleTower::performAttack()
{
    my_pos = getPosition();//获得我的位置
    float distance;

    //方向向量
    Vec2 direction = target_pos - my_pos;
    setAnchorPoint(Vec2(0.5, 0.5));
    // 计算角度（弧度）
    const float angle = atan2(direction.y, direction.x);

    // 将弧度转换为角度
    const float rotation = CC_RADIANS_TO_DEGREES(angle);

    // 设置炮塔的旋转角度
    setRotation(-rotation + 90);
}

void BottleTower::update(float d)
{
    if (isTargetInRange()) {
        attackTimer += d;
        performAttack();
        
        if (attackTimer >= attackInterval) {
            attackTimer = 0.0f;  // 重置计时器

            auto bulletSprite1 = GreenTowerBullet::create();
            this->addChild(bulletSprite1, 9);
            bulletSprite1->initGreenBullet(level);

            // 获取子弹发射起点和目标点的局部坐标
            auto bulletStartPos = this->convertToNodeSpace(my_pos);
            auto bulletTargetPos = this->convertToNodeSpace(target_pos);
            bulletSprite1->inputBulletAction(bulletStartPos, bulletTargetPos);
            bulletSprite1->shoot();

        }
    }
    
}