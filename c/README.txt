MIT license. Third party licenses are in THIRD-PARTY-NOTICES.txt.

C and C++

Include melo.h and link Melowrite.Core.dll.

Dll goes next to the exe.
Melo export folders (samples, soundfonts, etc) go next to the corresponding .melo file.

Examples/play.c is a console player using Windows audio. No extra libraries needed.
Build from this folder with MinGW-w64:

    gcc Examples/play.c -I. Melowrite.Core.dll -lwinmm -o play.exe
    play.exe

It plays Examples/Melo, world!.melo, included in the download. Pass a path to play another song:

    play.exe "path/to/song.melo"

Press Enter to stop.

C uses functions and handles:

    melo_host_init(44100);
    MeloInstanceHandle music = melo_load_instance("music.melo");
    melo_instance_play_chunk(music, NULL, 1, 0);

    // Later, to stop playback:
    melo_instance_stop(music);

C++ provides classes with shorter calls:

    using namespace Melowrite;
    MeloHost::Init(44100);
    MeloInstance music = Melo::Load("music.melo");
    music.PlayChunk();

    // Later, to stop playback:
    music.Stop();
