

#include <iostream>
#include <filesystem>
#include <cstdlib>    //for CreateProcess()
#include <string>     //for getline()

namespace fs = std::filesystem;

int main()
{
    std::cout << "Welcome to Folder Viewer 0.30!" << "\n";
    drive_choosing:
    std::cout << "Avaible drives: " << "\n";
    for (std::string avaibleDrive = "A"; avaibleDrive <= "Z"; avaibleDrive=avaibleDrive[0]+1) { //strings A,B...Z
        if (fs::is_directory(avaibleDrive + ":\\\\"))
            std::cout << avaibleDrive << "\n";
    }
    std::cout << "Enter the drive name to explore that drive" << "\n";
    std::cout << "Drive letter: ";
    std::string drive;
    getline(std::cin, drive);
    while (!fs::is_directory( drive + ":\\" )) {
        std::cout<<"\""<<drive<<"\" Is not a valid drive name. Enter an existing drive name" << "\n";
        std::cout << "Drive letter: ";
        getline(std::cin, drive);
    }
    fs::path thePath(drive + ":\\");
    std::string command;
    system("cls");
    while (command != "/exit") {
        int errors = 0;
        std::cout << "The directory " << thePath << " contains:" << "\n";
        std::cout << "-----------------------------------------------------------" << "\n";
        std::cout << "----FOLDERS----" << "\n";
        for (const auto& entry : fs::directory_iterator(thePath)) {
            try {
                if (fs::is_directory(entry.path())) {
                    try {
                        std::cout << entry.path().filename() << std::endl;
                    }
                    catch (...) {
                        errors++;
                    }
                }
            }
            catch (...) {}
        }
        std::cout << "\n" << "-----FILES-----" << "\n";
        for (const auto& entry : fs::directory_iterator(thePath)) {
            try {
                if (!fs::is_directory(entry.path())) {
                    try {
                        std::cout << entry.path().filename() << std::endl;
                    }
                    catch (...) {
                        errors++;
                    }
                }
            }
            catch (...) {}
        }
        if (errors > 0) {
            std::cout << "-----------------------------------------------------------" << "\n";
            std::cout << errors << " file(s) named with unsupported special characters detected" << "\n";
            std::cout <<"showing their names would cause a crash" << "\n";
        }
        std::cout << "-----------------------------------------------------------" << "\n";
        std::cout << "Enter a file name (with its extension) to run it, or" << "\n"
                  << "enter a folder name to open it, or" << "\n"
                  << "type \"..\" or \"/back\" to go back, or" << "\n"
                  << "type \"/changedrive\" to exit the directory and change the drive letter, or" << "\n"
                  << "type \"/exit\" to exit the program." << "\n";
        std::cout << "The command: ";
        getline(std::cin, command);
        fs::path newPath = thePath;
        newPath /= command;
        std::cout << newPath << "\n";
        system("cls");
        if (command == ".." || command == "/back")
            if (thePath == thePath.root_path())
                goto drive_choosing;
            else
                thePath = thePath.parent_path();
        else if (command == "/changedrive")
            goto drive_choosing;
        else if (fs::exists(newPath)) {
            if (fs::is_directory(newPath))
                thePath = newPath;
            else {
                std::string toRun = "\"";

                toRun += newPath.string();
                toRun += "\"";
                system(toRun.c_str());
                command = "/exit";
            }
        }
        else {
            if (command == "/exit") {
                system("pause");
                return 0;
            }
            std::cout << "CANNOT RUN: " << newPath << "- No such file nor directory" << "\n";
        }

    }
}
