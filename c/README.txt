Melowrite for C and C++ (Windows x64)

1. Include melo.h and link against Melowrite.Core.dll. C++ requires C++17.
2. Put the DLL beside your executable. Keep exported asset folders beside the .melo file.
3. Call MeloHost::Init(sampleRate), then Melo::Load(path) and music.PlayChunk().
4. Call MeloHost::FillBuffer(buffer, frames) from your audio callback (stereo floats).
5. Call MeloHost::Tick() each frame. Stop audio callbacks before MeloHost::Shutdown().

C uses the matching melo_host_* and melo_instance_* functions in melo.h.
See Examples/NativeMiddleware for a working example.
MIT license. Third-party licenses are in THIRD-PARTY-NOTICES.txt.

