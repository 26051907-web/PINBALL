#pragma once
#include "Base/Base.h"

class Bumper : public Base
{
public:
    CImage m_img;

    // ŽOŠpŒ`‚Ì’¸“_
    CVector2D m_a;
    CVector2D m_b;
    CVector2D m_c;

public:
    Bumper(const CVector2D& pos);

    void Update();
    void Draw();

    bool IsHit(
        const CVector2D& ballPos,
        float ballRad,
        CVector2D& normal,
        float& overlap
    );
};
