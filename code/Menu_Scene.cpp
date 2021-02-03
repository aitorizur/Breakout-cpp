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
    int  x,y, xTouch, yTouch;

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
                case ID(touch-moved):
                {
                    int speedX, speedY;

                    xTouch = *event[ID(x)].as< var::Float > ();
                    yTouch = *event[ID(y)].as< var::Float > ();

                    Vector2f test = Vector2f({(float )(xTouch - x),(float)(yTouch - y)});
                    test = test.normalize();

                    testSprite->set_speed({test[0] * 1000, test[1] * 1000});
                    testSprite
                    break;

                }
                case ID(touch-ended):
                {
                    //x = *event[ID(x)].as< var::Float > ();
                    //y = *event[ID(y)].as< var::Float > ();
                    testSprite->set_speed({0, 0});
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

                if (texture)
                {
                    //canvas->fill_rectangle({640,640}, {100,100}, texture.get());
                }
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
                texture = Texture_2D::create (ID(test), context, "test.png");

                if (texture)
                {
                    context->add (texture);

                    state = RUNNING;
                }

                testSprite = shared_ptr<Sprite>(new Sprite(texture.get()));
                testSprite->set_position({200,200});
                testSprite->set_scale(0.5f);
            }
        }
    }

    void Menu_Scene::run (float time)
    {
        x = testSprite->get_position_x();
        y = testSprite->get_position_y();
        testSprite->update(time);
    }
}
