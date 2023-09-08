#ifndef __HybridConvolverDual__
#define __HybridConvolverDual__


#include "Convoluteur_Factory.h"
#include "REverb_Convoluteur.h"
#include <memory>

class HybridConvolverDual : public HybridConvolver
{
public:
    HybridConvolverDual(int lenShort,
                        int lenMedium,
                        SampleData *impulses);
    ~HybridConvolverDual(void);

    void process(const float** inputs,
                 float** outputs,
                 int numOutput,
                 int numSamples) override;

private:
    std::unique_ptr<HConvDual[]> m_filter;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HybridConvolverDual)
};


#endif