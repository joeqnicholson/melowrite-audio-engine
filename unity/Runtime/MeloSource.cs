using System;
using UnityEngine;
using Melowrite;

/// <summary>plays a .melo, drop it on a GameObject</summary>
[AddComponentMenu("Melowrite/Melo Source")]
public class MeloSource : MonoBehaviour
{
    [Tooltip("The .melo project to play.")]
    public MeloFile song;

    [Tooltip("Enter the chunk Index: 0, 1, 2, or the chunk name you'd like to play. Leave blank to play the arrangement")]
    public string startChunk = "";

    [Tooltip("Loop the start chunk. The arrangement loops the way it does in Melowrite.")]
    public bool loop = true;

    [Tooltip("Start playing on Awake.")]
    public bool playOnAwake = true;

    [Header("Mix (live)")]
    [Range(0f, 1f)] public float volume = 1f;
    [Range(-1f, 1f)] public float pan = 0f;
    [Tooltip("Beats per minute. 0 = the project's authored tempo.")]
    public int tempo = 0;

    /// <summary>the loaded song, null until something loads</summary>
    public MeloInstance Instance { get; private set; }

    void Awake()
    {
        if (song == null) return;
        Instance = Melo.Load(song);
        if (playOnAwake) Play();
    }

    void Update()
    {
        if (Instance == null) return;
        Instance.SetMasterVolume(volume);
        Instance.SetPan(pan);
        if (tempo > 0) Instance.SetTempo(tempo);
    }

    void OnDestroy()
    {
        if (Instance != null) Instance.Stop();
    }

    /// <summary>plays startChunk, or the arrangement if it's blank</summary>
    public void Play()
    {
        if (Instance == null && song != null) Instance = Melo.Load(song);
        if (Instance == null) return;
        if (string.IsNullOrEmpty(startChunk)) Instance.PlayArrangement();
        else if (int.TryParse(startChunk, out int idx)) Instance.PlayChunk(idx, loop);
        else Instance.PlayChunk(startChunk, loop);
    }

    /// <summary>load without playing</summary>
    public MeloInstance Load(MeloFile file)
    {
        if (Instance != null) Instance.Stop();
        song = file;
        Instance = file != null ? Melo.Load(file) : null;
        return Instance;
    }

    /// <summary>load off the main thread, onLoaded fires on the main thread</summary>
    public void LoadAsync(MeloFile file, Action<MeloInstance> onLoaded = null)
    {
        if (Instance != null) Instance.Stop();
        song = file;
        Instance = null;
        if (file == null)
        {
            if (onLoaded != null) onLoaded(null);
            return;
        }
        Melo.LoadAsync(file, loaded =>
        {
            Instance = loaded;
            if (onLoaded != null) onLoaded(loaded);
        });
    }

    public void Unload()
    {
        if (Instance != null) Instance.Unload();
        Instance = null;
    }

    public MeloFile File => Instance != null ? Instance.File : song;
    public bool IsLoaded => Instance != null && Instance.IsLoaded;

    public void PlayChunk(string chunk = null, bool looping = true, float fadeOut = 0f) { if (Instance != null) Instance.PlayChunk(chunk, looping, fadeOut); }
    public void PlayChunk(int index, bool looping = true, float fadeOut = 0f) { if (Instance != null) Instance.PlayChunk(index, looping, fadeOut); }
    public void PlayArrangement(float fadeOut = 0f) { if (Instance != null) Instance.PlayArrangement(fadeOut); }
    public void Stop() { if (Instance != null) Instance.Stop(); }
    public void Pause() { if (Instance != null) Instance.Pause(); }
    public void Resume() { if (Instance != null) Instance.Resume(); }
    public void FadeOut(float duration = 1f) { if (Instance != null) Instance.FadeOut(duration); }

    public void SwitchChunk(string chunk, MeloSwitch when = MeloSwitch.Bar) { if (Instance != null) Instance.SwitchChunk(chunk, when); }
    public void SwitchChunk(int index, MeloSwitch when = MeloSwitch.Bar) { if (Instance != null) Instance.SwitchChunk(index, when); }
    public void SwitchSong(MeloFile file, MeloSwitch when = MeloSwitch.Bar, float fadeOut = 0f) { if (Instance != null) Instance.SwitchSong(file, when, fadeOut); }
    public void SwitchSong(MeloFile file, string startChunk, MeloSwitch when = MeloSwitch.Bar, float fadeOut = 0f) { if (Instance != null) Instance.SwitchSong(file, startChunk, when, fadeOut); }
    public void SwitchSong(MeloFile file, int startChunk, MeloSwitch when = MeloSwitch.Bar, float fadeOut = 0f) { if (Instance != null) Instance.SwitchSong(file, startChunk, when, fadeOut); }
    public void SwitchSongCrossfade(MeloFile file, float duration, MeloSwitch when = MeloSwitch.Bar) { if (Instance != null) Instance.SwitchSongCrossfade(file, duration, when); }
    public void SwitchSongCrossfade(MeloFile file, string startChunk, float duration, MeloSwitch when = MeloSwitch.Bar) { if (Instance != null) Instance.SwitchSongCrossfade(file, startChunk, duration, when); }
    public void SwitchSongCrossfade(MeloFile file, int startChunk, float duration, MeloSwitch when = MeloSwitch.Bar) { if (Instance != null) Instance.SwitchSongCrossfade(file, startChunk, duration, when); }

    // these stick, Update pushes the fields every frame
    /// <summary>0 goes back to the song's own tempo</summary>
    public void SetTempo(int bpm) { tempo = bpm; if (Instance != null) Instance.SetTempo(bpm); }
    public void SetMasterVolume(float v) { volume = v; if (Instance != null) Instance.SetMasterVolume(v); }
    public void SetPan(float p) { pan = p; if (Instance != null) Instance.SetPan(p); }
}
