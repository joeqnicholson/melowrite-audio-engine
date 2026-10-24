/* Melowrite C and C++ API. Link with Melowrite.Core.dll. */
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
typedef void (*MeloAudioCommand)(MeloInstanceHandle engine, void* user_data);
typedef void (*MeloLogCallback)(const char* message, void* user_data);
typedef const char* (*MeloPathResolver)(const char* path, void* user_data);
typedef int (*MeloPathResolverUtf8)(const char* path, char* buffer, int size, void* user_data);
typedef enum MeloSwitchTiming { MELO_SWITCH_NOW, MELO_SWITCH_BEAT, MELO_SWITCH_BAR, MELO_SWITCH_QUEUE } MeloSwitchTiming;
/**
 * Initialize on the main thread before loading content or starting audio.
 * The sample rate must match the output device. Defaults to 44100 if invalid.
 * To change the rate after initialization, stop audio and call Shutdown first.
 */
MELO_API void melo_host_init(int sample_rate);

/**
 * Current output sample rate.
 */
MELO_API int melo_host_sample_rate(void);

/**
 * Engine build identifier for host diagnostics.
 */
MELO_API int melo_host_build(char* buffer, int size);

/**
 * Audio thread: fill frames of interleaved stereo float samples.
 */
MELO_API int melo_host_fill_buffer(float* buffer, int frames);

/**
 * Main thread: deliver async load callbacks and note events once per frame.
 */
MELO_API void melo_host_tick(void);

/**
 * Release engine resources after stopping the audio device and callbacks.
 */
MELO_API void melo_host_shutdown(void);

/**
 * Map a content path to a disk path. Defaults to the supplied path.
 */
MELO_API void melo_host_set_path_resolver(MeloPathResolver callback, void* user_data);

/**
 * Map a content path to a disk path. Defaults to the supplied path.
 */
MELO_API void melo_host_set_path_resolver_utf8(MeloPathResolverUtf8 callback, void* user_data);

/**
 * Receives engine errors. Defaults to Console.Error.WriteLine.
 */
MELO_API void melo_host_set_log_error(MeloLogCallback callback, void* user_data);

/**
 * Receives engine warnings. Defaults to Console.WriteLine.
 */
MELO_API void melo_host_set_log_warning(MeloLogCallback callback, void* user_data);

/**
 * Load an exported .melo project.
 */
MELO_API MeloInstanceHandle melo_load_instance(const char* path);

/**
 * Load off the main thread. The callback is delivered by MeloHost.Tick.
 */
MELO_API void melo_load_async(const char* path, MeloLoadCallback callback, void* user_data);

/**
 * Master volume for all playback, from 0 to 1.
 */
MELO_API float melo_get_volume(void);

/**
 * Master volume for all playback, from 0 to 1.
 */
MELO_API void melo_set_volume(float value);

/**
 * Stop all music instances. Loaded pads remain available.
 */
MELO_API void melo_stop_all(void);

/**
 * Unload all projects and clear shared sound caches.
 */
MELO_API void melo_unload_all(void);

/**
 * Note events: instance, track index, MIDI pitch, velocity (0 to 127). Delivered by MeloHost.Tick.
 */
MELO_API void melo_on_note_add(MeloNoteCallback callback, void* user_data);

/**
 * Note events: instance, track index, MIDI pitch, velocity (0 to 127). Delivered by MeloHost.Tick.
 */
MELO_API void melo_on_note_remove(MeloNoteCallback callback, void* user_data);

/**
 * Play an audio file and return its voice ID. pitch is a playback-rate multiplier: 1 is normal, 2 is one octave up.
 */
MELO_API int melo_play_one_shot(const char* file, float volume, float pan, float pitch, int loop, int effects);

/**
 * Play an audio file in a loop until Stop(voice).
 */
MELO_API int melo_play_loop(const char* file, float volume, float pan, float pitch, int effects);

/**
 * Whether a global sound voice is still playing, including its release tail.
 */
MELO_API int melo_global_voice_is_playing(int voice);

MELO_API void melo_global_stop_voice(int voice);

MELO_API void melo_global_stop_sfx(void);

MELO_API void melo_global_voice_volume(int voice, float value);

MELO_API void melo_global_voice_pan(int voice, float value);

MELO_API void melo_global_voice_pitch(int voice, float value);

/**
 * Sound effect volume from 0 to 1. MasterVolume also applies.
 */
MELO_API float melo_get_sfx_volume(void);

/**
 * Sound effect volume from 0 to 1. MasterVolume also applies.
 */
MELO_API void melo_set_sfx_volume(float value);

MELO_API void melo_set_reverb(float mix, float room_size, float damping);

MELO_API void melo_set_delay(float mix, float interval_ms, float decay_ms);

MELO_API void melo_clear_effects(void);

/**
 * Copy a project bus effect chain to the SFX bus. wetMix >= 0 overrides each effect mix.
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
 * Play a chunk. Null selects chunk 0. fadeOut is in seconds.
 */
MELO_API void melo_instance_play_chunk(MeloInstanceHandle instance, const char* chunk, int looping, float fade_out);

/**
 * Crossfade to another chunk over duration seconds.
 */
MELO_API void melo_instance_play_chunk_crossfade(MeloInstanceHandle instance, const char* chunk, float duration, int looping);

/**
 * Play a chunk once alongside the current music. Releases its playback engine when finished.
 */
MELO_API void melo_instance_play_chunk_headless(MeloInstanceHandle instance, const char* chunk);

/**
 * Switch to a chunk at the requested musical boundary.
 */
MELO_API void melo_instance_switch_chunk(MeloInstanceHandle instance, const char* chunk, int when);

MELO_API void melo_instance_play_chunk_index(MeloInstanceHandle instance, int chunk, int looping, float fade_out);

MELO_API void melo_instance_play_chunk_crossfade_index(MeloInstanceHandle instance, int chunk, float duration, int looping);

MELO_API void melo_instance_play_chunk_headless_index(MeloInstanceHandle instance, int chunk);

MELO_API void melo_instance_switch_chunk_index(MeloInstanceHandle instance, int chunk, int when);

/**
 * Play the arrangement. fadeOut is the preceding fade in seconds.
 */
MELO_API void melo_instance_play_arrangement(MeloInstanceHandle instance, float fade_out);

/**
 * Crossfade into the arrangement over duration seconds.
 */
MELO_API void melo_instance_play_arrangement_crossfade(MeloInstanceHandle instance, float duration);

/**
 * Stop playback and let effect tails finish. The instance remains loaded.
 */
MELO_API void melo_instance_stop(MeloInstanceHandle instance);

MELO_API void melo_instance_pause(MeloInstanceHandle instance);

MELO_API void melo_instance_resume(MeloInstanceHandle instance);

/**
 * Fade out over duration seconds. Zero or less stops immediately.
 */
MELO_API void melo_instance_fade_out(MeloInstanceHandle instance, float duration);

/**
 * Cancel a switch that has not started.
 */
MELO_API void melo_instance_cancel_switch(MeloInstanceHandle instance);

MELO_API void melo_instance_seek_bar(MeloInstanceHandle instance, int bar);

MELO_API void melo_instance_set_tempo(MeloInstanceHandle instance, int bpm);

MELO_API void melo_instance_set_master_volume(MeloInstanceHandle instance, float volume);

/**
 * Instance pan: -1 left, 0 center, 1 right.
 */
MELO_API void melo_instance_set_pan(MeloInstanceHandle instance, float pan);

MELO_API void melo_instance_swap_palette(MeloInstanceHandle instance, const char* palette);

/**
 * Switch to another project. Without a start chunk, play its arrangement. fadeOut is in seconds.
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
 * Switch to a loaded project without decoding it again.
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
 * Send level for bus A or B, from 0 to 1.
 */
MELO_API void melo_instance_set_send_a(MeloInstanceHandle instance, const char* track, float value);

MELO_API void melo_instance_set_send_a_index(MeloInstanceHandle instance, int track, float value);

MELO_API void melo_instance_set_send_b(MeloInstanceHandle instance, const char* track, float value);

MELO_API void melo_instance_set_send_b_index(MeloInstanceHandle instance, int track, float value);

MELO_API void melo_instance_set_bus_volume(MeloInstanceHandle instance, int bus, float volume);

/**
 * e.g. SetBusEffectParam(1, 0, "Mix", 0.5f)
 */
MELO_API void melo_instance_set_bus_effect_param(MeloInstanceHandle instance, int owner, int effect, const char* param, float value);

/**
 * Set a track effect parameter by name.
 */
MELO_API void melo_instance_set_track_effect_param(MeloInstanceHandle instance, int owner, int effect, const char* param, float value);

/**
 * Trigger a named pad. Velocity is 0 to 1; pan is -1 to 1. Repeated calls overlap.
 */
MELO_API void melo_instance_trigger(MeloInstanceHandle instance, const char* track, const char* pad, float velocity, float pan);

MELO_API void melo_instance_trigger_index(MeloInstanceHandle instance, int track, const char* pad, float velocity, float pan);

/**
 * Hold a pad until Release is called.
 */
MELO_API void melo_instance_hold(MeloInstanceHandle instance, const char* track, const char* pad, float velocity, float pan);

/**
 * Hold a pad until Release is called.
 */
MELO_API void melo_instance_hold_index(MeloInstanceHandle instance, int track, const char* pad, float velocity, float pan);

/**
 * Call each frame to sustain a pad. It releases about 0.12 seconds after the last call.
 */
MELO_API void melo_instance_sustain(MeloInstanceHandle instance, const char* track, const char* pad, float velocity, float pan);

MELO_API void melo_instance_sustain_index(MeloInstanceHandle instance, int track, const char* pad, float velocity, float pan);

/**
 * Release a held pad.
 */
MELO_API void melo_instance_release(MeloInstanceHandle instance, const char* track, const char* pad);

/**
 * Start a MIDI note with velocity from 0 to 1.
 */
MELO_API void melo_instance_note_on(MeloInstanceHandle instance, int track, int midi_note, float velocity);

/**
 * Release a MIDI note.
 */
MELO_API void melo_instance_note_off(MeloInstanceHandle instance, int track, int midi_note);

/**
 * Play a sound through this instance. effects enables its master effects.
 */
MELO_API int melo_instance_play_one_shot(MeloInstanceHandle instance, const char* file, float volume, float pan, float pitch, int loop, int effects);

/**
 * loops until Stop(voice)
 */
MELO_API int melo_instance_play_loop(MeloInstanceHandle instance, const char* file, float volume, float pan, float pitch, int effects);

MELO_API void melo_instance_stop_voice(MeloInstanceHandle instance, int voice);

MELO_API void melo_instance_stop_sfx(MeloInstanceHandle instance);

MELO_API void melo_instance_voice_volume(MeloInstanceHandle instance, int voice, float value);

MELO_API void melo_instance_voice_pan(MeloInstanceHandle instance, int voice, float value);

MELO_API void melo_instance_voice_pitch(MeloInstanceHandle instance, int voice, float value);

/**
 * Unload this instance. Use Melo.UnloadAll to clear shared content caches.
 */
MELO_API void melo_instance_unload(MeloInstanceHandle instance);

/**
 * Queue a command to run on the audio thread.
 */
MELO_API void melo_instance_run_on_audio_thread(MeloInstanceHandle instance, MeloAudioCommand callback, void* user_data);

/**
 * The underlying engine, or null after Unload.
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

/**
 * the master bus can also be found with Bus(0)
 */
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
 * null clears it
 */
MELO_API void melo_track_set_instrument_json(MeloInstanceHandle instance, int track, const char* json);

MELO_API int melo_effect_is_valid(MeloInstanceHandle instance, int owner, int bus, int effect);

MELO_API int melo_effect_name(MeloInstanceHandle instance, int owner, int bus, int effect, char* buffer, int size);

MELO_API int melo_effect_type(MeloInstanceHandle instance, int owner, int bus, int effect, char* buffer, int size);

/**
 * false = bypassed
 */
MELO_API int melo_effect_get_enabled(MeloInstanceHandle instance, int owner, int bus, int effect);

/**
 * false = bypassed
 */
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

/**
 * rolling sample buffer
 */
MELO_API int melo_effect_buffer(MeloInstanceHandle instance, int owner, int bus, int effect, float* buffer, int size);

#ifdef __cplusplus
} /* extern "C" */
#include <string>
#include <vector>

namespace Melowrite {
namespace detail {
template<class Query> inline std::string Text(Query query) {
    int size = query(nullptr, 0);
    if (size <= 0) return {};
    std::string value(static_cast<size_t>(size) + 1, '\0');
    query(&value[0], size + 1);
    value.resize(static_cast<size_t>(size));
    return value;
}
}

/**
 * When a chunk or song switch takes effect.
 */

enum class MeloSwitch
{
        Now,     // immediately
        Beat,    // on the next beat
        Bar,     // on the next bar
        Queue    // when the current chunk finishes its loop
};
enum class ReverbAlgorithm { Freeverb, Psx };
enum class MeloFilterType { Lowpass, Highpass, Bandpass, Notch };

/* Native effect references retain their instance and owner indices. Re-fetch
 * track/bus/effect views after switching songs or changing an effect chain.
 * Instrument values cross the ABI as InstrumentDto JSON; no managed object
 * pointer is exposed. Visualizer buffers are copied into native-owned memory.
 */
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
    /**
 * raw effect for whatever the wrapper doesnt cover
 */

    MeloEffect Underlying() const { return *this; }
    /**
 * false = bypassed
 */

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
    /**
 * raw Effect -> typed wrapper
 */

    static MeloEffect Wrap(MeloEffect effect) { return effect; }
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
    float RoomSize() const { return static_cast<float>(Get("RoomSize")); }
    void RoomSize(float value) const { Set("RoomSize", static_cast<float>(value)); }
/**
 * 0-1, higher = darker
 */
    float Damping() const { return static_cast<float>(Get("Damping")); }
    void Damping(float value) const { Set("Damping", static_cast<float>(value)); }
/**
 * wet/dry 0-1
 */
    float Mix() const { return static_cast<float>(Get("Mix")); }
    void Mix(float value) const { Set("Mix", static_cast<float>(value)); }
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
    float Interval() const { return static_cast<float>(Get("Interval")); }
    void Interval(float value) const { Set("Interval", static_cast<float>(value)); }
/**
 * ms for repeats to die out
 */
    float DecayTime() const { return static_cast<float>(Get("DecayTime")); }
    void DecayTime(float value) const { Set("DecayTime", static_cast<float>(value)); }
/**
 * wet/dry 0-1
 */
    float Mix() const { return static_cast<float>(Get("Mix")); }
    void Mix(float value) const { Set("Mix", static_cast<float>(value)); }
/**
 * alternate repeats left/right
 */
    bool PingPong() const { return static_cast<bool>(Get("PingPong")); }
    void PingPong(bool value) const { Set("PingPong", static_cast<float>(value)); }
/**
 * hz where the feedback path rolls off, default 8000
 */
    float DampingFreq() const { return static_cast<float>(Get("DampingFreq")); }
    void DampingFreq(float value) const { Set("DampingFreq", static_cast<float>(value)); }
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
    float Rate() const { return static_cast<float>(Get("Rate")); }
    void Rate(float value) const { Set("Rate", static_cast<float>(value)); }
/**
 * ms, 0.5-10
 */
    float Depth() const { return static_cast<float>(Get("Depth")); }
    void Depth(float value) const { Set("Depth", static_cast<float>(value)); }
/**
 * wet/dry 0-1
 */
    float Mix() const { return static_cast<float>(Get("Mix")); }
    void Mix(float value) const { Set("Mix", static_cast<float>(value)); }
/**
 * 0-0.8, higher goes flangery
 */
    float Feedback() const { return static_cast<float>(Get("Feedback")); }
    void Feedback(float value) const { Set("Feedback", static_cast<float>(value)); }
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
    float Cutoff() const { return static_cast<float>(Get("Cutoff")); }
    void Cutoff(float value) const { Set("Cutoff", static_cast<float>(value)); }
/**
 * Q, 0.5-20. high values whistle and can self oscillate
 */
    float Resonance() const { return static_cast<float>(Get("Resonance")); }
    void Resonance(float value) const { Set("Resonance", static_cast<float>(value)); }
/**
 * wet/dry 0-1, default 1
 */
    float Mix() const { return static_cast<float>(Get("Mix")); }
    void Mix(float value) const { Set("Mix", static_cast<float>(value)); }
/**
 * env follower amount in semitones, 0-48
 */
    float EnvAmount() const { return static_cast<float>(Get("EnvAmount")); }
    void EnvAmount(float value) const { Set("EnvAmount", static_cast<float>(value)); }
/**
 * seconds
 */
    float EnvAttack() const { return static_cast<float>(Get("EnvAttack")); }
    void EnvAttack(float value) const { Set("EnvAttack", static_cast<float>(value)); }
/**
 * seconds
 */
    float EnvRelease() const { return static_cast<float>(Get("EnvRelease")); }
    void EnvRelease(float value) const { Set("EnvRelease", static_cast<float>(value)); }
/**
 * hz, 0.05-20
 */
    float LfoRate() const { return static_cast<float>(Get("LfoRate")); }
    void LfoRate(float value) const { Set("LfoRate", static_cast<float>(value)); }
/**
 * semitones of cutoff, 0-48. 0 = lfo off
 */
    float LfoDepth() const { return static_cast<float>(Get("LfoDepth")); }
    void LfoDepth(float value) const { Set("LfoDepth", static_cast<float>(value)); }
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
    float EnvelopeValue() const { return static_cast<float>(Get("EnvelopeValue")); }
/**
 * live lfo value -1..1
 */
    float LfoValue() const { return static_cast<float>(Get("LfoValue")); }
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
    float LowFreq() const { return static_cast<float>(Get("LowFreq")); }
    void LowFreq(float value) const { Set("LowFreq", static_cast<float>(value)); }
/**
 * db, -12..12
 */
    float LowGain() const { return static_cast<float>(Get("LowGain")); }
    void LowGain(float value) const { Set("LowGain", static_cast<float>(value)); }
/**
 * hz
 */
    float MidFreq() const { return static_cast<float>(Get("MidFreq")); }
    void MidFreq(float value) const { Set("MidFreq", static_cast<float>(value)); }
/**
 * db, -12..12
 */
    float MidGain() const { return static_cast<float>(Get("MidGain")); }
    void MidGain(float value) const { Set("MidGain", static_cast<float>(value)); }
/**
 * 0.1-10, higher = narrower
 */
    float MidQ() const { return static_cast<float>(Get("MidQ")); }
    void MidQ(float value) const { Set("MidQ", static_cast<float>(value)); }
/**
 * hz
 */
    float HighFreq() const { return static_cast<float>(Get("HighFreq")); }
    void HighFreq(float value) const { Set("HighFreq", static_cast<float>(value)); }
/**
 * db, -12..12
 */
    float HighGain() const { return static_cast<float>(Get("HighGain")); }
    void HighGain(float value) const { Set("HighGain", static_cast<float>(value)); }
};
class MeloEcho : public MeloEffect {
public:
    using MeloEffect::MeloEffect;
    static const char* TypeId() { return "SnesEcho"; }
    float Size() const { return static_cast<float>(Get("Size")); }
    void Size(float value) const { Set("Size", static_cast<float>(value)); }
    float Feedback() const { return static_cast<float>(Get("Feedback")); }
    void Feedback(float value) const { Set("Feedback", static_cast<float>(value)); }
    float Mix() const { return static_cast<float>(Get("Mix")); }
    void Mix(float value) const { Set("Mix", static_cast<float>(value)); }
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
    float Mix() const { return static_cast<float>(Get("Mix")); }
    void Mix(float value) const { Set("Mix", static_cast<float>(value)); }
/**
 * set in melowrite before export
 */
    std::string IrName() const { return detail::Text([&](char* b, int n) { return melo_effect_ir_name(instance_, owner_, bus_, effect_, b, n); }); }
};
/**
 * pass through tap for visualizers, doesnt touch the signal
 */
class MeloVisualizer : public MeloEffect {
public:
    using MeloEffect::MeloEffect;
    static const char* TypeId() { return "Visualizer"; }
/**
 * 0.01-1, lower = smoother
 */
    float WaveLerpSpeed() const { return static_cast<float>(Get("WaveLerpSpeed")); }
    void WaveLerpSpeed(float value) const { Set("WaveLerpSpeed", static_cast<float>(value)); }
/**
 * rolling sample buffer
 */
    std::vector<float> Buffer() const { int n = melo_effect_buffer(instance_, owner_, bus_, effect_, nullptr, 0); std::vector<float> v(n); if (n) melo_effect_buffer(instance_, owner_, bus_, effect_, v.data(), n); return v; }
};


/**
 * handle to one track, cheap to hold or refetch. from MeloInstance.Track()
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
 * null clears it
 */

    void SetInstrument(const char* instrumentJson) const { melo_track_set_instrument_json(instance_, index_, instrumentJson); }
    std::string Instrument() const { return detail::Text([&](char* b, int n) { return melo_track_instrument_json(instance_, index_, b, n); }); }
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

/**
 * A loaded project. Create it with Melo.Load.
 */

class MeloInstance {
    MeloInstanceHandle handle_ = nullptr;
public:
    MeloInstance() = default;
    explicit MeloInstance(MeloInstanceHandle handle) : handle_(handle) {}
    MeloInstanceHandle NativeHandle() const { return handle_; }
    /**
 * The underlying engine, or null after Unload.
 */

    MeloInstanceHandle Engine() const { return melo_instance_engine(handle_); }
    /**
 * Names of the chunks in the loaded project.
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
 * Play a chunk. Null selects chunk 0. fadeOut is in seconds.
 */

    void PlayChunk(const char* chunk = nullptr, bool looping = true, float fadeOut = 0) const { melo_instance_play_chunk(handle_, chunk, looping, fadeOut); }
    
    void PlayChunk(int index, bool looping = true, float fadeOut = 0) const { melo_instance_play_chunk_index(handle_, index, looping, fadeOut); }
    /**
 * Crossfade to another chunk over duration seconds.
 */

    void PlayChunkCrossfade(const char* chunk, float duration, bool looping = true) const { melo_instance_play_chunk_crossfade(handle_, chunk, duration, looping); }
    
    void PlayChunkCrossfade(int index, float duration, bool looping = true) const { melo_instance_play_chunk_crossfade_index(handle_, index, duration, looping); }
    /**
 * Play the arrangement. fadeOut is the preceding fade in seconds.
 */

    void PlayArrangement(float fadeOut = 0) const { melo_instance_play_arrangement(handle_, fadeOut); }
    /**
 * Crossfade into the arrangement over duration seconds.
 */

    void PlayArrangementCrossfade(float duration) const { melo_instance_play_arrangement_crossfade(handle_, duration); }
    /**
 * Play a chunk once alongside the current music. Releases its playback engine when finished.
 */

    void PlayChunkHeadless(const char* chunk = nullptr) const { melo_instance_play_chunk_headless(handle_, chunk); }
    
    void PlayChunkHeadless(int index) const { melo_instance_play_chunk_headless_index(handle_, index); }
    /**
 * Stop playback and let effect tails finish. The instance remains loaded.
 */

    void Stop() const { melo_instance_stop(handle_); }
    void Pause() const { melo_instance_pause(handle_); }
    void Resume() const { melo_instance_resume(handle_); }
    /**
 * Fade out over duration seconds. Zero or less stops immediately.
 */

    void FadeOut(float duration = 1) const { melo_instance_fade_out(handle_, duration); }
    /**
 * Switch to a chunk at the requested musical boundary.
 */

    void SwitchChunk(const char* chunk, MeloSwitch when = MeloSwitch::Bar) const { melo_instance_switch_chunk(handle_, chunk, static_cast<int>(when)); }
    void SwitchChunk(int index, MeloSwitch when = MeloSwitch::Bar) const { melo_instance_switch_chunk_index(handle_, index, static_cast<int>(when)); }
    /**
 * Switch to another project. Without a start chunk, play its arrangement. fadeOut is in seconds.
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
 * Switch to a loaded project without decoding it again.
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
 * Cancel a switch that has not started.
 */

    void CancelSwitch() const { melo_instance_cancel_switch(handle_); }
    void SeekToBar(int bar) const { melo_instance_seek_bar(handle_, bar); }
    void SetTempo(int bpm) const { melo_instance_set_tempo(handle_, bpm); }
    void SetMasterVolume(float volume) const { melo_instance_set_master_volume(handle_, volume); }
    /**
 * Instance pan: -1 left, 0 center, 1 right.
 */

    void SetPan(float pan) const { melo_instance_set_pan(handle_, pan); }
    void SetTrackMuted(const char* track, bool value) const { melo_instance_set_track_muted(handle_, track, value); }
    void SetTrackMuted(int track, bool value) const { melo_instance_set_track_muted_index(handle_, track, value); }
    void SetTrackVolume(const char* track, float value) const { melo_instance_set_track_volume(handle_, track, value); }
    void SetTrackVolume(int track, float value) const { melo_instance_set_track_volume_index(handle_, track, value); }
    /**
 * Send level for bus A or B, from 0 to 1.
 */

    void SetSendA(const char* track, float level) const { melo_instance_set_send_a(handle_, track, level); }
    void SetSendA(int track, float level) const { melo_instance_set_send_a_index(handle_, track, level); }
    void SetSendB(const char* track, float level) const { melo_instance_set_send_b(handle_, track, level); }
    void SetSendB(int track, float level) const { melo_instance_set_send_b_index(handle_, track, level); }
    void SetBusVolume(int bus, float volume) const { melo_instance_set_bus_volume(handle_, bus, volume); }
    void SwapPalette(const char* palette) const { melo_instance_swap_palette(handle_, palette); }
    /**
 * e.g. SetBusEffectParam(1, 0, "Mix", 0.5f)
 */

    void SetBusEffectParam(int bus, int effect, const char* param, float value) const { melo_instance_set_bus_effect_param(handle_, bus, effect, param, value); }
    /**
 * Set a track effect parameter by name.
 */

    void SetTrackEffectParam(int track, int effect, const char* param, float value) const { melo_instance_set_track_effect_param(handle_, track, effect, param, value); }
    /**
 * Trigger a named pad. Velocity is 0 to 1; pan is -1 to 1. Repeated calls overlap.
 */

    void Trigger(const char* track, const char* pad, float velocity = 1, float pan = 0) const { melo_instance_trigger(handle_, track, pad, velocity, pan); }
    
    void Trigger(int track, const char* pad, float velocity = 1, float pan = 0) const { melo_instance_trigger_index(handle_, track, pad, velocity, pan); }
    /**
 * Hold a pad until Release is called.
 */

    void Hold(const char* track, const char* pad, float velocity = 1, float pan = 0) const { melo_instance_hold(handle_, track, pad, velocity, pan); }
    /**
 * Call each frame to sustain a pad. It releases about 0.12 seconds after the last call.
 */

    void Sustain(const char* track, const char* pad, float velocity = 1, float pan = 0) const { melo_instance_sustain(handle_, track, pad, velocity, pan); }
    void Sustain(int track, const char* pad, float velocity = 1, float pan = 0) const { melo_instance_sustain_index(handle_, track, pad, velocity, pan); }
    /**
 * Release a held pad.
 */

    void Release(const char* track, const char* pad) const { melo_instance_release(handle_, track, pad); }
    /**
 * Start a MIDI note with velocity from 0 to 1.
 */

    void NoteOn(int track, int midiNote, float velocity = 1) const { melo_instance_note_on(handle_, track, midiNote, velocity); }
    /**
 * Release a MIDI note.
 */

    void NoteOff(int track, int midiNote) const { melo_instance_note_off(handle_, track, midiNote); }
    /**
 * Play a sound through this instance. effects enables its master effects.
 */

    int PlayOneShot(const char* file, float volume = 1, float pan = 0, float pitch = 1, bool loop = false, bool effects = true) const { return melo_instance_play_one_shot(handle_, file, volume, pan, pitch, loop, effects); }
    /**
 * loops until Stop(voice)
 */

    int PlayLoop(const char* file, float volume = 1, float pan = 0, float pitch = 1, bool effects = true) const { return melo_instance_play_loop(handle_, file, volume, pan, pitch, effects); }
    void Stop(int voice) const { melo_instance_stop_voice(handle_, voice); }
    void StopSfx() const { melo_instance_stop_sfx(handle_); }
    void SetVolume(int voice, float value) const { melo_instance_voice_volume(handle_, voice, value); }
    void SetPan(int voice, float value) const { melo_instance_voice_pan(handle_, voice, value); }
    void SetPitch(int voice, float value) const { melo_instance_voice_pitch(handle_, voice, value); }
    /**
 * by name, check IsValid
 */

    MeloTrack Track(const char* name) const { return MeloTrack(handle_, melo_instance_track_index(handle_, name)); }
    /**
 * by index, check IsValid
 */

    MeloTrack Track(int index) const { return MeloTrack(handle_, index); }
    std::vector<MeloTrack> AllTracks() const { std::vector<MeloTrack> values; for (int i = 0; i < TrackCount(); ++i) values.emplace_back(handle_, i); return values; }
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
 * Queue a command to run on the audio thread.
 */

    void RunOnAudioThread(MeloAudioCommand command, void* userData = nullptr) const { melo_instance_run_on_audio_thread(handle_, command, userData); }
    /**
 * Unload this instance. Use Melo.UnloadAll to clear shared content caches.
 */

    void Unload() { melo_instance_unload(handle_); handle_ = nullptr; }
};

/**
 * Integration hooks for audio hosts. Gameplay code uses Melo and MeloInstance.
 * Configure and initialize before starting the audio device.
 */

struct MeloHost {
    /**
 * Initialize on the main thread before loading content or starting audio.
 * The sample rate must match the output device. Defaults to 44100 if invalid.
 * To change the rate after initialization, stop audio and call Shutdown first.
 */

    static void Init(int sampleRate) { melo_host_init(sampleRate); }
    /**
 * Map a content path to a disk path. Defaults to the supplied path.
 */

    static void ResolvePath(MeloPathResolver callback, void* userData = nullptr) { melo_host_set_path_resolver(callback, userData); }
    /**
 * Receives engine errors. Defaults to Console.Error.WriteLine.
 */

    static void LogError(MeloLogCallback callback, void* userData = nullptr) { melo_host_set_log_error(callback, userData); }
    /**
 * Receives engine warnings. Defaults to Console.WriteLine.
 */

    static void LogWarning(MeloLogCallback callback, void* userData = nullptr) { melo_host_set_log_warning(callback, userData); }
    /**
 * Engine build identifier for host diagnostics.
 */

    static std::string Build() { return detail::Text([](char* b, int n) { return melo_host_build(b, n); }); }
    /**
 * Current output sample rate.
 */

    static int SampleRate() { return melo_host_sample_rate(); }
    /**
 * Audio thread: fill frames of interleaved stereo float samples.
 */

    static void FillBuffer(float* buffer, int frames) { melo_host_fill_buffer(buffer, frames); }
    /**
 * Main thread: deliver async load callbacks and note events once per frame.
 */

    static void Tick() { melo_host_tick(); }
    /**
 * Release engine resources after stopping the audio device and callbacks.
 */

    static void Shutdown() { melo_host_shutdown(); }
};

/**
 * Load and control music and sound effects.
 */

struct Melo {
    /**
 * Load an exported .melo project.
 */

    static MeloInstance Load(const char* projectPath) { return MeloInstance(melo_load_instance(projectPath)); }
    /**
 * Load off the main thread. The callback is delivered by MeloHost.Tick.
 */

    static void LoadAsync(const char* projectPath, MeloLoadCallback onLoaded = nullptr, void* userData = nullptr) { melo_load_async(projectPath, onLoaded, userData); }
    /**
 * Master volume for all playback, from 0 to 1.
 */

    static float MasterVolume() { return melo_get_volume(); }
    static void MasterVolume(float value) { melo_set_volume(value); }
    /**
 * Stop all music instances. Loaded pads remain available.
 */

    static void StopAll() { melo_stop_all(); }
    /**
 * Unload all projects and clear shared sound caches.
 */

    static void UnloadAll() { melo_unload_all(); }
    /**
 * Note events: instance, track index, MIDI pitch, velocity (0 to 127). Delivered by MeloHost.Tick.
 */

    static void OnNote(MeloNoteCallback listener, void* userData = nullptr) { melo_on_note_add(listener, userData); }
    static void RemoveOnNote(MeloNoteCallback listener, void* userData = nullptr) { melo_on_note_remove(listener, userData); }
    /**
 * Play an audio file and return its voice ID. pitch is a playback-rate multiplier: 1 is normal, 2 is one octave up.
 */

    static int PlayOneShot(const char* file, float volume = 1, float pan = 0, float pitch = 1, bool loop = false, bool effects = true) { return melo_play_one_shot(file, volume, pan, pitch, loop, effects); }
    /**
 * Play an audio file in a loop until Stop(voice).
 */

    static int PlayLoop(const char* file, float volume = 1, float pan = 0, float pitch = 1, bool effects = true) { return melo_play_loop(file, volume, pan, pitch, effects); }
    static void Stop(int voice) { melo_global_stop_voice(voice); }
    static void StopSfx() { melo_global_stop_sfx(); }
    /**
 * Whether a global sound voice is still playing, including its release tail.
 */

    static bool IsPlaying(int voice) { return melo_global_voice_is_playing(voice) != 0; }
    static void SetVolume(int voice, float value) { melo_global_voice_volume(voice, value); }
    static void SetPan(int voice, float value) { melo_global_voice_pan(voice, value); }
    static void SetPitch(int voice, float value) { melo_global_voice_pitch(voice, value); }
    /**
 * Sound effect volume from 0 to 1. MasterVolume also applies.
 */

    static float SfxVolume() { return melo_get_sfx_volume(); }
    static void SfxVolume(float value) { melo_set_sfx_volume(value); }
    static void SetReverb(float mix = 0.3f, float roomSize = 0.7f, float damping = 0.5f) { melo_set_reverb(mix, roomSize, damping); }
    static void SetDelay(float mix = 0.3f, float intervalMs = 350, float decayMs = 1500) { melo_set_delay(mix, intervalMs, decayMs); }
    static void ClearEffects() { melo_clear_effects(); }
    /**
 * Copy a project bus effect chain to the SFX bus. wetMix >= 0 overrides each effect mix.
 */

    static void CopyEffectsFrom(MeloInstance instance, const char* busName, float wetMix = -1) { melo_copy_effects_from(instance.NativeHandle(), busName, wetMix); }
};
} // namespace Melowrite

#endif /* __cplusplus */
#endif /* MELO_H */
