#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
#include "definitions.hpp"

using namespace std;
using json = nlohmann::json;

class Config
{
public:
    string processID; // UUID assigned to this process instance, i.e. to this zkProver execution

    bool runProverServer;
    bool runProverServerMock;
    bool runProverClient;
    bool runExecutorServer;
    bool runExecutorClient;
    bool runExecutorClientMultithread;
    bool runStateDBServer;
    bool runStateDBTest;
    bool runAggregatorServer;
    bool runAggregatorClient;

    bool runFileGenProof;                   // [Deprecated] Full proof = Executor + Stark + StarkC12a + StarkC12b + Groth16 (Snark) 
    bool runFileGenBatchProof;              // Proof of 1 batch = Executor + Stark + StarkC12a + Recursive1
    bool runFileGenAggregatedProof;         // Proof of 2 batches = Recursive2 (of the 2 batches StarkC12a)
    bool runFileGenFinalProof;              // Final proof of an aggregated proof = RecursiveF + Groth16 (Snark)
    bool runFileProcessBatch;               // Executor (only main SM)
    bool runFileProcessBatchMultithread;    // Executor (only main SM) in parallel

    bool runKeccakScriptGenerator;
    bool runKeccakTest;
    bool runStorageSMTest;
    bool runBinarySMTest;
    bool runMemAlignSMTest;
    bool runSHA256Test;
    bool runBlakeTest;
    
    bool executeInParallel;
    bool useMainExecGenerated;

    bool saveRequestToFile; // Saves the grpc service request, in text format
    bool saveInputToFile; // Saves the grpc input data, in json format
    bool saveDbReadsToFile; // Saves the grpc input data, including database reads done during execution, in json format
    bool saveDbReadsToFileOnChange; // Same as saveDbReadsToFile, but saving the file at every read (slow, but useful if executor crashes)
    bool saveOutputToFile; // Saves the grpc output data, in json format
    bool saveResponseToFile; // Saves the grpc service response, in text format
    bool loadDBToMemCache;
    bool opcodeTracer;
    bool logRemoteDbReads;
    bool logExecutorServerResponses;

    uint16_t proverServerPort;
    uint16_t proverServerMockPort;
    uint64_t proverServerMockTimeout;
    uint16_t proverClientPort;
    string proverClientHost;

    uint16_t executorServerPort;
    bool executorROMLineTraces;
    uint16_t executorClientPort;
    string executorClientHost;

    uint16_t stateDBServerPort;
    string stateDBURL;

    uint16_t aggregatorServerPort;
    uint16_t aggregatorClientPort;
    string aggregatorClientHost;

    string inputFile;
    string inputFile2; // Used as the second input in genAggregatedProof
    string outputPath;
    string romFile;
    string cmPolsFile;
    string cmPolsFileC12a;
    string cmPolsFileRecursive1;
    string constPolsFile;
    string constPolsC12aFile;
    string constPolsRecursive1File;
    bool mapConstPolsFile;
    string constantsTreeFile;
    string constantsTreeC12aFile;
    string constantsTreeRecursive1File;
    bool mapConstantsTreeFile;
    string starkFile;
    string starkFilec12a;
    string starkFileRecursive1;
    string starkZkIn;
    string starkZkInC12a;
    string starkZkInRecursive1;
    string verifierFile;
    string verifierFileRecursive1;
    string witnessFile;
    string witnessFileRecursive1;
    string execC12aFile;
    string execRecursive1File;
    string starkVerifierFile;
    string publicStarkFile;
    string publicFile;
    string proofFile;
    string keccakScriptFile;
    string keccakPolsFile;
    string keccakConnectionsFile;
    string storageRomFile;
    string starkInfoFile;
    string starkInfoC12aFile;
    string starkInfoRecursive1File;
    string databaseURL;
    string dbNodesTableName;
    string dbProgramTableName;
    bool dbAsyncWrite;
    uint64_t cleanerPollingPeriod;
    uint64_t requestsPersistence;
    uint64_t maxExecutorThreads;
    uint64_t maxProverThreads;
    uint64_t maxStateDBThreads;
    void load(json &config);
    bool generateProof(void) const { return runProverServer || runFileGenProof || runFileGenBatchProof || runFileGenAggregatedProof || runFileGenFinalProof || runAggregatorClient; }
    void print(void);
};

#endif