#include"Barrier.h"
#include"GameMap1.h"
#include"GameMap2.h"
#include"GameMap3.h"

USING_NS_CC;
using namespace cocos2d::ui;

extern int game_money1;
cocos2d::Vector<Barrier*> BarrierContainer;

//��ʼ���ϰ�
void Barrier::initBarrier(int hp, int money, Texture2D* texture, Vec2 pos) {
	BarrierContainer.pushBack(this);
	initWithTexture(texture);

	hp_total = hp;
	_money = money;
	this->setPosition(pos);

	behit = Sprite::create();
	behit->setPosition(60, 10);			//���ÿ��λ��
	behit->setVisible(false);			// ����Ϊ�ɼ�  
	this->addChild(behit);				//�ӵ�Ĭ��ͼ������ȥ

	_hp = hp_total;
	is_alive = 1;

	this->create_Health_bar();			//����Ѫ��
	this->scheduleBlood();				//����Ѫ��
	this->schedule(schedule_selector(Barrier::update), 0.1f);
}

//�����ϰ�
void Barrier::update(float dt) {
	this->isDie();
	this->scheduleBlood();
	this->touched();
	if (!this->is_alive)
		this->unschedule(schedule_selector(Barrier::update));
}

void Barrier::create_Health_bar() {
	//����Ѫ��������
	bloodbox = Sprite::create("monster/3.png");					// �������ȿ�
	bloodbox->setScaleX(0.1f);									// ��������x���ϵĴ�С��С��ԭ����50%  
	bloodbox->setScaleY(0.1f);									// ��������y���ϵĴ�С��С��ԭ����50%
	bloodbox->setPosition(42, 90);								//���ÿ��λ��
	this->addChild(bloodbox);									//�ӵ�Ĭ��ͼ������ȥ
	bloodbox->setVisible(false);
	sprBlood = Sprite::create("monster/4.png");					// ����Ѫ��
	
	//����Ѫ��
	ProgressTimer* progress = ProgressTimer::create(sprBlood);	// ����progress����
	progress->setType(ProgressTimer::Type::BAR);				// ���ͣ���״
	progress->setScaleX(0.1f);									// ��������x���ϵĴ�С��С��ԭ����50%  
	progress->setScaleY(0.1f);									// ��������y���ϵĴ�С��С��ԭ����50%
	progress->setVisible(false);
	progress->setPosition(40, 90);

	//���ҵ������Ѫ��
	progress->setMidpoint(Point(0, 0.5));						
	progress->setBarChangeRate(Point(1, 0));
	progress->setTag(BLOOD_BAR);								//��һ�����
	this->addChild(progress);
}

void Barrier::scheduleBlood() {
	auto progress = (ProgressTimer*)this->getChildByTag(BLOOD_BAR);
	progress->setPercentage(((_hp) / hp_total) * 100);
}

void Barrier::clear() {
	for (int i = 0; i < 4; i++) {
		//���Ϊ�գ��˳�
		if (position[i].x == 0 && position[i].y == 0) {
			break;
		}
		else{
			map[position[i].x][position[i].y] = 0;
			map1[position[i].x][position[i].y] = 0;
			map2[position[i].x][position[i].y] = 0;
		}
	}
}

void Barrier::betouched() {
	if (this->choosed == 1 || !this->is_alive)
		return;
	
	choice = Sprite::create("monster/16.png");	// �������ȿ�
	this->addChild(choice, 1000);				// �ӵ�Ĭ��ͼ������ȥ
	choice->setPosition(50, 100);				// ���ÿ��λ��
	
	// �������������ü��
	Animation* animation = Animation::create();
	animation->setDelayPerUnit(0.2 / 2);		
	animation->setRestoreOriginalFrame(true);	// ����ִ�����ָ���һ֡
	
	// ���ؾ���֡
	Texture2D* texture1 = Director::getInstance()->getTextureCache()->addImage("monster/16.png");
	animation->addSpriteFrameWithTexture(texture1, Rect(0, 0, 34, 43));
	Texture2D* texture2 = Director::getInstance()->getTextureCache()->addImage("monster/17.png");
	animation->addSpriteFrameWithTexture(texture2, Rect(0, 0, 34, 43));
	
	// ���ж���
	choice->runAction(RepeatForever::create(Animate::create(animation)));
}

void Barrier::touched() {
	// ����������¼�������  
	auto listener = EventListenerMouse::create();
	listener->onMouseDown = [=](EventMouse* event) {
		// ��ȡ�������λ��  
		Vec2 location = event->getLocationInView();

		if (this->getBoundingBox().containsPoint(location)) {
			if (choose_a_object == 0) {
				if (this->choosed == 0)
					this->betouched();
				choose_a_object = 1;
				this->choosed = 1;
			}
			else {
				for (auto monster : BarrierContainer) {
					if (monster->choosed == 1) {
						monster->choosed = 0;
						if (monster->choice != nullptr)
							monster->choice->removeFromParent();
					}
				}
				for (auto monster : monsterContainer) {
					if (monster->choosed == 1) {
						monster->choosed = 0;
						if (monster->choice != nullptr)
							monster->choice->removeFromParent();
					}
				}
				this->betouched();
				this->choosed = 1;
				choose_a_object = 1;
			}
		}
		};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	if (!this->is_alive){
		_eventDispatcher->removeEventListener(listener);
		return;
	}
}

bool Barrier::isDie() {
	if (_hp <= 0){
		game_money1 += getMoney();
		this->clear();

		//����ѡ��״̬
		if (this->choosed == 1 && this->choice != nullptr){
			getActionManager()->removeAllActionsFromTarget(choice);
			this->choice->setVisible(false);
			choose_a_object = 0;
		}
		bloodbox->setVisible(false);
		getActionManager()->removeAllActionsFromTarget(this);
		is_alive = 0;

		// ���鱬ը
		auto texture = Director::getInstance()->getTextureCache()->addImage("monster/2.png");
		this->setTexture(texture);
		Animation* animation = Animation::create();
		animation->setDelayPerUnit(0.4 / 6);
		animation->setRestoreOriginalFrame(true);		// ����ִ����󷵻ص�һ֡
		
		// ���ؾ���֡
		Texture2D* texture1 = Director::getInstance()->getTextureCache()->addImage("monster/7.png");
		animation->addSpriteFrameWithTexture(texture1, Rect(0, 0, 100, 100));
		Texture2D* texture2 = Director::getInstance()->getTextureCache()->addImage("monster/8.png");
		animation->addSpriteFrameWithTexture(texture2, Rect(0, 0, 100, 100));
		Texture2D* texture3 = Director::getInstance()->getTextureCache()->addImage("monster/9.png");
		animation->addSpriteFrameWithTexture(texture3, Rect(0, 0, 100, 100));
		Texture2D* texture4 = Director::getInstance()->getTextureCache()->addImage("monster/10.png");
		animation->addSpriteFrameWithTexture(texture4, Rect(0, 0, 100, 100));
		Texture2D* texture5 = Director::getInstance()->getTextureCache()->addImage("monster/11.png");
		animation->addSpriteFrameWithTexture(texture5, Rect(0, 0, 100, 100));
		Texture2D* texture6 = Director::getInstance()->getTextureCache()->addImage("monster/12.png");
		animation->addSpriteFrameWithTexture(texture6, Rect(0, 0, 100, 100));
		
		// ���ж���
		this->runAction(RepeatForever::create(Animate::create(animation)));
		this->schedule(schedule_selector(Barrier::deletemonster), 0.4f);
		BarrierContainer.eraseObject(this, 0);
		die_monsternum++;
		return true;
	}
	else return false;
}

void Barrier::behurt(int monster_blood, int type) {
	_hp -= monster_blood;
	behit->setVisible(true); // ����Ϊ�ɼ�  

	if (this->bloodbox != nullptr)
		bloodbox->setVisible(true);

	auto progress = (ProgressTimer*)this->getChildByTag(BLOOD_BAR);
	progress->setVisible(true);

	auto texture = Director::getInstance()->getTextureCache()->addImage("monster/behit1.png");

	// �����µ�����  
	behit->setTexture(texture);

	// �������������ü��
	Animation* animation = Animation::create();
	animation->setDelayPerUnit(0.2 / 2);
	animation->setRestoreOriginalFrame(true);//����ִ����󷵻ص�һ֡

	// ���ؾ���֡
	Texture2D* texture1 = Director::getInstance()->getTextureCache()->addImage("monster/behit1.png");
	animation->addSpriteFrameWithTexture(texture1, Rect(0, 0, 100, 100));
	Texture2D* texture2 = Director::getInstance()->getTextureCache()->addImage("monster/7.png");
	animation->addSpriteFrameWithTexture(texture2, Rect(0, 0, 100, 100));

	//���ж���
	behit->runAction(RepeatForever::create(Animate::create(animation)));


	this->isDie();
	schedule(schedule_selector(Barrier::deletebehit), 0.4f);
}

void Barrier::deletebehit(float a) {
	behit->setVisible(false);
}

void Barrier::deletemonster(float a) {
	this->removeFromParent();
}