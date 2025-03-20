#include "Settings.h"

Settings::Settings(std::function<std::shared_ptr<FileUtil>()> fileUtil)
    : fileUtil_(std::move(fileUtil)
{
    // init();
}

void Settings::init() {
    GetFilePaths();
    CheckJsonFiles();
    ReadDataFromJson();
};