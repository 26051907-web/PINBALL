#include "Base/Base.h"
#include "Game/SlopeWall.h"

SlopeWall::SlopeWall(const CVector2D& pos, bool isLeft, float length) :Base(eType_SlopeWall)
{
    m_img = COPY_RESOURCE("SlopeWall", CImage);
    m_pos = pos;
    m_isLeft = isLeft;
    m_length = length;

    //斜め壁の回転軸
    if (m_isLeft)
    {
        m_img.SetCenter(0, 16);
    }
    else
    {
        m_img.SetCenter(m_length, 16);
    }
    m_img.SetSize(m_length, 32);

    if (m_isLeft)
    {
        m_ang = 0.52f;
    }
    else
    {
        m_ang = -0.52f;
    }

    m_img.SetAng(-m_ang);
}
void SlopeWall::Update()
{
   
}
void SlopeWall::Draw()
{
    m_img.SetPos(m_pos);

    if (!m_isLeft)
    {
        m_img.m_flipH = true;
    }
    else
    {
        m_img.m_flipH = false;
    }

    m_img.Draw();
}
bool SlopeWall::IsHit(const CVector2D& ballPos, float ballRad, CVector2D& normal,
    float& overlap)
{
    CVector2D start;
    CVector2D end;

    float dx = m_length * cos(m_ang);
    float dy = m_length * sin(m_ang);

    if (m_isLeft)
    {
        // 左：左端を回転軸にする
        start = m_pos;
        end.x = m_pos.x + dx;
        end.y = m_pos.y + dy;
    }
    else
    {
        // 右：右端を回転軸にする
        start = m_pos;
        end.x = m_pos.x - dx;
        end.y = m_pos.y - dy;
    }

    //line:斜め壁の端から端までのベクトル　斜め壁そのものからボールまでのベクトル
    CVector2D line = end - start;

    //toBall:斜め壁の左端からボールまでのベクトル
    CVector2D toBall = ballPos - start;

    //Dot:内積
    //t:斜め壁の線分上のどの位置にボールが近いかを表す値
    //Length() :ベクトルの長さ
    //LengthSq() :ベクトルの長さの2乗
    float t = toBall.Dot(line) / line.LengthSq();

    //tは、場合によってはマイナスや1より大きくなることがある
    //今回はtを必ず0～1の範囲に収める
    //これで線分の外にある場合も、最も近い端点を使って距離を計算できる
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    //最短距離になる点を求めている
    CVector2D closest = start + line * t;
    //diff:ボールと最短地点の距離を表すベクトル
    CVector2D diff = ballPos - closest;
    float distance = diff.Length();

    if (distance > 0.0f)//ボールが壁から離れている：壁からボールへ向かう方向を使う
    {
        normal = diff * (1.0f / distance);
    }
    else//ボールが壁の真上にいる：壁に垂直な方向を作る
    {
        normal.x = -line.y;
        normal.y = line.x;
        normal = normal * (1.0f / line.Length());
    }

    float hitRadius = ballRad + 16.0f;

    overlap = hitRadius - distance;

    return distance <= hitRadius;
}