#include "Convoluteur_Factory.h"
#include "REverb_Convoluteur.h"
#include <memory>
#include <stdio.h>
#include <string.h>

#ifndef NO_OMP
#include <omp.h>
#endif

#ifdef _WIN32
#include <malloc.h>
#define alloca _alloca
#else
#include <alloca.h>
#endif


HybridConvolverDual::HybridConvolverDual(int lenShort,
                                         int lenMedium,
                                         SampleData *impulses)
{
    m_bufpos    = 0;
    m_lenShort  = lenShort;
    m_lenMedium = lenMedium;
    m_numFilter = 0;

    for (int i = 0; i < 4; i++)
        if (impulses->hasData(i) == true)
            m_numFilter++;

    m_inbuf.reset(new float[m_numFilter * lenShort]());
    m_outbuf.reset(new float[m_numFilter * lenShort]());

    m_filter.reset(new HConvDual[m_numFilter]);

    m_routeIn.reset(new int[m_numFilter]);
    m_routeOut.reset(new int[m_numFilter]);

    int hlen = impulses->getDataLen();
    int k = 0;
    for (int i = 0; i < 4; i++)
    {
        if (impulses->hasData(i) == true)
        {
            m_routeIn[k]  = (i / 2) & 1;
            m_routeOut[k] = i & 1;
            hcInitDual(&(m_filter[k]), impulses->getData(i), hlen, m_lenShort, m_lenMedium);
            m_filter[k].step = k % m_filter[k].maxstep;
            k++;
        }
    }
}

HybridConvolverDual::~HybridConvolverDual(void)
{
    for (int i = 0; i < m_numFilter; i++)
    {
        hcCloseDual(&(m_filter[i]));
    }
}


void HybridConvolverDual::process(const float** inputs, float** outputs, int numOutput, int numSamples)
{
    int pos, new_pos;

    // allocate buffer for SMP processing on the stack
    float **outputSMP = (float**)alloca(m_numFilter * sizeof(float*));
    for (int f = 0; f < m_numFilter; f++)
    {
        outputSMP[f] = (float*)alloca(numSamples * sizeof(float));;
    }

#ifndef NO_OMP
    omp_set_num_threads(m_numFilter);
#pragma omp parallel for private(pos)
#endif
    for (int f = 0; f < m_numFilter; f++)
    {
        pos = m_bufpos;
        const float *in  = inputs[m_routeIn[f]];
        float *out = outputSMP[f];
        float *buf_in  = &(m_inbuf[f * m_lenShort]);
        float *buf_out = &(m_outbuf[f * m_lenShort]);
        for (int s = 0; s < numSamples; s++)
        {
            buf_in[pos] = in[s];
            out[s] = buf_out[pos];
            pos++;
            if (pos == m_lenShort)
            {
                hcProcessDual(&(m_filter[f]), buf_in, buf_out);
                pos = 0;
            }
        }
        new_pos = pos;
    }

    m_bufpos = new_pos;

    // clear output buffers
    for (int o = 0; o < numOutput; o++)
        memset(outputs[o], 0, numSamples * sizeof(float));

    for (int f = 0; f < m_numFilter; f++)
        for (int s = 0; s < numSamples; s++)
            outputs[m_routeOut[f]][s] += outputSMP[f][s];
}
