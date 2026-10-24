using System;
using UnityEngine;
using Melowrite;
using Melowrite.Audio;

[AddComponentMenu("")]
public sealed class MeloUnityHost : MonoBehaviour
{
    private float[] _stereo = Array.Empty<float>();

    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.BeforeSceneLoad)]
    static void Bootstrap()
    {
        MeloHost.ResolvePath = MeloContent.Resolve;
        MeloHost.LogError    = m => Debug.LogError(m);
        MeloHost.LogWarning  = m => Debug.LogWarning(m);
        int rate = AudioSettings.outputSampleRate;
        MeloHost.Init(rate > 0 ? rate : 44100);


        var go = new GameObject("[Melo]") { hideFlags = HideFlags.HideInHierarchy };
        DontDestroyOnLoad(go);
        go.AddComponent<AudioSource>();
        go.AddComponent<MeloUnityHost>();
    }

    void Update() => MeloHost.Tick();

    void OnAudioFilterRead(float[] data, int channels)
    {
        if (channels <= 0) { Array.Clear(data, 0, data.Length); return; }
        int frames = data.Length / channels;

        if (channels == 2)
        {
            MeloHost.FillBuffer(data, frames);
            return;
        }

        int stereoLen = frames * 2;
        if (_stereo.Length < stereoLen) _stereo = new float[stereoLen];
        MeloHost.FillBuffer(_stereo, frames);
        for (int f = 0; f < frames; f++)
        {
            float l = _stereo[f * 2], r = _stereo[f * 2 + 1];
            int dst = f * channels;
            if (channels == 1) { data[f] = (l + r) * 0.5f; continue; }
            data[dst] = l; data[dst + 1] = r;
            for (int c = 2; c < channels; c++) data[dst + c] = 0f;
        }
    }

    void OnDestroy() => MeloHost.Shutdown();
}
