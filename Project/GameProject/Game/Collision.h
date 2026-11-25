#pragma once
#include "Base/Base.h"

bool IsHitEdge(
    const CVector2D& start,//start, end：辺の両端
    const CVector2D& end,
    const CVector2D& ballPos,//ballPos：ボールの中心
    const CVector2D& center,//三角形の中心座標
    float ballRad,//ballRad：ボールの半径
    CVector2D& normal,//normal：押し戻す方向
    float& overlap,//overlap：めり込んだ距離
    CVector2D& closest
);