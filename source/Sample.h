/* 
 * File:   sample.h
 * Author: C0320318
 *
 * Created on November 18, 2014, 10:14 AM
 */

#ifndef SAMPLE_H
#define SAMPLE_H
#include "mbed-drivers/mbed.h"

template <class a_T>
class Sample {
public:
    Sample(unsigned int mnos);
    a_T Update(a_T sample_data);
    a_T GetSum();
    a_T GetCurrent();
    a_T GetAverage();
    //sample(const sample& orig);
    virtual ~Sample();
private:
    a_T sum;
    a_T current;
    a_T *samples;
    a_T average;
    //a_T calibrated;
    unsigned int sample_index;
    unsigned int max_samples;
};

#endif  /* SAMPLE_H */

