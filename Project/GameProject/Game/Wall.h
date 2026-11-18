#pragma once
#include "Base/Base.h"
class Wall :public Base {
public:
	CImage m_img;

	//壁の中心から端までの距離
	//m_halfWidth：左右の端まで16px
	float m_halfWidth = 225.0f;
	//m_halfHeight：上下の端まで80px
	float m_halfHeight = 800.0f;
public:
	Wall(const CVector2D& pos);
	Wall(const CVector2D& pos, float width, float height);
	void Update();
	void Draw();
};
