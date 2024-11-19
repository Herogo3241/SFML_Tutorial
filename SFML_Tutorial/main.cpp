#include <SFML/Graphics.hpp>
#include <random>

#define SPEED 400.f



int main()
{

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> direction(0, 3);

    std::uniform_real_distribution<> distance(1, 5);

    std::vector<int> directions = { 0, 1, 2, 3 };

    

    float moveX = 0.f;
    float moveY = 0.f;
    

    sf::ContextSettings settings;
    settings.antialiasingLevel = 1;

    sf::RenderWindow window(sf::VideoMode(800, 800), "Dino Game", sf::Style::Default | sf::Style::Close, settings);


    sf::CircleShape shape(2.f);
    shape.setFillColor(sf::Color::Yellow);
    shape.setOrigin(shape.getRadius(), shape.getRadius());
    shape.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);

    float distanceX = window.getSize().x / 2.f;
    float distanceY = window.getSize().y / 2.f;




    sf::Clock clock;
    

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            }
        

        }


        float deltaTime = clock.restart().asSeconds();
        printf("%.2f\n", deltaTime);

       

        

        if (abs(shape.getPosition().x) < abs(distanceX) || abs(shape.getPosition().y) < abs(distanceY)) {
            shape.move(moveX , moveY );
        }
        else {

            moveX = 0.f;
            moveY = 0.f;

           
            std::shuffle(directions.begin(), directions.end(), gen);
            int randomNumber = directions.front();
            int randomDistance = distance(gen);

            printf("%d %d\n", randomNumber, randomDistance);

            switch (randomNumber) {
            case 0:
                moveY = -1.f;
                //distanceY = -abs(distanceY);
                distanceY -= randomDistance;
                break;
            case 1:
                moveX = -1.f;
                //distanceX = -abs(distanceX);
                distanceX -= randomDistance;
                break;
            case 2:
                moveY = 1.f;
                //distanceY = abs(distanceY);
                distanceY += randomDistance;
                break;
            case 3:
                moveX = 1.f;
                //distanceX = abs(distanceX);
                distanceX += randomDistance;
                break;
            }
        }

        

     

        

        



        //window.clear();
        window.draw(shape);

        window.display();
    }

    return 0;
}