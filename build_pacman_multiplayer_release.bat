set QMAKE_PATH=C:\Qt\5.15.0\mingw81_64\bin
set MINGW32_MAKE_PATH=C:\Qt\Tools\mingw810_64\bin
set REPO_PATH=C:\Users\Adam\Desktop\repozytoria\PacmanMultiplayer_Qt_Cplusplus
set PROCESSOR_COUNT=24

rem DELETE EXECUTABLES
del Server\executable\Pacman_Server.exe
del Client\executable\Pacman_Client.exe

rmdir /Q /S build-Pacman_Server-Desktop_Qt_5_15_0_MinGW_64_bit-Release
rmdir /Q /S build-Pacman_Client-Desktop_Qt_5_15_0_MinGW_64_bit-Release

rem BUILD SERVER
mkdir build-Pacman_Server-Desktop_Qt_5_15_0_MinGW_64_bit-Release
cd build-Pacman_Server-Desktop_Qt_5_15_0_MinGW_64_bit-Release

%QMAKE_PATH%\qmake.exe %REPO_PATH%\Server\Pacman_Server.pro -spec win32-g++ "CONFIG+=qtquickcompiler"
%MINGW32_MAKE_PATH%\mingw32-make.exe -j%PROCESSOR_COUNT% -f %REPO_PATH%/build-Pacman_Server-Desktop_Qt_5_15_0_MinGW_64_bit-Release/Makefile qmake_all
%MINGW32_MAKE_PATH%\mingw32-make.exe -j%PROCESSOR_COUNT% -f Makefile.Release

cd ..

rem BUILD CLIENT
mkdir build-Pacman_Client-Desktop_Qt_5_15_0_MinGW_64_bit-Release
cd build-Pacman_Client-Desktop_Qt_5_15_0_MinGW_64_bit-Release

%QMAKE_PATH%\qmake.exe %REPO_PATH%\Client\Pacman_Client.pro -spec win32-g++ "CONFIG+=qtquickcompiler"
%MINGW32_MAKE_PATH%\mingw32-make.exe -j%PROCESSOR_COUNT% -f %REPO_PATH%/build-Pacman_Client-Desktop_Qt_5_15_0_MinGW_64_bit-Release/Makefile qmake_all
%MINGW32_MAKE_PATH%\mingw32-make.exe -j%PROCESSOR_COUNT% -f Makefile.Release