#include "Entetys.h"
#include "Game.h"
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

	Game game(window.getSize().x, window.getSize().y);
	//hämta fönstrets storlek
	float x = window.getSize().x;
	float y = window.getSize().y;

	//level värde 0 = meny
	int level = 0;

	//spela musik
	Music music;
	if (!music.openFromFile("content/levels/menu/menu.ogg"))
		return -1; // error
	music.play();
	music.setLoop(true);

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

	//Menu Timing Clock
	Clock clock;
	Time passedTime;

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
			game.markcolision();
			game.takeDamage();
			//rotera karaktärens arm
			if (event.type == Event::MouseMoved)
			{
				game.RotateRatchetArm();
			}
			game.SyphoidernasAI();
		}

		//förflytning av karaktären höger, vänster och upp
		if (left == true)
		{
			game.MoveRatchet(-4.f, 0.f);
			left = false;
		}
		if (right == true)
		{
			game.MoveRatchet(4, 0.f);
			right = false;
		}
		if (jumpVal > 0 && jumpVal < 50)
		{
			if (left == true)
			{
				game.MoveRatchet(-4.f, -4.f);
			}
			else if (right == true)
			{
				game.MoveRatchet(4.f, -4.f);
			}
			else
				game.MoveRatchet(0.f, -4.f);

			jumpVal++;
		}

		//skot kolision
		game.fire(fired);
		fired =game.hit(fired);

		game.movelevel();
		//rita upp på skärmen
		window.clear(Color::Black);
		if (passedTime <= seconds(4) && level == 0)
			game.drawTitle(window);
		else if (level == 0)
		{
			window.clear();
			game.drawMenu(window);
			start.drawTo(window);
			load.drawTo(window);
			exit.drawTo(window);
		}
		else if (level == 1)
		{
			window.clear(Color::White);
			game.drawLevel(window);

			game.drawBullet(window);
			game.drawRatchet(window);

			game.drawEnemys(window);
		}
		window.display();
	}
	return 0;
}