#pragma once
#include "Base/Base.h"
class Wall :public Base {
public:
	CImage m_img;

	//壁の中心から端までの距離
	//m_halfWidth：左右の端まで16px
	//m_halfHeight：上下の端まで80px
	float m_halfWidth = 16.0f;
	float m_halfHeight = 80.0f;
public:
	Wall(const CVector2D& pos);
	void Update();
	void Draw();
};
