//   Copyright (c) 2014-2016 SSPA Sweden AB

#pragma once


#include <vector>
#include <map>
#include <utility>
#include <string>
#include <memory>

#include "PysimTypes.hpp"

struct Dptr;

class StoreHandler{
public:
    StoreHandler();
    virtual ~StoreHandler();

    void doStoreStep(double time);

    //Store handling
    const std::vector<double>& getStoreVector(char* name);
    void fillWithStore(char* name, double* p,int rows, int columns);
    void fillWithTime(double* p);
    int getStoreSize();
    int getStoreColumns(char* name);
    void setStoreInterval(double interval);
    std::vector<std::string> getStoreNames();
    void store_scalar(char* name, double* pointer);
    void store_vector(char* name, pysim::vector* pointer);

protected:

private:
    Dptr* d_ptr;

};

