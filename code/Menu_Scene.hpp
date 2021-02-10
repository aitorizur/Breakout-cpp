/*
 * MENU SCENE
 * Copyright © 2021+ Aitor Izurrategui de Castro
 *
 * aitorizur@hotmail.com
 */
/*
#include <memory>
#include <basics/Scene>
#include <basics/Texture_2D>
#include <basics/Timer>
#include "Sprite.hpp"

using namespace std;

namespace basics
{
    class Menu_Scene : public basics::Scene
    {

    private:

        bool           suspended;

        unsigned       canvas_width;
        unsigned       canvas_height;

        Timer timer;

        shared_ptr<Texture_2D> playButtonTexture;
        shared_ptr<Sprite> playButton;


    public:

        Menu_Scene();

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

        void LoadTexture ();
        void CreateSprite ();
        bool CheckButtonClick(Event &event, Sprite *sprite);

    };

}
*/