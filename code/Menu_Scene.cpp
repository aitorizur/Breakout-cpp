/*
 * SAMPLE SCENE
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#include "Menu_Scene.hpp"
#include <basics/Canvas>
#include <basics/Director>
#include <basics/Log>
#include <basics/Scaling>
#include <basics/Rotation>
#include <basics/Translation>
#include <basics/internal/Raster_Font.hpp>

using namespace basics;
using namespace std;



namespace example
{
    int  Speed;
    basics::Vector2f velocityVector;

    Menu_Scene::Menu_Scene()
    {
        canvas_width  = 1280;
        canvas_height =  720;
    }

    bool Menu_Scene::initialize ()
    {
        state     = LOADING;
        suspended = false;

        return true;
    }

    void Menu_Scene::suspend ()
    {
        suspended = true;
    }

    void Menu_Scene::resume ()
    {
        suspended = false;
    }

    void Menu_Scene::handle (Event & event)
    {
        if (state == RUNNING)
        {
            switch (event.id)
            {
                case ID(touch-started): {break;}
                case ID(touch-moved): {break;}
                case ID(touch-ended):
                {
                    //x = *event[ID(x)].as< var::Float > ();
                    //y = *event[ID(y)].as< var::Float > ();
                    break;
                }
            }
        }
    }

    void Menu_Scene::update (float time)
    {
        switch (state)
        {
            case LOADING: load ();     break;
            case RUNNING: run  (time); break;
        }
    }

    void Menu_Scene::render (basics::Graphics_Context::Accessor & context)
    {
        if (!suspended && state == RUNNING)
        {
            Canvas * canvas = context->get_renderer< Canvas > (ID(canvas));

            if (!canvas)
            {
                canvas = Canvas::create (ID(canvas), context, {{ canvas_width, canvas_height }});
            }

            if (canvas)
            {
                canvas->clear        ();
                canvas->set_color    (255, 255, 255);

                testSprite->render(*canvas);
                bottomBorder->render(*canvas);
                topBorder->render(*canvas);
            }
        }
    }

    void Menu_Scene::load ()
    {
        if (!suspended)
        {
            Graphics_Context::Accessor context = director.lock_graphics_context ();

            if (context)
            {
                ballTexture = Texture_2D::create (ID(test), context, "ball.png");
                horizontalLimitTexture = Texture_2D::create(ID(topBorderTest),context, "horizontal-bar.png");

                if (ballTexture)
                {
                    context->add (ballTexture);
                    context->add(horizontalLimitTexture);
                    state = RUNNING;
                }

                testSprite = shared_ptr<Sprite>(new Sprite(ballTexture.get()));
                bottomBorder = shared_ptr<Sprite>(new Sprite(horizontalLimitTexture.get()));
                topBorder = shared_ptr<Sprite>(new Sprite(horizontalLimitTexture.get()));

                bottomBorder->set_position({640,15});
                topBorder->set_position({640,705});

                testSprite->set_position({200,200});
                testSprite->set_scale(1.f);
                testSprite->set_speed({50, 400});
            }
        }
    }

    void Menu_Scene::run (float time)
    {
        testSprite->update(time);

        if (testSprite->intersects(*topBorder))
        {
            testSprite->set_position_y(topBorder->get_bottom_y() - testSprite->get_height()/2.f);
            testSprite->set_speed_y(-testSprite->get_speed_y());
        }
        else if (testSprite->intersects(*bottomBorder))
        {
            testSprite->set_position_y(bottomBorder->get_top_y() + testSprite->get_height()/2.f);
            testSprite->set_speed_y(-testSprite->get_speed_y());
        }

    }
}
