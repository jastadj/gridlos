#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

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
    //add point 1 and point 2
    //gridsOnLine.push_back( sf::Vector2f(x1, y1));
    //gridsOnLine.push_back( sf::Vector2f(x2, y2));

    return gridsOnLine;

}

int main(int argc, char *argv[])
{

    sf::RenderWindow *screen = new sf::RenderWindow( sf::VideoMode(MAP_WIDTH * TILE_SIZE, MAP_HEIGHT * TILE_SIZE,32), "Test");

    bool quit = false;

    while(!quit)
    {
        sf::Event event;

        screen->clear();

        while(screen->pollEvent(event) )
        {
            if(event.type == sf::Event::Closed) quit = true;
            else if(event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::Escape) quit = true;
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
        std::vector< sf::Vector2f> mygrids = gridLine(2,2, 2,19);
        for(int i = 0; i < int(mygrids.size()); i++)
        {
            drawGrid( screen, mygrids[i].x, mygrids[i].y, sf::Color::Green, sf::Color::Transparent);
        }


        //screen update
        screen->display();
    }


    return 0;
}
