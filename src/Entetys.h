#include "Platform/Platform.hpp"
#include <math.h>

#define SFML_NO_DEPRECATED_WARNINGS
#define PI 3.14159265

class KPBullet
{
public:
	KPBullet(float SpawnX, float SpawnY)
	{
		bullet.setRadius(10);
		bullet.setFillColor(sf::Color::Yellow);
		bullet.setPosition(SpawnX, SpawnY);
		bullet.setOrigin(bullet.getRadius(), bullet.getRadius());
	}

	void setPosition(float SpawnX, float SpawnY)
	{
		bullet.setPosition(SpawnX, SpawnY);
	}
	sf::Vector2f getPosition(void) const
	{
		return bullet.getPosition();
	}

	void Fired(float startX, float startY, float endX, float endY)
	{
		bullet.move((endX - startX) / 100, (endY - startY) / 100);
	}

	sf::FloatRect getGlobalBounds(void) const
	{
		return bullet.getGlobalBounds();
	}
	int power(void)
	{
		return this->damage;
	}

	void drawTo(sf::RenderWindow& window)
	{
		window.draw(bullet);
	}

private:
	sf::CircleShape bullet;
	int damage = 3;
};