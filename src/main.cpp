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

typedef struct fileInfo {

    std::string name;
    int type;
    std::string perms;

    //fileInfo(std::string name, int type, std::string perms) : name(name), type(type), perms(perms) {}

}fileInfo;

typedef struct AppData {
    TTF_Font *font;
    //std::string currDir;
    
    std::vector <fileInfo> files;
    //SDL_Texture *penguin;
    SDL_Texture *phrase;
    SDL_Texture * code;
    SDL_Texture * directory;
    SDL_Texture * executableIcon;
    SDL_Texture * image;
    SDL_Texture * other;
    SDL_Texture * video;
    SDL_Texture * leftArrow;
    SDL_Texture * rightArrow;

    
    SDL_Rect backRect [11];
    int shownFileTypeVals [11];
    bool phrase_selected;
    bool icon_selected;
    SDL_Point offset;
    //bool pengSelected;
} AppData;





void initialize(SDL_Renderer *renderer, AppData *data_ptr);
void render(SDL_Renderer *renderer, AppData *data_ptr);
void quit(AppData *data_ptr);
std::vector<fileInfo> listDirectory(std::string path, AppData data);
//std::vector <int> getFileTypes(std::string filepath, AppData data);

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

    fileInfo parentDir;

    AppData data;

    //need to add .. directory
    parentDir.name = homeDirStr + "/..";
    parentDir.type = 2;
    parentDir.perms = "rwxrwxrwx";

    std::vector<fileInfo> infos = listDirectory(homeDirStr, data);
    //std::vector<std::string> currDir;
    data.files = infos;

    infos.push_back(parentDir);
    
    //sort(currDir.begin(),currDir.end());
    /*
    for(int i = 0; i < currDir.size(); i++){

        std::cout<< currDir.at(i)<< std::endl;

    }
    */

    // initialize and perform rendering loop
    
    //AppData.currDir = homeDirStr;
    //data.filesInDir = &currDir;
    //data.fileTypeVals = getFileTypes(homeDirStr);
    puts("b i\n");
    initialize(renderer, &data);
    puts("a i \n");
    render(renderer, &data);
    puts("rr\n");
    SDL_Event event;
    SDL_WaitEvent(&event);
    while (event.type != SDL_QUIT)
    {
        SDL_WaitEvent(&event);
        
        switch(event.type)
        {
            /*
            case SDL_MOUSEMOTION:
                if (data.penguin_selected)
                {
                    data.penguin_rect.x = event.motion.x - data.offset.x;
                    data.penguin_rect.y = event.motion.y - data.offset.y;
                }
                else if (data.phrase_selected)
                {
                    data.phrase_rect.x = event.motion.x - data.offset.x;
                    data.phrase_rect.y = event.motion.y - data.offset.y;
                }
                break;
            */

            case SDL_MOUSEBUTTONDOWN:
                for(int i = 0; i < 11; i++){
                    if (event.button.button == SDL_BUTTON_LEFT &&
                        event.button.x >= data.backRect[i].x &&
                        event.button.x <= data.backRect[i].x + data.backRect[i].w &&
                        event.button.y >= data.backRect[i].y &&
                        event.button.y <= data.backRect[i].y + data.backRect[i].h)
                    {
                        puts("i CLICKED");
                        data.phrase_selected = true;
                        data.offset.x = event.button.x - data.backRect[i].x;
                        data.offset.y = event.button.y - data.backRect[i].y;

                        //determine what file type it is
                        //int fileType = getFileType(currDir[i]);
                        //if it is a directory go into that dir
                        
                        //if it is an executable run it

                        break;
                    } 
                    
                }
                
                
                
            case SDL_MOUSEBUTTONUP:
                data.phrase_selected = false;
                
                break;
        }
        
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

    //draw background we will also be clicking that instead of text
    SDL_Rect bg;
    for (int i = 0; i < 11; i++){
        data_ptr->backRect[i].x = 0;
        data_ptr->backRect[i].y = i * 50;
        data_ptr->backRect[i].w = 800;
        data_ptr->backRect[i].h = 50;
        bg = data_ptr->backRect[i];
        SDL_RenderFillRect(renderer,&bg);
        
        if(i%2){
            SDL_SetRenderDrawColor(renderer,255,255,255,255);
        }
        else{

            SDL_SetRenderDrawColor(renderer,150,150,150,255);
        }
        //bg.y +=50;
    }

    SDL_Rect rect;
    rect.x = 10;
    rect.y = 10;
    rect.w = 40;
    rect.h = 40;
    //SDL_RenderCopy(renderer, data_ptr->code, NULL, &rect);
    //SDL_RenderCopy()
    
    SDL_Rect textRect;
    textRect.x = 60;
    textRect.y = 20;
    textRect.w = 200;
    textRect.h = 25;

    int yVal = 10;
    const char* charName;
    
    for (int i = 0; i < 11; i++){
        rect.y = yVal;
        
        int fileType  = data_ptr->files.at(i).type;
        if(fileType == 2){
            SDL_RenderCopy(renderer, data_ptr->directory, NULL, &rect);
        }
        else{
            SDL_RenderCopy(renderer, data_ptr->code, NULL, &rect);
        }
        yVal = yVal + 50;

        
        //std::cout << "________________\n";
        std::string currFileName = data_ptr->files.at(i).name;
        
        int nameLen = currFileName.length();

        //this is to make the names look nicer
        textRect.w = 9 * nameLen;
        charName = currFileName.c_str();
        //std::cout << currFileName << std::endl;
        
        SDL_Color color = { 0, 120, 120 };
        SDL_Surface *phrase_surf = TTF_RenderText_Solid(data_ptr->font, charName, color);
        data_ptr->phrase = SDL_CreateTextureFromSurface(renderer, phrase_surf);
        SDL_FreeSurface(phrase_surf);

        SDL_RenderCopy(renderer, data_ptr->phrase, NULL, &textRect);
        
        textRect.y +=50;
    }

    textRect.x = 300;
    textRect.y = 550;
    textRect.w = 35;
    textRect.h = 50;

    //data_ptr->leftArrow.x = 350;
    //data_ptr->leftArrow.y = 550;
    //data_ptr->leftArrow.w = 50;
    //data_ptr->leftArrow.h = 50;

    SDL_Color color = { 0, 0, 0 };
    SDL_Surface *lArrow = TTF_RenderText_Solid(data_ptr->font, "<<", color);
    data_ptr->leftArrow = SDL_CreateTextureFromSurface(renderer, lArrow);
    SDL_FreeSurface(lArrow);
    SDL_RenderCopy(renderer, data_ptr->leftArrow, NULL, &textRect);

    textRect.x = 400;
    SDL_Surface *rArrow = TTF_RenderText_Solid(data_ptr->font, ">>", color);
    data_ptr->rightArrow = SDL_CreateTextureFromSurface(renderer, rArrow);
    SDL_FreeSurface(rArrow);
    SDL_RenderCopy(renderer, data_ptr->rightArrow, NULL, &textRect);

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
    //TODO: NEED TO DESTROY TEXTURES
    SDL_DestroyTexture(data_ptr->phrase);
    TTF_CloseFont(data_ptr->font);
}

std::vector<fileInfo> listDirectory(std::string path, AppData data){


    std::vector<fileInfo> dirContents;

     
    int dirCount = 0;
    int count = 0;
    int i = 0;
    for (const auto & file : directory_iterator(path)){
        //std::cout << file.path() << std::endl;


        fileInfo currFile;
        currFile.name = file.path();
        currFile.perms = "rwxrwxrwx";
        currFile.type = 2;
        dirContents.push_back(currFile);
        std::cout << file.path() << std::endl;
        /*
        if(std::filesystem::is_directory(file.path())){

            std::cout << "ITS A DIR\n";
            std::cout << file.path() << std::endl;
            //dirCount++;
        }
        else {
            std::cout << "ITS NOT DIR\n";
            std::cout << file.path() << std::endl;
        }
        */
       
        //count++;
        i++;
    }
    std::filesystem::perms p = std::filesystem::status(path).permissions();
        std::cout << ((p & std::filesystem::perms::owner_read) != std::filesystem::perms::none ? "r" : "-")
              << ((p & std::filesystem::perms::owner_write) != std::filesystem::perms::none ? "w" : "-")
              << ((p & std::filesystem::perms::owner_exec) != std::filesystem::perms::none ? "x" : "-")
              << ((p & std::filesystem::perms::group_read) != std::filesystem::perms::none ? "r" : "-")
              << ((p & std::filesystem::perms::group_write) != std::filesystem::perms::none ? "w" : "-")
              << ((p & std::filesystem::perms::group_write) != std::filesystem::perms::none ? "w" : "-")
              << ((p & std::filesystem::perms::group_exec) != std::filesystem::perms::none ? "x" : "-")
              << ((p & std::filesystem::perms::others_read) != std::filesystem::perms::none ? "r" : "-")
              << ((p & std::filesystem::perms::others_write) != std::filesystem::perms::none ? "w" : "-")
              << ((p & std::filesystem::perms::others_exec) != std::filesystem::perms::none ? "x" : "-")
              << '\n';
    //std::cout << count << std::endl;
    //std::cout << dirCount << std::endl;
    return dirContents;
}
/*
std::vector <int> getFileTypes(std::string path){

    int i = 0;
    std::vector <int> fileVals;
    for (const auto & file : directory_iterator(path)){
        if(std::filesystem::is_directory(file.path())){ //it is a dir
            fileVals.push_back(2);
        }else{
            fileVals.push_back(1);
        }
        i++;
     }
     return fileVals;
    
}
*/
