#include "Dep.h"
#include "Globals.h"

Dep::Dep(std::string spritePath)
	: Entity(spritePath)
{
	auto selfSize = m_Sprite->getContentSize();
	auto physicsBody = cocos2d::PhysicsBody::createBox(cocos2d::Size(selfSize.width, selfSize.height),
		cocos2d::PhysicsMaterial(0.1f, 0.3f, 0.2f));
	physicsBody->setDynamic(true);
	physicsBody->setCategoryBitmask(int(PhysicsCategory::dep));
	physicsBody->setCategoryBitmask(int(PhysicsCategory::muc_tieu));
	physicsBody->setContactTestBitmask(int(PhysicsCategory::dep));
	m_Sprite->setTag(int(PhysicsCategory::dep));
	m_Sprite->setPhysicsBody(physicsBody);
}

void Dep::Reset()
{
	m_Sprite->getPhysicsBody()->setVelocity(cocos2d::Vec2(0, 0));
	m_Sprite->getPhysicsBody()->setAngularVelocity(0);
	m_Sprite->setRotation(0);
	m_Sprite->setPosition(m_defaultPos);
}
