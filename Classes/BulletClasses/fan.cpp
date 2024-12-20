#include "Classes\BulletClasses\fan.h"
#include "Classes\Monster\My_monster.h"

using namespace cocos2d::ui;

bool fanBullet::initfanBullet(int grade) {

    if (!Bullet::init()) return false;

    myGrade = grade;
    speed = (grade - 1) * 200 + 1000;
    attackDamage = 5 + (grade - 1) * 2;


    bulletSprite->initWithFile("Bullets/FanBullets/fanbullet" + StringUtils::toString(grade * 10 + 1) + ".png");

    //�ɵĹ��̵�����ı仯����
    Animation* blinkAnimation = Animation::create();
    blinkAnimation->setDelayPerUnit(0.2f / 4);
    blinkAnimation->setRestoreOriginalFrame(true);	//����ִ����󷵻ص�һ֡

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
    //��������
    const Vec2 direction = dst - src;

    //��������
    const float distance = src.distance(dst);

    //��������ʱ��
    const float durTime = distance / speed;

    // ����Ƕȣ����ȣ�
    const float angle = atan2(direction.y, direction.x);

    // ������ת��Ϊ�Ƕ�
    const float rotation = CC_RADIANS_TO_DEGREES(angle);

    // ���þ������ת�Ƕ�
    bulletSprite->setRotation(-rotation);
}

void fanBullet::inputBulletAction(Point towerLoc, Point monsterLoc)
{
    src = towerLoc, dst = monsterLoc;	//����������
    bulletSprite->setPosition(src);		//��ʼ��λ
    this->rotateSpriteToDirection();	//����Ƕ���������
}

void fanBullet::shoot()
{
   // �����ӵ�����ѡ��һ��ԶԶ������Ļ�ĵ���Ϊ�յ�
    auto winSize = Director::getInstance()->getWinSize();
    Vec2 direction = (dst - src).getNormalized();

    float longDistance = std::max(winSize.width, winSize.height) * 2;
    Vec2 endPos = src + direction * longDistance;

    // ���㶯������ʱ��
    float distance = src.distance(endPos);
    float durTime = distance * 5 / speed;

    auto moveTo = MoveTo::create(durTime, endPos);

    // �ڶ���ִ���ڼ䣬���ӵ�������ײ���
    this->schedule(CC_SCHEDULE_SELECTOR(fanBullet::checkCollision), 0.5f);

    runAction(Sequence::create(moveTo, CallFunc::create([=]() {
        // ����ִ����ϣ����ӵ��ɳ���Ļ���Ƴ��ӵ�
        this->removeFromParent();
    }), nullptr));
}

void fanBullet::checkCollision(float dt)
{
    //��ȡλ�ã�ת��Ϊ��������
    Vec2 childAbsolutePosition = bulletSprite->getParent()->convertToWorldSpace(bulletSprite->getPosition());

    //������ײ����
    auto bulletRect = Rect(
        childAbsolutePosition.x - this->getContentSize().width / 2,
        childAbsolutePosition.y - this->getContentSize().height / 2,
        this->bulletSprite->getContentSize().width,
        this->bulletSprite->getContentSize().height);

    //�����ϰ�
    for (int j = 0; j < BarrierContainer.size(); j++) {

        auto barrier = BarrierContainer.at(j);
        auto barrierRect = barrier->getBoundingBox();

        if (barrierRect.intersectsRect(bulletRect)){
            barrier->behurt(this->getAttackDamage(), 3);
        }
    }

    //���й���
    for (int j = 0; j < monsterContainer.size(); j++) {

        auto monster = monsterContainer.at(j);
        auto monsterRect = monster->getBoundingBox();

        if (monsterRect.intersectsRect(bulletRect)) {
            monster->beHurt(this->getAttackDamage(), 3);
        }
    }
}