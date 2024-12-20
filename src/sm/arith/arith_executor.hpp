#ifndef ARITH_SM_HPP
#define ARITH_SM_HPP

#include "definitions.hpp"
#include "config.hpp"
#include "arith_action.hpp"
#include "utils.hpp"
#include "sm/pols_generated/commit_pols.hpp"
#include "ffiasm/fec.hpp"
#include "scalar.hpp"
#include "exit_process.hpp"
#include "fork_info.hpp"

USING_PROVER_FORK_NAMESPACE;

class ArithExecutor
{
private:
    Goldilocks &fr;
    RawFec fec;
    const Config &config;
    const uint64_t N;
    mpz_class pFec;
public:
    const uint64_t maxInputs;

public:
    ArithExecutor (Goldilocks &fr, const Config &config) :
        fr(fr),
        config(config),
        N(getForkN(PROVER_FORK_ID)),
        maxInputs(N/32)
    {
        // Calculate the prime number
        fec2scalar(fec, fec.negOne(), pFec);
        pFec++;
    }
    ~ArithExecutor ()
    {
    }
    void execute (vector<ArithAction> &action, PROVER_FORK_NAMESPACE::ArithCommitPols &pols);
};

#endif