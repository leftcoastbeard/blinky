/* 
 * File:   sample.cpp
 * Author: C0320318
 * 
 * Created on November 18, 2014, 10:14 AM
 */

#include "Sample.h"

template <typename T>
Sample<T>::Sample(unsigned mnos) {
    max_samples = (unsigned int) mnos;
    sample_index = 0;
    samples = new T[mnos];
#if __cplusplus == 201103L
    for(int i: samples) samples[i]=0;
#else
    for(int i=0; i<max_samples; i++){
        samples[i]=0;
    }
#endif
    sum = (T)(0);
    current = (T)(0);
    average = (T)(0);
    //calibrated = (T)(0);
}

template <typename T>
T Sample<T>::GetAverage(){
    return average;
}

template <typename T>
T Sample<T>::GetCurrent(){
    return current;
}

template <typename T>
T Sample<T>::GetSum(){
    return sum;
}

template <typename T>
T Sample<T>::Update(T sample_data){
  current = sample_data;
  sum += current;
  sum -= samples[sample_index];
  samples[sample_index] = current;
  average = sum/max_samples;
  sample_index++;
  if(sample_index == max_samples) sample_index = 0;
  return average;
}

//Float version, because Templates are not working properly...?
template <>
float Sample<float>::Update(float sample_data){
  current = sample_data;
  sum += current;
  sum -= samples[sample_index];
  samples[sample_index] = current;
  average = sum/max_samples;
  sample_index++;
  if(sample_index == max_samples) sample_index = 0;
  return average;
}

/*
sample::sample(const sample& orig) {
}*/
template <typename T>
Sample<T>::~Sample() {
    delete samples;
}

