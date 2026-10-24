Melowrite for C# (Windows x64, .NET Standard 2.1)

1. Add Melo.cs to your project. Copy Melowrite.Core.dll beside your executable.
   The DLL is native; do not add it as a .NET assembly reference.
2. Keep exported asset folders beside your .melo file.
3. Call MeloHost.Init(sampleRate), then Melo.Load(path) and music.PlayChunk().
4. Call MeloHost.FillBuffer(buffer, frames) from your audio callback (stereo floats).
5. Call MeloHost.Tick() each frame. Stop audio callbacks before MeloHost.Shutdown().

MonoGame/FNA: add hosts/MeloMonoGameHost.cs, create the host, call Update() each
frame, and Dispose() when finished. SDL2-CS has hosts/MeloSdlHost.cs.

See Examples/RenderAudio for a working example and Melo.cs for API help.
Engine handles are IntPtr. Instrument settings use JSON.
MIT license. Third-party licenses are in THIRD-PARTY-NOTICES.txt.
