#define _SCL_SECURE_NO_WARNINGS

#include "StoreHandler.hpp"


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

struct StoreHandlerPrivate {
    StoreMap storemap;
    StoreVectorMap storeVectorMap;
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
}

//Put the state, der, input or output named "name" in the vector of pointers 
//to be stored. If none with "name" is found the function raises an invalid_argument
//exception.
void  StoreHandler::store_scalar(char* name, double* value_p) {
    shared_ptr<StoreStruct<double>> p(new StoreStruct<double>(value_p));
    d_ptr->storemap[name] = p;
}

void  StoreHandler::store_vector(char* name, pysim::vector* value_p) {
    shared_ptr<StoreStruct<pysim::vector>> p(new StoreStruct<pysim::vector>(value_p));
    d_ptr->storeVectorMap[name] = p;
}

const std::vector<double>& StoreHandler::getStoreVector(char* name) {
    return d_ptr->storemap[name]->storearray;
};

int StoreHandler::getStoreSize() {
    return d_ptr->storetimes.size();
}

int StoreHandler::getStoreColumns(char* name) {
    if (d_ptr->storemap.count(name) > 0) {
        return 1;
    } else if (d_ptr->storeVectorMap.count(name) > 0) {
        int columns = d_ptr->storeVectorMap[name]->storearray.back().size();
        return columns;
    }else{
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

void StoreHandler::fillWithStore(char* name, double* p, int rows, int columns) {
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

void StoreHandler::fillWithTime(double* p) {
    double* ptemp = p;

    for (auto i = d_ptr->storetimes.cbegin(); i != d_ptr->storetimes.cend(); ++i) {
        *(ptemp++) = *i;
    }
}

}
