#include "Game/Game.h"
#include "Game/Player.h"
#include "Game/Wall.h"
#include "Game/Flipper.h"
#include "Title/Result.h"

Game::Game() :Base(eType_Scene)
{
	//new Player(CVector2D(860, 700));
	new Player(CVector2D(620, 400));
	new Wall(CVector2D(600, 500));
	new Flipper(CVector2D(610, 900),true);
	new Flipper(CVector2D(790, 900), false);
}

void::Game::Update()
{
    //’¼ÚƒŠƒUƒ‹ƒg‚ğo‚·
    if (PUSH(CInput::eButton2)) {
        if (!Base::FindObject(eType_Result)) {
            Base::KillAll();
            new Result();
        }
        return;
    }
}