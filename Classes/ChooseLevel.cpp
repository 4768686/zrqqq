#include "ChooseLevel.h"
#include "MainScene.h"
#include "EnterScene1.h"
#include "EnterScene2.h"
#include "EnterScene3.h"
#include "EnterScene4.h"
#include "SimpleAudioEngine.h"
#include <vector>

USING_NS_CC;
using namespace CocosDenshion;
using namespace cocos2d::ui;
using namespace std;

extern int is_Pass[10] = { 0 };


//ö�٣����б����Ÿ���ڵ�Ĳ㼶
enum zOrderMap
{
	route = 1,//��·�㼶
	routeButtonEffect = 5,//��ť��Ч�㼶
	levelButton = 10//��ť�㼶
};

//����ѡ��ؿ�����
cocos2d::Scene* ChooseLevel::createScene()
{
	return ChooseLevel::create();
}

//ѡ��ؿ������ʼ��
//��ChooseLevel���е�init()�����е���
bool ChooseLevel::init()
{
	if (!Scene::init()) return false;

	InitUI();

	InitEvent();

	return true;
}

//UI���ʼ��
void ChooseLevel::InitUI()
{
	//������������
	auto scrollView = ScrollView::create();
	//���ù�������
	scrollView->setDirection(ScrollView::Direction::BOTH);
	//���������¼�
	scrollView->setTouchEnabled(true);
	//�������ݴ�С
	scrollView->setContentSize(Director::getInstance()->getVisibleSize());
	//���ع�����
	scrollView->setScrollBarEnabled(false);
	this->addChild(scrollView);

	//����������ͼ
	Sprite* imageView = NULL;
	string filePath;
	float nextPosX = 0.0f;
	float nextPosY = 0.0f;
	filePath = "ChooseLevel/chooselevel1.png";
	imageView = Sprite::create(filePath);
	imageView->setAnchorPoint(Vec2(0, 0));								//����ê��
	nextPosX += imageView->getContentSize().width;						//�ۼӺ���ƫ����
	nextPosY += imageView->getContentSize().height;						//�ۼ�����ƫ����
	scrollView->addChild(imageView);
	//����ScrollView�Ĳ���������С
	scrollView->setInnerContainerSize(Size(nextPosX, nextPosY));

	//UI��ť�ؼ�
	auto layerUI = Layer::create();
	this->addChild(layerUI);
	auto leftPanelPinfo = AutoPolygon::generatePolygon("ChooseLevel/stagemap_toolbar.png");
	auto leftPanel = Sprite::create(leftPanelPinfo);
	//���øñ���ͼʼ������Ļ�����Ͻ�
	leftPanel->setAnchorPoint(Vec2(0, 1));
	leftPanel->setPosition(0, Director::getInstance()->getVisibleSize().height);
	leftPanel->setScale(float(0.8));
	layerUI->addChild(leftPanel);

	//��������ͼ������ӽڵ㣨������ť��
	//��ť1
	auto toolbarHomeFilePath = "ChooseLevel/stagemap_toolbar_home.png";
	_toolbarHome = Button::create(toolbarHomeFilePath, toolbarHomeFilePath, "");
	_toolbarHome->setAnchorPoint(Vec2(0, 0));
	_toolbarHome->setPosition(Vec2(11, 10));
	_toolbarHome->setPressedActionEnabled(true);
	leftPanel->addChild(_toolbarHome);

	//��ť2
	auto toolbarShopFilePath = "ChooseLevel/stagemap_toolbar_shop.png";
	_toolbarShop = Button::create(toolbarShopFilePath, toolbarShopFilePath, "");
	_toolbarShop->setAnchorPoint(Vec2(0, 0));
	_toolbarShop->setPosition(Vec2(123, 10));
	_toolbarShop->setPressedActionEnabled(true);
	leftPanel->addChild(_toolbarShop);

	//��ť3
	auto toolbarLeaderboardFilePath = "ChooseLevel/stagemap_toolbar_leaderboard.png";
	_toolbarLeaderboard = Button::create(toolbarLeaderboardFilePath, toolbarLeaderboardFilePath, "");
	_toolbarLeaderboard->setAnchorPoint(Vec2(0, 0));
	_toolbarLeaderboard->setPosition(Vec2(235, 10));
	_toolbarLeaderboard->setPressedActionEnabled(true);
	leftPanel->addChild(_toolbarLeaderboard);

	TMXTiledMap* map = TMXTiledMap::create("ChooseLevel/tiledmap.tmx");
	//��ȡ��Ƭ��ͼ�е�point�����
	auto objectGroup = map->getObjectGroup("point");
	//��������е����ж����ȡ����
	auto objs = objectGroup->getObjects();
	//��������
	for (int i = 0; i < objs.size(); i++) {
		//ͼƬ����
		string texture = "ChooseLevel/stagepoint.png";

		//������Ƭ��ͼ��λ��������Ϣ���ùؿ���ť
		auto button = Button::create(texture, texture, "");
		scrollView->addChild(button, zOrderMap::levelButton, i);
		std::vector<cocos2d::ui::Button*> routeButtonArray;
		routeButtonArray.push_back(button);

		auto valueMap = objs[i].asValueMap();//�������ֵ��Valueת����ValueMap

		//��ť��λ������
		button->setPosition(Vec2(valueMap["x"].asFloat() - 5, valueMap["y"].asFloat() + 60));//��ȡ���������
		//���õ�ǰ�ؿ�����������0��ʼ����0��ʾ��1�أ�1��ʾ��2�أ��Դ����ơ�
		button->setTag(i);						//���ñ��
		button->setScale(1.0);					//���ô�С
		button->setPressedActionEnabled(true);	//���ð���Ч��

		switch (i) {
		case 0:
			//�����¼�������
			button->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
				if (type == ui::Widget::TouchEventType::ENDED) {
					SimpleAudioEngine::getInstance()->playEffect("Music/Select.mp3", false, 1.0f, 1.0f, 1.0f);
					//��ת�������ؿ�1
					SimpleAudioEngine::getInstance()->stopBackgroundMusic();
					auto enterscene = EnterScene1::createScene();
					Director::getInstance()->replaceScene(enterscene);
				}
				});
			break;
		case 1:
			//�����¼�������
			button->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
				if (type == ui::Widget::TouchEventType::ENDED) {
					SimpleAudioEngine::getInstance()->playEffect("Music/Select.mp3", false, 1.0f, 1.0f, 1.0f);
					//��ת�������ؿ�2
					if (is_Pass[1] == 0) {
						// �������ֲ�
						auto maskLayer = LayerColor::create(Color4B(0, 0, 0, 128)); // ǳ��ɫ����
						this->addChild(maskLayer, 200);

						// ��Ӵ��������¼�����ֹ��͸
						auto listener = EventListenerTouchOneByOne::create();
						listener->setSwallowTouches(true);
						listener->onTouchBegan = [](Touch* touch, Event* event) {
							return true; // �������ɴ����¼�
							};
						_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, maskLayer);

						auto lockmenu = Sprite::create("ChooseLevel/lock_image.png");

						SimpleAudioEngine::getInstance()->pauseBackgroundMusic();

						// ���ò˵�λ��
						lockmenu->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
							Director::getInstance()->getVisibleSize().height * 2));
						this->addChild(lockmenu, 201);

						// ִ�в˵����Ϸ��ƶ�������Ķ���
						auto moveAction = EaseBackOut::create(MoveTo::create(0.5f, Vec2(Director::getInstance()->getVisibleSize().width / 2,
							Director::getInstance()->getVisibleSize().height / 2)));
						lockmenu->runAction(moveAction);

						auto lock_btn = Button::create("ChooseLevel/lock_btn_normal.png", "ChooseLevel/lock_btn_selected.png", "");
						lockmenu->addChild(lock_btn);
						lock_btn->setPressedActionEnabled(true);
						lock_btn->setPosition(Vec2(333, 70));
						lock_btn->setScale(1.12);
						lock_btn->addClickEventListener([=](Ref* sender) {
							SimpleAudioEngine::getInstance()->playEffect("Music/Select.mp3");
							Director::getInstance()->resume();
							lockmenu->removeFromParent();
							maskLayer->removeFromParent(); // �Ƴ�����
							});
					}
					else {
						SimpleAudioEngine::getInstance()->stopBackgroundMusic();
						auto enterscene = EnterScene2::createScene();
						Director::getInstance()->replaceScene(enterscene);
					}
				}
			});
			break;
		case 2:
			//�����¼�������
			button->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
				if (type == ui::Widget::TouchEventType::ENDED) {
					SimpleAudioEngine::getInstance()->playEffect("Music/Select.mp3", false, 1.0f, 1.0f, 1.0f);
					//��ת�������ؿ�3
					if (is_Pass[2] == 0) {
						// �������ֲ�
						auto maskLayer = LayerColor::create(Color4B(0, 0, 0, 128)); // ǳ��ɫ����
						this->addChild(maskLayer, 200);

						// ��Ӵ��������¼�����ֹ��͸
						auto listener = EventListenerTouchOneByOne::create();
						listener->setSwallowTouches(true);
						listener->onTouchBegan = [](Touch* touch, Event* event) {
							return true; // �������ɴ����¼�
							};
						_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, maskLayer);

						auto lockmenu = Sprite::create("ChooseLevel/lock_image.png");

						SimpleAudioEngine::getInstance()->pauseBackgroundMusic();

						// ���ò˵�λ��
						lockmenu->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
							Director::getInstance()->getVisibleSize().height * 2));
						this->addChild(lockmenu, 201);

						// ִ�в˵����Ϸ��ƶ�������Ķ���
						auto moveAction = EaseBackOut::create(MoveTo::create(0.5f, Vec2(Director::getInstance()->getVisibleSize().width / 2,
							Director::getInstance()->getVisibleSize().height / 2)));
						lockmenu->runAction(moveAction);

						auto lock_btn = Button::create("ChooseLevel/lock_btn_normal.png", "ChooseLevel/lock_btn_selected.png", "");
						lockmenu->addChild(lock_btn);
						lock_btn->setPressedActionEnabled(true);
						lock_btn->setPosition(Vec2(333, 70));
						lock_btn->setScale(1.12);
						lock_btn->addClickEventListener([=](Ref* sender) {
							SimpleAudioEngine::getInstance()->playEffect("Music/Select.mp3");
							Director::getInstance()->resume();
							lockmenu->removeFromParent();
							maskLayer->removeFromParent(); // �Ƴ�����
							});
					}
					else {
						SimpleAudioEngine::getInstance()->stopBackgroundMusic();
						auto enterscene = EnterScene3::createScene();
						Director::getInstance()->replaceScene(enterscene);
					}
				}
				});
			break;
		case 3:
			//�����¼�������
			button->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
				if (type == ui::Widget::TouchEventType::ENDED) {
					SimpleAudioEngine::getInstance()->playEffect("Music/Select.mp3", false, 1.0f, 1.0f, 1.0f);
					//��ת�������ؿ�4
					if (is_Pass[2] == 0) {
						// �������ֲ�
						auto maskLayer = LayerColor::create(Color4B(0, 0, 0, 128)); // ǳ��ɫ����
						this->addChild(maskLayer, 200);

						// ��Ӵ��������¼�����ֹ��͸
						auto listener = EventListenerTouchOneByOne::create();
						listener->setSwallowTouches(true);
						listener->onTouchBegan = [](Touch* touch, Event* event) {
							return true; // �������ɴ����¼�
							};
						_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, maskLayer);

						auto lockmenu = Sprite::create("ChooseLevel/lock_image.png");

						SimpleAudioEngine::getInstance()->pauseBackgroundMusic();

						// ���ò˵�λ��
						lockmenu->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
							Director::getInstance()->getVisibleSize().height * 2));
						this->addChild(lockmenu, 201);

						// ִ�в˵����Ϸ��ƶ�������Ķ���
						auto moveAction = EaseBackOut::create(MoveTo::create(0.5f, Vec2(Director::getInstance()->getVisibleSize().width / 2,
							Director::getInstance()->getVisibleSize().height / 2)));
						lockmenu->runAction(moveAction);

						auto lock_btn = Button::create("ChooseLevel/lock_btn_normal.png", "ChooseLevel/lock_btn_selected.png", "");
						lockmenu->addChild(lock_btn);
						lock_btn->setPressedActionEnabled(true);
						lock_btn->setPosition(Vec2(333, 70));
						lock_btn->setScale(1.12);
						lock_btn->addClickEventListener([=](Ref* sender) {
							SimpleAudioEngine::getInstance()->playEffect("Music/Select.mp3");
							Director::getInstance()->resume();
							lockmenu->removeFromParent();
							maskLayer->removeFromParent(); // �Ƴ�����
							});
					}
					else {
						SimpleAudioEngine::getInstance()->stopBackgroundMusic();
						auto enterscene = EnterScene4::createScene();
						Director::getInstance()->replaceScene(enterscene);
					}
				}
				});
			break;
		//������ӹؿ�
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:

		default:
			break;
		}
	}
}

//�ײ��¼����ʼ��
void ChooseLevel::InitEvent()
{
	//��ʼ����ҳ��ť���̵갴ť�����а�ť�������¼�
	//��ҳ��ť
	_toolbarHome->addTouchEventListener([](Ref* sender, Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED){
			SimpleAudioEngine::getInstance()->playEffect("Music/Select.mp3", false, 1.0f, 1.0f, 1.0f);
			auto mainScene = MainScene::createScene();
			Director::getInstance()->replaceScene(mainScene);
		}
	});
	//�̵갴ť
	_toolbarShop->addTouchEventListener([](Ref* sender, Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED) 
			SimpleAudioEngine::getInstance()->playEffect("Music/Select.mp3", false, 1.0f, 1.0f, 1.0f);
		//...��ת
		});
	//���а�ť
	_toolbarLeaderboard->addTouchEventListener([](Ref* sender, Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
			SimpleAudioEngine::getInstance()->playEffect("Music/Select.mp3", false, 1.0f, 1.0f, 1.0f);
		//...��ת
		});
}
