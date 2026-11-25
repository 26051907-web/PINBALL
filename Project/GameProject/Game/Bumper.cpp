#include "Base/Base.h"
#include "Game/Bumper.h"
#include "Game/Collision.h"

Bumper::Bumper(const CVector2D& pos) :Base(eType_Bumper)
{
    m_img = COPY_RESOURCE("Bumper", CImage);

    m_pos = pos;
    m_img.SetSize(128, 128);
    m_img.SetCenter(64, 64);

    // 三角形の頂点
    m_a = CVector2D(pos.x - 64, pos.y - 64);//左下 頂点
    m_b = CVector2D(pos.x - 64, pos.y + 64);//左上 頂点
    m_c = CVector2D(pos.x + 64, pos.y);//右中央 頂点
}
void Bumper::Update()
{
   
}
void Bumper::Draw()
{
    m_img.SetPos(m_pos);
    m_img.Draw();
}
bool IsInside(
    CVector2D p,
    CVector2D a,
    CVector2D b,
    CVector2D c)
{
    //AB辺の外積
    CVector2D ab = b - a;
    CVector2D ap = p - a;

    float cross = ab.x * ap.y - ab.y * ap.x;
    //BC辺の外積
    CVector2D bc = c - b;
    CVector2D bp = p - b;

    float cross2 = bc.x * bp.y - bc.y * bp.x;
    //CA辺の外積
    CVector2D ca = a - c;
    CVector2D cp = p - c;

    float cross3 = ca.x * cp.y - ca.y * cp.x;

    if ((cross >= 0 && cross2 >= 0 && cross3 >= 0 )||
        ( cross <= 0 && cross2 <= 0 && cross3 <= 0))
    {
        return true;
    }
    return false;
}
bool Bumper::IsHit(
    const CVector2D& ballPos,
    float ballRad,
    CVector2D& normal,
    float& overlap)
{
    // 最近接点を保存する変数
    CVector2D closestAB;
    CVector2D closestBC;
    CVector2D closestCA; //この3つは関数の中だけで使う一時的な変数だから、メンバー変数にする必要はない

    //3辺それぞれの法線を保存する変数
    CVector2D normalAB;
    CVector2D normalBC;
    CVector2D normalCA;

    //辺ごとのめり込み量
    float overlapAB;
    float overlapBC;
    float overlapCA;

    bool hitAB = IsHitEdge(
        m_a, m_b, ballPos, m_pos,
        ballRad, normalAB, overlapAB, closestAB
    );
    bool hitBC = IsHitEdge(
        m_b, m_c, ballPos, m_pos,
        ballRad, normalBC, overlapBC, closestBC
    );
    bool hitCA = IsHitEdge(
        m_c, m_a, ballPos, m_pos,
        ballRad, normalCA, overlapCA, closestCA
    );

    if (hitAB)//AB辺に接触しているか
    {
        normal = normalAB;//AB辺の法線を採用
        overlap = overlapAB;//AB辺のめり込み量を採用
        return true;//衝突したことを返す
    }
    if (hitBC)
    {
        normal = normalBC;
        overlap = overlapBC;
        return true;
    }
    if (hitCA)
    {
        normal = normalCA;
        overlap = overlapCA;
        return true;
    }

    if (IsInside(ballPos, m_a, m_b, m_c))// ボールが三角形の内側にいる
    {
        //各辺までの距離を計算
        float distAB = (ballPos - closestAB).LengthSq();
        float distBC = (ballPos - closestBC).LengthSq();
        float distCA = (ballPos - closestCA).LengthSq();

        //一番近い辺を判定する
        if (distAB <= distBC && distAB <= distCA)
        {
            normal = normalAB;
            overlap = sqrt(distAB) + ballRad;
        }
        else if (distBC <= distCA && distBC <= distAB)
        {
            normal = normalBC;
            overlap = sqrt(distBC) + ballRad;
        }
        else if (distCA <= distAB && distCA <= distBC)
        {
            normal = normalCA;
            overlap = sqrt(distCA) + ballRad;
        }
        printf("Player Pos: %f, %f\n", ballPos.x, ballPos.y);
        printf("Normal: %f, %f\n", normal.x, normal.y);
        printf("Overlap: %f\n", overlap);

        return true;
    }

    //どの辺にも接触していなかった場合
    return false;
}