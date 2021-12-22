#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;

int ground = 700;

float offsetX = 0, offsetY = 0;

const int H = 9;
const int W = 16;


String TileMap[H] = {

"BBBBBBBBBBBBBBBB",
"B              B",
"B        B     B",
"BBB      B     B",
"B        B     B",
"B        BBBB  B",
"B              B",
"BBB      B     B",
"BBBBBBBBBBBBBBBB",
};

void Build(int a, int b)
{
    TileMap[b][a] = 'B';
}
void Destroy(int a, int b)
{
    TileMap[b][a] = ' ';
}

class PLAYER {

public:

    float dx, dy;
    FloatRect rect;
    bool onGround;
    Sprite sprite;
    float currentFrame;

    PLAYER(Texture& image)
    {
        sprite.setTexture(image);
        rect = FloatRect(300, 700, 200, 100);

        dx = 300;
        dy = 700;
        currentFrame = 0;
    }


    void update(float time)
    {
        rect.left += dx * time;
        Collision(0);
        if (!onGround) dy = dy + 0.0005 * time;
        rect.top += dy * time;
        onGround = false;
        Collision(1);
        if (rect.top > ground)
        {
            rect.top = ground;
            dy = 0;
            onGround = true;
        }
        currentFrame += 0.005 * time;
        if (currentFrame > 2) currentFrame -= 2;
        if ((dx < 0) and (dy==0)) sprite.setTextureRect(IntRect(200 * int(currentFrame), 0, 200, 100));
        if ((dx > 0) and (dy==0)) sprite.setTextureRect(IntRect(200 * int(currentFrame) + 200, 0, -200, 100));
        if ((dx < 0) and (dy != 0)) sprite.setTextureRect(IntRect(200 * int(currentFrame) + 200, 0, 200, 100));
        if ((dx > 0) and (dy != 0)) sprite.setTextureRect(IntRect(400 * int(currentFrame)+200, 0, -200, 100));
        dx = 0;
        sprite.setPosition(rect.left-offsetX, rect.top-offsetY);
    }

    void Collision(int dir)
    {
        for (int i = rect.top / 100; i < (rect.top + rect.height) / 100; i++)
            for (int j = rect.left / 100; j < (rect.left + rect.width) / 100; j++)
            {
                if (TileMap[i][j] == 'B')
                {
                    if ((dx > 0) && (dir == 0)) rect.left = j * 100 - rect.width;
                    if ((dx < 0) && (dir == 0)) rect.left = j * 100 + 100;
                    if ((dy > 0) && (dir == 1)) { rect.top = i * 100 - rect.height;  dy = 0;   onGround = true; }
                    if ((dy < 0) && (dir == 1)) { rect.top = i * 100 + 100;   dy = 0; }
                }

            }

    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(1600, 900), "Marussia`s Bizzare Adventure");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    Texture martex;
    martex.loadFromFile("1.png");

    int tempX = 0;
    int tempY = 0;
    float currentFrame = 0;
    PLAYER mar(martex);

    Clock clock;
    RectangleShape rectangle(Vector2f(100, 100));
    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time = time / 300;
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        Vector2i pos = Mouse::getPosition(window);


            if (event.type == Event::MouseButtonPressed)
                if (event.key.code == Mouse::Left) 
                {
                    tempX = (pos.x + offsetX)/100;
                    tempY = (pos.y+offsetY)/100;
                    Build(tempX, tempY);
                }
            if (event.key.code == Mouse::Right)
            {
                tempX = (pos.x + offsetX) / 100;
                tempY = (pos.y + offsetY) / 100;
                Destroy(tempX, tempY);
            }
            
        if (Keyboard::isKeyPressed(Keyboard::D))
        {
            mar.dx = 0.1;
        }
        if (Keyboard::isKeyPressed(Keyboard::A))
        {
            mar.dx = -0.1;
        }
        if (Keyboard::isKeyPressed(Keyboard::Space))
        {
            mar.dy = -0.2;
            mar.onGround = false;
        }

        mar.update(time);

        offsetX = mar.rect.left - 1600/2;
        offsetY = mar.rect.top - 900/2;
        window.clear(Color::White);
        int i = 0; int j = 0;
        for (int i = 0; i < H; i++)
            for (int j = 0; j < W; j++)
            {
                if (TileMap[i][j] == 'B') rectangle.setFillColor(Color::Black);

                if (TileMap[i][j] == ' ') continue;

                rectangle.setPosition(j * 100-offsetX, i * 100-offsetY);
                window.draw(rectangle);
            }

        window.draw(mar.sprite);
        window.display();
    }

    return 0;
}