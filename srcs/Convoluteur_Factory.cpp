#include "Convoluteur_Factory.h"
#include "REverb_Convoluteur.h"
#include <memory>

HybridConvolver *ConvolverFactory::create(
    const ParamPartitionWisdom &wisdom, SampleData *impulses,
    int sflen, int strategy)
{
    std::unique_ptr<HybridConvolver> convolver;

    Partitioner partitioner(wisdom);
    int hlen = impulses->getDataLen();
    partitioner.analyze(hlen, sflen, strategy);
    int mflen = partitioner.getM();
    int lflen = partitioner.getL();
    int type  = partitioner.getType();

    switch (type) {
    case PARTITION_TYPE_TRIPPLE:
        convolver.reset(new HybridConvolverTripple(sflen, mflen, lflen, impulses));
        break;
    case PARTITION_TYPE_DUAL:
        convolver.reset(new HybridConvolverDual(sflen, mflen, impulses));
        break;
    default:
        convolver.reset(new HybridConvolverSingle(sflen, impulses));
    }

    return convolver.release();
}
