#include <string>
#include <nlohmann/json.hpp>
#include "config.hpp"

using namespace std;
using json = nlohmann::json;

void Config::load(json &config)
{
    runProverServer = false;
    if (config.contains("runProverServer") && 
        config["runProverServer"].is_boolean())
    {
        runProverServer = config["runProverServer"];
    }
    runProverServerMock = false;
    if (config.contains("runProverServerMock") && 
        config["runProverServerMock"].is_boolean())
    {
        runProverServerMock = config["runProverServerMock"];
    }
    runProverClient = false;
    if (config.contains("runProverClient") && 
        config["runProverClient"].is_boolean())
    {
        runProverClient = config["runProverClient"];
    }
    runExecutorServer = false;
    if (config.contains("runExecutorServer") && 
        config["runExecutorServer"].is_boolean())
    {
        runExecutorServer = config["runExecutorServer"];
    }
    runExecutorClient = false;
    if (config.contains("runExecutorClient") && 
        config["runExecutorClient"].is_boolean())
    {
        runExecutorClient = config["runExecutorClient"];
    }
    runStateDBServer = false;
    if (config.contains("runStateDBServer") && 
        config["runStateDBServer"].is_boolean())
    {
        runStateDBServer = config["runStateDBServer"];
    }
    runStateDBTest = false;
    if (config.contains("runStateDBTest") && 
        config["runStateDBTest"].is_boolean())
    {
        runStateDBTest = config["runStateDBTest"];
    }
    runFile = false;
    if (config.contains("runFile") && 
        config["runFile"].is_boolean())
    {
        runFile = config["runFile"];
    }
    runFileFast = false;
    if (config.contains("runFileFast") && 
        config["runFileFast"].is_boolean())
    {
        runFileFast = config["runFileFast"];
    }
    runKeccakScriptGenerator = false;
    if (config.contains("runKeccakScriptGenerator") && 
        config["runKeccakScriptGenerator"].is_boolean())
    {
        runKeccakScriptGenerator = config["runKeccakScriptGenerator"];
    }
    runKeccakTest = false;
    if (config.contains("runKeccakTest") && 
        config["runKeccakTest"].is_boolean())
    {
        runKeccakTest = config["runKeccakTest"];
    }
    runStorageSMTest = false;
    if (config.contains("runStorageSMTest") && 
        config["runStorageSMTest"].is_boolean())
    {
        runStorageSMTest = config["runStorageSMTest"];
    }
    runBinarySMTest = false;
    if (config.contains("runBinarySMTest") && 
        config["runBinarySMTest"].is_boolean())
    {
        runBinarySMTest = config["runBinarySMTest"];
    }
    runMemAlignSMTest = false;
    if (config.contains("runMemAlignSMTest") && 
        config["runMemAlignSMTest"].is_boolean())
    {
        runMemAlignSMTest = config["runMemAlignSMTest"];
    }
    runStarkTest = false;
    if (config.contains("runStarkTest") && 
        config["runStarkTest"].is_boolean())
    {
        runStarkTest = config["runStarkTest"];
    }
    useMainExecGenerated = false;
    if (config.contains("useMainExecGenerated") && 
        config["useMainExecGenerated"].is_boolean())
    {
        useMainExecGenerated = config["useMainExecGenerated"];
    }
    executeInParallel = false;
    if (config.contains("executeInParallel") && 
        config["executeInParallel"].is_boolean())
    {
        executeInParallel = config["executeInParallel"];
    }
    proverServerPort = 50051;
    if (config.contains("proverServerPort") && 
        config["proverServerPort"].is_number())
    {
        proverServerPort = config["proverServerPort"];
    }
    proverServerMockPort = 50052;
    if (config.contains("proverServerMockPort") && 
        config["proverServerMockPort"].is_number())
    {
        proverServerMockPort = config["proverServerMockPort"];
    }
    proverClientPort = 50051;
    if (config.contains("proverClientPort") && 
        config["proverClientPort"].is_number())
    {
        proverClientPort = config["proverClientPort"];
    }
    proverClientHost = "127.0.0.1";
    if (config.contains("proverClientHost") && 
        config["proverClientHost"].is_string())
    {
        proverClientHost = config["proverClientHost"];
    }
    executorServerPort = 50071;
    if (config.contains("executorServerPort") && 
        config["executorServerPort"].is_number())
    {
        executorServerPort = config["executorServerPort"];
    }
    executorClientPort = 50071;
    if (config.contains("executorClientPort") && 
        config["executorClientPort"].is_number())
    {
        executorClientPort = config["executorClientPort"];
    }
    executorClientHost = "127.0.0.1";
    if (config.contains("executorClientHost") && 
        config["executorClientHost"].is_string())
    {
        executorClientHost = config["executorClientHost"];
    }
    stateDBServerPort = 50061;
    if (config.contains("stateDBServerPort") && 
        config["stateDBServerPort"].is_number())
    {
        stateDBServerPort = config["stateDBServerPort"];
    }    
    stateDBURL = "local";
    if (config.contains("stateDBURL") && 
        config["stateDBURL"].is_string())
    {
        stateDBURL = config["stateDBURL"];
    }    
    if (config.contains("inputFile") && 
        config["inputFile"].is_string())
    {
        inputFile = config["inputFile"];
    }
    if (config.contains("romFile") && 
        config["romFile"].is_string())
    {
        romFile = config["romFile"];
    }
    if (config.contains("outputPath") && 
        config["outputPath"].is_string())
    {
        outputPath = config["outputPath"];
    }
    if (config.contains("cmPolsFile") && 
        config["cmPolsFile"].is_string())
    {
        cmPolsFile = config["cmPolsFile"];
    }
    if (config.contains("constPolsFile") && 
        config["constPolsFile"].is_string())
    {
        constPolsFile = config["constPolsFile"];
    }
    mapConstPolsFile = true;
    if (config.contains("mapConstPolsFile") && 
        config["mapConstPolsFile"].is_boolean())
    {
        mapConstPolsFile = config["mapConstPolsFile"];
    }
    if (config.contains("constantsTreeFile") && 
        config["constantsTreeFile"].is_string())
    {
        constantsTreeFile = config["constantsTreeFile"];
    }
    mapConstantsTreeFile = true;
    if (config.contains("mapConstantsTreeFile") && 
        config["mapConstantsTreeFile"].is_boolean())
    {
        mapConstantsTreeFile = config["mapConstantsTreeFile"];
    }
    if (config.contains("starkFile") && 
        config["starkFile"].is_string())
    {
        starkFile = config["starkFile"];
    }
    if (config.contains("verifierFile") && 
        config["verifierFile"].is_string())
    {
        verifierFile = config["verifierFile"];
    }
    if (config.contains("witnessFile") && 
        config["witnessFile"].is_string())
    {
        witnessFile = config["witnessFile"];
    }
    if (config.contains("execFile") && 
        config["execFile"].is_string())
    {
        execFile = config["execFile"];
    }
    if (config.contains("starkVerifierFile") && 
        config["starkVerifierFile"].is_string())
    {
        starkVerifierFile = config["starkVerifierFile"];
    }
    if (config.contains("proofFile") && 
        config["proofFile"].is_string())
    {
        proofFile = config["proofFile"];
    }
    if (config.contains("publicFile") && 
        config["publicFile"].is_string())
    {
        publicFile = config["publicFile"];
    }
    if (config.contains("keccakScriptFile") && 
        config["keccakScriptFile"].is_string())
    {
        keccakScriptFile = config["keccakScriptFile"];
    }
    if (config.contains("keccakPolsFile") && 
        config["keccakPolsFile"].is_string())
    {
        keccakPolsFile = config["keccakPolsFile"];
    }
    if (config.contains("keccakConnectionsFile") && 
        config["keccakConnectionsFile"].is_string())
    {
        keccakConnectionsFile = config["keccakConnectionsFile"];
    }
    if (config.contains("storageRomFile") && 
        config["storageRomFile"].is_string())
    {
        storageRomFile = config["storageRomFile"];
    }
    if (config.contains("storagePilFile") && 
        config["storagePilFile"].is_string())
    {
        storagePilFile = config["storagePilFile"];
    }
    if (config.contains("storagePolsFile") && 
        config["storagePolsFile"].is_string())
    {
        storagePolsFile = config["storagePolsFile"];
    }
    if (config.contains("memoryPilFile") && 
        config["memoryPilFile"].is_string())
    {
        memoryPilFile = config["memoryPilFile"];
    }
    if (config.contains("memoryPolsFile") && 
        config["memoryPolsFile"].is_string())
    {
        memoryPolsFile = config["memoryPolsFile"];
    }
    if (config.contains("binaryPilFile") && 
        config["binaryPilFile"].is_string())
    {
        binaryPilFile = config["binaryPilFile"];
    }
    if (config.contains("binaryPolsFile") && 
        config["binaryPolsFile"].is_string())
    {
        binaryPolsFile = config["binaryPolsFile"];
    }
    if (config.contains("binaryConstPolsFile") && 
        config["binaryConstPolsFile"].is_string())
    {
        binaryConstPolsFile = config["binaryConstPolsFile"];
    }
    if (config.contains("starkInfoFile") && 
        config["starkInfoFile"].is_string())
    {
        starkInfoFile = config["starkInfoFile"];
    }
    if (config.contains("stateDBURL") && 
        config["stateDBURL"].is_string())
    {
        stateDBURL = config["stateDBURL"];
    }    
    if (config.contains("databaseURL") && 
        config["databaseURL"].is_string())
    {
        databaseURL = config["databaseURL"];
    }
    if (config.contains("dbTableName") && 
        config["dbTableName"].is_string())
    {
        dbTableName = config["dbTableName"];
    }
    dbAsyncWrite = false;
    if (config.contains("dbAsyncWrite") && 
        config["dbAsyncWrite"].is_boolean())
    {
        dbAsyncWrite = config["dbAsyncWrite"];
    }    
    if (config.contains("cleanerPollingPeriod") && 
        config["cleanerPollingPeriod"].is_number())
    {
        cleanerPollingPeriod = config["cleanerPollingPeriod"];
    }
    if (config.contains("requestsPersistence") && 
        config["requestsPersistence"].is_number())
    {
        requestsPersistence = config["requestsPersistence"];
    }
}

void Config::print (void)
{
    cout << "Configuration:" << endl;
    if (runProverServer) cout << "runProverServer=true" << endl;
    if (runProverServerMock) cout << "runProverServerMock=true" << endl;
    if (runProverClient) cout << "runProverClient=true" << endl;
    if (runExecutorServer) cout << "runExecutorServer=true" << endl;
    if (runExecutorClient) cout << "runExecutorClient=true" << endl;
    if (runStateDBServer) cout << "runStateDBServer=true" << endl;
    if (runStateDBTest) cout << "runStateDBTest=true" << endl;
    if (runFile) cout << "runFile=true" << endl;
    if (runFileFast) cout << "runFileFast=true" << endl;
    if (runKeccakScriptGenerator) cout << "runKeccakScriptGenerator=true" << endl;
    if (runKeccakTest) cout << "runKeccakTest=true" << endl;
    if (runStorageSMTest) cout << "runStorageSMTest=true" << endl;
    if (runBinarySMTest) cout << "runBinarySMTest=true" << endl;
    if (runMemAlignSMTest) cout << "runMemAlignSMTest=true" << endl;
    if (runStarkTest) cout << "runStarkTest=true" << endl;
    if (executeInParallel) cout << "executeInParallel=true" << endl;
    if (useMainExecGenerated) cout << "useMainExecGenerated=true" << endl;
    cout << "proverServerPort=" << to_string(proverServerPort) << endl;
    cout << "proverServerMockPort=" << to_string(proverServerMockPort) << endl;
    cout << "proverClientPort=" << to_string(proverClientPort) << endl;
    cout << "proverClientHost=" << proverClientHost << endl;
    cout << "executorServerPort=" << to_string(executorServerPort) << endl;
    cout << "executorClientPort=" << to_string(executorClientPort) << endl;
    cout << "executorClientHost=" << executorClientHost << endl;
    cout << "stateDBServerPort=" << to_string(stateDBServerPort) << endl;
    cout << "stateDBURL=" << stateDBURL << endl;
    cout << "inputFile=" << inputFile << endl;
    cout << "outputPath=" << outputPath << endl;
    cout << "romFile=" << romFile << endl;
    cout << "cmPolsFile=" << cmPolsFile << endl;
    cout << "constPolsFile=" << constPolsFile << endl;
    if (mapConstPolsFile) cout << "mapConstPolsFile=true" << endl;
    cout << "constantsTreeFile=" << constantsTreeFile << endl;
    if (mapConstantsTreeFile) cout << "mapConstantsTreeFile=true" << endl;
    cout << "starkFile=" << starkFile << endl;
    cout << "verifierFile=" << verifierFile << endl;
    cout << "witnessFile=" << witnessFile << endl;
    cout << "starkVerifierFile=" << starkVerifierFile << endl;
    cout << "publicFile=" << publicFile << endl;
    cout << "proofFile=" << proofFile << endl;
    cout << "keccakScriptFile=" << keccakScriptFile << endl;
    cout << "keccakPolsFile=" << keccakPolsFile << endl;
    cout << "keccakConnectionsFile=" << keccakConnectionsFile << endl;
    cout << "storageRomFile=" << storageRomFile << endl;
    cout << "storagePilFile=" << storagePilFile << endl;
    cout << "storagePolsFile=" << storagePolsFile << endl;
    cout << "memoryPilFile=" << memoryPilFile << endl;
    cout << "memoryPolsFile=" << memoryPolsFile << endl;
    cout << "binaryPilFile=" << binaryPilFile << endl;
    cout << "binaryPolsFile=" << binaryPolsFile << endl;
    cout << "binaryConstPolsFile=" << binaryConstPolsFile << endl;
    cout << "starkInfoFile=" << starkInfoFile << endl;
    cout << "databaseURL=" << databaseURL << endl;
    cout << "dbTableName=" << dbTableName << endl;
    cout << "dbAsyncWrite=" << to_string(dbAsyncWrite) << endl;
    cout << "cleanerPollingPeriod=" << cleanerPollingPeriod << endl;
    cout << "requestsPersistence=" << requestsPersistence << endl;
}