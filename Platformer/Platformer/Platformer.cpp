#include <iostream>
#include <SFML/Graphics.hpp>
#include "Platformer.h"

using namespace sf;


    class Player {
    public:
        float x, y, dx, dy, w, h;
        float speed = 0;
        int direction = 0;
        String File;
        Sprite sprite;
        Image image;
        Texture texture;
        Player(String F, int X, int Y, float W, float H) {
            File = F;
            w = W;
            h = H;
            image.loadFromFile("../../Sprites/PNG/" + File);
            texture.loadFromImage(image);
            sprite.setTexture(texture);
            x = X;
            y = Y;
            sprite.setTextureRect(IntRect(0, 0, w, h));

}
   
    void update(float time) {
        switch (direction)
        {
        case 0: dx = speed; dy = 0;
            break;
        case 1: dx = -speed; dy = 0; 
            break;
        case 2: dx = 0, dy = speed;
            break;
        case 3: dx = 0, dy = -speed;
            break;
        default:
            break;
        }
        x +=dx * time; // перемещение координаты х за время,когда задаем расстояние перемещения за время, получаем новую координату
        y +=dy * time; // то же, что и с иксом

        speed = 0;
        sprite.setPosition(x, y);

    }
    };

    int main()
    {

    RenderWindow window(VideoMode(1280, 960), "Dangerino");

    Image map_image;
    map_image.loadFromFile("../../Sprites/PNG/Tileset.png");
    Texture map;
    map.loadFromImage(map_image);
    Sprite mainmap;
    mainmap.setTexture(map);


    Player Dino("DinoSpriteDoux.png", 250,250, 65.0, 100.0);

    float currentframe = 0; // переменная,которая хранит кадр анимации спрайта
    Clock gametime; // привязка ко времени сфмл, а не к процессору

   
   

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
            Dino.direction = 1;
            Dino.speed = 0.1;
            currentframe += 0.007 * time;
            if (currentframe > 3) currentframe -= 3;
            Dino.sprite.setTextureRect(IntRect(66 * int(currentframe) + 66, 113, -66, 91));
         
        }
        if (Keyboard::isKeyPressed(Keyboard::D))
        {   
            Dino.direction = 0;
            Dino.speed = 0.1;
            currentframe += 0.005 * time;
            if (currentframe > 3) currentframe -= 3;
            Dino.sprite.setTextureRect(IntRect(66*int(currentframe),113,66,91)); 
       
           
        }
        if (Keyboard::isKeyPressed(Keyboard::W))
        {
            Dino.direction = 3;
            Dino.speed = 0.1;
            currentframe += 0.005 * time;
            if (currentframe > 5) currentframe -= 5;
            Dino.sprite.setTextureRect(IntRect(74 * int(currentframe), 222, 74, 96));
           
        }
        if (Keyboard::isKeyPressed(Keyboard::S))
        { 
            Dino.direction = 2;
            Dino.speed = 0.1;
            currentframe += 0.005 * time;
            if (currentframe > 3) currentframe -= 3;
            Dino.sprite.setTextureRect(IntRect(66 * int(currentframe), 113, 65, 91));
            
        }
        Dino.update(time);

        window.clear();

        for ( int i = 0; i < height_map; i++)
            for (int j = 0; j < width_map; j++)
            {
                if (TileMap[i][j] == ' ')  mainmap.setTextureRect(IntRect(0, 0, 65, 65));
                if (TileMap[i][j] == '0') mainmap.setTextureRect(IntRect(93, 50, 65 , 65));


                mainmap.setPosition(j * 65, i * 65);


                window.draw(mainmap);
            }
        window.draw(Dino.sprite);
        window.display();
    }

    return 0;
}