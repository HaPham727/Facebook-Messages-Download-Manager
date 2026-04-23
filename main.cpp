#include <chrono>
#include <iostream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

fs::path getLastDir(fs::path p)
{
    p += "\\";
    return p.parent_path().filename();
}

fs::path concatPath(fs::path p, fs::path d)
{
    return (p / d);
}

fs::path concatPathToInbox(fs::path p)
{
    return concatPath(p, "your_facebook_activity\\messages\\inbox");
}

fs::path concatPathToE2EE(fs::path p)
{
    return concatPath(p, "your_facebook_activity\\messages\\e2ee_cutover");
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
    return ((opt == "y" || opt == "Y") ? true : false);
}

bool isMessageFile(fs::path p)
{
    return (p.extension() == ".html" || p.extension() == ".json");
}

int main()
{
    auto start = std::chrono::high_resolution_clock::now();

    std::string start_string, final_string, media_string, e2ee_string;
    fs::path start_path, final_path;
    fs::copy_options message_copy_options = fs::copy_options::skip_existing;
    fs::copy_options media_copy_options = fs::copy_options::skip_existing | fs::copy_options::recursive;
    bool media_bool, e2ee_bool;

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
    } while (std::getline(std::cin, media_string) && !optionIsValid(media_string));
    media_bool = setOption(media_string);

    do {
        std::cout << "Do you want to copy End-to-end-encrypted chats? [y/N] ";
    } while (std::getline(std::cin, e2ee_string) && !optionIsValid(e2ee_string));
    e2ee_bool = setOption(e2ee_string);
    
    std::cout << "Copying Facebook " << (media_bool ? "media and messages" : "messsages") << (e2ee_bool ? " (including end-to-end-encrypted chats)" : "") << " from " << start_path << " to " << final_path << ".\n" << "Working";

    for (const auto& package_iterator : fs::directory_iterator{start_path}) 
    {
        std::cout << '.';

        fs::path inbox_path = concatPathToInbox(package_iterator.path());
        fs::path e2ee_path = concatPathToE2EE(package_iterator.path());

        if (fs::exists(inbox_path))
        {
            for (const auto& group_iterator : fs::directory_iterator{inbox_path}) 
            {
                fs::path group_start_path = group_iterator.path();
                fs::path group_final_path = concatPath(final_path, getLastDir(group_start_path));

                if (!fs::exists(group_final_path))
                    fs::create_directory(group_final_path);

                for (const auto& media_iterator : fs::directory_iterator{group_start_path}) 
                {
                    fs::path media_group_start_path = media_iterator.path();
                    fs::path media_group_final_path = concatPath(group_final_path, getLastDir(media_group_start_path));

                    if (isMessageFile(media_group_start_path))
                        fs::copy(media_group_start_path, group_final_path, message_copy_options);
                    else if (media_bool)
                    {
                        if (!fs::exists(media_group_final_path))
                            fs::create_directory(media_group_final_path);

                        fs::copy(media_group_start_path, media_group_final_path, media_copy_options);
                    }
                }
            }
        }
        else
            std::cout << "\nThe package " << inbox_path << " doesn't have the expected format." << "\nContinuing.";

        if (e2ee_bool && fs::exists(e2ee_path))
        {
            for (const auto& chat_iterator : fs::directory_iterator{e2ee_path})
            {
                fs::path chat_start_path = chat_iterator.path();
                fs::path chat_final_path = concatPath(final_path, getLastDir(chat_start_path));

                if (!fs::exists(chat_final_path))
                    fs::create_directory(chat_final_path);

                for (const auto& media_iterator : fs::directory_iterator{chat_start_path}) 
                {
                    fs::path media_chat_start_path = media_iterator.path();
                    fs::path media_chat_final_path = concatPath(chat_final_path, getLastDir(media_chat_start_path));

                    if (isMessageFile(media_chat_start_path))
                        fs::copy(media_chat_start_path, chat_final_path, message_copy_options);
                    else 
                    {
                        if (!fs::exists(media_chat_final_path))
                            fs::create_directory(media_chat_final_path);

                        fs::copy(media_chat_start_path, media_chat_final_path, media_copy_options);
                    }
                }
            }
        }
    }
    
    auto stop = std::chrono::high_resolution_clock::now();

    std::cout << "\nFinished after " << duration_cast<std::chrono::seconds>(stop - start) << "!";

    return 0;
}