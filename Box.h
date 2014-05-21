#ifndef BOX_H
#define BOX_H

#include <SFML/Graphics.hpp>
#include <Box2d/Box2d.h>

class Box
{
    public:
        Box(b2World& world, int x, int y, float density);
        virtual ~Box();
        sf::Sprite& GetSprite(){return mSprite;}
        b2Body* GetBody(){return body;}
        void Update();
    protected:
    private:
        sf::Sprite mSprite;

        b2BodyDef bodyDef;
        b2Body* body;
        b2CircleShape shapeDef;
        b2FixtureDef cartFixture;
        float32 lastAngle;
        b2World& mWorld;
//        UserData mData;
};
#endif // BOX_H
