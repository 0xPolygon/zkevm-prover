#include "config.hpp"
#include "prover_service.hpp"
#include "input.hpp"
#include "proof.hpp"
#include <grpcpp/grpcpp.h>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

::grpc::Status ZKProverServiceImpl::GetStatus(::grpc::ServerContext* context, const ::zkprover::v1::GetStatusRequest* request, ::zkprover::v1::GetStatusResponse* response)
{
    // Lock the prover
    prover.lock();

    // Set last computed request data
    response->set_last_computed_request_id(prover.lastComputedRequestId);
    response->set_last_computed_end_time(prover.lastComputedRequestEndTime);

    // If computing, set the current request data
    if ((prover.pCurrentRequest != NULL) || (prover.pendingRequests.size() > 0))
    {
        response->set_state(zkprover::v1::GetStatusResponse_StatusProver_STATUS_PROVER_COMPUTING);
        if (prover.pCurrentRequest != NULL)
        {
            response->set_current_computing_request_id(prover.pCurrentRequest->uuid);
            response->set_current_computing_start_time(prover.pCurrentRequest->startTime);
        }
        else
        {
            response->set_current_computing_request_id("");
            response->set_current_computing_start_time(0);
        }
    }
    else
    {
        response->set_state(zkprover::v1::GetStatusResponse_StatusProver_STATUS_PROVER_IDLE);
        response->set_current_computing_request_id("");
        response->set_current_computing_start_time(0);
    }

    // Set the versions
    response->set_version_proto("v0_0_1");
    response->set_version_server("0.0.1");

    // Set the list of pending requests uuids
    for (uint64_t i=0; i<prover.pendingRequests.size(); i++)
    {
        response->add_pending_request_queue_ids(prover.pendingRequests[i]->uuid);
    }

    // Unlock the prover
    prover.unlock();

#ifdef LOG_SERVICE
    cout << "ZKProverServiceImpl::GetStatus() returns: " << response->DebugString() << endl;
#endif
    return Status::OK;
}

::grpc::Status ZKProverServiceImpl::GenProof(::grpc::ServerContext* context, const ::zkprover::v1::GenProofRequest* request, ::zkprover::v1::GenProofResponse* response)
{
#ifdef LOG_SERVICE
    cout << "ZKProverServiceImpl::GenProof() called with request: " << request->DebugString() << endl;
#endif
    ProverRequest * pProverRequest = new ProverRequest(fr, config, prt_genProof);
    if (pProverRequest == NULL)
    {
        cerr << "ZKProverServiceImpl::GenProof() failed allocation a new ProveRequest" << endl;
        exitProcess();
    }
#ifdef LOG_SERVICE
    cout << "ZKProverServiceImpl::GenProof() created a new prover request: " << to_string((uint64_t)pProverRequest) << endl;
#endif

    // Parse public inputs

    pProverRequest->input.publicInputsExtended.publicInputs.oldStateRoot = "0x" + ba2string(request->input().public_inputs().old_state_root());
    if (pProverRequest->input.publicInputsExtended.publicInputs.oldStateRoot.size() > (2 + 64))
    {
        cerr << "Error: ZKProverServiceImpl::GenProof() got oldStateRoot too long, size=" << pProverRequest->input.publicInputsExtended.publicInputs.oldStateRoot.size() << endl;
        return Status::CANCELLED;
    }

    pProverRequest->input.publicInputsExtended.publicInputs.oldAccInputHash = "0x" + ba2string(request->input().public_inputs().old_acc_input_hash());
    if (pProverRequest->input.publicInputsExtended.publicInputs.oldAccInputHash.size() > (2 + 64))
    {
        cerr << "Error: ZKProverServiceImpl::GenProof() got oldAccInputHash too long, size=" << pProverRequest->input.publicInputsExtended.publicInputs.oldAccInputHash.size() << endl;
        return Status::CANCELLED;
    }
    
    pProverRequest->input.publicInputsExtended.publicInputs.oldBatchNum = request->input().public_inputs().old_batch_num();

    pProverRequest->input.publicInputsExtended.publicInputs.chainID = request->input().public_inputs().chain_id();
    if (pProverRequest->input.publicInputsExtended.publicInputs.chainID == 0)
    {
        cerr << "Error: ZKProverServiceImpl::GenProof() got chainID = 0" << endl;
        return Status::CANCELLED;
    }

    pProverRequest->input.publicInputsExtended.publicInputs.batchL2Data = "0x" + ba2string(request->input().public_inputs().batch_l2_data());
    if (pProverRequest->input.publicInputsExtended.publicInputs.batchL2Data.size() > (MAX_BATCH_L2_DATA_SIZE*2 + 2))
    {
        cerr << "Error: ZKProverServiceImpl::GenProof() found batchL2Data.size()=" << pProverRequest->input.publicInputsExtended.publicInputs.batchL2Data.size() << " > (MAX_BATCH_L2_DATA_SIZE*2+2)=" << (MAX_BATCH_L2_DATA_SIZE*2+2) << endl;
        return Status::CANCELLED;
   }

    pProverRequest->input.publicInputsExtended.publicInputs.globalExitRoot = "0x" + ba2string(request->input().public_inputs().global_exit_root());
    if (pProverRequest->input.publicInputsExtended.publicInputs.globalExitRoot.size() > (2 + 64))
    {
        cerr << "Error: ZKProverServiceImpl::GenProof() got globalExitRoot too long, size=" << pProverRequest->input.publicInputsExtended.publicInputs.globalExitRoot.size() << endl;
        return Status::CANCELLED;
   }

    pProverRequest->input.publicInputsExtended.publicInputs.timestamp = request->input().public_inputs().eth_timestamp();

    pProverRequest->input.publicInputsExtended.publicInputs.sequencerAddr = Add0xIfMissing(request->input().public_inputs().sequencer_addr());
    if (pProverRequest->input.publicInputsExtended.publicInputs.sequencerAddr.size() > (2 + 40))
    {
        cerr << "Error: ZKProverServiceImpl::GenProof() got sequencerAddr too long, size=" << pProverRequest->input.publicInputsExtended.publicInputs.sequencerAddr.size() << endl;
        return Status::CANCELLED;
    }

    pProverRequest->input.publicInputsExtended.publicInputs.aggregatorAddress = Add0xIfMissing(request->input().public_inputs().aggregator_addr());
    if (pProverRequest->input.publicInputsExtended.publicInputs.aggregatorAddress.size() > (2 + 40))
    {
        cerr << "Error: ZKProverServiceImpl::GenProof() got aggregator address too long, size=" << pProverRequest->input.publicInputsExtended.publicInputs.aggregatorAddress.size() << endl;
        return Status::CANCELLED;
    }

    // Parse keys map
    google::protobuf::Map<std::__cxx11::basic_string<char>, std::__cxx11::basic_string<char> > db;
    db = request->input().db();
    google::protobuf::Map<std::__cxx11::basic_string<char>, std::__cxx11::basic_string<char> >::iterator it;
    for (it=db.begin(); it!=db.end(); it++)
    {
        if (it->first.size() > (64))
        {
            cerr << "Error: ZKProverServiceImpl::GenProof() got db key too long, size=" << it->first.size() << endl;
            return Status::CANCELLED;
        }
        vector<Goldilocks::Element> dbValue;
        string concatenatedValues = it->second;
        if (concatenatedValues.size()%16!=0)
        {
            cerr << "Error: ZKProverServiceImpl::GenProof() found invalid db value size: " << concatenatedValues.size() << endl;
            return Status::CANCELLED;
        }
        for (uint64_t i=0; i<concatenatedValues.size(); i+=16)
        {
            Goldilocks::Element fe;
            string2fe(fr, concatenatedValues.substr(i, 16), fe);
            dbValue.push_back(fe);
        }
        pProverRequest->input.db[it->first] = dbValue;
    }

    // Parse contracts data
    google::protobuf::Map<std::__cxx11::basic_string<char>, std::__cxx11::basic_string<char> > contractsBytecode;
    contractsBytecode = request->input().contracts_bytecode();
    google::protobuf::Map<std::__cxx11::basic_string<char>, std::__cxx11::basic_string<char> >::iterator itp;
    for (itp=contractsBytecode.begin(); itp!=contractsBytecode.end(); itp++)
    {
        /*if (it->first.size() != (2+64))
        {
            cerr << "Error: ZKProverServiceImpl::GenProof() got contracts bytecode key too long, size=" << it->first.size() << endl;
            return Status::CANCELLED;
        }*/
        vector<uint8_t> dbValue;
        string contractValue = string2ba(itp->second);
        for (uint64_t i=0; i<contractValue.size(); i++)
        {
            dbValue.push_back(contractValue.at(i));
        }
        pProverRequest->input.contractsBytecode[itp->first] = dbValue;
    }

    // Submit the prover request
    string uuid = prover.submitRequest(pProverRequest);

    // Build the response as Ok, returning the UUID assigned by the prover to this request
    response->set_result(zkprover::v1::GenProofResponse_ResultGenProof_RESULT_GEN_PROOF_OK);
    response->set_id(uuid.c_str());

#ifdef LOG_SERVICE
    cout << "ZKProverServiceImpl::GenProof() returns: " << response->DebugString() << endl;
#endif

    return Status::OK;
}

::grpc::Status ZKProverServiceImpl::Cancel(::grpc::ServerContext* context, const ::zkprover::v1::CancelRequest* request, ::zkprover::v1::CancelResponse* response)
{
    // Get the prover request UUID
    string uuid = request->id();

    // Lock the prover
    prover.lock();

    // Map uuid to the corresponding prover request
    std::unordered_map<std::string, ProverRequest *>::iterator it = prover.requestsMap.find(uuid);
    if (it == prover.requestsMap.end())
    {
        prover.unlock();
        cerr << "ZKProverServiceImpl::Cancel() unknown uuid: " << uuid << endl;
        response->set_result(zkprover::v1::CancelResponse_ResultCancel_RESULT_CANCEL_ERROR);
        return Status::OK;
    }

    // Check if it is already completed
    if (it->second->bCompleted)
    {
        prover.unlock();
        cerr << "ZKProverServiceImpl::Cancel() already completed uuid: " << uuid << endl;
        response->set_result(zkprover::v1::CancelResponse_ResultCancel_RESULT_CANCEL_ERROR);
        return Status::OK;
    }

    // Mark the request as cancelling
    it->second->bCancelling = true;

    // Unlock the prover
    prover.unlock();

    response->set_result(zkprover::v1::CancelResponse_ResultCancel_RESULT_CANCEL_OK);

#ifdef LOG_SERVICE
    cout << "ZKProverServiceImpl::Cancel() returns: " << response->DebugString() << endl;
#endif
    
    return Status::OK;
}

::grpc::Status ZKProverServiceImpl::GetProof(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::zkprover::v1::GetProofResponse, ::zkprover::v1::GetProofRequest>* stream)
{
#ifdef LOG_SERVICE
    cout << "ZKProverServiceImpl::GetProof() stream starts" << endl;
#endif
    zkprover::v1::GetProofRequest request;
    while (stream->Read(&request))
    {
#ifdef LOG_SERVICE
        cout << "ZKProverServiceImpl::GetProof() received request: " << request.DebugString();
#endif
        // Get the prover request UUID from the request
        string uuid = request.id();

        zkprover::v1::GetProofResponse response;

        // Lock the prover
        prover.lock();

        // Map uuid to the corresponding prover request
        std::unordered_map<std::string, ProverRequest *>::iterator it = prover.requestsMap.find(uuid);

        // If UUID is not found, return the proper error
        if (it == prover.requestsMap.end())
        {
            cerr << "ZKProverServiceImpl::GetProof() invalid uuid:" << uuid << endl;
            response.set_result(zkprover::v1::GetProofResponse_ResultGetProof_RESULT_GET_PROOF_ERROR);
            response.set_result_string("invalid UUID");
        }
        else
        {
            ProverRequest * pProverRequest = it->second;

            // If request is not completed, return the proper result
            if (!pProverRequest->bCompleted)
            {
                //cerr << "ZKProverServiceImpl::GetProof() not completed uuid=" << uuid << endl;
                response.set_result(zkprover::v1::GetProofResponse_ResultGetProof_RESULT_GET_PROOF_PENDING);
                response.set_result_string("pending");
            }
            // If request is completed, return the proof
            else
            {
                // Request is completed
                response.set_id(uuid);
                if (pProverRequest->result != ZKR_SUCCESS)
                {
                    response.set_result(zkprover::v1::GetProofResponse_ResultGetProof_RESULT_GET_PROOF_COMPLETED_ERROR);
                    response.set_result_string("completed_error");
                }
                else
                {
                    response.set_result(zkprover::v1::GetProofResponse_ResultGetProof_RESULT_GET_PROOF_COMPLETED_OK);
                    response.set_result_string("completed");
                }

                // Convert the returned Proof to zkprover::Proof

                zkprover::v1::Proof * pProofProver = new zkprover::v1::Proof();

                // Set proofA
                for (uint64_t i=0; i<pProverRequest->proof.proofA.size(); i++)
                {
                    pProofProver->add_proof_a(pProverRequest->proof.proofA[i]);
                }

                // Set proofB
                for (uint64_t i=0; i<pProverRequest->proof.proofB.size(); i++)
                {
                    zkprover::v1::ProofB *pProofB = pProofProver->add_proof_b();
                    for (uint64_t j=0; j<pProverRequest->proof.proofB[i].proof.size(); j++)
                    {
                        pProofB->add_proofs(pProverRequest->proof.proofB[i].proof[j]);
                    }
                }

                // Set proofC
                for (uint64_t i=0; i<pProverRequest->proof.proofC.size(); i++)
                {
                    pProofProver->add_proof_c(pProverRequest->proof.proofC[i]);
                }

                response.set_allocated_proof(pProofProver);

                // Set public inputs extended
                zkprover::v1::PublicInputs* pPublicInputs = new(zkprover::v1::PublicInputs);
                pPublicInputs->set_old_state_root(string2ba(pProverRequest->proof.publicInputsExtended.publicInputs.oldStateRoot));
                pPublicInputs->set_old_acc_input_hash(string2ba(pProverRequest->proof.publicInputsExtended.publicInputs.oldAccInputHash));
                pPublicInputs->set_old_batch_num(pProverRequest->proof.publicInputsExtended.publicInputs.chainID);
                pPublicInputs->set_chain_id(pProverRequest->proof.publicInputsExtended.publicInputs.timestamp);
                pPublicInputs->set_batch_l2_data(string2ba(pProverRequest->proof.publicInputsExtended.publicInputs.batchL2Data));
                pPublicInputs->set_global_exit_root(string2ba(pProverRequest->proof.publicInputsExtended.publicInputs.globalExitRoot));
                pPublicInputs->set_eth_timestamp(pProverRequest->proof.publicInputsExtended.publicInputs.timestamp);
                pPublicInputs->set_sequencer_addr(pProverRequest->proof.publicInputsExtended.publicInputs.sequencerAddr);
                pPublicInputs->set_aggregator_addr(pProverRequest->proof.publicInputsExtended.publicInputs.aggregatorAddress);
                zkprover::v1::PublicInputsExtended* pPublicInputsExtended = new(zkprover::v1::PublicInputsExtended);
                pPublicInputsExtended->set_allocated_public_inputs(pPublicInputs);
                pPublicInputsExtended->set_new_state_root(string2ba(pProverRequest->proof.publicInputsExtended.newStateRoot));
                pPublicInputsExtended->set_new_acc_input_hash(string2ba(pProverRequest->proof.publicInputsExtended.newAccInputHash));
                pPublicInputsExtended->set_new_local_exit_root(string2ba(pProverRequest->proof.publicInputsExtended.newLocalExitRoot));
                pPublicInputsExtended->set_new_batch_num(pProverRequest->proof.publicInputsExtended.newBatchNum);
                response.set_allocated_public_(pPublicInputsExtended);
            }
        }
        
        prover.unlock();

#ifdef LOG_SERVICE
        cout << "ZKProverServiceImpl::GetProof() sends response: " << response.DebugString();
#endif
        // Return the response via the stream
        stream->Write(response);
    }

#ifdef LOG_SERVICE
    cout << "ZKProverServiceImpl::GetProof() stream done" << endl;
#endif

    return Status::OK;
}