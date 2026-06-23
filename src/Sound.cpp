#define MINIAUDIO_IMPLEMENTATION
#include "Sound.h"

bool SoundCore::Init() {
    statusSoundCore = ma_engine_init(NULL, &engineSoundCore);
    if (statusSoundCore != MA_SUCCESS) {
        #ifdef APPLICATION_DEBUG_MODE   
            // é uma boa pratica     
            std::cerr << "MINIAUDIO::ENGINE_INITIALIZATION::FAILED\n";
            /*
                std::cerr é feito para codigos de erro, diferente do cout,
                ele nao passa pelo buffer do sistema, o que significa que ele
                é lançado no console imediatamente.
            */
        #endif
        return false;
    }
    return true;
}

void SoundCore::Release() {
    ma_engine_uninit(&engineSoundCore);
    // verificação de anulidade ja é feita dentro do codigo minisound
}

void SoundCore::SCPlaySound(const char* audio_path) {
    if (statusSoundCore == MA_SUCCESS) {
        ma_engine_play_sound(&engineSoundCore, audio_path, NULL);
    }
    // funcao padrao
}