#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <conio.h>
#include <math.h>
const int pixel = 100;

void logErrorMessage()
{
    std::cout << "Error! File not found!" << std::endl;
    exit(1);
}

struct Object
{
    sf::Texture texture;
    sf::Sprite sprite;
};

struct World
{
    Object cat;
    Object pointer;
};

void initObject(Object &obj, const std::string &path, sf::Vector2f position = {}) //добавь первоначальную позицию
{
    if (!obj.texture.loadFromFile(path)) //если не получится загрузить файл
    {
        logErrorMessage();
    }
    obj.texture.setSmooth(true);
    obj.sprite.setTexture(obj.texture);
    obj.sprite.setPosition(position);
}

void initWorld(World &world)
{

    initObject(world.cat, "images/cat.png", sf::Vector2f(400, 300));
    initObject(world.pointer, "images/red_pointer.png", sf::Vector2f(-100, -100));
}

void update(World &world, const sf::Vector2f &mousePosition, sf::Clock &clock, bool &start)
{
    const float deltaTime = clock.restart().asSeconds();
    const sf::Vector2f deltaPosition = mousePosition - world.cat.sprite.getPosition();
    sf::Vector2f catPosition = world.cat.sprite.getPosition();
    world.cat.sprite.setOrigin({24, 23});
    if (mousePosition.x < catPosition.x)
    {
        world.cat.sprite.setScale(-1, 1);
    }
    else
    {
        world.cat.sprite.setScale(1, 1);
    }
    float maxMovement = deltaTime * pixel;
    float d = hypot(deltaPosition.x, deltaPosition.y);
    if (d > 30 && start == true)
    {
        float x = (deltaPosition.x / d);
        float y = deltaPosition.y / d;
        const sf::Vector2f newDirection = {x, y};
        const sf::Vector2f newCatposition = world.cat.sprite.getPosition();
        world.cat.sprite.setPosition(newCatposition + newDirection * maxMovement);
    }
}

void updatePointer(Object &obj, sf::Vector2f mousePosition)
{
    obj.sprite.setPosition(mousePosition);
    obj.sprite.setOrigin({21, 22});
}

void onMousePressed(World &world, sf::Vector2f mousePosition)
{
    //updateCat(world.cat, mousePosition);
    updatePointer(world.pointer, mousePosition);
}

//опрашивает и обрабатывает доступные события в цикле
void pollEvents(sf::RenderWindow &window, World &world, sf::Vector2f &mousePosition, bool &start)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::MouseButtonPressed:
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                mousePosition = {float(event.mouseButton.x), float(event.mouseButton.y)};
                onMousePressed(world, mousePosition);
                start = true;
            }
            break;
        default:
            break;
        }
    }
}

// Рисует и выводит один кадр
void redrawFrame(sf::RenderWindow &window, World &world)
{
    window.clear(sf::Color(254, 254, 254, 255));
    window.draw(world.cat.sprite);
    window.draw(world.pointer.sprite);
    window.display();
}

int main()
{
    constexpr unsigned WINDOW_WIDTH = 800;
    constexpr unsigned WINDOW_HEIGHT = 600;
    sf::Clock clock;
    const float maxRotSpeed = 100;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(
        sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),

        "Arrow follows mouse", sf::Style::Default, settings);
    bool start = false;
    World world;
    initWorld(world);
    sf::Vector2f mousePosition;

    while (window.isOpen())
    {
        pollEvents(window, world, mousePosition, start);
        update(world, mousePosition, clock, start);
        redrawFrame(window, world);
    }
    return 0;
}