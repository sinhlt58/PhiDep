#include "MucTieu.h"
#include "Globals.h"

MucTieu::MucTieu(std::string spritePath)
	: Entity(spritePath)
{
	auto selfSize = m_Sprite->getContentSize();
	auto physicsBody = cocos2d::PhysicsBody::createCircle(selfSize.width/2,
		cocos2d::PhysicsMaterial(0.1f, 0.3f, 0.2f));
	physicsBody->setDynamic(true);
	physicsBody->setCategoryBitmask(int(PhysicsCategory::muc_tieu));
	physicsBody->setCategoryBitmask(int(PhysicsCategory::dep));
	physicsBody->setContactTestBitmask(int(PhysicsCategory::muc_tieu));
	m_Sprite->setTag(int(PhysicsCategory::muc_tieu));
	m_Sprite->setPhysicsBody(physicsBody);
}

void MucTieu::Reset()
{
	m_Sprite->getPhysicsBody()->setVelocity(cocos2d::Vec2(0, 0));
	m_Sprite->getPhysicsBody()->setAngularVelocity(0);
	m_Sprite->setRotation(0);
	m_Sprite->setPosition(m_defaultPos);
}
