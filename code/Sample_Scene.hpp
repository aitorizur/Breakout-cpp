/*
 * GAME SCENE
 * Copyright © 2021+ Aitor Izurrategui de Castro
 *
 * aitorizur@hotmail.com
 */

#include <memory>
#include <basics/Scene>
#include <basics/Texture_2D>
#include "Sprite.hpp"

namespace basics
{
    class Sample_Scene : public basics::Scene
    {

    private:

        ///////////////// Basic scene control setup ///////////////////

        enum State
        {
            LOADING,
            RUNNING,
        };

        State          state;
        bool           suspended;

        unsigned       canvas_width;
        unsigned       canvas_height;

        /////////////////////////////////////////////////////////

        ///////////////// Basic scene control setup ///////////////////

        std::list<std::shared_ptr<Sprite>> sceneSpritesList;

        /////////////////////////////////////////////////////////

    public:

        Sample_Scene();

        basics::Size2u get_view_size () override
        {
            return { canvas_width, canvas_height };
        }

        bool initialize () override;
        void suspend    () override;
        void resume     () override;

        void handle     (basics::Event & event) override;
        void update     (float time) override;
        void render     (basics::Graphics_Context::Accessor & context) override;

    private:

        void load ();
        void run  (float time);
        float CalculateDistance(basics::Point2f firstPoint, basics::Point2f secondPoint);
    };

}
