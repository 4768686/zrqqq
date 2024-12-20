#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"

class EnterScene4 :public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(EnterScene4);
private:

	bool InitUI();
};
#pragma once
