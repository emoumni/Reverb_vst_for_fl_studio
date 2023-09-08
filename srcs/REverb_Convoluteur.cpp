#include "Convoluteur_Factory.h"
#include "REverb_Convoluteur.h"
#include <memory>

HybridConvolver::HybridConvolver()
{
}

void HybridConvolver::process(AudioSampleBuffer& buffer)
{
    int numSamples = buffer.getNumSamples();
    int numChannels = buffer.getNumChannels();

    const float **inputs = buffer.getArrayOfReadPointers();
    float **outputs = buffer.getArrayOfWritePointers();

    process(inputs, outputs, numChannels, numSamples);
}
