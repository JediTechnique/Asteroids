#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include "randomEngine.h"

const float PI = 3.1415926535897;

namespace spr
{

	class Bullet
	{
	private:
		float angle;
		float speed = 10.;

	public:
		sf::CircleShape shape;
		sf::Vector2f position;

		Bullet(float x, float y, float ang)
		{
			position = sf::Vector2f(x, y);
			angle = ang;

			shape.setRadius(4.f);
			shape.setPosition(position);
		}

		void moveForward()
		{
			position.y -= (speed * cos(angle * PI / 180));
			position.x += (speed * sin(angle * PI / 180));

			shape.setPosition(position);
		}

		~Bullet() {};
	};

	class Asteroid
	{
	private:
		float speed;
		int up, down, left, right;

	public:
		sf::Vector2f position;
		sf::ConvexShape shape;
		float size;

		Asteroid(float x, float y, float spd, float ratio, int u, int d, int r, int l, int type, float rotation)
		{
			position = sf::Vector2f(x, y);
			up = u;
			down = d;
			left = l;
			right = r;
			speed = spd;
			size = ratio;

			if (type == 1)
			{
				shape.setPointCount(5);
				shape.setPoint(0, sf::Vector2f(50.f, 25.f));
				shape.setPoint(1, sf::Vector2f(100.f, 25.f));
				shape.setPoint(2, sf::Vector2f(100.f, 75.f));
				shape.setPoint(3, sf::Vector2f(50.f, 100.f));
				shape.setPoint(4, sf::Vector2f(25.f, 50.f));
			}

			if (type == 2)
			{
				shape.setPointCount(6);
				shape.setPoint(0, sf::Vector2f(50.f, 25.f));
				shape.setPoint(1, sf::Vector2f(100.f, 25.f));
				shape.setPoint(2, sf::Vector2f(100.f, 75.f));
				shape.setPoint(3, sf::Vector2f(50.f, 100.f));
				shape.setPoint(4, sf::Vector2f(70.f, 50.f));
				shape.setPoint(5, sf::Vector2f(30.f, 70.f));
			}

			if (type == 3)
			{
				shape.setPointCount(5);
				shape.setPoint(0, sf::Vector2f(50.f, 25.f));
				shape.setPoint(1, sf::Vector2f(100.f, 25.f));
				shape.setPoint(2, sf::Vector2f(100.f, 75.f));
				shape.setPoint(3, sf::Vector2f(50.f, 100.f));
				shape.setPoint(4, sf::Vector2f(70.f, 50.f));
			}

			shape.setOrigin(sf::Vector2f(65.f, 55.f));

			shape.setFillColor(sf::Color::Black);
			shape.setOutlineThickness(2.);
			shape.setOutlineColor(sf::Color::White);

			shape.setRotation(rotation);

			shape.setScale(ratio, ratio);

			shape.setPosition(position);
		}

		void moveForward()
		{
			position.x += speed * right;
			position.x -= speed * left;
			position.y += speed * down;
			position.y -= speed * up;

			shape.setPosition(position);
		}

		~Asteroid() {};
	};

	class AsteroidsSpawner
	{
	private:
		float minSpeed, maxSpeed;
		int up, down, right, left;
		float minX, maxX, minY, maxY;
		float spawnRate;

	public:
		std::vector<Asteroid> asteroids;

		AsteroidsSpawner(int position, float ms, float Ms, float sr)
		{
			switch (position)
			{
			case 1:
				down = 1;
				up = 0;
				right = 0;
				left = 0;
				minX = 10;
				maxX = 830;
				minY = -100;
				maxY = -50;
				break;
			case 2:
				down = 0;
				up = 0;
				right = 0;
				left = 1;
				minX = 870;
				maxX = 900;
				minY = 10;
				maxY = 620;
				break;
			case 3:
				down = 0;
				up = 1;
				right = 0;
				left = 0;
				minX = 10;
				maxX = 830;
				minY = 680;
				maxY = 700;
				break;
			case 4:
				down = 0;
				up = 0;
				right = 1;
				left = 0;
				minX = -100;
				maxX = -50;
				minY = 10;
				maxY = 630;
				break;
			}
			minSpeed = ms;
			maxSpeed = Ms;
			spawnRate = sr;
		}

		void spawn()
		{

			float x = rnd::generateRandomFloat(minX, maxX);
			float y = rnd::generateRandomFloat(minY, maxY);
			float speed = rnd::generateRandomFloat(minSpeed, maxSpeed);
			float ratio = rnd::generateRandomFloat(0.7, 1.5);
			float type = (int)rnd::generateRandomFloat(1, 3.9);
			float rotation = rnd::generateRandomFloat(0.0, 360.0);

			Asteroid ast(x, y, speed, ratio, up, down, right, left, type, rotation);
			asteroids.push_back(ast);
		}

		void erase(int i)
		{
			if (asteroids[i].size > 1)
			{
				for (int j = 0; j < 4; j++)
				{
					float x = asteroids[i].position.x;
					float y = asteroids[i].position.y;
					float speed = rnd::generateRandomFloat(minSpeed, maxSpeed);
					float ratio = rnd::generateRandomFloat(0.3, 0.7);
					float type = (int)rnd::generateRandomFloat(1, 3.9);
					float rotation = rnd::generateRandomFloat(0.0, 360.0);
					int u = 0, d = 0, r = 0, l = 0;
					switch (j)
					{
					case 0:
						u = 1;
						break;
					case 1:
						d = 1;
						break;
					case 2:
						r = 1;
						break;
					case 3:
						l = 1;
						break;
					}
					Asteroid ast(x, y, speed, ratio, u, d, r, l, type, rotation);
					asteroids.push_back(ast);
				}
			}
			asteroids.erase(asteroids.begin() + i);
		}

		~AsteroidsSpawner() {};
	};

	class Player
	{
	private:
		float rotationSpeed;
		float speed;
		float currentSpeed = 0.;
		float angle = 0.;
		float dragAngle = 0.;
		float dragSpeed = 0.;
		float thrustSpeed = 0.;
		float last_shot = 0.;

	public:
		sf::Texture texture;
		sf::Sprite sprite;
		sf::Vector2f position;
		std::vector<Bullet> bullets;
		int isInvincible = 0;

		bool ok = false;

		Player(float spd, float rspd)
		{
			if (!texture.loadFromFile("player.png"))
			{
				// error...
			}
			texture.setSmooth(true);
			sprite.setTexture(texture);
			sprite.setScale(sf::Vector2f(0.5f, 0.5f));

			speed = spd;
			rotationSpeed = rspd;

			sprite.setOrigin(sf::Vector2f(32.f, 32.f));
			sprite.setPosition(sf::Vector2f(425.f, 325.f));
			position = sf::Vector2f(425.f, 325.f);
		}

		void rotate(float rotationAngle)
		{
			this->angle += rotationAngle;
			if (this->angle > 360)
				this->angle -= 360;
			if (this->angle < 0)
				this->angle = 360 - this->angle;
			sprite.setRotation(this->angle);
		}

		void accelerate()
		{
			if (this->thrustSpeed < this->speed)
			{
				this->thrustSpeed += 0.03;
			}
			else
				this->thrustSpeed = this->speed;
		}

		void decelerate()
		{
			if (this->dragSpeed > 0)
			{
				this->dragSpeed -= 0.03;
			}
			else
				this->dragSpeed = 0.;
		}

		void moveForward(float distance1, float angle1, float distance2, float angle2)
		{
			position.y -= (distance1 * cos(angle1 * PI / 180) + distance2 * cos(angle2 * PI / 180));
			position.x += (distance1 * sin(angle1 * PI / 180) + distance2 * sin(angle2 * PI / 180));

			sprite.setPosition(position);
		}

		void shoot(float time)
		{
			if (time - last_shot > 0.5)
			{
				last_shot = time;
				Bullet bullet(position.x, position.y, angle);
				bullets.push_back(bullet);
			}
		}

		void getInput(float time)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				rotate(-this->rotationSpeed);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				rotate(this->rotationSpeed);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
			{
				shoot(time);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				this->ok = true;
				accelerate();
				decelerate();
				moveForward(this->thrustSpeed, this->angle, this->dragSpeed, this->dragAngle);
			}
			if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				if (this->ok && this->dragSpeed == 0)
				{
					this->dragAngle = angle;
					this->dragSpeed = thrustSpeed;
					this->thrustSpeed = 0.;
					this->ok = false;
				}
				decelerate();
				moveForward(this->thrustSpeed, this->angle, this->dragSpeed, this->dragAngle);
			}
		}

		void checkPosition()
		{
			if (position.x < -40)
			{
				position.x = 850;
				sprite.setPosition(position);
			}

			if (position.x > 890)
			{
				position.x = 0;
				sprite.setPosition(position);
			}

			if (position.y < -40)
			{
				position.y = 650;
				sprite.setPosition(position);
			}

			if (position.y > 690)
			{
				position.y = 0;
				sprite.setPosition(position);
			}
		}

		~Player() {};
	};

}