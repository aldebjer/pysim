//   Copyright (c) 2014-2016 SSPA Sweden AB
#pragma once

#include <vector>
#include <map>
#include <string>
#include <utility>

#include "PysimTypes.hpp"
#include <Eigen/Dense>

namespace pysim {

struct StoreHandlerPrivate;
class Variable;

class StoreHandler{
public:
    StoreHandler();
    virtual ~StoreHandler();

    void doStoreStep(double time);

    //Store handling
    const std::vector<double>& getStoreVector(char* name);
    void fillWithStore(char* name, double* p, size_t rows, size_t columns);
    void fillWithScalars(char* name, double* p, size_t timesteps);
    void fillWithVectors(char* name, double* p, size_t timesteps, size_t rows);
    void fillWithMatrices(char* name, double* p, size_t timesteps, size_t rows, size_t columns);
    void fillWithTime(double* p);
    size_t getStoreSize();
    size_t getStoreColumns(char* name);
    std::pair<size_t, size_t> getStoreRowColumns(char* name);
    void setStoreInterval(double interval);
    std::vector<std::string> getStoreNames();
    std::vector<std::string> getStoreMatricesNames();
    void store(const char* name, double* pointer);
    void store(const char* name, pysim::vector* pointer);
    void store(const char* name, Eigen::MatrixXd* pointer);
    bool checkAndStore(const char* name, const Variable& v);

protected:

private:
    std::unique_ptr<StoreHandlerPrivate> d_ptr;

};

}
