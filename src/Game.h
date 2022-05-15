#include "Entetys.h"
#include "Platform/Platform.hpp"
#include "charecters.h"

#define SFML_NO_DEPRECATED_WARNINGS

class Menu
{
public:
	Menu()
	{}

	void setSize(float Width, float Hight)
	{
		frame.setSize(sf::Vector2f(Width, Hight));
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
	}

private:
	sf::Texture Tframe;
	sf::RectangleShape frame;
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
	Game(float x, float y)
	{
		//Start Meny
		menuframe.setSize(x, y);

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

	void fire(bool fired)
	{
		if (fired == true)
		{
			bullet.Fired(ratchet.getBulletSpawn().x, ratchet.getBulletSpawn().y, ratchet.getRange().x, ratchet.getRange().y);
		}
		else
			bullet.setPosition(ratchet.getBulletSpawn().x, ratchet.getBulletSpawn().y);
	}
	bool hit(bool fired)
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
			else if ((ratchet.getRange().x > 0 && bullet.getPosition().x > ratchet.getRange().x) || (ratchet.getRange().x < 0 && bullet.getPosition().x < ratchet.getRange().x))
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

	void drawTitle(sf::RenderWindow& window)
	{
		menuframe.drawTitle(window);
	}
	void drawMenu(sf::RenderWindow& window)
	{
		menuframe.drawMenu(window);
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
	Menu menuframe;
	Level Demo;
	Ratchet ratchet;
	KPBullet bullet;
	//bool fired = false;
	Ssyphoid ss1, ss2, ss3;
};