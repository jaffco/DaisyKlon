#include "../libDaisy/src/daisy_seed.h"
#include "../Portaklon/Portaklon.hpp"

using namespace daisy;

static DaisySeed hardware;
static Portaklon portaklon;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
    // Use left input channel for mono processing
    float monoInput[128];
    for (size_t i = 0; i < size; ++i) {
        monoInput[i] = in[0][i];
    }

    float monoOutput[128];
    portaklon.process(monoInput, monoOutput, size);

    // Output to both channels
    for (size_t i = 0; i < size; ++i) {
        out[0][i] = monoOutput[i];
        out[1][i] = monoOutput[i];
    }
}

int main() {
  hardware.Init();

  // Set audio parameters
  hardware.SetAudioBlockSize(128);
  hardware.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);

  // Initialize Portaklon
  portaklon.init(48000, 128);

  // Start audio
  hardware.StartAudio(AudioCallback);

  hardware.StartLog(); 
  hardware.PrintLine("===========================================");
  hardware.PrintLine("          Portaklon Integrated!            ");
  hardware.PrintLine("===========================================");

  // Main loop
  while (true) {
    System::Delay(1000);
  }

  return 0;
}