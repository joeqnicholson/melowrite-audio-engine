Native example (Windows x64, C++17, MinGW-w64)

Run from this folder:

    g++ -std=c++17 main.cpp ../../Melowrite.Core.dll -static-libgcc -static-libstdc++ -o NativeMiddleware.exe
    copy ..\..\Melowrite.Core.dll .
    NativeMiddleware.exe C:\path\to\music.melo

Renders the project offline and prints its peak level. No audio device needed.
