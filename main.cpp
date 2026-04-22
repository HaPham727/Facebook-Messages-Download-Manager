#include <iostream>
#include <filesystem>
#include <sstream>
#include <string>

namespace fs = std::filesystem;

fs::path concatPath(fs::path in_path, fs::path dir)
{
    fs::path out_path = in_path;
    return out_path /= dir;
}

fs::path concatPathToInbox(fs::path curr_path)
{
    return concatPath(curr_path, "your_facebook_activity\\messages\\inbox");
}

fs::path getLastDir(fs::path dir_path)
{
    dir_path += "\\";
    return dir_path.parent_path().filename();
}

bool pathExists(std::string str)
{
    return fs::exists(static_cast<fs::path>(str));
}

bool optionIsValid(std::string_view opt)
{
    return (opt.size() == 1 && (opt == "y" || opt == "Y" || opt == "n" || opt == "N"));
}

bool setOption(std::string_view opt)
{
    return (opt == "y" || opt == "Y") ? true : false;
}

int main()
{
    std::string start_string, final_string, option_string;
    fs::path start_path, final_path;
    bool option_bool;

    do {
        std::cout << "Enter path to starting folder: ";
    } while (std::getline(std::cin, start_string) && !pathExists(start_string));
    start_path = static_cast<fs::path>(start_string);

    do {
        std::cout << "Enter path to final folder: ";
    } while (std::getline(std::cin, final_string) && !pathExists(final_string));
    final_path = static_cast<fs::path>(final_string);

    do {
        std::cout << "Do you want to copy media? [y/N] ";
    } while (std::getline(std::cin, option_string) && !optionIsValid(option_string));
    option_bool = setOption(option_string);
    
    std::cout << "Copying Facebook " << (option_bool ? "messsages and media" : "messsages") << " from " << start_path << " to " << final_path << ".\n";

    std::cout << "Working";

    for (const auto& package_iterator : fs::directory_iterator{start_path}) 
    {
        std::cout << '.';

        fs::path inbox_path = concatPathToInbox(package_iterator.path());

        if (fs::exists(inbox_path))
        {
            for (auto const& group_iterator : fs::directory_iterator{inbox_path}) 
            {
                fs::path group_start_path = group_iterator.path();
                fs::path curr_group = getLastDir(group_start_path);
                fs::path group_final_path = concatPath(final_path, curr_group);

                fs::current_path(final_path);
                fs::create_directory(curr_group);

                for (auto const& media_iterator : fs::directory_iterator{group_start_path}) 
                {
                    fs::path media_group_start_path = media_iterator.path();
                    fs::path curr_media = getLastDir(media_group_start_path); 
                    fs::path media_group_final_path = concatPath(group_final_path, curr_media);

                    if (curr_media != "message_1.html")
                    {
                        fs::current_path(group_final_path);

                        if (!fs::exists(media_group_final_path))
                            fs::create_directory(curr_media);

                        if (fs::exists(media_group_start_path) && option_bool)
                            fs::copy(media_group_start_path, media_group_final_path, fs::copy_options::skip_existing);
                    }
                    else
                        if (fs::exists(media_group_start_path) && fs::exists(group_final_path))
                            fs::copy(media_group_start_path, group_final_path, fs::copy_options::skip_existing);
                }
            }
        }
        else
            std::cout << "\nThe package " << inbox_path << " doesn't have the expected format." << '\n';
    }

    std::cout << "\nDone!";

    return 0;
}