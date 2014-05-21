#include <iostream>
#include <deque>
#include "Manager.h"
#include "Box.h"
int main()
{
    sf::RenderWindow* App = Manager::getRenderWindow();

    App->setFramerateLimit(60);
	b2AABB worldAABB;
	worldAABB.lowerBound.Set(-2000.0f, -2000.0f);
	worldAABB.upperBound.Set(2000.0f, 2000.0f);
	b2Vec2 gravity(0.0f, 9.8f);
	b2World world(gravity);

    //Ground
    b2Vec2 mVertices[4];
    mVertices[0].x = 0/METERTOPIXEL;
    mVertices[1].x = 800/METERTOPIXEL;
    mVertices[2].x = 800/METERTOPIXEL;
    mVertices[3].x = 0/METERTOPIXEL;

    mVertices[0].y = 0/METERTOPIXEL;
    mVertices[1].y = -60/METERTOPIXEL;
    mVertices[2].y = 10/METERTOPIXEL;
    mVertices[3].y = 10/METERTOPIXEL;

    b2BodyDef bodyDef;
    bodyDef.position.Set(100/METERTOPIXEL, 500/METERTOPIXEL);
    bodyDef.type = b2_staticBody;
    b2Body* body = world.CreateBody(&bodyDef);
    b2PolygonShape triangleDef;
    triangleDef.Set(mVertices,4);

    b2FixtureDef polyFixture;
    polyFixture.shape = &triangleDef;
    body->CreateFixture(&polyFixture);
    //Timestep
	float32 timeStep = 1.0f / 60.0f;
	int32 iterations = 8;

    sf::Sprite background(Manager::LoadImage("data/background.jpg"));
    background.setOrigin(0,0);
    background.setPosition(0,0);

    std::deque<Box*> box;

    sf::Shader thresholdShader;
    if(!thresholdShader.loadFromFile("data/threshold.glsl", sf::Shader::Fragment))
    {
        std::cout<<"Erro ao carregar shader\n";
    }

    sf::Shader blurh;
    if(!blurh.loadFromFile("data/blurh.glsl", sf::Shader::Fragment))
    {
        std::cout<<"Erro ao carregar shader\n";
    }

    sf::Shader blurv;
    if(!blurv.loadFromFile("data/blurv.glsl", sf::Shader::Fragment))
    {
        std::cout<<"Erro ao carregar shader\n";
    }

    sf::RenderTexture backgroundtext;
    backgroundtext.create(1280,720);
    sf::RenderTexture backgroundtext2;
    backgroundtext2.create(1280,720);
    sf::RenderTexture backgroundtext3;
    backgroundtext3.create(1280,720);


    b2ParticleSystemDef particleSystemDef;
    particleSystemDef.radius = 4/METERTOPIXEL;
    b2ParticleSystem* m_particleSystem = world.CreateParticleSystem(&particleSystemDef);
    thresholdShader.setParameter("resolution", sf::Vector2f(128.0,96.0));

    sf::Sprite water(Manager::LoadImage("data/water.png"));
    water.setOrigin(8,8);

    sf::Clock deltaClock;
    while (App->isOpen())
    {

        sf::Event Event;
        while (App->pollEvent(Event))
        {
            if (Event.type == sf::Event::Closed)
                App->close();
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
                App->close();

            if (Event.type == sf::Event::MouseButtonPressed)
            {
                if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
                {
                    sf::Vector2i mouse_pos = sf::Mouse::getPosition(*App);
                    box.push_back(new Box(world,mouse_pos.x,mouse_pos.y,5));
                }
                if(sf::Mouse::isButtonPressed(sf::Mouse::Middle))
                {
                    sf::Vector2i mouse_pos = sf::Mouse::getPosition(*App);
                    box.push_back(new Box(world,mouse_pos.x,mouse_pos.y,0.9));
                }
            }
        }
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            sf::Vector2i mouse_pos = sf::Mouse::getPosition(*App);
			b2ParticleDef pd;
			pd.lifetime = 60;
			pd.flags = b2_springParticle;
			pd.velocity = b2Vec2(0,5);

			pd.position.Set(mouse_pos.x/METERTOPIXEL,mouse_pos.y/METERTOPIXEL);
			m_particleSystem->CreateParticle(pd);

			pd.position.Set((mouse_pos.x-8)/METERTOPIXEL,mouse_pos.y/METERTOPIXEL);
			m_particleSystem->CreateParticle(pd);

			pd.position.Set((mouse_pos.x+8)/METERTOPIXEL,mouse_pos.y/METERTOPIXEL);
			m_particleSystem->CreateParticle(pd);

			pd.position.Set((mouse_pos.x-16)/METERTOPIXEL,mouse_pos.y/METERTOPIXEL);
			m_particleSystem->CreateParticle(pd);

			pd.position.Set((mouse_pos.x+16)/METERTOPIXEL,mouse_pos.y/METERTOPIXEL);
			m_particleSystem->CreateParticle(pd);
        }


        App->clear();
        world.Step(timeStep, iterations,2);

        backgroundtext.clear(sf::Color(0, 0, 0, 0));
        backgroundtext2.clear(sf::Color(0, 0, 0, 0));
        backgroundtext3.clear(sf::Color(0, 0, 0, 0));

        App->draw(background);
        //Render particle
        for(int x = 0; x < m_particleSystem->GetParticleCount();++x)
        {
            b2Vec2 pos = m_particleSystem->GetPositionBuffer()[x];
            water.setPosition(pos.x*METERTOPIXEL,pos.y*METERTOPIXEL);
            backgroundtext.draw(water);
        }
        backgroundtext.display();
        //First blur pass
        sf::Sprite sprite(backgroundtext.getTexture());
        backgroundtext2.draw(sprite,&blurh);
        backgroundtext2.display();
        //Second blur pass
        sf::Sprite sprite2(backgroundtext2.getTexture());
        backgroundtext3.draw(sprite2,&blurv);
        backgroundtext3.display();
        //Water shader
        sf::Sprite sprite3(backgroundtext3.getTexture());
        thresholdShader.setParameter("time",deltaClock.getElapsedTime().asSeconds());
        App->draw(sprite3, &thresholdShader);

        //Draw boxes
        for(std::deque<Box*>::iterator it = box.begin();it!=box.end();++it)
        {
            (*it)->Update();
            App->draw((*it)->GetSprite());
        }

        //Draw ground
        b2Fixture *l = body->GetFixtureList();
        while(l!=NULL)
        {
            b2PolygonShape *s = (b2PolygonShape*)l[0].GetShape();
            sf::ConvexShape Polygon;
            Polygon.setPointCount(4);
            for(int i = 0; i < 4; ++i)
            {
                Polygon.setPoint(i, sf::Vector2f(body->GetWorldPoint(s->GetVertex(i)).x*METERTOPIXEL,
                                 body->GetWorldPoint(s->GetVertex(i)).y*METERTOPIXEL));
            }
            Polygon.setOutlineColor(sf::Color(250, 150, 100));
            App->draw(Polygon);
            l = l->GetNext();
        }

        App->display();
    }

    return 0;
}
