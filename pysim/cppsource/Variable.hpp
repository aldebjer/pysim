#pragma once
#include <vector>
#include <map>
#include <memory>
#include <string>


namespace pysim {

struct VariablePrivate;

class Variable{
public:
    Variable();
    std::vector<std::string> getScalarNames();
    std::vector<std::string> getVectorNames();
    std::vector<std::string> getMatrixNames();
    void setScalar(char* name, double value);
    void setVector(char* name, std::vector<double> value);
    void setMatrix(char* name, std::vector<std::vector<double>> value);
    double getScalar(char* name);
    std::vector<double> getVector(char* name);
    std::vector<std::vector<double>> getMatrix(char* name);
    std::map<std::string, std::string> getDescriptionMap();
    
    std::unique_ptr<VariablePrivate> d_ptr;
};

}
