namespace Mixer {

struct AudioMixer {
    u64 num_sounds;
    Sound **sound;

    // Lists of played sounds, carried over between
    // frames, synced and such.

    Util::MemoryArena *arena;
} audio_mixer;

struct AudioID {
    u8 gen;
    u16 slot;
};

const f32 NEXT_TONE = 1.0594630943593;
const f32 BASE_TONE = 440;

constexpr f32 AUDIO_DEFAULT_GAIN = 0.2;
constexpr f32 AUDIO_DEFAULT_VARIANCE = 0.01;

void lock_audio();
void unlock_audio();

bool init();

//*
// Plays a sound in the game world, the sound should have been
// loaded by the asset system:<br>
//  - asset_id, the sound asset to play.<br>
//  - pitch, how fast the sound should be played.<br>
//  - gain, how loud the sound should be played.<br>
//  - pitch_variance, how much random variance there should be applied to the pitch.<br>
//  - gain_variance, how much random variance there should be applied to the gain.<br>
//  - loop, if the sound should loop or not.<br>
AudioID play_sound(AssetID asset_id, f32 pitch = 1.0,
                   f32 gain = AUDIO_DEFAULT_GAIN,
                   f32 pitch_variance = AUDIO_DEFAULT_VARIANCE,
                   f32 gain_variance = AUDIO_DEFAULT_VARIANCE,
                   bool loop = false);

//*
// Plays a sound in the game world at a specific place thus the sound
// has applied distance attenuation. The sound
// should have been loaded by the asset system:<br>
//  - asset_id, the sound asset to play.<br>
//  - position, where in the game world the sound should come from.<br>
//  - pitch, how fast the sound should be played.<br>
//  - gain, how loud the sound should be played.<br>
//  - pitch_variance, how much random variance there should be applied to the pitch.<br>
//  - gain_variance, how much random variance there should be applied to the gain.<br>
//  - loop, if the sound should loop or not.
AudioID play_sound_at(AssetID asset_id, Vec2 position, f32 pitch = 1.0,
                      f32 gain = AUDIO_DEFAULT_GAIN,
                      f32 pitch_variance = AUDIO_DEFAULT_VARIANCE,
                      f32 gain_variance = AUDIO_DEFAULT_VARIANCE,
                      bool loop = false);

//*
// Stops a sound from playing.<br>
//  - id, the sound that is wished to stop.
void stop_sound(AudioID id);

};
