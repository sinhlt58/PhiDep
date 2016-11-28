#pragma once
#include <2d/CCSprite.h>

class Entity
{
public:
	Entity();
	Entity(std::string spritePath);
	virtual ~Entity();
	
	cocos2d::Sprite* m_Sprite;
	cocos2d::Action* m_OnceTimeAnimation;
	cocos2d::Action* m_LoopAnimation;

	/*Animation functions*/
	void setOnceTimeAnimationAction(const char *format, int count);
	void setLoopAnimationAction(const char *format, int count);
	void startOnceTimeAnimation();
	void startLoopAnimation();
	void stopOnceTimeAnimation();
	void stopLoopAnimation();

	virtual void Reset();
	
	cocos2d::Vec2 m_defaultPos;
	void setDefaultPos(cocos2d::Vec2 pos);
	virtual bool isOutOffTheScreen(cocos2d::Size winSize, float upperYMax);
};
