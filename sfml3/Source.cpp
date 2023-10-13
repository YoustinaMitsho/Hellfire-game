#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;

int main() {
	RenderWindow window(VideoMode(1300, 700), "Hell Fire");
	window.setFramerateLimit(30);

	Font font;
	font.loadFromFile("RetroGrade.otf");

	int score = 0;
	int health = 3;

	Text text;
	text.setFont(font);
	text.setString("Score: " + to_string(score) + "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t Health: " + to_string(health));
	text.setPosition(10, 10);
	text.setCharacterSize(50);

	Text gameover;
	gameover.setFont(font);
	gameover.setString("Game Over!");
	gameover.setFillColor(Color::Red);
	gameover.setCharacterSize(72);

	Vector2f center = Vector2f(635, 650);

	Texture backgroundT;
	backgroundT.loadFromFile("background1.jpg");
	Sprite background(backgroundT);
	background.setScale(1.5, 1.5);

	vector <CircleShape> bullets;
	CircleShape bullet;
	bullet.setRadius(5.f);
	bullet.setFillColor(Color::Red);
	bullet.setPosition(center.x + 40, center.y);
	bullets.push_back(CircleShape(bullet));

	int shootTimer = 0;

	vector <Sprite> enemies;
	int enemyspawntimer = 0;
	Texture EnemyT;
	EnemyT.loadFromFile("enemy.png");
	Sprite enemy(EnemyT);
	enemy.setScale(0.5, 0.5);
	enemies.push_back(Sprite(enemy));

	Texture PlayerT;
	PlayerT.loadFromFile("player.png");
	Sprite player(PlayerT);
	player.setScale(0.1, 0.1);
	player.setPosition(0, window.getSize().y - 100);
	bool is_alive = true;
	
	// sounds
	
	//shot
	SoundBuffer Buffer1; Buffer1.loadFromFile("shot.wav");
	Sound shot; shot.setBuffer(Buffer1); shot.setVolume(100);
	//damage
	SoundBuffer Buffer2; Buffer2.loadFromFile("damage.wav");
	Sound damage; damage.setBuffer(Buffer2); damage.setVolume(100);


	while (window.isOpen()) {
		Event closed;
		while (window.pollEvent(closed)) {
			if (closed.type == Event::Closed) {
				window.close();
			}

			if (closed.type == Event::KeyPressed && closed.key.code == Keyboard::Escape)
				window.close();
		}

		if (is_alive) {
			player.setPosition(Mouse::getPosition(window).x - player.getScale().x, player.getPosition().y);

			if (shootTimer < 5)
				shootTimer++;
			if (Mouse::isButtonPressed(Mouse::Left) && shootTimer >= 5)
			{
				bullet.setPosition(player.getPosition().x + 40, bullet.getPosition().y);
				bullets.push_back(CircleShape(bullet));
				shootTimer = 0;
			}
			for (size_t i = 0; i < bullets.size(); i++)
			{
				bullets[i].move(0.f, -10.f);
				if (bullets[i].getPosition().y <= 0)
					bullets.erase(bullets.begin() + i);
			}
			if (enemyspawntimer < 4) enemyspawntimer++;

			if (enemyspawntimer >= 4)
			{
				enemy.setPosition((rand() % int(window.getSize().x - enemy.getScale().x)), 0.f);
				enemies.push_back(Sprite(enemy));
				enemyspawntimer = 0;
			}
			for (size_t i = 0; i < enemies.size(); i++)
			{
				enemies[i].move(0.f, 5.f);
				if (enemies[i].getPosition().y > window.getSize().y)
					enemies.erase(enemies.begin() + i);
			}
			for (size_t i = 0; i < bullets.size(); i++)
			{
				for (size_t j = 0; j < enemies.size(); j++)
				{
					if (bullets[i].getGlobalBounds().intersects(enemies[j].getGlobalBounds())) {
						bullets.erase(bullets.begin() + i);
						enemies.erase(enemies.begin() + j);
						score++;
						shot.play();
						text.setString("Score: " + to_string(score) + "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t Health: " + to_string(health));
						break;
					}
				}
			}

			for (size_t j = 0; j < enemies.size(); j++)
			{
				if (enemies[j].getGlobalBounds().intersects(player.getGlobalBounds())) {
					enemies.erase(enemies.begin() + j);
					health--;
					damage.play();
					text.setString("Score: " + to_string(score) + "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t Health: " + to_string(health));
					if (health <= 0) {
						is_alive = false;
					}
				}
			}
		}


		window.clear();
		window.draw(background);
		window.draw(text);
		for (size_t i = 0; i < enemies.size(); i++) window.draw(enemies[i]);
		for (size_t i = 0; i < bullets.size(); i++) window.draw(bullets[i]);
		window.draw(player);
		if (!is_alive) {
			gameover.setPosition(player.getPosition().x - 315, player.getPosition().y - 80);
			window.draw(gameover);
		}
		window.display();
	}
}