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

using namespace basics;
using namespace std;



namespace example
{

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
                case ID(touch-started):
                case ID(touch-moved):
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
                canvas->set_color    (1, 1, 1);
                canvas->draw_point   ({ 360, 360 });
                canvas->draw_segment ({   0,   0 }, { 1280, 720 });
                canvas->draw_segment ({   0, 720 }, { 1280,   0 });

                //canvas->draw_text()

                if (texture)
                {

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
            }
        }
    }

    void Menu_Scene::run (float time)
    {

    }
}
