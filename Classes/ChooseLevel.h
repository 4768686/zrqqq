#ifndef __CHOOSE_LEVEL_H__
#define __CHOOSE_LEVEL_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <string.h>
#define page_num 2


class ChooseLevel :public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(ChooseLevel);

private:
	void InitUI();
	void InitEvent();

private:
	cocos2d::ui::Button* _toolbarHome;
	cocos2d::ui::Button* _toolbarShop;
	cocos2d::ui::Button* _toolbarLeaderboard;
	cocos2d::ui::Button* _toolbarRightbg;

};
#endif // __CHOOSE_LEVEL_H__
