#ifndef __HybridConvolver__
#define __HybridConvolver__

#include "Convoluteur_Factory.h"
#include "REverb_Convoluteur.h"
#include <memory>

class HybridConvolver
{
public:
    HybridConvolver();
    virtual ~HybridConvolver() {}

    virtual void process(const float** inputs,
                         float** outputs,
                         int numOutput,
                         int numSamples) = 0;
    void process(AudioSampleBuffer& buffer);

protected:
    std::unique_ptr<float[]> m_inbuf;
    std::unique_ptr<float[]> m_outbuf;
    int m_bufpos = 0;
    int m_lenShort = 256;
    int m_lenMedium = 1024;
    int m_lenLong = 4096;
    int m_numFilter = 0;
    std::unique_ptr<int[]> m_routeIn;
    std::unique_ptr<int[]> m_routeOut;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HybridConvolver)
};


#endif
