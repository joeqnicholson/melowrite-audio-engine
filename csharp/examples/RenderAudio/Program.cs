using Melowrite;

// An offline integration check: no audio device or Melowrite app is needed.
const int sampleRate = 44100;
string output = Path.GetFullPath("rendered.wav");
string tone = Path.Combine(Path.GetTempPath(), "melowrite-check-" + Guid.NewGuid().ToString("N") + ".wav");

try
{
    MeloHost.Init(sampleRate);
    if (args.Length > 0)
    {
        MeloInstance music = Melo.Load(Path.GetFullPath(args[0]));
        music.PlayArrangement();
    }
    else
    {
        float[] input = new float[sampleRate / 2 * 2];
        for (int frame = 0; frame < input.Length / 2; frame++)
            input[frame * 2] = input[frame * 2 + 1] = 0.3f * MathF.Sin(frame * 2f * MathF.PI * 440f / sampleRate);
        WriteWav(tone, input);
        if (Melo.PlayOneShot(tone) < 0)
            throw new InvalidOperationException("WAV decoding failed.");
    }

    float[] rendered = new float[sampleRate * 5 * 2];
    float[] block = new float[1024 * 2];
    for (int offset = 0; offset < rendered.Length; offset += block.Length)
    {
        int samples = Math.Min(block.Length, rendered.Length - offset);
        MeloHost.FillBuffer(block, samples / 2);
        Array.Copy(block, 0, rendered, offset, samples);
        MeloHost.Tick();
    }
    WriteWav(output, rendered);
    float peak = rendered.Max(v => MathF.Abs(v));
    if (args.Length == 0 && peak < 0.01f) throw new InvalidOperationException("Runtime rendered silence.");
    Console.WriteLine($"Rendered {output} (peak {peak:0.000}). Open this WAV to listen.");
}
finally
{
    MeloHost.Shutdown();
    if (File.Exists(tone)) File.Delete(tone);
}

static void WriteWav(string path, float[] stereo)
{
    using var file = File.Create(path);
    using var writer = new BinaryWriter(file);
    writer.Write("RIFF"u8); writer.Write(36 + stereo.Length * 2); writer.Write("WAVE"u8);
    writer.Write("fmt "u8); writer.Write(16); writer.Write((short)1); writer.Write((short)2);
    writer.Write(sampleRate); writer.Write(sampleRate * 4); writer.Write((short)4); writer.Write((short)16);
    writer.Write("data"u8); writer.Write(stereo.Length * 2);
    foreach (float sample in stereo) writer.Write((short)(Math.Clamp(sample, -1f, 1f) * short.MaxValue));
}
