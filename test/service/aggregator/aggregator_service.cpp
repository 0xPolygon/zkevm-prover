#include "config.hpp"
#include "aggregator_service.hpp"
#include "input.hpp"
#include "proof.hpp"
#include <grpcpp/grpcpp.h>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

::grpc::Status AggregatorServiceImpl::Channel(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::aggregator::v1::AggregatorMessage, ::aggregator::v1::ProverMessage>* stream)
{
#ifdef LOG_SERVICE
    cout << "AggregatorServiceImpl::Channel() stream starts" << endl;
#endif
    aggregator::v1::AggregatorMessage aggregatorMessage;
    aggregator::v1::ProverMessage proverMessage;
    bool bResult;
    string uuid;

    //while (true)
    {
        // CALL GET STATUS

        // Send a get status request message
        aggregatorMessage.Clear();
        aggregator::v1::GetStatusRequest * pGetStatusRequest = new aggregator::v1::GetStatusRequest();
        zkassert(pGetStatusRequest != NULL);
        aggregatorMessage.set_allocated_get_status_request(pGetStatusRequest);
        messageId++;
        aggregatorMessage.set_id(to_string(messageId));
        bResult = stream->Write(aggregatorMessage);
        if (!bResult)
        {
            cerr << "AggregatorServiceImpl::Channel() failed calling stream->Write(aggregatorMessage)" << endl;
            return Status::CANCELLED;
        }

        // Receive the corresponding get status response message
        proverMessage.Clear();
        bResult = stream->Read(&proverMessage);
        if (!bResult)
        {
            cerr << "AggregatorServiceImpl::Channel() failed calling stream->Read(proverMessage)" << endl;
            return Status::CANCELLED;
        }
        
        // Check type
        if (proverMessage.response_case() != aggregator::v1::ProverMessage::ResponseCase::kGetStatusResponse)
        {
            cerr << "AggregatorServiceImpl::Channel() got proverMessage.response_case=" << proverMessage.response_case() << " instead of GET_STATUS_RESPONSE" << endl;
            return Status::CANCELLED;
        }

        // Check id
        if (proverMessage.id() != aggregatorMessage.id())
        {
            cerr << "AggregatorServiceImpl::Channel() got proverMessage.id=" << proverMessage.id() << " instead of aggregatorMessage.id=" << aggregatorMessage.id() << endl;
            return Status::CANCELLED;
        }

        sleep(1);

        // CALL CANCEL (it should return an error)

        // Send a cancel request message
        aggregatorMessage.Clear();
        messageId++;
        aggregatorMessage.set_id(to_string(messageId));
        aggregator::v1::CancelRequest * pCancelRequest = new aggregator::v1::CancelRequest();
        zkassert(pCancelRequest != NULL);
        pCancelRequest->set_id("invalid_id");
        aggregatorMessage.set_allocated_cancel_request(pCancelRequest);
        bResult = stream->Write(aggregatorMessage);
        if (!bResult)
        {
            cerr << "AggregatorServiceImpl::Channel() failed calling stream->Write(aggregatorMessage)" << endl;
            return Status::CANCELLED;
        }

        // Receive the corresponding cancel response message
        proverMessage.Clear();
        bResult = stream->Read(&proverMessage);
        if (!bResult)
        {
            cerr << "AggregatorServiceImpl::Channel() failed calling stream->Read(proverMessage)" << endl;
            return Status::CANCELLED;
        }
        
        // Check type
        if (proverMessage.response_case() != aggregator::v1::ProverMessage::ResponseCase::kCancelResponse)
        {
            cerr << "AggregatorServiceImpl::Channel() got proverMessage.response_case=" << proverMessage.response_case() << " instead of CANCEL_RESPONSE" << endl;
            return Status::CANCELLED;
        }

        // Check id
        if (proverMessage.id() != aggregatorMessage.id())
        {
            cerr << "AggregatorServiceImpl::Channel() got proverMessage.id=" << proverMessage.id() << " instead of aggregatorMessage.id=" << aggregatorMessage.id() << endl;
            return Status::CANCELLED;
        }

        // Check cancel result
        if (proverMessage.cancel_response().result() != aggregator::v1::Result::ERROR)
        {
            cerr << "AggregatorServiceImpl::Channel() got proverMessage.cancel_response().result()=" << proverMessage.cancel_response().result() << " instead of RESULT_CANCEL_ERROR" << endl;
            return Status::CANCELLED;
        }

        sleep(1);

        // Call GEN PROOF

        if (config.inputFile.size() == 0)
        {
            cerr << "Error: AggregatorServiceImpl::Channel() found config.inputFile empty" << endl;
            exitProcess();
        }
    //::grpc::ClientContext context;
        aggregator::v1::InputProver *pInputProver = new aggregator::v1::InputProver();
        zkassert(pInputProver != NULL);
        Input input(fr);
        json inputJson;
        file2json(config.inputFile, inputJson);
        zkresult zkResult = input.load(inputJson);
        if (zkResult != ZKR_SUCCESS)
        {
            cerr << "Error: AggregatorServiceImpl::Channel() failed calling input.load() zkResult=" << zkResult << "=" << zkresult2string(zkResult) << endl;
            exitProcess();
        }

        // Parse public inputs
        aggregator::v1::PublicInputs * pPublicInputs = new aggregator::v1::PublicInputs();
        pPublicInputs->set_old_state_root(string2ba(input.publicInputsExtended.publicInputs.oldStateRoot));
        pPublicInputs->set_old_acc_input_hash(string2ba(input.publicInputsExtended.publicInputs.oldAccInputHash));
        pPublicInputs->set_old_batch_num(input.publicInputsExtended.publicInputs.oldBatchNum);
        pPublicInputs->set_chain_id(input.publicInputsExtended.publicInputs.chainID);
        pPublicInputs->set_batch_l2_data(string2ba(input.publicInputsExtended.publicInputs.batchL2Data));
        pPublicInputs->set_global_exit_root(string2ba(input.publicInputsExtended.publicInputs.globalExitRoot));
        pPublicInputs->set_eth_timestamp(input.publicInputsExtended.publicInputs.timestamp);
        pPublicInputs->set_sequencer_addr(input.publicInputsExtended.publicInputs.sequencerAddr);
        pPublicInputs->set_aggregator_addr(input.publicInputsExtended.publicInputs.aggregatorAddress);
        pInputProver->set_allocated_public_inputs(pPublicInputs);

        // Parse keys map
        DatabaseMap::MTMap::const_iterator it;
        for (it=input.db.begin(); it!=input.db.end(); it++)
        {
            string key = NormalizeToNFormat(it->first, 64);
            string value;
            vector<Goldilocks::Element> dbValue = it->second;
            for (uint64_t i=0; i<dbValue.size(); i++)
            {
                value += NormalizeToNFormat(fr.toString(dbValue[i], 16), 16);
            }
            (*pInputProver->mutable_db())[key] = value;
        }

        // Parse contracts data
        DatabaseMap::ProgramMap::const_iterator itc;
        for (itc=input.contractsBytecode.begin(); itc!=input.contractsBytecode.end(); itc++)
        {
            string key = NormalizeToNFormat(itc->first, 64);
            string value;
            vector<uint8_t> contractValue = itc->second;
            for (uint64_t i=0; i<contractValue.size(); i++)
            {
                value += byte2string(contractValue[i]);
            }
            (*pInputProver->mutable_contracts_bytecode())[key] = value;
        }

        // Allocate the gen proof request
        aggregator::v1::GenProofRequest *pGenProofRequest = new aggregator::v1::GenProofRequest();
        zkassert(pGenProofRequest != NULL );
        pGenProofRequest->set_allocated_input(pInputProver);

        // Send the gen proof request
        aggregatorMessage.Clear();
        messageId++;
        aggregatorMessage.set_id(to_string(messageId));
        aggregatorMessage.set_allocated_gen_proof_request(pGenProofRequest);
        bResult = stream->Write(aggregatorMessage);
        if (!bResult)
        {
            cerr << "AggregatorServiceImpl::Channel() failed calling stream->Write(aggregatorMessage)" << endl;
            return Status::CANCELLED;
        }

        // Receive the corresponding get proof response message
        proverMessage.Clear();
        bResult = stream->Read(&proverMessage);
        if (!bResult)
        {
            cerr << "AggregatorServiceImpl::Channel() failed calling stream->Read(proverMessage)" << endl;
            return Status::CANCELLED;
        }
        
        // Check type
        if (proverMessage.response_case() != aggregator::v1::ProverMessage::ResponseCase::kGenProofResponse)
        {
            cerr << "AggregatorServiceImpl::Channel() got proverMessage.response_case=" << proverMessage.response_case() << " instead of GEN_PROOF_RESPONSE" << endl;
            return Status::CANCELLED;
        }

        // Check id
        if (proverMessage.id() != aggregatorMessage.id())
        {
            cerr << "AggregatorServiceImpl::Channel() got proverMessage.id=" << proverMessage.id() << " instead of aggregatorMessage.id=" << aggregatorMessage.id() << endl;
            return Status::CANCELLED;
        }

        uuid = proverMessage.gen_proof_response().id();

        // CALL GET PROOF AND CHECK IT IS PENDING

        // Send a get proof request message
        aggregatorMessage.Clear();
        messageId++;
        aggregatorMessage.set_id(to_string(messageId));
        aggregator::v1::GetProofRequest * pGetProofRequest = new aggregator::v1::GetProofRequest();
        zkassert(pGetProofRequest != NULL);
        pGetProofRequest->set_id(uuid);
        aggregatorMessage.set_allocated_get_proof_request(pGetProofRequest);
        bResult = stream->Write(aggregatorMessage);
        if (!bResult)
        {
            cerr << "AggregatorServiceImpl::Channel() failed calling stream->Write(aggregatorMessage)" << endl;
            return Status::CANCELLED;
        }

        // Receive the corresponding get proof response message
        proverMessage.Clear();
        bResult = stream->Read(&proverMessage);
        if (!bResult)
        {
            cerr << "AggregatorServiceImpl::Channel() failed calling stream->Read(proverMessage)" << endl;
            return Status::CANCELLED;
        }
        
        // Check type
        if (proverMessage.response_case() != aggregator::v1::ProverMessage::ResponseCase::kGetProofResponse)
        {
            cerr << "AggregatorServiceImpl::Channel() got proverMessage.response_case=" << proverMessage.response_case() << " instead of GET_PROOF_RESPONSE" << endl;
            return Status::CANCELLED;
        }

        // Check id
        if (proverMessage.id() != aggregatorMessage.id())
        {
            cerr << "AggregatorServiceImpl::Channel() got proverMessage.id=" << proverMessage.id() << " instead of aggregatorMessage.id=" << aggregatorMessage.id() << endl;
            return Status::CANCELLED;
        }

        // Check get proof result
        if (proverMessage.get_proof_response().result() != aggregator::v1::GetProofResponse_Result_PENDING)
        {
            cerr << "AggregatorServiceImpl::Channel() got proverMessage.get_proof_response().result()=" << proverMessage.get_proof_response().result() << " instead of RESULT_GET_PROOF_PENDING" << endl;
            return Status::CANCELLED;
        }

        sleep(1);
    }

#ifdef LOG_SERVICE
    cout << "AggregatorServiceImpl::Channel() stream done" << endl;
#endif

    return Status::OK;
}