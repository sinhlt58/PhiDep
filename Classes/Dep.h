#pragma once
#include "Entity.h"

class Dep : public Entity
{
public:
	Dep(std::string spritePath);
	void Reset() override;
};
