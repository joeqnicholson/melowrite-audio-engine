/* melowrite c and c++ api, link against Melowrite.Core.dll */
#ifndef MELO_H
#define MELO_H
#include <stdint.h>
#ifndef MELO_API
#if defined(_WIN32)
#define MELO_API __declspec(dllimport)
#else
#define MELO_API
#endif
#endif
#ifdef __cplusplus
extern "C" {
#endif
typedef void* MeloInstanceHandle;
typedef void (*MeloLoadCallback)(MeloInstanceHandle instance, void* user_data);
typedef void (*MeloNoteCallback)(MeloInstanceHandle instance, int track, int pitch, int velocity, void* user_data);
typedef void (*MeloBeatCallback)(MeloInstanceHandle instance, int bar, int beat, void* user_data);
typedef void (*MeloBarCallback)(MeloInstanceHandle instance, int bar, void* user_data);
typedef void (*MeloTimedCallback)(MeloInstanceHandle instance, void* user_data);
typedef void (*MeloAudioCommand)(MeloInstanceHandle engine, void* user_data);
typedef void (*MeloLogCallback)(const char* message, void* user_data);
typedef const char* (*MeloPathResolver)(const char* path, void* user_data);
typedef int (*MeloPathResolverUtf8)(const char* path, char* buffer, int size, void* user_data);
typedef enum MeloSwitchTiming { MELO_SWITCH_NOW, MELO_SWITCH_BEAT, MELO_SWITCH_BAR, MELO_SWITCH_QUEUE } MeloSwitchTiming;
typedef enum MeloSpatialDimension { MELO_SPATIAL_3D, MELO_SPATIAL_2D } MeloSpatialDimension;
/**
 * call before loading any projects or playing any audio. sample_rate has to match the audio device (bad values fall back to 44100). to change it later stop audio and melo_host_shutdown first
 */
MELO_API void melo_host_init(int sample_rate);
/**
 * listener pose in +X right, +Y up, -Z forward coordinates
 */
MELO_API void melo_set_spatial_listener(float x, float y, float z, float forward_x, float forward_y, float forward_z, float up_x, float up_y, float up_z);
MELO_API void melo_set_spatial_defaults(float min_dist, float falloff, float blend, int mode);
/**
 * preallocate independent spatial voices before gameplay
 */
MELO_API int melo_instance_prepare_spatial_pad(MeloInstanceHandle instance, const char* track, const char* pad, int voices);
/**
 * play a pad at a world position. returns a unique playback handle, or -1 if unavailable
 */
MELO_API int melo_instance_trigger_spatial_pad(MeloInstanceHandle instance, const char* track, const char* pad, float x, float y, float z, float velocity);
MELO_API int melo_instance_hold_spatial_pad(MeloInstanceHandle instance, const char* track, const char* pad, float x, float y, float z, float velocity);
/**
 * move only this spatial playback; completed handles are ignored
 */
MELO_API void melo_instance_move_spatial_pad(MeloInstanceHandle instance, int voice, float x, float y, float z);
/**
 * release only this spatial playback
 */
MELO_API void melo_instance_release_spatial_pad(MeloInstanceHandle instance, int voice);
MELO_API int melo_instance_is_spatial_pad_playing(MeloInstanceHandle instance, int voice);

/**
 * output sample rate
 */
MELO_API int melo_host_sample_rate(void);

/**
 * engine build string
 */
MELO_API int melo_host_build(char* buffer, int size);

/**
 * audio thread. fills frames of interleaved stereo floats, handled by Unity plugin, MonoGame host and SDL host
 */
MELO_API int melo_host_fill_buffer(float* buffer, int frames);

/**
 * call once a frame. fires load callbacks, events and melo_instance_run_at callbacks
 */
MELO_API void melo_host_tick(void);

/**
 * frees the engine. stop the audio device first
 */
MELO_API void melo_host_shutdown(void);

/**
 * maps a content path to a file on disk. passes it through by default
 */
MELO_API void melo_host_set_path_resolver(MeloPathResolver callback, void* user_data);

/**
 * maps a content path to a file on disk. passes it through by default
 */
MELO_API void melo_host_set_path_resolver_utf8(MeloPathResolverUtf8 callback, void* user_data);

/**
 * engine errors write here, stderr by default
 */
MELO_API void melo_host_set_log_error(MeloLogCallback callback, void* user_data);

/**
 * engine warnings write here, stdout by default
 */
MELO_API void melo_host_set_log_warning(MeloLogCallback callback, void* user_data);

/**
 * load an exported .melo
 */
MELO_API MeloInstanceHandle melo_load_instance(const char* path);

/**
 * load on a background thread. the callback comes through melo_host_tick
 */
MELO_API void melo_load_async(const char* path, MeloLoadCallback callback, void* user_data);

/**
 * master volume 0-1, music and sfx
 */
MELO_API float melo_get_volume(void);

/**
 * master volume 0-1, music and sfx
 */
MELO_API void melo_set_volume(float value);

/**
 * stops all music. pads stay loaded
 */
MELO_API void melo_stop_all(void);

/**
 * unloads everything and clears the sound caches
 */
MELO_API void melo_unload_all(void);

/**
 * master mix project all audio routes through, one-shots and raw audio as well. stays until melo_unload_mix
 */
MELO_API MeloInstanceHandle melo_load_mix(const char* path);

/**
 * removes global mix
 */
MELO_API void melo_unload_mix(void);

/**
 * every note played: instance, track index, midi pitch, velocity 0-127. fires from melo_host_tick
 */
MELO_API void melo_on_note_add(MeloNoteCallback callback, void* user_data);

MELO_API void melo_on_note_remove(MeloNoteCallback callback, void* user_data);

/**
 * every beat: instance, bar, beat in the bar, both from 0. fires from melo_host_tick
 */
MELO_API void melo_on_beat_add(MeloBeatCallback callback, void* user_data);

MELO_API void melo_on_beat_remove(MeloBeatCallback callback, void* user_data);

/**
 * every bar: instance, bar from 0. fires from melo_host_tick
 */
MELO_API void melo_on_bar_add(MeloBarCallback callback, void* user_data);

MELO_API void melo_on_bar_remove(MeloBarCallback callback, void* user_data);

/**
 * play an audio file, returns a voice id. pitch is a rate multiplier, 2 = up an octave
 */
MELO_API int melo_play_one_shot(const char* file, float volume, float pan, float pitch, int loop, int effects);

/**
 * loops until melo_global_stop_voice(voice)
 */
MELO_API int melo_play_loop(const char* file, float volume, float pan, float pitch, int effects);

MELO_API int melo_play_spatial_one_shot(const char* file, float x, float y, float z, float volume, float pitch, int loop, int effects);

MELO_API void melo_global_move_spatial_one_shot(int voice, float x, float y, float z);

/**
 * true while the voice or its tail is still going
 */
MELO_API int melo_global_voice_is_playing(int voice);

/**
 * stop sound started with melo_play_one_shot or melo_play_loop, both return ints for use with this call.
 */
MELO_API void melo_global_stop_voice(int voice);

/**
 * stops all loops and one shots, music is unaffected
 */
MELO_API void melo_global_stop_sfx(void);

MELO_API void melo_global_voice_volume(int voice, float value);

MELO_API void melo_global_voice_pan(int voice, float value);

MELO_API void melo_global_voice_pitch(int voice, float value);

/**
 * sfx volume 0-1, on top of melo_get_volume
 */
MELO_API float melo_get_sfx_volume(void);

/**
 * sfx volume 0-1, on top of melo_get_volume
 */
MELO_API void melo_set_sfx_volume(float value);

MELO_API void melo_set_reverb(float mix, float room_size, float damping);

MELO_API void melo_set_delay(float mix, float interval_ms, float decay_ms);

MELO_API void melo_clear_effects(void);

/**
 * copy a project bus's effects onto the sfx bus. wet_mix 0-1 overrides every effect's mix, -1 leaves them
 */
MELO_API void melo_copy_effects_from(MeloInstanceHandle instance, const char* bus, float wet_mix);

MELO_API int melo_clip_is_loaded(MeloInstanceHandle instance, int global, const char* key);

MELO_API int melo_clip_load_pcm(MeloInstanceHandle instance, int global, const char* key, const float* left, const float* right, int frames, int sample_rate);

MELO_API int melo_clip_play(MeloInstanceHandle instance, int global, const char* key, float volume, float pan, float pitch, int loop, int effects);

MELO_API MeloInstanceHandle melo_metadata_open(const char* path);

MELO_API void melo_metadata_close(MeloInstanceHandle metadata);

MELO_API int melo_metadata_number(MeloInstanceHandle metadata, int field);

MELO_API int melo_metadata_text(MeloInstanceHandle metadata, int field, int index, char* buffer, int size);

/**
 * play a chunk, NULL = chunk 0. fade_out in seconds
 */
MELO_API void melo_instance_play_chunk(MeloInstanceHandle instance, const char* chunk, int looping, float fade_out);

/**
 * crossfade to a chunk over duration seconds
 */
MELO_API void melo_instance_play_chunk_crossfade(MeloInstanceHandle instance, const char* chunk, float duration, int looping);

/**
 * play a chunk once on top of the current music. cleans itself up when done
 */
MELO_API void melo_instance_play_chunk_headless(MeloInstanceHandle instance, const char* chunk);

/**
 * switch chunks now, on the next beat, next bar or when the loop ends
 */
MELO_API void melo_instance_switch_chunk(MeloInstanceHandle instance, const char* chunk, int when);

MELO_API void melo_instance_play_chunk_index(MeloInstanceHandle instance, int chunk, int looping, float fade_out);

MELO_API void melo_instance_play_chunk_crossfade_index(MeloInstanceHandle instance, int chunk, float duration, int looping);

MELO_API void melo_instance_play_chunk_headless_index(MeloInstanceHandle instance, int chunk);

MELO_API void melo_instance_switch_chunk_index(MeloInstanceHandle instance, int chunk, int when);

/**
 * play the arrangement. fade_out fades the current music first, in seconds
 */
MELO_API void melo_instance_play_arrangement(MeloInstanceHandle instance, float fade_out);

/**
 * crossfade into the arrangement over duration seconds
 */
MELO_API void melo_instance_play_arrangement_crossfade(MeloInstanceHandle instance, float duration);

/**
 * stop, tails ring out. stays loaded
 */
MELO_API void melo_instance_stop(MeloInstanceHandle instance);

MELO_API void melo_instance_pause(MeloInstanceHandle instance);

MELO_API void melo_instance_resume(MeloInstanceHandle instance);

/**
 * fade out over duration seconds, 0 stops right away
 */
MELO_API void melo_instance_fade_out(MeloInstanceHandle instance, float duration);

/**
 * cancel a switch that hasnt happened yet
 */
MELO_API void melo_instance_cancel_switch(MeloInstanceHandle instance);

MELO_API void melo_instance_seek_bar(MeloInstanceHandle instance, int bar);

MELO_API void melo_instance_set_tempo(MeloInstanceHandle instance, int bpm);

MELO_API void melo_instance_set_master_volume(MeloInstanceHandle instance, float volume);

/**
 * -1 left, 0 center, 1 right
 */
MELO_API void melo_instance_set_pan(MeloInstanceHandle instance, float pan);

MELO_API void melo_instance_swap_palette(MeloInstanceHandle instance, const char* palette);

/**
 * switch to another song. no start chunk = its arrangement. fade_out in seconds
 */
MELO_API void melo_instance_switch_song(MeloInstanceHandle instance, const char* target, int when, float fade_out);

/**
 * start on a named chunk
 */
MELO_API void melo_instance_switch_song_at_name(MeloInstanceHandle instance, const char* target, const char* start_chunk, int when, float fade_out);

/**
 * start on a chunk index
 */
MELO_API void melo_instance_switch_song_at_index(MeloInstanceHandle instance, const char* target, int start_chunk, int when, float fade_out);

/**
 * switch to a song you already loaded, skips the decode
 */
MELO_API void melo_instance_switch_song_loaded(MeloInstanceHandle instance, MeloInstanceHandle target, int when, float fade_out);

MELO_API void melo_instance_switch_song_loaded_at_name(MeloInstanceHandle instance, MeloInstanceHandle target, const char* start_chunk, int when, float fade_out);

MELO_API void melo_instance_switch_song_loaded_at_index(MeloInstanceHandle instance, MeloInstanceHandle target, int start_chunk, int when, float fade_out);

MELO_API void melo_instance_switch_song_crossfade(MeloInstanceHandle instance, const char* target, float duration, int when);

MELO_API void melo_instance_switch_song_crossfade_at_name(MeloInstanceHandle instance, const char* target, const char* start_chunk, float duration, int when);

MELO_API void melo_instance_switch_song_crossfade_at_index(MeloInstanceHandle instance, const char* target, int start_chunk, float duration, int when);

MELO_API void melo_instance_switch_song_crossfade_loaded(MeloInstanceHandle instance, MeloInstanceHandle target, float duration, int when);

MELO_API void melo_instance_switch_song_crossfade_loaded_at_name(MeloInstanceHandle instance, MeloInstanceHandle target, const char* start_chunk, float duration, int when);

MELO_API void melo_instance_switch_song_crossfade_loaded_at_index(MeloInstanceHandle instance, MeloInstanceHandle target, int start_chunk, float duration, int when);

MELO_API void melo_instance_set_track_muted(MeloInstanceHandle instance, const char* track, int value);

MELO_API void melo_instance_set_track_muted_index(MeloInstanceHandle instance, int track, int value);

MELO_API void melo_instance_set_track_volume(MeloInstanceHandle instance, const char* track, float value);

MELO_API void melo_instance_set_track_volume_index(MeloInstanceHandle instance, int track, float value);

/**
 * send to bus A or B, 0-1
 */
MELO_API void melo_instance_set_send_a(MeloInstanceHandle instance, const char* track, float value);

MELO_API void melo_instance_set_send_a_index(MeloInstanceHandle instance, int track, float value);

MELO_API void melo_instance_set_send_b(MeloInstanceHandle instance, const char* track, float value);

MELO_API void melo_instance_set_send_b_index(MeloInstanceHandle instance, int track, float value);

MELO_API void melo_instance_set_bus_volume(MeloInstanceHandle instance, int bus, float volume);

/**
 * e.g. melo_instance_set_bus_effect_param(instance, 1, 0, "Mix", 0.5f)
 */
MELO_API void melo_instance_set_bus_effect_param(MeloInstanceHandle instance, int owner, int effect, const char* param, float value);

/**
 * same thing for a track effect
 */
MELO_API void melo_instance_set_track_effect_param(MeloInstanceHandle instance, int owner, int effect, const char* param, float value);

/**
 * hit a pad by name. velocity 0-1, pan -1 to 1. hits overlap
 */
MELO_API void melo_instance_trigger_pad(MeloInstanceHandle instance, const char* track, const char* pad, float velocity, float pan);

MELO_API void melo_instance_trigger_pad_index(MeloInstanceHandle instance, int track, const char* pad, float velocity, float pan);

MELO_API void melo_instance_set_pad_held(MeloInstanceHandle instance, const char* track, const char* pad, int held, float velocity, float pan);

MELO_API void melo_instance_set_pad_held_index(MeloInstanceHandle instance, int track, const char* pad, int held, float velocity, float pan);

/**
 * let go of a held pad
 */
MELO_API void melo_instance_release_pad_held(MeloInstanceHandle instance, const char* track, const char* pad);

MELO_API void melo_instance_release_pad_held_index(MeloInstanceHandle instance, int track, const char* pad);

/**
 * call every frame to keep a pad held. lets go about 0.12s after you stop calling
 */
MELO_API void melo_instance_hold_pad_this_frame(MeloInstanceHandle instance, const char* track, const char* pad, float velocity, float pan);

MELO_API void melo_instance_hold_pad_this_frame_index(MeloInstanceHandle instance, int track, const char* pad, float velocity, float pan);

/**
 * start a midi note, velocity 0-1
 */
MELO_API void melo_instance_note_on(MeloInstanceHandle instance, int track, int midi_note, float velocity);

/**
 * stop a midi note
 */
MELO_API void melo_instance_note_off(MeloInstanceHandle instance, int track, int midi_note);

/**
 * play a sound through this song. effects = through its master effects too
 */
MELO_API int melo_instance_play_one_shot(MeloInstanceHandle instance, const char* file, float volume, float pan, float pitch, int loop, int effects);

/**
 * loops until melo_instance_stop_voice(instance, voice)
 */
MELO_API int melo_instance_play_loop(MeloInstanceHandle instance, const char* file, float volume, float pan, float pitch, int effects);

MELO_API int melo_instance_play_spatial_one_shot(MeloInstanceHandle instance, const char* file, float x, float y, float z, float volume, float pitch, int loop, int effects);

MELO_API void melo_instance_move_spatial_one_shot(MeloInstanceHandle instance, int voice, float x, float y, float z);

/**
 * stop sound started with melo_instance_play_one_shot or melo_instance_play_loop, both return ints for use with this call.
 */
MELO_API void melo_instance_stop_voice(MeloInstanceHandle instance, int voice);

/**
 * stops all loops and one shots, music is unaffected
 */
MELO_API void melo_instance_stop_sfx(MeloInstanceHandle instance);

MELO_API void melo_instance_voice_volume(MeloInstanceHandle instance, int voice, float value);

MELO_API void melo_instance_voice_pan(MeloInstanceHandle instance, int voice, float value);

MELO_API void melo_instance_voice_pitch(MeloInstanceHandle instance, int voice, float value);

/**
 * unload this song. melo_unload_all also clears the shared caches
 */
MELO_API void melo_instance_unload(MeloInstanceHandle instance);

/**
 * instance-specific second global mix project, with parameters and all that, the loaded song passes through. stays until melo_instance_unload_mix
 */
MELO_API MeloInstanceHandle melo_instance_load_mix(MeloInstanceHandle instance, const char* path);

/**
 * removes the instance's second global mix project
 */
MELO_API void melo_instance_unload_mix(MeloInstanceHandle instance);

/**
 * queue a command for the audio thread
 */
MELO_API void melo_instance_run_on_audio_thread(MeloInstanceHandle instance, MeloAudioCommand callback, void* user_data);

/**
 * run code on the main thread at the next beat, bar or loop. fires from melo_host_tick
 */
MELO_API void melo_instance_run_at(MeloInstanceHandle instance, int when, MeloTimedCallback callback, void* user_data);

/**
 * audio thread version, at the next beat, bar or loop. keep it quick
 */
MELO_API void melo_instance_run_on_audio_thread_at(MeloInstanceHandle instance, int when, MeloAudioCommand callback, void* user_data);

/**
 * drop any melo_instance_run_at and melo_instance_run_on_audio_thread_at still waiting. switches already queued stay
 */
MELO_API void melo_instance_cancel_run_at(MeloInstanceHandle instance);

/**
 * native engine handle, zero after melo_instance_unload
 */
MELO_API MeloInstanceHandle melo_instance_engine(MeloInstanceHandle instance);

MELO_API int melo_instance_is_loaded(MeloInstanceHandle instance);

MELO_API int melo_instance_current_bar(MeloInstanceHandle instance);

MELO_API int melo_instance_current_beat(MeloInstanceHandle instance);

MELO_API int melo_instance_tempo(MeloInstanceHandle instance);

MELO_API int melo_instance_chunk_count(MeloInstanceHandle instance);

MELO_API int melo_instance_track_count(MeloInstanceHandle instance);

MELO_API int melo_instance_file(MeloInstanceHandle instance, char* buffer, int size);

MELO_API int melo_instance_active_chunk(MeloInstanceHandle instance, char* buffer, int size);

MELO_API int melo_instance_chunk_name(MeloInstanceHandle instance, int index, char* buffer, int size);

MELO_API int melo_instance_track_name(MeloInstanceHandle instance, int index, char* buffer, int size);

MELO_API int melo_instance_bus_count(MeloInstanceHandle instance);

MELO_API int melo_instance_bus_name(MeloInstanceHandle instance, int index, char* buffer, int size);

MELO_API int melo_instance_palette_count(MeloInstanceHandle instance);

MELO_API int melo_instance_palette_name(MeloInstanceHandle instance, int index, char* buffer, int size);

MELO_API int melo_instance_track_index(MeloInstanceHandle instance, const char* name);

MELO_API int melo_instance_bus_index(MeloInstanceHandle instance, const char* name);

MELO_API int melo_instance_set_game_param(MeloInstanceHandle instance, const char* name, float value);

MELO_API int melo_instance_release_game_param(MeloInstanceHandle instance, const char* name);

MELO_API float melo_instance_get_game_param(MeloInstanceHandle instance, const char* name);

MELO_API int melo_instance_game_param_count(MeloInstanceHandle instance);

MELO_API int melo_instance_game_param_name(MeloInstanceHandle instance, int index, char* buffer, int size);

MELO_API float melo_instance_game_param_default(MeloInstanceHandle instance, int index);

MELO_API int melo_instance_master_bus_index(MeloInstanceHandle instance);

MELO_API int melo_owner_is_valid(MeloInstanceHandle instance, int owner, int bus);

MELO_API int melo_owner_name(MeloInstanceHandle instance, int owner, int bus, char* buffer, int size);

/**
 * 0-1
 */
MELO_API float melo_owner_get_volume(MeloInstanceHandle instance, int owner, int bus);

/**
 * 0-1
 */
MELO_API void melo_owner_set_volume(MeloInstanceHandle instance, int owner, int bus, float value);

/**
 * -1 left, 0 center, 1 right
 */
MELO_API float melo_owner_get_pan(MeloInstanceHandle instance, int owner, int bus);

/**
 * -1 left, 0 center, 1 right
 */
MELO_API void melo_owner_set_pan(MeloInstanceHandle instance, int owner, int bus, float value);

MELO_API int melo_owner_get_muted(MeloInstanceHandle instance, int owner, int bus);

MELO_API void melo_owner_set_muted(MeloInstanceHandle instance, int owner, int bus, int value);

MELO_API int melo_owner_get_solo(MeloInstanceHandle instance, int owner, int bus);

MELO_API void melo_owner_set_solo(MeloInstanceHandle instance, int owner, int bus, int value);

/**
 * send to bus A, 0-1
 */
MELO_API float melo_owner_get_senda(MeloInstanceHandle instance, int owner, int bus);

/**
 * send to bus A, 0-1
 */
MELO_API void melo_owner_set_senda(MeloInstanceHandle instance, int owner, int bus, float value);

/**
 * send to bus B, 0-1
 */
MELO_API float melo_owner_get_sendb(MeloInstanceHandle instance, int owner, int bus);

/**
 * send to bus B, 0-1
 */
MELO_API void melo_owner_set_sendb(MeloInstanceHandle instance, int owner, int bus, float value);

MELO_API int melo_owner_effect_count(MeloInstanceHandle instance, int owner, int bus);

/**
 * set any value on this track by path: track/volume, instrument/osc1/detune, instrument/pad/36/pitch,
 * fx/0/mix, fx/Reverb/roomSize. false if nothing answers to that path
 */
MELO_API int melo_owner_set_param(MeloInstanceHandle instance, int owner, int bus, const char* path, float value);

/**
 * read any value on this track by path. NaN if nothing answers to that path
 */
MELO_API float melo_owner_get_param(MeloInstanceHandle instance, int owner, int bus, const char* path);

MELO_API int melo_owner_param_count(MeloInstanceHandle instance, int owner, int bus);

MELO_API int melo_owner_param_path(MeloInstanceHandle instance, int owner, int bus, int index, char* buffer, int size);

MELO_API int melo_owner_param_range(MeloInstanceHandle instance, int owner, int bus, int index, float* min, float* max, float* default_value);

MELO_API int melo_owner_find_effect(MeloInstanceHandle instance, int owner, int bus, const char* type);

/**
 * appends to the chain
 */
MELO_API int melo_owner_add_effect(MeloInstanceHandle instance, int owner, int bus, const char* type);

/**
 * removes the first T, true if it found one
 */
MELO_API int melo_owner_remove_effect(MeloInstanceHandle instance, int owner, int bus, const char* type);

MELO_API int melo_track_instrument_json(MeloInstanceHandle instance, int track, char* buffer, int size);

/**
 * instrument settings as json, NULL clears it
 */
MELO_API void melo_track_set_instrument_json(MeloInstanceHandle instance, int track, const char* json);

MELO_API int melo_effect_is_valid(MeloInstanceHandle instance, int owner, int bus, int effect);

MELO_API int melo_effect_name(MeloInstanceHandle instance, int owner, int bus, int effect, char* buffer, int size);

MELO_API int melo_effect_type(MeloInstanceHandle instance, int owner, int bus, int effect, char* buffer, int size);

MELO_API int melo_effect_get_enabled(MeloInstanceHandle instance, int owner, int bus, int effect);

MELO_API void melo_effect_set_enabled(MeloInstanceHandle instance, int owner, int bus, int effect, int value);

/**
 * clears delay lines, tails, envelopes
 */
MELO_API void melo_effect_reset(MeloInstanceHandle instance, int owner, int bus, int effect);

MELO_API float melo_effect_get_param(MeloInstanceHandle instance, int owner, int bus, int effect, const char* param);

MELO_API int melo_effect_set_param(MeloInstanceHandle instance, int owner, int bus, int effect, const char* param, float value);

/**
 * set in melowrite before export
 */
MELO_API int melo_effect_ir_name(MeloInstanceHandle instance, int owner, int bus, int effect, char* buffer, int size);

MELO_API void melo_effect_spatial_set_positions(MeloInstanceHandle instance, int owner, int bus, int effect, float listener_x, float listener_y, float listener_z, float source_x, float source_y, float source_z);

MELO_API void melo_effect_spatial_set_positions_2d(MeloInstanceHandle instance, int owner, int bus, int effect, float listener_x, float listener_y, float source_x, float source_y);

#ifdef __cplusplus
} /* extern "C" */
#include <string>
#include <vector>

namespace Melowrite {
namespace detail {
// asks for the size then reads. reads again if the text grew in between, like a chunk switching
template<class Query> inline std::string Text(Query query) {
    int size = query(nullptr, 0);
    while (size > 0) {
        std::string value(static_cast<size_t>(size) + 1, '\0');
        int actual = query(&value[0], size + 1);
        if (actual <= size) { value.resize(static_cast<size_t>(actual)); return value; }
        size = actual;
    }
    return {};
}
}

/**
 * when a switch lands
 */
enum class MeloSwitch
{
    Now,     // immediately
    Beat,    // on the next beat
    Bar,     // on the next bar
    Queue    // when the current chunk finishes its loop
};
enum class MeloSpatialMode { ThreeD, TwoD };
enum class ReverbAlgorithm { Freeverb, Psx };
enum class MeloFilterType { Lowpass, Highpass, Bandpass, Notch };

// these are indices into a loaded song, refetch them after a song switch or an fx chain change
/**
 * handle to one effect on a track or bus. subclasses expose the params as properties
 */
class MeloEffect {
protected:
    MeloInstanceHandle instance_ = nullptr;
    int owner_ = -1, bus_ = 0, effect_ = -1;
public:
    MeloEffect() = default;
    MeloEffect(MeloInstanceHandle instance, int owner, int bus, int effect)
        : instance_(instance), owner_(owner), bus_(bus), effect_(effect) {}
    bool IsValid() const { return melo_effect_is_valid(instance_, owner_, bus_, effect_) != 0; }
    std::string Name() const { return detail::Text([&](char* b, int n) { return melo_effect_name(instance_, owner_, bus_, effect_, b, n); }); }
    bool Enabled() const { return melo_effect_get_enabled(instance_, owner_, bus_, effect_) != 0; }
    void Enabled(bool enabled) const { melo_effect_set_enabled(instance_, owner_, bus_, effect_, enabled); }
    void Bypass() const { Enabled(false); }
    void UnBypass() const { Enabled(true); }
    /**
     * clears delay lines, tails, envelopes
     */
    void Reset() const { melo_effect_reset(instance_, owner_, bus_, effect_); }
    float Get(const char* param) const { return melo_effect_get_param(instance_, owner_, bus_, effect_, param); }
    void Set(const char* param, float value) const { melo_effect_set_param(instance_, owner_, bus_, effect_, param, value); }
};

/**
 * fallback for anything without a wrapper
 */
class MeloUnknownEffect : public MeloEffect { public: using MeloEffect::MeloEffect; };

/**
 * algorithmic reverb. either psx or freeverb
 */
class MeloAlgorithmicReverb : public MeloEffect {
public:
    using MeloEffect::MeloEffect;
    static const char* TypeId() { return "Reverb"; }
    /**
     * algorithmic reverb. either psx or freeverb
     */
    ReverbAlgorithm Algorithm() const { return static_cast<ReverbAlgorithm>(Get("Algorithm")); }
    void Algorithm(ReverbAlgorithm value) const { Set("Algorithm", static_cast<float>(value)); }
    /**
     * 0-1, room size or PSX preset
     */
    float RoomSize() const { return Get("RoomSize"); }
    void RoomSize(float value) const { Set("RoomSize", value); }
    /**
     * 0-1, higher = darker
     */
    float Damping() const { return Get("Damping"); }
    void Damping(float value) const { Set("Damping", value); }
    /**
     * wet/dry 0-1
     */
    float Mix() const { return Get("Mix"); }
    void Mix(float value) const { Set("Mix", value); }
};
/**
 * stereo delay. SyncEnabled + SyncDivisionIndex for tempo locked repeats, otherwise Interval in ms
 */
class MeloDelay : public MeloEffect {
public:
    using MeloEffect::MeloEffect;
    static const char* TypeId() { return "Delay"; }
    /**
     * ms when sync is off, ~1-3000
     */
    float Interval() const { return Get("Interval"); }
    void Interval(float value) const { Set("Interval", value); }
    /**
     * ms for repeats to die out
     */
    float DecayTime() const { return Get("DecayTime"); }
    void DecayTime(float value) const { Set("DecayTime", value); }
    /**
     * wet/dry 0-1
     */
    float Mix() const { return Get("Mix"); }
    void Mix(float value) const { Set("Mix", value); }
    /**
     * alternate repeats left/right
     */
    bool PingPong() const { return static_cast<bool>(Get("PingPong")); }
    void PingPong(bool value) const { Set("PingPong", static_cast<float>(value)); }
    /**
     * hz where the feedback path rolls off, default 8000
     */
    float DampingFreq() const { return Get("DampingFreq"); }
    void DampingFreq(float value) const { Set("DampingFreq", value); }
    /**
     * true = ignore Interval and track tempo via SyncDivisionIndex
     */
    bool SyncEnabled() const { return static_cast<bool>(Get("SyncEnabled")); }
    void SyncEnabled(bool value) const { Set("SyncEnabled", static_cast<float>(value)); }
    /**
     * 3 = 1/4 by default
     */
    int SyncDivisionIndex() const { return static_cast<int>(Get("SyncDivisionIndex")); }
    void SyncDivisionIndex(int value) const { Set("SyncDivisionIndex", static_cast<float>(value)); }
};
/**
 * stereo chorus
 */
class MeloChorus : public MeloEffect {
public:
    using MeloEffect::MeloEffect;
    static const char* TypeId() { return "Chorus"; }
    /**
     * hz, 0.1-5
     */
    float Rate() const { return Get("Rate"); }
    void Rate(float value) const { Set("Rate", value); }
    /**
     * ms, 0.5-10
     */
    float Depth() const { return Get("Depth"); }
    void Depth(float value) const { Set("Depth", value); }
    /**
     * wet/dry 0-1
     */
    float Mix() const { return Get("Mix"); }
    void Mix(float value) const { Set("Mix", value); }
    /**
     * 0-0.8, higher goes flangery
     */
    float Feedback() const { return Get("Feedback"); }
    void Feedback(float value) const { Set("Feedback", value); }
    /**
     * 1-3
     */
    int Voices() const { return static_cast<int>(Get("Voices")); }
    void Voices(int value) const { Set("Voices", static_cast<float>(value)); }
};
/**
 * resonant filter with env follower + lfo
 */
class MeloFilter : public MeloEffect {
public:
    using MeloEffect::MeloEffect;
    static const char* TypeId() { return "Filter"; }
    MeloFilterType Type() const { return static_cast<MeloFilterType>(Get("Type")); }
    void Type(MeloFilterType value) const { Set("Type", static_cast<float>(value)); }
    /**
     * hz, 20-20000. lower = darker
     */
    float Cutoff() const { return Get("Cutoff"); }
    void Cutoff(float value) const { Set("Cutoff", value); }
    /**
     * Q, 0.5-20. high values whistle and can self oscillate
     */
    float Resonance() const { return Get("Resonance"); }
    void Resonance(float value) const { Set("Resonance", value); }
    /**
     * wet/dry 0-1, default 1
     */
    float Mix() const { return Get("Mix"); }
    void Mix(float value) const { Set("Mix", value); }
    /**
     * env follower amount in semitones, 0-48
     */
    float EnvAmount() const { return Get("EnvAmount"); }
    void EnvAmount(float value) const { Set("EnvAmount", value); }
    /**
     * seconds
     */
    float EnvAttack() const { return Get("EnvAttack"); }
    void EnvAttack(float value) const { Set("EnvAttack", value); }
    /**
     * seconds
     */
    float EnvRelease() const { return Get("EnvRelease"); }
    void EnvRelease(float value) const { Set("EnvRelease", value); }
    /**
     * hz, 0.05-20
     */
    float LfoRate() const { return Get("LfoRate"); }
    void LfoRate(float value) const { Set("LfoRate", value); }
    /**
     * semitones of cutoff, 0-48. 0 = lfo off
     */
    float LfoDepth() const { return Get("LfoDepth"); }
    void LfoDepth(float value) const { Set("LfoDepth", value); }
    /**
     * true = ignore LfoRate and sync to tempo via LfoSyncDivisionIndex
     */
    bool LfoSyncEnabled() const { return static_cast<bool>(Get("LfoSyncEnabled")); }
    void LfoSyncEnabled(bool value) const { Set("LfoSyncEnabled", static_cast<float>(value)); }
    /**
     * 5 = 1/4 by default
     */
    int LfoSyncDivisionIndex() const { return static_cast<int>(Get("LfoSyncDivisionIndex")); }
    void LfoSyncDivisionIndex(int value) const { Set("LfoSyncDivisionIndex", static_cast<float>(value)); }
    /**
     * live env value 0-1
     */
    float EnvelopeValue() const { return Get("EnvelopeValue"); }
    /**
     * live lfo value -1..1
     */
    float LfoValue() const { return Get("LfoValue"); }
};
/**
 * 3 band, low shelf / mid peak / high shelf
 */
class MeloEq : public MeloEffect {
public:
    using MeloEffect::MeloEffect;
    static const char* TypeId() { return "EQ"; }
    /**
     * hz
     */
    float LowFreq() const { return Get("LowFreq"); }
    void LowFreq(float value) const { Set("LowFreq", value); }
    /**
     * db, -12..12
     */
    float LowGain() const { return Get("LowGain"); }
    void LowGain(float value) const { Set("LowGain", value); }
    /**
     * hz
     */
    float MidFreq() const { return Get("MidFreq"); }
    void MidFreq(float value) const { Set("MidFreq", value); }
    /**
     * db, -12..12
     */
    float MidGain() const { return Get("MidGain"); }
    void MidGain(float value) const { Set("MidGain", value); }
    /**
     * 0.1-10, higher = narrower
     */
    float MidQ() const { return Get("MidQ"); }
    void MidQ(float value) const { Set("MidQ", value); }
    /**
     * hz
     */
    float HighFreq() const { return Get("HighFreq"); }
    void HighFreq(float value) const { Set("HighFreq", value); }
    /**
     * db, -12..12
     */
    float HighGain() const { return Get("HighGain"); }
    void HighGain(float value) const { Set("HighGain", value); }
};
class MeloEcho : public MeloEffect {
public:
    using MeloEffect::MeloEffect;
    static const char* TypeId() { return "SnesEcho"; }
    float Size() const { return Get("Size"); }
    void Size(float value) const { Set("Size", value); }
    float Feedback() const { return Get("Feedback"); }
    void Feedback(float value) const { Set("Feedback", value); }
    float Mix() const { return Get("Mix"); }
    void Mix(float value) const { Set("Mix", value); }
};
/**
 * impulses response reverb, heavier proccessing than algorithmic reverb
 */
class MeloConvolutionReverb : public MeloEffect {
public:
    using MeloEffect::MeloEffect;
    static const char* TypeId() { return "ConvolutionReverb"; }
    /**
     * wet/dry 0-1
     */
    float Mix() const { return Get("Mix"); }
    void Mix(float value) const { Set("Mix", value); }
    /**
     * set in melowrite before export
     */
    std::string IrName() const { return detail::Text([&](char* b, int n) { return melo_effect_ir_name(instance_, owner_, bus_, effect_, b, n); }); }
};
class MeloSpatial : public MeloEffect {
public:
    using MeloEffect::MeloEffect;
    static const char* TypeId() { return "Spatial"; }
    float Forward() const { return Get("Forward"); }
    void Forward(float value) const { Set("Forward", value); }
    float Up() const { return Get("Up"); }
    void Up(float value) const { Set("Up", value); }
    float Distance() const { return Get("Distance"); }
    void Distance(float value) const { Set("Distance", value); }
    float MinDist() const { return Get("MinDist"); }
    void MinDist(float value) const { Set("MinDist", value); }
    float Falloff() const { return Get("Falloff"); }
    void Falloff(float value) const { Set("Falloff", value); }
    float Blend() const { return Get("Blend"); }
    void Blend(float value) const { Set("Blend", value); }
    void SetPositions(float listenerX, float listenerY, float listenerZ, float sourceX, float sourceY, float sourceZ) const { melo_effect_spatial_set_positions(instance_, owner_, bus_, effect_, listenerX, listenerY, listenerZ, sourceX, sourceY, sourceZ); }
    void SetPositions(float listenerX, float listenerY, float sourceX, float sourceY) const { melo_effect_spatial_set_positions_2d(instance_, owner_, bus_, effect_, listenerX, listenerY, sourceX, sourceY); }
};

/**
 * handle to one track, cheap to hold or refetch. from MeloInstance::Track()
 */
class MeloTrack {
protected:
    MeloInstanceHandle instance_ = nullptr;
    int index_ = -1, bus_ = 0;
public:
    MeloTrack() = default;
    MeloTrack(MeloInstanceHandle instance, int index, int bus = 0) : instance_(instance), index_(index), bus_(bus) {}
    /**
     * index in the project: top in app = 0
     */
    int Index() const { return index_; }
    bool IsValid() const { return melo_owner_is_valid(instance_, index_, bus_) != 0; }
    std::string Name() const { return detail::Text([&](char* b, int n) { return melo_owner_name(instance_, index_, bus_, b, n); }); }
    /**
     * 0-1
     */
    float Volume() const { return melo_owner_get_volume(instance_, index_, bus_); }
    void Volume(float value) const { melo_owner_set_volume(instance_, index_, bus_, value); }
    /**
     * -1 left, 0 center, 1 right
     */
    float Pan() const { return melo_owner_get_pan(instance_, index_, bus_); }
    void Pan(float value) const { melo_owner_set_pan(instance_, index_, bus_, value); }
    bool Muted() const { return melo_owner_get_muted(instance_, index_, bus_) != 0; }
    void Muted(bool value) const { melo_owner_set_muted(instance_, index_, bus_, value); }
    bool Solo() const { return melo_owner_get_solo(instance_, index_, bus_) != 0; }
    void Solo(bool value) const { melo_owner_set_solo(instance_, index_, bus_, value); }
    /**
     * send to bus A, 0-1
     */
    float SendA() const { return melo_owner_get_senda(instance_, index_, bus_); }
    void SendA(float value) const { melo_owner_set_senda(instance_, index_, bus_, value); }
    /**
     * send to bus B, 0-1
     */
    float SendB() const { return melo_owner_get_sendb(instance_, index_, bus_); }
    void SendB(float value) const { melo_owner_set_sendb(instance_, index_, bus_, value); }
    /**
     * instrument settings as json, nullptr clears it
     */
    void SetInstrument(const char* instrumentJson) const { melo_track_set_instrument_json(instance_, index_, instrumentJson); }
    std::string Instrument() const { return detail::Text([&](char* b, int n) { return melo_track_instrument_json(instance_, index_, b, n); }); }

    // -- Effects --
    int EffectCount() const { return melo_owner_effect_count(instance_, index_, bus_); }
    std::vector<MeloEffect> Effects() const { std::vector<MeloEffect> values; for (int i = 0; i < EffectCount(); ++i) values.emplace_back(instance_, index_, bus_, i); return values; }
    template<class T> T GetEffect() const { return T(instance_, index_, bus_, melo_owner_find_effect(instance_, index_, bus_, T::TypeId())); }
    template<class T> bool TryGetEffect(T& effect) const { effect = GetEffect<T>(); return effect.IsValid(); }
    template<class T> bool HasEffect() const { return GetEffect<T>().IsValid(); }
    /**
     * appends to the chain
     */
    template<class T> T AddEffect() const { return T(instance_, index_, bus_, melo_owner_add_effect(instance_, index_, bus_, T::TypeId())); }
    /**
     * removes the first T, true if it found one
     */
    template<class T> bool RemoveEffect() const { return melo_owner_remove_effect(instance_, index_, bus_, T::TypeId()) != 0; }
};

/**
 * handle to one bus. shared send destinations (reverb, delay, master). same effect access as MeloTrack
 */
class MeloBus : public MeloTrack {
private:
    using MeloTrack::SetInstrument;
    using MeloTrack::Instrument;
    using MeloTrack::SendA;
    using MeloTrack::SendB;
public:
    MeloBus() = default;
    MeloBus(MeloInstanceHandle instance, int index) : MeloTrack(instance, index, 1) {}
    /**
     * 0 is master
     */
    int Index() const { return index_; }
};

class MeloMix {
    MeloInstanceHandle handle_ = nullptr;
public:
    MeloMix() = default;
    explicit MeloMix(MeloInstanceHandle handle) : handle_(handle) {}
    /**
     * false after UnloadMix
     */
    bool IsLoaded() const { return melo_instance_is_loaded(handle_) != 0; }
    /**
     * the mix's master bus
     */
    MeloBus Master() const { return MeloBus(handle_, melo_instance_master_bus_index(handle_)); }
    std::vector<std::string> GameParamNames() const { std::vector<std::string> values; for (int i = 0; i < melo_instance_game_param_count(handle_); ++i) values.push_back(detail::Text([&](char* b, int n) { return melo_instance_game_param_name(handle_, i, b, n); })); return values; }
    bool SetGameParam(const char* name, float value) const { return melo_instance_set_game_param(handle_, name, value) != 0; }
    bool ReleaseGameParam(const char* name) const { return melo_instance_release_game_param(handle_, name) != 0; }
    float GetGameParam(const char* name) const { return melo_instance_get_game_param(handle_, name); }
};

/**
 * a loaded project, from Melo::Load
 */
class MeloInstance {
    MeloInstanceHandle handle_ = nullptr;
public:
    MeloInstance() = default;
    explicit MeloInstance(MeloInstanceHandle handle) : handle_(handle) {}
    MeloInstanceHandle NativeHandle() const { return handle_; }
    /**
     * native engine handle, zero after Unload
     */
    MeloInstanceHandle Engine() const { return melo_instance_engine(handle_); }
    /**
     * chunk names
     */
    std::vector<std::string> ChunkNames() const { std::vector<std::string> values; for (int i = 0; i < ChunkCount(); ++i) values.push_back(detail::Text([&](char* b, int n) { return melo_instance_chunk_name(handle_, i, b, n); })); return values; }
    std::vector<std::string> TrackNames() const { std::vector<std::string> values; for (int i = 0; i < TrackCount(); ++i) values.push_back(detail::Text([&](char* b, int n) { return melo_instance_track_name(handle_, i, b, n); })); return values; }
    std::vector<std::string> BusNames() const { std::vector<std::string> values; for (int i = 0; i < melo_instance_bus_count(handle_); ++i) values.push_back(detail::Text([&](char* b, int n) { return melo_instance_bus_name(handle_, i, b, n); })); return values; }
    std::vector<std::string> GetPaletteNames() const { std::vector<std::string> values; for (int i = 0; i < melo_instance_palette_count(handle_); ++i) values.push_back(detail::Text([&](char* b, int n) { return melo_instance_palette_name(handle_, i, b, n); })); return values; }
    bool IsLoaded() const { return melo_instance_is_loaded(handle_) != 0; }
    std::string File() const { return detail::Text([&](char* b, int n) { return melo_instance_file(handle_, b, n); }); }
    std::string ActiveChunk() const { return detail::Text([&](char* b, int n) { return melo_instance_active_chunk(handle_, b, n); }); }
    int CurrentBar() const { return melo_instance_current_bar(handle_); }
    int CurrentBeat() const { return melo_instance_current_beat(handle_); }
    int Tempo() const { return melo_instance_tempo(handle_); }
    int ChunkCount() const { return melo_instance_chunk_count(handle_); }
    int TrackCount() const { return melo_instance_track_count(handle_); }
    /**
     * play a chunk, nullptr = chunk 0. fadeOut in seconds
     */
    void PlayChunk(const char* chunk = nullptr, bool looping = true, float fadeOut = 0) const { melo_instance_play_chunk(handle_, chunk, looping, fadeOut); }
    void PlayChunk(int index, bool looping = true, float fadeOut = 0) const { melo_instance_play_chunk_index(handle_, index, looping, fadeOut); }
    /**
     * crossfade to a chunk over duration seconds
     */
    void PlayChunkCrossfade(const char* chunk, float duration, bool looping = true) const { melo_instance_play_chunk_crossfade(handle_, chunk, duration, looping); }
    void PlayChunkCrossfade(int index, float duration, bool looping = true) const { melo_instance_play_chunk_crossfade_index(handle_, index, duration, looping); }
    /**
     * play the arrangement. fadeOut fades the current music first, in seconds
     */
    void PlayArrangement(float fadeOut = 0) const { melo_instance_play_arrangement(handle_, fadeOut); }
    /**
     * crossfade into the arrangement over duration seconds
     */
    void PlayArrangementCrossfade(float duration) const { melo_instance_play_arrangement_crossfade(handle_, duration); }
    /**
     * play a chunk once on top of the current music. cleans itself up when done
     */
    void PlayChunkHeadless(const char* chunk = nullptr) const { melo_instance_play_chunk_headless(handle_, chunk); }
    void PlayChunkHeadless(int index) const { melo_instance_play_chunk_headless_index(handle_, index); }
    /**
     * stop, tails ring out. stays loaded
     */
    void Stop() const { melo_instance_stop(handle_); }
    void Pause() const { melo_instance_pause(handle_); }
    void Resume() const { melo_instance_resume(handle_); }
    /**
     * fade out over duration seconds, 0 stops right away
     */
    void FadeOut(float duration = 1) const { melo_instance_fade_out(handle_, duration); }
    /**
     * switch chunks now, on the next beat, next bar or when the loop ends
     */
    void SwitchChunk(const char* chunk, MeloSwitch when = MeloSwitch::Bar) const { melo_instance_switch_chunk(handle_, chunk, static_cast<int>(when)); }
    void SwitchChunk(int index, MeloSwitch when = MeloSwitch::Bar) const { melo_instance_switch_chunk_index(handle_, index, static_cast<int>(when)); }
    /**
     * switch to another song. no start chunk = its arrangement. fadeOut in seconds
     */
    void SwitchSong(const char* projectPath, MeloSwitch when = MeloSwitch::Bar, float fadeOut = 0) const { melo_instance_switch_song(handle_, projectPath, static_cast<int>(when), fadeOut); }
    /**
     * start on a named chunk
     */
    void SwitchSong(const char* projectPath, const char* startChunk, MeloSwitch when = MeloSwitch::Bar, float fadeOut = 0) const { melo_instance_switch_song_at_name(handle_, projectPath, startChunk, static_cast<int>(when), fadeOut); }
    /**
     * start on a chunk index
     */
    void SwitchSong(const char* projectPath, int startChunk, MeloSwitch when = MeloSwitch::Bar, float fadeOut = 0) const { melo_instance_switch_song_at_index(handle_, projectPath, startChunk, static_cast<int>(when), fadeOut); }
    /**
     * switch to a song you already loaded, skips the decode
     */
    void SwitchSong(MeloInstance other, MeloSwitch when = MeloSwitch::Bar, float fadeOut = 0) const { melo_instance_switch_song_loaded(handle_, other.handle_, static_cast<int>(when), fadeOut); }
    void SwitchSong(MeloInstance other, const char* startChunk, MeloSwitch when = MeloSwitch::Bar, float fadeOut = 0) const { melo_instance_switch_song_loaded_at_name(handle_, other.handle_, startChunk, static_cast<int>(when), fadeOut); }
    void SwitchSong(MeloInstance other, int startChunk, MeloSwitch when = MeloSwitch::Bar, float fadeOut = 0) const { melo_instance_switch_song_loaded_at_index(handle_, other.handle_, startChunk, static_cast<int>(when), fadeOut); }
    void SwitchSongCrossfade(const char* projectPath, float duration, MeloSwitch when = MeloSwitch::Bar) const { melo_instance_switch_song_crossfade(handle_, projectPath, duration, static_cast<int>(when)); }
    void SwitchSongCrossfade(const char* projectPath, const char* startChunk, float duration, MeloSwitch when = MeloSwitch::Bar) const { melo_instance_switch_song_crossfade_at_name(handle_, projectPath, startChunk, duration, static_cast<int>(when)); }
    void SwitchSongCrossfade(const char* projectPath, int startChunk, float duration, MeloSwitch when = MeloSwitch::Bar) const { melo_instance_switch_song_crossfade_at_index(handle_, projectPath, startChunk, duration, static_cast<int>(when)); }
    void SwitchSongCrossfade(MeloInstance other, float duration, MeloSwitch when = MeloSwitch::Bar) const { melo_instance_switch_song_crossfade_loaded(handle_, other.handle_, duration, static_cast<int>(when)); }
    void SwitchSongCrossfade(MeloInstance other, const char* startChunk, float duration, MeloSwitch when = MeloSwitch::Bar) const { melo_instance_switch_song_crossfade_loaded_at_name(handle_, other.handle_, startChunk, duration, static_cast<int>(when)); }
    void SwitchSongCrossfade(MeloInstance other, int startChunk, float duration, MeloSwitch when = MeloSwitch::Bar) const { melo_instance_switch_song_crossfade_loaded_at_index(handle_, other.handle_, startChunk, duration, static_cast<int>(when)); }
    /**
     * cancel a switch that hasnt happened yet
     */
    void CancelSwitch() const { melo_instance_cancel_switch(handle_); }
    void SeekToBar(int bar) const { melo_instance_seek_bar(handle_, bar); }
    void SetTempo(int bpm) const { melo_instance_set_tempo(handle_, bpm); }
    void SetMasterVolume(float volume) const { melo_instance_set_master_volume(handle_, volume); }
    /**
     * -1 left, 0 center, 1 right
     */
    void SetPan(float pan) const { melo_instance_set_pan(handle_, pan); }
    void SetTrackMuted(const char* track, bool value) const { melo_instance_set_track_muted(handle_, track, value); }
    void SetTrackMuted(int track, bool value) const { melo_instance_set_track_muted_index(handle_, track, value); }
    void SetTrackVolume(const char* track, float value) const { melo_instance_set_track_volume(handle_, track, value); }
    void SetTrackVolume(int track, float value) const { melo_instance_set_track_volume_index(handle_, track, value); }
    /**
     * send to bus A or B, 0-1
     */
    void SetSendA(const char* track, float level) const { melo_instance_set_send_a(handle_, track, level); }
    void SetSendA(int track, float level) const { melo_instance_set_send_a_index(handle_, track, level); }
    void SetSendB(const char* track, float level) const { melo_instance_set_send_b(handle_, track, level); }
    void SetSendB(int track, float level) const { melo_instance_set_send_b_index(handle_, track, level); }
    void SetBusVolume(int bus, float volume) const { melo_instance_set_bus_volume(handle_, bus, volume); }
    bool SetGameParam(const char* name, float value) const { return melo_instance_set_game_param(handle_, name, value) != 0; }
    bool ReleaseGameParam(const char* name) const { return melo_instance_release_game_param(handle_, name) != 0; }
    float GetGameParam(const char* name) const { return melo_instance_get_game_param(handle_, name); }
    std::vector<std::string> GameParamNames() const { std::vector<std::string> values; for (int i = 0; i < melo_instance_game_param_count(handle_); ++i) values.push_back(detail::Text([&](char* b, int n) { return melo_instance_game_param_name(handle_, i, b, n); })); return values; }
    void SwapPalette(const char* palette) const { melo_instance_swap_palette(handle_, palette); }
    /**
     * start a midi note, velocity 0-1
     */
    void NoteOn(int track, int midiNote, float velocity = 1) const { melo_instance_note_on(handle_, track, midiNote, velocity); }
    /**
     * stop a midi note
     */
    void NoteOff(int track, int midiNote) const { melo_instance_note_off(handle_, track, midiNote); }
    /**
     * by name, check IsValid
     */
    MeloTrack Track(const char* name) const { return MeloTrack(handle_, melo_instance_track_index(handle_, name)); }
    /**
     * by index, check IsValid
     */
    MeloTrack Track(int index) const { return MeloTrack(handle_, index); }
    /**
     * track handles in project order
     */
    std::vector<MeloTrack> Tracks() const { std::vector<MeloTrack> values; for (int i = 0; i < TrackCount(); ++i) values.emplace_back(handle_, i); return values; }
    /**
     * bus by name, check IsValid
     */
    MeloBus Bus(const char* name) const { return MeloBus(handle_, melo_instance_bus_index(handle_, name)); }
    /**
     * bus by index, 0 is master
     */
    MeloBus Bus(int index) const { return MeloBus(handle_, index); }
    /**
     * the master bus can also be found with Bus(0)
     */
    MeloBus Master() const { return MeloBus(handle_, melo_instance_master_bus_index(handle_)); }
    std::vector<MeloBus> AllBuses() const { std::vector<MeloBus> values; for (int i = 0; i < melo_instance_bus_count(handle_); ++i) values.emplace_back(handle_, i); return values; }
    /**
     * queue a command for the audio thread
     */
    void RunOnAudioThread(MeloAudioCommand command, void* userData = nullptr) const { melo_instance_run_on_audio_thread(handle_, command, userData); }
    /**
     * run code on the main thread at the next beat, bar or loop. fires from MeloHost::Tick
     */
    void RunAt(MeloSwitch when, MeloTimedCallback callback, void* userData = nullptr) const { melo_instance_run_at(handle_, static_cast<int>(when), callback, userData); }
    /**
     * audio thread version, at the next beat, bar or loop. keep it quick
     */
    void RunOnAudioThreadAt(MeloSwitch when, MeloAudioCommand command, void* userData = nullptr) const { melo_instance_run_on_audio_thread_at(handle_, static_cast<int>(when), command, userData); }
    /**
     * drop any RunAt and RunOnAudioThreadAt still waiting. switches already queued stay
     */
    void CancelRunAt() const { melo_instance_cancel_run_at(handle_); }
    /**
     * unload this song. Melo::UnloadAll also clears the shared caches
     */
    void Unload() { melo_instance_unload(handle_); handle_ = nullptr; }
    /**
     * instance-specific second global mix project, with parameters and all that, the loaded song passes through. stays until UnloadMix
     */
    MeloMix LoadMix(const char* projectPath) const { return MeloMix(melo_instance_load_mix(handle_, projectPath)); }
    /**
     * removes the instance's second global mix project
     */
    void UnloadMix() const { melo_instance_unload_mix(handle_); }

    // -- Effects --
    /**
     * e.g. SetBusEffectParam(1, 0, "Mix", 0.5f)
     */
    void SetBusEffectParam(int bus, int effect, const char* param, float value) const { melo_instance_set_bus_effect_param(handle_, bus, effect, param, value); }
    /**
     * same thing for a track effect
     */
    void SetTrackEffectParam(int track, int effect, const char* param, float value) const { melo_instance_set_track_effect_param(handle_, track, effect, param, value); }

    // -- Pads --
    /**
     * hit a pad by name. velocity 0-1, pan -1 to 1. hits overlap
     */
    void TriggerPad(const char* track, const char* pad, float velocity = 1, float pan = 0) const { melo_instance_trigger_pad(handle_, track, pad, velocity, pan); }
    /**
     * preallocate independent spatial voices before gameplay
     */
    bool PrepareSpatialPad(const char* track, const char* pad, int voices = 16) const { return melo_instance_prepare_spatial_pad(handle_, track, pad, voices) != 0; }
    /**
     * play a pad at a world position. returns a unique playback handle, or -1 if unavailable
     */
    int TriggerSpatialPad(const char* track, const char* pad, float x, float y, float z, float velocity = 1) const { return melo_instance_trigger_spatial_pad(handle_, track, pad, x, y, z, velocity); }
    int HoldSpatialPad(const char* track, const char* pad, float x, float y, float z, float velocity = 1) const { return melo_instance_hold_spatial_pad(handle_, track, pad, x, y, z, velocity); }
    /**
     * move only this spatial playback; completed handles are ignored
     */
    void MoveSpatialPad(int voice, float x, float y, float z) const { melo_instance_move_spatial_pad(handle_, voice, x, y, z); }
    /**
     * release only this spatial playback
     */
    void ReleaseSpatialPad(int voice) const { melo_instance_release_spatial_pad(handle_, voice); }
    bool IsSpatialPadPlaying(int voice) const { return melo_instance_is_spatial_pad_playing(handle_, voice) != 0; }
    void TriggerPad(int track, const char* pad, float velocity = 1, float pan = 0) const { melo_instance_trigger_pad_index(handle_, track, pad, velocity, pan); }
    void SetPadHeld(const char* track, const char* pad, bool held = true, float velocity = 1, float pan = 0) const { melo_instance_set_pad_held(handle_, track, pad, held ? 1 : 0, velocity, pan); }
    void SetPadHeld(int track, const char* pad, bool held = true, float velocity = 1, float pan = 0) const { melo_instance_set_pad_held_index(handle_, track, pad, held ? 1 : 0, velocity, pan); }
    /**
     * let go of a held pad
     */
    void ReleasePadHeld(const char* track, const char* pad) const { melo_instance_release_pad_held(handle_, track, pad); }
    void ReleasePadHeld(int track, const char* pad) const { melo_instance_release_pad_held_index(handle_, track, pad); }
    /**
     * call every frame to keep a pad held. lets go about 0.12s after you stop calling
     */
    void HoldPadThisFrame(const char* track, const char* pad, float velocity = 1, float pan = 0) const { melo_instance_hold_pad_this_frame(handle_, track, pad, velocity, pan); }
    void HoldPadThisFrame(int track, const char* pad, float velocity = 1, float pan = 0) const { melo_instance_hold_pad_this_frame_index(handle_, track, pad, velocity, pan); }

    // -- SFX --
    /**
     * play a sound through this song. effects = through its master effects too
     */
    int PlayOneShot(const char* file, float volume = 1, float pan = 0, float pitch = 1, bool loop = false, bool effects = true) const { return melo_instance_play_one_shot(handle_, file, volume, pan, pitch, loop, effects); }
    /**
     * loops until StopSfxVoice(voice)
     */
    int PlayLoop(const char* file, float volume = 1, float pan = 0, float pitch = 1, bool effects = true) const { return melo_instance_play_loop(handle_, file, volume, pan, pitch, effects); }
    int PlaySpatialOneShot(const char* file, float x, float y, float z, float volume = 1, float pitch = 1, bool loop = false, bool effects = true) const { return melo_instance_play_spatial_one_shot(handle_, file, x, y, z, volume, pitch, loop, effects); }
    void MoveSpatialOneShot(int voice, float x, float y, float z) const { melo_instance_move_spatial_one_shot(handle_, voice, x, y, z); }
    /**
     * stop sound started with PlayOneShot or PlayLoop, both return ints for use with this call.
     */
    void StopSfxVoice(int voice) const { melo_instance_stop_voice(handle_, voice); }
    /**
     * stops all loops and one shots, music is unaffected
     */
    void StopSfx() const { melo_instance_stop_sfx(handle_); }
    void SetVolume(int voice, float value) const { melo_instance_voice_volume(handle_, voice, value); }
    void SetPan(int voice, float value) const { melo_instance_voice_pan(handle_, voice, value); }
    void SetPitch(int voice, float value) const { melo_instance_voice_pitch(handle_, voice, value); }
};

/**
 * hooks for whatever owns the audio device. all the processing happens in the native dll
 */
struct MeloHost {
    /**
     * call before loading any projects or playing any audio. sampleRate has to match the audio device (bad values fall back to 44100). to change it later stop audio and Shutdown first
     */
    static void Init(int sampleRate) { melo_host_init(sampleRate); }
    /**
     * maps a content path to a file on disk. passes it through by default
     */
    static void ResolvePath(MeloPathResolver callback, void* userData = nullptr) { melo_host_set_path_resolver(callback, userData); }
    /**
     * engine errors write here, stderr by default
     */
    static void LogError(MeloLogCallback callback, void* userData = nullptr) { melo_host_set_log_error(callback, userData); }
    /**
     * engine warnings write here, stdout by default
     */
    static void LogWarning(MeloLogCallback callback, void* userData = nullptr) { melo_host_set_log_warning(callback, userData); }
    /**
     * engine build string
     */
    static std::string Build() { return detail::Text([](char* b, int n) { return melo_host_build(b, n); }); }
    /**
     * output sample rate
     */
    static int SampleRate() { return melo_host_sample_rate(); }
    /**
     * audio thread. fills frames of interleaved stereo floats, handled by Unity plugin, MonoGame host and SDL host
     */
    static void FillBuffer(float* buffer, int frames) { melo_host_fill_buffer(buffer, frames); }
    /**
     * call once a frame. fires load callbacks, events and RunAt callbacks
     */
    static void Tick() { melo_host_tick(); }
    /**
     * frees the engine. stop the audio device first
     */
    static void Shutdown() { melo_host_shutdown(); }
};

/**
 * loads songs and plays sfx, start here
 */
struct Melo {
    /**
     * load an exported .melo
     */
    static MeloInstance Load(const char* projectPath) { return MeloInstance(melo_load_instance(projectPath)); }
    /**
     * load on a background thread. the callback comes through MeloHost::Tick
     */
    static void LoadAsync(const char* projectPath, MeloLoadCallback onLoaded = nullptr, void* userData = nullptr) { melo_load_async(projectPath, onLoaded, userData); }
    /**
     * master volume 0-1, music and sfx
     */
    static float MasterVolume() { return melo_get_volume(); }
    static void MasterVolume(float value) { melo_set_volume(value); }
    /**
     * listener pose in +X right, +Y up, -Z forward coordinates
     */
    static void SetSpatialListener(float x, float y, float z, float forwardX, float forwardY, float forwardZ, float upX, float upY, float upZ) { melo_set_spatial_listener(x, y, z, forwardX, forwardY, forwardZ, upX, upY, upZ); }
    static void SetSpatialDefaults(float minDist, float falloff, float blend, MeloSpatialMode mode) { melo_set_spatial_defaults(minDist, falloff, blend, static_cast<int>(mode)); }
    /**
     * stops all music. pads stay loaded
     */
    static void StopAll() { melo_stop_all(); }
    /**
     * unloads everything and clears the sound caches
     */
    static void UnloadAll() { melo_unload_all(); }
    /**
     * master mix project all audio routes through, one-shots and raw audio as well. stays until UnloadMix
     */
    static MeloMix LoadMix(const char* projectPath) { return MeloMix(melo_load_mix(projectPath)); }
    /**
     * removes global mix
     */
    static void UnloadMix() { melo_unload_mix(); }
    /**
     * every note played: instance, track index, midi pitch, velocity 0-127. fires from MeloHost::Tick
     */
    static void OnNote(MeloNoteCallback listener, void* userData = nullptr) { melo_on_note_add(listener, userData); }
    static void RemoveOnNote(MeloNoteCallback listener, void* userData = nullptr) { melo_on_note_remove(listener, userData); }
    /**
     * every beat: instance, bar, beat in the bar, both from 0. fires from MeloHost::Tick
     */
    static void OnBeat(MeloBeatCallback listener, void* userData = nullptr) { melo_on_beat_add(listener, userData); }
    static void RemoveOnBeat(MeloBeatCallback listener, void* userData = nullptr) { melo_on_beat_remove(listener, userData); }
    /**
     * every bar: instance, bar from 0. fires from MeloHost::Tick
     */
    static void OnBar(MeloBarCallback listener, void* userData = nullptr) { melo_on_bar_add(listener, userData); }
    static void RemoveOnBar(MeloBarCallback listener, void* userData = nullptr) { melo_on_bar_remove(listener, userData); }

    // -- SFX --
    /**
     * play an audio file, returns a voice id. pitch is a rate multiplier, 2 = up an octave
     */
    static int PlayOneShot(const char* file, float volume = 1, float pan = 0, float pitch = 1, bool loop = false, bool effects = true) { return melo_play_one_shot(file, volume, pan, pitch, loop, effects); }
    /**
     * loops until StopSfxVoice(voice)
     */
    static int PlayLoop(const char* file, float volume = 1, float pan = 0, float pitch = 1, bool effects = true) { return melo_play_loop(file, volume, pan, pitch, effects); }
    static int PlaySpatialOneShot(const char* file, float x, float y, float z, float volume = 1, float pitch = 1, bool loop = false, bool effects = true) { return melo_play_spatial_one_shot(file, x, y, z, volume, pitch, loop, effects); }
    static void MoveSpatialOneShot(int voice, float x, float y, float z) { melo_global_move_spatial_one_shot(voice, x, y, z); }
    /**
     * stop sound started with PlayOneShot or PlayLoop, both return ints for use with this call.
     */
    static void StopSfxVoice(int voice) { melo_global_stop_voice(voice); }
    /**
     * stops all loops and one shots, music is unaffected
     */
    static void StopSfx() { melo_global_stop_sfx(); }
    /**
     * true while the voice or its tail is still going
     */
    static bool IsPlaying(int voice) { return melo_global_voice_is_playing(voice) != 0; }
    static void SetVolume(int voice, float value) { melo_global_voice_volume(voice, value); }
    static void SetPan(int voice, float value) { melo_global_voice_pan(voice, value); }
    static void SetPitch(int voice, float value) { melo_global_voice_pitch(voice, value); }
    /**
     * sfx volume 0-1, on top of MasterVolume
     */
    static float SfxVolume() { return melo_get_sfx_volume(); }
    static void SfxVolume(float value) { melo_set_sfx_volume(value); }

    // -- Effects --
    static void SetReverb(float mix = 0.3f, float roomSize = 0.7f, float damping = 0.5f) { melo_set_reverb(mix, roomSize, damping); }
    static void SetDelay(float mix = 0.3f, float intervalMs = 350, float decayMs = 1500) { melo_set_delay(mix, intervalMs, decayMs); }
    static void ClearEffects() { melo_clear_effects(); }
    /**
     * copy a project bus's effects onto the sfx bus. wetMix 0-1 overrides every effect's mix, -1 leaves them
     */
    static void CopyEffectsFrom(MeloInstance instance, const char* busName, float wetMix = -1) { melo_copy_effects_from(instance.NativeHandle(), busName, wetMix); }
};
} // namespace Melowrite

#endif /* __cplusplus */
#endif /* MELO_H */
