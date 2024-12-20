#include"ChooseLevel.h"
#include"MainScene.h"
#include"EnterScene1.h"
#include"GameMap1.h"
#include"EnterScene2.h"
#include"GameMap2.h"
#include <vector>
#include <cstring>
#include"Classes\Monster\My_monster.h"
#include"Classes\Tower\Tower.h"
#include"Classes\Tower\BottleTower.h"
#include"Classes\Tower\ShitTower.h"
#include"Classes\Tower\FanTower.h"
#include"Classes\Barrier\Barrier.h"
#include"SimpleAudioEngine.h"

#define x_min 90
#define x_max 1050
#define i_max 6			//6行
#define j_max 13		//13列

USING_NS_CC;
using namespace cocos2d::ui;
using namespace CocosDenshion;

extern int is_Pass[10];

int monsternum;
int die_monsternum;
int game_money1;//金钱

int map[i_max][j_max] = {
/*   0 1 2 3 4 5 6 7 8 9 0 1 2   */
/*0*/0,0,3,0,3,3,0,0,0,0,0,0,0,
/*1*/0,1,0,0,3,3,3,3,0,0,0,1,0,
/*2*/0,1,3,0,3,3,3,3,0,3,3,1,0,
/*3*/0,1,0,0,1,1,1,1,1,3,0,1,0,
/*4*/0,1,1,1,1,0,3,3,1,1,1,1,0,
/*5*/0,0,0,0,3,0,3,0,0,0,3,0,0
};//1路线，3障碍，0空地

cocos2d::Scene* GameMap1::createScene()
{
	return GameMap1::create();
}

static void problemLoading(const char* filename)
{
	printf("无法加载:%s\n", filename);
}

void GameMap1::resetMap()
{
	removeAllChildrenWithCleanup(true);

	for (int i = 0; i < i_max; i++) {
		for (int j = 0; j < j_max; j++) {
			if (map[i][j] == PLACED)
				map[i][j] = EMPTY;
		}
	}

	// 清空塔的容器并从父节点移除
	for (auto tower : bottletowers) {
		tower->removeFromParent();
	}
	bottletowers.clear();

	for (auto tower : shittowers) {
		tower->removeFromParent();
	}
	shittowers.clear();

	for (auto tower : fantowers) {
		tower->removeFromParent();
	}
	fantowers.clear();

	for (auto monster : monsterContainer) {
		monster->removeFromParent();
	}
	monsterContainer.clear();

	this->init();
}

bool GameMap1::init()
{
	if (!Scene::init()) return false;

	isGamePaused = false;	// 开始时为未暂停状态

	game_money1 = 500;		// 初始金钱为500

	// 设置并播放音乐
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("Music/BackGroundMusic/BGMusic01.mp3");
	SimpleAudioEngine::getInstance()->playBackgroundMusic("Music/BackGroundMusic/BGMusic01.mp3", true);

	// 设置背景地图
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// 添加背景
	auto background = Sprite::create("GameMap/map1.png");
	background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

	// 计算缩放比例，以适应屏幕
	float scaleX = visibleSize.width / background->getContentSize().width;
	float scaleY = visibleSize.height / background->getContentSize().height;

	// 设置精灵的缩放
	background->setScale(scaleX, scaleY);
	this->addChild(background);

	// 怪物出生点
	auto flag = Sprite::create("GameMap/flag.png");
	flag->setPosition(145, 505);
	this->addChild(flag, 10);

	// UI按钮控件栏
	auto layerUI = Layer::create();
	this->addChild(layerUI,200);
	auto upPanelPinfo = AutoPolygon::generatePolygon("GameMap/top.png");
	auto upPanel = Sprite::create(upPanelPinfo);
	upPanel->setAnchorPoint(Vec2(0, 1));
	upPanel->setPosition(0.0f, (Director::getInstance()->getVisibleSize().height + 330));
	upPanel->setScale(1.05);
	layerUI->addChild(upPanel);

	// 显示金币
	// 个位
	auto gewei = Sprite::create("GameMap/num.png");
	gewei->setTextureRect(Rect(0, 0, 27, 28));
	gewei->setPosition(200, 603);
	layerUI->addChild(gewei);
	// 十位
	shiwei = Sprite::create("GameMap/num.png");
	shiwei->setTextureRect(Rect(0, 0, 27, 28));
	shiwei->setPosition(175, 603);
	layerUI->addChild(shiwei);
	// 百位
	baiwei = Sprite::create("GameMap/num.png");
	baiwei->setTextureRect(Rect(0, 0, 30, 28));
	baiwei->setPosition(150, 603);
	layerUI->addChild(baiwei);
	// 千位
	qianwei = Sprite::create("GameMap/num.png");
	qianwei->setTextureRect(Rect(0, 0, 30, 28));
	qianwei->setPosition(125, 603);
	layerUI->addChild(qianwei);

	// 每0.01s更新一次
	this->schedule(schedule_selector(GameMap1::updatemoney), 0.01f);

	// 设置暂停键和菜单键
	setPauseButton(layerUI);
	setMenuButton(layerUI);

	// 倒计时
	countDown();

	// 放置萝卜
	auto CarrotSprite = Carrot::create();
	CarrotSprite->setLocation(Vec2(1000, 460));
	this->addChild(CarrotSprite, 10);
	CarrotSprite->initCarrot();
	CarrotSprite->schedule(schedule_selector(Carrot::update), 0.5f);
	
	// 放置障碍物
	InitBarrier();

	// 设置点击事件监听
	auto listener = EventListenerMouse::create();
	listener->onMouseDown = CC_CALLBACK_1(GameMap1::onMouseDown, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	
	current_wave = 1;
	monsternum = 0;
	die_monsternum = 0;
	this->scheduleOnce(schedule_selector(GameMap1::bo), 5);

	auto wave_background = Sprite::create("GameMap/waves_bg.png");
	wave_background->setPosition(570, 592);
	layerUI->addChild(wave_background);

	auto wave_05 = Sprite::create("GameMap/wave05.png");
	wave_05->setPosition(570, 600);
	layerUI->addChild(wave_05);
	// 设置精灵的缩放
	wave_05->setScale(0.78, 0.78);

	wave = Sprite::create("GameMap/wave_0.png");
	wave->setPosition(505, 600);
	layerUI->addChild(wave);

	this->schedule(schedule_selector(Monster::update), 0.05f);

	return true;
}

void GameMap1::updatemoney(float a) {
	int temp;

	if (game_money1 > 999) {
		qianwei->setVisible(true);
		baiwei->setVisible(true);
		shiwei->setVisible(true);

		temp = game_money1 / 10;
		int j = temp % 10;
		shiwei->setTextureRect(Rect(30 * j, 0, 30, 28));

		temp = temp / 10;
		j = temp % 10;
		baiwei->setTextureRect(Rect(30 * j, 0, 30, 28));

		temp = temp / 10;
		j = temp % 10;
		qianwei->setTextureRect(Rect(30 * j, 0, 30, 28));

		return;
	}
	qianwei->setVisible(false);
	if (game_money1 > 99) {
		baiwei->setVisible(true);
		shiwei->setVisible(true);

		temp = game_money1 / 10;
		int j = temp % 10;
		shiwei->setTextureRect(Rect(30 * j, 0, 30, 28));

		temp = temp / 10;
		j = temp % 10;
		baiwei->setTextureRect(Rect(30 * j, 0, 30, 28));
		return;
	}
	baiwei->setVisible(false);
	if (game_money1 > 9) {
		shiwei->setVisible(true);

		temp = game_money1 / 10;
		shiwei->setTextureRect(Rect(temp * 30, 0, 30, 28));

		return;
	}
	shiwei->setVisible(false);
}

void GameMap1::InitBarrier()
{
	//加载纹理
	Texture2D* texture50 = Director::getInstance()->getTextureCache()->addImage("Barrier/barrier50.png");
	Texture2D* texture51 = Director::getInstance()->getTextureCache()->addImage("Barrier/barrier51.png");
	Texture2D* texture52 = Director::getInstance()->getTextureCache()->addImage("Barrier/barrier52.png");
	Texture2D* texture53 = Director::getInstance()->getTextureCache()->addImage("Barrier/barrier53.png");
	Texture2D* texture54 = Director::getInstance()->getTextureCache()->addImage("Barrier/barrier54.png");

	auto BarrierSprite1 = Barrier::create();
	this->addChild(BarrierSprite1, 5);
	BarrierSprite1->initBarrier(300, 300, texture50, Vec2(460, 485));
	BarrierSprite1->position[0] = { 0,4 };
	BarrierSprite1->position[1] = { 0,5 };
	BarrierSprite1->position[2] = { 1,4 };
	BarrierSprite1->position[3] = { 1,5 };

	auto BarrierSprite2 = Barrier::create();
	this->addChild(BarrierSprite2, 0);
	BarrierSprite2->initBarrier(2500, 2500, texture51, Vec2(610, 400));
	BarrierSprite2->position[0] = { 1,6 };
	BarrierSprite2->position[1] = { 1,7 };
	BarrierSprite2->position[2] = { 2,6 };
	BarrierSprite2->position[3] = { 2,7 };

	auto BarrierSprite3 = Barrier::create();
	this->addChild(BarrierSprite3, 0);
	BarrierSprite3->initBarrier(150, 150, texture52, Vec2(600, 205));
	BarrierSprite3->position[0] = { 4,6 };
	BarrierSprite3->position[1] = { 4,7 };

	auto BarrierSprite4 = Barrier::create();
	this->addChild(BarrierSprite4, 0);
	BarrierSprite4->initBarrier(150, 150, texture52, Vec2(890, 365));
	BarrierSprite4->position[0] = { 2,8 };
	BarrierSprite4->position[1] = { 2,9 };

	auto BarrierSprite5 = Barrier::create();
	this->addChild(BarrierSprite5, 0);
	BarrierSprite5->initBarrier(70, 70, texture53, Vec2(420, 125));
	BarrierSprite5->position[0] = { 5,4 };

	auto BarrierSprite6 = Barrier::create();
	this->addChild(BarrierSprite6, 0);
	BarrierSprite6->initBarrier(70, 70, texture54, Vec2(250, 365));
	BarrierSprite6->position[0] = { 2,2 };

	auto BarrierSprite7 = Barrier::create();
	this->addChild(BarrierSprite7, 0);
	BarrierSprite7->initBarrier(70, 70, texture53, Vec2(250, 525));
	BarrierSprite7->position[0] = { 0,2 };

	auto BarrierSprite8 = Barrier::create();
	this->addChild(BarrierSprite8, 0);
	BarrierSprite8->initBarrier(70, 70, texture53, Vec2(420, 365));
	BarrierSprite8->position[0] = { 2,4 };

	auto BarrierSprite9 = Barrier::create();
	this->addChild(BarrierSprite9, 0);
	BarrierSprite9->initBarrier(70, 70, texture53, Vec2(500, 365));
	BarrierSprite9->position[0] = { 2,5 };

	auto BarrierSprite10 = Barrier::create();
	this->addChild(BarrierSprite10, 0);
	BarrierSprite10->initBarrier(70, 70, texture53, Vec2(580, 125));
	BarrierSprite10->position[0] = { 3,6 };

	auto BarrierSprite11 = Barrier::create();
	this->addChild(BarrierSprite11, 0);
	BarrierSprite11->initBarrier(70, 70, texture54, Vec2(885, 125));
	BarrierSprite11->position[0] = { 5,10 };

	auto BarrierSprite12 = Barrier::create();
	this->addChild(BarrierSprite12, 0);
	BarrierSprite12->initBarrier(70, 70, texture54, Vec2(800, 270));
	BarrierSprite12->position[0] = { 3,9 };
}

// 菜单按钮
void GameMap1::setMenuButton(Layer* layerUI)
{
	menubtn = Button::create("GameMap/menu.png", "GameMap/menu.png");
	menubtn->setPressedActionEnabled(true);
	menubtn->setPosition(Vec2(975, 590));
	menubtn->setScale(1.05);
	layerUI->addChild(menubtn);

	// 设置按钮点击事件监听器
	menubtn->addClickEventListener([=](Ref* sender) {
		SimpleAudioEngine::getInstance()->playEffect("Music/Select.mp3");
		
		// 创建遮罩层
		auto maskLayer = LayerColor::create(Color4B(0, 0, 0, 128)); // 浅黑色遮罩
		this->addChild(maskLayer, 200);

		// 添加触摸吞噬事件，防止穿透
		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = [](Touch* touch, Event* event) {
			return true; // 总是吞噬触摸事件
			};
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, maskLayer);

		// 创建菜单背景
		auto gamemenu = Sprite::create("GameMap/gamemenu.png");
		
		SimpleAudioEngine::getInstance()->pauseBackgroundMusic();

		// 设置菜单位置
		gamemenu->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, 
			Director::getInstance()->getVisibleSize().height * 2));
		this->addChild(gamemenu, 201);

		// 执行菜单从上方移动到中央的动作
		auto moveAction = EaseBackOut::create(MoveTo::create(0.5f, Vec2(Director::getInstance()->getVisibleSize().width / 2, 
			Director::getInstance()->getVisibleSize().height / 2)));
		gamemenu->runAction(moveAction);

		// 使用Sequence连接动作和回调函数
		gamemenu->runAction(Sequence::create(moveAction, CallFunc::create([this]() {
			// 在动作执行完后执行的回调函数
			// 游戏暂停
			isGamePaused = true;
			Director::getInstance()->pause();
			}), nullptr));

		//继续游戏按钮
		auto continuebtn = Button::create("GameMap/continue.png", "GameMap/continue.png", "");
		gamemenu->addChild(continuebtn);
		continuebtn->setPressedActionEnabled(true);
		continuebtn->setPosition(Vec2(233, 310));
		continuebtn->setScale(1.12);
		continuebtn->addClickEventListener([=](Ref* sender) {
			SimpleAudioEngine::getInstance()->playEffect("Music/Select.mp3");
			isGamePaused = false;
			SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
			Director::getInstance()->resume();
			gamemenu->removeFromParent();
			maskLayer->removeFromParent(); // 移除遮罩
			});

		//选择关卡按钮
		auto chooselevelbtn = Button::create("GameMap/chooselevel.png", "GameMap/chooselevel.png", "");
		gamemenu->addChild(chooselevelbtn);
		chooselevelbtn->setPressedActionEnabled(true);
		chooselevelbtn->setPosition(Vec2(233, 202));
		chooselevelbtn->setScale(1.12);
		chooselevelbtn->addClickEventListener([=](Ref* sender){
			resetMap();
			removeAllChildrenWithCleanup(true);
			SimpleAudioEngine::getInstance()->playEffect("Music/Select.mp3", false, 1.0f, 1.0f, 1.0f);
			SimpleAudioEngine::getInstance()->stopBackgroundMusic();
			SimpleAudioEngine::getInstance()->preloadBackgroundMusic("Music/BackGroundMusic/StartBGMusic.mp3");
			SimpleAudioEngine::getInstance()->playBackgroundMusic("Music/BackGroundMusic/StartBGMusic.mp3", true);
			Director::getInstance()->resume();
			auto chooselevel = ChooseLevel::create();
			Director::getInstance()->replaceScene(chooselevel);
			});

		//重新开始按钮
		auto restartbtn = Button::create("GameMap/restart.png", "GameMap/restart.png", "");
		gamemenu->addChild(restartbtn);
		restartbtn->setPressedActionEnabled(true);
		restartbtn->setPosition(Vec2(233, 100));
		restartbtn->setScale(1.12);
		restartbtn->addClickEventListener([=](Ref* sender) {
			resetMap();
			removeAllChildrenWithCleanup(true);
			SimpleAudioEngine::getInstance()->playEffect("Music/Select.mp3", false, 1.0f, 1.0f, 1.0f);
			SimpleAudioEngine::getInstance()->stopAllEffects();
			SimpleAudioEngine::getInstance()->stopBackgroundMusic();
			Director::getInstance()->resume();

			auto gamemap = GameMap1::create();
			Director::getInstance()->replaceScene(gamemap);
			});

		});
}

void GameMap1::setPauseButton(Layer* layerUI)
{
	pausebtn = Button::create("GameMap/pause_0.png", "GameMap/pause_0.png");
	pausebtn->setPressedActionEnabled(true);
	pausebtn->setPosition(Vec2(900, 590));
	pausebtn->setScale(1.05);
	layerUI->addChild(pausebtn);

	// 设置按钮点击事件监听器
	pausebtn->addClickEventListener([=](Ref* sender) {
		SimpleAudioEngine::getInstance()->playEffect("Music/Select.mp3");
		
		// 添加透明遮罩
		auto maskLayer = LayerColor::create(Color4B(0, 0, 0, 0)); // 透明遮罩
		this->addChild(maskLayer, 199);

		// 添加触摸吞噬事件，防止穿透
		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = [](Touch* touch, Event* event) {
			return true;
			};
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, maskLayer);

		if (isGamePaused) {
			// 游戏继续
			maskLayer->removeFromParent();
			Director::getInstance()->resume();
			isGamePaused = false;
			pausebtn->loadTextures("GameMap/pause_0.png", "GameMap/pause_0.png");
			SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		}
		else {
			// 游戏暂停
			Director::getInstance()->pause();
			isGamePaused = true;
			pausebtn->loadTextures("GameMap/pause_1.png", "GameMap/pause_1.png");
			SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		}

		});
}

bool GameMap1::is_out_of_range(int x, int y)
{
	if (x<x_min || x>x_max || y<y_min || y>y_max)
		return true;
	else
		return false;
}

//坐标改为网格
void GameMap1::transform_xy_to_ij(int& i, int& j, const int x, const int y)
{
	j = (x - 40) / 80;
	i = 6 - (y - 10) / 80;
}

//网格改为坐标
void GameMap1::transform_ij_to_xy(const int i, const int j, int& x, int& y)
{
	x = 90 + 80 * j;
	y = 40 + 80 * (6 - i);
}

int GameMap1::getStatus(int x, int y)
{
	int i, j;
	transform_xy_to_ij(i, j, x, y);
	
	//如果在地图边缘，则不能放置
	if (is_out_of_range(x,y))
		return -1;
	//如果在路径上，则不能被放置
	if (map[i][j] == PATH)
		return -1;
	//如果位置为空，则可以放置
	if (map[i][j] == EMPTY)
		return EMPTY;
	//如果位置有炮塔，则可以升级
	if (map[i][j] == PLACED)
		return PLACED;
	//如果位置有障碍，则可以破坏
	if (map[i][j] == BARRIER)
		return BARRIER;
	return -1;
}

// 设置bottletower按钮点击事件处理函数
void GameMap1::bottlebuttonClickCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type,int i,int j,int placex,int placey)
{
	// 处理按钮点击事件
	if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
		//扣钱
		if (game_money1 >= 100){
			SimpleAudioEngine::getInstance()->playEffect("Music/TowerMusic/TowerBulid.mp3");
			game_money1 -= 100;
		}
		else{
			SimpleAudioEngine::getInstance()->playEffect("Music/SelectFault.mp3");
			return;
		}

		map[i][j] = PLACED;

		// 移除所有按钮
		hideButton();

		// 创建BottleTower
		auto bottletower = BottleTower::create("Tower/Bottle11.png");
		bottletower->pos_i = i;
		bottletower->pos_j = j;
		bottletower->setPosition(Vec2(placex, placey));

		// 将防御塔添加到场景和容器中
		this->addChild(bottletower,8);
		bottletowers.pushBack(bottletower);
	}
}

// 设置shittower按钮点击事件处理函数
void GameMap1::shitbuttonClickCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type, int i, int j, int placex, int placey)
{
	// 处理按钮点击事件
	if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
		//扣钱
		if (game_money1 >= 120) {
			SimpleAudioEngine::getInstance()->playEffect("Music/TowerMusic/TowerBulid.mp3");
			game_money1 -= 120;
		}
		else {
			SimpleAudioEngine::getInstance()->playEffect("Music/SelectFault.mp3");
			return;
		}

		map[i][j] = PLACED;

		// 移除所有按钮
		hideButton();

		// 创建ShitTower
		auto shittower = ShitTower::create("Tower/shit1.png");
		shittower->pos_i = i;
		shittower->pos_j = j;
		shittower->setPosition(Vec2(placex, placey));
		shittower->setScale(0.9);

		// 将防御塔添加到场景和容器中
		this->addChild(shittower, 8);
		shittowers.pushBack(shittower);
	}
}

// 设置fantower按钮点击事件处理函数
void GameMap1::fanbuttonClickCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type, int i, int j, int placex, int placey)
{
	// 处理按钮点击事件
	if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
		//扣钱
		if (game_money1 >= 160) {
			SimpleAudioEngine::getInstance()->playEffect("Music/TowerMusic/TowerBulid.mp3");
			game_money1 -= 160;
		}
		else {
			SimpleAudioEngine::getInstance()->playEffect("Music/SelectFault.mp3");
			return;
		}

		map[i][j] = PLACED;

		// 移除所有按钮
		hideButton();

		// 创建FanTower
		auto fantower = FanTower::create("Tower/fan11.png");
		fantower->pos_i = i;
		fantower->pos_j = j;
		fantower->setPosition(Vec2(placex, placey));
		fantower->setScale(0.9);

		// 将防御塔添加到场景和容器中
		this->addChild(fantower, 8);
		fantowers.pushBack(fantower);
	}
}

//移除button
void GameMap1::hideButton()
{
	if (setTower) {
		setTower->removeFromParent();
		setTower = NULL;
	}
	if (bottlebtn) {
		bottlebtn->removeFromParent();
		bottlebtn = NULL;
	}
	if (shitbtn) {
		shitbtn->removeFromParent();
		shitbtn = NULL;
	}
	if (fanbtn) {
		fanbtn->removeFromParent();
		fanbtn = NULL;
	}
	if (upgradebtn) {
		upgradebtn->removeFromParent();
		upgradebtn = NULL;
	}
	if (cancelbtn) {
		cancelbtn->removeFromParent();
		cancelbtn = NULL;
	}
	if (maxbtn) {
		maxbtn->removeFromParent();
		maxbtn = NULL;
	}
	if (range) {
		float scaleDuration = 0.3f;
		auto rangePtr = range;

		range->runAction(Sequence::create(
			// 缩放动作，从初始大小到 0
			ScaleTo::create(scaleDuration, 0.0f),

			// 隐藏精灵
			Hide::create(),

			// 结束动作序列
			cocos2d::CallFunc::create([rangePtr]() {
				if (rangePtr)
					rangePtr->removeFromParent();
				}),
			nullptr
		));

		range = nullptr;
	}
	selectedPosition = Vec2::ZERO;
}

void GameMap1::onMouseDown(EventMouse* event)
{
	// 如果游戏暂停或显示菜单，阻止操作
	if (isGamePaused)
		return;

	// 获取点击的位置
	int mousex = event->getCursorX();
	int mousey = event->getCursorY();
	int placex, placey;

	Vec2 clickLocation = Vec2(mousex, mousey);
	int i, j;
	int status = getStatus(mousex, mousey);

	if ((pausebtn && pausebtn->getBoundingBox().containsPoint(clickLocation)) ||
		(menubtn && menubtn->getBoundingBox().containsPoint(clickLocation))) {
		return; // 点击在 UI 按钮上，直接返回
	}

	// 如果已经选中了位置，取消选中并隐藏按钮
	if (selectedPosition != Vec2::ZERO) {
		//消除所有按钮和显示
		if (bottlebtn && bottlebtn->getBoundingBox().containsPoint(clickLocation)) {
			// 如果点击到了按钮，则不取消选中，而是处理按钮点击事件
			return;
		}
		else if (shitbtn && shitbtn->getBoundingBox().containsPoint(clickLocation)) {
			return;
		}
		else if (fanbtn && fanbtn->getBoundingBox().containsPoint(clickLocation)) {
			return;
		}
		else if (upgradebtn && upgradebtn->getBoundingBox().containsPoint(clickLocation)) {
			return;
		}
		else if (cancelbtn && cancelbtn->getBoundingBox().containsPoint(clickLocation)) {
			return;
		}
		else if (maxbtn && maxbtn->getBoundingBox().containsPoint(clickLocation)) {
			return;
		}
		else {
			hideButton();
			selectedPosition = Vec2::ZERO;
			return;
		}
	}

	if (status == -1) {		// 当前位置不能放置炮塔

		// 创建警告精灵并设置图片
		auto warning = Sprite::create("GameMap/warning.png");
		SimpleAudioEngine::getInstance()->playEffect("Music/SelectFault.mp3");
		warning->setScale(0.8);
		warning->setPosition(clickLocation);

		// 将警告精灵添加到场景
		this->addChild(warning, 6);

		// 延迟一定时间后移除精灵
		auto delayAction = DelayTime::create(0.3f); // 延迟0.5秒
		auto removeAction = RemoveSelf::create();
		auto sequence = Sequence::create(delayAction, removeAction, nullptr);

		warning->runAction(sequence);
		return;
	}
	else if (status == EMPTY) {		// 当前位置可以放置炮塔

		transform_xy_to_ij(i, j, mousex, mousey);
		transform_ij_to_xy(i, j, placex, placey);

		// 显示添加炮塔的图片框
		setTower = Sprite::create("GameMap/select_01.png");
		setTower->setPosition(Vec2(90 + 80 * j, 40 + 80 * (6 - i)));
		this->addChild(setTower, 5);
		selectedPosition = clickLocation;

		// 获取屏幕可视范围
		auto visibleSize = Director::getInstance()->getVisibleSize();
		float screenMinX = 0;
		float screenMaxX = visibleSize.width;
		float screenMinY = 0;
		float screenMaxY = visibleSize.height;

		// 初始化按钮位置
		Vec2 bottleBtnPosition = Vec2(placex - 70, placey - 70);
		Vec2 shitBtnPosition = Vec2(placex, placey - 70);
		Vec2 fanBtnPosition = Vec2(placex + 70, placey - 70);

		// 调整 bottlebtn 的位置，确保在屏幕范围内
		if (bottleBtnPosition.x < screenMinX) bottleBtnPosition.x = screenMinX + 100; // 保持按钮稍微离屏幕边缘
		if (bottleBtnPosition.x > screenMaxX) bottleBtnPosition.x = screenMaxX - 100;
		if (bottleBtnPosition.y < screenMinY) bottleBtnPosition.y = screenMinY + 100;
		if (bottleBtnPosition.y > screenMaxY) bottleBtnPosition.y = screenMaxY - 100;

		// 调整 shitbtn 的位置，确保在屏幕范围内
		if (shitBtnPosition.x < screenMinX) shitBtnPosition.x = screenMinX + 100;
		if (shitBtnPosition.x > screenMaxX) shitBtnPosition.x = screenMaxX - 100;
		if (shitBtnPosition.y < screenMinY) shitBtnPosition.y = screenMinY + 100;
		if (shitBtnPosition.y > screenMaxY) shitBtnPosition.y = screenMaxY - 100;

		// 调整 fanbtn 的位置，确保在屏幕范围内
		if (fanBtnPosition.x < screenMinX) fanBtnPosition.x = screenMinX + 100;
		if (fanBtnPosition.x > screenMaxX) fanBtnPosition.x = screenMaxX - 100;
		if (fanBtnPosition.y < screenMinY) fanBtnPosition.y = screenMinY + 100;
		if (fanBtnPosition.y > screenMaxY) fanBtnPosition.y = screenMaxY - 100;

		if (1) {
			// 创建按钮
			bottlebtn = Button::create("Tower/Bottle01.png", "Tower/Bottle01.png", "");
			bottlebtn->setPosition(bottleBtnPosition);
			bottlebtn->setScale(0.8);
			bottlebtn->addTouchEventListener(CC_CALLBACK_2(GameMap1::bottlebuttonClickCallback, this, i, j, placex, placey));
			this->addChild(bottlebtn, 10);

			shitbtn = Button::create("Tower/shit01.png", "Tower/shit01.png", "");
			shitbtn->setPosition(shitBtnPosition);
			shitbtn->setScale(0.7);
			shitbtn->addTouchEventListener(CC_CALLBACK_2(GameMap1::shitbuttonClickCallback, this, i, j, placex, placey));
			this->addChild(shitbtn, 10);

			fanbtn = Button::create("Tower/fan00.png", "Tower/fan00.png", "");
			fanbtn->setPosition(fanBtnPosition);
			fanbtn->setScale(0.7);
			fanbtn->addTouchEventListener(CC_CALLBACK_2(GameMap1::fanbuttonClickCallback, this, i, j, placex, placey));
			this->addChild(fanbtn, 10);

		}
		else {
			// 创建按钮
			bottlebtn = Button::create("Tower/Bottle00.png", "Tower/Bottle00.png", "");
			bottlebtn->setScale(0.8);
			bottlebtn->setPosition(Vec2(placex - 40, placey - 70));
			this->addChild(bottlebtn, 10);
		}
	}
	else if(status==PLACED){	// 当前位置有炮塔用来升级

		transform_xy_to_ij(i, j, mousex, mousey);
		selectedPosition = clickLocation;

		BottleTower* bt = NULL;	//找到（i，j）位置的bottletower，找不到就是NULL
		for (auto tower : bottletowers) {
			if (tower->pos_i == i && tower->pos_j == j) {
				bt = tower;
				break;
			}
		}

		ShitTower* st = NULL;	//找到（i，j）位置的shittower，找不到就是NULL
		for (auto tower : shittowers) {
			if (tower->pos_i == i && tower->pos_j == j) {
				st = tower;
				break;
			}
		}

		FanTower* ft = NULL;	//找到（i，j）位置的fantower，找不到就是NULL
		for (auto tower : fantowers) {
			if (tower->pos_i == i && tower->pos_j == j) {
				ft = tower;
				break;
			}
		}

		if (bt != NULL)
			upgradeBottle(bt);

		if (st != NULL)
			upgradeShit(st);

		if (ft != NULL)
			upgradeFan(ft);
	}
}

//升级按钮的回调函数
void GameMap1::upgradebuttonClickCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type, const std::string& towerImage,BottleTower* bt,ShitTower* st,FanTower* ft)
{
	// 处理按钮点击事件
	if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
		if (bt) {
			if (bt->level == 1){
				//扣钱
				if (game_money1 >= 180){
					SimpleAudioEngine::getInstance()->playEffect("Music/TowerMusic/TowerUpdate.mp3");
					game_money1 -= 180;
				}
				else {
					SimpleAudioEngine::getInstance()->playEffect("Music/SelectFault.mp3");
					return;
				}
			}
			else if (bt->level == 2){
				//扣钱
				if (game_money1 >= 260){
					SimpleAudioEngine::getInstance()->playEffect("Music/TowerMusic/TowerUpdate.mp3");
					game_money1 -= 260;
				}
				else{
					SimpleAudioEngine::getInstance()->playEffect("Music/SelectFault.mp3");
					return;
				}
			}

			//更新等级
			bt->level++;

			//升级换图片
			bt->setTexture(towerImage);

			// 移除所有按钮
			hideButton();

			return;
		}
		if (st) {
			if (st->level == 1){
				//扣钱
				if (game_money1 >= 220) {
					SimpleAudioEngine::getInstance()->playEffect("Music/TowerMusic/TowerUpdate.mp3");
					game_money1 -= 220;
				}
				else {
					SimpleAudioEngine::getInstance()->playEffect("Music/SelectFault.mp3");
					return;
				}
			}
			else if (st->level == 2)
			{
				//扣钱
				if (game_money1 >= 260) {
					SimpleAudioEngine::getInstance()->playEffect("Music/TowerMusic/TowerUpdate.mp3");
					game_money1 -= 260;
				}
				else {
					SimpleAudioEngine::getInstance()->playEffect("Music/SelectFault.mp3");
					return;
				}
			}
			//更新等级
			st->level++;

			//升级换图片
			st->setTexture(towerImage);

			// 移除所有按钮
			hideButton();

			return;
		}
		if (ft) {
			if (ft->level == 1) {
				//扣钱
				if (game_money1 >= 220) {
					SimpleAudioEngine::getInstance()->playEffect("Music/TowerMusic/TowerUpdate.mp3");
					game_money1 -= 220;
				}
				else {
					SimpleAudioEngine::getInstance()->playEffect("Music/SelectFault.mp3");
					return;
				}
			}
			else if (ft->level == 2)
			{
				//扣钱
				if (game_money1 >= 260) {
					SimpleAudioEngine::getInstance()->playEffect("Music/TowerMusic/TowerUpdate.mp3");
					game_money1 -= 260;
				}
				else {
					SimpleAudioEngine::getInstance()->playEffect("Music/SelectFault.mp3");
					return;
				}
			}
			//更新等级
			ft->level++;

			//升级换图片
			ft->setTexture(towerImage);

			// 移除所有按钮
			hideButton();

			return;
		}
	}
}

//拆除按钮的回调函数
void GameMap1::cancelbuttonClickCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type, BottleTower* bt, ShitTower* st,FanTower *ft)
{
	// 处理按钮点击事件
	if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
		//拆除bottle
		if (bt) {
			if (bt->level == 1)
				game_money1 += 80;
			else if (bt->level == 2)
				game_money1 += 224;
			else if (bt->level == 3)
				game_money1 += 432;

			map[bt->pos_i][bt->pos_j] = EMPTY;

			bt->removeFromParent();

			auto it = bottletowers.begin();
			while (it != bottletowers.end()) {
				if (*it == bt) {					// 找到当前炮塔
					delete* it;						// 释放内存
					it = bottletowers.erase(it);	// 从 vector 中删除元素
				}
				else ++it;
			}

			// 移除所有按钮
			hideButton();

			return;
		}
		//拆除shit
		if (st) {
			if (st->level == 1)
				game_money1 += 96;
			else if (st->level == 2)
				game_money1 += 272;
			else if (st->level == 3)
				game_money1 += 480;

			map[st->pos_i][st->pos_j] = EMPTY;

			st->removeFromParent();

			auto it = shittowers.begin();
			while (it != shittowers.end()) {
				if (*it == st) {					// 找到当前炮塔
					delete* it;						// 释放内存
					it = shittowers.erase(it);		// 从 vector 中删除元素
				}
				else
					++it;
			}

			// 移除所有按钮
			hideButton();

			return;
		}
		//拆除fan
		if (ft) {
			if (ft->level == 1)
				game_money1 += 96;
			else if (ft->level == 2)
				game_money1 += 272;
			else if (ft->level == 3)
				game_money1 += 480;

			map[ft->pos_i][ft->pos_j] = EMPTY;

			ft->removeFromParent();

			auto it = fantowers.begin();
			while (it != fantowers.end()) {
				if (*it == ft) {					// 找到当前炮塔
					delete* it;						// 释放内存
					it = fantowers.erase(it);		// 从 vector 中删除元素
				}
				else
					++it;
			}

			// 移除所有按钮
			hideButton();

			return;
		}
	}
}

//升级bottletower
void GameMap1::upgradeBottle(BottleTower* bt) 
{
	ShitTower* emptytower = NULL;
	FanTower* emptyfan = NULL;

	int pos_x, pos_y;
	transform_ij_to_xy(bt->pos_i, bt->pos_j, pos_x, pos_y);
	float scale1 = 0.4;
	float scale2 = 0.5;
	float scale3 = 0.65;

	//一级炮塔升级
	if (bt->level == 1) {
		
		//放置范围灰色圈
		setRange(scale1, pos_x, pos_y);

		//放置升级和删除的按钮
		std::string upgradebtnImage = "Tower/upgradebottle1.png";
		std::string towerImage = "Tower/Bottle21.png";
		std::string cancelbtnImage = "Tower/cancelbottle1.png";

		setUpgradeButton(upgradebtnImage, towerImage, pos_x, pos_y, 1, bt, emptytower, emptyfan);
		setCancelButton(cancelbtnImage, pos_x, pos_y, bt, emptytower, emptyfan);
		
	}
	//二级炮塔升级
	else if (bt->level == 2) {

		//放置范围灰色圈
		setRange(scale2, pos_x, pos_y);

		//放置升级和删除的按钮
		std::string upgradebtnImage = "Tower/upgradebottle2.png";
		std::string towerImage = "Tower/Bottle31.png";
		std::string cancelbtnImage = "Tower/cancelbottle2.png";

		setUpgradeButton(upgradebtnImage, towerImage, pos_x, pos_y, 1, bt, emptytower, emptyfan);
		setCancelButton(cancelbtnImage, pos_x, pos_y, bt, emptytower, emptyfan);

	}
	//三级炮塔升级
	else {

		std::string cancelbtnImage = "Tower/cancelbottle3.png";
		//放置范围灰色圈
		setRange(scale3, pos_x, pos_y);

		//放置max和删除的按钮
		maxbtn = Button::create("Tower/max.png", "Tower/max.png", "");
		maxbtn->setPosition(Vec2(pos_x, pos_y + 70));
		maxbtn->setScale(0.8);
		this->addChild(maxbtn, 10);

		setCancelButton(cancelbtnImage, pos_x, pos_y, bt, emptytower, emptyfan);
	}
}

// 升级shittower
void GameMap1::upgradeShit(ShitTower*st)
{
	BottleTower* emptytower = NULL;
	FanTower* emptyfan = NULL;
	int pos_x, pos_y;
	transform_ij_to_xy(st->pos_i, st->pos_j, pos_x, pos_y);
	float scale1 = 0.4;
	float scale2 = 0.5;
	float scale3 = 0.65;

	//一级炮塔升级
	if (st->level == 1) {
		//放置范围灰色圈
		setRange(scale1, pos_x, pos_y);

		//放置升级和删除的按钮
		std::string upgradebtnImage = "Tower/upgradeshit1.png";
		std::string towerImage = "Tower/shit2.png";
		std::string cancelbtnImage = "Tower/cancelshit1.png";

		setUpgradeButton(upgradebtnImage, towerImage, pos_x, pos_y, 1, emptytower, st, emptyfan);
		setCancelButton(cancelbtnImage, pos_x, pos_y, emptytower, st, emptyfan);

	}
	//二级炮塔升级
	else if (st->level == 2) {
		//放置范围灰色圈
		setRange(scale2, pos_x, pos_y);

		//放置升级和删除的按钮
		std::string upgradebtnImage = "Tower/upgradeshit2.png";
		std::string towerImage = "Tower/shit3.png";
		std::string cancelbtnImage = "Tower/cancelshit2.png";

		setUpgradeButton(upgradebtnImage, towerImage, pos_x, pos_y, 1, emptytower, st, emptyfan);
		setCancelButton(cancelbtnImage, pos_x, pos_y, emptytower, st, emptyfan);

	}
	//三级炮塔升级
	else {
		std::string cancelbtnImage = "Tower/cancelshit3.png";
		//放置范围灰色圈
		setRange(scale3, pos_x, pos_y);

		//放置max和删除的按钮
		maxbtn = Button::create("Tower/max.png", "Tower/max.png", "");
		maxbtn->setPosition(Vec2(pos_x, pos_y + 70));
		maxbtn->setScale(0.8);
		this->addChild(maxbtn, 10);

		setCancelButton(cancelbtnImage, pos_x, pos_y, emptytower, st, emptyfan);
	}
}

// 升级fantower
void GameMap1::upgradeFan(FanTower* ft)
{
	BottleTower* emptytower = NULL;
	ShitTower* emptyshit = NULL;
	int pos_x, pos_y;
	transform_ij_to_xy(ft->pos_i, ft->pos_j, pos_x, pos_y);
	float scale1 = 0.4;
	float scale2 = 0.5;
	float scale3 = 0.65;

	//一级炮塔升级
	if (ft->level == 1) {
		//放置范围灰色圈
		setRange(scale1, pos_x, pos_y);

		//放置升级和删除的按钮
		std::string upgradebtnImage = "Tower/upgradeshit1.png";
		std::string towerImage = "Tower/fan22.png";
		std::string cancelbtnImage = "Tower/cancelshit1.png";

		setUpgradeButton(upgradebtnImage, towerImage, pos_x, pos_y, 1, emptytower, emptyshit, ft);
		setCancelButton(cancelbtnImage, pos_x, pos_y, emptytower, emptyshit, ft);

	}
	//二级炮塔升级
	else if (ft->level == 2) {
		//放置范围灰色圈
		setRange(scale2, pos_x, pos_y);

		//放置升级和删除的按钮
		std::string upgradebtnImage = "Tower/upgradeshit2.png";
		std::string towerImage = "Tower/fan33.png";
		std::string cancelbtnImage = "Tower/cancelshit2.png";

		setUpgradeButton(upgradebtnImage, towerImage, pos_x, pos_y, 1, emptytower, emptyshit, ft);
		setCancelButton(cancelbtnImage, pos_x, pos_y, emptytower, emptyshit, ft);

	}
	//三级炮塔升级
	else {
		std::string cancelbtnImage = "Tower/cancelshit3.png";
		//放置范围灰色圈
		setRange(scale3, pos_x, pos_y);

		//放置max和删除的按钮
		maxbtn = Button::create("Tower/max.png", "Tower/max.png", "");
		maxbtn->setPosition(Vec2(pos_x, pos_y + 70));
		maxbtn->setScale(0.8);
		this->addChild(maxbtn, 10);

		setCancelButton(cancelbtnImage, pos_x, pos_y, emptytower, emptyshit, ft);
	}
}

void GameMap1::setUpgradeButton(const std::string& btnImage, const std::string& towerImage, int pos_x, int pos_y, bool can_be_clicked, BottleTower* bt, ShitTower* st, FanTower* ft)
{
	upgradebtn = Button::create(btnImage, btnImage, "");
	upgradebtn->setPosition(Vec2(pos_x, pos_y + 70));
	upgradebtn->setScale(0.8);
	if (can_be_clicked)
		upgradebtn->addTouchEventListener(CC_CALLBACK_2(GameMap1::upgradebuttonClickCallback, this, towerImage, bt, st, ft));
	this->addChild(upgradebtn, 10);
}

// 设定攻击范围
void GameMap1::setRange(float scale, int pos_x, int pos_y)
{
	range = Sprite::create("Tower/range.png");
	range->setPosition(Vec2(pos_x, pos_y));
	range->setScale(scale);
	this->addChild(range,9);
}

void GameMap1::setCancelButton(const std::string& btnImage, int pos_x, int pos_y, BottleTower* bt, ShitTower* st, FanTower* ft)
{
	cancelbtn = Button::create(btnImage, btnImage, "");
	cancelbtn->setPosition(Vec2(pos_x, pos_y - 70));
	cancelbtn->setScale(0.8);
	cancelbtn->addTouchEventListener(CC_CALLBACK_2(GameMap1::cancelbuttonClickCallback, this, bt, st, ft));
	this->addChild(cancelbtn, 10);
}

// 倒计时
void GameMap1::countDown()
{
	//获取屏幕大小
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//创建遮罩层
	auto time_layer = Layer::create();
	this->addChild(time_layer,600);

	//添加触摸事件吞噬
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [](Touch* touch, Event* event) {
		return true;
		};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, time_layer);

	//倒计时背景盘
	auto timepanel = Sprite::create("GameMap/timepanel.png");
	timepanel->setPosition(Vec2(origin.x + visibleSize.width / 2,origin.y + visibleSize.height / 2));
	time_layer->addChild(timepanel);

	//倒计时数字
	auto number = Sprite::create("GameMap/time3.png");
	number->setPosition(Vec2(origin.x + visibleSize.width * 0.5,origin.y + visibleSize.height / 2+10));
	time_layer->addChild(number);

	//倒计时转圈
	auto circle = Sprite::create("GameMap/timecircle.png");
	circle->setPosition(Vec2(visibleSize.width / 2, + visibleSize.height / 2));
	circle->setAnchorPoint(Vec2(1, 0.5));
	circle->setScale(0.95);
	time_layer->addChild(circle);
	circle->runAction(Sequence::create(Repeat::create(RotateBy::create(1, -360), 3), FadeOut::create(0.1), nullptr));
	timepanel->runAction(Sequence::create(DelayTime::create(4), FadeOut::create(0.1), nullptr));

	//利用帧动画完成数字的倒计时
	Vector<SpriteFrame*> frame;
	frame.pushBack(SpriteFrame::create("GameMap/time3.png", Rect(0, 0, 95, 114)));
	frame.pushBack(SpriteFrame::create("GameMap/time2.png", Rect(0, 0, 95, 114)));
	frame.pushBack(SpriteFrame::create("GameMap/time1.png", Rect(0, 0, 95, 114)));
	frame.pushBack(SpriteFrame::create("GameMap/timego.png", Rect(0, 0, 210, 114)));
	number->runAction(Sequence::create(Animate::create(Animation::createWithSpriteFrames(frame, 1)), FadeOut::create(0.1), nullptr));

	//三秒后把遮罩层删除
	auto start_call_back = CallFunc::create([=]() {
		this->removeChild(time_layer);
		});
	time_layer->runAction(Sequence::create(DelayTime::create(4.1), start_call_back, nullptr));
}

// 三种怪物
void GameMap1::init_m1(float delta) {
	auto monsterSprite = Monster::create();
	this->addChild(monsterSprite, 100 - monsternum);

	monsterSprite->mapNum = 1;
	monsterSprite->initMonsterType1(1);
	monsterSprite->schedule(schedule_selector(Monster::update), 0.05f);
	monsterSprite->setPosition(140, 485);
	monsternum++;
}

void GameMap1::init_m2(float delta) {
	auto monsterSprite = Monster::create();
	this->addChild(monsterSprite, 100 - monsternum);

	monsterSprite->mapNum = 1;
	monsterSprite->initMonsterType2(1);
	monsterSprite->schedule(schedule_selector(Monster::update), 0.05f);
	monsterSprite->setPosition(140, 485);
	monsternum++;
}

void GameMap1::init_m3(float delta) {
	auto monsterSprite = Monster::create();
	this->addChild(monsterSprite, 100 - monsternum);

	monsterSprite->mapNum = 1;
	monsterSprite->initMonsterType3(1);
	monsterSprite->schedule(schedule_selector(Monster::update), 0.05f);
	monsterSprite->setPosition(140, 485);
	monsternum++;
}

void GameMap1::init_boss(float delta) {
	auto monsterSprite = Monster::create();
	this->addChild(monsterSprite, 100 - monsternum);

	monsterSprite->mapNum = 1;
	monsterSprite->initMonsterboss(1);
	monsterSprite->schedule(schedule_selector(Monster::update), 0.05f);
	monsterSprite->setPosition(140, 485);
	monsternum++;
}

void GameMap1::bo(float a) {
	cocos2d::Texture2D* texture;
	switch (current_wave)
	{
		case 1:
			//第一波
			schedule(schedule_selector(GameMap1::init_m3), 1, 4, 1);
			texture = Director::getInstance()->getTextureCache()->addImage("Gamemap/wave_1.png");
			
			// 设置新的纹理  
			wave->setTexture(texture);
			break;
		case 2:
			//第二波
			texture = Director::getInstance()->getTextureCache()->addImage("Gamemap/wave_2.png");
			
			// 设置新的纹理  
			wave->setTexture(texture);
			schedule(schedule_selector(GameMap1::init_m1), 1, 4, 1);
			break;
		case 3:
			//第三波
			texture = Director::getInstance()->getTextureCache()->addImage("Gamemap/wave_3.png");
			
			// 设置新的纹理  
			wave->setTexture(texture);
			schedule(schedule_selector(GameMap1::init_m2), 1, 4, 1);
			break;
		case 4:
			//第四波
			texture = Director::getInstance()->getTextureCache()->addImage("Gamemap/wave_4.png");
			
			// 设置新的纹理  
			wave->setTexture(texture);
			schedule(schedule_selector(GameMap1::init_m3), 1, 4, 1);
			break;
		case 5:
			//第五波
			texture = Director::getInstance()->getTextureCache()->addImage("Gamemap/wave_5.png");
			
			// 设置新的纹理  
			wave->setTexture(texture);
			schedule(schedule_selector(GameMap1::init_m3), 1, 4, 1);
			break;
		case 6:
			schedule(schedule_selector(GameMap1::init_m2), 1, 4, 1);
			break;
		case 7:
			schedule(schedule_selector(GameMap1::init_m1), 1, 4, 1);
			break;
		case 8:
			schedule(schedule_selector(GameMap1::init_boss), 1, 0, 1);
			break;
		default:
			break;
	}
	current_wave++;
	if (current_wave >= 9)
		this->schedule(schedule_selector(GameMap1::next_bo), 0.05f);
	if (current_wave < 6)
		this->schedule(schedule_selector(GameMap1::next_bo), 3);
	else
		this->scheduleOnce(schedule_selector(GameMap1::bo), 5);
}

void GameMap1::next_bo(float a) {
	if (current_wave >= 9) {
		if (monsternum >= 36 && monsterContainer.empty()){//判断输赢
			win();
			this->unschedule(schedule_selector(GameMap1::next_bo));
		}
	}
	else if (monsterContainer.empty()){
		this->unschedule(schedule_selector(GameMap1::next_bo));
		this->GameMap1::bo(0.1f);
	}
}

void GameMap1::win()
{
	auto winmenu = Sprite::create("GameMap/win.png");

	// 设置菜单位置
	winmenu->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
		Director::getInstance()->getVisibleSize().height * 2));
	this->addChild(winmenu, 200);

	// 执行菜单从上方移动到中央的动作
	auto moveAction = EaseBackOut::create(MoveTo::create(0.5f, Vec2(Director::getInstance()->getVisibleSize().width / 2,
		Director::getInstance()->getVisibleSize().height / 2)));
	winmenu->runAction(moveAction);

	//奖杯的放置
	auto champion = Sprite::create("GameMap/champion.png");
	winmenu->addChild(champion);
	champion -> setPosition(Vec2(360, 400));

	is_Pass[1] = 1;

	//继续游戏按钮
	auto continuebtn = Button::create("GameMap/continue.png", "GameMap/continue.png", "");
	winmenu->addChild(continuebtn);
	continuebtn->setPressedActionEnabled(true);
	continuebtn->setPosition(Vec2(360, 255));
	continuebtn->setScale(1.12);
	continuebtn->addTouchEventListener([](Ref* sender, Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED) {
			SimpleAudioEngine::getInstance()->playEffect("Music/Select.mp3", false, 1.0f, 1.0f, 1.0f);
			SimpleAudioEngine::getInstance()->stopBackgroundMusic();
			Director::getInstance()->resume();

			auto enterscene = EnterScene2::createScene();
			Director::getInstance()->replaceScene(enterscene);
		}
		});

	//选择关卡按钮
	auto chooselevelbtn = Button::create("GameMap/chooselevel.png", "GameMap/chooselevel.png", "");
	winmenu->addChild(chooselevelbtn);
	chooselevelbtn->setPressedActionEnabled(true);
	chooselevelbtn->setPosition(Vec2(360, 100));
	chooselevelbtn->setScale(1.12);
	chooselevelbtn->addTouchEventListener([](Ref* sender, Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED) {
			SimpleAudioEngine::getInstance()->playEffect("Music/Select.mp3", false, 1.0f, 1.0f, 1.0f);
			SimpleAudioEngine::getInstance()->stopBackgroundMusic();
			Director::getInstance()->resume();

			auto chooseLevel = ChooseLevel::createScene();
			Director::getInstance()->replaceScene(chooseLevel);
		}
		});

	// 使用Sequence连接动作和回调函数
	winmenu->runAction(Sequence::create(moveAction, CallFunc::create([this]() {
		// 在动作执行完后执行的回调函数
		// 游戏暂停
		isGamePaused = true;
	Director::getInstance()->pause();
		}), nullptr));

}

void GameMap1::lose()
{
	auto losemenu = Sprite::create("GameMap/lose.png");

	// 设置菜单位置
	losemenu->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
		Director::getInstance()->getVisibleSize().height * 2));
	this->addChild(losemenu, 200);

	// 执行菜单从上方移动到中央的动作
	auto moveAction = EaseBackOut::create(MoveTo::create(0.5f, Vec2(Director::getInstance()->getVisibleSize().width / 2,
		Director::getInstance()->getVisibleSize().height / 2)));
	losemenu->runAction(moveAction);

	//重新开始游戏按钮
	auto restartbtn = Button::create("GameMap/restart.png", "GameMap/restart.png", "");
	losemenu->addChild(restartbtn);
	restartbtn->setPressedActionEnabled(true);
	restartbtn->setPosition(Vec2(360, 300));
	restartbtn->setScale(1.12);
	restartbtn->addClickEventListener([=](Ref* sender) {
		SimpleAudioEngine::getInstance()->playEffect("Music/Select.mp3", false, 1.0f, 1.0f, 1.0f);
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		removeAllChildrenWithCleanup(true);
		Director::getInstance()->resume();

		auto gamemap1 = GameMap1::create();
		Director::getInstance()->replaceScene(gamemap1);
		});

	//选择关卡按钮
	auto chooselevelbtn = Button::create("GameMap/chooselevel.png", "GameMap/chooselevel.png", "");
	losemenu->addChild(chooselevelbtn);
	chooselevelbtn->setPressedActionEnabled(true);
	chooselevelbtn->setPosition(Vec2(360, 150));
	chooselevelbtn->setScale(1.12);
	chooselevelbtn->addClickEventListener([=](Ref* sender) {
		SimpleAudioEngine::getInstance()->playEffect("Music/Select.mp3", false, 1.0f, 1.0f, 1.0f);
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		removeAllChildrenWithCleanup(true);
		Director::getInstance()->resume();

		auto chooselevel = ChooseLevel::create();
		Director::getInstance()->replaceScene(chooselevel);
		});

	// 使用Sequence连接动作和回调函数
	losemenu->runAction(Sequence::create(moveAction, CallFunc::create([this]() {
		// 在动作执行完后执行的回调函数
		// 游戏暂停
		isGamePaused = true;
	Director::getInstance()->pause();
		}), nullptr));

}
