#include "build_const_tree.hpp"
#include <string>
#include <nlohmann/json.hpp>
#include <algorithm>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fstream>
#include "stark_info.hpp"
#include <filesystem>
#include <cstdint>

using namespace std;
using json = nlohmann::json;

Goldilocks fr;

void buildConstTree(const string constFile, const string starkInfoFile, const string constTreeFile, const string verKeyFile)
{
    TimerStart(BUILD_CONST_TREE);

    StarkInfo starkInfo(starkInfoFile);

    uint64_t nBits = starkInfo.starkStruct.nBits;
    uint64_t nBitsExt = starkInfo.starkStruct.nBitsExt;
    uint64_t N = 1 << nBits;
    uint64_t NExtended = 1 << nBitsExt;
    uint64_t nPols = starkInfo.nConstants;

    uintmax_t constPolsSize = starkInfo.nConstants * sizeof(Goldilocks::Element) * N;
    
    TimerStart(LOADING_CONST_POLS);
    Goldilocks::Element *pConstPols = (Goldilocks::Element *)copyFile(constFile, constPolsSize);
    Goldilocks::Element *pConstPolsExt = (Goldilocks::Element *)malloc(NExtended * nPols * sizeof(Goldilocks::Element));
    TimerStopAndLog(LOADING_CONST_POLS);

    TimerStart(EXTEND_CONST_POLS);
    NTT_Goldilocks ntt(N);
    ntt.extendPol(pConstPolsExt, pConstPols, NExtended, N, nPols);
    TimerStopAndLog(EXTEND_CONST_POLS);

    if (starkInfo.starkStruct.verificationHashType == "GL") {
        TimerStart(MERKELIZE_CONST_TREE);
        Goldilocks::Element root[4];
        MerkleTreeGL mt(NExtended, nPols, pConstPolsExt);
        mt.merkelize();
        mt.getRoot(root);
        TimerStopAndLog(MERKELIZE_CONST_TREE);

        TimerStart(GENERATING_FILES);

        if (verKeyFile != "") {
            json jsonVerKey;
            json value;
            value[0] = Goldilocks::toU64(root[0]);
            value[1] = Goldilocks::toU64(root[1]);
            value[2] = Goldilocks::toU64(root[2]);
            value[3] = Goldilocks::toU64(root[3]);
            jsonVerKey["constRoot"] = value;
            json2file(jsonVerKey, verKeyFile);
        }

        // ConstTree
        ofstream fw(constTreeFile.c_str(), std::fstream::out | std::fstream::binary);
        fw.write((const char *)&(nPols), sizeof(uint64_t));
        fw.write((const char *)&(NExtended), sizeof(uint64_t));
        fw.write((const char *)pConstPolsExt, nPols * NExtended * sizeof(Goldilocks::Element));
        fw.write((const char *)mt.nodes, mt.getTreeNumElements() * sizeof(Goldilocks::Element));
        fw.close();

        TimerStopAndLog(GENERATING_FILES);

    } else if(starkInfo.starkStruct.verificationHashType == "BN128"){
        TimerStart(MERKELIZE_CONST_TREE);
        RawFr::Element rootC;
        MerkleTreeBN128 mt(starkInfo.merkleTreeArity, NExtended, nPols, pConstPolsExt);
        mt.merkelize();
        mt.getRoot(&rootC);
        TimerStopAndLog(MERKELIZE_CONST_TREE);

        if (verKeyFile != "") {
            json jsonVerKey;
            RawFr rawfr;
            jsonVerKey["constRoot"] = rawfr.toString(rootC);
            json2file(jsonVerKey, verKeyFile);
        }

        TimerStart(GENERATING_FILES);

        // ConstTree
        std::ofstream fw(constTreeFile.c_str(), std::fstream::out | std::fstream::binary);
        fw.write((const char *)&(mt.source_width), sizeof(mt.source_width));
        fw.write((const char *)&(mt.height), sizeof(mt.height));
        fw.write((const char *)mt.source, nPols * NExtended * sizeof(Goldilocks::Element));
        fw.write((const char *)mt.nodes, mt.numNodes * sizeof(RawFr::Element));
        fw.close();

        TimerStopAndLog(GENERATING_FILES);
    } else {
        cerr << "Invalid Hash Type: " << starkInfo.starkStruct.verificationHashType << endl;
        exit(-1);
    }

    free(pConstPolsExt);
    TimerStopAndLog(BUILD_CONST_TREE);
}