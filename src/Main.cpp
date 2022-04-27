#include "Entetys.h"
#include "Platform/Platform.hpp"
#include "charecters.h"
#include "rectanglebutton.h"
#include <iostream>
#include <math.h>

#define PI 3.14159265

using namespace sf;

bool left = false, right = false, jump = false;
float rvx = 0;
//int rvy = 0;
int jumpVal = 0;

bool fired = false;
bool bulletAlive = true;

int main()
{
	util::Platform platform;

	//skapande av obeject
	//skapa ett nytt fönster i variabeln window
	RenderWindow window(VideoMode(1920, 1080), "Ratchet & clank: a crack in time", Style::Fullscreen);
	platform.setIcon(window.getSystemHandle());
	//säter fps:en till 200
	window.setFramerateLimit(200);

	//hämta fönstrets storlek
	float x = window.getSize().x;
	float y = window.getSize().y;

	//level värde 0 = meny
	int level = 0;

	//START MENY
	//skapa två rektangel som bakgrund
	RectangleShape title(Vector2f(x, y));
	title.setFillColor(Color::White);

	RectangleShape mainmenu(Vector2f(x, y));
	mainmenu.setFillColor(Color::White);
	//säter en bild på rektanglarna
	Texture titleTexture;
	titleTexture.loadFromFile("content/levels/menu/title.png");
	title.setTexture(&titleTexture);
	Texture mainmenuTexture;
	mainmenuTexture.loadFromFile("content/levels/menu/main_menu.png");
	mainmenu.setTexture(&mainmenuTexture);

	//skapa en ny font och lada upp den
	Font font;
	font.loadFromFile("content/font/mini_pixel-7.ttf");
	//skapa start knapp
	RecButton start("START", { 185, 80 }, 100, Color::Transparent, Color::White);
	start.setPosition({ x / 2 - 350, y / 2 - 150 });
	start.setFont(font);
	//skapa load knapp
	//har ingen funktion i denna verision gjord för senare verisioner
	RecButton load("LOAD", { 150, 80 }, 100, Color::Transparent, Color::Black);
	load.setPosition({ x / 2 - 350, y / 2 - 60 });
	load.setFont(font);
	//skapa exit knapp
	RecButton exit("EXIT", { 150, 80 }, 100, Color::Transparent, Color::White);
	exit.setPosition({ x / 2 - 350, y / 2 + 30 });
	exit.setFont(font);

	//spela musik
	Music music;
	if (!music.openFromFile("content/levels/menu/menu.ogg"))
		return -1; // error
	music.play();
	music.setLoop(true);

	//Menu Timing Clock
	Clock clock;
	Time passedTime;


	//level obeject
	//Agorinan Battleplex texture
	Texture level1;
	level1.loadFromFile("content/levels/Agorian_battleplex/Agorian Battleplex.png");
	RectangleShape l1(Vector2f(level1.getSize().x, level1.getSize().y));
	l1.setFillColor(Color::White);
	l1.setPosition(0, 0);
	l1.setTexture(&level1);

	//obeject för mark colision detection
	RectangleShape l1m1(Vector2f(1991.f, 2.f));
	l1m1.setFillColor(Color::Transparent);
	l1m1.setPosition(64, 827);
	RectangleShape l1m2(Vector2f(1991.f, 4.f));
	l1m2.setFillColor(Color::Transparent);
	l1m2.setPosition(64, 829);
	//lava colision detection
	RectangleShape lava(Vector2f(window.getSize().x, 2.f));
	lava.setFillColor(Color::Transparent);
	lava.setPosition(0, 921);

	//Fiende: Liten Syphoid
	Ssyphoid ss1(0, 5, l1m1.getPosition().x, l1m1.getSize().x);
	Ssyphoid ss2(1, 5, l1m1.getPosition().x, l1m1.getSize().x);
	Ssyphoid ss3(2, 100, l1m1.getPosition().x, l1m1.getSize().x);

	//ratchet/spelare
	float startPosX = 980;
	float StartPosY = 550;
	Ratchet ratchet(startPosX, StartPosY);

	//start kordinater för projektil
	float BulletSpawnX = 0, BulletSpawnY = 0;
	Vertex BulletSpawn[2] = {
		Vertex(Vector2f(ratchet.getArmPosX(), ratchet.getArmPosY())),
		Vertex(Vector2f(ratchet.getArmPosX() + 150 * cos(ratchet.getArmRotation() * (PI / 180) + 0.71), ratchet.getArmPosY() + 150 * sin(ratchet.getArmRotation() * (PI / 180) + 0.71)))
	};
	BulletSpawn[0].color = Color::Blue;
	BulletSpawn[1].color = Color::Blue;
	KPBullet ett(BulletSpawn[1].texCoords.x, BulletSpawn[1].texCoords.y);

	//slut kordinater för projektil
	float BulletRangeDieX = 0, BulletRangeDieY = 0;
	Vertex Range[2] = {
		Vertex(Vector2f(ratchet.getArmPosX(), ratchet.getArmPosY())),
		Vertex(Vector2f(ratchet.getArmPosX() + 500 * cos(ratchet.getArmRotation() * (PI / 180) + 0.71), ratchet.getArmPosY() + 550 * sin(ratchet.getArmRotation() * (PI / 180) + 0.71)))
	};
	Range[0].color = Color::Red;
	Range[1].color = Color::Red;

	while (window.isOpen())
	{
		//tid
		passedTime += clock.restart();

		Event event;
		//loopa när något Event sker
		//sät inte animationer här för detta sker när du rör på musen eller tryker på en knap
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				case Event::Closed:
					window.close();
					break;
				case Event::MouseMoved:
					//när musen hovrar över knaparna ändra färg
					if (start.isMouseOver(window))
					{
						start.setBackColor(Color::Green);
					}
					else
					{
						start.setBackColor(Color::White);
					}
					if (load.isMouseOver(window))
					{
						load.setBackColor(sf::Color::Red);
					}
					else
					{
						load.setBackColor(Color::Black);
					}
					if (exit.isMouseOver(window))
					{
						exit.setBackColor(Color::Green);
					}
					else
					{
						exit.setBackColor(Color::White);
					}

				default:
					break;
			}

			window.display();

			switch (event.key.code)
			{
				case Mouse::Left:
					//sätt funktion på knaparna
					if (start.isMouseOver(window))
					{
						//starta leveln när start knapen trycks
						level = +1;
					}
					else if (load.isMouseOver(window))
					{
						//ska ha en funktion att lada spar filer i senare verisioner
					}
					else if (exit.isMouseOver(window))
					{
						//stäng ned spelet när exit knapen trycks
						window.close();
					}
					else if (level == 1)
					{
						//spara start och slut kordinaterna för projektilen
						BulletSpawnX = BulletSpawn[1].position.x;
						BulletSpawnY = BulletSpawn[1].position.y;
						BulletRangeDieX = Range[1].position.x;
						BulletRangeDieY = Range[1].position.y;
						//sät sanings värdet fired till sant för senare i kåden
						fired = true;
					}

				default:
					break;
			}
			if (event.type == Event::KeyPressed)
			{
				//sät saningsvärdena left, right till sanna och jumpVal till 1 om respective D A W trycks ned
				if (event.key.code == Keyboard::D)
				{
					right = true;
					rvx = 4;
				}
				else
				{
					right = false;
					rvx = 0;
				}
				if (event.key.code == Keyboard::A)
				{
					left = true;
					rvx = -4;
				}
				else
				{
					left = false;
					rvx = 0;
				}
				if (event.key.code == Keyboard::W)
				{
					jumpVal = 1;
				}

				//gå tilbaka en nivå
				if (event.key.code == Keyboard::Escape)
				{
					level -= 1;
				}
			}
		}

		//här loopas allt efter framelimitorn så här skrivs allt som animeras eller sker utannågon input
		if (level == 1)
		{
			//gravitation och kolisions detection
			if (ratchet.getGlobalBounds().intersects(l1m1.getGlobalBounds()))
			{
				ratchet.move(0.f, 0.f);
			}
			else
			{
				ratchet.move(0.f, 2.f);
			}
			if (ratchet.getGlobalBounds().intersects(l1m2.getGlobalBounds()))
			{
				ratchet.move(0.f, -2.f);
			}

			//förflyta positionen för projectilens start kordinat
			BulletSpawn[0].position = Vector2f(ratchet.getArmPosX(), ratchet.getArmPosY());
			BulletSpawn[1].position = Vector2f(ratchet.getArmPosX() + 150 * cos(ratchet.getArmRotation() * (PI / 180) + 0.715), ratchet.getArmPosY() + 150 * sin(ratchet.getArmRotation() * (PI / 180) + 0.715));
			//förflyta positionen för projectilens slut kordinat
			Range[0].position = Vector2f(ratchet.getArmPosX(), ratchet.getArmPosY());
			Range[1].position = Vector2f(ratchet.getArmPosX() + 750 * cos(ratchet.getArmRotation() * (PI / 180) + 0.715), ratchet.getArmPosY() + 750 * sin(ratchet.getArmRotation() * (PI / 180) + 0.715));

			//rotera karaktärens arm
			if (event.type == Event::MouseMoved)
			{
				ratchet.RotateArm();
			}

			// stara Syphoidernas AI
			ss1.AI(ratchet.getPositionX(), ratchet.getSizeX());
			ss2.AI(ratchet.getPositionX(), ratchet.getSizeX());
			ss3.AI(ratchet.getPositionX(), ratchet.getSizeX());
		}

		//förflytning av karaktären höger, vänster och upp
		if (left == true)
		{
			ratchet.move(-4.f, 0.f);
			left = false;
		}
		if (right == true)
		{
			ratchet.move(4, 0.f);
			right = false;
		}
		if (jumpVal > 0 && jumpVal < 50)
		{
			if (left == true)
			{
				ratchet.move(-4.f, -4.f);
			}
			else if (right == true)
			{
				ratchet.move(4.f, -4.f);
			}
			else
				ratchet.move(0.f, -4.f);

			jumpVal++;
		}

		//skande colision detection
		if (ratchet.getHitBox().intersects(lava.getGlobalBounds()) ||ratchet.getHitBox().intersects(ss1.getGlobalBounds()) || ratchet.getHitBox().intersects(ss2.getGlobalBounds()) || ratchet.getHitBox().intersects(ss3.getGlobalBounds()))
		{
			ratchet.Damage();
		}

		//Förflyta kameran
		if (ratchet.getPositionX() < 400 && l1.getPosition().x < 0)
		{
			//flyta leveln
			l1.move(1.f, 0.f);
			l1m1.move(1.f, 0.f);
			l1m2.move(1.f, 0.f);

			ratchet.move(1.f, 0.f);
			ss1.move(1.f, 0.f);
			ss2.move(1.f, 0.f);
		}
		else if (ratchet.getPositionX() > 1400 && l1.getPosition().x > -200)
		{
			//flyta leveln
			l1.move(-1.f, 0.f);
			l1m1.move(-1.f, 0.f);
			l1m2.move(-1.f, 0.f);

			ratchet.move(-1.f, 0.f);
			ss1.move(-1.f, 0.f);
			ss2.move(-1.f, 0.f);
		}

		//skot kolision
		if (fired == true)
		{
			ett.Fired(BulletSpawnX, BulletSpawnY, BulletRangeDieX, BulletRangeDieY);
			if (ett.getGlobalBounds().intersects(ss1.getGlobalBounds()))
			{
				ss1.DamageTaken(ett.power());
				fired = false;
				ett.setPosition(BulletSpawn[1].position.x, BulletSpawn[1].position.y);
			}
			if (ett.getGlobalBounds().intersects(ss2.getGlobalBounds()))
			{
				ss2.DamageTaken(ett.power());
				fired = false;
				ett.setPosition(BulletSpawn[1].position.x, BulletSpawn[1].position.y);
			}
			if (ett.getGlobalBounds().intersects(ss3.getGlobalBounds()))
			{
				ss3.DamageTaken(ett.power());
				fired = false;
				ett.setPosition(BulletSpawn[1].position.x, BulletSpawn[1].position.y);
			}
			if ((BulletRangeDieX > 0 && ett.getPosition().x > BulletRangeDieX) || (BulletRangeDieX < 0 && ett.getPosition().x < BulletRangeDieX))
			{
				fired = false;
				ett.setPosition(BulletSpawn[1].position.x, BulletSpawn[1].position.y);
			}
		}
		else
			ett.setPosition(BulletSpawn[1].position.x, BulletSpawn[1].position.y);

		//rita upp på skärmen
		window.clear(Color::Black);
		if (passedTime <= seconds(4) && level == 0)
			window.draw(title);
		else if (level == 0)
		{
			window.clear();
			window.draw(mainmenu);
			start.drawTo(window);
			load.drawTo(window);
			exit.drawTo(window);
		}
		else if (level == 1)
		{
			window.clear(Color::White);
			window.draw(l1);
			window.draw(l1m1);
			window.draw(l1m2);
			window.draw(lava);

			ett.drawTo(window);
			ratchet.drawTo(window);

			ss1.drawTo(window);
			ss2.drawTo(window);
			ss3.drawTo(window);
		}
		window.display();
	}
	return 0;
}