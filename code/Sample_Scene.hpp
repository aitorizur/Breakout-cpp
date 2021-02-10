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

        enum GameplayState
        {
            WAITINGSTART,
            WAITINGRESTART,
            PLAYING,
            PAUSED,
            PAUSEDMENU,
            HELPMENU,
        };

        State          state;
        GameplayState  gamePlayState;

        bool           suspended;

        unsigned       canvas_width;
        unsigned       canvas_height;

        Timer timer;

        /////////////////////////////////////////////////////////

        ///////////////// Variables relacionadas con la carga de texturas y playingSprites ///////////////////

        //Creamos un struct que contendra la ruta de las texturas junto con un ID
        static struct   Texture_Data { Id id; const char * path; } textures_data[];

        //Numero de texturas que hay en el array la informacion de las texturas
        static unsigned textures_count;

        //Mapa en el que se guardan shared_ptr a las texturas cargadas
        map<Id, shared_ptr<Texture_2D>> textures;
        //Lista en la que se guardan shared_ptr a los playingSprites creados usados mientras se juega
        list<shared_ptr<Sprite>> playingSprites;
        //Lista en la que se guardan shared_ptr a los playingSprites creados usados en el menu de pausa
        list<shared_ptr<Sprite>> pausedSprites;
        //Lista en la que se guardan shared_ptr a los playingSprites creados usados en el menu de ayuda
        list<shared_ptr<Sprite>> helpSprites;

        list<shared_ptr<Sprite>> remainingBlockSprites;

        /////////////////////////////////////////////////////////

        ///////////////// Variables relacionadas con el gameplay ///////////////////

        float playerSpeed = 800;
        Vector2f playerDirection = {0, 0};

        float initialBallSpeed = 500;

        int brokenBlockCounter = 0;

        float  restartTimer = 1;

        Sprite *playerReference, *ballReference, *topWallReference, *rightWallReference, *leftWallReference
        , *leftButtonReference, *rightButtonReference, *youLostReference, *youWinReference, *optionsReference
        , *loadingReference, *backReference, *menuButtonReference, *restartButtonReference, *helpButtonReference;

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

        void LimitPlayerPosition();
        void StartBall();
        void CheckBallLimits();
        void CheckBallCollisions();
        void MovePlayer(float time);
        void CreateLoadingSprite();
        void CheckPlayerMoveInput(Event & event);
        bool CheckButtonClick(Event & event, Sprite * sprite);
        void RenderLoading(Canvas & canvas);
        void RenderPlay(Canvas & canvas);
        void RenderPausedMenu(Canvas & canvas);
        void RenderHelpMenu(Canvas & canvas);

    };

}
