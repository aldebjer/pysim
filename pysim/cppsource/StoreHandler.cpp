#define _SCL_SECURE_NO_WARNINGS

#include "StoreHandler.hpp"
#include "Variable_p.hpp"

#include <memory>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <string>

using std::string;
using std::shared_ptr;
using std::map;

namespace pysim {

template <class T>
class StoreStruct {
public:
    StoreStruct(T* p)
        : valueP(p) {
    }

    T* valueP;
    std::vector<T> storearray;
};

typedef map<string, shared_ptr<StoreStruct<double>>> StoreMap;
typedef map<string, shared_ptr<StoreStruct<pysim::vector>>> StoreVectorMap;
typedef map<string, shared_ptr<StoreStruct<Eigen::MatrixXd>>> StoreMatrixMap;

struct StoreHandlerPrivate {
    StoreMap storemap;
    StoreVectorMap storeVectorMap;
    StoreMatrixMap storeMatrixMap;
    std::vector<double> storetimes;

    double storeInterval;
    double nextStoreTime;
};



StoreHandler::StoreHandler():
    d_ptr(new StoreHandlerPrivate()){
}

StoreHandler::~StoreHandler() {
};

void StoreHandler::setStoreInterval(double interval) {
    d_ptr->storeInterval = interval;
}

void StoreHandler::doStoreStep(double time) {

    //Store interval funcionality, only if store interval > 0
    if (d_ptr->storeInterval > 0.0) {
        if (time < d_ptr->nextStoreTime) {
            return;
        }
        d_ptr->nextStoreTime = time + d_ptr->storeInterval;
    }



    //Check that the time is not already stored. This can happen when 
    //continuing simulations allready started.
    if (!d_ptr->storetimes.empty()) {
        if (d_ptr->storetimes.back() >= time) {
            return;
        }
    }

    d_ptr->storetimes.push_back(time);
    for (auto i = d_ptr->storemap.cbegin(); i != d_ptr->storemap.cend(); ++i) {
        double val = *(i->second->valueP);
        i->second->storearray.push_back(val);
    }
    for (auto i = d_ptr->storeVectorMap.cbegin(); i != d_ptr->storeVectorMap.cend(); ++i) {
        pysim::vector val = *(i->second->valueP);
        i->second->storearray.push_back(val);
    }
    for (auto i = d_ptr->storeMatrixMap.cbegin(); i != d_ptr->storeMatrixMap.cend(); ++i) {
        Eigen::MatrixXd val = *(i->second->valueP);
        i->second->storearray.push_back(val);
    }
}

//Put the state, der, input or output named "name" in the vector of pointers 
//to be stored. If none with "name" is found the function raises an invalid_argument
//exception.
void  StoreHandler::store(const char* name, double* value_p) {
    shared_ptr<StoreStruct<double>> p(new StoreStruct<double>(value_p));
    d_ptr->storemap[name] = p;
}

void  StoreHandler::store(const char* name, pysim::vector* value_p) {
    shared_ptr<StoreStruct<pysim::vector>> p(new StoreStruct<pysim::vector>(value_p));
    d_ptr->storeVectorMap[name] = p;
}

void  StoreHandler::store(const char* name, Eigen::MatrixXd* value_p) {
    shared_ptr<StoreStruct<Eigen::MatrixXd>> p(new StoreStruct<Eigen::MatrixXd>(value_p));
    d_ptr->storeMatrixMap[name] = p;
}

bool StoreHandler::checkAndStore(const char* name, const Variable& v) {
    if (v.d_ptr->scalars.count(name) == 1) {
        store(name, v.d_ptr->scalars[name]);
        return true;
    }else if (v.d_ptr->vectors.count(name) == 1) {
        store(name, v.d_ptr->vectors[name]);
        return true;
    }else if (v.d_ptr->matrices.count(name) == 1) {
        store(name, v.d_ptr->matrices[name]);
        return true;
    }
    return false;
}

const std::vector<double>& StoreHandler::getStoreVector(char* name) {
    return d_ptr->storemap[name]->storearray;
};

size_t StoreHandler::getStoreSize() {
    return d_ptr->storetimes.size();
}

size_t StoreHandler::getStoreColumns(char* name) {
    if (d_ptr->storemap.count(name) > 0) {
        return 1;
    } else if (d_ptr->storeVectorMap.count(name) > 0) {
        size_t columns = d_ptr->storeVectorMap[name]->storearray.back().size();
        return columns;
    }else{
        throw std::invalid_argument("Could not find column for stored value");
    }
}

std::pair<size_t, size_t> StoreHandler::getStoreRowColumns(char* name) {
    if (d_ptr->storemap.count(name) > 0) {
        return std::pair<size_t, size_t>(1,1);
    } else if (d_ptr->storeVectorMap.count(name) > 0) {
        size_t rows = d_ptr->storeVectorMap[name]->storearray.back().size();
        return std::pair<size_t, size_t>(rows, 1);
    } else if (d_ptr->storeMatrixMap.count(name) > 0) {
        size_t rows = d_ptr->storeMatrixMap[name]->storearray.back().rows();
        size_t columns = d_ptr->storeMatrixMap[name]->storearray.back().cols();
        return std::pair<size_t, size_t>(rows, columns);
    }
    else {
        throw std::invalid_argument("Could not find column for stored value");
    }
}

std::vector<string> StoreHandler::getStoreNames() {
    std::vector<string> out;
    for (auto i = d_ptr->storemap.cbegin(); i != d_ptr->storemap.cend(); ++i) {
        out.push_back(i->first);
    }
    for (auto i = d_ptr->storeVectorMap.cbegin(); i != d_ptr->storeVectorMap.cend(); ++i) {
        out.push_back(i->first);
    }

    return out;
}

std::vector<string> StoreHandler::getStoreMatricesNames() {
    std::vector<string> out;
    for (auto i = d_ptr->storeMatrixMap.cbegin(); i != d_ptr->storeMatrixMap.cend(); ++i) {
        out.push_back(i->first);
    }
    return out;
}

void StoreHandler::fillWithStore(char* name, double* p, size_t rows, size_t columns) {
    double* ptemp = p;

    if (d_ptr->storemap.count(name) > 0) {
        if (columns != 1) {
            throw std::runtime_error("Internal Pysim Error, invalid number of columns");
        }
        ptemp += rows;
        auto v = &(d_ptr->storemap[name]->storearray);
        int rowcounter = 0;
        for (auto i = v->rbegin(); i != v->rend(); ++i) {
            if (++rowcounter > rows) {
                throw std::runtime_error("Internal Pysim Error, invalid number of rows");
            }
            *(--ptemp) = *i;
        }
    } else if (d_ptr->storeVectorMap.count(name) > 0) {
        ptemp += rows*columns;
        auto mat = &(d_ptr->storeVectorMap[name]->storearray);
        int rowcounter = 0;
        for (auto row = mat->rbegin(); row != mat->rend(); ++row) {
            if (++rowcounter > rows) {
                throw std::runtime_error("Internal Pysim Error, invalid number of rows");
            }
            int columncounter = 0;
            for (auto i = row->rbegin(); i != row->rend(); ++i) {
                if (++columncounter > columns) {
                    throw std::runtime_error("Internal Pysim Error, invalid number of columns");
                }
                *(--ptemp) = *i;
            }
        }
    }
}

void StoreHandler::fillWithScalars(char* name, double* p, size_t timesteps) {
    if (d_ptr->storemap.count(name) <= 0) {
        throw std::invalid_argument("No such stored variable");
    }
    double* ptemp = p + timesteps;

    auto v = &(d_ptr->storemap[name]->storearray);
    if (v->size() != timesteps) {
        throw std::runtime_error("Internal Pysim Error, invalid number of timesteps");
    }

    for (auto i = v->rbegin(); i != v->rend(); ++i) {
        *(--ptemp) = *i;
    }
}

void StoreHandler::fillWithVectors(char* name, double* p, size_t timesteps, size_t rows) {
    double* ptemp = p + timesteps * rows;

    auto v = &(d_ptr->storeVectorMap[name]->storearray);
    if (v->size() != timesteps) {
        throw std::runtime_error("Internal Pysim Error, invalid number of timesteps");
    }

    for (auto ts = v->rbegin(); ts != v->rend(); ++ts) {
        if (ts->size() != rows) {
            throw std::runtime_error("Internal Pysim Error, invalid number of columns");
        }
        for (auto i = ts->rbegin(); i != ts->rend(); ++i) {
            *(--ptemp) = *i;
        }
    }
}

void StoreHandler::fillWithMatrices(char* name, double* p, size_t avail_size, size_t rows, size_t columns) {

    auto v = &(d_ptr->storeMatrixMap[name]->storearray);
    if (v->size() > avail_size) {
        throw std::runtime_error("Internal Pysim Error, invalid number of timesteps");
    }

    //If available timestems are smaller than timesteps to fill then dont
    //fill the first ones
    size_t skippedSteps = avail_size - v->size();
    double* ptemp = p+skippedSteps;

    for (auto ts = v->begin(); ts != v->end(); ++ts) {
        if (ts->rows() != rows) {
            throw std::runtime_error("Internal Pysim Error, invalid number of rows");
        }
        if (ts->cols() != columns) {
            throw std::runtime_error("Internal Pysim Error, invalid number of columns");
        }

        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < columns; ++j) {
                *(ptemp++) = ts->operator()(i,j);
            }
        }
    }
}

void StoreHandler::fillWithTime(double* p) {
    double* ptemp = p;

    for (auto i = d_ptr->storetimes.cbegin(); i != d_ptr->storetimes.cend(); ++i) {
        *(ptemp++) = *i;
    }
}

}
