using System;
using Microsoft.Xna.Framework.Audio;
using Melowrite;

public sealed class MeloMonoGameHost : IDisposable
{
    private readonly DynamicSoundEffectInstance _output;
    private readonly int _frames;
    private readonly float[] _float;
    private readonly byte[] _pcm16;

    public MeloMonoGameHost(int sampleRate = 44100, int frames = 1024)
    {
        _frames = frames;
        MeloHost.Init(sampleRate);

        _output = new DynamicSoundEffectInstance(sampleRate, AudioChannels.Stereo);
        _float  = new float[frames * 2];
        _pcm16  = new byte[frames * 2 * sizeof(short)];

        _output.BufferNeeded += (_, _) => Submit();
        Submit();
        Submit();
        _output.Play();
    }

    private void Submit()
    {
        MeloHost.FillBuffer(_float, _frames);
        for (int i = 0; i < _float.Length; i++)
        {
            float f = _float[i];
            if (f > 1f) f = 1f; else if (f < -1f) f = -1f;
            short s = (short)(f * short.MaxValue);
            _pcm16[i * 2]     = (byte)(s & 0xFF);
            _pcm16[i * 2 + 1] = (byte)((s >> 8) & 0xFF);
        }
        _output.SubmitBuffer(_pcm16);
    }

    // Call once per frame from Game.Update so OnNote events fire on the main thread.
    public void Update() => MeloHost.Tick();

    public void Dispose()
    {
        _output.Stop();
        _output.Dispose();
        MeloHost.Shutdown();
    }
}
