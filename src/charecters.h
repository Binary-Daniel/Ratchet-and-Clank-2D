#pragma once
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
	Ratchet()
	{
		float rsx = 120;
		float rsy = 240;

		RBt.loadFromFile("content/charecters/ratchet/ratchet1.png");
		RAt.loadFromFile("content/charecters/ratchet/ratchet arm kp.png");
		player.setTexture(&RBt);
		arm.setTexture(&RAt);

		player.setSize({ rsx, rsy });

		collision.setSize({ 8, player.getSize().y });
		collision.setFillColor(sf::Color::Transparent);


		arm.setSize({ 152, 136 });
		arm.setOrigin(25, 24);
		arm.setRotation(atan2(sf::Mouse::getPosition().y - arm.getPosition().y, sf::Mouse::getPosition().x - arm.getPosition().x) * 180 / PI * -1);

		BulletSpawn[0].position = sf::Vector2f(arm.getPosition().x, arm.getPosition().y);
		BulletSpawn[1].position = sf::Vector2f(arm.getPosition().x + 150 * cos(arm.getRotation() * (PI / 180) + 0.71), arm.getPosition().y + 150 * sin(arm.getRotation() * (PI / 180) + 0.71));
		BulletSpawn[0].color = sf::Color::Red;
		BulletSpawn[1].color = sf::Color::Red;

		Range[0].position = sf::Vector2f(arm.getPosition().x, arm.getPosition().y);
		Range[1].position = sf::Vector2f(arm.getPosition().x + 500 * cos(arm.getRotation() * (PI / 180) + 0.71), arm.getPosition().y + 550 * sin(arm.getRotation() * (PI / 180) + 0.71));
		Range[0].color = sf::Color::Blue;
		Range[1].color = sf::Color::Blue;
	}

	void StartPos(sf::Vector2f pos)
	{
		this->startpos = pos;
		player.setPosition(pos);
		collision.setPosition(player.getPosition().x + (player.getSize().x) / 2 + 12, player.getPosition().y);
		arm.setPosition((player.getPosition().x + (player.getSize().x) / 2 - 4) - 8 * 2 + 25, player.getPosition().y + 8 * 5 + 24);
	}

	sf::Vector2f getArmPos(void) const
	{
		return arm.getPosition();
	}
	void RotateArm(void)
	{
		arm.setRotation(atan2(sf::Mouse::getPosition().x - arm.getPosition().x, sf::Mouse::getPosition().y - arm.getPosition().y) * 180 / PI * -1 + 49);
		BulletSpawn[1].position = sf::Vector2f(arm.getPosition().x + 150 * cos(arm.getRotation() * (PI / 180) + 0.715), arm.getPosition().y + 150 * sin(arm.getRotation() * (PI / 180) + 0.715));
		Range[1].position = sf::Vector2f(arm.getPosition().x + 750 * cos(arm.getRotation() * (PI / 180) + 0.715), arm.getPosition().y + 750 * sin(arm.getRotation() * (PI / 180) + 0.715));
	}
	float getArmRotation(void)
	{
		return arm.getRotation();
		BulletSpawn[1].position = sf::Vector2f(arm.getPosition().x + 150 * cos(arm.getRotation() * (PI / 180) + 0.715), arm.getPosition().y + 150 * sin(arm.getRotation() * (PI / 180) + 0.715));
	}

	sf::Vector2f getBulletSpawn() const
	{
		return BulletSpawn[1].position;
	}
	sf::Vector2f getRange() const
	{
		return Range[1].position;
	}

	void Damage(void)
	{
		hp--;
		if (hp < -3)
		{
			player.setPosition(startpos);
			collision.setPosition(player.getPosition().x + (player.getSize().x) / 2 + 12, player.getPosition().y);
			arm.setPosition((player.getPosition().x + (player.getSize().x) / 2 - 4) - 8 * 2 + 25, player.getPosition().y + 8 * 5 + 24);
			hp = 3;
			//förflyta positionen för projectilens start kordinat
			BulletSpawn[0].position = sf::Vector2f(arm.getPosition().x, arm.getPosition().y);
			BulletSpawn[1].position = sf::Vector2f(arm.getPosition().x + 150 * cos(arm.getRotation() * (PI / 180) + 0.715), arm.getPosition().y + 150 * sin(arm.getRotation() * (PI / 180) + 0.715));

			Range[0].position = sf::Vector2f(arm.getPosition().x, arm.getPosition().y);
			Range[1].position = sf::Vector2f(arm.getPosition().x + 750 * cos(arm.getRotation() * (PI / 180) + 0.715), arm.getPosition().y + 750 * sin(arm.getRotation() * (PI / 180) + 0.715));
		}
	}

	void move(float moveX, float moveY)
	{
		player.move(moveX, moveY);
		collision.move(moveX, moveY);
		arm.move(moveX, moveY);
		//förflyta positionen för projectilens start kordinat
		BulletSpawn[0].position = sf::Vector2f(arm.getPosition().x, arm.getPosition().y);
		BulletSpawn[1].position = sf::Vector2f(arm.getPosition().x + 150 * cos(arm.getRotation() * (PI / 180) + 0.715), arm.getPosition().y + 150 * sin(arm.getRotation() * (PI / 180) + 0.715));

		Range[0].position = sf::Vector2f(arm.getPosition().x, arm.getPosition().y);
		Range[1].position = sf::Vector2f(arm.getPosition().x + 750 * cos(arm.getRotation() * (PI / 180) + 0.715), arm.getPosition().y + 750 * sin(arm.getRotation() * (PI / 180) + 0.715));
	}

	sf::Vector2f getPosition(void) const
	{
		return player.getPosition();
	}

	sf::Vector2f getSize(void) const
	{
		return player.getSize();
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
			window.draw(Range, 2, sf::Lines);
			window.draw(BulletSpawn, 2, sf::Lines);
		}
	}

private:
	sf::RectangleShape player;
	sf::RectangleShape collision;
	sf::RectangleShape arm;
	sf::Texture RBt;
	sf::Texture RAt;
	sf::Vertex BulletSpawn[2];
	sf::Vertex Range[2];
	float BulletRangeDieX = 0, BulletRangeDieY = 0;
	sf::Vector2f(startpos);
};

class Ssyphoid
{
public:
	int MinPos;
	int MaxPos;

	Ssyphoid()
	{
		Sst.loadFromFile("content/charecters/zyphoid/Small_syphoid.png");
		enemy.setSize(sf::Vector2f(Sst.getSize().x, Sst.getSize().y));
		enemy.setFillColor(sf::Color::White);
		enemy.setTexture(&Sst);
	}

	void setings(int Time, int hp, int minX, int maxX)
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
		enemy.setPosition(rand() % minX + 200, 732);
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