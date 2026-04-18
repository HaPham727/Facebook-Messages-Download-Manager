# Facebook Messages Download Manager
A small program to sort downloaded Facebook messages by groups/users (NOT including end-to-end encrypted chats). 

This repo contains a guide to download and sort your Facebook messages using this program.

# Download your Facebook messages
  - On PC, open [Facebook](https://web.facebook.com/).
  - Login if you haven't already.
  - Click on your profile picture in the top-right corner.
  - Click on "Settings & privacy".
  - Click on "Settings".
  - In the "Search Settings" bar, search for "Download your information".
  - In "Export your information", click on "Create export".
  - Pick the profile from which you'd like to export.
  - Pick "Export to device".
  - Under "Confirm your export", open "Customize information" and "Clear all" other than "Messages" and press "Save".
  - "Format" should be set to HTML.
  - Press "Start export" and wait for Facebook to notify you via Email or Facebook notifications.
  - Once the download is available, create a folder on your PC to hold all the .zip files.
  - Once the download is finished, "Extract all" of the .zip files and delete them.
  - Make sure NOT to modify your downloads in any way.

# Using the Facebook Messages Download Manager
  - Download the .zip file from this repo.
  - Extract all.
  - Open main.cpp in an IDE.
  - Set 'starting_file_path' to the path of the file containing the unzipped downloads from Facebook.
  - Set 'starting_file_path' to the path of the file to contain the sorted dowloads.
  - Set 'toCopyOrToNotCopy' to 'true'.
  - Build and run main.cpp.
