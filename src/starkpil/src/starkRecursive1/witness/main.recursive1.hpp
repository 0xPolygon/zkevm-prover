#ifndef ZKEVM_VERIFIER_MAIN_RECURSIVE_1_HPP
#define ZKEVM_VERIFIER_MAIN_RECURSIVE_1_HPP

#include <string>
#include <nlohmann/json.hpp>
#include "calcwit.recursive1.hpp"
#include "circom.recursive1.hpp"
#include "fr_goldilocks.hpp"

namespace CircomRecursive1
{
    using json = nlohmann::json;
    Circom_Circuit *loadCircuit(std::string const &datFileName);
    void freeCircuit(Circom_Circuit *circuit);
    void loadJson(Circom_CalcWit *ctx, std::string filename);
    void loadJsonImpl(Circom_CalcWit *ctx, json &j);
    void writeBinWitness(Circom_CalcWit *ctx, std::string wtnsFileName);
    void getBinWitness(Circom_CalcWit *ctx, FrGElement *&pWitness, uint64_t &witnessSize);
}
#endif