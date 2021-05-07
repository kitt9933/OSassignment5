#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

using std::filesystem::directory_iterator;

#define WIDTH 800
#define HEIGHT 600

typedef struct AppData {
    TTF_Font *font;
    SDL_Texture *penguin;
    SDL_Texture *phrase;
    bool phrase_selected;
    bool pengSelected;
} AppData;

void initialize(SDL_Renderer *renderer, AppData *data_ptr);
void render(SDL_Renderer *renderer, AppData *data_ptr);
void quit(AppData *data_ptr);
std::vector<std::string> listDirectory(std::string path);

int main(int argc, char **argv)
{
    char *home = getenv("HOME");
    
    printf("HOME: %s\n", home);

    // initializing SDL as Video
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    // create window and renderer
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);

    std::string homeDirStr(home);
    listDirectory(homeDirStr);

    // initialize and perform rendering loop
    AppData data;
    initialize(renderer, &data);
    render(renderer, &data);
    SDL_Event event;
    SDL_WaitEvent(&event);
    while (event.type != SDL_QUIT)
    {
        SDL_WaitEvent(&event);
        /*
        switch(event.type)
        {

            case SDL_MOUSEMOTION:
                printf("mouse: %d, %d\n",event.motion.x, event.motion.y);
                break;
                if(data.phrase_selected){

                    data.phrase_location.x = event.motion.x;
                    data.phrase_location.y = event.motion.y;
                }
            case SDL_MOUSEBUTTONDOWN:
                puts("Click\n");
                if (event.button.x >= data.phrase_location.x && 
                    event.button.x <= data.phrase_location.x + data.phrase_location.w &&
                    event.button.y >= data.phrase_location.y &&
                    event.button.y <= data.phrase_location.y + data.phrase_location.h;
                )
                {
                    data.phrase_selected = true;
                }
                else if (event.button.x >= data.penguin_location.x && 
                    event.button.x <= data.phrase_location.x + data.phrase_location.w &&
                    event.button.y >= data.phrase_location.y &&
                    event.button.y <= data.phrase_location.y + data.phrase_location.h;
                )
                {
                    data.phrase_selected = true;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                puts("unclick\n");
                break;
        }
        */
        render(renderer, &data);
    }

    // clean up
    quit(&data);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();


    return 0;
}

void initialize(SDL_Renderer *renderer, AppData *data_ptr)
{
    data_ptr->font = TTF_OpenFont("resrc/OpenSans-Regular.ttf", 24);

    SDL_Surface *img_surf = IMG_Load("resrc/linux-penguin.png");
    data_ptr->penguin = SDL_CreateTextureFromSurface(renderer, img_surf);
    SDL_FreeSurface(img_surf);

    SDL_Color color = { 0, 0, 0 };
    SDL_Surface *phrase_surf = TTF_RenderText_Solid(data_ptr->font, "Hello World!", color);
    data_ptr->phrase = SDL_CreateTextureFromSurface(renderer, phrase_surf);
    SDL_FreeSurface(phrase_surf);
}

void render(SDL_Renderer *renderer, AppData *data_ptr)
{
    // erase renderer content
    SDL_SetRenderDrawColor(renderer, 235, 235, 235, 255);
    SDL_RenderClear(renderer);
    
    // TODO: draw!
    SDL_Rect rect;
    rect.x = 200;
    rect.y = 100;
    rect.w = 165;
    rect.h = 200;
    SDL_RenderCopy(renderer, data_ptr->penguin, NULL, &rect);

    rect.x = 400;
    rect.y = 300;
    SDL_RenderCopy(renderer, data_ptr->penguin, NULL, &rect);

    SDL_QueryTexture(data_ptr->phrase, NULL, NULL, &(rect.w), &(rect.h));
    rect.x = 10;
    rect.y = 500;
    SDL_RenderCopy(renderer, data_ptr->phrase, NULL, &rect);

    // show rendered frame
    SDL_RenderPresent(renderer);
}

void quit(AppData *data_ptr)
{
    SDL_DestroyTexture(data_ptr->penguin);
    SDL_DestroyTexture(data_ptr->phrase);
    TTF_CloseFont(data_ptr->font);
}

std::vector<std::string> listDirectory(std::string path){


    std::vector<std::string> dirContents;

    

    for (const auto & file : directory_iterator(path)){
        std::cout << file.path() << std::endl;
        dirContents.push_back(file.path());
        
    }
    return dirContents;
}
    


