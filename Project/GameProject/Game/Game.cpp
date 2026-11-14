#include "Game/Game.h"
#include "Game/Player.h"
#include "Game/Wall.h"
#include "Game/Flipper.h"
#include "Title/Result.h"

Game::Game() :Base(eType_Scene)
{
	//new Player(CVector2D(860, 700));
	new Player(CVector2D(640, 400));

    new Wall(CVector2D(545, 300));
	new Wall(CVector2D(545, 500));
    new Wall(CVector2D(545, 700));
    new Wall(CVector2D(545, 900));

    new Wall(CVector2D(980, 300));
    new Wall(CVector2D(980, 500));
    new Wall(CVector2D(980, 700));
    new Wall(CVector2D(980, 900));

	new Flipper(CVector2D(612, 900),true);
	new Flipper(CVector2D(909, 900), false);
}

void::Game::Update()
{
    //íºê⁄ÉäÉUÉãÉgÇèoÇ∑
    if (PUSH(CInput::eButton2)) {
        if (!Base::FindObject(eType_Result)) {
            Base::KillAll();
            new Result();
        }
        return;
    }
}