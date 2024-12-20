#include "Classes\BulletClasses\fan.h"
#include "Classes\Monster\My_monster.h"

using namespace cocos2d::ui;

bool fanBullet::initfanBullet(int grade) {

    if (!Bullet::init()) return false;

    myGrade = grade;
    speed = (grade - 1) * 200 + 1000;
    attackDamage = 5 + (grade - 1) * 2;


    bulletSprite->initWithFile("Bullets/FanBullets/fanbullet" + StringUtils::toString(grade * 10 + 1) + ".png");

    //飞的过程的纹理的变化设置
    Animation* blinkAnimation = Animation::create();
    blinkAnimation->setDelayPerUnit(0.2f / 4);
    blinkAnimation->setRestoreOriginalFrame(true);	//动画执行完后返回第一帧

    Texture2D* texture1 = Director::getInstance()->getTextureCache()->addImage("Bullets/FanBullets/fanbullet" + StringUtils::toString(myGrade * 10 + 1) + ".png");
    blinkAnimation->addSpriteFrameWithTexture(texture1, Rect(0, 0, 100, 100));
    Texture2D* texture2 = Director::getInstance()->getTextureCache()->addImage("Bullets/FanBullets/fanbullet" + StringUtils::toString(myGrade * 10 + 2) + ".png");
    blinkAnimation->addSpriteFrameWithTexture(texture2, Rect(0, 0, 100, 100));
    Texture2D* texture3 = Director::getInstance()->getTextureCache()->addImage("Bullets/FanBullets/fanbullet" + StringUtils::toString(myGrade * 10 + 3) + ".png");
    blinkAnimation->addSpriteFrameWithTexture(texture3, Rect(0, 0, 100, 100));
    Texture2D* texture4 = Director::getInstance()->getTextureCache()->addImage("Bullets/FanBullets/fanbullet" + StringUtils::toString(myGrade * 10 + 4) + ".png");
    blinkAnimation->addSpriteFrameWithTexture(texture4, Rect(0, 0, 100, 100));

    bulletSprite->runAction(RepeatForever::create(Animate::create(blinkAnimation)));
    return true;
}

void fanBullet::rotateSpriteToDirection() {
    //方向向量
    const Vec2 direction = dst - src;

    //两点间距离
    const float distance = src.distance(dst);

    //动作持续时间
    const float durTime = distance / speed;

    // 计算角度（弧度）
    const float angle = atan2(direction.y, direction.x);

    // 将弧度转换为角度
    const float rotation = CC_RADIANS_TO_DEGREES(angle);

    // 设置精灵的旋转角度
    bulletSprite->setRotation(-rotation);
}

void fanBullet::inputBulletAction(Point towerLoc, Point monsterLoc)
{
    src = towerLoc, dst = monsterLoc;	//设置两个点
    bulletSprite->setPosition(src);		//初始定位
    this->rotateSpriteToDirection();	//计算角度制作动画
}

void fanBullet::shoot()
{
   // 根据子弹方向选择一个远远超出屏幕的点作为终点
    auto winSize = Director::getInstance()->getWinSize();
    Vec2 direction = (dst - src).getNormalized();

    float longDistance = std::max(winSize.width, winSize.height) * 2;
    Vec2 endPos = src + direction * longDistance;

    // 计算动作持续时间
    float distance = src.distance(endPos);
    float durTime = distance * 5 / speed;

    auto moveTo = MoveTo::create(durTime, endPos);

    // 在动作执行期间，对子弹进行碰撞检测
    this->schedule(CC_SCHEDULE_SELECTOR(fanBullet::checkCollision), 0.5f);

    runAction(Sequence::create(moveTo, CallFunc::create([=]() {
        // 动作执行完毕，即子弹飞出屏幕后，移除子弹
        this->removeFromParent();
    }), nullptr));
}

void fanBullet::checkCollision(float dt)
{
    //获取位置，转化为世界坐标
    Vec2 childAbsolutePosition = bulletSprite->getParent()->convertToWorldSpace(bulletSprite->getPosition());

    //构建碰撞矩形
    auto bulletRect = Rect(
        childAbsolutePosition.x - this->getContentSize().width / 2,
        childAbsolutePosition.y - this->getContentSize().height / 2,
        this->bulletSprite->getContentSize().width,
        this->bulletSprite->getContentSize().height);

    //击中障碍
    for (int j = 0; j < BarrierContainer.size(); j++) {

        auto barrier = BarrierContainer.at(j);
        auto barrierRect = barrier->getBoundingBox();

        if (barrierRect.intersectsRect(bulletRect)){
            barrier->behurt(this->getAttackDamage(), 3);
        }
    }

    //击中怪物
    for (int j = 0; j < monsterContainer.size(); j++) {

        auto monster = monsterContainer.at(j);
        auto monsterRect = monster->getBoundingBox();

        if (monsterRect.intersectsRect(bulletRect)) {
            monster->beHurt(this->getAttackDamage(), 3);
        }
    }
}