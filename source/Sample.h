/* 
 * File:   sample.h
 * Author: C0320318
 *
 * Created on November 18, 2014, 10:14 AM
 */

#ifndef SAMPLE_H
#define SAMPLE_H
#include "mbed-drivers/mbed.h"

template <typename T>
class Sample {
public:
    Sample(unsigned int mnos);
    T Update(T sample_data);
    T GetSum();
    T GetCurrent();
    T GetAverage();
    //sample(const sample& orig);
    virtual ~Sample();
private:
    T sum;
    T current;
    T *samples;
    T average;
    //T calibrated;
    unsigned int sample_index;
    unsigned int max_samples;
};

#endif  /* SAMPLE_H */

