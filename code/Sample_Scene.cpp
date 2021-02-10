/*
 * GAME SCENE
 * Copyright © 2021+ Aitor Izurrategui de Castro
 *
 * aitorizur@hotmail.com
 */

#include "Sample_Scene.hpp"
#include "Intro_Scene.hpp"
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
                    {ID(loading), "gameScene/loading.png"},
                    {ID(ball), "gameScene/ball.png"},
                    {ID(horizontalBar), "gameScene/horizontal-bar.png"},
                    {ID(verticalBar), "gameScene/vertical-bar.png"},
                    {ID(player), "gameScene/player.png"},
                    {ID(block), "gameScene/block.png"},
                    {ID(leftButton), "gameScene/button-left.png"},
                    {ID(rightButton), "gameScene/button-right.png"},
                    {ID(youWin), "gameScene/you-win.png"},
                    {ID(youLost), "gameScene/you-lost.png"},
                    {ID(options), "gameScene/options.png"},
                    {ID(back), "gameScene/back.png"},
                    {ID(menuBackground), "gameScene/menu-background.png"},
                    {ID(menuButton), "gameScene/menu-button.png"},
                    {ID(helpButton), "gameScene/help-button.png"},
                    {ID(restartButton), "gameScene/restart-button.png"},
                    {ID(helpText), "gameScene/help-text.png"},
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
        gamePlayState = WAITINGSTART;
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
                    //Comprobamos si el juego ya ha terminado de cargar para comenzar a registrar los inputs
                    if (state == RUNNING)
                    {
                        //Si el juego esta en espera para empezar, y se presiona los botones de movimiento,
                        //ponemos el juego en funconamiento y le damos un impulso inicial a la pelota
                        if (gamePlayState == WAITINGSTART)
                        {
                            if (rightButtonReference->contains({*event[ID(x)].as<var::Float>(), *event[ID(y)].as<var::Float>()}) ||
                            leftButtonReference->contains({*event[ID(x)].as<var::Float>(), *event[ID(y)].as<var::Float>()}))
                            {
                                gamePlayState = PLAYING;
                                StartBall();
                            }

                            CheckPlayerMoveInput(event);

                        }
                        else if (gamePlayState == PAUSED)
                        {
                            if (rightButtonReference->contains({*event[ID(x)].as<var::Float>(), *event[ID(y)].as<var::Float>()}) ||
                                leftButtonReference->contains({*event[ID(x)].as<var::Float>(), *event[ID(y)].as<var::Float>()}))
                            {
                                gamePlayState = PLAYING;
                            }

                            CheckPlayerMoveInput(event);

                            if (CheckButtonClick(event, optionsReference))
                            {
                                gamePlayState = PAUSEDMENU;
                            }
                        }
                        else if (gamePlayState == PLAYING)
                        {
                            CheckPlayerMoveInput(event);

                            if (CheckButtonClick(event, optionsReference))
                            {
                                gamePlayState = PAUSEDMENU;
                            }
                        }
                        else if (gamePlayState == PAUSEDMENU)
                        {
                            if (CheckButtonClick(event, backReference))
                            {
                                gamePlayState = PAUSED;
                            }
                            else if (CheckButtonClick(event, restartButtonReference))
                            {
                                director.run_scene (shared_ptr< Scene >(new Sample_Scene));
                            }
                            else if (CheckButtonClick(event, menuButtonReference))
                            {
                                director.run_scene (shared_ptr< Scene >(new Intro_Scene));
                            }
                            else if (CheckButtonClick(event, helpButtonReference))
                            {
                                gamePlayState = HELPMENU;
                            }
                        }
                        else if (gamePlayState == HELPMENU)
                        {
                            if (CheckButtonClick(event, backReference))
                            {
                                gamePlayState = PAUSEDMENU;
                            }
                        }
                    }

                    break;
                }
                case ID(touch-moved):
                {

                    break;
                }
                case ID(touch-ended):
                {
                    //Solo registramos los inputs si el juego ha terminado de cargar
                    if (state == RUNNING)
                    {
                        if (gamePlayState == PLAYING)
                        {
                            playerDirection = {0, 0};
                        }
                    }

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
        Canvas * canvas = context->get_renderer< Canvas > (ID(canvas));

        if (!canvas)
        {
            canvas = Canvas::create (ID(canvas), context, {{ canvas_width, canvas_height }});
        }

        if (canvas)
        {
            if (!suspended && state == LOADING)
            {
                canvas->clear        ();
                CreateLoadingSprite();
                RenderLoading(*canvas);
            }
            else if (!suspended && state == RUNNING)
            {
                if (gamePlayState == PLAYING || gamePlayState == PAUSED || gamePlayState == WAITINGSTART || gamePlayState == WAITINGRESTART)
                {
                    canvas->clear        ();

                    RenderPlay(*canvas);

                }
                else if (gamePlayState == PAUSEDMENU)
                {
                    canvas->clear();

                    RenderPausedMenu(*canvas);
                }
                else if (gamePlayState == HELPMENU)
                {
                    canvas->clear();

                    RenderHelpMenu(*canvas);
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
        //Ordenamos los playingSprites en la lista para que a la hora de rendearse controlemos que playingSprites se rendean por encima de otros

        //PRIMERO CREAMOS LOS SPRITES DEL FONDO
        shared_ptr<Sprite> topBar(new Sprite(textures[ID(horizontalBar)].get()));
        shared_ptr<Sprite> leftBar(new Sprite(textures[ID(verticalBar)].get()));
        shared_ptr<Sprite> rightBar(new Sprite(textures[ID(verticalBar)].get()));

        topBar->set_anchor(TOP | LEFT);
        topBar->set_position({canvas_width/4, canvas_height});

        leftBar->set_anchor(TOP | LEFT);
        leftBar->set_position({canvas_width/4 - rightBar->get_width(), canvas_height});

        rightBar->set_anchor(TOP | LEFT);
        rightBar->set_position({canvas_width/4 * 3 + 6, canvas_height});

        playingSprites.push_back(leftBar);
        playingSprites.push_back(rightBar);
        playingSprites.push_back(topBar);

        //DESPUES CREAMOS LOS RELACIONEADOS CON EL JUEGO
        shared_ptr<Sprite> ballSprite(new Sprite(textures[ID(ball)].get()));
        shared_ptr<Sprite> player(new Sprite(textures[ID(player)].get()));

        player->set_anchor(BOTTOM | LEFT);
        player->set_position({ canvas_width/2 - player->get_width()/2, 0});

        ballSprite->set_anchor(BOTTOM | LEFT);
        ballSprite->set_position({ canvas_width/2 - ballSprite->get_width()/2, player->get_height()});

        playingSprites.push_back(ballSprite);
        playingSprites.push_back(player);

        //CREAMOS LOS SPRITES DE INTERFAZ GRAFICA

        shared_ptr<Sprite> leftButtonSrpite(new Sprite(textures[ID(leftButton)].get()));
        shared_ptr<Sprite> rightButtonSrpite(new Sprite(textures[ID(rightButton)].get()));
        shared_ptr<Sprite> youLostSprite(new Sprite(textures[ID(youLost)].get()));
        shared_ptr<Sprite> youWinSprite(new Sprite(textures[ID(youWin)].get()));
        shared_ptr<Sprite> optionsSprite(new Sprite(textures[ID(options)].get()));

        leftButtonSrpite->set_anchor(BOTTOM | LEFT);
        leftButtonSrpite->set_position({ canvas_width/4 - leftBar->get_width() - leftButtonSrpite->get_width() - 20, 2 * player->get_height()});

        rightButtonSrpite->set_anchor(BOTTOM | LEFT);
        rightButtonSrpite->set_position({ canvas_width/4 * 3 + rightBar->get_width() + 20, 2 * player->get_height()});

        playingSprites.push_back(leftButtonSrpite);
        playingSprites.push_back(rightButtonSrpite);

        youLostSprite->set_anchor(BOTTOM | LEFT);
        youWinSprite->set_anchor(BOTTOM | LEFT);

        youLostSprite->set_position({ canvas_width / 2 - youLostSprite->get_width() / 2, canvas_height / 2});
        youWinSprite->set_position({ canvas_width / 2 - youWinSprite->get_width() / 2, canvas_height / 2});

        youLostSprite->hide();
        youWinSprite->hide();

        optionsSprite->set_anchor(BOTTOM | LEFT);
        optionsSprite->set_position({canvas_width/4 - leftBar->get_width() - optionsSprite->get_width() * 2, canvas_height - optionsSprite->get_height() * 2});

        //CREAMOS TODOS LOS BLOQUES
        for (int f = 0; f < 8; ++f)
        {
            for (int i = 0; i < 14; ++i)
            {
                shared_ptr<Sprite> blockSprite(new Sprite(textures[ID(block)].get()));
                blockSprite->set_anchor(BOTTOM | LEFT);
                blockSprite->set_position({canvas_width/4 + i*blockSprite->get_width(), canvas_height/5 * 3 + f * blockSprite->get_height()});
                playingSprites.push_back(blockSprite);
                remainingBlockSprites.push_back(blockSprite);
            }
        }

        //CREAMOS LOS SPRITES DEL MENU DE PAUSA

        shared_ptr<Sprite> backSprite(new Sprite(textures[ID(back)].get()));
        shared_ptr<Sprite> menuBackgroundSprite(new Sprite(textures[ID(menuBackground)].get()));
        shared_ptr<Sprite> menuButtonSrpite(new Sprite(textures[ID(menuButton)].get()));
        shared_ptr<Sprite> restartButtonSrpite(new Sprite(textures[ID(restartButton)].get()));
        shared_ptr<Sprite> helpButtonSrpite(new Sprite(textures[ID(helpButton)].get()));

        backSprite->set_anchor(BOTTOM | LEFT);
        backSprite->set_position({canvas_width/4 - leftBar->get_width() - optionsSprite->get_width() * 2, canvas_height - optionsSprite->get_height() * 2});

        menuBackgroundSprite->set_anchor(BOTTOM | LEFT);
        menuButtonSrpite->set_anchor(BOTTOM | LEFT);
        helpButtonSrpite->set_anchor(BOTTOM | LEFT);
        restartButtonSrpite->set_anchor(BOTTOM | LEFT);

        menuBackgroundSprite->set_position({canvas_width / 2 - menuBackgroundSprite->get_width() / 2, canvas_height / 2 - menuBackgroundSprite->get_height() / 2 + menuButtonSrpite->get_height() / 2});
        menuButtonSrpite->set_position({canvas_width / 2 - menuButtonSrpite->get_width() / 2, canvas_height / 2 + 1.5 * menuButtonSrpite->get_height()});
        helpButtonSrpite->set_position({canvas_width / 2 - menuButtonSrpite->get_width() / 2, canvas_height / 2});
        restartButtonSrpite->set_position({canvas_width / 2 - menuButtonSrpite->get_width() / 2, canvas_height / 2 - 1.5 * menuButtonSrpite->get_height()});

        pausedSprites.push_back(backSprite);
        pausedSprites.push_back(menuBackgroundSprite);
        pausedSprites.push_back(menuButtonSrpite);
        pausedSprites.push_back(helpButtonSrpite);
        pausedSprites.push_back(restartButtonSrpite);

        //CREAMOS SPRITES DEL MENU DE AYUDA

        shared_ptr<Sprite> helpTextSprite(new Sprite(textures[ID(helpText)].get()));

        helpTextSprite->set_anchor(BOTTOM | LEFT);
        helpTextSprite->set_position({canvas_width / 2 - helpTextSprite->get_width() / 2, canvas_height / 2 - helpTextSprite->get_height() / 2});

        helpSprites.push_back(backSprite);
        helpSprites.push_back(helpTextSprite);

        //CREAMOS REFERENCIAS A LOS SPRITES MAS USADOS
        playerReference = player.get();
        ballReference = ballSprite.get();
        topWallReference = topBar.get();
        leftWallReference = leftBar.get();
        rightWallReference = rightBar.get();
        leftButtonReference = leftButtonSrpite.get();
        rightButtonReference = rightButtonSrpite.get();
        youLostReference = youLostSprite.get();
        youWinReference = youWinSprite.get();
        optionsReference = optionsSprite.get();
        backReference = backSprite.get();
        helpButtonReference = helpButtonSrpite.get();
        menuButtonReference = menuButtonSrpite.get();
        restartButtonReference = restartButtonSrpite.get();

        //ORDENAMOS ALGUNOS SPRITES PARA QUE SE SUPERPONGAN COMO NECESITAMOS
        playingSprites.push_back(youLostSprite);
        playingSprites.push_back(youWinSprite);
        playingSprites.push_back(optionsSprite);

    }

    void  Sample_Scene::CreateLoadingSprite()
    {
        shared_ptr<Sprite> loading(new Sprite(textures[ID(loading)].get()));

        loading->set_anchor(BOTTOM | LEFT);
        loading->set_position({canvas_width/ 2 - loading->get_width() / 2, canvas_height / 2 - loading->get_height() / 2});

        loadingReference = loading.get();
    }

    void Sample_Scene::MovePlayer(float time)
    {
        playerReference->set_speed(playerDirection * playerSpeed);
    }

    void Sample_Scene::run (float time)
    {
        if (gamePlayState == PLAYING)
        {
            MovePlayer(time);

            playerReference->update(time);
            ballReference->update(time);

            LimitPlayerPosition();
            CheckBallLimits();
            CheckBallCollisions();
        }
        else if (gamePlayState == WAITINGRESTART)
        {
            restartTimer -= time;

            if (restartTimer <= 0)
            {
                director.run_scene (shared_ptr< Scene >(new Sample_Scene));
            }
        }
    }

    void Sample_Scene::CheckBallCollisions()
    {
        if (ballReference->intersects(*playerReference))
        {
            ballReference->set_position_y(playerReference->get_position_y() + playerReference->get_height());

            float vectorMagnitude = ballReference->get_speed().length();
            Point2f playerCenter = { playerReference->get_position_x() + playerReference->get_width()/2, playerReference->get_position_y()};
            Point2f ballCenter = { ballReference->get_position_x() + ballReference->get_width() / 2, ballReference->get_position_y() + ballReference->get_height() / 2};
            Vector2f newDirection = {ballCenter[0] - playerCenter[0], ballCenter[1] - playerCenter[1]};

            ballReference->set_speed(newDirection.normalize() * vectorMagnitude);
        }
        if (ballReference->intersects(*rightWallReference))
        {
            ballReference->set_speed_x(-ballReference->get_speed_x());
            ballReference->set_position_x(rightWallReference->get_position_x() - rightWallReference->get_width());
        }
        if (ballReference->intersects(*leftWallReference))
        {
            ballReference->set_speed_x(-ballReference->get_speed_x());
            ballReference->set_position_x(leftWallReference->get_position_x() + leftWallReference->get_width());
        }
        if (ballReference->intersects(*topWallReference))
        {
            ballReference->set_speed_y(-ballReference->get_speed_y());
            ballReference->set_position_y(topWallReference->get_position_y() - topWallReference->get_height() - ballReference->get_height());
        }
        for (auto i = remainingBlockSprites.begin(); i != remainingBlockSprites.end();)
        {
            if (i->get()->intersects(*ballReference))
            {
                i->get()->hide();
                i = remainingBlockSprites.erase(i);
                ballReference->set_speed_y(-ballReference->get_speed_y());
                brokenBlockCounter++;
                if (brokenBlockCounter == 112)
                {
                    youWinReference->show();
                    gamePlayState = WAITINGRESTART;
                }
            }
            else
            {
                ++i;
            }
        }
    }

    void Sample_Scene::CheckBallLimits()
    {
        if (ballReference->get_position_y() < 0 - ballReference->get_height())
        {
            youLostReference->show();
            gamePlayState = WAITINGRESTART;
        }
    }

    void  Sample_Scene::LimitPlayerPosition()
    {
        if (playerReference->get_position_x() > rightWallReference->get_position_x() + rightWallReference->get_width() - playerReference->get_width()/2)
        {
            playerReference->set_position_x(rightWallReference->get_position_x() + rightWallReference->get_width() - playerReference->get_width()/2);
        }
        else if (playerReference->get_position_x() < leftWallReference->get_position_x() - playerReference->get_width() / 2)
        {
            playerReference->set_position_x(leftWallReference->get_position_x() - playerReference->get_width() / 2);
        }
    }

    void Sample_Scene::StartBall()
    {
        srand((unsigned ) time(0));
        Vector2f randomDirection = { rand() % canvas_width - playerReference->get_position_x(), canvas_height - playerReference->get_position_y() };
        ballReference->set_speed(randomDirection.normalize() * initialBallSpeed);
    }

    void Sample_Scene::CheckPlayerMoveInput(Event & event)
    {
        if (rightButtonReference->contains({*event[ID(x)].as<var::Float>(), *event[ID(y)].as<var::Float>()}))
        {
            playerDirection = {1,0};
        }
        else if (leftButtonReference->contains({*event[ID(x)].as<var::Float>(), *event[ID(y)].as<var::Float>()}))
        {
            playerDirection = {-1,0};
        }
    }

    //Funcion general utilizada para comprobar si se está pulsando un sprite
    bool Sample_Scene::CheckButtonClick(Event &event, Sprite *sprite)
    {
        if (sprite->contains({*event[ID(x)].as<var::Float>(), *event[ID(y)].as<var::Float>()}))
        {
            return true;
        } else
        {
            return false;
        }
    }

    //Rendea el sprite de carga
    void Sample_Scene::RenderLoading(Canvas & canvas)
    {
        loadingReference->render(canvas);
    }

    //Rendea todos los objetos relacionados al gameplay principal
    void Sample_Scene::RenderPlay(Canvas & canvas)
    {
        for (auto & sprite : playingSprites)
        {
            sprite->render(canvas);
        }
    }

    //Rendea todos los objetos del menu de juego
    void Sample_Scene::RenderPausedMenu(Canvas &canvas)
    {
        for (auto & sprite : pausedSprites)
        {
            sprite->render(canvas);
        }
    }

    //Rendea todos los objetos del menu de ayuda
    void Sample_Scene::RenderHelpMenu(Canvas &canvas)
    {
        for (auto & sprite : helpSprites)
        {
            sprite->render(canvas);
        }
    }

}
