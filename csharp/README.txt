Melowrite for C# (Windows x64, .NET Standard 2.1)

Add Melo.cs to your project and copy Melowrite.Core.dll next to your exe. The dll is
native, so dont add it as an assembly reference. A .melo's exported folders stay next
to the .melo.

    MeloHost.Init(44100);
    MeloInstance music = Melo.Load("music.melo");
    music.PlayChunk();

See Examples/MonoGame.cs for how to get started.
Run it from this folder to play Examples/Melo, world!.melo, included in the download.
Pass a quoted path to play another exported .melo.
