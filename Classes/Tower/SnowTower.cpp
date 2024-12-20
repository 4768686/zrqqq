#include "SnowTower.h"
#include"Classes\Monster\My_monster.h"
#include"Classes\BulletClasses\snow.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

SnowTower::SnowTower()
{

}

SnowTower* SnowTower::create(const std::string& towerImage)
{
    SnowTower* tower = new SnowTower();
    if (tower && tower->init(towerImage)) {
        tower->autorelease();
        return tower;
    }
    else {
        delete tower;
        return nullptr;
    }
}

bool SnowTower::init(const std::string& towerImage)
{
    if (!Tower::init(towerImage))
        return false;
    attackInterval = 1.0f;

    schedule([=](float dt) {
        this->update(dt);
        }, "update_key");
    return true;
}

void SnowTower::performAttack()
{
    my_pos = getPosition();//获得我的位置
    setAnchorPoint(Vec2(0.5, 0.5));
}

void SnowTower::update(float d)
{
    // 更新攻击计时器
    if (isTargetInRange()) {
        attackTimer += d;
        performAttack();
        if (attackTimer >= attackInterval) {
            attackTimer = 0.0f;  // 重置计时器
            SimpleAudioEngine::getInstance()->playEffect("Music/TowerMusic/Sun.mp3");
            auto bulletSprite1 = SnowFlowerBullet::create();
            this->addChild(bulletSprite1, -1);
            bulletSprite1->initSnowBullet(level);
            int x = bulletSprite1->getContentSize().width;
            int y = bulletSprite1->getContentSize().height;

            bulletSprite1->inputBulletAction(Vec2(45, 50));
            bulletSprite1->spread();

        }
    }


}
