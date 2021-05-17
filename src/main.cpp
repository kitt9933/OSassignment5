#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>
#include <SDL.h>
#include <cmath>
#include <cstdlib>
#include <unistd.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <fstream>
#include <cstdlib>

using std::filesystem::directory_iterator;

#define WIDTH 800
#define HEIGHT 600

typedef struct fileInfo {

    std::string name;
    int type;
    std::string perms;
    std::uintmax_t fileSize;

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
    SDL_Rect leftRect;
    SDL_Rect rightRect;

    int pageStart = 0;
    int pageEnd = 11;

    std::string dir;
    
    bool phrase_selected;
    bool icon_selected;
    bool left_selected;
    bool right_selected;
    SDL_Point offset;
    //bool pengSelected;
} AppData;





void initialize(SDL_Renderer *renderer, AppData *data_ptr);
void render(SDL_Renderer *renderer, AppData *data_ptr);
void quit(AppData *data_ptr);
std::vector<fileInfo> listDirectory(std::string path, AppData data);
std::vector <int> getFileTypes(std::string filepath, AppData data);
void splitString(std::string text, char d, std::vector<std::string>& result);

bool compareInfo(fileInfo a, fileInfo b){

    int result  = strcmp(a.name.c_str(),b.name.c_str());

    /*
    if(result >= 0){
        return true;
    }
    else{
        return false;
    }
    */
    return (a.name < b.name);
}

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
    parentDir.perms = "rwxr-xr-x";

    std::vector<fileInfo> infos = listDirectory(homeDirStr, data);
    //std::vector<std::string> currDir;
    infos.push_back(parentDir);
    data.dir = homeDirStr;
    

    


    
    std::sort(infos.begin(),infos.end(), compareInfo);
    /*
    for(int i = 0; i < infos.size(); i++){

        std::cout<< infos.at(i).name<< std::endl;

    }
    */
    data.files = infos;

    // initialize and perform rendering loop
    
    //data.currDir = homeDirStr;
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


                if (event.button.button == SDL_BUTTON_LEFT &&
                        event.button.x >= data.leftRect.x &&
                        event.button.x <= data.leftRect.x + data.leftRect.w &&
                        event.button.y >= data.leftRect.y &&
                        event.button.y <= data.leftRect.y + data.leftRect.h)
                    {
                        puts("left CLICKED");
                        data.left_selected = true;
                        data.offset.x = event.button.x - data.leftRect.x;
                        data.offset.y = event.button.y - data.leftRect.y;

                        data.pageStart = data.pageStart - 11;
                        data.pageEnd =data.pageEnd - 11;

                        render(renderer, &data);

                        break;
                    }
                else if (event.button.button == SDL_BUTTON_LEFT &&
                        event.button.x >= data.rightRect.x &&
                        event.button.x <= data.rightRect.x + data.rightRect.w &&
                        event.button.y >= data.rightRect.y &&
                        event.button.y <= data.rightRect.y + data.rightRect.h)
                    {
                        puts("right CLICKED");
                        data.right_selected = true;
                        data.offset.x = event.button.x - data.rightRect.x;
                        data.offset.y = event.button.y - data.rightRect.y;

                        data.pageStart = data.pageStart + 11;
                        data.pageEnd =data.pageEnd + 11;

                        render(renderer, &data);
                            

                        break;
                    }    
                else{
                    for(int i = 0; i < 11; i++){
                        if (event.button.button == SDL_BUTTON_LEFT &&
                            event.button.x >= data.backRect[i].x &&
                            event.button.x <= data.backRect[i].x + data.backRect[i].w &&
                            event.button.y >= data.backRect[i].y &&
                            event.button.y <= data.backRect[i].y + data.backRect[i].h)
                        {
                            if(i >= data.files.size()){
                                break;
                            }
                            std::string filePath = data.files.at(i + data.pageStart).name.c_str();
                            printf("%s", data.files.at(i + data.pageStart).name.c_str());
                            puts(" CLICKED");
                            data.phrase_selected = true;
                            data.offset.x = event.button.x - data.backRect[i].x;
                            data.offset.y = event.button.y - data.backRect[i].y;

                            //determine what file type it is
                            //int fileType = getFileType(currDir[i]);
                            
                            int fileType = data.files.at(i+data.pageStart).type;
                            std::cout << filePath << " is type: " <<  fileType << std::endl;
                            //if it is a directory go into that dir
                            std::vector<std::string> pathParts;
                            splitString(filePath,'/',pathParts);

                            if(strcmp(pathParts.at(pathParts.size()-1).c_str(),"..") == 0) { //if the end of the path is ".." then we cd upwards 
                                std::string newPath = "/";
                                for(int j = 0; j < pathParts.size()-2; j++){
                                    newPath +=pathParts.at(j);
                                    newPath +="/";

                                }
                                //std::cout << "new path is " << newPath << std::endl;

                                fileInfo parDir;
                                parDir.name = newPath + "/..";
                                parDir.type = 2;
                                parDir.perms = "rwxr-xr-x";

                                std::vector<fileInfo> newInfo = listDirectory(newPath, data);
                                
                                newInfo.push_back(parDir);
                                std::sort(newInfo.begin(),newInfo.end(), compareInfo);
                                //std::vector<std::string> currDir;
                                data.dir = newPath;
                                data.files = newInfo;
                                data.pageStart = 0;
                                data.pageEnd = 11;

                                render(renderer, &data);
                            }
                            else if(fileType == 2) {        //This means it's a directory so go into it.
                                fileInfo parDir;
                                parDir.name = filePath + "/..";
                                parDir.type = 2;
                                parDir.perms = "rwxr-xr-x";

                                std::vector<fileInfo> newInfo = listDirectory(filePath, data);
                                
                                newInfo.push_back(parDir);
                                std::sort(newInfo.begin(),newInfo.end(), compareInfo);
                                //std::vector<std::string> currDir;
                                data.dir = filePath;
                                data.files = newInfo;
                                data.pageStart = 0;
                                data.pageEnd = 11;

                                render(renderer, &data);

                            } else if(fileType == 3) {      //This means it's an executable so run it.
                                char* args[] = { (char *)pathParts[pathParts.size()-1].c_str(), NULL };
                                int pid = fork();
                                if(pid == 0) {
                                    execv(data.files.at(i + data.pageStart).name.c_str(), args);
                                }
                            } else {    //This means it's some other file so try to run it with xdg-open.
                                char* args[] = { (char *)"xdg-open", (char *)data.files.at(i + data.pageStart).name.c_str(), NULL };
                                int pid = fork();
                                if(pid == 0) {
                                    execv("/usr/bin/xdg-open", args);
                                }
                            }
                            break;
                        } 
                        
                    }
                } 
                
                
            case SDL_MOUSEBUTTONUP:
                data.phrase_selected = false;
                data.left_selected = false;
                
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
    const char* permName;
    const char* sizeName;
    for (int i = data_ptr->pageStart; i < data_ptr->pageEnd; i++){

        if(i >= data_ptr->files.size()){
            puts("i >= end");
            break;
        }
        rect.y = yVal;
        
        int fileType  = data_ptr->files.at(i).type;
        if(fileType == 1){
            SDL_RenderCopy(renderer, data_ptr->code, NULL, &rect);
        }
        else if(fileType == 2){
            SDL_RenderCopy(renderer, data_ptr->directory, NULL, &rect);
        }
        else if(fileType == 3){
            SDL_RenderCopy(renderer, data_ptr->executableIcon, NULL, &rect);
        }
        else if(fileType == 4){
            SDL_RenderCopy(renderer, data_ptr->image, NULL, &rect);
        }
        else if(fileType == 5){
            SDL_RenderCopy(renderer, data_ptr->other, NULL, &rect);
        }
        else if(fileType == 6){
            SDL_RenderCopy(renderer, data_ptr->video, NULL, &rect);
        }
        yVal = yVal + 50;

        
        //std::cout << "________________\n";
        std::string currFileName = data_ptr->files.at(i).name;
        std::vector<std::string> nameParts;
        splitString(currFileName,'/',nameParts);

        std::string shortName = nameParts.at(nameParts.size()-1);

        int nameLen = shortName.length();

        //this is to make the names look nicer
        textRect.w = 9 * nameLen;
        charName = shortName.c_str();
        //std::cout << currFileName << std::endl;
        
        SDL_Color color = { 0, 120, 120 };
        SDL_Surface *phrase_surf = TTF_RenderText_Solid(data_ptr->font, charName, color);
        data_ptr->phrase = SDL_CreateTextureFromSurface(renderer, phrase_surf);
        SDL_FreeSurface(phrase_surf);

        SDL_RenderCopy(renderer, data_ptr->phrase, NULL, &textRect);

        //render permission
        permName = data_ptr->files.at(i).perms.c_str(); // convert the string that represents perms to a const char
        SDL_Surface *perm_surf = TTF_RenderText_Solid(data_ptr->font, permName, color);
        data_ptr->phrase = SDL_CreateTextureFromSurface(renderer, perm_surf);
        SDL_FreeSurface(perm_surf);

        textRect.x +=450;
        textRect.w = 100;
        SDL_RenderCopy(renderer, data_ptr->phrase, NULL, &textRect);

        //render file size
        textRect.x +=175;
        std::uintmax_t sizeInBytes = data_ptr->files.at(i).fileSize;

        std::string sizeInfo;
        if(sizeInBytes == 0){//this is an error
            sizeInfo = "unknown";
            textRect.w = 100;
        }
        else if(sizeInBytes >= 1024){ //show in KiB
            int sizeInK = sizeInBytes/1024;
            sizeInfo = std::to_string(sizeInK);
            
            sizeInfo += " KiB";
            textRect.w = sizeInfo.length() * 7;
        }
        else if(sizeInBytes >= 10480576){ //show in MiB
            int sizeInM = sizeInBytes/1048576;
            sizeInfo = std::to_string(sizeInM);
            
            sizeInfo += " MiB";
            textRect.w = sizeInfo.length() * 7;
        }
        else if(sizeInBytes >= 1073741824){
            long sizeInG = sizeInBytes/1073741824;
            sizeInfo = std::to_string(sizeInG);
            
            sizeInfo += " GiB";
            textRect.w = sizeInfo.length() * 7;

        }
        else{ //it is shown in bytes
            sizeInfo = std::to_string(sizeInBytes);
            sizeInfo += " B";
            textRect.w = sizeInfo.length() * 7;
        }
        sizeName = sizeInfo.c_str();
        SDL_Surface *size_surf = TTF_RenderText_Solid(data_ptr->font, sizeName, color);
        data_ptr->phrase = SDL_CreateTextureFromSurface(renderer, size_surf);
        SDL_FreeSurface(size_surf);
        SDL_RenderCopy(renderer, data_ptr->phrase, NULL, &textRect);

        textRect.x -=625;


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

    data_ptr->leftRect.x = 300;
    data_ptr->leftRect.y = 550;
    data_ptr->leftRect.w = 35;
    data_ptr->leftRect.h = 50;

    textRect.x = 400;
    SDL_Surface *rArrow = TTF_RenderText_Solid(data_ptr->font, ">>", color);
    data_ptr->rightArrow = SDL_CreateTextureFromSurface(renderer, rArrow);
    SDL_FreeSurface(rArrow);
    SDL_RenderCopy(renderer, data_ptr->rightArrow, NULL, &textRect);

    data_ptr->rightRect.x = 400;
    data_ptr->rightRect.y = 550;
    data_ptr->rightRect.w = 35;
    data_ptr->rightRect.h = 50;

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
        std::filesystem::perms p = std::filesystem::status(file.path()).permissions();

        fileInfo currFile;
        //get file name and path
        currFile.name = file.path();
        
        //get file type
        if(std::filesystem::is_directory(file.path()))
        {
            currFile.type = 2;//it a dir
        }
        //check for execute permissions
        else if(((p & std::filesystem::perms::owner_exec) != std::filesystem::perms::none) || ((p & std::filesystem::perms::group_exec) != std::filesystem::perms::none) || ((p & std::filesystem::perms::others_exec) != std::filesystem::perms::none)){
            currFile.type = 3;//it an executable
        }
        
        else{

            std::vector <std::string> filePart;

            splitString(currFile.name,'.',filePart);
            std::string fileEndStr = filePart.at(filePart.size()-1);
            const char* fileEnd = fileEndStr.c_str();
            //std::cout << "fileEnd is " << fileEnd << "\n";
            if(strcmp(fileEnd,"jpg") == 0|| strcmp(fileEnd,"jpeg") == 0 || strcmp(fileEnd,"png")== 0 || strcmp(fileEnd,"tif")== 0 || strcmp(fileEnd,"tiff")== 0 || strcmp(fileEnd,"gif")== 0){
                currFile.type = 4;//it an image
            }
            else if(strcmp(fileEnd,"mp4")== 0 ||strcmp(fileEnd,"mov")== 0 || strcmp(fileEnd,"mkv")== 0 || strcmp(fileEnd,"avi")== 0 || strcmp(fileEnd,"webm")== 0){
                currFile.type = 6;//it a video
            }
            else if(strcmp(fileEnd,"h")== 0 || strcmp(fileEnd,"c")== 0 || strcmp(fileEnd,"cpp")== 0 || strcmp(fileEnd,"py")== 0 || strcmp(fileEnd,"java")== 0 || strcmp(fileEnd,"js")== 0){
                currFile.type = 1;//it a code
            }
            else{
                currFile.type = 5;//it a other
            }
            
        }


        //get file size
        try{
            currFile.fileSize = std::filesystem::file_size(file.path());
        }
        catch(std::filesystem::filesystem_error& e){

            currFile.fileSize = 0;
        }
        std::cout << "type is" << currFile.type << std::endl;
        std::cout << "size is " << currFile.fileSize << std::endl;
        
       //get permisisons

        
        std::string stringBuild;
        stringBuild += ((p & std::filesystem::perms::owner_read) != std::filesystem::perms::none ? "r" : "-");
        stringBuild += ((p & std::filesystem::perms::owner_write) != std::filesystem::perms::none ? "w" : "-");
        stringBuild += ((p & std::filesystem::perms::owner_exec) != std::filesystem::perms::none ? "x" : "-");
        stringBuild += ((p & std::filesystem::perms::group_read) != std::filesystem::perms::none ? "r" : "-");
        stringBuild += ((p & std::filesystem::perms::group_write) != std::filesystem::perms::none ? "w" : "-");
        stringBuild += ((p & std::filesystem::perms::group_exec) != std::filesystem::perms::none ? "x" : "-");
        stringBuild += ((p & std::filesystem::perms::others_read) != std::filesystem::perms::none ? "r" : "-");
        stringBuild += ((p & std::filesystem::perms::others_write) != std::filesystem::perms::none ? "w" : "-");
        stringBuild += ((p & std::filesystem::perms::others_exec) != std::filesystem::perms::none ? "x" : "-");
        
        //count++;
        std::cout << stringBuild << std::endl;
        currFile.perms = stringBuild;
        dirContents.push_back(currFile);
        std::cout << file.path() << std::endl;
        i++;
    }
    
    //std::cout << count << std::endl;
    //std::cout << dirCount << std::endl;
    return dirContents;
}

void splitString(std::string text, char d, std::vector<std::string>& result)
{
    enum states { NONE, IN_WORD, IN_STRING } state = NONE;

    int i;
    std::string token;
    result.clear();
    for (i = 0; i < text.length(); i++)
    {
        char c = text[i];
        switch (state) {
            case NONE:
                if (c != d)
                {
                    if (c == '\"')
                    {
                        state = IN_STRING;
                        token = "";
                    }
                    else
                    {
                        state = IN_WORD;
                        token = c;
                    }
                }
                break;
            case IN_WORD:
                if (c == d)
                {
                    result.push_back(token);
                    state = NONE;
                }
                else
                {
                    token += c;
                }
                break;
            case IN_STRING:
                if (c == '\"')
                {
                    result.push_back(token);
                    state = NONE;
                }
                else
                {
                    token += c;
                }
                break;
        }
    }
    if (state != NONE)
    {
        result.push_back(token);
    }
}