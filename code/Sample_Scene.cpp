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
    //Rutas de las texturas e IDs de las texturas que se vana usar en esta escena
    Sample_Scene::Texture_Data Sample_Scene::textures_data[] =
            {
                    {ID(ball), "gameScene/ball.png"},
                    {ID(horizontalBar), "gameScene/horizontal-bar.png"},
                    {ID(verticalBar), "gameScene/vertical-bar.png"},
                    {ID(player), "gameScene/player.png"},
                    {ID(block), "gameScene/block.png"},
            };

    //Calculamos el numero items del array dividiendo el tamano total del array entre el tamano de un elemento
    unsigned Sample_Scene::textures_count = sizeof(textures_data) / sizeof(Texture_Data);


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
                    if (*event[ID(x)].as<var::Float>() > canvas_width/2)
                    {
                        playerDirection = {1,0};
                    }
                    else
                    {
                        playerDirection = {-1,0};
                    }

                    break;
                }
                case ID(touch-moved):
                {

                    break;
                }
                case ID(touch-ended):
                {
                    playerDirection = {0,0};
                    break;
                }
            }
        }
    }

    void Sample_Scene::update (float time)
    {
        if (!suspended)
        {
            switch (state)
            {
                case LOADING:
                {
                    LoadTextures();
                    break;
                }
                case RUNNING:
                {
                    run  (time);
                    break;
                }
            }
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

                if (state == LOADING)
                {
                    RenderLoading(*canvas);
                }
                else if (state == RUNNING)
                {
                    RenderPlay(*canvas);
                }
            }
        }
    }

    /* Metodo en el que se cargan las teturas
     *  Se realiza dentro del update para poder mostrar una textura de carga mientras se muestran el resto de texturas
     */
    void Sample_Scene::LoadTextures ()
    {
        if (textures.size() < textures_count)
        {
            Graphics_Context::Accessor context = director.lock_graphics_context ();

            if (context)
            {
                //Se carga una textura por frame
                Texture_Data & texture_data = textures_data[textures.size()];
                shared_ptr<Texture_2D> & texture = textures[texture_data.id] = Texture_2D::create(texture_data.id, context, texture_data.path);

                //Si la textura se ha cargado correctamente se añade al contexto grafico
                if (texture)
                {
                    context->add(texture);
                }
            }
        }
        else if (timer.get_elapsed_seconds() > 1.f)
        {
            CreateSprites();
            state = RUNNING;
        }
    }

    void Sample_Scene::CreateSprites()
    {
        //Ordenamos los sprites en la lista para que a la hora de rendearse controlemos que sprites se rendean por encima de otros

        //PRIMERO CREAMOS LOS SPRITES DEL FONDO
        shared_ptr<Sprite> topBar(new Sprite(textures[ID(horizontalBar)].get()));
        shared_ptr<Sprite> leftBar(new Sprite(textures[ID(verticalBar)].get()));
        shared_ptr<Sprite> rightBar(new Sprite(textures[ID(verticalBar)].get()));

        topBar->set_anchor(TOP | LEFT);
        topBar->set_position({canvas_width/4, canvas_height});

        rightBar->set_anchor(TOP | LEFT);
        rightBar->set_position({canvas_width/4 - rightBar->get_width(), canvas_height});

        leftBar->set_anchor(TOP | LEFT);
        leftBar->set_position({canvas_width/4 * 3 + 6, canvas_height});

        sprites.push_back(leftBar);
        sprites.push_back(rightBar);
        sprites.push_back(topBar);

        //DESPUES CREAMOS LOS RELACIONEADOS CON EL JUEGO
        shared_ptr<Sprite> ballSprite(new Sprite(textures[ID(ball)].get()));
        shared_ptr<Sprite> player(new Sprite(textures[ID(player)].get()));

        player->set_anchor(BOTTOM | LEFT);
        player->set_position({ canvas_width/2 - player->get_width()/2, 0});

        playerReference = player.get();

        sprites.push_back(ballSprite);
        sprites.push_back(player);

        //POR ULTIMO CREAMOS TODOS LOS BLOQUES
        for (int f = 0; f < 8; ++f)
        {
            for (int i = 0; i < 14; ++i)
            {
                shared_ptr<Sprite> blockSprite(new Sprite(textures[ID(block)].get()));
                blockSprite->set_anchor(BOTTOM | LEFT);
                blockSprite->set_position({canvas_width/4 + i*blockSprite->get_width(), canvas_height/5 * 3 + f * blockSprite->get_height()});
                sprites.push_back(blockSprite);
            }
        }
    }

    void Sample_Scene::MovePlayer(float time)
    {
        playerReference->set_speed(playerDirection * playerSpeed);
    }

    void Sample_Scene::run (float time)
    {
        MovePlayer(time);

        playerReference->update(time);
    }

    void Sample_Scene::RenderLoading(Canvas & canvas)
    {

    }

    void Sample_Scene::RenderPlay(Canvas & canvas)
    {
        for (auto & sprite : sprites)
        {
            sprite->render(canvas);
        }
    }

}
