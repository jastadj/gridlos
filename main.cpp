#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include <SFML\Graphics.hpp>


#define MAP_WIDTH 20
#define MAP_HEIGHT 20
#define TILE_SIZE 32

void drawGrid(sf::RenderWindow *screen, int x, int y, sf::Color gcolor = sf::Color::Transparent, sf::Color ocolor = sf::Color::Green)
{

    if(ocolor != sf::Color::Transparent)
    {
        sf::RectangleShape mygrid( sf::Vector2f(TILE_SIZE-2, TILE_SIZE-2));
        mygrid.setFillColor(gcolor);
        mygrid.setOutlineThickness(1);
        mygrid.setOutlineColor(sf::Color::Green);
        mygrid.setOrigin(-1,-1);

        mygrid.setPosition(x*TILE_SIZE, y*TILE_SIZE);

        screen->draw(mygrid);
    }
    else
    {
        sf::RectangleShape mygrid( sf::Vector2f(TILE_SIZE, TILE_SIZE));
        mygrid.setFillColor(gcolor);

        mygrid.setPosition(x*TILE_SIZE, y*TILE_SIZE);

        screen->draw(mygrid);
    }


}

std::vector< sf::Vector2f> gridLine(float x1, float y1, float x2, float y2)
{
    std::vector< sf::Vector2f> gridsOnLine;

    //if point 1 and point 2 are the same
    if(x1 == x2 && y1 == y2)
    {
        gridsOnLine.push_back(sf::Vector2f(x1, y1));
        return gridsOnLine;
    }

    //sort points so that point 1 is leftmost point
    if(x2 < x1)
    {
        float xt = x1;
        float yt = y1;

        x1 = x2;
        y1 = y2;

        x2 = xt;
        y2 = yt;
    }

    //if line is vertical
    if(x2 - x1 == 0)
    {
        if(y2 > y1)
        {
            for(int i = y1; i <= y2 - y1; i++)
            {
                gridsOnLine.push_back( sf::Vector2f(x1, i) );
            }
            return gridsOnLine;
        }
        else
        {
            for(int i = y2; i <= y1 - y2; i++)
            {
                gridsOnLine.push_back( sf::Vector2f( x1, i) );
            }
            return gridsOnLine;
        }
    }

    //get equation of a line
    // y = mx + b
    // b = y - mx
    // x = (b - y)/m
    float m = (y2 - y1) / (x2 - x1);
    float b = y1 - m*x1;

    //debug
    std::cout << "y = " << m << "*x + " << b << std::endl;

    //horizontal collection of grids
    for(int i = x1; i <= x2; i++)
    {
        gridsOnLine.push_back( sf::Vector2f( i, m*float(i) + b));
    }

    float y_low = y1;
    float y_high = y2;
    if(y_low > y_high)
    {
        y_low = y2;
        y_high = y1;
    }


    //vertical collection of grids
    for(int i = y_low; i <= y_high; i++)
    {
        if(m == 0) gridsOnLine.push_back( sf::Vector2f(x1, i));
        else
        {
            std::cout << (b-i)/m << "," << i << std::endl;
            gridsOnLine.push_back( sf::Vector2f( -floor((b-i)/m), i) );
        }
    }

    //add point 1 and point 2
    //gridsOnLine.push_back( sf::Vector2f(x1, y1));
    //gridsOnLine.push_back( sf::Vector2f(x2, y2));

    return gridsOnLine;

}

std::vector< sf::Vector2f> gridLine(sf::Vector2f p1, sf::Vector2f p2)
{
    return gridLine(p1.x, p1.y, p2.x, p2.y);
}

int main(int argc, char *argv[])
{

    sf::RenderWindow *screen = new sf::RenderWindow( sf::VideoMode(MAP_WIDTH * TILE_SIZE, MAP_HEIGHT * TILE_SIZE,32), "Test");

    bool quit = false;

    sf::Vector2f p1(2.5,2.5);
    sf::Vector2f p2(6.5,6.5);

    while(!quit)
    {
        sf::Event event;

        screen->clear();

        //capture mouse position
        sf::Vector2f mousePos = sf::Vector2f( sf::Mouse::getPosition(*screen));

        //convert mouse position into grid coordinates
        sf::Vector2f mousePosGrid = mousePos;
        mousePosGrid.x = floor(mousePosGrid.x / TILE_SIZE);
        mousePosGrid.y = floor(mousePosGrid.y / TILE_SIZE);

        std::cout << "mouse: " << mousePos.x << "," << mousePos.y << "  -  " << mousePosGrid.x << "," << mousePosGrid.y << std::endl;

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) p1 = mousePosGrid;
        else if(sf::Mouse::isButtonPressed(sf::Mouse::Right)) p2 = mousePosGrid;

        while(screen->pollEvent(event) )
        {
            if(event.type == sf::Event::Closed) quit = true;
            else if(event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::Escape) quit = true;
            }
            else if(event.type == sf::Event::MouseButtonPressed)
            {
                if(event.mouseButton.button == sf::Mouse::Left)
                {

                }
            }
        }


        //update


        //draw
        for(int i = 0; i < MAP_HEIGHT; i++)
        {
            for(int n = 0; n < MAP_WIDTH; n++)
            {
                drawGrid(screen, n, i);
            }
        }

        //draw grids on a line
        std::vector< sf::Vector2f> mygrids = gridLine(p1, p2);
        for(int i = 0; i < int(mygrids.size()); i++)
        {
            drawGrid( screen, mygrids[i].x, mygrids[i].y, sf::Color::Green, sf::Color::Transparent);
        }


        //screen update
        screen->display();
    }


    return 0;
}
