/*
 * MENU SCENE
 * Copyright © 2021+ Aitor Izurrategui de Castro
 *
 * aitorizur@hotmail.com
 */
/*
#include "Menu_Scene.hpp"
#include "Sample_Scene.hpp"
#include <basics/Canvas>
#include <basics/Director>
#include <basics/Transformation>

using namespace basics;
using namespace std;

namespace basics
{

    // ---------------------------------------------------------------------------------------------

    bool Menu_Scene::initialize ()
    {
        canvas_width  = 1920;
        canvas_height =  1080;

        //Se carga la textura y se crea e sprite
        LoadTexture();
        CreateSprite();

        return true;
    }

    // ---------------------------------------------------------------------------------------------

    void Menu_Scene::handle (basics::Event & event)
    {
        switch (event.id)
        {
            case ID(touch-started):         // El usuario toca la pantalla
            {
                // Si el usuario toca el boton de play se inicia la escena de juego
                if (CheckButtonClick(event, playButton.get()))
                {
                    director.run_scene (shared_ptr< Scene >(new Sample_Scene));
                }

                break;
            }
            case ID(touch-moved):
            {

                break;
            }

            case ID(touch-ended):           // El usuario deja de tocar la pantalla
            {

                break;
            }
        }

    }


    // ---------------------------------------------------------------------------------------------

    void Menu_Scene::render (Graphics_Context::Accessor & context)
    {
        if (!suspended)
        {
            // El canvas se puede haber creado previamente, en cuyo caso solo hay que pedirlo:

            Canvas * canvas = context->get_renderer< Canvas > (ID(canvas));

            // Si no se ha creado previamente, hay que crearlo una vez:

            if (!canvas)
            {
                 canvas = Canvas::create (ID(canvas), context, {{ canvas_width, canvas_height }});
            }

            // Si el canvas se ha podido obtener o crear, se puede dibujar con él:

            if (canvas)
            {
                canvas->clear ();
                //Se rendea el boton de jugar
                playButton->render(*canvas);
            }
        }
    }

    //Crea la textura del boton de jugar
    void Menu_Scene::LoadTexture()
    {
        Graphics_Context::Accessor context = director.lock_graphics_context ();

        if (context)
        {
            playButtonTexture = Texture_2D::create (ID(gorillaTextureLeft), context, "gorilla-arm-up-2.png");
            context->add (playButtonTexture);
        }
    }

    //Crea el sprite correspondiente a la textura del boton de jugar
    void Menu_Scene::CreateSprite()
    {
        playButton = shared_ptr<Sprite>(new Sprite(playButtonTexture.get()));
        playButton->set_anchor(BOTTOM | LEFT);
        playButton->set_position({ canvas_width/2 - playButton->get_width() / 2, canvas_height/2 - playButton->get_height() / 2});
    }

    //Funcion que comprueba si un sprite esta siendo tocado
    bool Menu_Scene::CheckButtonClick(Event &event, Sprite *sprite)
    {
        if (sprite->contains({*event[ID(x)].as<var::Float>(), *event[ID(y)].as<var::Float>()}))
        {
            return true;
        } else
        {
            return false;
        }
    }
}
*/