// Use as Program.cs with Melo.cs and the MonoGame.Framework.DesktopGL package.
// Put Melowrite.Core.dll beside the executable. Run from the download's root folder.
// Plays the bundled demo by default; optionally pass a path to another exported .melo.
using System;
using System.IO;
using Melowrite;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Input;

if (args.Length > 1)
{
    Console.WriteLine("dotnet run -- [path/to/music.melo]");
    return;
}

string songPath = args.Length == 1 ? args[0] : "Examples/Melo, world!.melo";
using MusicGame game = new MusicGame(Path.GetFullPath(songPath));
game.Run();

sealed class MusicGame : Game
{
    private readonly string songPath;
    private readonly float[] samples = new float[1024 * 2];
    private readonly byte[] pcm = new byte[1024 * 2 * sizeof(short)];
    private DynamicSoundEffectInstance? output;
    private bool engineStarted;

    public MusicGame(string songPath)
    {
        this.songPath = songPath;
        new GraphicsDeviceManager(this);
        Window.Title = "Melowrite - Escape to quit";
        IsMouseVisible = true;
    }

    protected override void Initialize()
    {
        base.Initialize();
        MeloHost.Init(44100);
        engineStarted = true;
        MeloInstance music = Melo.Load(songPath);
        if (!music.IsLoaded) throw new InvalidOperationException("Could not load " + songPath);
        music.PlayChunk();

        output = new DynamicSoundEffectInstance(44100, AudioChannels.Stereo);
        output.BufferNeeded += (_, _) => FillAudio();
        FillAudio();
        FillAudio();
        output.Play();
    }

    private void FillAudio()
    {
        MeloHost.FillBuffer(samples, samples.Length / 2);
        // MonoGame takes 16-bit PCM; Melowrite produces floats.
        for (int i = 0; i < samples.Length; i++)
        {
            short value = (short)(Math.Clamp(samples[i], -1f, 1f) * short.MaxValue);
            pcm[i * 2] = (byte)(value & 0xFF);
            pcm[i * 2 + 1] = (byte)((value >> 8) & 0xFF);
        }
        output!.SubmitBuffer(pcm);
    }

    protected override void Update(GameTime gameTime)
    {
        if (Keyboard.GetState().IsKeyDown(Keys.Escape)) Exit();
        MeloHost.Tick();
        base.Update(gameTime);
    }

    protected override void Draw(GameTime gameTime)
    {
        GraphicsDevice.Clear(Color.CornflowerBlue);
        base.Draw(gameTime);
    }

    protected override void Dispose(bool disposing)
    {
        if (disposing)
        {
            output?.Stop();
            output?.Dispose();
            output = null;
            if (engineStarted) MeloHost.Shutdown();
            engineStarted = false;
        }
        base.Dispose(disposing);
    }
}
