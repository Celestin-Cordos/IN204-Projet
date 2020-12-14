#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <SFML/Graphics.hpp>

// en pixels:
int HEIGHT = 600;
int WIDTH = 800;
int STEP = 10; // de combien de pixels descend la figure

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
    window.setPosition (sf::Vector2i(0, 0));
    window.setFramerateLimit (60);

    sf::CircleShape circle(50);
    circle.setFillColor(sf::Color::Green);
    circle.setPosition(400,300);

    sf::RectangleShape rectangle (sf::Vector2f(150,100));
    rectangle.setFillColor (sf::Color::Red);
    rectangle.setPosition(200,300);
    int i = 0;
    while (window.isOpen())
    {
        i++;
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        //Gestion du clavier:
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            rectangle.rotate(90);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            rectangle.rotate(-90);
        }
        // mouvement
        if (i%60==0)
        {
            if (rectangle.getSize().y+rectangle.getPosition().y <HEIGHT)
            {
                rectangle.move(0,STEP);
                std::cout << rectangle.getSize().y << " + " << rectangle.getPosition().y << " < " << HEIGHT << std::endl;
            }

        }

        //affichage:
        window.clear();
        window.draw(circle);
        window.draw(rectangle);
        window.display();
    }

    return 0;
}
