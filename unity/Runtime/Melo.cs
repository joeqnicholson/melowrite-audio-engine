// same native Melowrite.Core.dll the C and C++ downloads use
#nullable disable
using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;

namespace Melowrite
{
    /// <summary>when a switch lands</summary>
    public enum MeloSwitch
    {
        Now,     // immediately
        Beat,    // on the next beat
        Bar,     // on the next bar
        Queue    // when the current chunk finishes its loop
    }

    internal static class Native
    {
        const string Dll = "Melowrite.Core";

        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_host_init(int sample_rate);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_host_sample_rate();
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_host_build([Out] byte[] buffer, int size);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_host_fill_buffer([Out] float[] buffer, int frames);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_host_tick();
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_host_shutdown();
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_host_set_path_resolver(IntPtr callback, IntPtr user_data);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_host_set_path_resolver_utf8(IntPtr callback, IntPtr user_data);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_host_set_log_error(IntPtr callback, IntPtr user_data);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_host_set_log_warning(IntPtr callback, IntPtr user_data);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr melo_load_instance([MarshalAs(UnmanagedType.LPUTF8Str)] string path);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_load_async([MarshalAs(UnmanagedType.LPUTF8Str)] string path, IntPtr callback, IntPtr user_data);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern float melo_get_volume();
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_set_volume(float value);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_stop_all();
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_unload_all();
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr melo_load_mix([MarshalAs(UnmanagedType.LPUTF8Str)] string path);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_unload_mix();
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_on_note_add(IntPtr callback, IntPtr user_data);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_on_note_remove(IntPtr callback, IntPtr user_data);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_on_beat_add(IntPtr callback, IntPtr user_data);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_on_beat_remove(IntPtr callback, IntPtr user_data);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_on_bar_add(IntPtr callback, IntPtr user_data);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_on_bar_remove(IntPtr callback, IntPtr user_data);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_play_one_shot([MarshalAs(UnmanagedType.LPUTF8Str)] string file, float volume, float pan, float pitch, int loop, int effects);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_play_loop([MarshalAs(UnmanagedType.LPUTF8Str)] string file, float volume, float pan, float pitch, int effects);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_global_voice_is_playing(int voice);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_global_stop_voice(int voice);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_global_stop_sfx();
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_global_voice_volume(int voice, float value);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_global_voice_pan(int voice, float value);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_global_voice_pitch(int voice, float value);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern float melo_get_sfx_volume();
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_set_sfx_volume(float value);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_set_reverb(float mix, float room_size, float damping);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_set_delay(float mix, float interval_ms, float decay_ms);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_clear_effects();
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_copy_effects_from(IntPtr instance, [MarshalAs(UnmanagedType.LPUTF8Str)] string bus, float wet_mix);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_clip_is_loaded(IntPtr instance, int @global, [MarshalAs(UnmanagedType.LPUTF8Str)] string key);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_clip_load_pcm(IntPtr instance, int @global, [MarshalAs(UnmanagedType.LPUTF8Str)] string key, [In] float[] left, [In] float[] right, int frames, int sample_rate);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_clip_play(IntPtr instance, int @global, [MarshalAs(UnmanagedType.LPUTF8Str)] string key, float volume, float pan, float pitch, int loop, int effects);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr melo_metadata_open([MarshalAs(UnmanagedType.LPUTF8Str)] string path);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_metadata_close(IntPtr metadata);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_metadata_number(IntPtr metadata, int field);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_metadata_text(IntPtr metadata, int field, int index, [Out] byte[] buffer, int size);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_play_chunk(IntPtr instance, [MarshalAs(UnmanagedType.LPUTF8Str)] string chunk, int looping, float fade_out);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_play_chunk_crossfade(IntPtr instance, [MarshalAs(UnmanagedType.LPUTF8Str)] string chunk, float duration, int looping);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_play_chunk_headless(IntPtr instance, [MarshalAs(UnmanagedType.LPUTF8Str)] string chunk);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_switch_chunk(IntPtr instance, [MarshalAs(UnmanagedType.LPUTF8Str)] string chunk, int when);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_play_chunk_index(IntPtr instance, int chunk, int looping, float fade_out);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_play_chunk_crossfade_index(IntPtr instance, int chunk, float duration, int looping);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_play_chunk_headless_index(IntPtr instance, int chunk);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_switch_chunk_index(IntPtr instance, int chunk, int when);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_play_arrangement(IntPtr instance, float fade_out);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_play_arrangement_crossfade(IntPtr instance, float duration);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_stop(IntPtr instance);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_pause(IntPtr instance);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_resume(IntPtr instance);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_fade_out(IntPtr instance, float duration);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_cancel_switch(IntPtr instance);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_seek_bar(IntPtr instance, int bar);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_set_tempo(IntPtr instance, int bpm);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_set_master_volume(IntPtr instance, float volume);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_set_pan(IntPtr instance, float pan);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_swap_palette(IntPtr instance, [MarshalAs(UnmanagedType.LPUTF8Str)] string palette);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_switch_song(IntPtr instance, [MarshalAs(UnmanagedType.LPUTF8Str)] string target, int when, float fade_out);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_switch_song_at_name(IntPtr instance, [MarshalAs(UnmanagedType.LPUTF8Str)] string target, [MarshalAs(UnmanagedType.LPUTF8Str)] string start_chunk, int when, float fade_out);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_switch_song_at_index(IntPtr instance, [MarshalAs(UnmanagedType.LPUTF8Str)] string target, int start_chunk, int when, float fade_out);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_switch_song_loaded(IntPtr instance, IntPtr target, int when, float fade_out);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_switch_song_loaded_at_name(IntPtr instance, IntPtr target, [MarshalAs(UnmanagedType.LPUTF8Str)] string start_chunk, int when, float fade_out);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_switch_song_loaded_at_index(IntPtr instance, IntPtr target, int start_chunk, int when, float fade_out);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_switch_song_crossfade(IntPtr instance, [MarshalAs(UnmanagedType.LPUTF8Str)] string target, float duration, int when);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_switch_song_crossfade_at_name(IntPtr instance, [MarshalAs(UnmanagedType.LPUTF8Str)] string target, [MarshalAs(UnmanagedType.LPUTF8Str)] string start_chunk, float duration, int when);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_switch_song_crossfade_at_index(IntPtr instance, [MarshalAs(UnmanagedType.LPUTF8Str)] string target, int start_chunk, float duration, int when);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_switch_song_crossfade_loaded(IntPtr instance, IntPtr target, float duration, int when);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_switch_song_crossfade_loaded_at_name(IntPtr instance, IntPtr target, [MarshalAs(UnmanagedType.LPUTF8Str)] string start_chunk, float duration, int when);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_switch_song_crossfade_loaded_at_index(IntPtr instance, IntPtr target, int start_chunk, float duration, int when);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_set_track_muted(IntPtr instance, [MarshalAs(UnmanagedType.LPUTF8Str)] string track, int value);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_set_track_muted_index(IntPtr instance, int track, int value);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_set_track_volume(IntPtr instance, [MarshalAs(UnmanagedType.LPUTF8Str)] string track, float value);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_set_track_volume_index(IntPtr instance, int track, float value);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_set_send_a(IntPtr instance, [MarshalAs(UnmanagedType.LPUTF8Str)] string track, float value);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_set_send_a_index(IntPtr instance, int track, float value);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_set_send_b(IntPtr instance, [MarshalAs(UnmanagedType.LPUTF8Str)] string track, float value);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_set_send_b_index(IntPtr instance, int track, float value);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_set_bus_volume(IntPtr instance, int bus, float volume);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_set_bus_effect_param(IntPtr instance, int owner, int effect, [MarshalAs(UnmanagedType.LPUTF8Str)] string param, float value);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_set_track_effect_param(IntPtr instance, int owner, int effect, [MarshalAs(UnmanagedType.LPUTF8Str)] string param, float value);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_trigger_pad(IntPtr instance, [MarshalAs(UnmanagedType.LPUTF8Str)] string track, [MarshalAs(UnmanagedType.LPUTF8Str)] string pad, float velocity, float pan);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_trigger_pad_index(IntPtr instance, int track, [MarshalAs(UnmanagedType.LPUTF8Str)] string pad, float velocity, float pan);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_hold_pad(IntPtr instance, [MarshalAs(UnmanagedType.LPUTF8Str)] string track, [MarshalAs(UnmanagedType.LPUTF8Str)] string pad, float velocity, float pan);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_hold_pad_index(IntPtr instance, int track, [MarshalAs(UnmanagedType.LPUTF8Str)] string pad, float velocity, float pan);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_sustain_pad(IntPtr instance, [MarshalAs(UnmanagedType.LPUTF8Str)] string track, [MarshalAs(UnmanagedType.LPUTF8Str)] string pad, float velocity, float pan);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_sustain_pad_index(IntPtr instance, int track, [MarshalAs(UnmanagedType.LPUTF8Str)] string pad, float velocity, float pan);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_release_pad(IntPtr instance, [MarshalAs(UnmanagedType.LPUTF8Str)] string track, [MarshalAs(UnmanagedType.LPUTF8Str)] string pad);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_note_on(IntPtr instance, int track, int midi_note, float velocity);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_note_off(IntPtr instance, int track, int midi_note);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_instance_play_one_shot(IntPtr instance, [MarshalAs(UnmanagedType.LPUTF8Str)] string file, float volume, float pan, float pitch, int loop, int effects);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_instance_play_loop(IntPtr instance, [MarshalAs(UnmanagedType.LPUTF8Str)] string file, float volume, float pan, float pitch, int effects);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_stop_voice(IntPtr instance, int voice);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_stop_sfx(IntPtr instance);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_voice_volume(IntPtr instance, int voice, float value);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_voice_pan(IntPtr instance, int voice, float value);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_voice_pitch(IntPtr instance, int voice, float value);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_unload(IntPtr instance);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr melo_instance_load_mix(IntPtr instance, [MarshalAs(UnmanagedType.LPUTF8Str)] string path);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_unload_mix(IntPtr instance);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_run_on_audio_thread(IntPtr instance, IntPtr callback, IntPtr user_data);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_run_at(IntPtr instance, int when, IntPtr callback, IntPtr user_data);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_run_on_audio_thread_at(IntPtr instance, int when, IntPtr callback, IntPtr user_data);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_instance_cancel_run_at(IntPtr instance);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr melo_instance_engine(IntPtr instance);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_instance_is_loaded(IntPtr instance);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_instance_current_bar(IntPtr instance);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_instance_current_beat(IntPtr instance);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_instance_tempo(IntPtr instance);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_instance_chunk_count(IntPtr instance);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_instance_track_count(IntPtr instance);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_instance_file(IntPtr instance, [Out] byte[] buffer, int size);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_instance_active_chunk(IntPtr instance, [Out] byte[] buffer, int size);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_instance_chunk_name(IntPtr instance, int index, [Out] byte[] buffer, int size);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_instance_track_name(IntPtr instance, int index, [Out] byte[] buffer, int size);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_instance_bus_count(IntPtr instance);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_instance_bus_name(IntPtr instance, int index, [Out] byte[] buffer, int size);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_instance_palette_count(IntPtr instance);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_instance_palette_name(IntPtr instance, int index, [Out] byte[] buffer, int size);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_instance_track_index(IntPtr instance, [MarshalAs(UnmanagedType.LPUTF8Str)] string name);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_instance_bus_index(IntPtr instance, [MarshalAs(UnmanagedType.LPUTF8Str)] string name);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_instance_set_game_param(IntPtr instance, [MarshalAs(UnmanagedType.LPUTF8Str)] string name, float value);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_instance_release_game_param(IntPtr instance, [MarshalAs(UnmanagedType.LPUTF8Str)] string name);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern float melo_instance_get_game_param(IntPtr instance, [MarshalAs(UnmanagedType.LPUTF8Str)] string name);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_instance_game_param_count(IntPtr instance);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_instance_game_param_name(IntPtr instance, int index, [Out] byte[] buffer, int size);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern float melo_instance_game_param_default(IntPtr instance, int index);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_instance_master_bus_index(IntPtr instance);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_owner_is_valid(IntPtr instance, int owner, int bus);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_owner_name(IntPtr instance, int owner, int bus, [Out] byte[] buffer, int size);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern float melo_owner_get_volume(IntPtr instance, int owner, int bus);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_owner_set_volume(IntPtr instance, int owner, int bus, float value);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern float melo_owner_get_pan(IntPtr instance, int owner, int bus);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_owner_set_pan(IntPtr instance, int owner, int bus, float value);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_owner_get_muted(IntPtr instance, int owner, int bus);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_owner_set_muted(IntPtr instance, int owner, int bus, int value);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_owner_get_solo(IntPtr instance, int owner, int bus);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_owner_set_solo(IntPtr instance, int owner, int bus, int value);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern float melo_owner_get_senda(IntPtr instance, int owner, int bus);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_owner_set_senda(IntPtr instance, int owner, int bus, float value);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern float melo_owner_get_sendb(IntPtr instance, int owner, int bus);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_owner_set_sendb(IntPtr instance, int owner, int bus, float value);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_owner_effect_count(IntPtr instance, int owner, int bus);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_owner_set_param(IntPtr instance, int owner, int bus, [MarshalAs(UnmanagedType.LPUTF8Str)] string path, float value);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern float melo_owner_get_param(IntPtr instance, int owner, int bus, [MarshalAs(UnmanagedType.LPUTF8Str)] string path);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_owner_param_count(IntPtr instance, int owner, int bus);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_owner_param_path(IntPtr instance, int owner, int bus, int index, [Out] byte[] buffer, int size);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_owner_param_range(IntPtr instance, int owner, int bus, int index, [Out] float[] min, [Out] float[] max, [Out] float[] default_value);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_owner_find_effect(IntPtr instance, int owner, int bus, [MarshalAs(UnmanagedType.LPUTF8Str)] string type);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_owner_add_effect(IntPtr instance, int owner, int bus, [MarshalAs(UnmanagedType.LPUTF8Str)] string type);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_owner_remove_effect(IntPtr instance, int owner, int bus, [MarshalAs(UnmanagedType.LPUTF8Str)] string type);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_track_instrument_json(IntPtr instance, int track, [Out] byte[] buffer, int size);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_track_set_instrument_json(IntPtr instance, int track, [MarshalAs(UnmanagedType.LPUTF8Str)] string json);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_effect_is_valid(IntPtr instance, int owner, int bus, int effect);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_effect_name(IntPtr instance, int owner, int bus, int effect, [Out] byte[] buffer, int size);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_effect_type(IntPtr instance, int owner, int bus, int effect, [Out] byte[] buffer, int size);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_effect_get_enabled(IntPtr instance, int owner, int bus, int effect);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_effect_set_enabled(IntPtr instance, int owner, int bus, int effect, int value);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void melo_effect_reset(IntPtr instance, int owner, int bus, int effect);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern float melo_effect_get_param(IntPtr instance, int owner, int bus, int effect, [MarshalAs(UnmanagedType.LPUTF8Str)] string param);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_effect_set_param(IntPtr instance, int owner, int bus, int effect, [MarshalAs(UnmanagedType.LPUTF8Str)] string param, float value);
        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int melo_effect_ir_name(IntPtr instance, int owner, int bus, int effect, [Out] byte[] buffer, int size);
        // asks for the size then reads. reads again if the text grew in between, like a chunk switching
        internal static string Text(Func<byte[], int, int> read)
        {
            int size = read(null, 0);
            while (size > 0)
            {
                byte[] buffer = new byte[size + 1];
                int actual = read(buffer, buffer.Length);
                if (actual <= size) return Encoding.UTF8.GetString(buffer, 0, actual);
                size = actual;
            }
            return "";
        }
        internal static string Utf8(IntPtr text) => text == IntPtr.Zero ? null : Marshal.PtrToStringUTF8(text);
    }

    internal static class Callbacks
    {
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)] internal delegate void Completion(IntPtr instance, IntPtr context);
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)] internal delegate void Note(IntPtr instance, int track, int pitch, int velocity, IntPtr context);
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)] internal delegate void Beat(IntPtr instance, int bar, int beat, IntPtr context);
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)] internal delegate void Bar(IntPtr instance, int bar, IntPtr context);
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)] internal delegate void Log(IntPtr text, IntPtr context);
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)] internal delegate int Resolve(IntPtr path, IntPtr buffer, int size, IntPtr context);
        private static long nextKey;
        private static readonly ConcurrentDictionary<IntPtr, Action<IntPtr>> pending = new ConcurrentDictionary<IntPtr, Action<IntPtr>>();
        private static readonly ConcurrentQueue<Exception> failures = new ConcurrentQueue<Exception>();
        private static readonly Completion completeCallback = Complete;
        internal static readonly IntPtr CompletionPointer = Marshal.GetFunctionPointerForDelegate(completeCallback);
        internal static IntPtr Add(Action<IntPtr> action)
        {
            IntPtr key = new IntPtr(Interlocked.Increment(ref nextKey));
            pending[key] = action;
            return key;
        }
        internal static void Cancel(IntPtr key) { pending.TryRemove(key, out _); }
        internal static void Record(Exception error) { failures.Enqueue(error); }
#if UNITY_5_3_OR_NEWER
        [AOT.MonoPInvokeCallback(typeof(Completion))]
#endif
        private static void Complete(IntPtr instance, IntPtr context)
        {
            if (!pending.TryRemove(context, out Action<IntPtr> action)) return;
            try
            {
                action(instance);
            }
            catch (Exception failure) { Record(failure); }
        }
        internal static void Check()
        {
            List<Exception> errors = new List<Exception>();
            while (failures.TryDequeue(out Exception error)) errors.Add(error);
            if (errors.Count != 0) throw new AggregateException("A middleware callback failed.", errors);
        }
        internal static void Clear()
        {
            pending.Clear();
            while (failures.TryDequeue(out _)) { }
        }
    }

    /// <summary>hooks for whatever owns the audio device. all the processing happens in the native dll</summary>
    public static class MeloHost
    {
        private static Func<string, string> resolver = path => path;
        private static Action<string> logError = Console.Error.WriteLine, logWarning = Console.WriteLine;
        private static readonly Callbacks.Resolve resolveCallback = Resolve;
        private static readonly Callbacks.Log errorCallback = Error, warningCallback = Warning;
        private static bool configured;
        /// <summary>maps a content path to a file on disk. passes it through by default</summary>
        public static Func<string, string> ResolvePath
        {
            get => resolver;
            set
            {
                resolver = value;
                if (resolver == null) resolver = path => path;
            }
        }
        /// <summary>engine errors write here, Console.Error by default</summary>
        public static Action<string> LogError { get => logError; set => logError = value; }
        /// <summary>engine warnings write here, Console by default</summary>
        public static Action<string> LogWarning { get => logWarning; set => logWarning = value; }
        /// <summary>engine build string</summary>
        public static string Build => Native.Text(Native.melo_host_build);
        /// <summary>output sample rate</summary>
        public static int SampleRate => Native.melo_host_sample_rate();
        internal static void Configure()
        {
            if (configured) return;
            Native.melo_host_set_path_resolver_utf8(Marshal.GetFunctionPointerForDelegate(resolveCallback), IntPtr.Zero);
            Native.melo_host_set_log_error(Marshal.GetFunctionPointerForDelegate(errorCallback), IntPtr.Zero);
            Native.melo_host_set_log_warning(Marshal.GetFunctionPointerForDelegate(warningCallback), IntPtr.Zero);
            configured = true;
        }
        /// <summary>call before loading any projects or playing any audio. sampleRate has to match the audio device (bad values fall back to 44100). to change it later stop audio and Shutdown first</summary>
        public static void Init(int sampleRate) { Native.melo_host_init(sampleRate); Configure(); }
        /// <summary>audio thread. fills frames of interleaved stereo floats, handled by Unity plugin, MonoGame host and SDL host</summary>
        public static void FillBuffer(float[] buffer, int frames)
        {
            if (buffer == null) throw new ArgumentNullException(nameof(buffer));
            if (frames < 0 || frames > buffer.Length / 2) throw new ArgumentOutOfRangeException(nameof(frames));
            Native.melo_host_fill_buffer(buffer, frames);
        }
        /// <summary>call once a frame. fires load callbacks, events and RunAt callbacks</summary>
        public static void Tick() { Native.melo_host_tick(); Callbacks.Check(); }
        /// <summary>frees the engine. stop the audio device first</summary>
        public static void Shutdown()
        {
            Native.melo_host_shutdown();
            Melo.ResetBindings();
            Callbacks.Clear();
            configured = false;
            resolver = path => path;
            logError = Console.Error.WriteLine;
            logWarning = Console.WriteLine;
        }
#if UNITY_5_3_OR_NEWER
        [AOT.MonoPInvokeCallback(typeof(Callbacks.Resolve))]
#endif
        private static int Resolve(IntPtr path, IntPtr buffer, int size, IntPtr context)
        {
            try
            {
                string value = resolver(Native.Utf8(path));
                if (value == null) return -1;
                byte[] bytes = Encoding.UTF8.GetBytes(value);
                if (buffer != IntPtr.Zero && size > bytes.Length)
                {
                    Marshal.Copy(bytes, 0, buffer, bytes.Length);
                    Marshal.WriteByte(buffer, bytes.Length, 0);
                }
                return bytes.Length;
            }
            catch (Exception failure)
            {
                Callbacks.Record(failure);
                return -1;
            }
        }
#if UNITY_5_3_OR_NEWER
        [AOT.MonoPInvokeCallback(typeof(Callbacks.Log))]
#endif
        private static void Error(IntPtr text, IntPtr context)
        {
            try
            {
                if (logError != null) logError(Native.Utf8(text));
            }
            catch (Exception failure) { Callbacks.Record(failure); }
        }
#if UNITY_5_3_OR_NEWER
        [AOT.MonoPInvokeCallback(typeof(Callbacks.Log))]
#endif
        private static void Warning(IntPtr text, IntPtr context)
        {
            try
            {
                if (logWarning != null) logWarning(Native.Utf8(text));
            }
            catch (Exception failure) { Callbacks.Record(failure); }
        }
    }

    /// <summary>loads songs and plays sfx, start here</summary>
    public static partial class Melo
    {
        private static readonly Callbacks.Note noteCallback = Note;
        private static readonly Callbacks.Beat beatCallback = Beat;
        private static readonly Callbacks.Bar barCallback = Bar;
        private static Action<MeloInstance, int, int, int> notes;
        private static Action<MeloInstance, int, int> beats;
        private static Action<MeloInstance, int> bars;
        /// <summary>load an exported .melo</summary>
        public static MeloInstance Load(string projectPath)
        {
            if (projectPath == null) throw new ArgumentNullException(nameof(projectPath));
            MeloHost.Configure();
            MeloInstance instance = MeloInstance.Wrap(Native.melo_load_instance(projectPath));
            if (instance == null) throw new InvalidOperationException("Couldn't load " + projectPath);
            return instance;
        }
        /// <summary>load on a background thread. the callback comes through MeloHost.Tick</summary>
        public static void LoadAsync(string projectPath, Action<MeloInstance> onLoaded = null)
        {
            if (projectPath == null) throw new ArgumentNullException(nameof(projectPath));
            MeloHost.Configure();
            IntPtr context = onLoaded == null ? IntPtr.Zero : Callbacks.Add(handle => onLoaded(MeloInstance.Wrap(handle)));
            try { Native.melo_load_async(projectPath, onLoaded == null ? IntPtr.Zero : Callbacks.CompletionPointer, context); }
            catch { Callbacks.Cancel(context); throw; }
        }
        /// <summary>master volume 0-1, music and sfx</summary>
        public static float MasterVolume { get => Native.melo_get_volume(); set => Native.melo_set_volume(value); }
        /// <summary>unloads everything and clears the sound caches</summary>
        public static void UnloadAll() { Native.melo_unload_all(); MeloInstance.Clear(); }
        /// <summary>master mix project all audio routes through, one-shots and raw audio as well. stays until UnloadMix</summary>
        public static MeloMix LoadMix(string projectPath)
        {
            if (projectPath == null) throw new ArgumentNullException(nameof(projectPath));
            MeloHost.Configure();
            MeloMix mix = MeloMix.Wrap(Native.melo_load_mix(projectPath));
            if (mix == null) throw new InvalidOperationException("Couldn't load mix " + projectPath);
            return mix;
        }
        /// <summary>removes global mix</summary>
        public static void UnloadMix() => Native.melo_unload_mix();
        /// <summary>every note played: instance, track index, midi pitch, velocity 0-127. fires from MeloHost.Tick</summary>
        public static event Action<MeloInstance, int, int, int> OnNote
        {
            add
            {
                if (value == null) return;
                MeloHost.Configure();
                if (notes == null) Native.melo_on_note_add(Marshal.GetFunctionPointerForDelegate(noteCallback), IntPtr.Zero);
                notes += value;
            }
            remove
            {
                if (notes == null) return;
                notes -= value;
                if (notes == null) Native.melo_on_note_remove(Marshal.GetFunctionPointerForDelegate(noteCallback), IntPtr.Zero);
            }
        }
#if UNITY_5_3_OR_NEWER
        [AOT.MonoPInvokeCallback(typeof(Callbacks.Note))]
#endif
        private static void Note(IntPtr instance, int track, int pitch, int velocity, IntPtr context)
        {
            try
            {
                if (notes != null) notes(MeloInstance.Wrap(instance), track, pitch, velocity);
            }
            catch (Exception failure) { Callbacks.Record(failure); }
        }
        /// <summary>every beat: instance, bar, beat in the bar, both from 0. fires from MeloHost.Tick</summary>
        public static event Action<MeloInstance, int, int> OnBeat
        {
            add
            {
                if (value == null) return;
                MeloHost.Configure();
                if (beats == null) Native.melo_on_beat_add(Marshal.GetFunctionPointerForDelegate(beatCallback), IntPtr.Zero);
                beats += value;
            }
            remove
            {
                if (beats == null) return;
                beats -= value;
                if (beats == null) Native.melo_on_beat_remove(Marshal.GetFunctionPointerForDelegate(beatCallback), IntPtr.Zero);
            }
        }
#if UNITY_5_3_OR_NEWER
        [AOT.MonoPInvokeCallback(typeof(Callbacks.Beat))]
#endif
        private static void Beat(IntPtr instance, int bar, int beat, IntPtr context)
        {
            try
            {
                if (beats != null) beats(MeloInstance.Wrap(instance), bar, beat);
            }
            catch (Exception failure) { Callbacks.Record(failure); }
        }
        /// <summary>every bar: instance, bar from 0. fires from MeloHost.Tick</summary>
        public static event Action<MeloInstance, int> OnBar
        {
            add
            {
                if (value == null) return;
                MeloHost.Configure();
                if (bars == null) Native.melo_on_bar_add(Marshal.GetFunctionPointerForDelegate(barCallback), IntPtr.Zero);
                bars += value;
            }
            remove
            {
                if (bars == null) return;
                bars -= value;
                if (bars == null) Native.melo_on_bar_remove(Marshal.GetFunctionPointerForDelegate(barCallback), IntPtr.Zero);
            }
        }
#if UNITY_5_3_OR_NEWER
        [AOT.MonoPInvokeCallback(typeof(Callbacks.Bar))]
#endif
        private static void Bar(IntPtr instance, int bar, IntPtr context)
        {
            try
            {
                if (bars != null) bars(MeloInstance.Wrap(instance), bar);
            }
            catch (Exception failure) { Callbacks.Record(failure); }
        }
        internal static void ResetBindings()
        {
            notes = null;
            beats = null;
            bars = null;
            MeloInstance.Clear();
            ClearPlatformState();
        }
        static partial void ClearPlatformState();
        internal static bool ClipIsLoaded(IntPtr instance, string key)
            => Native.melo_clip_is_loaded(instance, instance == IntPtr.Zero ? 1 : 0, key) != 0;
        internal static bool LoadClipPcm(IntPtr instance, string key, float[] left, float[] right, int sampleRate)
        {
            if (left == null || right == null || left.Length != right.Length || left.Length < 2 || sampleRate <= 0)
                throw new ArgumentException("PCM channels must have matching lengths and a positive sample rate.");
            return Native.melo_clip_load_pcm(instance, instance == IntPtr.Zero ? 1 : 0, key, left, right, left.Length, sampleRate) != 0;
        }
        internal static int PlayClip(IntPtr instance, string key, float volume, float pan, float pitch, bool loop, bool effects)
            => Native.melo_clip_play(instance, instance == IntPtr.Zero ? 1 : 0, key, volume, pan, pitch, loop ? 1 : 0, effects ? 1 : 0);
        /// <summary>stops all music. pads stay loaded</summary>
        public static void StopAll() => Native.melo_stop_all();

        // -- SFX --
        /// <summary>sfx volume 0-1, on top of MasterVolume</summary>
        public static float SfxVolume { get => Native.melo_get_sfx_volume(); set => Native.melo_set_sfx_volume(value); }
        /// <summary>play an audio file, returns a voice id. pitch is a rate multiplier, 2 = up an octave</summary>
        public static int PlayOneShot(string file, float volume = 1f, float pan = 0f, float pitch = 1f, bool loop = false, bool effects = true) => Native.melo_play_one_shot(file, volume, pan, pitch, loop ? 1 : 0, effects ? 1 : 0);
        /// <summary>loops until StopSfxVoice(voice)</summary>
        public static int PlayLoop(string file, float volume = 1f, float pan = 0f, float pitch = 1f, bool effects = true) => Native.melo_play_loop(file, volume, pan, pitch, effects ? 1 : 0);
        /// <summary>true while the voice or its tail is still going</summary>
        public static bool IsPlaying(int voice) => Native.melo_global_voice_is_playing(voice) != 0;
        /// <summary>stop sound started with PlayOneShot or PlayLoop, both return ints for use with this call.</summary>
        public static void StopSfxVoice(int voice) => Native.melo_global_stop_voice(voice);
        /// <summary>stops all loops and one shots, music is unaffected</summary>
        public static void StopSfx() => Native.melo_global_stop_sfx();
        public static void SetVolume(int voice, float v) => Native.melo_global_voice_volume(voice, v);
        public static void SetPan(int voice, float p) => Native.melo_global_voice_pan(voice, p);
        public static void SetPitch(int voice, float s) => Native.melo_global_voice_pitch(voice, s);

        // -- Effects --
        public static void SetReverb(float mix = 0.3f, float roomSize = 0.7f, float damping = 0.5f) => Native.melo_set_reverb(mix, roomSize, damping);
        public static void SetDelay(float mix = 0.3f, float intervalMs = 350f, float decayMs = 1500f) => Native.melo_set_delay(mix, intervalMs, decayMs);
        public static void ClearEffects() => Native.melo_clear_effects();
        /// <summary>copy a project bus's effects onto the sfx bus. wetMix 0-1 overrides every effect's mix, -1 leaves them</summary>
        public static void CopyEffectsFrom(MeloInstance instance, string busName, float wetMix = -1f) => Native.melo_copy_effects_from(instance == null ? IntPtr.Zero : instance.NativeHandle, busName, wetMix);
    }

    /// <summary>a loaded project, from Melo.Load</summary>
    public sealed partial class MeloInstance
    {
        private static readonly ConcurrentDictionary<IntPtr, MeloInstance> instances = new ConcurrentDictionary<IntPtr, MeloInstance>();
        private IntPtr handle;
        private MeloInstance(IntPtr handle) { this.handle = handle; }
        internal static MeloInstance Wrap(IntPtr handle) => handle == IntPtr.Zero ? null : instances.GetOrAdd(handle, key => new MeloInstance(key));
        internal static void Clear() { instances.Clear(); }
        public IntPtr NativeHandle => handle;
        /// <summary>native engine handle, zero after Unload</summary>
        public IntPtr Engine => Native.melo_instance_engine(handle);
        public bool IsLoaded => Native.melo_instance_is_loaded(handle) != 0;
        internal string FilePath => Native.Text((buffer, size) => Native.melo_instance_file(handle, buffer, size));
#if !UNITY_5_3_OR_NEWER
        public string File => FilePath;
#endif
        public string ActiveChunk => Native.Text((buffer, size) => Native.melo_instance_active_chunk(handle, buffer, size));
        public int CurrentBar => Native.melo_instance_current_bar(handle);
        public int CurrentBeat => Native.melo_instance_current_beat(handle);
        public int Tempo => Native.melo_instance_tempo(handle);
        public int ChunkCount => Native.melo_instance_chunk_count(handle);
        public int TrackCount => Native.melo_instance_track_count(handle);
        private string[] Names(int count, Func<int, byte[], int, int> read)
        {
            string[] values = new string[count];
            for (int i = 0; i < count; i++) { int index = i; values[i] = Native.Text((buffer, size) => read(index, buffer, size)); }
            return values;
        }
        /// <summary>chunk names</summary>
        public string[] ChunkNames => Names(ChunkCount, (index, buffer, size) => Native.melo_instance_chunk_name(handle, index, buffer, size));
        public string[] TrackNames => Names(TrackCount, (index, buffer, size) => Native.melo_instance_track_name(handle, index, buffer, size));
        public string[] GameParamNames => Names(Native.melo_instance_game_param_count(handle), (index, buffer, size) => Native.melo_instance_game_param_name(handle, index, buffer, size));
        public string[] BusNames => Names(Native.melo_instance_bus_count(handle), (index, buffer, size) => Native.melo_instance_bus_name(handle, index, buffer, size));
        public string[] GetPaletteNames() => Names(Native.melo_instance_palette_count(handle), (index, buffer, size) => Native.melo_instance_palette_name(handle, index, buffer, size));
        /// <summary>by name, check IsValid</summary>
        public MeloTrack Track(string name) => Track(Native.melo_instance_track_index(handle, name));
        /// <summary>by index, check IsValid</summary>
        public MeloTrack Track(int index) => new MeloTrack(handle, index, 0);
        /// <summary>bus by name, check IsValid</summary>
        public MeloBus Bus(string name) => Bus(Native.melo_instance_bus_index(handle, name));
        /// <summary>bus by index, 0 is master</summary>
        public MeloBus Bus(int index) => new MeloBus(handle, index);
        /// <summary>the master bus can also be found with Bus(0)</summary>
        public MeloBus Master => Bus(Native.melo_instance_master_bus_index(handle));
        public IEnumerable<MeloTrack> AllTracks() { for (int i = 0; i < TrackCount; i++) yield return Track(i); }
        public IEnumerable<MeloBus> AllBuses() { for (int i = 0; i < Native.melo_instance_bus_count(handle); i++) yield return Bus(i); }
        /// <summary>queue a command for the audio thread</summary>
        public void RunOnAudioThread(Action<IntPtr> command)
        {
            if (command == null) throw new ArgumentNullException(nameof(command));
            if (!IsLoaded) return;
            IntPtr context = Callbacks.Add(command);
            try { Native.melo_instance_run_on_audio_thread(handle, Callbacks.CompletionPointer, context); }
            catch { Callbacks.Cancel(context); throw; }
        }
        /// <summary>run code on the main thread at the next beat, bar or loop. fires from MeloHost.Tick</summary>
        public void RunAt(MeloSwitch when, Action action)
            => Schedule(action, (callback, context) => Native.melo_instance_run_at(handle, (int)when, callback, context));
        /// <summary>audio thread version, at the next beat, bar or loop. keep it quick</summary>
        public void RunOnAudioThreadAt(MeloSwitch when, Action<IntPtr> command)
            => Schedule(command, (callback, context) => Native.melo_instance_run_on_audio_thread_at(handle, (int)when, callback, context));
        /// <summary>drop any RunAt and RunOnAudioThreadAt still waiting. switches already queued stay</summary>
        public void CancelRunAt() => Native.melo_instance_cancel_run_at(handle);
        private void Schedule(Action action, Action<IntPtr, IntPtr> native)
        {
            if (action == null) throw new ArgumentNullException(nameof(action));
            Schedule(engine => action(), native);
        }
        // the native side calls back once with the context, Callbacks.Complete runs it and forgets it
        private void Schedule(Action<IntPtr> action, Action<IntPtr, IntPtr> native)
        {
            if (action == null) throw new ArgumentNullException(nameof(action));
            if (!IsLoaded) return;
            IntPtr context = Callbacks.Add(action);
            try { native(Callbacks.CompletionPointer, context); }
            catch { Callbacks.Cancel(context); throw; }
        }
        /// <summary>unload this song. Melo.UnloadAll also clears the shared caches</summary>
        public void Unload()
        {
            IntPtr old = handle;
            Native.melo_instance_unload(old);
            instances.TryRemove(old, out _);
            handle = IntPtr.Zero;
        }
        /// <summary>instance-specific second global mix project, with parameters and all that, the loaded song passes through. stays until UnloadMix</summary>
        public MeloMix LoadMix(string projectPath)
        {
            if (projectPath == null) throw new ArgumentNullException(nameof(projectPath));
            MeloMix mix = MeloMix.Wrap(Native.melo_instance_load_mix(handle, projectPath));
            if (mix == null) throw new InvalidOperationException("Couldn't load mix " + projectPath);
            return mix;
        }
        /// <summary>removes the instance's second global mix project</summary>
        public void UnloadMix() => Native.melo_instance_unload_mix(handle);
        /// <summary>play a chunk, null = chunk 0. fadeOut in seconds</summary>
        public void PlayChunk(string chunk = null, bool looping = true, float fadeOut = 0f) => Native.melo_instance_play_chunk(handle, chunk, looping ? 1 : 0, fadeOut);
        /// <summary>crossfade to a chunk over duration seconds</summary>
        public void PlayChunkCrossfade(string chunk, float duration, bool looping = true) => Native.melo_instance_play_chunk_crossfade(handle, chunk, duration, looping ? 1 : 0);
        /// <summary>play a chunk once on top of the current music. cleans itself up when done</summary>
        public void PlayChunkHeadless(string chunk = null) => Native.melo_instance_play_chunk_headless(handle, chunk);
        /// <summary>switch chunks now, on the next beat, next bar or when the loop ends</summary>
        public void SwitchChunk(string chunk, MeloSwitch when = MeloSwitch.Bar) => Native.melo_instance_switch_chunk(handle, chunk, (int)when);
        public void PlayChunk(int index, bool looping = true, float fadeOut = 0f) => Native.melo_instance_play_chunk_index(handle, index, looping ? 1 : 0, fadeOut);
        public void PlayChunkCrossfade(int index, float duration, bool looping = true) => Native.melo_instance_play_chunk_crossfade_index(handle, index, duration, looping ? 1 : 0);
        public void PlayChunkHeadless(int index) => Native.melo_instance_play_chunk_headless_index(handle, index);
        public void SwitchChunk(int index, MeloSwitch when = MeloSwitch.Bar) => Native.melo_instance_switch_chunk_index(handle, index, (int)when);
        /// <summary>play the arrangement. fadeOut fades the current music first, in seconds</summary>
        public void PlayArrangement(float fadeOut = 0f) => Native.melo_instance_play_arrangement(handle, fadeOut);
        /// <summary>crossfade into the arrangement over duration seconds</summary>
        public void PlayArrangementCrossfade(float duration) => Native.melo_instance_play_arrangement_crossfade(handle, duration);
        /// <summary>stop, tails ring out. stays loaded</summary>
        public void Stop() => Native.melo_instance_stop(handle);
        public void Pause() => Native.melo_instance_pause(handle);
        public void Resume() => Native.melo_instance_resume(handle);
        /// <summary>fade out over duration seconds, 0 stops right away</summary>
        public void FadeOut(float duration = 1f) => Native.melo_instance_fade_out(handle, duration);
        /// <summary>cancel a switch that hasnt happened yet</summary>
        public void CancelSwitch() => Native.melo_instance_cancel_switch(handle);
        public void SeekToBar(int bar) => Native.melo_instance_seek_bar(handle, bar);
        public void SetTempo(int bpm) => Native.melo_instance_set_tempo(handle, bpm);
        public void SetMasterVolume(float volume) => Native.melo_instance_set_master_volume(handle, volume);
        /// <summary>-1 left, 0 center, 1 right</summary>
        public void SetPan(float pan) => Native.melo_instance_set_pan(handle, pan);
        public void SwapPalette(string palette) => Native.melo_instance_swap_palette(handle, palette);
        /// <summary>switch to another song. no start chunk = its arrangement. fadeOut in seconds</summary>
        public void SwitchSong(string projectPath, MeloSwitch when = MeloSwitch.Bar, float fadeOut = 0f) => Native.melo_instance_switch_song(handle, projectPath, (int)when, fadeOut);
        /// <summary>start on a named chunk</summary>
        public void SwitchSong(string projectPath, string startChunk, MeloSwitch when = MeloSwitch.Bar, float fadeOut = 0f) => Native.melo_instance_switch_song_at_name(handle, projectPath, startChunk, (int)when, fadeOut);
        /// <summary>start on a chunk index</summary>
        public void SwitchSong(string projectPath, int startChunk, MeloSwitch when = MeloSwitch.Bar, float fadeOut = 0f) => Native.melo_instance_switch_song_at_index(handle, projectPath, startChunk, (int)when, fadeOut);
        /// <summary>switch to a song you already loaded, skips the decode</summary>
        public void SwitchSong(MeloInstance other, MeloSwitch when = MeloSwitch.Bar, float fadeOut = 0f) => Native.melo_instance_switch_song_loaded(handle, other == null ? IntPtr.Zero : other.NativeHandle, (int)when, fadeOut);
        public void SwitchSong(MeloInstance other, string startChunk, MeloSwitch when = MeloSwitch.Bar, float fadeOut = 0f) => Native.melo_instance_switch_song_loaded_at_name(handle, other == null ? IntPtr.Zero : other.NativeHandle, startChunk, (int)when, fadeOut);
        public void SwitchSong(MeloInstance other, int startChunk, MeloSwitch when = MeloSwitch.Bar, float fadeOut = 0f) => Native.melo_instance_switch_song_loaded_at_index(handle, other == null ? IntPtr.Zero : other.NativeHandle, startChunk, (int)when, fadeOut);
        public void SwitchSongCrossfade(string projectPath, float duration, MeloSwitch when = MeloSwitch.Bar) => Native.melo_instance_switch_song_crossfade(handle, projectPath, duration, (int)when);
        public void SwitchSongCrossfade(string projectPath, string startChunk, float duration, MeloSwitch when = MeloSwitch.Bar) => Native.melo_instance_switch_song_crossfade_at_name(handle, projectPath, startChunk, duration, (int)when);
        public void SwitchSongCrossfade(string projectPath, int startChunk, float duration, MeloSwitch when = MeloSwitch.Bar) => Native.melo_instance_switch_song_crossfade_at_index(handle, projectPath, startChunk, duration, (int)when);
        public void SwitchSongCrossfade(MeloInstance other, float duration, MeloSwitch when = MeloSwitch.Bar) => Native.melo_instance_switch_song_crossfade_loaded(handle, other == null ? IntPtr.Zero : other.NativeHandle, duration, (int)when);
        public void SwitchSongCrossfade(MeloInstance other, string startChunk, float duration, MeloSwitch when = MeloSwitch.Bar) => Native.melo_instance_switch_song_crossfade_loaded_at_name(handle, other == null ? IntPtr.Zero : other.NativeHandle, startChunk, duration, (int)when);
        public void SwitchSongCrossfade(MeloInstance other, int startChunk, float duration, MeloSwitch when = MeloSwitch.Bar) => Native.melo_instance_switch_song_crossfade_loaded_at_index(handle, other == null ? IntPtr.Zero : other.NativeHandle, startChunk, duration, (int)when);
        public void SetTrackMuted(string track, bool m) => Native.melo_instance_set_track_muted(handle, track, m ? 1 : 0);
        public void SetTrackMuted(int track, bool m) => Native.melo_instance_set_track_muted_index(handle, track, m ? 1 : 0);
        public void SetTrackVolume(string track, float v) => Native.melo_instance_set_track_volume(handle, track, v);
        public void SetTrackVolume(int track, float v) => Native.melo_instance_set_track_volume_index(handle, track, v);
        /// <summary>send to bus A or B, 0-1</summary>
        public void SetSendA(string track, float level) => Native.melo_instance_set_send_a(handle, track, level);
        public void SetSendA(int track, float level) => Native.melo_instance_set_send_a_index(handle, track, level);
        public void SetSendB(string track, float level) => Native.melo_instance_set_send_b(handle, track, level);
        public void SetSendB(int track, float level) => Native.melo_instance_set_send_b_index(handle, track, level);
        public void SetBusVolume(int bus, float volume) => Native.melo_instance_set_bus_volume(handle, bus, volume);
        /// <summary>start a midi note, velocity 0-1</summary>
        public void NoteOn(int trackIndex, int midiNote, float velocity = 1f) => Native.melo_instance_note_on(handle, trackIndex, midiNote, velocity);
        /// <summary>stop a midi note</summary>
        public void NoteOff(int trackIndex, int midiNote) => Native.melo_instance_note_off(handle, trackIndex, midiNote);
        public bool SetGameParam(string name, float value) => Native.melo_instance_set_game_param(handle, name, value) != 0;
        public bool ReleaseGameParam(string name) => Native.melo_instance_release_game_param(handle, name) != 0;
        public float GetGameParam(string name) => Native.melo_instance_get_game_param(handle, name);

        // -- Effects --
        /// <summary>e.g. SetBusEffectParam(1, 0, "Mix", 0.5f)</summary>
        public void SetBusEffectParam(int bus, int effect, string param, float value) => Native.melo_instance_set_bus_effect_param(handle, bus, effect, param, value);
        /// <summary>same thing for a track effect</summary>
        public void SetTrackEffectParam(int track, int effect, string param, float value) => Native.melo_instance_set_track_effect_param(handle, track, effect, param, value);

        // -- Pads --
        /// <summary>hit a pad by name. velocity 0-1, pan -1 to 1. hits overlap</summary>
        public void TriggerPad(string track, string pad, float velocity = 1f, float pan = 0f) => Native.melo_instance_trigger_pad(handle, track, pad, velocity, pan);
        public void TriggerPad(int trackIndex, string pad, float velocity = 1f, float pan = 0f) => Native.melo_instance_trigger_pad_index(handle, trackIndex, pad, velocity, pan);
        /// <summary>hold a pad until ReleasePad</summary>
        public void HoldPad(string track, string pad, float velocity = 1f, float pan = 0f) => Native.melo_instance_hold_pad(handle, track, pad, velocity, pan);
        /// <summary>hold a pad until ReleasePad</summary>
        public void HoldPad(int track, string pad, float velocity = 1f, float pan = 0f) => Native.melo_instance_hold_pad_index(handle, track, pad, velocity, pan);
        /// <summary>call every frame to keep a pad held. lets go about 0.12s after you stop calling</summary>
        public void SustainPad(string track, string pad, float velocity = 1f, float pan = 0f) => Native.melo_instance_sustain_pad(handle, track, pad, velocity, pan);
        public void SustainPad(int trackIndex, string pad, float velocity = 1f, float pan = 0f) => Native.melo_instance_sustain_pad_index(handle, trackIndex, pad, velocity, pan);
        /// <summary>let go of a held pad</summary>
        public void ReleasePad(string track, string pad) => Native.melo_instance_release_pad(handle, track, pad);

        // -- SFX --
        /// <summary>play a sound through this song. effects = through its master effects too</summary>
        public int PlayOneShot(string file, float volume = 1f, float pan = 0f, float pitch = 1f, bool loop = false, bool effects = true) => Native.melo_instance_play_one_shot(handle, file, volume, pan, pitch, loop ? 1 : 0, effects ? 1 : 0);
        /// <summary>loops until StopSfxVoice(voice)</summary>
        public int PlayLoop(string file, float volume = 1f, float pan = 0f, float pitch = 1f, bool effects = true) => Native.melo_instance_play_loop(handle, file, volume, pan, pitch, effects ? 1 : 0);
        /// <summary>stop sound started with PlayOneShot or PlayLoop, both return ints for use with this call.</summary>
        public void StopSfxVoice(int voice) => Native.melo_instance_stop_voice(handle, voice);
        /// <summary>stops all loops and one shots, music is unaffected</summary>
        public void StopSfx() => Native.melo_instance_stop_sfx(handle);
        public void SetVolume(int voice, float v) => Native.melo_instance_voice_volume(handle, voice, v);
        public void SetPan(int voice, float p) => Native.melo_instance_voice_pan(handle, voice, p);
        public void SetPitch(int voice, float s) => Native.melo_instance_voice_pitch(handle, voice, s);
    }

    public sealed class MeloMix
    {
        private readonly IntPtr handle;
        private MeloMix(IntPtr handle) { this.handle = handle; }
        internal static MeloMix Wrap(IntPtr handle) => handle == IntPtr.Zero ? null : new MeloMix(handle);
        /// <summary>false after UnloadMix</summary>
        public bool IsLoaded => Native.melo_instance_is_loaded(handle) != 0;
        /// <summary>the mix's master bus</summary>
        public MeloBus Master => new MeloBus(handle, Native.melo_instance_master_bus_index(handle));
        public string[] GameParamNames
        {
            get
            {
                string[] values = new string[Native.melo_instance_game_param_count(handle)];
                for (int i = 0; i < values.Length; i++) { int index = i; values[i] = Native.Text((buffer, size) => Native.melo_instance_game_param_name(handle, index, buffer, size)); }
                return values;
            }
        }
        public bool SetGameParam(string name, float value) => Native.melo_instance_set_game_param(handle, name, value) != 0;
        public bool ReleaseGameParam(string name) => Native.melo_instance_release_game_param(handle, name) != 0;
        public float GetGameParam(string name) => Native.melo_instance_get_game_param(handle, name);
    }

    public enum ReverbAlgorithm { Freeverb, Psx }
    public enum MeloFilterType { Lowpass, Highpass, Bandpass, Notch }
    /// <summary>handle to one track, cheap to hold or refetch. from MeloInstance.Track()</summary>
    public readonly struct MeloTrack
    {
        private readonly IntPtr instance;
        private readonly int bus;
        /// <summary>index in the project: top in app = 0</summary>
        public int Index { get; }
        internal MeloTrack(IntPtr instance, int index, int bus) { this.instance = instance; Index = index; this.bus = bus; }
        public bool IsValid => Native.melo_owner_is_valid(instance, Index, bus) != 0;
        public string Name
        {
            get
            {
                IntPtr inst = instance;
                int index = Index, owner = bus;
                return Native.Text((buffer, size) => Native.melo_owner_name(inst, index, owner, buffer, size));
            }
        }
        /// <summary>0-1</summary>
        public float Volume { get => Native.melo_owner_get_volume(instance, Index, bus); set => Native.melo_owner_set_volume(instance, Index, bus, value); }
        /// <summary>-1 left, 0 center, 1 right</summary>
        public float Pan { get => Native.melo_owner_get_pan(instance, Index, bus); set => Native.melo_owner_set_pan(instance, Index, bus, value); }
        public bool Muted { get => Native.melo_owner_get_muted(instance, Index, bus) != 0; set => Native.melo_owner_set_muted(instance, Index, bus, value ? 1 : 0); }
        public bool Solo { get => Native.melo_owner_get_solo(instance, Index, bus) != 0; set => Native.melo_owner_set_solo(instance, Index, bus, value ? 1 : 0); }
        /// <summary>send to bus A, 0-1</summary>
        public float SendA { get => Native.melo_owner_get_senda(instance, Index, bus); set => Native.melo_owner_set_senda(instance, Index, bus, value); }
        /// <summary>send to bus B, 0-1</summary>
        public float SendB { get => Native.melo_owner_get_sendb(instance, Index, bus); set => Native.melo_owner_set_sendb(instance, Index, bus, value); }
        /// <summary>instrument settings as json, null clears it</summary>
        public void SetInstrument(string instrumentJson) => Native.melo_track_set_instrument_json(instance, Index, instrumentJson);
        public string Instrument
        {
            get
            {
                IntPtr inst = instance;
                int index = Index;
                return Native.Text((buffer, size) => Native.melo_track_instrument_json(inst, index, buffer, size));
            }
        }

        // -- Effects --
        public int EffectCount => Native.melo_owner_effect_count(instance, Index, bus);
        public IEnumerable<MeloEffect> Effects { get { for (int i = 0; i < EffectCount; i++) yield return MeloEffect.Create(instance, Index, bus, i); } }
        public T GetEffect<T>() where T : MeloEffect
        {
            int index = Native.melo_owner_find_effect(instance, Index, bus, MeloEffect.TypeFor<T>());
            return index < 0 ? null : (T)MeloEffect.Create(instance, Index, bus, index);
        }
        public bool TryGetEffect<T>(out T effect) where T : MeloEffect { effect = GetEffect<T>(); return effect != null; }
        public bool HasEffect<T>() where T : MeloEffect => GetEffect<T>() != null;
        /// <summary>appends to the chain</summary>
        public T AddEffect<T>() where T : MeloEffect
        {
            if (!IsValid) throw new InvalidOperationException("Track or bus is invalid.");
            int index = Native.melo_owner_add_effect(instance, Index, bus, MeloEffect.TypeFor<T>());
            return index < 0 ? null : (T)MeloEffect.Create(instance, Index, bus, index);
        }
        /// <summary>removes the first T, true if it found one</summary>
        public bool RemoveEffect<T>() where T : MeloEffect => Native.melo_owner_remove_effect(instance, Index, bus, MeloEffect.TypeFor<T>()) != 0;
    }
    /// <summary>handle to one bus. shared send destinations (reverb, delay, master). same effect access as MeloTrack</summary>
    public readonly struct MeloBus
    {
        private readonly MeloTrack track;
        internal MeloBus(IntPtr instance, int index) { track = new MeloTrack(instance, index, 1); }
        /// <summary>0 is master</summary>
        public int Index => track.Index;
        public bool IsValid => track.IsValid;
        public string Name => track.Name;
        public float Volume { get => track.Volume; set => track.Volume = value; }
        public float Pan { get => track.Pan; set => track.Pan = value; }
        public bool Muted { get => track.Muted; set => track.Muted = value; }

        // -- Effects --
        public int EffectCount => track.EffectCount;
        public IEnumerable<MeloEffect> Effects => track.Effects;
        public T GetEffect<T>() where T : MeloEffect => track.GetEffect<T>();
        public bool TryGetEffect<T>(out T effect) where T : MeloEffect => track.TryGetEffect(out effect);
        public bool HasEffect<T>() where T : MeloEffect => track.HasEffect<T>();
        public T AddEffect<T>() where T : MeloEffect => track.AddEffect<T>();
        public bool RemoveEffect<T>() where T : MeloEffect => track.RemoveEffect<T>();
    }
    /// <summary>handle to one effect on a track or bus. subclasses expose the params as properties</summary>
    public abstract class MeloEffect
    {
        protected readonly IntPtr Instance;
        protected readonly int Owner, Bus, Effect;
        protected MeloEffect(IntPtr instance, int owner, int bus, int effect) { Instance = instance; Owner = owner; Bus = bus; Effect = effect; }
        public bool IsValid => Native.melo_effect_is_valid(Instance, Owner, Bus, Effect) != 0;
        public string Name => Native.Text((buffer, size) => Native.melo_effect_name(Instance, Owner, Bus, Effect, buffer, size));
        public string TypeId => Native.Text((buffer, size) => Native.melo_effect_type(Instance, Owner, Bus, Effect, buffer, size));
        public bool Enabled { get => Native.melo_effect_get_enabled(Instance, Owner, Bus, Effect) != 0; set => Native.melo_effect_set_enabled(Instance, Owner, Bus, Effect, value ? 1 : 0); }
        public void Bypass() { Enabled = false; }
        public void UnBypass() { Enabled = true; }
        /// <summary>clears delay lines, tails, envelopes</summary>
        public void Reset() => Native.melo_effect_reset(Instance, Owner, Bus, Effect);
        protected float Get(string name) => Native.melo_effect_get_param(Instance, Owner, Bus, Effect, name);
        protected void Set(string name, float value) { Native.melo_effect_set_param(Instance, Owner, Bus, Effect, name, value); }
        public float? GetParam(string name) { float value = Get(name); return float.IsNaN(value) ? (float?)null : value; }
        public bool SetParam(string name, float value) => Native.melo_effect_set_param(Instance, Owner, Bus, Effect, name, value) != 0;
        internal static string TypeFor<T>() where T : MeloEffect
        {
            if (typeof(T) == typeof(MeloAlgorithmicReverb)) return "Reverb";
            if (typeof(T) == typeof(MeloDelay)) return "Delay";
            if (typeof(T) == typeof(MeloChorus)) return "Chorus";
            if (typeof(T) == typeof(MeloFilter)) return "Filter";
            if (typeof(T) == typeof(MeloEq)) return "EQ";
            if (typeof(T) == typeof(MeloEcho)) return "SnesEcho";
            if (typeof(T) == typeof(MeloConvolutionReverb)) return "ConvolutionReverb";
            throw new NotSupportedException("Unsupported effect: " + typeof(T).Name);
        }
        internal static MeloEffect Create(IntPtr instance, int owner, int bus, int effect)
        {
            string type = Native.Text((buffer, size) => Native.melo_effect_type(instance, owner, bus, effect, buffer, size));
            switch (type)
            {
                case "Reverb": return new MeloAlgorithmicReverb(instance, owner, bus, effect);
                case "Delay": return new MeloDelay(instance, owner, bus, effect);
                case "Chorus": return new MeloChorus(instance, owner, bus, effect);
                case "Filter": return new MeloFilter(instance, owner, bus, effect);
                case "EQ": return new MeloEq(instance, owner, bus, effect);
                case "SnesEcho": return new MeloEcho(instance, owner, bus, effect);
                case "ConvolutionReverb": return new MeloConvolutionReverb(instance, owner, bus, effect);
                default: return new MeloUnknownEffect(instance, owner, bus, effect);
            }
        }
    }
    /// <summary>fallback for anything without a wrapper</summary>
    public sealed class MeloUnknownEffect : MeloEffect
    {
        internal MeloUnknownEffect(IntPtr instance, int owner, int bus, int effect) : base(instance, owner, bus, effect) { }
    }
    /// <summary>algorithmic reverb. either psx or freeverb</summary>
    public sealed class MeloAlgorithmicReverb : MeloEffect
    {
        internal MeloAlgorithmicReverb(IntPtr instance, int owner, int bus, int effect) : base(instance, owner, bus, effect) { }
        /// <summary>algorithmic reverb. either psx or freeverb</summary>
        public ReverbAlgorithm Algorithm { get => (ReverbAlgorithm)Get("Algorithm"); set => Set("Algorithm", (float)value); }
        /// <summary>0-1, room size or PSX preset</summary>
        public float RoomSize { get => Get("RoomSize"); set => Set("RoomSize", value); }
        /// <summary>0-1, higher = darker</summary>
        public float Damping { get => Get("Damping"); set => Set("Damping", value); }
        /// <summary>wet/dry 0-1</summary>
        public float Mix { get => Get("Mix"); set => Set("Mix", value); }
    }
    /// <summary>stereo delay. SyncEnabled + SyncDivisionIndex for tempo locked repeats, otherwise Interval in ms</summary>
    public sealed class MeloDelay : MeloEffect
    {
        internal MeloDelay(IntPtr instance, int owner, int bus, int effect) : base(instance, owner, bus, effect) { }
        /// <summary>ms when sync is off, ~1-3000</summary>
        public float Interval { get => Get("Interval"); set => Set("Interval", value); }
        /// <summary>ms for repeats to die out</summary>
        public float DecayTime { get => Get("DecayTime"); set => Set("DecayTime", value); }
        /// <summary>wet/dry 0-1</summary>
        public float Mix { get => Get("Mix"); set => Set("Mix", value); }
        /// <summary>alternate repeats left/right</summary>
        public bool PingPong { get => Get("PingPong") != 0; set => Set("PingPong", value ? 1f : 0f); }
        /// <summary>hz where the feedback path rolls off, default 8000</summary>
        public float DampingFreq { get => Get("DampingFreq"); set => Set("DampingFreq", value); }
        /// <summary>true = ignore Interval and track tempo via SyncDivisionIndex</summary>
        public bool SyncEnabled { get => Get("SyncEnabled") != 0; set => Set("SyncEnabled", value ? 1f : 0f); }
        /// <summary>3 = 1/4 by default</summary>
        public int SyncDivisionIndex { get => (int)Get("SyncDivisionIndex"); set => Set("SyncDivisionIndex", (float)value); }
    }
    /// <summary>stereo chorus</summary>
    public sealed class MeloChorus : MeloEffect
    {
        internal MeloChorus(IntPtr instance, int owner, int bus, int effect) : base(instance, owner, bus, effect) { }
        /// <summary>hz, 0.1-5</summary>
        public float Rate { get => Get("Rate"); set => Set("Rate", value); }
        /// <summary>ms, 0.5-10</summary>
        public float Depth { get => Get("Depth"); set => Set("Depth", value); }
        /// <summary>wet/dry 0-1</summary>
        public float Mix { get => Get("Mix"); set => Set("Mix", value); }
        /// <summary>0-0.8, higher goes flangery</summary>
        public float Feedback { get => Get("Feedback"); set => Set("Feedback", value); }
        /// <summary>1-3</summary>
        public int Voices { get => (int)Get("Voices"); set => Set("Voices", (float)value); }
    }
    /// <summary>resonant filter with env follower + lfo</summary>
    public sealed class MeloFilter : MeloEffect
    {
        internal MeloFilter(IntPtr instance, int owner, int bus, int effect) : base(instance, owner, bus, effect) { }
        /// <summary></summary>
        public MeloFilterType Type { get => (MeloFilterType)Get("Type"); set => Set("Type", (float)value); }
        /// <summary>hz, 20-20000. lower = darker</summary>
        public float Cutoff { get => Get("Cutoff"); set => Set("Cutoff", value); }
        /// <summary>Q, 0.5-20. high values whistle and can self oscillate</summary>
        public float Resonance { get => Get("Resonance"); set => Set("Resonance", value); }
        /// <summary>wet/dry 0-1, default 1</summary>
        public float Mix { get => Get("Mix"); set => Set("Mix", value); }
        /// <summary>env follower amount in semitones, 0-48</summary>
        public float EnvAmount { get => Get("EnvAmount"); set => Set("EnvAmount", value); }
        /// <summary>seconds</summary>
        public float EnvAttack { get => Get("EnvAttack"); set => Set("EnvAttack", value); }
        /// <summary>seconds</summary>
        public float EnvRelease { get => Get("EnvRelease"); set => Set("EnvRelease", value); }
        /// <summary>hz, 0.05-20</summary>
        public float LfoRate { get => Get("LfoRate"); set => Set("LfoRate", value); }
        /// <summary>semitones of cutoff, 0-48. 0 = lfo off</summary>
        public float LfoDepth { get => Get("LfoDepth"); set => Set("LfoDepth", value); }
        /// <summary>true = ignore LfoRate and sync to tempo via LfoSyncDivisionIndex</summary>
        public bool LfoSyncEnabled { get => Get("LfoSyncEnabled") != 0; set => Set("LfoSyncEnabled", value ? 1f : 0f); }
        /// <summary>5 = 1/4 by default</summary>
        public int LfoSyncDivisionIndex { get => (int)Get("LfoSyncDivisionIndex"); set => Set("LfoSyncDivisionIndex", (float)value); }
        /// <summary>live env value 0-1</summary>
        public float EnvelopeValue => Get("EnvelopeValue");
        /// <summary>live lfo value -1..1</summary>
        public float LfoValue => Get("LfoValue");
    }
    /// <summary>3 band, low shelf / mid peak / high shelf</summary>
    public sealed class MeloEq : MeloEffect
    {
        internal MeloEq(IntPtr instance, int owner, int bus, int effect) : base(instance, owner, bus, effect) { }
        /// <summary>hz</summary>
        public float LowFreq { get => Get("LowFreq"); set => Set("LowFreq", value); }
        /// <summary>db, -12..12</summary>
        public float LowGain { get => Get("LowGain"); set => Set("LowGain", value); }
        /// <summary>hz</summary>
        public float MidFreq { get => Get("MidFreq"); set => Set("MidFreq", value); }
        /// <summary>db, -12..12</summary>
        public float MidGain { get => Get("MidGain"); set => Set("MidGain", value); }
        /// <summary>0.1-10, higher = narrower</summary>
        public float MidQ { get => Get("MidQ"); set => Set("MidQ", value); }
        /// <summary>hz</summary>
        public float HighFreq { get => Get("HighFreq"); set => Set("HighFreq", value); }
        /// <summary>db, -12..12</summary>
        public float HighGain { get => Get("HighGain"); set => Set("HighGain", value); }
    }
    public sealed class MeloEcho : MeloEffect
    {
        internal MeloEcho(IntPtr instance, int owner, int bus, int effect) : base(instance, owner, bus, effect) { }
        /// <summary></summary>
        public float Size { get => Get("Size"); set => Set("Size", value); }
        /// <summary></summary>
        public float Feedback { get => Get("Feedback"); set => Set("Feedback", value); }
        /// <summary></summary>
        public float Mix { get => Get("Mix"); set => Set("Mix", value); }
    }
    /// <summary>impulses response reverb, heavier proccessing than algorithmic reverb</summary>
    public sealed class MeloConvolutionReverb : MeloEffect
    {
        internal MeloConvolutionReverb(IntPtr instance, int owner, int bus, int effect) : base(instance, owner, bus, effect) { }
        /// <summary>wet/dry 0-1</summary>
        public float Mix { get => Get("Mix"); set => Set("Mix", value); }
        /// <summary>set in melowrite before export</summary>
        public string IrName => Native.Text((buffer, size) => Native.melo_effect_ir_name(Instance, Owner, Bus, Effect, buffer, size));
    }
    public sealed class MeloProjectMetadata
    {
        public string Name { get; private set; }
        public int Tempo { get; private set; }
        public int BeatsPerBar { get; private set; }
        public int BeatUnit { get; private set; }
        public List<string> ChunkNames { get; } = new List<string>();
        public List<string> TrackNames { get; } = new List<string>();
        public List<string> BusNames { get; } = new List<string>();
        public List<string> PaletteNames { get; } = new List<string>();
        /// <summary>a project's name, tempo and names, without loading samples or playing anything</summary>
        public static MeloProjectMetadata Read(string projectPath)
        {
            if (string.IsNullOrEmpty(projectPath)) return null;
            IntPtr handle = Native.melo_metadata_open(projectPath);
            if (handle == IntPtr.Zero) return null;
            try
            {
                MeloProjectMetadata metadata = new MeloProjectMetadata
                {
                    Name = Native.Text((buffer, size) => Native.melo_metadata_text(handle, 0, 0, buffer, size)),
                    Tempo = Native.melo_metadata_number(handle, 0), BeatsPerBar = Native.melo_metadata_number(handle, 1), BeatUnit = Native.melo_metadata_number(handle, 2)
                };
                List<string>[] lists = { metadata.ChunkNames, metadata.TrackNames, metadata.BusNames, metadata.PaletteNames };
                for (int field = 3; field <= 6; field++)
                    for (int i = 0; i < Native.melo_metadata_number(handle, field); i++)
                    {
                        int kind = field, index = i;
                        lists[field - 3].Add(Native.Text((buffer, size) => Native.melo_metadata_text(handle, kind, index, buffer, size)));
                    }
                return metadata;
            }
            finally { Native.melo_metadata_close(handle); }
        }
    }
}
