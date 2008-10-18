rem run in the cmd prompt or powershell by typing .\makeall-win32.bat
@echo on
cd dlflcore
qmake
mingw32-make
cd ..\dlflaux
qmake
mingw32-make
cd ..\arcball
qmake
mingw32-make
cd ..\vecmat
qmake
mingw32-make
cd..
qmake
lupdate topmod.pro
lrelease topmod.pro
mingw32-make
pause