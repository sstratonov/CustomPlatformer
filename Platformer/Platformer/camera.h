#include <SFML/Graphics.hpp>
using namespace sf;

View camera;

View getPlayerView(float x, float y) {
	camera.setCenter(x + 100, y);
	return camera;
}

View viewmap(float time)
{
	if (Keyboard::isKeyPressed(Keyboard::D)) {
		camera.move(0.1 * time, 0);
	}
	if (Keyboard::isKeyPressed(Keyboard::S)) {
		camera.move(0, 0.1*time);
	}
	if (Keyboard::isKeyPressed(Keyboard::A)) {
		camera.move(-0.1 * time, 0);
	}
	if (Keyboard::isKeyPressed(Keyboard::W)) {
		camera.move(0, -0.1*time);
	}
	return camera; // возвращаем камеру с измененными координатамиы
}