#include "Classes/BulletClasses/Bullet.h"

bool Bullet::init() {
    if (!Node::init()) {
        return false;
    }
    // 初始化其他成员变量
    attackDamage = 1;
    attackRange = 0;
    bulletType = BulletType::Normal;
    // 初始化子弹精灵
    bulletSprite = Sprite::create();
    this->addChild(bulletSprite);
    return true;
}
void Bullet::attack(Node* target) {
    // 移除子弹
    this->removeFromParent();

    // 造成伤害
   // target->takeDamage(attackDamage);
}
void Bullet::upgradeAttackDamage(int increase) {
    attackDamage += increase;
}