#include "Menu.h"

void Menu::InitMenu(olc::PixelGameEngine& pge)
{

	r2sprite = new olc::Sprite("r2d2menu.png");
	r2Light = new olc::Sprite("r2d2lightmenu.png");
	d2sprite = new olc::Decal(r2sprite);
	d2Light = new olc::Decal(r2Light);
	sprGFX = new olc::Sprite("frontRetroMenu.png");

	mo["main"].SetTable(1, 4);
	
	mo["main"]["Saber"].SetTable(1, 2);

	mo["main"]["Saber"]["DarkSaber"].SetTable(1,2);
	mo["main"]["Saber"]["DarkSaber"]["InnerColor"].SetTable(2, 4);
	mo["main"]["Saber"]["DarkSaber"]["OuterColor"].SetTable(2, 4);
	auto& darksaberinner = mo["main"]["Saber"]["DarkSaber"]["InnerColor"];
	auto& darksaberouter = mo["main"]["Saber"]["DarkSaber"]["OuterColor"];
	//inner darsaber light
	darksaberinner["Black"].SetDarkInnerColor(olc::BLACK);
	darksaberinner["White"].SetDarkInnerColor(olc::WHITE);
	darksaberinner["Blue"].SetDarkInnerColor(olc::BLUE);
	darksaberinner["Green"].SetDarkInnerColor(olc::GREEN);
	darksaberinner["Red"].SetDarkInnerColor(olc::RED);
	darksaberinner["Purple"].SetDarkInnerColor(olc::MAGENTA);
	//outer darksaber light
	darksaberouter["Black"].SetDarkOuterColor(olc::BLACK);
	darksaberouter["White"].SetDarkOuterColor(olc::WHITE);
	darksaberouter["Blue"].SetDarkOuterColor(olc::BLUE);
	darksaberouter["Green"].SetDarkOuterColor(olc::GREEN);
	darksaberouter["Red"].SetDarkOuterColor(olc::RED);
	darksaberouter["Purple"].SetDarkOuterColor(olc::MAGENTA);



	mo["main"]["Saber"]["LightSaber"].SetTable(1, 2);
	mo["main"]["Saber"]["LightSaber"]["InnerColor"].SetTable(2, 4);
	mo["main"]["Saber"]["LightSaber"]["OuterColor"].SetTable(2, 4);
	auto& Lightsaberinner = mo["main"]["Saber"]["LightSaber"]["InnerColor"];
	auto& Lightsaberouter = mo["main"]["Saber"]["LightSaber"]["OuterColor"];
	//inner lightsaber light
	Lightsaberinner["Black"];
	Lightsaberinner["White"];
	Lightsaberinner["Blue"];
	Lightsaberinner["Green"];
	Lightsaberinner["Red"];
	Lightsaberinner["Purple"];
	//outer lightsaber light
	Lightsaberouter["Black"];
	Lightsaberouter["White"];
	Lightsaberouter["Blue"];
	Lightsaberouter["Green"];
	Lightsaberouter["Red"];
	Lightsaberouter["Purple"];

	mo["main"]["Character"];
	mo["main"]["Force Powers"];
	mo["main"]["index"];
	
	mo.Build();

	mm.Open(&mo["main"]);
	
	
}

void Menu::updateMenu(olc::PixelGameEngine& pge, Saber& saber)
{
	menuobject* command = nullptr;

	if (pge.GetKey(olc::Key::M).bPressed)    mm.Open(&mo["main"]);

	if (pge.GetKey(olc::Key::UP).bPressed)    mm.OnUp();
	if (pge.GetKey(olc::Key::DOWN).bPressed)  mm.OnDown();
	if (pge.GetKey(olc::Key::LEFT).bPressed)  mm.OnLeft();
	if (pge.GetKey(olc::Key::RIGHT).bPressed) mm.OnRight();
	if (pge.GetKey(olc::Key::SPACE).bPressed) command = mm.OnConfirm();
	if (pge.GetKey(olc::Key::Z).bPressed)     mm.OnBack();

	if (command != nullptr)
	{
		saber.darkinner = command->GetDarkInnerColor();
		saber.darkouter = command->GetDarkOuterColor();
		sLastAction = "Selected: " + command->GetName() + " ID: " + std::to_string(command->GetID());
		mm.Close();
	}

	
	mm.Draw(pge, sprGFX, { 30,30 });

	if (light < 0.80f)
	{
		lightswitch = true;
	}


	if (light >= 1.0f)
	{
		lightswitch = false;
	}

	if (lightswitch)
	{
		light += 0.05f;
	}
	else
	{
		light -= 0.05f;
	}
	//gfx->SetDecalMode(olc::DecalMode::ADDITIVE);
	pge.DrawDecal({ Pos.x,Pos.y },d2sprite, { 0.30f,0.30f });
	//pge.SetDecalMode(olc::DecalMode::NORMAL);
	pge.DrawDecal({ Pos.x,Pos.y }, d2Light, { 0.30f,0.30f }, p * light);

	pge.DrawString(10, 200, sLastAction);
}
