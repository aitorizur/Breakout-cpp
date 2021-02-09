/*
 * GAME SCENE
 * Copyright © 2021+ Aitor Izurrategui de Castro
 *
 * aitorizur@hotmail.com
 */

#include "Sample_Scene.hpp"
#include <basics/Canvas>
#include <basics/Director>
#include <basics/Log>
#include <basics/Scaling>
#include <basics/Rotation>
#include <basics/Translation>

using namespace basics;
using namespace std;



namespace basics
{

    Sample_Scene::Sample_Scene()
    {
        canvas_width  = 1920;
        canvas_height =  1080;
    }

    bool Sample_Scene::initialize ()
    {
        state     = LOADING;
        suspended = false;

        return true;
    }

    void Sample_Scene::suspend ()
    {
        suspended = true;
    }

    void Sample_Scene::resume ()
    {
        suspended = false;
    }

    void Sample_Scene::handle (Event & event)
    {
        if (state == RUNNING)
        {
            switch (event.id)
            {
                case ID(touch-started):
                {

                    break;
                }
                case ID(touch-moved):
                {

                    break;
                }
                case ID(touch-ended):
                {
                    /*
                    x = *event[ID(x)].as< var::Float > ();
                    y = *event[ID(y)].as< var::Float > ();
                     */
                    break;
                }
            }
        }
    }

    void Sample_Scene::update (float time)
    {
        switch (state)
        {
            case LOADING: load ();     break;
            case RUNNING: run  (time); break;
        }
    }

    void Sample_Scene::render (basics::Graphics_Context::Accessor & context)
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

            }
        }
    }

    void Sample_Scene::load ()
    {
        if (!suspended)
        {
            Graphics_Context::Accessor context = director.lock_graphics_context ();

            if (context)
            {

                if (texture)
                {
                    context->add (texture);

                    state = RUNNING;
                }
            }
        }
    }

    void Sample_Scene::run (float time)
    {

    }

    float Sample_Scene::CalculateDistance(basics::Point2f firstPoint, basics::Point2f secondPoint)
    {
        float distance = std::abs(sqrt(pow(firstPoint[0] - secondPoint[0], 2) + pow( firstPoint[1] - secondPoint[1], 2)));
        return distance;
    }
}
