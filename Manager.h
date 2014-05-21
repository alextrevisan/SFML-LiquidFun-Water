#ifndef MANAGER_H
#define MANAGER_H

#include <SFML/Graphics.hpp>
#include <Box2d/Box2d.h>
#define METERTOPIXEL 50.0f

class Manager
{
    public:
        static sf::RenderWindow* getRenderWindow()
        {
            static sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(1280, 720), "SFML_liquidfun");
            return window;
        }
        static sf::Texture& LoadImage(const std::string& img)
        {
            if(mImageList.count(img)<=0)
            {
                sf::Image tmp;
                tmp.loadFromFile(img);
                tmp.createMaskFromColor(sf::Color(255,0,255),0);
                sf::Texture *tex = new sf::Texture();
                tex->loadFromImage(tmp);
                mImageList[img] = tex;
            }
            return *mImageList[img];
        }
        static std::map<std::string,sf::Texture*> mImageList;
};

#endif // MANAGER_H
