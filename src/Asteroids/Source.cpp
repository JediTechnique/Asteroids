#include <SFML/Graphics.hpp>
#include <iostream>
#include "sprites.h"

inline float distancesq(float x1, float y1, float x2, float y2)
{
	return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
}

int main()
{

	sf::RenderWindow window(sf::VideoMode(850, 650), "Asteroids");

	window.setFramerateLimit(60);
	spr::Player player(5., 3.);
	spr::AsteroidsSpawner spawner1(1, 1, 3, 5);
	spr::AsteroidsSpawner spawner2(2, 1, 3, 5);
	spr::AsteroidsSpawner spawner3(3, 1, 3, 5);
	spr::AsteroidsSpawner spawner4(4, 1, 3, 5);

	sf::Clock clock;

	float accumulator = 0;
	int lives = 3;
	long long score = 0;
	const float TIMESTEP = 1.0f / 60.0f;
	float last_frame = 0.;
	float last_spawn = 0.;
	float last_hit = 0.;

	int isGameOver = 0;

	sf::Font font;
	if (!font.loadFromFile("Silkscreen-Regular.ttf"))
	{
		// error
	}
	sf::Text text, scoreText, gameOverText;
	text.setFont(font);
	text.setFillColor(sf::Color::White);
	text.setCharacterSize(24);
	text.setPosition(sf::Vector2f(10.f, 10.f));

	scoreText.setFont(font);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setCharacterSize(24);
	scoreText.setPosition(sf::Vector2f(10.f, 50.f));

	gameOverText.setFont(font);
	gameOverText.setFillColor(sf::Color::White);
	gameOverText.setCharacterSize(60);
	gameOverText.setOrigin(sf::Vector2f(240, 30));
	gameOverText.setPosition(sf::Vector2f(470.f, 310.f));
	gameOverText.setString("Game Over");

	while (window.isOpen())
	{
		if (!isGameOver)
		{
			sf::Event e;
			while (window.pollEvent(e))
				if (e.type == sf::Event::Closed)
					window.close();

			accumulator += clock.getElapsedTime().asSeconds() - last_frame;
			last_frame = clock.getElapsedTime().asSeconds();

			while (accumulator >= TIMESTEP)
			{
				accumulator -= TIMESTEP;
				player.getInput(clock.getElapsedTime().asSeconds());
				player.checkPosition();
				if (clock.getElapsedTime().asSeconds() - last_hit > 2 && player.isInvincible == 1)
				{
					player.isInvincible = 0;
					player.sprite.setColor(sf::Color(255, 255, 255, 255));
				}

				if (clock.getElapsedTime().asSeconds() - last_spawn > 4)
				{
					last_spawn = clock.getElapsedTime().asSeconds();
					spawner1.spawn();
					spawner2.spawn();
					spawner3.spawn();
					spawner4.spawn();
				}

				for (unsigned int i = 0; i < player.bullets.size(); i++)
				{
					player.bullets[i].moveForward();
					if (player.bullets[i].position.x > 850 ||
						player.bullets[i].position.x < 1 ||
						player.bullets[i].position.y > 650 ||
						player.bullets[i].position.y < 1)
						player.bullets.erase(player.bullets.begin() + i);
				}

				for (unsigned int i = 0; i < spawner1.asteroids.size(); i++)
				{
					spawner1.asteroids[i].moveForward();

					float x = spawner1.asteroids[i].position.x;
					float y = spawner1.asteroids[i].position.y;
					float size = spawner1.asteroids[i].size;

					for (unsigned int j = 0; j < player.bullets.size(); j++)
					{
						if (distancesq(x, y, player.bullets[j].position.x, player.bullets[j].position.y) <= 2000 * size)
						{
							score += 50. / spawner1.asteroids[i].size;
							spawner1.erase(i);
							player.bullets.erase(player.bullets.begin() + j);
						}
					}

					if (distancesq(x, y, player.position.x, player.position.y) <= 2000 * size)
					{
						if (clock.getElapsedTime().asSeconds() - last_hit > 2)
						{
							last_hit = clock.getElapsedTime().asSeconds();
							player.isInvincible = 1;
							lives--;
							player.sprite.setColor(sf::Color::Red);
							if (lives < 0)
								isGameOver = 1;
						}
					}

					if (y > 1000)
					{
						spawner1.erase(i);
					}
				}

				for (unsigned int i = 0; i < spawner2.asteroids.size(); i++)
				{
					spawner2.asteroids[i].moveForward();
					float x = spawner2.asteroids[i].position.x;
					float y = spawner2.asteroids[i].position.y;
					float size = spawner2.asteroids[i].size;

					for (unsigned int j = 0; j < player.bullets.size(); j++)
					{
						if (distancesq(x, y, player.bullets[j].position.x, player.bullets[j].position.y) <= 2000 * size)
						{
							score += 50. / spawner2.asteroids[i].size;
							spawner2.erase(i);
							player.bullets.erase(player.bullets.begin() + j);
						}
					}

					if (distancesq(x, y, player.position.x, player.position.y) <= 2000 * size)
					{
						if (clock.getElapsedTime().asSeconds() - last_hit > 2)
						{
							last_hit = clock.getElapsedTime().asSeconds();
							player.isInvincible = 1;
							lives--;
							player.sprite.setColor(sf::Color::Red);
							if (lives < 0)
								isGameOver = 1;
						}
					}

					if (x < -50)
					{
						spawner2.erase(i);
					}
				}

				for (unsigned int i = 0; i < spawner3.asteroids.size(); i++)
				{
					spawner3.asteroids[i].moveForward();
					float x = spawner3.asteroids[i].position.x;
					float y = spawner3.asteroids[i].position.y;
					float size = spawner3.asteroids[i].size;

					for (unsigned int j = 0; j < player.bullets.size(); j++)
					{
						if (distancesq(x, y, player.bullets[j].position.x, player.bullets[j].position.y) <= 2000 * size)
						{
							score += 50. / spawner3.asteroids[i].size;
							spawner3.erase(i);
							player.bullets.erase(player.bullets.begin() + j);
						}
					}

					if (distancesq(x, y, player.position.x, player.position.y) <= 2000 * size)
					{
						if (clock.getElapsedTime().asSeconds() - last_hit > 2)
						{
							last_hit = clock.getElapsedTime().asSeconds();
							player.isInvincible = 1;
							lives--;
							player.sprite.setColor(sf::Color::Red);
							if (lives < 0)
								isGameOver = 1;
						}
					}

					if (y < -50)
					{
						spawner3.erase(i);
					}
				}

				for (unsigned int i = 0; i < spawner4.asteroids.size(); i++)
				{
					spawner4.asteroids[i].moveForward();
					float x = spawner4.asteroids[i].position.x;
					float y = spawner4.asteroids[i].position.y;
					float size = spawner4.asteroids[i].size;

					for (unsigned int j = 0; j < player.bullets.size(); j++)
					{
						if (distancesq(x, y, player.bullets[j].position.x, player.bullets[j].position.y) <= 2000 * size)
						{
							score += 50. / spawner4.asteroids[i].size;
							spawner4.erase(i);
							player.bullets.erase(player.bullets.begin() + j);
						}
					}

					if (distancesq(x, y, player.position.x, player.position.y) <= 2000 * size)
					{
						if (clock.getElapsedTime().asSeconds() - last_hit > 2)
						{
							last_hit = clock.getElapsedTime().asSeconds();
							player.isInvincible = 1;
							lives--;
							player.sprite.setColor(sf::Color::Red);
							if (lives < 0)
								isGameOver = 1;
						}
					}

					if (x > 1200)
					{
						spawner4.erase(i);
					}
				}
				text.setString("lives: " + std::to_string(lives));
				scoreText.setString("Score: " + std::to_string(score));
			}

			window.clear(sf::Color::Black);

			for (unsigned int i = 0; i < player.bullets.size(); i++)
			{
				window.draw(player.bullets[i].shape);
			}

			for (unsigned int i = 0; i < spawner1.asteroids.size(); i++)
			{
				window.draw(spawner1.asteroids[i].shape);
			}
			for (unsigned int i = 0; i < spawner2.asteroids.size(); i++)
			{
				window.draw(spawner2.asteroids[i].shape);
			}
			for (unsigned int i = 0; i < spawner3.asteroids.size(); i++)
			{
				window.draw(spawner3.asteroids[i].shape);
			}
			for (unsigned int i = 0; i < spawner4.asteroids.size(); i++)
			{
				window.draw(spawner4.asteroids[i].shape);
			}

			window.draw(player.sprite);

			window.draw(text);
			window.draw(scoreText);

			window.display();
		}
		else
		{
			sf::Event e;
			while (window.pollEvent(e))
				if (e.type == sf::Event::Closed)
					window.close();

			accumulator += clock.getElapsedTime().asSeconds() - last_frame;
			last_frame = clock.getElapsedTime().asSeconds();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			{
				isGameOver = 0;
				score = 0;
				lives = 3;
				spawner1.asteroids.clear();
				spawner2.asteroids.clear();
				spawner3.asteroids.clear();
				spawner4.asteroids.clear();
			}

			window.clear(sf::Color::Black);

			window.draw(scoreText);
			window.draw(gameOverText);

			window.display();
		}
	}
	return 0;
}