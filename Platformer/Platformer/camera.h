#include <SFML/Graphics.hpp>
using namespace sf;

View camera;

View getPlayerView(float x, float y) {
	float temporaryX = x;
	float temporaryY = y;
	if (x < 640) temporaryX = 640;
	if (x > 6400) temporaryX = 6400;
	if (y > 0) temporaryY = 1464;
	
	camera.setCenter(temporaryX, temporaryY);
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
	return camera; // возвращаем камеру с измененными координатами
}