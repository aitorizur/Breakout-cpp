/*
 * SAMPLE SCENE
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
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



namespace example
{

    Sample_Scene::Sample_Scene()
    {
        canvas_width  = 1280;
        canvas_height =  720;
    }

    bool Sample_Scene::initialize ()
    {
        state     = LOADING;
        suspended = false;
        x         = 640;
        y         = 360;
        xBounce = 640;
        yBounce = 360;
        yDirection = 1;
        xDirection = 1;
        sampleWidth = 100;
        sampleHeight = 100;

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
                case ID(touch-moved):
                case ID(touch-ended):
                {
                    x = *event[ID(x)].as< var::Float > ();
                    y = *event[ID(y)].as< var::Float > ();
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
                canvas->draw_point   ({ 360, 360 });
                canvas->draw_segment ({   0,   0 }, { 1280, 720 });
                canvas->draw_segment ({   0, 720 }, { 1280,   0 });

                if (texture)
                {
                    canvas->fill_rectangle ({ x, y }, { sampleWidth, sampleHeight }, texture.get ());
                    canvas->fill_rectangle ({ xBounce, yBounce }, { sampleWidth, sampleHeight }, texture.get ());
                }
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
                texture = Texture_2D::create (ID(test), context, "test.png");

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
        if  (yBounce > 670)
        {
          yDirection = -1;
        }
        else if (yBounce < 50)
        {
            yDirection = 1;
        }

        if  (xBounce > 1230)
        {
            xDirection = -1;
        }
        else if (xBounce < 50)
        {
            xDirection = 1;
        }

        yBounce+= 100 * time * yDirection;
        xBounce+= 100 * time * xDirection;

        if  (CalculateDistance({xBounce, yBounce}, {x, y}) < 100)
        {
            sampleHeight = 200;
            sampleWidth = 200;
        }
        else
        {
            sampleHeight = 100;
            sampleWidth = 100;
        }
    }

    float Sample_Scene::CalculateDistance(basics::Point2f firstPoint, basics::Point2f secondPoint)
    {
        float distance = std::abs(sqrt(pow(firstPoint[0] - secondPoint[0], 2) + pow( firstPoint[1] - secondPoint[1], 2)));
        return distance;
    }
}
