#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 960), "Dangerino");
    Image DinoBlue;
    DinoBlue.loadFromFile("../../Sprites/PNG/DinoSpriteDoux.png");

    Texture BlueDino;
    BlueDino.loadFromImage(DinoBlue);

    float currentframe = 0; // переменная,которая хранит кадр анимации спрайта
    Clock gametime; // привязка ко времени сфмл, а не к процессору

    Sprite player;
    player.setTexture(BlueDino);
    player.setTextureRect(IntRect(0, 0, 65, 91));
    
    player.setPosition(430, 30);

    while (window.isOpen())
    {
        float time = gametime.getElapsedTime().asMicroseconds();
        gametime.restart();
        time = time / 800;
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (Keyboard::isKeyPressed(Keyboard::A))
        {
            currentframe += 0.007 * time;
            if (currentframe > 3) currentframe -= 3;
            player.setTextureRect(IntRect(66 * int(currentframe)+ 66, 113, -66, 91));
            player.move(-0.1*time, 0);
        }
        if (Keyboard::isKeyPressed(Keyboard::D))
        {
            currentframe += 0.005 * time;
            if (currentframe > 3) currentframe -= 3;
            player.setTextureRect(IntRect(66*int(currentframe),113,66,91)); 
            player.move(0.1*time, 0);
           
        }
        if (Keyboard::isKeyPressed(Keyboard::W))
        {
            currentframe += 0.005 * time;
            if (currentframe > 5) currentframe -= 5;
            player.setTextureRect(IntRect(74 * int(currentframe), 222, 74, 96));
            player.move(0,-0.1 * time);
        }
        if (Keyboard::isKeyPressed(Keyboard::S))
        {
            currentframe += 0.005 * time;
            if (currentframe > 3) currentframe -= 3;
            player.setTextureRect(IntRect(66 * int(currentframe), 113, 65, 91));
            player.move(0, 0.1 * time);
        }

        window.clear();
        window.draw(player);
        window.display();
    }

    return 0;
}