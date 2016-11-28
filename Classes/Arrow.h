#pragma once
#include <2d/CCSprite.h>

class Arrow
{
public:
	Arrow(std::string headFile, std::string bodyFile);
	cocos2d::Sprite* m_Head;
	cocos2d::Sprite* m_Body;
	cocos2d::Vec2 m_DefaultPos;
	void setPos(cocos2d::Vec2 pos);
	void rotate(float angle);
	void scale(float factor);
	void setDefaultPos(cocos2d::Vec2 pos);
	void resetToDefaultPos();
};
