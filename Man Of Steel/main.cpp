#include "CommonFunc.h"
#include "PlayerObject.h"
#include "ImpTimer.h"
#include "BlockObject.h"
#include "Geometric.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 640


BaseObject g_background;
BaseObject g_ground;

Mix_Chunk* g_bgm = NULL;
TTF_Font* g_font_text = NULL;
TTF_Font* g_font_MENU = NULL;

bool InitData()
{
    BOOL Sucess = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if (ret < 0)
        return false;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    g_window = SDL_CreateWindow("Man Of Steel", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,SDL_WINDOW_OPENGL);

    if (g_window != NULL)
    {
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
        if (g_screen != NULL)
        {
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) && imgFlags))
                Sucess = false;
        }

        //init text
        if (TTF_Init() == -1)
        {
            Sucess = false;
        }

        //init audio
        if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
        {
            Sucess = false;
        }

        // load background music
        g_bgm = Mix_LoadWAV("sound//backsound.wav");
        if (g_bgm == NULL)
        {
            return false;
        }

        //load font
        g_font_text = TTF_OpenFont("font//The Impostor.ttf", 40);  //score
        if (g_font_text == NULL)
        {
            return false;
        }

        g_font_MENU = TTF_OpenFont("font//FlappyBirdy.ttf", 100);  //menu
        if (g_font_MENU == NULL)
        {
            return false;
        }
    }

    return Sucess;
}


bool LoadBackground()
{
    bool ret = g_background.LoadImageFile("img//bg.png", g_screen);
    return ret;
}


void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true) {
    if ( SDL_WaitEvent(&e) != 0 && e.type == SDL_QUIT )
    return;
    SDL_Delay(100);
 }
}


void close()
{
    g_background.Free();
    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;

    SDL_DestroyWindow(g_window);
    g_window = NULL;

    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[])
{

    if (InitData() == false)
    {
        return -1;
    }

    Mix_PlayChannel(-1, g_bgm, -1);

    ImpTimer fps;
    bool quit = false;


// tạo menu game -----------------------------------------------------------------------------------------------------
    int ret_menu = SDLCommonFunc::ShowMenu(g_screen, g_font_MENU, "Play Game", "Exit", "img//MENU.png");
    if (ret_menu == 1)    quit = true;
    if (!LoadBackground())
    {
        return -1;
    }


    TextObject text_count_;  // khởi tạo điểm
    text_count_.setColor(TextObject::WHITE_TEXT);

    PlayerObject player;
    bool ret = player.LoadImg("img//superman.png", g_screen);
    player.SetRect(400, 100);
    if (ret == false)
    {
        return -1;
    }

    BlockManager manage_block;
    ret = manage_block.InitBlockList(g_screen);
    if (ret == false)
        return -1;

    while (!quit)
    {
        fps.start();
        while (SDL_PollEvent(&g_event) != 0)
        {
            if (g_event.type == SDL_QUIT)
            {
                quit = true;
            }

            player.HandleInputAction(g_event, g_screen);
        }


        SDL_RenderClear(g_screen);

        g_background.Render(g_screen, NULL);

        manage_block.SetPlayerRect(player.GetRect());

        bool is_falling = player.GetFalling();
        if (is_falling == true)
        {
            manage_block.SetStopMoving(true);
        }

        manage_block.Render(g_screen);

        bool end_game = manage_block.GetColState();
        if (end_game == true)
        {
            player.SetFalling(true);
        }
        player.DoFalling(g_screen);
        player.Show(g_screen);

        //Draw Geometric


        int count = manage_block.GetCount();
        std::string count_str = std::to_string(count);
        text_count_.SetText(count_str);
        text_count_.loadFromRenderedText(g_font_text, g_screen);
        text_count_.RenderText(g_screen, SCREEN_WIDTH*0.5, 2);

        g_ground.Render(g_screen);
        SDL_RenderPresent(g_screen);

        // Make menu game over
        bool game_over = player.GetIsDie();
        if (game_over == true)
        {
            Sleep(500);
            SDLCommonFunc::ShowMenu(g_screen, g_font_MENU, "", "", "img//gameover.png");
            quit = true;
            void waitUntilKeyPressed();
            continue;
        }

        //Cap the frame rate
//        int val1 = fps.get_ticks();
        if (fps.get_ticks() < 1000 / FRAMES_PER_SECOND)
        {
            SDL_Delay((1000 / FRAMES_PER_SECOND) - fps.get_ticks());
        }
    }

    close();
    return 0;
}


