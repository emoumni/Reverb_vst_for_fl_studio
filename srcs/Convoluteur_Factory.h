#ifndef HR2_CONVOLVER_FACTORY_H
#define HR2_CONVOLVER_FACTORY_H

class HybridConvolver;
class SampleData;

class ConvolverFactory
{
public:
    HybridConvolver *create(
        const ParamPartitionWisdom &wisdom, SampleData *impulses,
        int sflen, int strategy);
};

#endif
