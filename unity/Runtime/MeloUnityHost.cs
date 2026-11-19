using System;
using UnityEngine;
using Melowrite;

[AddComponentMenu("")]
public sealed class MeloUnityHost : MonoBehaviour
{
    private float[] stereo = Array.Empty<float>();

    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.BeforeSceneLoad)]
    static void Bootstrap()
    {
        MeloHost.ResolvePath = MeloContent.Resolve;
        MeloHost.LogError    = m => Debug.LogError(m);
        MeloHost.LogWarning  = m => Debug.LogWarning(m);
        int rate = AudioSettings.outputSampleRate;
        // 44100 if unity hasnt picked an output rate yet
        MeloHost.Init(rate > 0 ? rate : 44100);

        GameObject go = new GameObject("[Melo]") { hideFlags = HideFlags.HideInHierarchy };
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
        if (stereo.Length < stereoLen) stereo = new float[stereoLen];
        MeloHost.FillBuffer(stereo, frames);
        for (int f = 0; f < frames; f++)
        {
            float l = stereo[f * 2], r = stereo[f * 2 + 1];
            int dst = f * channels;
            if (channels == 1) { data[f] = (l + r) * 0.5f; continue; }
            data[dst] = l; data[dst + 1] = r;
            for (int c = 2; c < channels; c++) data[dst + c] = 0f;
        }
    }

    void OnDestroy() => MeloHost.Shutdown();
}
