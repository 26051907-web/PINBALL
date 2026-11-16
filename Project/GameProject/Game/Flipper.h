#pragma once
#include "Base/Base.h"
class Flipper :public Base {
public:
	CImage m_img;
	bool m_isLeft;
	float m_ang;
	//1フレームでフリッパーの角度がどれだけ変化したかを記録
	float m_angVelocity;
	bool m_isPressed;
public:
	Flipper(const CVector2D& pos,bool isLeft);
	void Update();
	void Draw();

	//tangent:フリッパーに沿ってボールを動かす処理で使用
	bool IsHit(const CVector2D& ballPos, CVector2D& normal, CVector2D& tangent,
		 float& overlap, float& hitPosition);
};