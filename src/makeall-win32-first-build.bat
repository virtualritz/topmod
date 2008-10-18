::Make sure you have the following programs installed
::Qt 4.4.x for minGW (make sure you choose to installed MinGW with it),
::MSYS, SVN, and TortoiseSVN, and Python2.5

::Also be sure to edit your PATH environment variable to point to
::C:\Qt\4.4.x\bin;C:\MinGW\bin;C:\MSYS\1.x\bin;C:\<path to subversion>;C:\Python25

::now you should be ready to download the topmod source code through TortoiseSVN
::and compile it for a first time build using the script below

::once you have run this once, you should only need to run mingw32-make in the main directory 
::and the include directory. It should rebuild the makefile for you now.

::put this batch script in the topmod_gui source folder and run it from there.

::create vecmat makefile and build it for the first time
cd vecmat
qmake
mingw32-make

::create dlflcore makefile and build it for the first time
cd ..\dlflcore
qmake
mingw32-make

::create dlflaux makefile and build it for the first time
cd ..\dlflaux
qmake
mingw32-make

cd ..\arcball
qmake
mingw32-make

::run qmake && mingw32-make in the include directory
cd ..
qmake
mingw32-make

::compile the python scripting engine
cd pydlfl
mingw32-make

::run qmake and mingw32-make for the main directory
cd ..\..
qmake
::for language translation files
lupdate topmod.pro
lrelease topmod.pro

::run the main topmod makefile
mingw32-make

pause