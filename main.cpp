#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <assert.h>

namespace fs = std::filesystem;

//Path of folder holding only unzipped Facebook dowload packages
const fs::path start_path = "E:\\Facebook-Info\\Full-Time-Messages";

//Path of folder to sort messages & media into 
const fs::path final_path = "E:\\Facebook-Info\\test";

//Do you want to copy 
bool copy_or_nah = false;

fs::path messages_file_path{};

const auto copy_options = fs::copy_options::skip_existing;

fs::path concatPath(fs::path in_path, fs::path dir)
{
    fs::path out_path = in_path;
    return out_path /= dir;
}

//Turns the package's path into the path to the inbox directory (destination)
fs::path concatPathToInbox(fs::path curr_path)
{
    return concatPath(curr_path, "your_facebook_activity\\messages\\inbox");
}

//Example: eg = getLastDir(E:\\foo\\bar\\) 
//         eg = bar
fs::path getLastDir(fs::path dir_path)
{
    dir_path += "\\";
    return dir_path.parent_path().filename();
}


int main()
{
    if (fs::exists(start_path) && !start_path.empty() && fs::exists(final_path)) 
    {
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

                            if (fs::exists(media_group_start_path) && copy_or_nah)
                                fs::copy(media_group_start_path, media_group_final_path, copy_options);
                        }
                        else
                            if (fs::exists(media_group_start_path) && fs::exists(group_final_path))
                                fs::copy(media_group_start_path, group_final_path, copy_options);
                    }
                }
            }
            else
                std::cout << "\nThe package " << inbox_path << " doesn't have the expected format." << '\n';
        }

        std::cout << "\nDone!";
    }
}
