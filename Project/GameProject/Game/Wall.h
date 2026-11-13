#pragma once
#include "Base/Base.h"
class Wall :public Base {
public:
	CImage m_img;
public:
	Wall(const CVector2D& pos);
	void Update();
	void Draw();
};
