#pragma once
#include "Base/Base.h"
class Flipper :public Base {
public:
	CImage m_img;
	bool m_isLeft;
	float m_ang;
	bool m_isPressed;
public:
	Flipper(const CVector2D& pos,bool isLeft);
	void Update();
	void Draw();
	bool IsHit(const CVector2D& ballPos, CVector2D& normal, CVector2D& tangent, float& overlap);
};