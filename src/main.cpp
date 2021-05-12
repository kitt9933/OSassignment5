#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

using std::filesystem::directory_iterator;

#define WIDTH 800
#define HEIGHT 600

typedef struct AppData {
    TTF_Font *font;
    //std::string currDir;
    std::vector <std::string> *filesInDir;
    //SDL_Texture *penguin;
    SDL_Texture *phrase;
    SDL_Texture * code;
    SDL_Texture * directory;
    SDL_Texture * executableIcon;
    SDL_Texture * image;
    SDL_Texture * other;
    SDL_Texture * video;

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
    std::vector<std::string> currDir = listDirectory(homeDirStr);
    sort(currDir.begin(),currDir.end());

    for(int i = 0; i < currDir.size(); i++){

        std::cout<< currDir.at(i)<< std::endl;

    }
    

    // initialize and perform rendering loop
    AppData data;
    //AppData.currDir = homeDirStr;
    data.filesInDir = &currDir;
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
        //render(renderer, &data);
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

    //SDL_Surface *img_surf = IMG_Load("resrc/linux-penguin.png");
    //data_ptr->penguin = SDL_CreateTextureFromSurface(renderer, img_surf);
    //SDL_FreeSurface(img_surf);

    SDL_Surface *code_surf = IMG_Load("resrc/images/code.png");
    data_ptr->code = SDL_CreateTextureFromSurface(renderer, code_surf);
    SDL_FreeSurface(code_surf);

    SDL_Surface *dir_surf = IMG_Load("resrc/images/directory.png");
    data_ptr->directory = SDL_CreateTextureFromSurface(renderer, dir_surf);
    SDL_FreeSurface(dir_surf);

    SDL_Surface *exec_surf = IMG_Load("resrc/images/executable.png");
    data_ptr->executableIcon = SDL_CreateTextureFromSurface(renderer, exec_surf);
    SDL_FreeSurface(exec_surf);

    SDL_Surface *img_surf = IMG_Load("resrc/images/image.png");
    data_ptr->image = SDL_CreateTextureFromSurface(renderer, img_surf);
    SDL_FreeSurface(img_surf);

    SDL_Surface *other_surf = IMG_Load("resrc/images/other.png");
    data_ptr->other = SDL_CreateTextureFromSurface(renderer, other_surf);
    SDL_FreeSurface(other_surf);

    SDL_Surface *video_surf = IMG_Load("resrc/images/video.png");
    data_ptr->video = SDL_CreateTextureFromSurface(renderer,video_surf);
    SDL_FreeSurface(video_surf);


    SDL_Color color = { 0, 120, 120 };
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

    //render background
    SDL_Rect backRect;
    backRect.x = 0;
    backRect.y = 0;
    backRect.w = 800;
    backRect.h = 50;

    for (int i = 0; i < 12; i++){

        SDL_RenderFillRect(renderer,&backRect);
        
        if(i%2){
            SDL_SetRenderDrawColor(renderer,255,255,255,255);
        }
        else{

            SDL_SetRenderDrawColor(renderer,150,150,150,255);
        }
        backRect.y +=50;
    }

    SDL_Rect rect;
    rect.x = 10;
    rect.y = 10;
    rect.w = 40;
    rect.h = 40;
    SDL_RenderCopy(renderer, data_ptr->code, NULL, &rect);
    //SDL_RenderCopy()
    
    SDL_Rect textRect;
    textRect.x = 60;
    textRect.y = 20;
    textRect.w = 200;
    textRect.h = 25;

    int yVal = 60;
    const char* charName;
    for (int i = 0; i < 12; i++){
        rect.y = yVal;
        SDL_RenderCopy(renderer, data_ptr->directory, NULL, &rect);
        yVal = yVal + 50;


        //std::cout << "________________\n";
        std::string currFileName = data_ptr->filesInDir->at(i);
        charName = currFileName.c_str();
        //std::cout << currFileName << std::endl;
        
        SDL_Color color = { 0, 120, 120 };
        SDL_Surface *phrase_surf = TTF_RenderText_Solid(data_ptr->font, charName, color);
        data_ptr->phrase = SDL_CreateTextureFromSurface(renderer, phrase_surf);
        SDL_FreeSurface(phrase_surf);

        SDL_RenderCopy(renderer, data_ptr->phrase, NULL, &textRect);
        
        textRect.y +=50;
    }

    //rect.x = 400;
    //rect.y = 300;
    //SDL_RenderCopy(renderer, data_ptr->code, NULL, &rect);

    SDL_QueryTexture(data_ptr->phrase, NULL, NULL, &(rect.w), &(rect.h));
    //rect.x = 10;
    //rect.y = 500;
    //SDL_RenderCopy(renderer, data_ptr->phrase, NULL, &rect);

    // show rendered frame
    SDL_RenderPresent(renderer);
}

void quit(AppData *data_ptr)
{
    //SDL_DestroyTexture(data_ptr->penguin);
    SDL_DestroyTexture(data_ptr->phrase);
    TTF_CloseFont(data_ptr->font);
}

std::vector<std::string> listDirectory(std::string path){


    std::vector<std::string> dirContents;

    

    for (const auto & file : directory_iterator(path)){
        //std::cout << file.path() << std::endl;
        dirContents.push_back(file.path());
        
    }
    return dirContents;
}
    


