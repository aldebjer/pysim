#pragma once
#include <vector>
#include <map>

#include "CompositeSystemImpl.hpp"

#include "StoreHandler.hpp"
#include "PysimTypes.hpp"


namespace pysim {

struct CompositeSystemImplPrivate {
    std::vector<CommonSystemImpl*> subsystems_common;
    std::map<std::string, CommonSystemImpl*> subsystems_common_map;

    std::vector<std::pair<double*, double* > > connected_scalars;
    std::vector<std::pair<pysim::vector*, pysim::vector* > > connected_vectors;

    StoreHandler storeHandler;
};

}

