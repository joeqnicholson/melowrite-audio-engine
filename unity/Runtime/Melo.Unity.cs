using System;
using System.Collections.Generic;
using UnityEngine;

namespace Melowrite
{
    // unity only: MeloFile assets and AudioClip sfx. other hosts dont compile this
    public static partial class Melo
    {
        // resolved path back to its MeloFile, so instance.File can hand the asset back
        private static readonly Dictionary<string, MeloFile> fileByPath = new Dictionary<string, MeloFile>();

        internal static void Register(MeloFile file)
        {
            if (file == null) return;
            string resolved = MeloHost.ResolvePath(file.ProjectPath);
            if (resolved != null) fileByPath[resolved] = file;
        }

        internal static MeloFile FileForPath(string resolvedPath)
        {
            MeloFile file;
            if (resolvedPath != null && fileByPath.TryGetValue(resolvedPath, out file)) return file;
            return null;
        }

        // load a .melo you dragged into the inspector
        public static MeloInstance Load(MeloFile file)
        {
            if (file == null) throw new ArgumentNullException(nameof(file));
            Register(file);
            return Load(file.ProjectPath);
        }

        // load off the main thread. onLoaded fires on the main thread, or leave it null to just warm it up for a SwitchSong
        public static void LoadAsync(MeloFile file, Action<MeloInstance> onLoaded = null)
        {
            if (file == null)
            {
                if (onLoaded != null) onLoaded(null);
                return;
            }
            Register(file);
            LoadAsync(file.ProjectPath, onLoaded);
        }

        // a mix you dragged into the inspector
        public static MeloMix LoadMix(MeloFile file)
        {
            if (file == null) throw new ArgumentNullException(nameof(file));
            return LoadMix(file.ProjectPath);
        }

        // play an AudioClip once. the pcm gets copied in the first time and cached.
        // clip needs Decompress On Load, Streaming wont read
        public static int PlayOneShot(AudioClip clip, float volume = 1f, float pan = 0f, float pitch = 1f,
                                      bool loop = false, bool effects = true)
        {
            string key = LoadUnityClip(IntPtr.Zero, clip);
            if (key == null) return -1;
            return PlayClip(IntPtr.Zero, key, volume, pan, pitch, loop, effects);
        }

        // copies the clip's pcm in once, keyed by instance id. null if it couldnt
        internal static string LoadUnityClip(IntPtr owner, AudioClip clip)
        {
            if (clip == null)
            {
                Debug.LogError("[Melo] PlayOneShot: null AudioClip");
                return null;
            }
            string key = "unity:" + clip.GetInstanceID();
            if (ClipIsLoaded(owner, key)) return key;

            int channels = Mathf.Max(1, clip.channels);
            int frames = clip.samples;
            if (frames < 2)
            {
                Debug.LogError("[Melo] AudioClip '" + clip.name + "' has no samples");
                return null;
            }

            float[] interleaved = new float[frames * channels];
            if (!clip.GetData(interleaved, 0))
            {
                Debug.LogError("[Melo] Couldn't read AudioClip '" + clip.name + "' (set its Load Type to Decompress On Load, not Streaming)");
                return null;
            }
            float[] left = new float[frames];
            float[] right = new float[frames];
            for (int i = 0; i < frames; i++)
            {
                left[i] = interleaved[i * channels];
                // mono clips play the same thing both sides
                right[i] = channels >= 2 ? interleaved[i * channels + 1] : interleaved[i * channels];
            }

            if (!LoadClipPcm(owner, key, left, right, clip.frequency)) return null;
            return key;
        }

        static partial void ClearPlatformState()
        {
            fileByPath.Clear();
        }
    }

    public sealed partial class MeloInstance
    {
        // the .melo this is playing right now, changes when a SwitchSong lands.
        // null if it didnt come from a MeloFile
        public MeloFile File => Melo.FileForPath(FilePath);

        // switch to another song by its MeloFile
        public void SwitchSong(MeloFile file, MeloSwitch when = MeloSwitch.Bar, float fadeOut = 0f)
        {
            if (file == null) return;
            Melo.Register(file);
            SwitchSong(file.ProjectPath, when, fadeOut);
        }

        public void SwitchSong(MeloFile file, string startChunk, MeloSwitch when = MeloSwitch.Bar, float fadeOut = 0f)
        {
            if (file == null) return;
            Melo.Register(file);
            SwitchSong(file.ProjectPath, startChunk, when, fadeOut);
        }

        public void SwitchSong(MeloFile file, int startChunk, MeloSwitch when = MeloSwitch.Bar, float fadeOut = 0f)
        {
            if (file == null) return;
            Melo.Register(file);
            SwitchSong(file.ProjectPath, startChunk, when, fadeOut);
        }

        public void SwitchSongCrossfade(MeloFile file, float duration, MeloSwitch when = MeloSwitch.Bar)
        {
            if (file == null) return;
            Melo.Register(file);
            SwitchSongCrossfade(file.ProjectPath, duration, when);
        }

        public void SwitchSongCrossfade(MeloFile file, string startChunk, float duration, MeloSwitch when = MeloSwitch.Bar)
        {
            if (file == null) return;
            Melo.Register(file);
            SwitchSongCrossfade(file.ProjectPath, startChunk, duration, when);
        }

        public void SwitchSongCrossfade(MeloFile file, int startChunk, float duration, MeloSwitch when = MeloSwitch.Bar)
        {
            if (file == null) return;
            Melo.Register(file);
            SwitchSongCrossfade(file.ProjectPath, startChunk, duration, when);
        }

        // a mix you dragged into the inspector, stays through SwitchSong
        public MeloMix LoadMix(MeloFile file)
        {
            if (file == null) throw new ArgumentNullException(nameof(file));
            return LoadMix(file.ProjectPath);
        }

        // AudioClip through this song's master effects and volume. same clip rules as Melo.PlayOneShot
        public int PlayOneShot(AudioClip clip, float volume = 1f, float pan = 0f, float pitch = 1f,
                               bool loop = false, bool effects = true)
        {
            if (!IsLoaded) return -1;
            string key = Melo.LoadUnityClip(NativeHandle, clip);
            if (key == null) return -1;
            return Melo.PlayClip(NativeHandle, key, volume, pan, pitch, loop, effects);
        }
    }
}
