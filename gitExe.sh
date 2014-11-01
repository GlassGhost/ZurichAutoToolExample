#!/bin/bash
##uncomment these 2 lines if you want to git the repo
#git clone https://github.com/GlassGhost/ZurichAutoToolExample.git
#cd ./ZurichAutoToolExample/src


cd ./src
touch NEWS README AUTHORS ChangeLog 
autoreconf --install
##to configure for linux it's just:
#./configure
##to cross compile to a Windows Exe comment previous line and uncomment the next one.
./configure --build i686-pc-linux-gnu --host i586-mingw32msvc
make
wine './hw/hw.exe' 33 34 35


##After the autools and building is finish this 40Kib repository becomes ~3400Mib

##To avoid these bytes and Megabytes of files auto-tools generates when compiling
##Commit BEFORE YOU COMPILE, commit but DON'T push to up-stream
##to delete these files you need to run;

#"git gui &"

## Repository -> Visualize all branch history and find the ref that you want to checkout

##to restore to a certain commit
#git checkout -- .;git clean -f -x -d;

##http://crypto.stanford.edu/~blynn/gitmagic/ch02.html#_advanced_undo_redo
##WHEN you are ready to commit to upstream;
##Delete all your garbage commits, by checkin out the one you want to keep with:

#git reset --hard REF;git clean -f -x -d;

##
##THEN, you are ready to push back to your fork & do a pull request.
