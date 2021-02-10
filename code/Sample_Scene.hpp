/*
 * GAME SCENE
 * Copyright © 2021+ Aitor Izurrategui de Castro
 *
 * aitorizur@hotmail.com
 */

#include <memory>
#include <basics/Scene>
#include <basics/Texture_2D>
#include <basics/Timer>
#include "Sprite.hpp"

using namespace std;

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

        Timer timer;

        /////////////////////////////////////////////////////////

        ///////////////// Variables relacionadas con la carga de texturas y sprites ///////////////////

        //Creamos un struct que contendra la ruta de las texturas junto con un ID
        static struct   Texture_Data { Id id; const char * path; } textures_data[];

        //Numero de texturas que hay en el array la informacion de las texturas
        static unsigned textures_count;

        //Mapa en el que se guardan shared_ptr a las texturas cargadas
        map<Id, shared_ptr<Texture_2D>> textures;
        //Lista en la que se guardan shared_ptr a los sprites creados
        list<shared_ptr<Sprite>> sprites;

        /////////////////////////////////////////////////////////

        ///////////////// Basic scene control setup ///////////////////

        float playerSpeed = 500;
        Vector2f playerDirection = {0, 0};

        Sprite * playerReference;

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

        void LoadTextures ();
        void CreateSprites ();
        void run  (float time);

        void MovePlayer(float time);
        void RenderLoading(Canvas & canvas);
        void RenderPlay(Canvas & canvas);

    };

}
