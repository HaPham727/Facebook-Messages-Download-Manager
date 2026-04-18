#include <iostream>
#include <fstream>
#include <filesystem>
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <assert.h>


namespace fs = std::filesystem;

//Standard:
//functionName() = camelCase
//variable_name = snake_case

//Path of folder holding only unzipped Facebook dowload packages
const fs::path starting_file_path = "E:\\Facebook-Info\\Full-Time-Messages";

//Path of folder to sort messages & media into 
const fs::path final_file_path = "E:\\Facebook-Info\\Sorted-Messages";

bool toCopyOrToNotCopy = false;

fs::path messages_file_path{};

const auto copy_options = fs::copy_options::skip_existing
                        | fs::copy_options::recursive
                        ;

fs::path concatPath(fs::path in_path, fs::path dir)
{
    fs::path out_path = in_path;
    return out_path /= dir;
}

//Turns the package's path into the path to the inbox directory (destination)
fs::path concatPathToDest(fs::path curr_path)
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
    if (fs::exists(starting_file_path) && !starting_file_path.empty() && fs::exists(final_file_path)) 
    {
        std::unordered_map<fs::path, std::vector<fs::path>> group_and_dirs;
        std::unordered_map<fs::path, std::unordered_set<fs::path>> group_and_media;

        //Store the name and paths of groups
        for (auto const& dir_iterator : fs::directory_iterator{starting_file_path}) 
        {
            fs::path curr_path = concatPathToDest(dir_iterator.path());

            if (fs::exists(curr_path))
            {
                for (auto const& group_dir_iterator : fs::directory_iterator{curr_path}) 
                {
                    fs::path group_dir_path = group_dir_iterator.path();

                    //std::cout << "Currently at: " << group_dir_path.parent_path().filename() << '\n';
                    //std::cout << "Currently at: " << getLastDir(group_dir_path) << '\n';

                    //if (!m.contains(getLastDir(group_dir_path)))
                    group_and_dirs[getLastDir(group_dir_path)].emplace_back(group_dir_path);
                    //else
                }
            }
        }

        //Create folders for groups and media types at the final file path 
        fs::current_path(final_file_path);
        for (const auto & [group, dirs] : group_and_dirs) 
        {
            fs::current_path(final_file_path);
            if (!fs::exists(concatPath(final_file_path, group)))
                fs::create_directory(group);
            fs::current_path(concatPath(final_file_path, group));

            //std::cout << "Paths for group " << group << " are as follows: " << '\n';

            for (auto i{0uz}; i < dirs.size(); i++)
            {
                //if (i != 0)
                //    std::cout << '\n';
                //std::cout << dirs[i];

                for (auto const& group_media_iterator : fs::directory_iterator{group_and_dirs[group][i]}) 
                {
                    fs::path group_media_path = group_media_iterator.path();
                    if (getLastDir(group_media_path) != "message_1.html")
                        group_and_media[group].insert(getLastDir(group_media_path));
                    //HERE IS WHERE I ADD CONDITION TO SAVE THE PATH OF THE PACKAGE THAT HOLDS MESSAGES
                }
            }

            //std::cout << '\n' << '\n';
        }

        //Media debug
        for (const auto & [group, medias] : group_and_media) 
        {
            std::cout << "Medias for group " << group << " are as follows: " << '\n';

            bool check = true;
            for (const auto& m : medias) 
            {
                if (check)
                {
                    std::cout << '\n';
                    check = false;                    
                }

                std::cout << m << " ";
            }

            std::cout << '\n' << '\n';
        }

        //Copy messages and media to final file
        //For each group
        for (const auto & [group, dirs] : group_and_dirs)
        {
            fs::path curr_path = concatPath(final_file_path, group);

            assert(fs::exists(curr_path));

            //For each media type the group has been known to have
            for (const auto& m : group_and_media[group]) 
            {
                //Make sure to goto final group dir
                fs::current_path(curr_path);

                //Make media path inside final group dir
                fs::path to_media_path = concatPath(curr_path, m);

                //Check if the dir for media type exists yet, if not, create it
                if (!fs::exists(to_media_path))
                    fs::create_directory(m);

                //For each from dir
                for (int i{0uz}; i < dirs.size(); i++)
                {
                    fs::path from_media_path = concatPath(dirs[i], m);
                    //Check if the media type exists in the FROM dir
                    if (fs::exists(from_media_path) && toCopyOrToNotCopy)
                        fs::copy(from_media_path, to_media_path, copy_options);
                }
            }
        }

        std::cout << "Done!";
    }
}
