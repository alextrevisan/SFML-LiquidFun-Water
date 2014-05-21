#include "Box.h"
#include "Manager.h"

Box::Box(b2World& world)
:mSprite(Manager::LoadImage("data/box_texture.jpg"))
,mWorld(world)
{
    mSprite.setOrigin(0,0);
    mSprite.setPosition(512,384);
    //mSprite.scale(0.4,0.4);

    b2Vec2 mVertices[4];
    mVertices[0].x = 0/METERTOPIXEL;
    mVertices[1].x = 72/METERTOPIXEL;
    mVertices[2].x = 72/METERTOPIXEL;
    mVertices[3].x = 0/METERTOPIXEL;

    mVertices[0].y = 0/METERTOPIXEL;
    mVertices[1].y = 0/METERTOPIXEL;
    mVertices[2].y = 72/METERTOPIXEL;
    mVertices[3].y = 72/METERTOPIXEL;
    bodyDef.position.Set(512/METERTOPIXEL, 384/METERTOPIXEL);
    bodyDef.type = b2_dynamicBody;
    body = world.CreateBody(&bodyDef);

    b2PolygonShape triangleDef;
    triangleDef.Set(mVertices,4);

    cartFixture.density = 1000.0;
    cartFixture.friction = 1.0;
    cartFixture.restitution = 100.95;
    cartFixture.shape = &triangleDef;

    body->CreateFixture(&cartFixture);

    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(512/METERTOPIXEL, 384/METERTOPIXEL);

    body = world.CreateBody(&bodyDef);
    // Define another box shape for our dynamic body.
    mSprite.setPosition(body->GetPosition().x*METERTOPIXEL,body->GetPosition().y*METERTOPIXEL);
    //lastAngle = body->GetAngle()* (180.0f / 3.141569);
    //ctor
}

Box::Box(b2World& world, int x, int y, float density)
:mSprite(density>1?Manager::LoadImage("data/steel_box_texture.jpg"):Manager::LoadImage("data/box_texture.jpg"))
,mWorld(world)
{

    mSprite.setOrigin(36,36);
    mSprite.setPosition(x,y);
    //mSprite.scale(0.4,0.4);

    b2Vec2 mVertices[4];
    mVertices[0].x = -36/METERTOPIXEL;
    mVertices[1].x = 36/METERTOPIXEL;
    mVertices[2].x = 36/METERTOPIXEL;
    mVertices[3].x = -36/METERTOPIXEL;

    mVertices[0].y = -36/METERTOPIXEL;
    mVertices[1].y = -36/METERTOPIXEL;
    mVertices[2].y = 36/METERTOPIXEL;
    mVertices[3].y = 36/METERTOPIXEL;
    bodyDef.position.Set(x/METERTOPIXEL, y/METERTOPIXEL);
    bodyDef.type = b2_dynamicBody;
    body = world.CreateBody(&bodyDef);

    b2PolygonShape triangleDef;
    triangleDef.Set(mVertices,4);

    cartFixture.density = density;
    cartFixture.friction = 1.0;
    cartFixture.restitution = 0.5;
    cartFixture.shape = &triangleDef;

    body->CreateFixture(&cartFixture);

    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x/METERTOPIXEL, y/METERTOPIXEL);

    // Define another box shape for our dynamic body.
    mSprite.setPosition(body->GetPosition().x*METERTOPIXEL,body->GetPosition().y*METERTOPIXEL);
    lastAngle = body->GetAngle()* (180.0f / 3.141569);
    //ctor
}
void Box::Update()
{
    mSprite.setPosition(body->GetPosition().x*METERTOPIXEL, body->GetPosition().y*METERTOPIXEL);
    if(body->GetAngle()* (180.0f / 3.141569f)!= lastAngle)
    {
        mSprite.rotate(-lastAngle + body->GetAngle() * (180.0f / 3.141569));
        lastAngle = body->GetAngle()* (180.0f / 3.141569);
    }
}

Box::~Box()
{
    mWorld.DestroyBody(body);
    body = NULL;
    //dtor
}
