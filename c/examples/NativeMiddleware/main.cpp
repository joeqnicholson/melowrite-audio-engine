#include "../../melo.h"
#include <algorithm>
#include <cmath>
#include <cstdio>

// Offline integration check; your game supplies the real audio callback.
int main(int argc, char** argv) {
    if (argc != 2) {
        std::puts("Usage: NativeMiddleware path/to/music.melo");
        return 1;
    }
    using namespace Melowrite;
    MeloHost::Init(44100);
    MeloInstance music = Melo::Load(argv[1]);
    if (!music.IsLoaded()) { MeloHost::Shutdown(); return 1; }
    music.PlayChunk();
    float buffer[2048], peak = 0;
    for (int block = 0; block < 100; ++block) {
        MeloHost::FillBuffer(buffer, 1024);
        MeloHost::Tick();
        for (float sample : buffer) peak = std::max(peak, std::fabs(sample));
    }
    std::printf("Rendered first chunk. Peak: %.3f\n", static_cast<double>(peak));
    music.Unload();
    MeloHost::Shutdown();
    return 0;
}
