#include "Base/Base.h"
#include "Game/Flipper.h"

Flipper::Flipper(const CVector2D& pos,bool isLeft) :Base(eType_Flipper)
{
	m_img = COPY_RESOURCE("Flipper", CImage);
	m_pos = pos;
	m_img.SetCenter(80, 16);
	//m_img.SetSize(1000, 1000);
	m_isLeft = isLeft;

    if (m_isLeft)
    {
        m_ang = 0.52f;
    }
    else
    {
        m_ang = -0.52f;
    }

    m_isPressed = false;

}
void Flipper::Update()
{
    //6:A 7:D
    if (m_isLeft)
    {
        if (HOLD(CInput::eButton6))
        {
            m_isPressed = true;

            // 回転させる
            m_ang = -0.79f;
        }
        else
        {
            m_isPressed = false;

            // 元の角度に戻す
            m_ang = 0.52f;
        }
    }
    else
    {
        if (HOLD(CInput::eButton7))
        {
            // 回転させる
            m_ang = 0.79f;
        }
        else
        {
            // 元の角度に戻す
            m_ang = -0.52f;
        }
    }
    m_img.SetAng(-m_ang);
}
void Flipper::Draw()
{
	m_img.SetPos(m_pos);
	m_img.Draw();
}
bool Flipper::IsHit(const CVector2D& ballPos, CVector2D& normal, CVector2D& tangent, float& overlap)
{
    CVector2D start;
    CVector2D end;

    float dx = 80 * cos(m_ang);
    float dy = 80 * sin(m_ang);

    start.x = m_pos.x - dx;
    start.y = m_pos.y - dy;

    end.x = m_pos.x + dx;
    end.y = m_pos.y + dy;

    //line:フリッパーの端から端までのベクトル　フリッパーそのものからボールまでのベクトル
    CVector2D line = end - start;

    //tangent はフリッパーの長さ方向を向く、長さ1のベクトル ボールがフリッパーに沿って動くための方向
    tangent = line * (1.0f / line.Length());

    //toBall:フリッパーの左端からボールまでのベクトル
    CVector2D toBall = ballPos - start;

    //Dot:内積
    //t:フリッパーの線分上のどの位置にボールが近いかを表す値
    //Length() :ベクトルの長さ
    //LengthSq() :ベクトルの長さの2乗
    float t = toBall.Dot(line) / line.LengthSq();

    //tは、場合によってはマイナスや1より大きくなることがある
    //今回はtを必ず0～1の範囲に収める
    //これで線分の外にある場合も、最も近い端点を使って距離を計算できる
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    //最短距離になるか点を求めている
    CVector2D closest = start + line * t;
    //diff:ボールと最短地点の距離を表すベクトル
    CVector2D diff = ballPos - closest;
    float distance = diff.Length();

    overlap = 24.0f - distance;

    //overlap：ボールが判定範囲にどれだけ入り込んだか
    //normal：ボールを押し戻す方向
    normal.x = -tangent.y;
    normal.y = tangent.x;

    if (normal.y > 0.0f)
    {
        normal = normal * -1.0f;
    }

    //距離を2乗して比較
    return diff.LengthSq() <= 24 * 24;
}