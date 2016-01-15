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

sf::Vector2f screenToGrid(sf::Vector2f tpos)
{
    return sf::Vector2f( tpos.x / TILE_SIZE, tpos.y / TILE_SIZE);
}

//returns list of grids in a line between point 1 and point 2
std::vector< sf::Vector2f> gridLine(int x1, int y1, int x2, int y2)
{
    std::vector< sf::Vector2f> gridsOnLine;

    //convert points to floats to get line equation from middle of grid points
    float x1f = float(x1) + 0.5f;
    float y1f = float(y1) + 0.5f;
    float x2f = float(x2) + 0.5f;
    float y2f = float(y2) + 0.5f;

    //calculate angle between points using first point as origin
    //the angle is used to determine which algorithm to use ( y=mx+b or x=(b-y)/m )
    float angle = acos( (x2f-x1f)  / sqrtf( powf(x2f-x1f, 2) + powf(y2f-y1f, 2)  ));
    if(y2f > y1f) angle = 2*3.14159 - angle;
    angle = angle * 180 / 3.14159;
    //std::cout << "angle=" << angle << std::endl;


    //if point 1 and point 2 are the same
    if(x1 == x2 && y1 == y2)
    {
        gridsOnLine.push_back(sf::Vector2f(x1, y1));
        return gridsOnLine;
    }

    //if line is vertical
    if(x2 - x1 == 0)
    {
        //add point 1
        gridsOnLine.push_back(sf::Vector2f(x1, y1));

        for(int i = y1; i != y2; i += (y2-y1)/abs(y2-y1))
        {
            if(i == y1) continue;
            gridsOnLine.push_back( sf::Vector2f(x1, i) );
        }

        //add point 2
        gridsOnLine.push_back( sf::Vector2f(x2, y2));
        return gridsOnLine;
    }

    //get equation of a line
    // y = mx + b
    // b = y - mx
    // x = (b - y)/m
    float m = (y2f - y1f) / (x2f - x1f);
    float b = y1f - (m*x1f);

    //debug
    //std::cout << "y = " << m << "*x + " << b << std::endl;

    //add point 1
    gridsOnLine.push_back(sf::Vector2f(x1, y1));

    //if angle between points is more lateral, use x = (b-y)/m
    if( (angle > 45 && angle < 135) || (angle > 225 && angle < 315) )
    {
        for(float i = y1f; i != y2f; i += (y2f-y1f)/fabs(y2f-y1f))
        {
            if(i == y1f) continue;
            //std::cout << " x = (b-i)/m   =  " << round((b-i)/m) << "=" << "(" << b << " - " << i << ") / " << m << std::endl;
            //had to add a -1 to the x-axis, everything is off by 1?  not sure why, also floor is negative because y-axis is flipped
            gridsOnLine.push_back( sf::Vector2f( -floor((b-i)/m)-1, floor(i) ) );
        }
    }
    //if angle between points is more longitudinal use y=mx+b
    else
    {
        for(float i = x1f; i != x2f; i += (x2f-x1f)/fabs(x2f-x1f))
        {
            if(i == x1f) continue;
            gridsOnLine.push_back( sf::Vector2f( floor(i), floor(m*i + b)) );
        }
    }

    //add point #2 at the end
    gridsOnLine.push_back( sf::Vector2f(x2,y2));


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

    sf::Vector2f p1(273,307);
    sf::Vector2f p2(565,180);

    while(!quit)
    {
        sf::Event event;

        screen->clear();

        //capture mouse position
        sf::Vector2f mousePos = sf::Vector2f( sf::Mouse::getPosition(*screen));

        //convert mouse position into grid coordinates
        sf::Vector2f mousePosGrid = screenToGrid(mousePos);

        //std::cout << "mouse: " << mousePos.x << "," << mousePos.y << "  -  " << mousePosGrid.x << "," << mousePosGrid.y << std::endl;

        //if mouse cursor is within window, handle events
        if(screen->getViewport( screen->getView()).contains(sf::Vector2i(mousePos)) && screen->hasFocus())
        {
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) p1 = mousePos;
            else if(sf::Mouse::isButtonPressed(sf::Mouse::Right)) p2 = mousePos;
        }


        while(screen->pollEvent(event) )
        {
            if(!screen->hasFocus()) break;

            if(event.type == sf::Event::Closed) quit = true;
            else if(event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::Escape) quit = true;
            }
            else if(event.type == sf::Event::MouseButtonPressed && screen->getViewport( screen->getView()).contains(sf::Vector2i(mousePos)) )
            {
                std::cout << "screenpos : " << mousePos.x << "," << mousePos.y << std::endl;
                std::cout << "gridpos : " << mousePosGrid.x << "," << mousePosGrid.y << std::endl;

                if(event.mouseButton.button == sf::Mouse::Left)
                {
                }
            }
            else if(event.type == sf::Event::MouseButtonReleased)
            {


            }
        }


        //update


        //draw
            //DRAW GRID
            for(int i = 0; i < MAP_HEIGHT; i++)
            {
                for(int n = 0; n < MAP_WIDTH; n++)
                {
                    drawGrid(screen, n, i);
                }
            }

            //convert points to grids
            sf::Vector2f p1grid = screenToGrid(p1);
            sf::Vector2f p2grid = screenToGrid(p2);



            //draw grids on a line
            std::vector< sf::Vector2f> mygrids = gridLine(p1grid, p2grid);
            for(int i = 0; i < int(mygrids.size()); i++)
            {
                drawGrid( screen, mygrids[i].x, mygrids[i].y, sf::Color::Green, sf::Color::Transparent);
            }

            //DRAW POINT 1 and POINT 2 grid locations
            drawGrid(screen, p1grid.x, p1grid.y, sf::Color(0,0,128,128), sf::Color::Transparent);
            drawGrid(screen, p2grid.x, p2grid.y, sf::Color(0,0,128,128), sf::Color::Transparent);

        //draw line
        sf::VertexArray myline(sf::Lines, 2);
        myline[0].position = p1;
        myline[0].color = sf::Color::Red;
        myline[1].position = p2;
        myline[1].color = sf::Color::Red;
        screen->draw(myline);

        //screen update
        screen->display();
    }


    return 0;
}
