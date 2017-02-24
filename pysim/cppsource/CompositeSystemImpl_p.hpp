#pragma once
#include <vector>
#include <map>

#include <Eigen/Dense>

#include "CompositeSystemImpl.hpp"

#include "StoreHandler.hpp"
#include "PysimTypes.hpp"


namespace pysim {

struct CompositeSystemImplPrivate {
    std::vector<CommonSystemImpl*> subsystems_common;
    std::map<std::string, CommonSystemImpl*> subsystems_common_map;

    std::vector<std::unique_ptr<double>> scalar_inports;
    std::vector<std::unique_ptr<pysim::vector>> vector_inports;
    std::vector<std::unique_ptr<Eigen::MatrixXd>> matrix_inports;
    std::vector<std::pair<double*, double* > > connected_inport_scalars;
    std::vector<std::pair<pysim::vector*, pysim::vector* > > connected_vectors;
    std::vector<std::pair<Eigen::MatrixXd*, Eigen::MatrixXd* > > connected_matrices;

    StoreHandler storeHandler;
};

}

