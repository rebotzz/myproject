#define SDL_MAIN_HANDLED
#include <iostream>

#include "resourcesMgr.h"


int main(int, char**)
{
    ResMgr::getInstance().create_NameResID_map("assets", "nameId_ResID.txt");

    std::cout << "code done!\n";

    // using namespace std::filesystem;

    // path entry_path("assets");
    // if(!exists(entry_path))
    // {
    //     SDL_Log("not path");
    //     return 1;
    // }
    // std::cout << entry_path << std::endl;
    // std::cout << entry_path.relative_path() << std::endl;

    // for(auto& entry : recursive_directory_iterator("assets"))
    // {
    //     if(!entry.is_regular_file()) continue;
    //     auto sub_path = entry.path();
    //     std::cout << "sub_path absolute:" << sub_path.u8string() << std::endl;
    //     std::cout << "sub_path relative:" << sub_path.relative_path().u8string() << std::endl;
    // }


    // create_directories(path("1/2/3"));

    return 0;
}