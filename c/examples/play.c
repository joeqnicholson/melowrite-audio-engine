#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#include <mmreg.h>
#include "melo.h"
#include <conio.h>
#include <stdio.h>

#define FRAMES 1024
#define BUFFERS 3

int main(int argc, char **argv)
{
    const char *path = argc > 1 ? argv[1] : "Examples/Melo, world!.melo";

    melo_host_init(44100);
    MeloInstanceHandle music = melo_load_instance(path);
    if (!music || !melo_instance_is_loaded(music)) {
        fprintf(stderr, "Could not load %s\n", path);
        melo_host_shutdown();
        return 1;
    }

    // Windows plays the same stereo floats that Melowrite produces.
    WAVEFORMATEX format = {WAVE_FORMAT_IEEE_FLOAT, 2, 44100, 44100 * 8, 8, 32, 0};
    HWAVEOUT output;
    if (waveOutOpen(&output, WAVE_MAPPER, &format, 0, 0, CALLBACK_NULL) != MMSYSERR_NOERROR) {
        fputs("Could not open the audio device.\n", stderr);
        melo_host_shutdown();
        return 1;
    }

    float samples[BUFFERS][FRAMES * 2] = {{0}};
    WAVEHDR headers[BUFFERS] = {{0}};
    int prepared = 0, result = 1;
    for (int i = 0; i < BUFFERS; i++) {
        headers[i].lpData = (LPSTR)samples[i];
        headers[i].dwBufferLength = sizeof(samples[i]);
        if (waveOutPrepareHeader(output, &headers[i], sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
            goto cleanup;
        prepared++;
    }

    melo_instance_play_chunk(music, NULL, 1, 0);
    puts("Playing. Press Enter to stop.");

    while (!_kbhit() || _getch() != '\r') {
        for (int i = 0; i < BUFFERS; i++) {
            if (headers[i].dwFlags & WHDR_INQUEUE) continue;
            if (melo_host_fill_buffer(samples[i], FRAMES) != FRAMES
                || waveOutWrite(output, &headers[i], sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
                goto cleanup;
        }
        melo_host_tick();
        Sleep(1);
    }

    result = 0;

cleanup:
    if (result) fputs("Audio playback failed.\n", stderr);
    waveOutReset(output);
    for (int i = 0; i < prepared; i++) waveOutUnprepareHeader(output, &headers[i], sizeof(WAVEHDR));
    waveOutClose(output);
    melo_host_shutdown();
    return result;
}
