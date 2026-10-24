using System;
using System.Runtime.InteropServices;
using SDL2;
using Melowrite;

public sealed class MeloSdlHost : IDisposable
{
    private readonly uint _dev;
    private readonly SDL.SDL_AudioCallback _callback;   // keep a ref so the GC can't collect it
    private float[] _buf = Array.Empty<float>();

    public MeloSdlHost(int sampleRate = 44100, ushort frames = 1024)
    {
        MeloHost.Init(sampleRate);

        _callback = AudioCallback;
        var want = new SDL.SDL_AudioSpec
        {
            freq     = sampleRate,
            format   = SDL.AUDIO_F32SYS,   // interleaved stereo float, native byte order
            channels = 2,
            samples  = frames,
            callback = _callback,
        };
        _dev = SDL.SDL_OpenAudioDevice(IntPtr.Zero, 0, ref want, out _, 0);
        if (_dev == 0) throw new InvalidOperationException("SDL_OpenAudioDevice failed: " + SDL.SDL_GetError());
        SDL.SDL_PauseAudioDevice(_dev, 0);   // 0 = unpause = start playing
    }

    // Runs on SDL's audio thread. `len` is bytes; the stream wants interleaved stereo floats.
    private void AudioCallback(IntPtr userdata, IntPtr stream, int len)
    {
        int floats = len / sizeof(float);
        int frames = floats / 2;
        if (_buf.Length < floats) _buf = new float[floats];
        MeloHost.FillBuffer(_buf, frames);
        Marshal.Copy(_buf, 0, stream, floats);
    }

    // Call once per frame from your game loop so OnNote events fire on the main thread.
    public void Update() => MeloHost.Tick();

    public void Dispose()
    {
        SDL.SDL_CloseAudioDevice(_dev);
        MeloHost.Shutdown();
    }
}
