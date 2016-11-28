#include "Arrow.h"
#include <Globals.h>

Arrow::Arrow(std::string headFile, std::string bodyFile)
{
	m_Head = cocos2d::Sprite::create(headFile);
	m_Body = cocos2d::Sprite::create(bodyFile);
	m_Head->setAnchorPoint(cocos2d::Vec2(0.5, 0));
	m_Body->setAnchorPoint(cocos2d::Vec2(0.5, 0));
}

void Arrow::setPos(cocos2d::Vec2 pos)
{
	m_Body->setPosition(pos);
	auto bodySize = m_Body->getContentSize();
	m_Head->setPosition(cocos2d::Vec2(pos.x, pos.y + bodySize.height*m_Body->getScaleY()));
}

void Arrow::rotate(float degree)
{
	float radian = DegreeToRadian(degree);
	cocos2d::Vec2 baseX = cocos2d::Vec2(0, 1);
	cocos2d::Vec2 baseY = cocos2d::Vec2(1, 0);
	auto bodySize = m_Body->getContentSize();
	auto bodyPos = m_Body->getPosition();
	cocos2d::Vec2 headNewPos = bodySize.height*m_Body->getScaleY() 
		* (cos(radian)*baseX + sin(radian)*baseY)
		+ bodyPos;
	m_Head->setPosition(headNewPos);
	m_Head->setRotation(degree);
	m_Body->setRotation(degree);
	
}

void Arrow::scale(float factor)
{
	m_Body->setScaleY(factor);
	auto bodySize = m_Body->getContentSize();
	auto bodyPos = m_Body->getPosition();
	m_Head->setPosition(cocos2d::Vec2(bodyPos.x, bodyPos.y + bodySize.height*factor));
}

void Arrow::setDefaultPos(cocos2d::Vec2 pos)
{
	m_DefaultPos = pos;
}

void Arrow::resetToDefaultPos()
{
	setPos(m_DefaultPos);
}
