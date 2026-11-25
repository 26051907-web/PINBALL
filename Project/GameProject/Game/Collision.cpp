#include "Base/Base.h"
#include "Game/Collision.h"

bool IsHitEdge(
    const CVector2D& start,
    const CVector2D& end,
    const CVector2D& ballPos,
    const CVector2D& center,
    float ballRad,
    CVector2D& normal,//normal：衝突時に使う単位法線ベクトル
    float& overlap,
    CVector2D& closest)
{
    //長さ0のベクトルを使って割り算すると問題が起きる ベクトルの長さが0かどうか調べている
    CVector2D line = end - start;//line：辺に沿った方向

    if (line.LengthSq() == 0.0f)
    {
        return false;
    }
    //

    //始点からボール中心へのベクトル
    CVector2D toBall = ballPos - start;
    //t:辺の始点から終点までの、どのあたりに近いかを表す割合
    float t = toBall.Dot(line) / line.LengthSq();
    //tを0～1に収める
    t = max(0.0f, min(1.0f, t));
    closest = start + line * t;
    //最近点からボール中心へのベクトル
    CVector2D diff = ballPos - closest; //diff：最近接点からボールへの方向

    if (diff.Length() == 0.0f)
    {
        // diffが0のときの専用法線
        normal = CVector2D(line.y, -line.x);//辺に対して垂直なベクトルを作っている
        normal = normal / normal.Length();//法線を長さ1にする 正規化
    }
    else
    {
        // 通常の法線
        normal = diff / diff.Length();
    }

    //法線が内向きだった場合、反転して外向きにできる
    CVector2D toCenter = center - closest;

    if (normal.Dot(toCenter) > 0.0f)
    {
        normal *= -1.0f;
    }

    //ボール中心と辺の距離が半径より小さければ、めり込んでいる
    if (diff.Length() < ballRad)
    {
        //ボールが辺にどれだけめり込んでいるか計算
        overlap = ballRad - diff.Length();

        return true;
    }
    return false;

}