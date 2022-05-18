#include "Entetys.h"
#include "Platform/Platform.hpp"
#include "charecters.h"
#include "rectanglebutton.h"

#define SFML_NO_DEPRECATED_WARNINGS

class Menu
{
public:
	Menu(float Width, float Hight)
	{
		frame.setSize(sf::Vector2f(Width, Hight));
		//skapa start knapp
		start.setings("START", { 185, 80 }, 100, sf::Color::Transparent, sf::Color::White);
		start.setPosition({ Width / 2 - 350, Hight / 2 - 150 });
		//skapa load knapp
		//har ingen funktion i denna verision gjord för senare verisioner
		load.setings("LOAD", { 150, 80 }, 100, sf::Color::Transparent, sf::Color::Black);
		load.setPosition({ Width / 2 - 350, Hight / 2 - 60 });
		//skapa exit knapp
		exit.setings("EXIT", { 150, 80 }, 100, sf::Color::Transparent, sf::Color::White);
		exit.setPosition({ Width / 2 - 350, Hight / 2 + 30 });
	}

	void setFont(sf::Font& font)
	{
		start.setFont(font);
		load.setFont(font);
		exit.setFont(font);
	}

	void onHover(sf::RenderWindow& window)
	{
		//när musen hovrar över knaparna ändra färg
		if (start.isMouseOver(window))
		{
			start.setBackColor(sf::Color::Green);
		}
		else
		{
			start.setBackColor(sf::Color::White);
		}
		if (load.isMouseOver(window))
		{
			load.setBackColor(sf::Color::Red);
		}
		else
		{
			load.setBackColor(sf::Color::Black);
		}
		if (exit.isMouseOver(window))
		{
			exit.setBackColor(sf::Color::Green);
		}
		else
		{
			exit.setBackColor(sf::Color::White);
		}
	}

	int onClik(sf::RenderWindow& window,int& level)
	{
		//sätt funktion på knaparna
		if (start.isMouseOver(window))
		{
			//starta leveln när start knapen trycks
			return 1;
		}
		else if (load.isMouseOver(window))
		{
			//ska ha en funktion att lada spar filer i senare verisioner
			return 0;
		}
		else if (exit.isMouseOver(window))
		{
			//stäng ned spelet när exit knapen trycks
			window.close();
			return 0;
		}
		return level;
	}

	void drawTitle(sf::RenderWindow& window)
	{
		//säter en bild på rektangeln
		Tframe.loadFromFile("content/levels/menu/title.png");
		frame.setTexture(&Tframe);

		window.draw(frame);
	}
	void drawMenu(sf::RenderWindow& window)
	{
		//säter en bild på rektangeln
		Tframe.loadFromFile("content/levels/menu/main_menu.png");
		frame.setTexture(&Tframe);

		window.draw(frame);
		start.drawTo(window);
		load.drawTo(window);
		exit.drawTo(window);
	}

private:
	sf::Texture Tframe;
	sf::RectangleShape frame;
	RecButton start, load, exit;
	sf::Event event;
};

class Level
{
public:
	Level()
	{
		Tframe.loadFromFile("content/levels/Agorian_battleplex/Agorian Battleplex.png");
		frame.setSize(sf::Vector2f(Tframe.getSize().x, Tframe.getSize().y));
		frame.setPosition(0, 0);
		//säter en bild på rektangeln
		frame.setTexture(&Tframe);

		//obeject för mark colision detection
		mark1.setSize(sf::Vector2f(1991.f, 2.f));
		mark1.setPosition(64, 827);
		mark2.setSize(sf::Vector2f(1991.f, 4.f));
		mark2.setPosition(64, 829);

		//lava colision detection
		lava.setPosition(0, 921);
	}

	void setSize(float windowSize)
	{
		lava.setSize(sf::Vector2f(windowSize, 2.f));
	}

	sf::Vector2f getPosition() const
	{
		return frame.getPosition();
	}

	void move(sf::Vector2f value)
	{
		frame.move(value);
		mark1.move(value);
		mark2.move(value);
	}

	void drawLevel(sf::RenderWindow& window)
	{
		window.draw(frame);
	}

	sf::Vector2f getGroundPosition() const
	{
		return mark1.getPosition();
	}

	sf::Vector2f getGroundSize() const
	{
		return mark1.getSize();
	}

	sf::FloatRect getGlobalBoundsMark1() const
	{
		return mark1.getGlobalBounds();
	}
	sf::FloatRect getGlobalBoundsMark2() const
	{
		return mark2.getGlobalBounds();
	}

	sf::FloatRect getGlobalBoundsLava() const
	{
		return lava.getGlobalBounds();
	}

private:
	sf::Texture Tframe;
	sf::RectangleShape frame;

	sf::RectangleShape mark1;
	sf::RectangleShape mark2;

	sf::RectangleShape lava;
};

class Game
{
public:
	Game(float x)
	{
		//level obeject
		//Agorinan Battleplex demo
		Demo.setSize(x);
		//Spelare(Ratchet)
		ratchet.StartPos(sf::Vector2f(980.f, 550.f));
		//projektil
		bullet.setPosition(ratchet.getBulletSpawn().x, ratchet.getBulletSpawn().y);
		//Fiende: Liten Syphoid
		ss1.setings(0, 20, Demo.getGroundPosition().x, Demo.getGroundSize().x);
		ss2.setings(1, 20, Demo.getGroundPosition().x, Demo.getGroundSize().x);
		ss3.setings(2, 20, Demo.getGroundPosition().x, Demo.getGroundSize().x);
	}

	void markcolision()
	{
		//gravitation och kolisions detection
		if (ratchet.getGlobalBounds().intersects(Demo.getGlobalBoundsMark1()))
		{
			ratchet.move(0.f, 0.f);
		}
		else
		{
			ratchet.move(0.f, 2.f);
			ratchet.RotateArm();
		}
		if (ratchet.getGlobalBounds().intersects(Demo.getGlobalBoundsMark2()))
		{
			ratchet.move(0.f, -2.f);
			ratchet.RotateArm();
		}
	}
	//skande colision detection
	void takeDamage()
	{
		if (ratchet.getHitBox().intersects(Demo.getGlobalBoundsLava()) || ratchet.getHitBox().intersects(ss1.getGlobalBounds()) || ratchet.getHitBox().intersects(ss2.getGlobalBounds()) || ratchet.getHitBox().intersects(ss3.getGlobalBounds()))
		{
			ratchet.Damage();
		}
	}

	void movelevel()
	{
		//Förflyta kameran
		if (ratchet.getPosition().x < 400 && Demo.getPosition().x < 0)
		{
			//flyta leveln
			Demo.move(sf::Vector2f(1.f, 0.f));

			ratchet.move(1.f, 0.f);
			ss1.move(1.f, 0.f);
			ss2.move(1.f, 0.f);
		}
		else if (ratchet.getPosition().x > 1400 && Demo.getPosition().x > -200)
		{
			//flyta leveln
			Demo.move(sf::Vector2f(-1.f, 0.f));

			ratchet.move(-1.f, 0.f);
			ss1.move(-1.f, 0.f);
			ss2.move(-1.f, 0.f);
		}
	}
	void SyphoidernasAI()
	{
		//stara Syphoidernas AI
		ss1.AI(ratchet.getPosition().x, ratchet.getSize().x);
		ss2.AI(ratchet.getPosition().x, ratchet.getSize().x);
		ss3.AI(ratchet.getPosition().x, ratchet.getSize().x);
	}

	sf::Vector2f getBulletRange() const
	{
		return ratchet.getRange();
	}
	void fire(bool fired, float bulletendX, float bulletendY)
	{
		if (fired == true)
		{
			bullet.Fired(ratchet.getBulletSpawn().x, ratchet.getBulletSpawn().y, bulletendX, bulletendY);
		}
	}
	bool hit(bool fired, float bulletendX)
	{
		if (fired == true)
		{
			if (bullet.getGlobalBounds().intersects(ss1.getGlobalBounds()))
			{
				ss1.DamageTaken(bullet.power());
				fired = false;
				bullet.setPosition(ratchet.getBulletSpawn().x, ratchet.getBulletSpawn().y);
			}
			else if (bullet.getGlobalBounds().intersects(ss2.getGlobalBounds()))
			{
				ss2.DamageTaken(bullet.power());
				fired = false;
				bullet.setPosition(ratchet.getBulletSpawn().x, ratchet.getBulletSpawn().y);
			}
			else if (bullet.getGlobalBounds().intersects(ss3.getGlobalBounds()))
			{
				ss3.DamageTaken(bullet.power());
				fired = false;
				bullet.setPosition(ratchet.getBulletSpawn().x, ratchet.getBulletSpawn().y);
			}
			else if ((bulletendX > 0 && bullet.getPosition().x > bulletendX) || (bulletendX < 0 && bullet.getPosition().x < bulletendX))
			{
				ss2.DamageTaken(bullet.power());
				fired = false;
				bullet.setPosition(ratchet.getBulletSpawn().x, ratchet.getBulletSpawn().y);
			}
		}
		else
			bullet.setPosition(ratchet.getBulletSpawn().x, ratchet.getBulletSpawn().y);
		return fired;
	}

	void MoveRatchet(float Vx, float Vy)
	{
		ratchet.move(Vx, Vy);
		ratchet.RotateArm();
	}
	void RotateRatchetArm()
	{
		ratchet.RotateArm();
	}

	void drawLevel(sf::RenderWindow& window)
	{
		Demo.drawLevel(window);
	}
	void drawBullet(sf::RenderWindow& window)
	{
		bullet.drawTo(window);
	}
	void drawRatchet(sf::RenderWindow& window)
	{
		ratchet.drawTo(window);
	}
	void drawEnemys(sf::RenderWindow& window)
	{
		ss1.drawTo(window);
		ss2.drawTo(window);
		ss3.drawTo(window);
	}

private:
	float x, y;
	Level Demo;
	Ratchet ratchet;
	KPBullet bullet;
	Ssyphoid ss1, ss2, ss3;
	sf::Event input;
};