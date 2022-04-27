#include "Platform/Platform.hpp"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SFML_NO_DEPRECATED_WARNINGS
#define PI 3.14159265

class Ratchet
{
public:
	//Ratchet()
	//{}
	int hp = 3;
	float RSPX, RSPY;
	Ratchet(float SPX, float SPY)
	{
		RSPX = SPX;
		RSPY = SPY;
		float rsx = 120;
		float rsy = 240;

		RBt.loadFromFile("content/charecters/ratchet/ratchet1.png");
		RAt.loadFromFile("content/charecters/ratchet/ratchet arm kp.png");
		player.setTexture(&RBt);
		arm.setTexture(&RAt);

		player.setPosition(sf::Vector2f(SPX, SPY));
		player.setSize({ rsx, rsy });

		collision.setPosition(player.getPosition().x + (player.getSize().x) / 2 + 12, player.getPosition().y);
		collision.setSize({ 8, player.getSize().y });
		collision.setFillColor(sf::Color::Yellow);

		arm.setPosition((player.getPosition().x + (player.getSize().x) / 2 - 4) - 8 * 2 + 25, player.getPosition().y + 8 * 5 + 24);
		arm.setSize({ 152, 136 });
		arm.setOrigin(25, 24);
		arm.setRotation(atan2(sf::Mouse::getPosition().y - arm.getPosition().y, sf::Mouse::getPosition().x - arm.getPosition().x) * 180 / PI * -1);
	}

	void StartPos(sf::Vector2f pos)
	{
		player.setPosition(pos);
	}

	float getArmPosX(void)
	{
		return arm.getPosition().x;
	}
	float getArmPosY(void)
	{
		return arm.getPosition().y;
	}
	void RotateArm(void)
	{
		arm.setRotation(atan2(sf::Mouse::getPosition().x - arm.getPosition().x, sf::Mouse::getPosition().y - arm.getPosition().y) * 180 / PI * -1 + 49);
	}
	float getArmRotation(void)
	{
		return arm.getRotation();
	}

	void Damage(void)
	{
		hp--;
		if (hp < -3)
		{
			player.setPosition(sf::Vector2f(RSPX, RSPY));
			collision.setPosition(player.getPosition().x + (player.getSize().x) / 2 + 12, player.getPosition().y);
			arm.setPosition((player.getPosition().x + (player.getSize().x) / 2 - 4) - 8 * 2 + 25, player.getPosition().y + 8 * 5 + 24);
			hp = 3;
		}
	}

	void move(float moveX, float moveY)
	{
		player.move(moveX, moveY);
		collision.move(moveX, moveY);
		arm.move(moveX, moveY);
	}

	float getPositionX(void)
	{
		return player.getPosition().x;
	}
	float getPositionY(void)
	{
		return player.getPosition().y;
	}
	float getSizeX(void)
	{
		return player.getSize().x;
	}
	float getSizeY(void)
	{
		return player.getSize().y;
	}

	sf::FloatRect getHitBox(void) const
	{
		return player.getGlobalBounds();
	}
	sf::FloatRect getGlobalBounds(void) const
	{
		return collision.getGlobalBounds();
	}

	void drawTo(sf::RenderWindow& window)
	{
		if (hp > 0)
		{
			window.draw(player);
			window.draw(collision);
			window.draw(arm);
			//window.draw(BulletSpawn, 2, sf::Lines);
		}
	}

private:
	sf::RectangleShape player;
	sf::RectangleShape collision;
	sf::RectangleShape arm;
	sf::Texture RBt;
	sf::Texture RAt;
};

class Ssyphoid
{
public:
	Ssyphoid()
	{}

	int MinPos;
	int MaxPos;

	Ssyphoid(int Time, int hp, int minX , int maxX)
	{
		this->hp = hp;
		this->MinPos = minX;
		this->MaxPos = maxX - 249;
		sf::Clock clock;
		sf::Time passedTime;
		if (passedTime == sf::seconds(Time))
		{
			srand(time(NULL));
		}

		Sst.loadFromFile("content/charecters/zyphoid/Small_syphoid.png");
		enemy.setSize(sf::Vector2f(Sst.getSize().x, Sst.getSize().y));
		enemy.setPosition(rand() % minX + 200, 732);
		enemy.setFillColor(sf::Color::White);
		enemy.setTexture(&Sst);
	}
	void setPosition(sf::Vector2f pos)
	{
		enemy.setPosition(pos);
	}
	sf::FloatRect getGlobalBounds(void) const
	{
		if (hp > 0)
		{
			return enemy.getGlobalBounds();
		}
		else
			return enemy.getLocalBounds();
	}
	void AI(int RPos, int RWidth)
	{
		if (this->LR == 1)
		{
			if (this->i <= this->steg)
			{
				if (RPos + RWidth > enemy.getPosition().x - 50 && RPos + RWidth / 2 < enemy.getPosition().x + enemy.getSize().x / 2)
				{
					enemy.move(-1.f, 0.f);
				}
				else if (RPos < enemy.getPosition().x + enemy.getSize().x + 50 && RPos + RWidth / 2 > enemy.getPosition().x + enemy.getSize().x / 2)
				{
					enemy.move(1.f, 0.f);
				}
				else if (enemy.getPosition().x < this->MaxPos)
				{
					enemy.move(1.f, 0.f);
					text.setString(std::to_string(enemy.getPosition().x));
				}
				this->i++;
			}
			else
			{
				//int steg = 1-10
				this->steg = rand() % 10 + 40;
				this->LR = rand() % 2;
				this->i = 0;
			}
		}
		else if (this->LR == 0)
		{
			if (this->i <= this->steg)
			{
				if (RPos + RWidth > enemy.getPosition().x - 300 && RPos + RWidth / 2 < enemy.getPosition().x + enemy.getSize().x / 2)
				{
					enemy.move(-1.f, 0.f);
					text2.setString("R");
				}
				else if (RPos < enemy.getPosition().x + enemy.getSize().x + 300 && RPos + RWidth / 2 > enemy.getPosition().x + enemy.getSize().x / 2)
				{
					enemy.move(1.f, 0.f);
					text2.setString("L");
				}
				else if (enemy.getPosition().x > this->MinPos)
				{
					enemy.move(-1.f, 0.f);
					text.setString(std::to_string(enemy.getPosition().x));
				}
				this->i++;
			}
			else
			{
				//int steg = 1-10
				this->steg = rand() % 10 + 20;
				this->LR = rand() % 2;
				this->i = 0;
			}
		}
		else
		{
			this->LR = rand() % 2;
		}
	}

	void move(float moveX, float moveY)
	{
		enemy.move(moveX, moveY);
	}

	int getHP(void)
	{
		return this->hp;
	}
	void DamageTaken(int damage)
	{
		this->hp -= damage;
	}

	void drawTo(sf::RenderWindow& window)
	{
		if (hp > 0)
		{
			window.draw(enemy);
		}
	}

private:
	sf::RectangleShape enemy;
	sf::Texture Sst;
	sf::Text text, text2;
	int hp;
	int LR = -1;
	int steg = -1;
	int i = 0;
};