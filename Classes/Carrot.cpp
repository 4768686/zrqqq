#include "Classes\Carrot.h"
#include "SimpleAudioEngine.h"
#include "GameMap1.h"
#include "GameMap2.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace CocosDenshion;

Vector<Carrot*> myCarrot;
extern int game_money1;

bool Carrot::initCarrot() 
{
	myCarrot.pushBack(this);

	if (!initWithFile("Carrot/CarrotNormal/Carrot_1.png")) 
		return false;

	this->setHP(10.0);
	bitenDamage = 1;

	scale = 1;

	myGrade = 1;
	_beBiten = false;

	this->createBloodBox();
	this->changeBloodBox();

	isAlive = true;

	 //�����ܲ���ҧ���״̬
	for (int i = 0; i < 10; i++)
		bitenTectures[i].fileName = "Carrot/CarrotBiten/hlb1_" + std::to_string(i + 1) + ".png";

	this->runTwistAnimation();
}

void Carrot::setLocation(Point mapDest) 
{
	CarrotPos = mapDest;
	CarrotPos.add(Vec2(0, 50));
	this->setPosition(CarrotPos);
}

void Carrot::setBitenAttack(float a) 
{
	bitenDamage = a;
	_beBiten = true;
}

void Carrot::runTwistAnimation() 
{
	SpriteFrameCache* cache = SpriteFrameCache::getInstance();

	// ����֡������
	for (int i = 10; i <= 21; i++) {
		std::string texturePath = "Carrot/CarrotTwist/hlb1_" + std::to_string(i) + ".png";
		auto frame = SpriteFrame::create(texturePath, Rect(0, 0, 150 * scale, 168 * scale));
		if (frame) 
			cache->addSpriteFrame(frame, "hlb1_" + std::to_string(i));
	}

	// ������������
	Animation* twistAnimation = Animation::create();
	twistAnimation->setDelayPerUnit(1.5f / 12);
	twistAnimation->setRestoreOriginalFrame(false);

	for (int i = 10; i <= 21; i++) {
		std::string frameName = "hlb1_" + std::to_string(i);
		auto frame = cache->getSpriteFrameByName(frameName);
		if (frame) 
			twistAnimation->addSpriteFrame(frame);
	}

	SimpleAudioEngine::getInstance()->playEffect("Music/carrot.mp3");
	this->runAction(Animate::create(twistAnimation));
}

void Carrot::update(float dt) 
{
	this->isDie();
	this->changeCarrotAppearance();//��ʾ�µ�hp��Ӧ���ܲ�
	this->beBiten();
	if (_hp == 10)
		this->beTouched();
	if (!this->isAlive)
		this->unschedule(schedule_selector(Carrot::update));
}

void Carrot::beBiten() 
{
	if (_beBiten == true) {
		_hp -= bitenDamage;
		_beBiten = false;
		bitenDamage = 1;
		this->changeBloodBox();			//����Ѫ������
		this->changeCarrotAppearance();	//��ʾ�µ�hp��Ӧ���ܲ�
	}
}

void Carrot::beTouched() {
	if (!this->isAlive)
		return;

	// ������¼������� 
	auto listener = EventListenerMouse::create();
	listener->onMouseDown = [=](EventMouse* event) {
		// ��ȡ�������λ��  
		Vec2 location = event->getLocationInView();

		if (this->getBoundingBox().containsPoint(location) && _hp == 10) {
			this->runTwistAnimation();
			this->upgrade();
		}
		else if (this->getBoundingBox().containsPoint(location) && _hp < 10 && game_money1 >= 180) {
			game_money1 -= 180;
			_hp += 1;
			this->runTwistAnimation();
			this->changeBloodBox();			//����Ѫ������
			this->changeCarrotAppearance();	//��ʾ�µ�hp��Ӧ���ܲ�
		}
		};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void Carrot::changeCarrotAppearance() {
	// �ڶ�����ɺ�ִ�е��߼�
	if(_hp>0)
		this->setTexture(bitenTectures[int(_hp - 1)].fileName);
}

void Carrot::createBloodBox() {
	CarrotBloodBox = Sprite::create("Carrot/CarrotBlood/carrot_hp_bg.png");
	if (!CarrotBloodBox) {
		CCLOG("Failed to load carrot image");
	}
	CarrotBloodBox->setScale(0.8f);
	CarrotBloodBox->setPosition(Vec2(85, 120));
	this->addChild(CarrotBloodBox, 20);       

	BloodNum = Sprite::create("Carrot/CarrotBlood/number_10.png"); 
	BloodNum->setScale(0.5f);
	BloodNum->setPosition(Vec2(72, 120));
	this->addChild(BloodNum, 10000);
}

void Carrot::changeBloodBox() {
	std::string hpFrameName = StringUtils::format("Carrot/CarrotBlood/number_%d.png", static_cast<int>(_hp));
	BloodNum->setTexture(hpFrameName);
}

bool Carrot::isDie() {
	if (_hp <= 0) {
		CarrotBloodBox->removeFromParent();
		BloodNum->removeFromParent();
		getActionManager()->removeAllActionsFromTarget(this);
		isAlive = false;

		//director ��ת������gameover
		GameMap1* sceneInstance = dynamic_cast<GameMap1*>(this->getParent());
		if (sceneInstance) {  
			sceneInstance->lose();
		}
		GameMap2* sceneInstance2 = dynamic_cast<GameMap2*>(this->getParent());
		if (sceneInstance2) {
			sceneInstance2->lose();
		}
		return true;
	}
	else return false;
}

bool Carrot::upgrade() {
	if (game_money1 >= 550) {
		myGrade += 1;
		if (myGrade > 3){
			myGrade = 3;
			if (_hp < 3){
				_hp = 10;
				this->changeBloodBox();
				this->changeCarrotAppearance();
			}
		}
		else {
			scale = myGrade / 20 + 1;
			this->setScale(scale);
			this->runTwistAnimation();
			game_money1 -= 125;
			for (int j = 0; j < monsterContainer.size(); j++) {
				auto monster = monsterContainer.at(j);
				monster->beHurt(monster->getHP() / (myGrade * 10 - 10), 4);
			}
		}
			return true;
		
	}
	return false;
}