#include "Base/Base.h"
#include "Game/Wall.h"

Wall::Wall(const CVector2D& pos) :Base(eType_Wall)
{
	m_img = COPY_RESOURCE("Wall", CImage);
	m_pos = pos;
	m_img.SetCenter(225, 800);
	m_img.SetSize(450, 1600);

}
Wall::Wall(const CVector2D& pos, float width, float height)
    :Base(eType_Wall)
{
    m_img = COPY_RESOURCE("Wall", CImage);
    m_pos = pos;

    //画像の中心
    m_img.SetCenter(225, 800);

    //画像サイズ
    m_img.SetSize(width, height);

    //当たり判定サイズ（中心から端まで）
    m_halfWidth = width / 2;
    m_halfHeight = height / 2;
}
void Wall::Update()
{

}
void Wall::Draw()
{
	m_img.SetPos(m_pos);
	m_img.Draw();
}