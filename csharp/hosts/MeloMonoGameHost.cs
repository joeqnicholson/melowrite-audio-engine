using System;
using Microsoft.Xna.Framework.Audio;
using Melowrite;

public sealed class MeloMonoGameHost : IDisposable
{
    private readonly DynamicSoundEffectInstance output;
    private readonly int frames;
    private readonly float[] floats;
    private readonly byte[] pcm16;

    public MeloMonoGameHost(int sampleRate = 44100, int frames = 1024)
    {
        this.frames = frames;
        MeloHost.Init(sampleRate);

        output = new DynamicSoundEffectInstance(sampleRate, AudioChannels.Stereo);
        floats = new float[frames * 2];
        pcm16  = new byte[frames * 2 * sizeof(short)];

        // two buffers queued up front so it never starves on the first frame
        output.BufferNeeded += (sender, e) => Submit();
        Submit();
        Submit();
        output.Play();
    }

    private void Submit()
    {
        // monogame wants 16 bit pcm
        MeloHost.FillBuffer(floats, frames);
        for (int i = 0; i < floats.Length; i++)
        {
            float f = floats[i];
            if (f > 1f) f = 1f; else if (f < -1f) f = -1f;
            short s = (short)(f * short.MaxValue);
            pcm16[i * 2]     = (byte)(s & 0xFF);
            pcm16[i * 2 + 1] = (byte)((s >> 8) & 0xFF);
        }
        output.SubmitBuffer(pcm16);
    }

    // call every frame in Game.Update
    public void Update() => MeloHost.Tick();

    // call on dispose
    public void Dispose()
    {
        output.Stop();
        output.Dispose();
        MeloHost.Shutdown();
    }
}
