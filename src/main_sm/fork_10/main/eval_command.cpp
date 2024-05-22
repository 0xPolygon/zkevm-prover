#include <iostream>
#include "definitions.hpp"
#include "config.hpp"
#include "main_sm/fork_10/main/eval_command.hpp"
#include "main_sm/fork_10/main/main_exec_required.hpp"
#include "scalar.hpp"
#include "utils.hpp"
#include "zkassert.hpp"
#include "zklog.hpp"
#include "zkglobals.hpp"
#include "BLS12_381_utils.hpp"
#include "main_definitions.hpp"
#include "arith_executor.hpp"

namespace fork_10
{

#ifdef DEBUG
#define CHECK_EVAL_COMMAND_PARAMETERS
#endif

void evalCommand (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
    if (cmd.op == op_functionCall)
    {
        switch (cmd.function)
        {
            case f_getGlobalExitRoot:               return eval_getGlobalExitRoot(ctx, cmd, cr);
            case f_getSequencerAddr:                return eval_getSequencerAddr(ctx, cmd, cr);
            case f_getTxs:                          return eval_getTxs(ctx, cmd, cr);
            case f_getTxsLen:                       return eval_getTxsLen(ctx, cmd, cr);
            case f_eventLog:                        return eval_eventLog(ctx, cmd, cr);
            case f_cond:                            return eval_cond(ctx, cmd, cr);
            case f_inverseFpEc:                     return eval_inverseFpEc(ctx, cmd, cr);
            case f_inverseFnEc:                     return eval_inverseFnEc(ctx, cmd, cr);
            case f_sqrtFpEc:                        return eval_sqrtFpEc(ctx, cmd, cr);
            case f_sqrtFpEcParity:                  return eval_sqrtFpEcParity(ctx, cmd, cr);
            case f_xAddPointEc:                     return eval_xAddPointEc(ctx, cmd, cr);
            case f_yAddPointEc:                     return eval_yAddPointEc(ctx, cmd, cr);
            case f_xDblPointEc:                     return eval_xDblPointEc(ctx, cmd, cr);
            case f_yDblPointEc:                     return eval_yDblPointEc(ctx, cmd, cr);
            case f_bitwise_and:                     return eval_bitwise_and(ctx, cmd, cr);
            case f_bitwise_or:                      return eval_bitwise_or(ctx, cmd, cr);
            case f_bitwise_xor:                     return eval_bitwise_xor(ctx, cmd, cr);
            case f_bitwise_not:                     return eval_bitwise_not(ctx, cmd, cr);
            case f_comp_lt:                         return eval_comp_lt(ctx, cmd, cr);
            case f_comp_gt:                         return eval_comp_gt(ctx, cmd, cr);
            case f_comp_eq:                         return eval_comp_eq(ctx, cmd, cr);
            case f_loadScalar:                      return eval_loadScalar(ctx, cmd, cr);
            case f_log:                             return eval_log(ctx, cmd, cr);
            case f_exp:                             return eval_exp(ctx, cmd, cr);
            case f_storeLog:                        return eval_storeLog(ctx, cmd, cr);
            case f_memAlignWR_W0:                   return eval_memAlignWR_W0(ctx, cmd, cr);
            case f_memAlignWR_W1:                   return eval_memAlignWR_W1(ctx, cmd, cr);
            case f_memAlignRD:                      return eval_memAlignRD(ctx, cmd, cr);
            case f_beforeLast:                      return eval_beforeLast(ctx, cmd, cr);

            // Etrog (fork 7) new methods:
            case f_getL1InfoRoot:                   return eval_getL1InfoRoot(ctx, cmd, cr);
            case f_getL1InfoGER:                    return eval_getL1InfoGER(ctx, cmd, cr);
            case f_getL1InfoBlockHash:              return eval_getL1InfoBlockHash(ctx, cmd, cr);
            case f_getL1InfoTimestamp:              return eval_getL1InfoTimestamp(ctx, cmd, cr);
            case f_getL1InfoMinTimestamp:           return eval_getL1InfoMinTimestamp(ctx, cmd, cr);
            case f_getTimestampLimit:               return eval_getTimestampLimit(ctx, cmd, cr);
            case f_getForcedBlockHashL1:            return eval_getForcedBlockHashL1(ctx, cmd, cr);
            case f_getSmtProof:                     return eval_getSmtProof(ctx, cmd, cr);
            case f_MPdiv:                           return eval_MPdiv(ctx, cmd, cr);
            case f_MPdiv_short:                     return eval_MPdiv_short(ctx, cmd, cr);
            case f_receiveLenQuotient_short:        return eval_receiveLenQuotient_short(ctx, cmd, cr);
            case f_receiveQuotientChunk_short:      return eval_receiveQuotientChunk_short(ctx, cmd, cr);
            case f_receiveRemainderChunk_short:     return eval_receiveRemainderChunk_short(ctx, cmd, cr);
            case f_receiveLenRemainder:             return eval_receiveLenRemainder(ctx, cmd, cr);
            case f_receiveRemainderChunk:           return eval_receiveRemainderChunk(ctx, cmd, cr);
            case f_receiveLenQuotient:              return eval_receiveLenQuotient(ctx, cmd, cr);
            case f_receiveQuotientChunk:            return eval_receiveQuotientChunk(ctx, cmd, cr);
            case f_receiveLen:                      return eval_receiveLen(ctx, cmd, cr);
            case f_ARITH_BN254_ADDFP2:              return eval_ARITH_BN254_ADDFP2(ctx, cmd, cr);
            case f_ARITH_BN254_SUBFP2:              return eval_ARITH_BN254_SUBFP2(ctx, cmd, cr);
            case f_ARITH_BN254_MULFP2_X:            return eval_ARITH_BN254_MULFP2_X(ctx, cmd, cr);
            case f_ARITH_BN254_MULFP2_Y:            return eval_ARITH_BN254_MULFP2_Y(ctx, cmd, cr);
            case f_ARITH_BLS12381_MULFP2_X:         return eval_ARITH_BLS12381_MULFP2_X(ctx, cmd, cr);
            case f_ARITH_BLS12381_MULFP2_Y:         return eval_ARITH_BLS12381_MULFP2_Y(ctx, cmd, cr);
            case f_ARITH_BLS12381_ADDFP2:           return eval_ARITH_BLS12381_ADDFP2(ctx, cmd, cr);
            case f_ARITH_BLS12381_SUBFP2:           return eval_ARITH_BLS12381_SUBFP2(ctx, cmd, cr);
            case f_FROM_384_TO_256_H:               return eval_FROM_384_TO_256_H(ctx, cmd, cr);
            case f_FROM_384_TO_256_L:               return eval_FROM_384_TO_256_L(ctx, cmd, cr);
            case f_fp2InvBN254_x:                   return eval_fp2InvBN254_x(ctx, cmd, cr);
            case f_fp2InvBN254_y:                   return eval_fp2InvBN254_y(ctx, cmd, cr);
            case f_fpBN254inv:                      return eval_fpBN254inv(ctx, cmd, cr);

            // Feijoa (fork 10) new methods:
            case f_getForcedTimestamp:              return eval_getForcedTimestamp(ctx, cmd, cr);
            case f_getType:                         return eval_getType(ctx, cmd, cr);
            case f_getForcedGER:                    return eval_getForcedGER(ctx, cmd, cr);
            case f_getL1HistoricRoot:               return eval_getL1HistoricRoot(ctx, cmd, cr);
            case f_getPendingRID:                   return eval_getPendingRID(ctx, cmd, cr);
            case f_getZkGasLimit:                   return eval_getZkGasLimit(ctx, cmd, cr);
            case f_getZ:                            return eval_getZ(ctx, cmd, cr);
            case f_getY:                            return eval_getY(ctx, cmd, cr);
            case f_getBlobL2HashData:               return eval_getBlobL2HashData(ctx, cmd, cr);
            case f_getForcedHashData:               return eval_getForcedHashData(ctx, cmd, cr);
            case f_getBlobLen:                      return eval_getBlobLen(ctx, cmd, cr);
            case f_frBLS12_381inv:                  return eval_frBLS12_381inv(ctx, cmd, cr);
            case f_dump:                            return eval_dump(ctx, cmd, cr);
            case f_check4096Root:                   return eval_check4096Root(ctx, cmd, cr);
            case f_get4096RootIndex:                return eval_get4096RootIndex(ctx, cmd, cr);
            case f_getLastL1InfoTreeRoot:           return eval_getLastL1InfoTreeRoot(ctx, cmd, cr);
            case f_getLastL1InfoTreeIndex:          return eval_getLastL1InfoTreeIndex(ctx, cmd, cr);
            case f_signedComparison:                return eval_signedComparison(ctx, cmd, cr);
            case f_signedComparisonWithConst:       return eval_signedComparisonWithConst(ctx, cmd, cr);
            case f_getFirstDiffChunkRem:            return eval_getFirstDiffChunkRem(ctx, cmd, cr);
            case f_getSmtProofPreviousIndex:        return eval_getSmtProofPreviousIndex(ctx, cmd, cr);
            case f_getBatchHashData:                return eval_getBatchHashData(ctx, cmd, cr);

            case f_getMemValue:                     return eval_getMemValue(ctx, cmd, cr);
            case f_getMemAddr:                      return eval_getMemAddr(ctx, cmd, cr);
            case f_dumpRegs:                        return eval_dumpRegs(ctx, cmd, cr);
            case f_dumphex:                         return eval_dumphex(ctx, cmd, cr);
            case f_break:                           return eval_break(ctx, cmd, cr);
            case f_getVersionedHash:                return eval_getVersionedHash(ctx, cmd, cr);
            case f_getKzgCommitmentHash:            return eval_getKzgCommitmentHash(ctx, cmd, cr);
            case f_getKzgProof:                     return eval_getKzgProof(ctx, cmd, cr);

            case f_fpBLS12_381_sqrt:                return eval_fpBLS12_381_sqrt(ctx, cmd, cr);
            case f_lenBinDecomp:                    return eval_lenBinDecomp(ctx, cmd, cr);
            case f_frBLS12_381_inv:                 return eval_frBLS12_381_inv(ctx, cmd, cr);
            case f_fpBLS12_381_inv:                 return eval_fpBLS12_381_inv(ctx, cmd, cr);
            case f_fp2BLS12_381_inv_x:              return eval_fp2BLS12_381_inv_x(ctx, cmd, cr);
            case f_fp2BLS12_381_inv_y:              return eval_fp2BLS12_381_inv_y(ctx, cmd, cr);
            
            default:
                zklog.error("evalCommand() found invalid function=" + to_string(cmd.function) + "=" + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
                exitProcess();
        }
    }
    switch (cmd.op)
    {
        case op_number:         return eval_number(ctx, cmd, cr);
        case op_declareVar:     return eval_declareVar(ctx, cmd, cr);
        case op_setVar:         return eval_setVar(ctx, cmd, cr);
        case op_getVar:         return eval_getVar(ctx, cmd, cr);
        case op_getReg:         return eval_getReg(ctx, cmd, cr);
        case op_add:            return eval_add(ctx, cmd, cr);
        case op_sub:            return eval_sub(ctx, cmd, cr);
        case op_neg:            return eval_neg(ctx, cmd, cr);
        case op_mul:            return eval_mul(ctx, cmd, cr);
        case op_div:            return eval_div(ctx, cmd, cr);
        case op_mod:            return eval_mod(ctx, cmd, cr);
        case op_or:             return eval_logical_or(ctx, cmd, cr);
        case op_and:            return eval_logical_and(ctx, cmd, cr);
        case op_gt:             return eval_logical_gt(ctx, cmd, cr);
        case op_ge:             return eval_logical_ge(ctx, cmd, cr);
        case op_lt:             return eval_logical_lt(ctx, cmd, cr);
        case op_le:             return eval_logical_le(ctx, cmd, cr);
        case op_eq:             return eval_logical_eq(ctx, cmd, cr);
        case op_ne:             return eval_logical_ne(ctx, cmd, cr);
        case op_not:            return eval_logical_not(ctx, cmd, cr);
        case op_bitand:         return eval_bit_and(ctx, cmd, cr);
        case op_bitor:          return eval_bit_or(ctx, cmd, cr);
        case op_bitxor:         return eval_bit_xor(ctx, cmd, cr);
        case op_bitnot:         return eval_bit_not(ctx, cmd, cr);
        case op_shl:            return eval_bit_shl(ctx, cmd, cr);
        case op_shr:            return eval_bit_shr(ctx, cmd, cr);
        case op_if:             return eval_if(ctx, cmd, cr);
        case op_getMemValue:    return eval_getMemValue(ctx, cmd, cr);
        case op_getMemAddr:     return eval_getMemAddr(ctx, cmd, cr);
        default:
            zklog.error("evalCommand() found invalid operation=" + op2String(cmd.op) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
            exitProcess();
    }
}

void eval_number(Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
    cr.type = crt_scalar;
    cr.scalar = cmd.num;
}

/*************/
/* Variables */
/*************/

/* Declares a new variable, and fails if it already exists */
void eval_declareVar (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check the variable name
    if (cmd.varName == "")
    {
        zklog.error("eval_declareVar() Variable name not found step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }

    // Check that this variable does not exists
    if ( (cmd.varName[0] != '_') && (ctx.vars.find(cmd.varName) != ctx.vars.end()) )
    {
        zklog.error("eval_declareVar() Variable already declared: " + cmd.varName + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Create the new variable with a zero value
    ctx.vars[cmd.varName] = 0;

#ifdef LOG_VARIABLES
    zklog.info("Declare variable: " + cmd.varName);
#endif

    // Return the current value of this variable
    cr.type = crt_scalar;
    cr.scalar = 0;
}

/* Gets the value of the variable, and fails if it does not exist */
void eval_getVar (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check the variable name
    if (cmd.varName == "")
    {
        zklog.error("eval_getVar() Variable name not found step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Check that this variable exists
    std::unordered_map<std::string, mpz_class>::iterator it = ctx.vars.find(cmd.varName);
    if (it == ctx.vars.end())
    {
        zklog.error("eval_getVar() Undefined variable: " + cmd.varName + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }

#ifdef LOG_VARIABLES
    zklog.info("Get variable: " + cmd.varName + " scalar: " + ctx.vars[cmd.varName].get_str(16));
#endif

    // Return the current value of this variable
    cr.type = crt_scalar;
    cr.scalar = it->second;
}

// Forward declaration, used by eval_setVar
void eval_left (Context &ctx, const RomCommand &cmd, CommandResult &cr);

/* Sets variable to value, and fails if it does not exist */
void eval_setVar (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check that tag contains a values array
    if (cmd.values.size() == 0) {
        zklog.error("eval_setVar() could not find array values in setVar command step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get varName from the first element in values
    eval_left(ctx,*cmd.values[0], cr);
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_string)
    {
        zklog.error("eval_setVar() unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    string varName = cr.str;

    // Check that this variable exists
    std::unordered_map<std::string, mpz_class>::iterator it = ctx.vars.find(varName);
    if (it == ctx.vars.end())
    {
        zklog.error("eval_setVar() Undefined variable: " + varName + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }

    // Call evalCommand() to build the field element value for this variable
    evalCommand(ctx, *cmd.values[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }

    // Get the field element value from the command result
    mpz_class auxScalar;
    cr2scalar(ctx, cr, auxScalar);

    // Store the value as the new variable value
    it->second = auxScalar;

    // Return the current value of the variable
    cr.type = crt_scalar;
    cr.scalar = auxScalar;

#ifdef LOG_VARIABLES
    zklog.info("Set variable: " + varName + " scalar: " + ctx.vars[varName].get_str(16));
#endif
}

void eval_left (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
    switch (cmd.op)
    {
        case op_declareVar:
        {
            eval_declareVar(ctx, cmd, cr);
            cr.type = crt_string;
            cr.str = cmd.varName;
            return;
        }
        case op_getVar:
        {
            cr.type = crt_string;
            cr.str = cmd.varName;
            return;
        }
        default:
        {
            zklog.error("eval_left() invalid left expression, op: " + op2String(cmd.op) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
            exitProcess();
        }
    }
}

/*************/
/* Registers */
/*************/

void eval_getReg (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
    // Get registry value, with the proper registry type
    switch (cmd.reg)
    {
        case reg_A:
            cr.type = crt_scalar;
            if (!ctx.multiBaseFea2scalar(ctx.fr, cr.scalar, ctx.pols.A0[*ctx.pStep], ctx.pols.A1[*ctx.pStep], ctx.pols.A2[*ctx.pStep], ctx.pols.A3[*ctx.pStep], ctx.pols.A4[*ctx.pStep], ctx.pols.A5[*ctx.pStep], ctx.pols.A6[*ctx.pStep], ctx.pols.A7[*ctx.pStep]))
            {
                cr.zkResult = ZKR_SM_MAIN_FEA2SCALAR;
                return;
            }
            break;
        case reg_B:
            cr.type = crt_scalar;
            if (!ctx.multiBaseFea2scalar(ctx.fr, cr.scalar, ctx.pols.B0[*ctx.pStep], ctx.pols.B1[*ctx.pStep], ctx.pols.B2[*ctx.pStep], ctx.pols.B3[*ctx.pStep], ctx.pols.B4[*ctx.pStep], ctx.pols.B5[*ctx.pStep], ctx.pols.B6[*ctx.pStep], ctx.pols.B7[*ctx.pStep]))
            {
                cr.zkResult = ZKR_SM_MAIN_FEA2SCALAR;
                return;
            }
            break;
        case reg_C:
            cr.type = crt_scalar;
            if (!ctx.multiBaseFea2scalar(ctx.fr, cr.scalar, ctx.pols.C0[*ctx.pStep], ctx.pols.C1[*ctx.pStep], ctx.pols.C2[*ctx.pStep], ctx.pols.C3[*ctx.pStep], ctx.pols.C4[*ctx.pStep], ctx.pols.C5[*ctx.pStep], ctx.pols.C6[*ctx.pStep], ctx.pols.C7[*ctx.pStep]))
            {
                cr.zkResult = ZKR_SM_MAIN_FEA2SCALAR;
                return;
            }
            break;
        case reg_D:
            cr.type = crt_scalar;
            if (!ctx.multiBaseFea2scalar(ctx.fr, cr.scalar, ctx.pols.D0[*ctx.pStep], ctx.pols.D1[*ctx.pStep], ctx.pols.D2[*ctx.pStep], ctx.pols.D3[*ctx.pStep], ctx.pols.D4[*ctx.pStep], ctx.pols.D5[*ctx.pStep], ctx.pols.D6[*ctx.pStep], ctx.pols.D7[*ctx.pStep]))
            {
                cr.zkResult = ZKR_SM_MAIN_FEA2SCALAR;
                return;
            }
            break;
        case reg_E:
            cr.type = crt_scalar;
            if (!ctx.multiBaseFea2scalar(ctx.fr, cr.scalar, ctx.pols.E0[*ctx.pStep], ctx.pols.E1[*ctx.pStep], ctx.pols.E2[*ctx.pStep], ctx.pols.E3[*ctx.pStep], ctx.pols.E4[*ctx.pStep], ctx.pols.E5[*ctx.pStep], ctx.pols.E6[*ctx.pStep], ctx.pols.E7[*ctx.pStep]))
            {
                cr.zkResult = ZKR_SM_MAIN_FEA2SCALAR;
                return;
            }
            break;
        case reg_SR:
            cr.type = crt_scalar;
            if (!ctx.multiBaseFea2scalar(ctx.fr, cr.scalar, ctx.pols.SR0[*ctx.pStep], ctx.pols.SR1[*ctx.pStep], ctx.pols.SR2[*ctx.pStep], ctx.pols.SR3[*ctx.pStep], ctx.pols.SR4[*ctx.pStep], ctx.pols.SR5[*ctx.pStep], ctx.pols.SR6[*ctx.pStep], ctx.pols.SR7[*ctx.pStep]))
            {
                cr.zkResult = ZKR_SM_MAIN_FEA2SCALAR;
                return;
            }
            break;
        case reg_CTX:
            cr.type = crt_u32;
            cr.u32 = ctx.fr.toU64(ctx.pols.CTX[*ctx.pStep]);
            break;
        case reg_SP:
            cr.type = crt_u16;
            cr.u16 = ctx.fr.toU64(ctx.pols.SP[*ctx.pStep]);
            break;
        case reg_PC:
            cr.type = crt_u32;
            cr.u32 = ctx.fr.toU64(ctx.pols.PC[*ctx.pStep]);
            break;
        case reg_GAS:
            cr.type = crt_u64;
            cr.u64 = ctx.fr.toU64(ctx.pols.GAS[*ctx.pStep]);
            break;
        case reg_zkPC:
            cr.type = crt_u32;
            cr.u32 = ctx.fr.toU64(ctx.pols.zkPC[*ctx.pStep]);
            break;
        case reg_RR:
            cr.type = crt_u64;
            cr.u64 = ctx.fr.toU64(ctx.pols.RR[*ctx.pStep]);
            break;
        case reg_CNT_ARITH:
            cr.type = crt_u64;
            cr.u64 = ctx.fr.toU64(ctx.pols.cntArith[*ctx.pStep]);
            break;
        case reg_CNT_BINARY:
            cr.type = crt_u64;
            cr.u64 = ctx.fr.toU64(ctx.pols.cntBinary[*ctx.pStep]);
            break;
        case reg_CNT_KECCAK_F:
            cr.type = crt_u64;
            cr.u64 = ctx.fr.toU64(ctx.pols.cntKeccakF[*ctx.pStep]);
            break;
        case reg_CNT_MEM_ALIGN:
            cr.type = crt_u64;
            cr.u64 = ctx.fr.toU64(ctx.pols.cntMemAlign[*ctx.pStep]);
            break;
        case reg_CNT_PADDING_PG:
            cr.type = crt_u64;
            cr.u64 = ctx.fr.toU64(ctx.pols.cntPaddingPG[*ctx.pStep]);
            break;
        case reg_CNT_POSEIDON_G:
            cr.type = crt_u64;
            cr.u64 = ctx.fr.toU64(ctx.pols.cntPoseidonG[*ctx.pStep]);
            break;
        case reg_STEP:
            cr.type = crt_u64;
            cr.u64 = *ctx.pStep;
            break;
        case reg_HASHPOS:
            cr.type = crt_u64;
            cr.u64 = ctx.fr.toU64(ctx.pols.HASHPOS[*ctx.pStep]);
            break;
        case reg_RCX:
            cr.type = crt_u64;
            cr.u64 = ctx.fr.toU64(ctx.pols.RCX[*ctx.pStep]);
            break;
        case reg_RID:
            cr.type = crt_u64;
            cr.u64 = ctx.fr.toU64(ctx.pols.RID[*ctx.pStep]);
            break;
        default:
            zklog.error("eval_getReg() Invalid register=" + reg2string(cmd.reg) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
            exitProcess();
    }
}

/*****************************/
/* Command result conversion */
/*****************************/

void cr2fe (Context &ctx, const CommandResult &cr, Goldilocks::Element &fe)
{
    switch (cr.type)
    {
        case crt_fe:
            fe = cr.fe;
            return;
        case crt_scalar:
            scalar2fe(ctx.fr, cr.scalar, fe);
            return;
        default:
            zklog.error("cr2fe() unexpected type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
            exitProcess();
    }
}

void cr2scalar (Context &ctx, const CommandResult &cr, mpz_class &s)
{
    switch (cr.type)
    {
        case crt_scalar:
            s = cr.scalar;
            return;
        case crt_fe:
            fe2scalar(ctx.fr, s, cr.fe);
            return;
        case crt_u64:
            s = cr.u64;
            return;
        case crt_u32:
            s = cr.u32;
            return;
        case crt_u16:
            s = cr.u16;
            return;
        default:
            zklog.error("cr2scalar() unexpected type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
            exitProcess();
    }
}

/*************************/
/* Arithmetic operations */
/*************************/

void eval_add(Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check number of values
    if (cmd.values.size() != 2)
    {
        zklog.error("eval_add() found invalid number of values=" + to_string(cmd.values.size()) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    evalCommand(ctx, *cmd.values[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
    mpz_class a;
    cr2scalar(ctx, cr, a);

    evalCommand(ctx, *cmd.values[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
    mpz_class b;
    cr2scalar(ctx, cr, b);

    cr.type = crt_scalar;
    cr.scalar = a + b;
}

void eval_sub(Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check number of values
    if (cmd.values.size() != 2)
    {
        zklog.error("eval_sub() found invalid number of values=" + to_string(cmd.values.size()) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    evalCommand(ctx, *cmd.values[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
    mpz_class a;
    cr2scalar(ctx, cr, a);

    evalCommand(ctx, *cmd.values[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
    mpz_class b;
    cr2scalar(ctx, cr, b);

    cr.type = crt_scalar;
    cr.scalar = a - b;
}

void eval_neg(Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check number of values
    if (cmd.values.size() != 1)
    {
        zklog.error("eval_neg() found invalid number of values=" + to_string(cmd.values.size()) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    evalCommand(ctx, *cmd.values[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
    mpz_class a;
    cr2scalar(ctx, cr, a);

    cr.type = crt_scalar;
    cr.scalar = -a;
}

void eval_mul(Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check number of values
    if (cmd.values.size() != 2)
    {
        zklog.error("eval_mul() found invalid number of values=" + to_string(cmd.values.size()) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    evalCommand(ctx, *cmd.values[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
    mpz_class a;
    cr2scalar(ctx, cr, a);

    evalCommand(ctx, *cmd.values[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
    mpz_class b;
    cr2scalar(ctx, cr, b);

    cr.type = crt_scalar;
    cr.scalar = a * b;
}

void eval_div(Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check number of values
    if (cmd.values.size() != 2)
    {
        zklog.error("eval_div() found invalid number of values=" + to_string(cmd.values.size()) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    evalCommand(ctx, *cmd.values[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
    mpz_class a;
    cr2scalar(ctx, cr, a);

    evalCommand(ctx, *cmd.values[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
    mpz_class b;
    cr2scalar(ctx, cr, b);

    cr.type = crt_scalar;
    cr.scalar = a / b;
}

void eval_mod(Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check number of values
    if (cmd.values.size() != 2)
    {
        zklog.error("eval_mod() found invalid number of values=" + to_string(cmd.values.size()) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    evalCommand(ctx, *cmd.values[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
    mpz_class a;
    cr2scalar(ctx, cr, a);

    evalCommand(ctx, *cmd.values[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
    mpz_class b;
    cr2scalar(ctx, cr, b);

    cr.type = crt_scalar;
    cr.scalar = a % b;
}

/**********************/
/* Logical operations */
/**********************/

void eval_logical_or (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check number of values
    if (cmd.values.size() != 2)
    {
        zklog.error("eval_logical_or() found invalid number of values=" + to_string(cmd.values.size()) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    evalCommand(ctx, *cmd.values[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
    mpz_class a;
    cr2scalar(ctx, cr, a);

    evalCommand(ctx, *cmd.values[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
    mpz_class b;
    cr2scalar(ctx, cr, b);

    cr.type = crt_scalar;
    cr.scalar = (a || b) ? 1 : 0;
}

void eval_logical_and (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check number of values
    if (cmd.values.size() != 2)
    {
        zklog.error("eval_logical_and() found invalid number of values=" + to_string(cmd.values.size()) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    evalCommand(ctx, *cmd.values[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
    mpz_class a;
    cr2scalar(ctx, cr, a);

    evalCommand(ctx, *cmd.values[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
    mpz_class b;
    cr2scalar(ctx, cr, b);

    cr.type = crt_scalar;
    cr.scalar = (a && b) ? 1 : 0;
}

void eval_logical_gt (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check number of values
    if (cmd.values.size() != 2)
    {
        zklog.error("eval_logical_gt() found invalid number of values=" + to_string(cmd.values.size()) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    evalCommand(ctx, *cmd.values[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
    mpz_class a;
    cr2scalar(ctx, cr, a);

    evalCommand(ctx, *cmd.values[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
    mpz_class b;
    cr2scalar(ctx, cr, b);

    cr.type = crt_scalar;
    cr.scalar = (a > b) ? 1 : 0;
}

void eval_logical_ge (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check number of values
    if (cmd.values.size() != 2)
    {
        zklog.error("eval_logical_ge() found invalid number of values=" + to_string(cmd.values.size()) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    evalCommand(ctx, *cmd.values[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
    mpz_class a;
    cr2scalar(ctx, cr, a);

    evalCommand(ctx, *cmd.values[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
    mpz_class b;
    cr2scalar(ctx, cr, b);

    cr.type = crt_scalar;
    cr.scalar = (a >= b) ? 1 : 0;
}

void eval_logical_lt (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check number of values
    if (cmd.values.size() != 2)
    {
        zklog.error("eval_logical_lt() found invalid number of values=" + to_string(cmd.values.size()) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    evalCommand(ctx, *cmd.values[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
    mpz_class a;
    cr2scalar(ctx, cr, a);

    evalCommand(ctx, *cmd.values[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
    mpz_class b;
    cr2scalar(ctx, cr, b);

    cr.type = crt_scalar;
    cr.scalar = (a < b) ? 1 : 0;
}

void eval_logical_le (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check number of values
    if (cmd.values.size() != 2)
    {
        zklog.error("eval_logical_le() found invalid number of values=" + to_string(cmd.values.size()) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    evalCommand(ctx, *cmd.values[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
    mpz_class a;
    cr2scalar(ctx, cr, a);

    evalCommand(ctx, *cmd.values[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
    mpz_class b;
    cr2scalar(ctx, cr, b);

    cr.type = crt_scalar;
    cr.scalar = (a <= b) ? 1 : 0;
}

void eval_logical_eq (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check number of values
    if (cmd.values.size() != 2)
    {
        zklog.error("eval_logical_eq() found invalid number of values=" + to_string(cmd.values.size()) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    evalCommand(ctx, *cmd.values[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
    mpz_class a;
    cr2scalar(ctx, cr, a);

    evalCommand(ctx, *cmd.values[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
    mpz_class b;
    cr2scalar(ctx, cr, b);

    cr.type = crt_scalar;
    cr.scalar = (a == b) ? 1 : 0;
}

void eval_logical_ne (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check number of values
    if (cmd.values.size() != 2)
    {
        zklog.error("eval_logical_ne() found invalid number of values=" + to_string(cmd.values.size()) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    evalCommand(ctx, *cmd.values[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
    mpz_class a;
    cr2scalar(ctx, cr, a);

    evalCommand(ctx, *cmd.values[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
    mpz_class b;
    cr2scalar(ctx, cr, b);

    cr.type = crt_scalar;
    cr.scalar = (a != b) ? 1 : 0;
}

void eval_logical_not (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check number of values
    if (cmd.values.size() != 1)
    {
        zklog.error("eval_logical_not() found invalid number of values=" + to_string(cmd.values.size()) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    evalCommand(ctx, *cmd.values[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
    mpz_class a;
    cr2scalar(ctx, cr, a);

    cr.type = crt_scalar;
    cr.scalar = (a) ? 0 : 1;
}

/*********************/
/* Binary operations */
/*********************/

void eval_bit_and (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check number of values
    if (cmd.values.size() != 2)
    {
        zklog.error("eval_bit_and() found invalid number of values=" + to_string(cmd.values.size()) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    evalCommand(ctx, *cmd.values[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
    mpz_class a;
    cr2scalar(ctx, cr, a);

    evalCommand(ctx, *cmd.values[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
    mpz_class b;
    cr2scalar(ctx, cr, b);

    cr.type = crt_scalar;
    cr.scalar = a & b;
}

void eval_bit_or (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check number of values
    if (cmd.values.size() != 2)
    {
        zklog.error("eval_bit_or() found invalid number of values=" + to_string(cmd.values.size()) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    evalCommand(ctx, *cmd.values[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
    mpz_class a;
    cr2scalar(ctx, cr, a);

    evalCommand(ctx, *cmd.values[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
    mpz_class b;
    cr2scalar(ctx, cr, b);

    cr.type = crt_scalar;
    cr.scalar = a | b;
}

void eval_bit_xor (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check number of values
    if (cmd.values.size() != 2)
    {
        zklog.error("eval_bit_xor() found invalid number of values=" + to_string(cmd.values.size()) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    evalCommand(ctx, *cmd.values[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
    mpz_class a;
    cr2scalar(ctx, cr, a);

    evalCommand(ctx, *cmd.values[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
    mpz_class b;
    cr2scalar(ctx, cr, b);

    cr.type = crt_scalar;
    cr.scalar = a ^ b;
}

void eval_bit_not (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check number of values
    if (cmd.values.size() != 1)
    {
        zklog.error("eval_bit_not() found invalid number of values=" + to_string(cmd.values.size()) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    evalCommand(ctx, *cmd.values[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
    mpz_class a;
    cr2scalar(ctx, cr, a);

    cr.type = crt_scalar;
    cr.scalar = ~a;
}

void eval_bit_shl (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check number of values
    if (cmd.values.size() != 2)
    {
        zklog.error("eval_bit_shl() found invalid number of values=" + to_string(cmd.values.size()) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    evalCommand(ctx, *cmd.values[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
    mpz_class a;
    cr2scalar(ctx, cr, a);

    evalCommand(ctx, *cmd.values[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
    mpz_class b;
    cr2scalar(ctx, cr, b);

    cr.type = crt_scalar;
    cr.scalar = (a << b.get_ui());
}

void eval_bit_shr (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check number of values
    if (cmd.values.size() != 2)
    {
        zklog.error("eval_bit_shr() found invalid number of values=" + to_string(cmd.values.size()) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    evalCommand(ctx, *cmd.values[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
    mpz_class a;
    cr2scalar(ctx, cr, a);

    evalCommand(ctx, *cmd.values[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
    mpz_class b;
    cr2scalar(ctx, cr, b);

    cr.type = crt_scalar;
    cr.scalar = (a >> b.get_ui());
}

/*****************/
/* If: a ? b : c */
/*****************/

void eval_if (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check number of values
    if (cmd.values.size() != 3)
    {
        zklog.error("eval_if() found invalid number of values=" + to_string(cmd.values.size()) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    evalCommand(ctx, *cmd.values[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
    mpz_class a;
    cr2scalar(ctx, cr, a);

    if (a)
    {
        evalCommand(ctx, *cmd.values[1], cr);
        if (cr.zkResult != ZKR_SUCCESS)
        {
            return;
        }
        mpz_class b;
        cr2scalar(ctx, cr, b);

        cr.type = crt_scalar;
        cr.scalar = b;
    }
    else
    {
        evalCommand(ctx, *cmd.values[2], cr);
        if (cr.zkResult != ZKR_SUCCESS)
        {
            return;
        }
        mpz_class c;
        cr2scalar(ctx, cr, c);

        cr.type = crt_scalar;
        cr.scalar = c;
    }
}

/***************/
/* Memory read */
/***************/

void eval_getMemAddr (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 1)
    {
        zklog.error("eval_getMemAddr() invalid number of parameters function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get m0 by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_getMemAddr() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    uint64_t addr = cr.scalar.get_ui();
    if (cmd.useCTX == 1)
    {
        addr += ctx.fr.toU64(ctx.pols.CTX[*ctx.pStep]) * 0x40000;
    }
    cr.type = crt_scalar;
    cr.scalar = addr;
}

void eval_getMemValue (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 1)
    {
        zklog.error("eval_getMemValue() invalid number of parameters function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get m0 by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_getMemValue() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    uint64_t addr = cr.scalar.get_ui();
    cr.type = crt_scalar;
    if (cmd.useCTX == 1)
    {
        addr += ctx.fr.toU64(ctx.pols.CTX[*ctx.pStep]) * 0x40000;
    }
    Fea fea = ctx.mem[addr];
    if (!fea2scalar(ctx.fr, cr.scalar, fea.fe0, fea.fe1, fea.fe2, fea.fe3, fea.fe4, fea.fe5, fea.fe6, fea.fe7))
    {
        cr.zkResult = ZKR_SM_MAIN_FEA2SCALAR;
        return;
    }
}

/**************/
/* Input data */
/**************/

void eval_getGlobalExitRoot(Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 0)
    {
        zklog.error("eval_getGlobalExitRoot() invalid number of parameters function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Return ctx.proverRequest.input.publicInputs.globalExitRoot as a field element array
    cr.type = crt_fea;
    scalar2fea(ctx.fr, ctx.proverRequest.input.publicInputsExtended.publicInputs.globalExitRoot, cr.fea0, cr.fea1, cr.fea2, cr.fea3, cr.fea4, cr.fea5, cr.fea6, cr.fea7);
}

void eval_getSequencerAddr(Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 0)
    {
        zklog.error("eval_getSequencerAddr() invalid number of parameters function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Return ctx.proverRequest.input.publicInputs.sequencerAddr as a field element array
    cr.type = crt_fea;
    scalar2fea(ctx.fr, ctx.proverRequest.input.publicInputsExtended.publicInputs.sequencerAddr, cr.fea0, cr.fea1, cr.fea2, cr.fea3, cr.fea4, cr.fea5, cr.fea6, cr.fea7);
}

void eval_getTxsLen(Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 0)
    {
        zklog.error("eval_getTxsLen() invalid number of parameters function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Return ctx.proverRequest.input.txsLen/2 as a field element array
    cr.type = crt_fea;
    cr.fea0 = ctx.fr.fromU64(ctx.proverRequest.input.publicInputsExtended.publicInputs.batchL2Data.size());
    cr.fea1 = ctx.fr.zero();
    cr.fea2 = ctx.fr.zero();
    cr.fea3 = ctx.fr.zero();
    cr.fea4 = ctx.fr.zero();
    cr.fea5 = ctx.fr.zero();
    cr.fea6 = ctx.fr.zero();
    cr.fea7 = ctx.fr.zero();
}

void eval_getTxs(Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 2)
    {
        zklog.error("eval_getTxs() invalid number of parameters function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get offset by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar) {
        zklog.error("eval_getTxs() 1 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    uint64_t offset = cr.scalar.get_ui();

    // Get offset by executing cmd.params[1]
    evalCommand(ctx, *cmd.params[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_getTxs() 2 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    uint64_t len = cr.scalar.get_ui();

    // Return result as a field element array
    cr.type = crt_fea;
    ba2fea(ctx.fr, (uint8_t *)(ctx.proverRequest.input.publicInputsExtended.publicInputs.batchL2Data.c_str()) + offset, len, cr.fea0, cr.fea1, cr.fea2, cr.fea3, cr.fea4, cr.fea5, cr.fea6, cr.fea7);
}

/*********************/
/* Full tracer event */
/*********************/

void eval_eventLog(Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() < 1)
    {
        zklog.error("eval_eventLog() invalid number of parameters function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    zkassert(ctx.proverRequest.input.publicInputsExtended.publicInputs.forkID == 10); // fork_10
    cr.zkResult = ((fork_10::FullTracer *)ctx.proverRequest.pFullTracer)->handleEvent(ctx, cmd);

    // Return an empty array of field elements
    cr.type = crt_fea;
    cr.fea0 = ctx.fr.zero();
    cr.fea1 = ctx.fr.zero();
    cr.fea2 = ctx.fr.zero();
    cr.fea3 = ctx.fr.zero();
    cr.fea4 = ctx.fr.zero();
    cr.fea5 = ctx.fr.zero();
    cr.fea6 = ctx.fr.zero();
    cr.fea7 = ctx.fr.zero();
}

void eval_getTimestamp(Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 0)
    {
        zklog.error("eval_getTimestamp() invalid number of parameters function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Return ctx.proverRequest.input.publicInputs.timestamp as a field element array
    cr.type = crt_fea;
    cr.fea0 = ctx.fr.fromU64(ctx.proverRequest.input.publicInputsExtended.publicInputs.timestamp);
    cr.fea1 = ctx.fr.zero();
    cr.fea2 = ctx.fr.zero();
    cr.fea3 = ctx.fr.zero();
    cr.fea4 = ctx.fr.zero();
    cr.fea5 = ctx.fr.zero();
    cr.fea6 = ctx.fr.zero();
    cr.fea7 = ctx.fr.zero();
}

/*************/
/* Condition */
/*************/

void eval_cond (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 1)
    {
        zklog.error("eval_cond() invalid number of parameters function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get offset by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_cond() unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    cr.type = crt_fea;
    if (cr.scalar != 0)
    {
        cr.fea0 = ctx.fr.negone(); // -1
    }
    else
    {
        cr.fea0 = ctx.fr.zero();
    }
    cr.fea1 = ctx.fr.zero();
    cr.fea2 = ctx.fr.zero();
    cr.fea3 = ctx.fr.zero();
    cr.fea4 = ctx.fr.zero();
    cr.fea5 = ctx.fr.zero();
    cr.fea6 = ctx.fr.zero();
    cr.fea7 = ctx.fr.zero();
}

/***************/
/* Expotential */
/***************/

void eval_exp (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 2)
    {
        zklog.error("eval_exp() invalid number of parameters function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get a by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_exp() 1 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    mpz_class a = cr.scalar;

    // Get b by executing cmd.params[1]
    evalCommand(ctx, *cmd.params[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_exp() 2 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    mpz_class b = cr.scalar;

    // auxScalar = a^b
    mpz_class auxScalar;
    mpz_pow_ui(auxScalar.get_mpz_t(), a.get_mpz_t(), b.get_ui());

    // Return as a field element array
    cr.type = crt_fea;
    scalar2fea(ctx.fr, auxScalar, cr.fea0, cr.fea1, cr.fea2, cr.fea3, cr.fea4, cr.fea5, cr.fea6, cr.fea7);
}

/*********************/
/* Binary operations */
/*********************/

void eval_bitwise_and (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 2)
    {
        zklog.error("eval_bitwise_and() invalid number of parameters function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get a by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_bitwise_and() 1 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    mpz_class a = cr.scalar;

    // Get b by executing cmd.params[1]
    evalCommand(ctx, *cmd.params[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_bitwise_and() 2 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    mpz_class b = cr.scalar;

    cr.type = crt_scalar;
    cr.scalar = a & b;
}

void eval_bitwise_or (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 2)
    {
        zklog.error("eval_bitwise_or() invalid number of parameters function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get a by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_bitwise_or() 1 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    mpz_class a = cr.scalar;

    // Get b by executing cmd.params[1]
    evalCommand(ctx, *cmd.params[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_bitwise_or() 2 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    mpz_class b = cr.scalar;

    cr.type = crt_scalar;
    cr.scalar = a | b;
}

void eval_bitwise_xor (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 2)
    {
        zklog.error("eval_bitwise_xor() invalid number of parameters function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get a by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_bitwise_xor() 1 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    mpz_class a = cr.scalar;

    // Get b by executing cmd.params[1]
    evalCommand(ctx, *cmd.params[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_bitwise_xor() 2 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    mpz_class b = cr.scalar;

    cr.type = crt_scalar;
    cr.scalar = a ^ b;
}

void eval_bitwise_not (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 1)
    {
        zklog.error("eval_bitwise_not() invalid number of parameters function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get a by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_bitwise_not() unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    mpz_class a = cr.scalar;

    cr.type = crt_scalar;
    cr.scalar = a ^ ScalarMask256;
}

/**************************/
/* Before last evaluation */
/**************************/

void eval_beforeLast (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 0)
    {
        zklog.error("eval_beforeLast() invalid number of parameters function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Return a field element array
    cr.type = crt_fea;
    if (*ctx.pStep >= ctx.N-2)
    {
        cr.fea0 = ctx.fr.zero();
    }
    else
    {
        cr.fea0 = ctx.fr.negone();
    }
    cr.fea1 = ctx.fr.zero();
    cr.fea2 = ctx.fr.zero();
    cr.fea3 = ctx.fr.zero();
    cr.fea4 = ctx.fr.zero();
    cr.fea5 = ctx.fr.zero();
    cr.fea6 = ctx.fr.zero();
    cr.fea7 = ctx.fr.zero();
}

/*************************/
/* Comparison operations */
/*************************/

void eval_comp_lt (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 2)
    {
        zklog.error("eval_comp_lt() invalid number of parameters function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get a by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_comp_lt() 1 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    mpz_class a = cr.scalar;

    // Get b by executing cmd.params[1]
    evalCommand(ctx, *cmd.params[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_comp_lt() 2 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    mpz_class b = cr.scalar;

    cr.type = crt_scalar;
    cr.scalar = (a < b) ? 1 : 0;
}

void eval_comp_gt (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 2)
    {
        zklog.error("eval_comp_gt() invalid number of parameters function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get a by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_comp_gt() 1 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    mpz_class a = cr.scalar;

    // Get b by executing cmd.params[1]
    evalCommand(ctx, *cmd.params[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_comp_gt() 2 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    mpz_class b = cr.scalar;

    cr.type = crt_scalar;
    cr.scalar = (a > b) ? 1 : 0;
}

void eval_comp_eq (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 2)
    {
        zklog.error("eval_comp_eq() invalid number of parameters function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get a by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_comp_eq() 1 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    mpz_class a = cr.scalar;

    // Get b by executing cmd.params[1]
    evalCommand(ctx, *cmd.params[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_comp_eq() 2 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    mpz_class b = cr.scalar;

    cr.type = crt_scalar;
    cr.scalar = (a == b) ? 1 : 0;
}

void eval_loadScalar (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 1)
    {
        zklog.error("eval_loadScalar() invalid number of parameters function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
}

/*************/
/* Store log */
/*************/

void eval_storeLog (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
    zkassert(ctx.proverRequest.input.publicInputsExtended.publicInputs.forkID == 10); // fork_10
    cr.zkResult = ((fork_10::FullTracer *)ctx.proverRequest.pFullTracer)->handleEvent(ctx, cmd);

    // Return an empty array of field elements
    cr.type = crt_fea;
    cr.fea0 = ctx.fr.zero();
    cr.fea1 = ctx.fr.zero();
    cr.fea2 = ctx.fr.zero();
    cr.fea3 = ctx.fr.zero();
    cr.fea4 = ctx.fr.zero();
    cr.fea5 = ctx.fr.zero();
    cr.fea6 = ctx.fr.zero();
    cr.fea7 = ctx.fr.zero();
}

/*******/
/* Log */
/*******/

void eval_log (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 1)
    {
        zklog.error("eval_log() invalid number of parameters function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get indexLog by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }

    mpz_class scalarLog;
    switch (cr.type)
    {
        case crt_fea:
            if (!fea2scalar(ctx.fr, scalarLog, cr.fea0, cr.fea1, cr.fea2, cr.fea3, cr.fea4, cr.fea5, cr.fea6, cr.fea7))
            {
                cr.zkResult = ZKR_SM_MAIN_FEA2SCALAR;
                return;
            }
            break;
        case crt_u64:
            scalarLog = cr.u64;
            break;
        default:
            zklog.error("eval_storeLog() 1 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
            exitProcess();
    }

    // Print the log
    string hexLog = Add0xIfMissing(scalarLog.get_str(16));
    zklog.info("Log regname=" + reg2string(cmd.params[0]->reg) + " hexLog=" + hexLog);

    // Return an empty array of field elements
    cr.type = crt_fea;
    cr.fea0 = ctx.fr.zero();
    cr.fea1 = ctx.fr.zero();
    cr.fea2 = ctx.fr.zero();
    cr.fea3 = ctx.fr.zero();
    cr.fea4 = ctx.fr.zero();
    cr.fea5 = ctx.fr.zero();
    cr.fea6 = ctx.fr.zero();
    cr.fea7 = ctx.fr.zero();
}

/***************************/
/* Memory align operations */
/***************************/

void eval_memAlignWR_W0 (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 3)
    {
        zklog.error("eval_memAlignWR_W0() invalid number of parameters function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get m0 by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_memAlignWR_W0() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    mpz_class m0 = cr.scalar;

    // Get value by executing cmd.params[1]
    evalCommand(ctx, *cmd.params[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
    mpz_class value;
    if (cr.type == crt_u64)
    {
        value = cr.u64;
    }
    else if (cr.type == crt_scalar)
    {
        value = cr.scalar;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    else
    {
        zklog.error("eval_memAlignWR_W0() 1 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get offset by executing cmd.params[2]
    evalCommand(ctx, *cmd.params[2], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_memAlignWR_W0() 2 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    uint64_t mode = cr.scalar.get_ui();

    uint64_t offset = mode & 0x7F;
    uint64_t len = (mode >> 7) & 0x3F;
    bool leftAlignment = mode & 0x2000;
    bool littleEndian = mode & 0x4000;

    if (offset>64 || len > 32 || mode > 0x7FFFF)
    {
        zklog.error("eval_memAlignWR_W0() invalid mode : " + to_string(mode) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
    uint64_t _len = len == 0 ? 32 : len;
    if ((_len + offset) > 64) 
    {
        _len = 64 - offset;
    }
    mpz_class maskV = ScalarMask256 >> (8 * (32 - _len));
    uint64_t shiftBits = (64 - offset - _len) * 8;

    if (leftAlignment && _len < 32) 
    {
        value = value >> (8* (32 - len));
    }
    value = value & maskV;
    if (littleEndian) 
    {
        // reverse bytes
        mpz_class _tmpv = 0;
        for (uint64_t ilen = 0; ilen < _len; ++ilen) 
        {
            _tmpv = (_tmpv << 8) | (value & 0xFF);
            value = value >> 8;
        }
        value = _tmpv;
    }
    mpz_class w0 = ((m0 << 256 & (ScalarMask512 ^ (maskV << shiftBits))) | (value << shiftBits)) >> 256;
    cr.type = crt_fea;
    scalar2fea(ctx.fr, w0, cr.fea0, cr.fea1, cr.fea2, cr.fea3, cr.fea4, cr.fea5, cr.fea6, cr.fea7);
}

void eval_memAlignWR_W1 (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 3)
    {
        zklog.error("eval_memAlignWR_W1() invalid number of parameters function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get m1 by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_memAlignWR_W1() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    mpz_class m1 = cr.scalar;

    // Get value by executing cmd.params[1]
    evalCommand(ctx, *cmd.params[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_memAlignWR_W1() 1 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    mpz_class value = cr.scalar;

    // Get offset by executing cmd.params[2]
    evalCommand(ctx, *cmd.params[2], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_memAlignWR_W1() 2 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    uint64_t mode = cr.scalar.get_ui();

    uint64_t offset = mode & 0x7F;
    uint64_t len = (mode >> 7) & 0x3F;
    bool leftAlignment = mode & 0x2000;
    bool littleEndian = mode & 0x4000;

    if (offset>64 || len > 32 || mode > 0x7FFFF)
    {
        zklog.error("eval_memAlignWR_W1() invalid mode : " + to_string(mode) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
    uint64_t _len = len == 0 ? 32 : len;
    if ((_len + offset) > 64) 
    {
        _len = 64 - offset;
    }
    mpz_class maskV = ScalarMask256 >> (8 * (32 - _len));
    uint64_t shiftBits = (64 - offset - _len) * 8;

    if (leftAlignment && _len < 32) 
    {
        value = value >> (8* (32 - len));
    }
    value = value & maskV;
    if (littleEndian) 
    {
        // reverse bytes
        mpz_class _tmpv = 0;
        for (uint64_t ilen = 0; ilen < _len; ++ilen) 
        {
            _tmpv = (_tmpv << 8) | (value & 0xFF);
            value = value >> 8;
        }
        value = _tmpv;
    }
    mpz_class w1 = ((m1 & (ScalarMask512 ^ (maskV << shiftBits))) | (value << shiftBits)) &  ScalarMask256;
    cr.type = crt_fea;
    scalar2fea(ctx.fr, w1, cr.fea0, cr.fea1, cr.fea2, cr.fea3, cr.fea4, cr.fea5, cr.fea6, cr.fea7);

}

void eval_memAlignRD (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 3)
    {
        zklog.error("eval_memAlignRD() invalid number of parameters function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get m0 by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_memAlignRD() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    mpz_class m0 = cr.scalar;

    // Get value by executing cmd.params[1]
    evalCommand(ctx, *cmd.params[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_memAlignRD() 1 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    mpz_class m1 = cr.scalar;

    // Get offset by executing cmd.params[2]
    evalCommand(ctx, *cmd.params[2], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_memAlignRD() 2 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    uint64_t mode = cr.scalar.get_ui();
    uint64_t offset = mode & 0x7F;
    uint64_t len = (mode >> 7) & 0x3F;
    bool leftAlignment = mode & 0x2000;
    bool littleEndian = mode & 0x4000;

    if (offset>64 || len > 32 || mode > 0x7FFFF)
    {
        zklog.error("eval_memAlignRD() 2 invalid mode: " + to_string(mode) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
    uint64_t _len = len == 0 ? 32 : len;
    if ((_len + offset) > 64) 
    {
        _len = 64 - offset;
    }
    mpz_class m = (m0 << 256) | m1;
    mpz_class maskV = ScalarMask256 >> (8 * (32 - _len));
    uint64_t shiftBits = (64 - offset - _len) * 8;
    if (shiftBits > 0) 
    {
        m = m >> shiftBits;
    }
    mpz_class _v = m & maskV;
    if (littleEndian) 
    {
        // reverse bytes
        mpz_class _tmpv = 0;
        for (uint64_t ilen = 0; ilen < _len; ++ilen) 
        {
            _tmpv = (_tmpv << 8) | (_v & 0xFF);
            _v = _v >> 8;
        }
        _v = _tmpv;
    }
    if (leftAlignment && _len < 32) 
    {
        _v = _v << ((32 - len) * 8);
    }
    cr.type = crt_fea;
    scalar2fea(ctx.fr, _v, cr.fea0, cr.fea1, cr.fea2, cr.fea3, cr.fea4, cr.fea5, cr.fea6, cr.fea7);
}

/*************************/
/* Inverse field element */
/*************************/

void eval_inverseFpEc (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 1)
    {
        zklog.error("eval_inverseFpEc() invalid number of parameters function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get a by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_inverseFpEc() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    RawFec::Element a;
    ctx.fec.fromString(a, cr.scalar.get_str(16), 16);
    if (ctx.fec.isZero(a))
    {
        zklog.error("eval_inverseFpEc() Division by zero step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }

    RawFec::Element r;
    ctx.fec.inv(r, a);

    cr.type = crt_scalar;
    ctx.fec.toMpz(cr.scalar.get_mpz_t(), r);
}

void eval_inverseFnEc (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 1)
    {
        zklog.error("eval_inverseFnEc() invalid number of parameters function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get a by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_inverseFnEc() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    RawFnec::Element a;
    ctx.fnec.fromMpz(a, cr.scalar.get_mpz_t());
    if (ctx.fnec.isZero(a))
    {
        zklog.error("eval_inverseFnEc() Division by zero step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }

    RawFnec::Element r;
    ctx.fnec.inv(r, a);

    cr.type = crt_scalar;
    ctx.fnec.toMpz(cr.scalar.get_mpz_t(), r);
}

/*****************************/
/* Square root field element */
/*****************************/

mpz_class pow ( const mpz_class &x, const mpz_class &n, const mpz_class &p )
{
    if (n == 0)
    {
        return 1;
    }
    if ((n & 1) == 1)
    {
        return (pow(x, n-1, p) * x) % p;
    }
    mpz_class x2 = pow(x, n/2, p);
    return (x2 * x2) % p;
}

mpz_class sqrtTonelliShanks ( const mpz_class &n, const mpz_class &p )
{
    mpz_class s = 1;
    mpz_class q = p - 1;
    while ((q & 1) == 0)
    {
        q = q / 2;
        ++s;
    }
    if (s == 1)
    {
        mpz_class r = pow(n, (p+1)/4, p);
        if ((r * r) % p == n)
        {
            return r;
        }
        return ScalarMask256;
    }

    mpz_class z = 1;
    while (pow(++z, (p - 1)/2, p) != (p - 1));
//    std::cout << "Z found: " << z << "\n";
    mpz_class c = pow(z, q, p);
    mpz_class r = pow(n, (q+1)/2, p);
    mpz_class t = pow(n, q, p);
    mpz_class m = s;
    while (t != 1)
    {
        mpz_class tt = t;
        mpz_class i = 0;
        while (tt != 1)
        {
            tt = (tt * tt) % p;
            ++i;
            if (i == m)
            {
                return ScalarMask256;
            }
        }
        mpz_class b = pow(c, pow(2, m-i-1, p-1), p);
        mpz_class b2 = (b * b) % p;
        r = (r * b) % p;
        t = (t * b2) % p;
        c = b2;
        m = i;
    }
    if (((r * r) % p) == n)
    {
        r = r % p;
        if (r > (p/2))
        {
            r = p - r; // return only the possitive solution of the square root
        }
        return r;
    }
    return ScalarMask256;
}

void eval_sqrtFpEc (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 1)
    {
        zklog.error("eval_sqrtFpEc() invalid number of parameters function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get a by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_sqrtFpEc() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    
    mpz_class a = cr.scalar;
    cr.type = crt_scalar;
    sqrtF3mod4(cr.scalar, cr.scalar);
}

void eval_sqrtFpEcParity (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 2)
    {
        zklog.error("eval_sqrtFpEcParity() invalid number of parameters function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get a by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_sqrtFpEc() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif    
    mpz_class a = cr.scalar;

    // Get parity by executing cmd.params[1]
    evalCommand(ctx, *cmd.params[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_sqrtFpEc() 1 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif    
    mpz_class parity = cr.scalar;

    // Call the sqrt function
    cr.type = crt_scalar;
    sqrtF3mod4(cr.scalar, a);

    // Post-process the result
    if (cr.scalar == ScalarMask256)
    {
        // This sqrt does not have a solution
    }
    else if ((cr.scalar & 1) == parity)
    {
        // Return r as it is, since it has the requested parity
    }
    else
    {
        // Negate the result
        RawFec::Element fe;
        fec.fromMpz(fe, cr.scalar.get_mpz_t());
        fe = fec.neg(fe);
        fec.toMpz(cr.scalar.get_mpz_t(), fe);
    }
}

/********************/
/* Point operations */
/********************/

void eval_AddPointEc (Context &ctx, const RomCommand &cmd, bool dbl, RawFec::Element &x3, RawFec::Element &y3);

void eval_xAddPointEc (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
    RawFec::Element x3;
    if(ctx.ecRecoverPrecalcBuffer.filled == true){
        x3 = ctx.ecRecoverPrecalcBuffer.buffer[ctx.ecRecoverPrecalcBuffer.pos++];
    }else{
        RawFec::Element y3;
        eval_AddPointEc(ctx, cmd, false, x3, y3);    
    }
    cr.type = crt_scalar;
    ctx.fec.toMpz(cr.scalar.get_mpz_t(), x3);
}

void eval_yAddPointEc (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
    RawFec::Element y3;
    if(ctx.ecRecoverPrecalcBuffer.filled == true){
        y3 = ctx.ecRecoverPrecalcBuffer.buffer[ctx.ecRecoverPrecalcBuffer.pos++];
    }else{
        RawFec::Element x3;
        eval_AddPointEc(ctx, cmd, false, x3, y3);  
    }
    cr.type = crt_scalar;
    ctx.fec.toMpz(cr.scalar.get_mpz_t(), y3);
}

void eval_xDblPointEc (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
    RawFec::Element x3;
    if(ctx.ecRecoverPrecalcBuffer.filled == true){
        x3 = ctx.ecRecoverPrecalcBuffer.buffer[ctx.ecRecoverPrecalcBuffer.pos++];
    }else{
        RawFec::Element y3;
        eval_AddPointEc(ctx, cmd, true, x3, y3);    
    }
    cr.type = crt_scalar;
    ctx.fec.toMpz(cr.scalar.get_mpz_t(), x3);
}

void eval_yDblPointEc (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
    RawFec::Element y3;
    if(ctx.ecRecoverPrecalcBuffer.filled == true){
        y3 = ctx.ecRecoverPrecalcBuffer.buffer[ctx.ecRecoverPrecalcBuffer.pos++];
    }else{
        RawFec::Element x3;
        eval_AddPointEc(ctx, cmd, true, x3, y3);    
    }
    cr.type = crt_scalar;
    ctx.fec.toMpz(cr.scalar.get_mpz_t(), y3);
}

void eval_AddPointEc (Context &ctx, const RomCommand &cmd, bool dbl, RawFec::Element &x3, RawFec::Element &y3)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != (dbl ? 2 : 4))
    {
        zklog.error("eval_AddPointEc() invalid number of parameters function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    CommandResult cr;

    // Get x1 by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_AddPointEc() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    RawFec::Element x1;
    ctx.fec.fromMpz(x1, cr.scalar.get_mpz_t());

    // Get y1 by executing cmd.params[1]
    evalCommand(ctx, *cmd.params[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_AddPointEc() 1 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    RawFec::Element y1;
    ctx.fec.fromMpz(y1, cr.scalar.get_mpz_t());

    RawFec::Element x2, y2;
    if (dbl)
    {
        x2 = x1;
        y2 = y1;
    }
    else
    {
        // Get x2 by executing cmd.params[2]
        evalCommand(ctx, *cmd.params[2], cr);
        if (cr.zkResult != ZKR_SUCCESS)
        {
            return;
        }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
        if (cr.type != crt_scalar)
        {
            zklog.error("eval_AddPointEc() 2 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
            exitProcess();
        }
#endif
        ctx.fec.fromMpz(x2, cr.scalar.get_mpz_t());

        // Get y2 by executing cmd.params[3]
        evalCommand(ctx, *cmd.params[3], cr);
        if (cr.zkResult != ZKR_SUCCESS)
        {
            return;
        }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
        if (cr.type != crt_scalar)
        {
            zklog.error("eval_AddPointEc() 3 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
            exitProcess();
        }
#endif
        ctx.fec.fromMpz(y2, cr.scalar.get_mpz_t());
    }

    cr.zkResult = AddPointEc(ctx, dbl, x1, y1, x2, y2, x3, y3);
}

zkresult AddPointEc (Context &ctx, bool dbl, const RawFec::Element &x1, const RawFec::Element &y1, const RawFec::Element &x2, const RawFec::Element &y2, RawFec::Element &x3, RawFec::Element &y3)
{
    
    // Check if results are buffered
    if(ctx.ecRecoverPrecalcBuffer.filled == true){
        if(ctx.ecRecoverPrecalcBuffer.pos < 2){
            zklog.error("ecRecoverPrecalcBuffer.buffer buffer is not filled, but pos < 2 (pos=" + to_string(ctx.ecRecoverPrecalcBuffer.pos) + ")");
            exitProcess();
        }
        x3 = ctx.ecRecoverPrecalcBuffer.buffer[ctx.ecRecoverPrecalcBuffer.pos-2];
        y3 = ctx.ecRecoverPrecalcBuffer.buffer[ctx.ecRecoverPrecalcBuffer.pos-1];
        return ZKR_SUCCESS;
    }

    // Check if we have just computed this operation
    if ( (ctx.lastECAdd.bDouble == dbl) &&
         ctx.fec.eq(ctx.lastECAdd.x1, x1) &&
         ctx.fec.eq(ctx.lastECAdd.y1, y1) &&
         ( dbl || (ctx.fec.eq(ctx.lastECAdd.x2, x2) && ctx.fec.eq(ctx.lastECAdd.y2, y2) ) ) )
    {
        //zklog.info("eval_AddPointEc() reading from cache");
        x3 = ctx.lastECAdd.x3;
        y3 = ctx.lastECAdd.y3;
        return ZKR_SUCCESS;
    }

    RawFec::Element aux1, aux2, s;

    if (dbl)
    {
        // s = 3*x1*x1/2*y1
        ctx.fec.mul(aux1, x1, x1);
        ctx.fec.fromUI(aux2, 3);
        ctx.fec.mul(aux1, aux1, aux2);
        ctx.fec.add(aux2, y1, y1);
        if (ctx.fec.isZero(aux2))
        {
            zklog.error("AddPointEc() got denominator=0 1");
            return ZKR_SM_MAIN_ARITH_ECRECOVER_DIVIDE_BY_ZERO;
        }
        ctx.fec.div(s, aux1, aux2);
    }
    else
    {
        // s = (y2-y1)/(x2-x1)
        ctx.fec.sub(aux1, y2, y1);
        ctx.fec.sub(aux2, x2, x1);
        if (ctx.fec.isZero(aux2))
        {
            zklog.error("AddPointEc() got denominator=0 2");
            return ZKR_SM_MAIN_ARITH_ECRECOVER_DIVIDE_BY_ZERO;
        }
        ctx.fec.div(s, aux1, aux2);
    }

    // x3 = s*s - (x1+x2)
    ctx.fec.mul(aux1, s, s);
    ctx.fec.add(aux2, x1, x2);
    ctx.fec.sub(x3, aux1, aux2);

    // y3 = s*(x1-x3) - y1
    ctx.fec.sub(aux1, x1, x3);;
    ctx.fec.mul(aux1, aux1, s);
    ctx.fec.sub(y3, aux1, y1);

    // Save parameters and result for later reuse
    ctx.lastECAdd.bDouble = dbl;
    ctx.lastECAdd.x1 = x1;
    ctx.lastECAdd.y1 = y1;
    ctx.lastECAdd.x2 = x2;
    ctx.lastECAdd.y2 = y2;
    ctx.lastECAdd.x3 = x3;
    ctx.lastECAdd.y3 = y3;

    return ZKR_SUCCESS;
}

zkresult eval_addReadWriteAddress (Context &ctx, const mpz_class value, const Goldilocks::Element (&key)[4])
{
    zkassert(ctx.proverRequest.input.publicInputsExtended.publicInputs.forkID == 10); // fork_10
    return ((fork_10::FullTracer *)ctx.proverRequest.pFullTracer)->addReadWriteAddress(
        ctx.pols.A0[0], ctx.pols.A1[0], ctx.pols.A2[0], ctx.pols.A3[0], ctx.pols.A4[0], ctx.pols.A5[0], ctx.pols.A6[0], ctx.pols.A7[0],
        ctx.pols.B0[0], ctx.pols.B1[0], ctx.pols.B2[0], ctx.pols.B3[0], ctx.pols.B4[0], ctx.pols.B5[0], ctx.pols.B6[0], ctx.pols.B7[0],
        ctx.pols.C0[0], ctx.pols.C1[0], ctx.pols.C2[0], ctx.pols.C3[0], ctx.pols.C4[0], ctx.pols.C5[0], ctx.pols.C6[0], ctx.pols.C7[0],
        value,
        key);
}

void eval_getL1InfoRoot (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 0)
    {
        zklog.error("eval_getL1InfoRoot() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    cr.type = crt_fea;
    scalar2fea(fr, ctx.proverRequest.input.publicInputsExtended.publicInputs.l1InfoRoot, cr.fea0, cr.fea1, cr.fea2, cr.fea3, cr.fea4, cr.fea5, cr.fea6, cr.fea7);
}

void eval_getL1InfoGER (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 1)
    {
        zklog.error("eval_getL1InfoGER() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get index by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_getL1InfoGER() unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    uint64_t indexL1InfoTree = cr.scalar.get_ui();

    unordered_map<uint64_t, L1Data>::const_iterator it;
    it = ctx.proverRequest.input.l1InfoTreeData.find(indexL1InfoTree);
    if (ctx.proverRequest.input.l1InfoTreeData.find(indexL1InfoTree) == ctx.proverRequest.input.l1InfoTreeData.end())
    {
        zklog.error("eval_getL1InfoGER() could not find index=" + to_string(indexL1InfoTree) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        cr.zkResult = ZKR_SM_MAIN_INVALID_L1_INFO_TREE_INDEX;
        return;
    }

    cr.type = crt_fea;
    scalar2fea(fr, it->second.globalExitRoot, cr.fea0, cr.fea1, cr.fea2, cr.fea3, cr.fea4, cr.fea5, cr.fea6, cr.fea7);
}

void eval_getL1InfoBlockHash (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{ 
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 1)
    {
        zklog.error("eval_getL1InfoBlockHash() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get index by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_getL1InfoBlockHash() unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    uint64_t indexL1InfoTree = cr.scalar.get_ui();

    unordered_map<uint64_t, L1Data>::const_iterator it;
    it = ctx.proverRequest.input.l1InfoTreeData.find(indexL1InfoTree);
    if (ctx.proverRequest.input.l1InfoTreeData.find(indexL1InfoTree) == ctx.proverRequest.input.l1InfoTreeData.end())
    {
        zklog.error("eval_getL1InfoBlockHash() could not find index=" + to_string(indexL1InfoTree) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        cr.zkResult = ZKR_SM_MAIN_INVALID_L1_INFO_TREE_INDEX;
        return;
    }

    cr.type = crt_fea;
    scalar2fea(fr, it->second.blockHashL1, cr.fea0, cr.fea1, cr.fea2, cr.fea3, cr.fea4, cr.fea5, cr.fea6, cr.fea7);
}

void eval_getL1InfoTimestamp (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 1)
    {
        zklog.error("eval_getL1InfoTimestamp() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get index by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_getL1InfoTimestamp() unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    uint64_t indexL1InfoTree = cr.scalar.get_ui();

    unordered_map<uint64_t, L1Data>::const_iterator it;
    it = ctx.proverRequest.input.l1InfoTreeData.find(indexL1InfoTree);
    if (ctx.proverRequest.input.l1InfoTreeData.find(indexL1InfoTree) == ctx.proverRequest.input.l1InfoTreeData.end())
    {
        zklog.error("eval_getL1InfoTimestamp() could not find index=" + to_string(indexL1InfoTree) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        cr.zkResult = ZKR_SM_MAIN_INVALID_L1_INFO_TREE_INDEX;
        return;
    }

    cr.type = crt_fea;
    scalar2fea(fr, it->second.minTimestamp, cr.fea0, cr.fea1, cr.fea2, cr.fea3, cr.fea4, cr.fea5, cr.fea6, cr.fea7);
}

void eval_getL1InfoMinTimestamp (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 1)
    {
        zklog.error("eval_getL1InfoMinTimestamp() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get index by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_getL1InfoMinTimestamp() unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    uint64_t indexL1InfoTree = cr.scalar.get_ui();

    unordered_map<uint64_t, L1Data>::const_iterator it;
    it = ctx.proverRequest.input.l1InfoTreeData.find(indexL1InfoTree);
    if (ctx.proverRequest.input.l1InfoTreeData.find(indexL1InfoTree) == ctx.proverRequest.input.l1InfoTreeData.end())
    {
        zklog.error("eval_getL1InfoMinTimestamp() could not find index=" + to_string(indexL1InfoTree) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        cr.zkResult = ZKR_SM_MAIN_INVALID_L1_INFO_TREE_INDEX;
        return;
    }

    cr.type = crt_fea;
    scalar2fea(fr, it->second.minTimestamp, cr.fea0, cr.fea1, cr.fea2, cr.fea3, cr.fea4, cr.fea5, cr.fea6, cr.fea7);
}

void eval_getTimestampLimit (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 0)
    {
        zklog.error("eval_getTimestampLimit() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    cr.type = crt_fea;
    cr.fea0 = fr.fromU64(ctx.proverRequest.input.publicInputsExtended.publicInputs.timestampLimit);
    cr.fea1 = fr.zero();
    cr.fea2 = fr.zero();
    cr.fea3 = fr.zero();
    cr.fea4 = fr.zero();
    cr.fea5 = fr.zero();
    cr.fea6 = fr.zero();
    cr.fea7 = fr.zero();
}

void eval_getForcedBlockHashL1 (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 0)
    {
        zklog.error("eval_getForcedBlockHashL1() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    cr.type = crt_fea;
    scalar2fea(fr, ctx.proverRequest.input.publicInputsExtended.publicInputs.forcedData.blockHashL1, cr.fea0, cr.fea1, cr.fea2, cr.fea3, cr.fea4, cr.fea5, cr.fea6, cr.fea7);
}

mpz_class MOCK_VALUE_SMT_PROOF("0xd4e56740f876aef8c010b86a40d5f56745a118d0906a34e69aec8c0db1cb8fa3");

void eval_getSmtProof (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 2)
    {
        zklog.error("eval_getSmtProof() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get index by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_getSmtProof() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    uint64_t index = cr.scalar.get_ui();

    // Get level by executing cmd.params[1]
    evalCommand(ctx, *cmd.params[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_getSmtProof() 1 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    uint64_t level = cr.scalar.get_ui();

    mpz_class leafValue;
    if (ctx.proverRequest.input.bSkipVerifyL1InfoRoot)
    {
        leafValue = MOCK_VALUE_SMT_PROOF;
    }
    else
    {
        unordered_map<uint64_t, L1Data>::const_iterator it;
        it = ctx.proverRequest.input.l1InfoTreeData.find(index);
        if (ctx.proverRequest.input.l1InfoTreeData.find(index) == ctx.proverRequest.input.l1InfoTreeData.end())
        {
            zklog.error("eval_getSmtProof() could not find index=" + to_string(index) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
            cr.zkResult = ZKR_SM_MAIN_INVALID_L1_INFO_TREE_INDEX;
            return;
        }
        if (level >= it->second.smtProof.size())
        {
            zklog.error("eval_getSmtProof() invalid level=" + to_string(level) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
            cr.zkResult = ZKR_SM_MAIN_INVALID_L1_INFO_TREE_SMT_PROOF_VALUE;
            return;
        }
        leafValue = it->second.smtProof[level];
    }

    cr.type = crt_fea;
    scalar2fea(fr, leafValue, cr.fea0, cr.fea1, cr.fea2, cr.fea3, cr.fea4, cr.fea5, cr.fea6, cr.fea7);
}

// Compares two unsigned integers represented as arrays of BigInts.
//    a - Unsigned integer represented as an array of BigInts.
//    b - Unsigned integer represented as an array of BigInts.
//    returns 1 if a > b, -1 if a < b, 0 if a == b.

int64_t compare (const vector<mpz_class> &a, const vector<mpz_class> &b)
{
    uint64_t aSize = a.size();
    uint64_t bSize = b.size();
    
    if (aSize != bSize)
    {
        return aSize >= bSize ? 1 : -1;
    }
    for (int64_t i = aSize - 1; i >= 0; i--)
    {
        if (a[i] != b[i])
        {
            return a[i] > b[i] ? 1 : -1;
        }
    }
    return 0;
}

// Removes leading zeros from a.
//  a - Unsigned integer represented as an array of BigInts.
//  returns a with leading zeros removed. It sets a.length = 0 if a = [0n]
void trim (vector<mpz_class> &a)
{
    while (a.size() > 0)
    {
        if (a[a.size()-1] == 0)
        {
            a.pop_back();
        }
        else
        {
            break;
        }
    }
}

// Computes the subtraction of two unsigned integers a,b represented as arrays of BigInts. Assumes a >= b.
//  param a - Unsigned integer represented as an array of BigInts.
//  param b - Unsigned integer represented as an array of BigInts.
//  returns a - b.
void _MP_sub (const vector<mpz_class> &a, const vector<mpz_class> &b, vector<mpz_class> &result)
{
    uint64_t aSize = a.size();
    uint64_t bSize = b.size();
    result.clear();
    mpz_class diff = 0;
    mpz_class carry = 0;

    uint64_t i = 0;
    for (i = 0; i < bSize; i++)
    {
        diff = a[i] - b[i] - carry;
        carry = diff < 0 ? 1 : 0;
        result.emplace_back(diff + carry * ScalarTwoTo256);
    }
    for (i = bSize; i < aSize; i++)
    {
        diff = a[i] - carry;
        if (diff < 0)
        {
            diff += ScalarTwoTo256;
        }
        else
        {
            result.emplace_back(diff);
            i++;
            break;
        }
        result.emplace_back(diff);
        i++;
    }
    for (; i < aSize; i++)
    {
        result.emplace_back(a[i]);
    }
    trim(result);
}

// Computes the subtraction of two unsigned integers represented as arrays of BigInts.
//  a - Unsigned integer represented as an array of BigInts.
//  b - Unsigned integer represented as an array of BigInts.
//  returns a - b.
void MP_sub (const vector<mpz_class> &a, const vector <mpz_class> &b, vector<mpz_class> &result)
{
    result.clear();
    if (compare(a, b) >= 0)
    {
        _MP_sub(a, b, result);
    }
    else
    {
        _MP_sub(b, a, result);
        result[result.size() - 1] = -result[result.size() - 1];
    }
    if (result.size() == 0)
    {
        result.emplace_back(0);
    }
}

// Computes the multiplication of an unsigned integer represented as an array of BigInts and an unsigned integer represented as a BigInt.
//  a - Unsigned integer represented as an array of BigInts.
//  b - Unsigned integer represented as a BigInt.
//  returns a * b
void MP_short_mul (const vector<mpz_class> &a, const mpz_class &b, vector<mpz_class> &result)
{
    uint64_t aSize = a.size();
    uint64_t size = aSize;
    result.clear();
    for (uint64_t i=0; i<size; i++)
    {
        result.emplace_back(0);
    }
    mpz_class product;
    mpz_class carry = 0;
    uint64_t i;
    for (i = 0; i < aSize; i++)
    {
        product = a[i] * b + carry;
        carry = product / ScalarTwoTo256;
        result[i] = product - carry * ScalarTwoTo256;
    }
    while (carry > 0)
    {
        result.emplace_back(carry % ScalarTwoTo256);
        carry /= ScalarTwoTo256;
    }
    trim(result);
}

// Computes the normalisation of two unsigned integers a,b as explained here https://www.codeproject.com/Articles/1276311/Multiple-Precision-Arithmetic-Division-Algorithm.
//  a - Unsigned integer represented as an array of BigInts.
//  b - Unsigned integer represented as an array of BigInts.
//  returns Normalised a and b to achieve better performance for MPdiv.
void normalize (vector<mpz_class> &a, vector<mpz_class> &b, mpz_class &shift)
{
    mpz_class bm = b[b.size() - 1];
    shift = 1; // shift cannot be larger than log2(base) - 1
    while (bm < (ScalarTwoTo256 / 2))
    {
        vector<mpz_class> aux;
        MP_short_mul(b, 2, aux); // left-shift b by 2
        b.swap(aux);
        bm = b[b.size() - 1];
        shift *= 2;
    }

    vector<mpz_class> result;
    MP_short_mul(a, shift, result); // left-shift a by 2^shift
    a.swap(result);
}

void _MPdiv_short (const vector<mpz_class> &a, const mpz_class &b, vector<mpz_class> &quotient, mpz_class &remainder);

// Computes the next digit of the quotient.
//  an - Unsigned integer represented as an array of BigInts. This is the current dividend.
//  b - Unsigned integer represented as an array of BigInts.
//  returns The next digit of the quotient.
void findQn (const vector<mpz_class> &an, const vector<mpz_class> &b, mpz_class &result)
{
    uint64_t b_l = b.size();
    mpz_class bm = b[b_l - 1];
    if (compare(an, b) == -1)
    {
        result = 0;
        return;
    }

    uint64_t n = an.size();
    vector<mpz_class> aguess;
    if (an[n-1] < bm)
    {
        aguess.emplace_back(an[n-2]);
        aguess.emplace_back(an[n-1]);
    }
    else
    {
        aguess.emplace_back(an[n-1]);
    }

    if (an[n-1] < bm)
    {
        vector<mpz_class> quotient;
        mpz_class remainder;
        _MPdiv_short(aguess, bm, quotient, remainder);
        if (quotient.size() == 0)
        {
            zklog.error("findQn() called _MPdiv_short() but got a quotient with size=0");
            exitProcess();
        }
        result = quotient[0]; // this is always a single digit
    }
    else if (an[n-1] == bm)
    {
        if (b_l < n)
        {
            result = ScalarTwoTo256 - 1;
            return;
        }
        else
        {
            result = 1;
            return;
        }
    }
    else
    {
        result = 1;
        return;
    }
}

// Computes the division of two unsigned integers represented as arrays of BigInts.
//  a - Unsigned integer represented as an array of BigInts.
//  b - Unsigned integer represented as an array of BigInts.
//  returns [quotient, remainder] of a / b.
void _MPdiv (vector<mpz_class> &a, vector<mpz_class> &b, vector<mpz_class> &quotient, vector<mpz_class> &remainder)
{
    mpz_class shift;
    normalize(a, b, shift);
    int64_t a_l = a.size();
    quotient.clear();
    remainder.clear();
    vector<mpz_class> an;
    while (compare(an, b) == -1)
    {
        an.emplace(an.begin(), a[--a_l]);
    }

    vector<mpz_class> test;
    mpz_class qn;
    while (a_l >= 0)
    {
        findQn(an, b, qn);
        MP_short_mul(b, qn, test);
        while (compare(test, an) == 1)
        {
            // maximum 2 iterations
            qn--;
            vector<mpz_class> aux;
            MP_sub(test, b, aux);
            test.swap(aux);
        }

        quotient.emplace(quotient.begin(), qn);
        MP_sub(an, test, remainder);
        an = remainder;
        if (a_l == 0)
        {
            break;
        }
        an.emplace(an.begin(), a[--a_l]);
    }
    vector<mpz_class> auxQuotient;
    mpz_class auxRemainder;
    _MPdiv_short(remainder, shift, auxQuotient, auxRemainder); // TODO: review with Carlos
    remainder.swap(auxQuotient);
    trim(quotient);
    trim(remainder);
}

// Computes the division of an unsigned integer represented as an array of BigInts and an unsigned integer represented as a BigInt.
//  a - Unsigned integer represented as an array of BigInts.
//  b - Unsigned integer represented as a BigInt.
//  returns [quotient, remainder] of a / b.
void _MPdiv_short (const vector<mpz_class> &a, const mpz_class &b, vector<mpz_class> &quotient, mpz_class &remainder)
{
    uint64_t a_l = a.size();
    quotient.clear();
    quotient.insert(quotient.begin(), a_l, ScalarZero);
    remainder = 0;

    mpz_class dividendi;
    mpz_class qi;
    for (int64_t i = a_l - 1; i >= 0; i--)
    {
        dividendi = remainder*ScalarTwoTo256 + a[i];
        qi = dividendi / b;
        remainder = dividendi - qi * b;
        quotient[i] = qi;
    }
    trim(quotient);
}

// Computes the division of two unsigned integers represented as arrays of BigInts.
//  sets ctx.quotient and ctx.remainder.
void eval_MPdiv (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 4)
    {
        zklog.error("eval_MPdiv() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get addr1 by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_MPdiv() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    uint64_t addr1 = cr.scalar.get_ui();

    // Get len1 by executing cmd.params[1]
    evalCommand(ctx, *cmd.params[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_MPdiv() 1 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    uint64_t len1 = cr.scalar.get_ui();

    // Get addr2 by executing cmd.params[2]
    evalCommand(ctx, *cmd.params[2], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_MPdiv() 2 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    uint64_t addr2 = cr.scalar.get_ui();

    // Get len2 by executing cmd.params[3]
    evalCommand(ctx, *cmd.params[3], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_MPdiv() 3 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    uint64_t len2 = cr.scalar.get_ui();

    vector<mpz_class> input1;
    vector<mpz_class> input2;
    unordered_map<uint64_t, fork_10::Fea>::const_iterator it;
    mpz_class auxScalar;
    for (uint64_t i = 0; i < len1; i++)
    {
        it = ctx.mem.find(addr1 + i);
        if (it == ctx.mem.end())
        {
            zklog.error("eval_MPdiv() cound not find ctx.mem entry for address=" + to_string(addr1 + i) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
            exitProcess();
        }
        if (!ctx.multiBaseFea2scalar(fr, auxScalar, it->second.fe0, it->second.fe1, it->second.fe2, it->second.fe3, it->second.fe4, it->second.fe5, it->second.fe6, it->second.fe7))
        {
            zklog.error("eval_MPdiv() failed calling ctx.multiBaseFea2scalar for address=" + to_string(addr1 + i) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
            exitProcess();
        }
        input1.emplace_back(auxScalar);
    }
    for (uint64_t i = 0; i < len2; i++)
    {
        it = ctx.mem.find(addr2 + i);
        if (it == ctx.mem.end())
        {
            zklog.error("eval_MPdiv() cound not find ctx.mem entry for address=" + to_string(addr2 + i) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
            exitProcess();
        }
        if (!ctx.multiBaseFea2scalar(fr, auxScalar, it->second.fe0, it->second.fe1, it->second.fe2, it->second.fe3, it->second.fe4, it->second.fe5, it->second.fe6, it->second.fe7))
        {
            zklog.error("eval_MPdiv() failed calling ctx.multiBaseFea2scalar for address=" + to_string(addr2 + i) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
            exitProcess();
        }
        input2.emplace_back(auxScalar);
    }

    _MPdiv(input1, input2, ctx.quotient, ctx.remainder);
}

void eval_MPdiv_short (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 3)
    {
        zklog.error("eval_MPdiv_short() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get addr1 by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_MPdiv_short() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    uint64_t addr1 = cr.scalar.get_ui();

    // Get len1 by executing cmd.params[1]
    evalCommand(ctx, *cmd.params[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_MPdiv_short() 1 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    uint64_t len1 = cr.scalar.get_ui();

    // Get addr2 by executing cmd.params[2]
    evalCommand(ctx, *cmd.params[2], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_MPdiv_short() 2 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    mpz_class &input2 = cr.scalar;

    vector<mpz_class> input1;
    unordered_map<uint64_t, fork_10::Fea>::const_iterator it;
    mpz_class auxScalar;
    for (uint64_t i = 0; i < len1; i++)
    {
        it = ctx.mem.find(addr1 + i);
        if (it == ctx.mem.end())
        {
            zklog.error("eval_MPdiv_short() cound not find ctx.mem entry for address=" + to_string(addr1 + i) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
            exitProcess();
        }
        if (!ctx.multiBaseFea2scalar(fr, auxScalar, it->second.fe0, it->second.fe1, it->second.fe2, it->second.fe3, it->second.fe4, it->second.fe5, it->second.fe6, it->second.fe7))
        {
            zklog.error("eval_MPdiv_short() failed calling ctx.multiBaseFea2scalar for address=" + to_string(addr1 + i) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
            exitProcess();
        }
        input1.emplace_back(auxScalar);
    }
    
    _MPdiv_short(input1, input2, ctx.quotientShort, ctx.remainderShort);
}

void eval_receiveLenQuotient_short (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
    cr.type = crt_scalar;
    cr.scalar = ctx.quotientShort.size();
}

void eval_receiveQuotientChunk_short (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 1)
    {
        zklog.error("eval_receiveQuotientChunk_short() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get position by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_u64)
    {
        zklog.error("eval_receiveQuotientChunk_short() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    uint64_t position = cr.u64;

    if (position >= ctx.quotientShort.size())
    {
        zklog.error("eval_receiveQuotientChunk_short() 0 unexpected position=" + to_string(position) + " >= ctx.quotientShort.size()=" + to_string(ctx.quotientShort.size()) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }

    cr.type = crt_scalar;
    cr.scalar = ctx.quotientShort[position];
}

void eval_receiveRemainderChunk_short (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
    cr.type = crt_scalar;
    cr.scalar = ctx.remainderShort;
}

void eval_receiveLenRemainder (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
    cr.type = crt_scalar;
    cr.scalar = ctx.remainder.size();
}

void eval_receiveRemainderChunk (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 1)
    {
        zklog.error("eval_receiveRemainderChunk() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get position by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_receiveRemainderChunk() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    uint64_t position = cr.scalar.get_ui();

    if (position >= ctx.remainder.size())
    {
        zklog.error("eval_receiveRemainderChunk() 0 unexpected position=" + to_string(position) + " >= ctx.remainder.size()=" + to_string(ctx.remainder.size()) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }

    cr.type = crt_scalar;
    cr.scalar = ctx.remainder[position];
}

void eval_receiveLenQuotient (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
    cr.type = crt_scalar;
    cr.scalar = ctx.quotient.size();
}

void eval_receiveQuotientChunk (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 1)
    {
        zklog.error("eval_receiveQuotientChunk() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get position by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_u64)
    {
        zklog.error("eval_receiveQuotientChunk() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    uint64_t position = cr.u64;

    if (position >= ctx.quotient.size())
    {
        zklog.error("eval_receiveQuotientChunk() 0 unexpected position=" + to_string(position) + " >= ctx.quotient.size()=" + to_string(ctx.quotient.size()) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }

    cr.type = crt_scalar;
    cr.scalar = ctx.quotient[position];
}

// Length of the binary representation of the input scalar. If there are multiple input scalars, it returns the maximum length
void eval_receiveLen (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() == 0)
    {
        zklog.error("eval_receiveLen() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    uint64_t len = 0;

    for (uint64_t i = 0; i < cmd.params.size(); i++)
    {
        evalCommand(ctx, *cmd.params[i], cr);
        if (cr.zkResult != ZKR_SUCCESS)
        {
            return;
        }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
        if (cr.type != crt_scalar)
        {
            zklog.error("eval_receiveLen() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
            exitProcess();
        }
#endif
        mpz_class ki = cr.scalar;
        if (ki == 0)
        {
            continue;
        }

        uint64_t leni = 0;
        while (ki != 1)
        {
            ki >>= 1;
            leni++;
        }
        len = zkmax(len, leni);
    }

    cr.type = crt_u64;
    cr.u64 = len;
}

void eval_ARITH_BN254_MULFP2_X (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 4)
    {
        zklog.error("eval_ARITH_BN254_MULFP2_X() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get x1 by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_ARITH_BN254_MULFP2_X() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    RawFq::Element x1;
    fq.fromMpz(x1, cr.scalar.get_mpz_t());

    // Get y1 by executing cmd.params[1]
    evalCommand(ctx, *cmd.params[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_ARITH_BN254_MULFP2_X() 1 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    RawFq::Element y1;
    fq.fromMpz(y1, cr.scalar.get_mpz_t());

    // Get x2 by executing cmd.params[2]
    evalCommand(ctx, *cmd.params[2], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_ARITH_BN254_MULFP2_X() 2 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    RawFq::Element x2;
    fq.fromMpz(x2, cr.scalar.get_mpz_t());

    // Get y2 by executing cmd.params[3]
    evalCommand(ctx, *cmd.params[3], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_ARITH_BN254_MULFP2_X() 3 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    RawFq::Element y2;
    fq.fromMpz(y2, cr.scalar.get_mpz_t());

    // Calculate the point coordinate: x1*x2 - y1*y2
    RawFq::Element result;
    result = fq.sub(fq.mul(x1, x2), fq.mul(y1, y2));

    // Convert result to scalar
    cr.type = crt_scalar;
    fq.toMpz(cr.scalar.get_mpz_t(), result);
}

void eval_ARITH_BN254_MULFP2_Y (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 4)
    {
        zklog.error("eval_ARITH_BN254_MULFP2_Y() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get x1 by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_ARITH_BN254_MULFP2_Y() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    RawFq::Element x1;
    fq.fromMpz(x1, cr.scalar.get_mpz_t());

    // Get y1 by executing cmd.params[1]
    evalCommand(ctx, *cmd.params[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_ARITH_BN254_MULFP2_Y() 1 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    RawFq::Element y1;
    fq.fromMpz(y1, cr.scalar.get_mpz_t());

    // Get x2 by executing cmd.params[2]
    evalCommand(ctx, *cmd.params[2], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_ARITH_BN254_MULFP2_Y() 2 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    RawFq::Element x2;
    fq.fromMpz(x2, cr.scalar.get_mpz_t());

    // Get y2 by executing cmd.params[3]
    evalCommand(ctx, *cmd.params[3], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_ARITH_BN254_MULFP2_Y() 3 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    RawFq::Element y2;
    fq.fromMpz(y2, cr.scalar.get_mpz_t());

    // Calculate the point coordinate: x1*y2 + x2*y1
    RawFq::Element result;
    result = fq.add(fq.mul(x1, y2), fq.mul(x2, y1));

    // Convert result to scalar
    cr.type = crt_scalar;
    fq.toMpz(cr.scalar.get_mpz_t(), result);
}

void eval_ARITH_BN254_ADDFP2 (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 2)
    {
        zklog.error("eval_ARITH_BN254_ADDFP2() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get x1 by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_ARITH_BN254_ADDFP2() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    RawFq::Element x1;
    fq.fromMpz(x1, cr.scalar.get_mpz_t());

    // Get x2 by executing cmd.params[1]
    evalCommand(ctx, *cmd.params[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_ARITH_BN254_ADDFP2() 1 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    RawFq::Element x2;
    fq.fromMpz(x2, cr.scalar.get_mpz_t());

    // Calculate the point coordinate
    RawFq::Element result;
    result = fq.add(x1, x2);

    // Convert result to scalar
    cr.type = crt_scalar;
    fq.toMpz(cr.scalar.get_mpz_t(), result);
}

void eval_ARITH_BN254_SUBFP2 (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 2)
    {
        zklog.error("eval_ARITH_BN254_SUBFP2() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get x1 by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_ARITH_BN254_SUBFP2() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    RawFq::Element x1;
    fq.fromMpz(x1, cr.scalar.get_mpz_t());

    // Get x2 by executing cmd.params[1]
    evalCommand(ctx, *cmd.params[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_ARITH_BN254_SUBFP2() 1 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    RawFq::Element x2;
    fq.fromMpz(x2, cr.scalar.get_mpz_t());

    // Calculate the point coordinate
    RawFq::Element result;
    result = fq.sub(x1, x2);

    // Convert result to scalar
    cr.type = crt_scalar;
    fq.toMpz(cr.scalar.get_mpz_t(), result);
}

// BLS12-381

void eval_ARITH_BLS12381_MULFP2_X (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{

#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 4)
    {
        zklog.error("eval_ARITH_BLS12381_MULFP2_X() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get x1 by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_ARITH_BLS12381_MULFP2_X() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    RawBLS12_381_384::Element x1;
    bls12_381_384.fromMpz(x1, cr.scalar.get_mpz_t());

    // Get y1 by executing cmd.params[1]
    evalCommand(ctx, *cmd.params[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_ARITH_BLS12381_MULFP2_X() 1 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    RawBLS12_381_384::Element y1;
    bls12_381_384.fromMpz(y1, cr.scalar.get_mpz_t());

    // Get x2 by executing cmd.params[2]
    evalCommand(ctx, *cmd.params[2], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_ARITH_BLS12381_MULFP2_X() 2 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    RawBLS12_381_384::Element x2;
    bls12_381_384.fromMpz(x2, cr.scalar.get_mpz_t());

    // Get y2 by executing cmd.params[3]
    evalCommand(ctx, *cmd.params[3], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_ARITH_BLS12381_MULFP2_X() 3 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    RawBLS12_381_384::Element y2;
    bls12_381_384.fromMpz(y2, cr.scalar.get_mpz_t());

    // Calculate the point coordinate: x1*x2 - y1*y2
    RawBLS12_381_384::Element result;
    result = bls12_381_384.sub( bls12_381_384.mul(x1, x2), bls12_381_384.mul(y1, y2) );

    // Convert result to scalar
    mpz_class resultScalar;
    bls12_381_384.toMpz(resultScalar.get_mpz_t(), result);

    // Return as a field element array
    cr.type = crt_fea;
    scalar2fea384(fr, resultScalar, cr.fea0, cr.fea1, cr.fea2, cr.fea3, cr.fea4, cr.fea5, cr.fea6, cr.fea7);
}

void eval_ARITH_BLS12381_MULFP2_Y (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 4)
    {
        zklog.error("eval_ARITH_BLS12381_MULFP2_Y() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get x1 by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_ARITH_BLS12381_MULFP2_Y() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    RawBLS12_381_384::Element x1;
    bls12_381_384.fromMpz(x1, cr.scalar.get_mpz_t());

    // Get y1 by executing cmd.params[1]
    evalCommand(ctx, *cmd.params[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_ARITH_BLS12381_MULFP2_Y() 1 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    RawBLS12_381_384::Element y1;
    bls12_381_384.fromMpz(y1, cr.scalar.get_mpz_t());

    // Get x2 by executing cmd.params[2]
    evalCommand(ctx, *cmd.params[2], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_ARITH_BLS12381_MULFP2_Y() 2 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    RawBLS12_381_384::Element x2;
    bls12_381_384.fromMpz(x2, cr.scalar.get_mpz_t());

    // Get y2 by executing cmd.params[3]
    evalCommand(ctx, *cmd.params[3], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_ARITH_BLS12381_MULFP2_Y() 3 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    RawBLS12_381_384::Element y2;
    bls12_381_384.fromMpz(y2, cr.scalar.get_mpz_t());

    // Calculate the point coordinate: x1*y2 + x2*y1
    RawBLS12_381_384::Element result;
    result = bls12_381_384.add( bls12_381_384.mul(x1, y2), bls12_381_384.mul(x2, y1) );

    // Convert result to scalar
    mpz_class resultScalar;
    bls12_381_384.toMpz(resultScalar.get_mpz_t(), result);

    // Return as a field element array
    cr.type = crt_fea;
    scalar2fea384(fr, resultScalar, cr.fea0, cr.fea1, cr.fea2, cr.fea3, cr.fea4, cr.fea5, cr.fea6, cr.fea7);
}

void eval_ARITH_BLS12381_ADDFP2 (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 2)
    {
        zklog.error("eval_ARITH_BLS12381_ADDFP2() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get x1 by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_ARITH_BLS12381_ADDFP2() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    RawBLS12_381_384::Element x1;
    bls12_381_384.fromMpz(x1, cr.scalar.get_mpz_t());

    // Get x2 by executing cmd.params[1]
    evalCommand(ctx, *cmd.params[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_ARITH_BLS12381_ADDFP2() 1 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    RawBLS12_381_384::Element x2;
    bls12_381_384.fromMpz(x2, cr.scalar.get_mpz_t());


    // Calculate the result: x1 + X2
    RawBLS12_381_384::Element result;
    result = bls12_381_384.add( x1, x2 );

    // Convert result to scalar
    mpz_class resultScalar;
    bls12_381_384.toMpz(resultScalar.get_mpz_t(), result);

    // Return as a field element array
    cr.type = crt_fea;
    scalar2fea384(fr, resultScalar, cr.fea0, cr.fea1, cr.fea2, cr.fea3, cr.fea4, cr.fea5, cr.fea6, cr.fea7);
}

void eval_ARITH_BLS12381_SUBFP2 (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 2)
    {
        zklog.error("eval_ARITH_BLS12381_SUBFP2() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get x1 by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_ARITH_BLS12381_SUBFP2() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    RawBLS12_381_384::Element x1;
    bls12_381_384.fromMpz(x1, cr.scalar.get_mpz_t());

    // Get x2 by executing cmd.params[1]
    evalCommand(ctx, *cmd.params[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_ARITH_BLS12381_SUBFP2() 1 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    RawBLS12_381_384::Element x2;
    bls12_381_384.fromMpz(x2, cr.scalar.get_mpz_t());


    // Calculate the result: x1 - X2
    RawBLS12_381_384::Element result;
    result = bls12_381_384.sub( x1, x2 );

    // Convert result to scalar
    mpz_class resultScalar;
    bls12_381_384.toMpz(resultScalar.get_mpz_t(), result);

    // Return as a field element array
    cr.type = crt_fea;
    scalar2fea384(fr, resultScalar, cr.fea0, cr.fea1, cr.fea2, cr.fea3, cr.fea4, cr.fea5, cr.fea6, cr.fea7);
}

void eval_FROM_384_TO_256_H (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 1)
    {
        zklog.error("eval_FROM_384_TO_256_H() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get value by executing cmd.params[0]
    ctx.forceMode384 = true;
    evalCommand(ctx, *cmd.params[0], cr);
    ctx.forceMode384 = false;
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_FROM_384_TO_256_H() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Calculate the result: value & MASK_256
    cr.scalar = (cr.scalar >> 256) & ScalarMask128;

    // Return as a field element array
    cr.type = crt_fea;
    scalar2fea(fr, cr.scalar, cr.fea0, cr.fea1, cr.fea2, cr.fea3, cr.fea4, cr.fea5, cr.fea6, cr.fea7);
}

void eval_FROM_384_TO_256_L (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 1)
    {
        zklog.error("eval_FROM_384_TO_256_L() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get value by executing cmd.params[0]
    ctx.forceMode384 = true;
    evalCommand(ctx, *cmd.params[0], cr);
    ctx.forceMode384 = false;
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_FROM_384_TO_256_L() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Calculate the result: value & MASK_256
    cr.scalar &= ScalarMask256;

    // Return as a field element array
    cr.type = crt_fea;
    scalar2fea(fr, cr.scalar, cr.fea0, cr.fea1, cr.fea2, cr.fea3, cr.fea4, cr.fea5, cr.fea6, cr.fea7);
}

// Computes the "real" part of the inverse of the given Fp2 element.
void eval_fp2InvBN254_x (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 2)
    {
        zklog.error("eval_fp2InvBN254_x() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get a by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_fp2InvBN254_x() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    RawFq::Element a;
    fq.fromMpz(a, cr.scalar.get_mpz_t());

    // Get b by executing cmd.params[1]
    evalCommand(ctx, *cmd.params[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_fp2InvBN254_x() 1 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    RawFq::Element b;
    fq.fromMpz(b, cr.scalar.get_mpz_t());

    // Calculate the denominator
    RawFq::Element den;
    den = fq.add(fq.mul(a, a), fq.mul(b, b));

    // Calculate x
    RawFq::Element result;
    fq.div(result, a, den);

    // Convert back to scalar
    fq.toMpz(cr.scalar.get_mpz_t(), result);
}

// Computes the "imaginary" part of the inverse of the given Fp2 element.
void eval_fp2InvBN254_y (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 2)
    {
        zklog.error("eval_fp2InvBN254_y() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get a by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_fp2InvBN254_y() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    RawFq::Element a;
    fq.fromMpz(a, cr.scalar.get_mpz_t());

    // Get b by executing cmd.params[1]
    evalCommand(ctx, *cmd.params[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_fp2InvBN254_y() 1 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    RawFq::Element b;
    fq.fromMpz(b, cr.scalar.get_mpz_t());

    // Calculate the denominator
    RawFq::Element den;
    den = fq.add(fq.mul(a, a), fq.mul(b, b));

    // Calculate y
    RawFq::Element result;
    fq.div(result, fq.neg(b), den);

    // Convert back to scalar
    fq.toMpz(cr.scalar.get_mpz_t(), result);
}

// Computes the inverse of the given Fp element
void eval_fpBN254inv (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 1)
    {
        zklog.error("eval_fpBN254inv() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get a by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_fpBN254inv() unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get the field element from the command result
    RawFq::Element a;
    fq.fromMpz(a, cr.scalar.get_mpz_t());

    // Calculate the inverse of this field element
    RawFq::Element aInv;
    fq.inv(aInv, a);

    // Convert back to scalar
    fq.toMpz(cr.scalar.get_mpz_t(), aInv);
}

// Feijoa (fork 9) new methods:

void eval_getForcedTimestamp (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 0)
    {
        zklog.error("eval_getForcedTimestamp() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    cr.type = crt_fea;
    cr.fea0 = fr.fromU64(ctx.proverRequest.input.publicInputsExtended.publicInputs.forcedData.minTimestamp);
    cr.fea1 = fr.zero();
    cr.fea2 = fr.zero();
    cr.fea3 = fr.zero();
    cr.fea4 = fr.zero();
    cr.fea5 = fr.zero();
    cr.fea6 = fr.zero();
    cr.fea7 = fr.zero();
}

void eval_getType (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 0)
    {
        zklog.error("eval_getType() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    cr.type = crt_fea;
    cr.fea0 = fr.fromU64(ctx.proverRequest.input.publicInputsExtended.publicInputs.blobType);
    cr.fea1 = fr.zero();
    cr.fea2 = fr.zero();
    cr.fea3 = fr.zero();
    cr.fea4 = fr.zero();
    cr.fea5 = fr.zero();
    cr.fea6 = fr.zero();
    cr.fea7 = fr.zero();
}

void eval_getForcedGER (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 0)
    {
        zklog.error("eval_getForcedGER() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    cr.type = crt_fea;
    mpz_class globalExitRoot = ctx.proverRequest.input.publicInputsExtended.publicInputs.forcedData.globalExitRoot;
    scalar2fea(fr, globalExitRoot, cr.fea0, cr.fea1, cr.fea2, cr.fea3, cr.fea4, cr.fea5, cr.fea6, cr.fea7);
}

void eval_getL1HistoricRoot (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 1)
    {
        zklog.error("eval_getL1HistoricRoot() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get indexL1InfoTree by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_getL1HistoricRoot() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    uint64_t indexL1InfoTree = cr.scalar.get_ui();

    cr.type = crt_fea;
    mpz_class historicRootL1InfoTree = ctx.proverRequest.input.l1InfoTreeData[indexL1InfoTree].initialHistoricRoot;
    scalar2fea(fr, historicRootL1InfoTree, cr.fea0, cr.fea1, cr.fea2, cr.fea3, cr.fea4, cr.fea5, cr.fea6, cr.fea7);
}

void eval_getPendingRID (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 1)
    {
        zklog.error("eval_getPendingRID() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get rid by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_getPendingRID() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    uint64_t rid = cr.scalar.get_ui();

    // Find a saved context that is not equal to rid and has not been restored yet
    int _rid = -1;
    map<uint64_t, Saved>::const_iterator it;
    for (it = ctx.saved.begin(); it != ctx.saved.end(); it++)
    {
        if (it->first == rid)
        {
            continue;
        }
        if (it->second.restored)
        {
            continue;
        }
        _rid = it->first;
        break;
    }

    // Return _rid in fea0
    cr.type = crt_fea;
    cr.fea0 = fr.fromS64(_rid);
    cr.fea1 = fr.zero();
    cr.fea2 = fr.zero();
    cr.fea3 = fr.zero();
    cr.fea4 = fr.zero();
    cr.fea5 = fr.zero();
    cr.fea6 = fr.zero();
    cr.fea7 = fr.zero();
}


// Feijoa (fork 10) new methods:

void eval_getZkGasLimit (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 0)
    {
        zklog.error("eval_getZkGasLimit() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    cr.type = crt_fea;
    mpz_class zkGasLimit = ctx.proverRequest.input.publicInputsExtended.publicInputs.zkGasLimit;
    scalar2fea(fr, zkGasLimit, cr.fea0, cr.fea1, cr.fea2, cr.fea3, cr.fea4, cr.fea5, cr.fea6, cr.fea7);
}

void eval_getZ (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 0)
    {
        zklog.error("eval_getZ() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    cr.type = crt_fea;
    mpz_class pointZ = ctx.proverRequest.input.publicInputsExtended.publicInputs.pointZ;
    scalar2fea(fr, pointZ, cr.fea0, cr.fea1, cr.fea2, cr.fea3, cr.fea4, cr.fea5, cr.fea6, cr.fea7);
}

void eval_getY (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 0)
    {
        zklog.error("eval_getY() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    cr.type = crt_fea;
    mpz_class pointY = ctx.proverRequest.input.publicInputsExtended.publicInputs.pointY;
    scalar2fea(fr, pointY, cr.fea0, cr.fea1, cr.fea2, cr.fea3, cr.fea4, cr.fea5, cr.fea6, cr.fea7);
}

void eval_getVersionedHash (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 0)
    {
        zklog.error("eval_getVersionedHash() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    cr.type = crt_fea;
    mpz_class versionedHash = ctx.proverRequest.input.publicInputsExtended.publicInputs.versionedHash;
    scalar2fea(fr, versionedHash, cr.fea0, cr.fea1, cr.fea2, cr.fea3, cr.fea4, cr.fea5, cr.fea6, cr.fea7);
}

void eval_getKzgCommitmentHash (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 0)
    {
        zklog.error("eval_getKzgCommitmentHash() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    cr.type = crt_fea;
    mpz_class kzgCommitmentHash = ctx.proverRequest.input.publicInputsExtended.publicInputs.kzgCommitmentHash;
    scalar2fea(fr, kzgCommitmentHash, cr.fea0, cr.fea1, cr.fea2, cr.fea3, cr.fea4, cr.fea5, cr.fea6, cr.fea7);
}

void eval_getKzgProof (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 1)
    {
        zklog.error("eval_getKzgProof() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    cr.type = crt_fea;
    mpz_class kzgProof;
    if (cmd.params[0]->varName == "high")
    {
        kzgProof = (ctx.proverRequest.input.publicInputsExtended.publicInputs.kzgProof >> 32);
    }
    else if (cmd.params[0]->varName == "low")
    {
        kzgProof = (ctx.proverRequest.input.publicInputsExtended.publicInputs.kzgProof & ScalarMask256);
    }
    else
    {
        zklog.error("eval_getKzgProof() cmd.params[0].varName=" + cmd.params[0]->varName + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();

    }
    scalar2fea(fr, kzgProof, cr.fea0, cr.fea1, cr.fea2, cr.fea3, cr.fea4, cr.fea5, cr.fea6, cr.fea7);
}

void eval_getBlobL2HashData (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 0)
    {
        zklog.error("eval_getBlobL2HashData() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    cr.type = crt_fea;
    scalar2fea(fr, ctx.proverRequest.input.publicInputsExtended.publicInputs.blobL2HashData, cr.fea0, cr.fea1, cr.fea2, cr.fea3, cr.fea4, cr.fea5, cr.fea6, cr.fea7);
}

void eval_getForcedHashData (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 0)
    {
        zklog.error("eval_getForcedHashData() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    cr.type = crt_fea;
    mpz_class forcedHashData = ctx.proverRequest.input.publicInputsExtended.publicInputs.forcedHashData;
    scalar2fea(fr, forcedHashData, cr.fea0, cr.fea1, cr.fea2, cr.fea3, cr.fea4, cr.fea5, cr.fea6, cr.fea7);

}

void eval_getBlobLen (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 0)
    {
        zklog.error("eval_getBlobLen() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    cr.type = crt_fea;
    cr.fea0 = fr.fromU64(ctx.proverRequest.input.publicInputsExtended.publicInputs.blobData.size());
    cr.fea1 = fr.zero();
    cr.fea2 = fr.zero();
    cr.fea3 = fr.zero();
    cr.fea4 = fr.zero();
    cr.fea5 = fr.zero();
    cr.fea6 = fr.zero();
    cr.fea7 = fr.zero();
}

// Computes the inverse of the given element of the BLS12-381 scalar field
void eval_frBLS12_381inv (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 1)
    {
        zklog.error("eval_frBLS12_381inv() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get scalar to invert by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_frBLS12_381inv() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    mpz_class aScalar = cr.scalar;

    // Call BLS12-381 inverse function
    RawBLS12_381::Element a, r;
    bls12_381.fromMpz(a, aScalar.get_mpz_t());    
    bls12_381.inv(r, a);
    mpz_class rScalar;
    bls12_381.toMpz(rScalar.get_mpz_t(), r);

    // Return result
    cr.type = crt_scalar;
    cr.scalar = rScalar;
}

void eval_dump (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
    //zklog.error("eval_dump() not implemented");
    cr.type = crt_scalar;
    cr.scalar = 0;
}

// Checks if the given element of the BLS12-381 scalar field is a 4096-th root of unity
void eval_check4096Root (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 1)
    {
        zklog.error("eval_check4096Root() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get rid by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_check4096Root() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    mpz_class z = cr.scalar;
    for (uint64_t i = 0; i < 4096; i++)
    {
        if (z == BLS12_381_4096thRootsOfUnity[i])
        {
            ctx.BLS512_381root.z = z;
            ctx.BLS512_381root.index = i;

            cr.type = crt_fea;
            cr.fea0 = fr.one();
            cr.fea1 = fr.zero();
            cr.fea2 = fr.zero();
            cr.fea3 = fr.zero();
            cr.fea4 = fr.zero();
            cr.fea5 = fr.zero();
            cr.fea6 = fr.zero();
            cr.fea7 = fr.zero();

            return;
        }
    }

    cr.type = crt_fea;
    cr.fea0 = fr.zero();
    cr.fea1 = fr.zero();
    cr.fea2 = fr.zero();
    cr.fea3 = fr.zero();
    cr.fea4 = fr.zero();
    cr.fea5 = fr.zero();
    cr.fea6 = fr.zero();
    cr.fea7 = fr.zero();
}

// Returns the index of the given element of the BLS12-381 scalar field if it is a 4096-th root of unity
void eval_get4096RootIndex (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 1)
    {
        zklog.error("eval_get4096RootIndex() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get rid by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_get4096RootIndex() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    mpz_class z = cr.scalar;

    if (ctx.BLS512_381root.z == z)
    {

            cr.type = crt_fea;
            cr.fea0 = fr.fromU64(ctx.BLS512_381root.index);
            cr.fea1 = fr.zero();
            cr.fea2 = fr.zero();
            cr.fea3 = fr.zero();
            cr.fea4 = fr.zero();
            cr.fea5 = fr.zero();
            cr.fea6 = fr.zero();
            cr.fea7 = fr.zero();

            return;
    }

    for (uint64_t i = 0; i < 4096; i++)
    {
        if (z == BLS12_381_4096thRootsOfUnity[i])
        {
            ctx.BLS512_381root.z = z;
            ctx.BLS512_381root.index = i;

            cr.type = crt_fea;
            cr.fea0 = fr.one();
            cr.fea1 = fr.zero();
            cr.fea2 = fr.zero();
            cr.fea3 = fr.zero();
            cr.fea4 = fr.zero();
            cr.fea5 = fr.zero();
            cr.fea6 = fr.zero();
            cr.fea7 = fr.zero();

            return;
        }
    }

    zklog.error("eval_get4096RootIndex() root not found z=" + z.get_str(16));
    cr.zkResult = ZKR_SM_MAIN_ASSERT;

    cr.type = crt_fea;
    cr.fea0 = fr.zero();
    cr.fea1 = fr.zero();
    cr.fea2 = fr.zero();
    cr.fea3 = fr.zero();
    cr.fea4 = fr.zero();
    cr.fea5 = fr.zero();
    cr.fea6 = fr.zero();
    cr.fea7 = fr.zero();
}

void eval_getLastL1InfoTreeRoot (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 0)
    {
        zklog.error("eval_getLastL1InfoTreeRoot() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    cr.type = crt_fea;
    mpz_class lastL1InfoTreeRoot = ctx.proverRequest.input.publicInputsExtended.publicInputs.lastL1InfoTreeRoot;
    scalar2fea(fr, lastL1InfoTreeRoot, cr.fea0, cr.fea1, cr.fea2, cr.fea3, cr.fea4, cr.fea5, cr.fea6, cr.fea7);
}

void eval_getLastL1InfoTreeIndex (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 0)
    {
        zklog.error("eval_getLastL1InfoTreeIndex() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    cr.type = crt_fea;
    cr.fea0 = fr.fromU64(ctx.proverRequest.input.publicInputsExtended.publicInputs.lastL1InfoTreeIndex);
    cr.fea1 = fr.zero();
    cr.fea2 = fr.zero();
    cr.fea3 = fr.zero();
    cr.fea4 = fr.zero();
    cr.fea5 = fr.zero();
    cr.fea6 = fr.zero();
    cr.fea7 = fr.zero();
}

// Compares two unsigned integers represented as arrays of scalars (a and b)
// Returns i+1 if a > b, -i-1 if a < b, 0 if a == b, where i is the position of the first different chunk
void eval_signedComparison (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if ((cmd.params.size() != 2) && (cmd.params.size() != 3))
    {
        zklog.error("eval_signedComparison() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get addr1 by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_signedComparison() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    uint64_t addr1 = cr.scalar.get_ui();

    // Get addr2 by executing cmd.params[1]
    evalCommand(ctx, *cmd.params[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_signedComparison() 1 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    uint64_t addr2 = cr.scalar.get_ui();

    // Get len by executing cmd.params[2], or set to 1
    uint64_t len;
    if (cmd.params.size() == 3)
    {
        evalCommand(ctx, *cmd.params[2], cr);
        if (cr.zkResult != ZKR_SUCCESS)
        {
            return;
        }
    #ifdef CHECK_EVAL_COMMAND_PARAMETERS
        if (cr.type != crt_scalar)
        {
            zklog.error("eval_signedComparison() 2 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
            exitProcess();
        }
    #endif
        len = cr.scalar.get_ui();
        if (len == 0)
        {
            zklog.error("eval_signedComparison() unexpected length=0 step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
            exitProcess();
        }
    }
    else
    {
        len = 1;
    }

    for (int64_t i = len - 1; i >= 0; i--)
    {
        unordered_map<uint64_t, Fea>::const_iterator it;

        it = ctx.mem.find(addr1 + i);
        if (it == ctx.mem.end())
        {
            zklog.error("eval_signedComparison() mem not found addr1=" + to_string(addr1) + " i=" + to_string(i) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
            cr.zkResult = ZKR_SM_MAIN_ASSERT;
            return;
        }
        mpz_class input1i;
        if (!ctx.multiBaseFea2scalar(fr, input1i, it->second.fe0, it->second.fe1, it->second.fe2, it->second.fe3, it->second.fe4, it->second.fe5, it->second.fe6, it->second.fe7))
        {
            zklog.error("eval_signedComparison() failed calling ctx.multiBaseFea2scalar addr1=" + to_string(addr1) + " i=" + to_string(i) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
            cr.zkResult = ZKR_SM_MAIN_FEA2SCALAR;
            return;
        }
        
        it = ctx.mem.find(addr2 + i);
        if (it == ctx.mem.end())
        {
            zklog.error("eval_signedComparison() mem not found addr2=" + to_string(addr2) + " i=" + to_string(i) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
            cr.zkResult = ZKR_SM_MAIN_ASSERT;
            return;
        }
        mpz_class input2i;
        if (!ctx.multiBaseFea2scalar(fr, input2i, it->second.fe0, it->second.fe1, it->second.fe2, it->second.fe3, it->second.fe4, it->second.fe5, it->second.fe6, it->second.fe7))
        {
            zklog.error("eval_signedComparison() failed calling ctx.multiBaseFea2scalar addr2=" + to_string(addr1) + " i=" + to_string(i) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
            cr.zkResult = ZKR_SM_MAIN_FEA2SCALAR;
            return;
        }

        if (input1i != input2i)
        {
            int64_t iResult = input1i < input2i ? (-i-1) : (i+1);

            // Return iResult in fea0
            cr.type = crt_fea;
            cr.fea0 = fr.fromS64(iResult);
            cr.fea1 = fr.zero();
            cr.fea2 = fr.zero();
            cr.fea3 = fr.zero();
            cr.fea4 = fr.zero();
            cr.fea5 = fr.zero();
            cr.fea6 = fr.zero();
            cr.fea7 = fr.zero();

            return;
        }
    }

    // Return 0 in fea0
    cr.type = crt_fea;
    cr.fea0 = fr.zero();
    cr.fea1 = fr.zero();
    cr.fea2 = fr.zero();
    cr.fea3 = fr.zero();
    cr.fea4 = fr.zero();
    cr.fea5 = fr.zero();
    cr.fea6 = fr.zero();
    cr.fea7 = fr.zero();
}

// Compares two unsigned integers represented as arrays of scalars (a and b)
// Returns i+1 if a > b, -i-1 if a < b, 0 if a == b, where i is the position of the first different chunk
// The function does the following:
// --------------------------------
// if input < constant => return -1
// if input > constant => return  1
// if input = constant => return  0
// --------------------------------
void eval_signedComparisonWithConst (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 2)
    {
        zklog.error("eval_signedComparisonWithConst() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get addr by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_signedComparisonWithConst() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    uint64_t addr = cr.scalar.get_ui();

    // Get constant by executing cmd.params[1]
    evalCommand(ctx, *cmd.params[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_signedComparisonWithConst() 1 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    uint64_t constant = cr.scalar.get_ui();

    // Get input from memory[address]
    unordered_map<uint64_t, Fea>::const_iterator it;
    it = ctx.mem.find(addr);
    if (it == ctx.mem.end())
    {
        zklog.error("eval_signedComparisonWithConst() mem not found addr=" + to_string(addr) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        cr.zkResult = ZKR_SM_MAIN_ASSERT;
        return;
    }
    mpz_class input;
    if (!ctx.multiBaseFea2scalar(fr, input, it->second.fe0, it->second.fe1, it->second.fe2, it->second.fe3, it->second.fe4, it->second.fe5, it->second.fe6, it->second.fe7))
    {
        zklog.error("eval_signedComparison() failed calling ctx.multiBaseFea2scalar addr=" + to_string(addr) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        cr.zkResult = ZKR_SM_MAIN_FEA2SCALAR;
        return;
    }
    
    // Calculate iResult
    int64_t iResult = (input == constant) ? 0 : (input < constant) ? (-1) : 1;

    // Return iResult in fea0
    cr.type = crt_fea;
    cr.fea0 = fr.fromS64(iResult);
    cr.fea1 = fr.zero();
    cr.fea2 = fr.zero();
    cr.fea3 = fr.zero();
    cr.fea4 = fr.zero();
    cr.fea5 = fr.zero();
    cr.fea6 = fr.zero();
    cr.fea7 = fr.zero();
}

// Gets the first different chunk between two unsigned integers represented as arrays of BigInts
// Returns i, where i is the position of the first different chunk
void eval_getFirstDiffChunkRem (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 2)
    {
        zklog.error("eval_getFirstDiffChunkRem() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get addr by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_getFirstDiffChunkRem() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    uint64_t addr = cr.scalar.get_ui();

    // Get len by executing cmd.params[1]
    evalCommand(ctx, *cmd.params[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_getFirstDiffChunkRem() 1 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    uint64_t len = cr.scalar.get_ui();

    for (int64_t i = len - 1; i >= 0; i--)
    {
        unordered_map<uint64_t, Fea>::const_iterator it;

        it = ctx.mem.find(addr + i);
        if (it == ctx.mem.end())
        {
            zklog.error("eval_getFirstDiffChunkRem() mem not found addr=" + to_string(addr) + " i=" + to_string(i) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
            cr.zkResult = ZKR_SM_MAIN_ASSERT;
            return;
        }
        mpz_class inputi;
        if (!ctx.multiBaseFea2scalar(fr, inputi, it->second.fe0, it->second.fe1, it->second.fe2, it->second.fe3, it->second.fe4, it->second.fe5, it->second.fe6, it->second.fe7))
        {
            zklog.error("eval_getFirstDiffChunkRem() failed calling ctx.multiBaseFea2scalar addr=" + to_string(addr) + " i=" + to_string(i) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
            cr.zkResult = ZKR_SM_MAIN_FEA2SCALAR;
            return;
        }

        if (i >= (int64_t)ctx.remainder.size())
        {
            zklog.error("eval_getFirstDiffChunkRem() ctx.remainder.size=" + to_string(ctx.remainder.size()) + " too small addr=" + to_string(addr) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
            cr.zkResult = ZKR_SM_MAIN_ASSERT;
            return;
        }

        if (inputi != ctx.remainder[i])
        {
            // Return i in fea0
            cr.type = crt_fea;
            cr.fea0 = fr.fromS64(i);
            cr.fea1 = fr.zero();
            cr.fea2 = fr.zero();
            cr.fea3 = fr.zero();
            cr.fea4 = fr.zero();
            cr.fea5 = fr.zero();
            cr.fea6 = fr.zero();
            cr.fea7 = fr.zero();
            return;
        }
    }

    zklog.error("eval_getFirstDiffChunkRem() ctx.remainder and input are equal addr=" + to_string(addr) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
    cr.zkResult = ZKR_SM_MAIN_ASSERT;
    return;
}

void eval_getSmtProofPreviousIndex (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 2)
    {
        zklog.error("eval_getSmtProofPreviousIndex() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get index by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_getSmtProofPreviousIndex() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    uint64_t index = cr.scalar.get_ui();

    // Get level by executing cmd.params[1]
    evalCommand(ctx, *cmd.params[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_getSmtProofPreviousIndex() 1 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    uint64_t level = cr.scalar.get_ui();

    // Get leaf value
    unordered_map<uint64_t, L1Data>::const_iterator it;
    it = ctx.proverRequest.input.l1InfoTreeData.find(index);
    if (it == ctx.proverRequest.input.l1InfoTreeData.end())
    {
        zklog.error("eval_getSmtProofPreviousIndex() index not found index=" + to_string(index) + " level=" + to_string(level) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        cr.zkResult = ZKR_SM_MAIN_ASSERT;
        return;
    }
    if (level >= it->second.smtProofPreviousIndex.size())
    {
        zklog.error("eval_getSmtProofPreviousIndex() level too big index=" + to_string(index) + " level=" + to_string(level) + " smt_proof_previous_index.size=" + to_string(it->second.smtProofPreviousIndex.size()) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        cr.zkResult = ZKR_SM_MAIN_ASSERT;
        return;
    }
    mpz_class leafValue = ctx.proverRequest.input.l1InfoTreeData[index].smtProofPreviousIndex[level];

    // Return as a fea
    cr.type = crt_fea;
    scalar2fea(fr, leafValue, cr.fea0, cr.fea1, cr.fea2, cr.fea3, cr.fea4, cr.fea5, cr.fea6, cr.fea7);
}

void eval_getBatchHashData (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 0)
    {
        zklog.error("eval_getBatchHashData() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    cr.type = crt_fea;
    scalar2fea(fr, ctx.proverRequest.input.publicInputsExtended.publicInputs.batchHashData, cr.fea0, cr.fea1, cr.fea2, cr.fea3, cr.fea4, cr.fea5, cr.fea6, cr.fea7);
}

void eval_dumpRegs                    (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
    cr.type = crt_scalar;
    cr.scalar = 0;
}

void eval_dumphex                     (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
    cr.type = crt_scalar;
    cr.scalar = 0;
}

void eval_break                       (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
    cr.type = crt_scalar;
    cr.scalar = 0;
}

bool Arith_isFreeInEquation (uint64_t arithEquation)
{
    return (arithEquation == ARITH_MOD) || (arithEquation == ARITH_384_MOD) || (arithEquation == ARITH_256TO384);
}

zkresult Arith_calculate (Context &ctx, Goldilocks::Element &fi0, Goldilocks::Element &fi1, Goldilocks::Element &fi2, Goldilocks::Element &fi3, Goldilocks::Element &fi4, Goldilocks::Element &fi5, Goldilocks::Element &fi6, Goldilocks::Element &fi7)
{
    mpz_class result;

    zkassert(ctx.pZKPC != NULL);
    uint64_t arithEquation = ctx.rom.line[*ctx.pZKPC].arithEquation;
    zkassert(ctx.pStep != NULL);
    uint64_t step = *ctx.pStep;
    mpz_class _a, _b, _c, _d;
    switch(arithEquation)
    {
        case ARITH_MOD:
            if (!fea2scalar(fr, _c, ctx.pols.C0[step], ctx.pols.C1[step], ctx.pols.C2[step], ctx.pols.C3[step], ctx.pols.C4[step], ctx.pols.C5[step], ctx.pols.C6[step], ctx.pols.C7[step]))
            {
                zklog.error("Arith_calculate() ARITH_MOD failed calling fea2scalar(C)");
                return ZKR_SM_MAIN_FEA2SCALAR;
            }
            if (!fea2scalar(fr, _d, ctx.pols.D0[step], ctx.pols.D1[step], ctx.pols.D2[step], ctx.pols.D3[step], ctx.pols.D4[step], ctx.pols.D5[step], ctx.pols.D6[step], ctx.pols.D7[step]))
            {
                zklog.error("Arith_calculate() ARITH_MOD failed calling fea2scalar(D)");
                return ZKR_SM_MAIN_FEA2SCALAR;
            }
        case ARITH_256TO384:
            if (!fea2scalar(fr, _a, ctx.pols.A0[step], ctx.pols.A1[step], ctx.pols.A2[step], ctx.pols.A3[step], ctx.pols.A4[step], ctx.pols.A5[step], ctx.pols.A6[step], ctx.pols.A7[step]))
            {
                zklog.error("Arith_calculate() ARITH_256TO384/ARITH_MOD failed calling fea2scalar(A)");
                return ZKR_SM_MAIN_FEA2SCALAR;
            }
            if (!fea2scalar(fr, _b, ctx.pols.B0[step], ctx.pols.B1[step], ctx.pols.B2[step], ctx.pols.B3[step], ctx.pols.B4[step], ctx.pols.B5[step], ctx.pols.B6[step], ctx.pols.B7[step]))
            {
                zklog.error("Arith_calculate() ARITH_256TO384/ARITH_MOD failed calling fea2scalar(A)");
                return ZKR_SM_MAIN_FEA2SCALAR;
            }
            break;
        case ARITH_384_MOD:
            if (!fea384ToScalar(fr, _a, ctx.pols.A0[step], ctx.pols.A1[step], ctx.pols.A2[step], ctx.pols.A3[step], ctx.pols.A4[step], ctx.pols.A5[step], ctx.pols.A6[step], ctx.pols.A7[step]))
            {
                zklog.error("Arith_calculate() ARITH_384_MOD failed calling fea384ToScalar(A)");
                return ZKR_SM_MAIN_FEA2SCALAR;
            }
            if (!fea384ToScalar(fr, _b, ctx.pols.B0[step], ctx.pols.B1[step], ctx.pols.B2[step], ctx.pols.B3[step], ctx.pols.B4[step], ctx.pols.B5[step], ctx.pols.B6[step], ctx.pols.B7[step]))
            {
                zklog.error("Arith_calculate() ARITH_384_MOD failed calling fea384ToScalar(A)");
                return ZKR_SM_MAIN_FEA2SCALAR;
            }
            if (!fea384ToScalar(fr, _c, ctx.pols.C0[step], ctx.pols.C1[step], ctx.pols.C2[step], ctx.pols.C3[step], ctx.pols.C4[step], ctx.pols.C5[step], ctx.pols.C6[step], ctx.pols.C7[step]))
            {
                zklog.error("Arith_calculate() ARITH_384_MOD failed calling fea384ToScalar(C)");
                return ZKR_SM_MAIN_FEA2SCALAR;
            }
            if (!fea384ToScalar(fr, _d, ctx.pols.D0[step], ctx.pols.D1[step], ctx.pols.D2[step], ctx.pols.D3[step], ctx.pols.D4[step], ctx.pols.D5[step], ctx.pols.D6[step], ctx.pols.D7[step]))
            {
                zklog.error("Arith_calculate() ARITH_384_MOD failed calling fea384ToScalar(D)");
                return ZKR_SM_MAIN_FEA2SCALAR;
            }
            break;
        default:
            zklog.error("Arith_calculate() invalid arithEquation=" + to_string(arithEquation));
            exitProcess();
    }

    if (arithEquation == ARITH_256TO384)
    {
        if (_b > ScalarMask128)
        {
            zklog.error("Arith_calculate() ARITH_256TO384 invalid b=" + _b.get_str(16) + " > 128 bits");
            return ZKR_SM_MAIN_ARITH_MISMATCH;
        }
        result = _a + (_b << 256);
        scalar2fea384(fr, result, fi0, fi1, fi2, fi3, fi4, fi5, fi6, fi7);
        return ZKR_SUCCESS;
    }
    if (_d == 0)
    {
        zklog.error("Arith_calculate() modular arithmetic is undefined when D is zero");
        return ZKR_SM_MAIN_ARITH_MISMATCH;
    }
    result = ((_a * _b) + _c) % _d;
    if (arithEquation == ARITH_MOD)
    {
        scalar2fea(fr, result, fi0, fi1, fi2, fi3, fi4, fi5, fi6, fi7);
    }
    else
    {
        scalar2fea384(fr, result, fi0, fi1, fi2, fi3, fi4, fi5, fi6, fi7);
    }
    return ZKR_SUCCESS;
}

zkresult Arith_verify ( Context &ctx,
                        Goldilocks::Element &op0, Goldilocks::Element &op1, Goldilocks::Element &op2, Goldilocks::Element &op3, Goldilocks::Element &op4, Goldilocks::Element &op5, Goldilocks::Element &op6, Goldilocks::Element &op7,
                        MainExecRequired *required,
                        uint64_t &same12, uint64_t &useE, uint64_t &useCD)
{
    zkassert(ctx.pZKPC != NULL);
    uint64_t arithEquation = ctx.rom.line[*ctx.pZKPC].arithEquation;
    zkassert(ctx.pStep != NULL);
    uint64_t step = *ctx.pStep;
    
    same12 = 0;
    useE = 1;
    useCD = 1;
    
    bool is384 = (arithEquation >= ARITH_384_MOD);

    if ((arithEquation == ARITH_BASE) || (arithEquation == ARITH_MOD) || (arithEquation == ARITH_384_MOD))
    {
        useE = 0;

        mpz_class A, B, C, D, op;

        if (is384)
        {
            if (!fea384ToScalar(fr, A, ctx.pols.A0[step], ctx.pols.A1[step], ctx.pols.A2[step], ctx.pols.A3[step], ctx.pols.A4[step], ctx.pols.A5[step], ctx.pols.A6[step], ctx.pols.A7[step]))
            {
                zklog.error("Arith_verify() failed calling fea384ToScalar(A)");
                return ZKR_SM_MAIN_FEA2SCALAR;
            }
            if (!fea384ToScalar(fr, B, ctx.pols.B0[step], ctx.pols.B1[step], ctx.pols.B2[step], ctx.pols.B3[step], ctx.pols.B4[step], ctx.pols.B5[step], ctx.pols.B6[step], ctx.pols.B7[step]))
            {
                zklog.error("Arith_verify() failed calling fea384ToScalar(A)");
                return ZKR_SM_MAIN_FEA2SCALAR;
            }
            if (!fea384ToScalar(fr, C, ctx.pols.C0[step], ctx.pols.C1[step], ctx.pols.C2[step], ctx.pols.C3[step], ctx.pols.C4[step], ctx.pols.C5[step], ctx.pols.C6[step], ctx.pols.C7[step]))
            {
                zklog.error("Arith_verify() failed calling fea384ToScalar(C)");
                return ZKR_SM_MAIN_FEA2SCALAR;
            }
            if (!fea384ToScalar(fr, D, ctx.pols.D0[step], ctx.pols.D1[step], ctx.pols.D2[step], ctx.pols.D3[step], ctx.pols.D4[step], ctx.pols.D5[step], ctx.pols.D6[step], ctx.pols.D7[step]))
            {
                zklog.error("Arith_verify() failed calling fea384ToScalar(D)");
                return ZKR_SM_MAIN_FEA2SCALAR;
            }
            if (!fea384ToScalar(fr, op, op0, op1, op2, op3, op4, op5, op6, op7))
            {
                zklog.error("Arith_verify() failed calling fea384ToScalar(op)");
                return ZKR_SM_MAIN_FEA2SCALAR;
            }
        }
        else
        {
            if (!fea2scalar(fr, A, ctx.pols.A0[step], ctx.pols.A1[step], ctx.pols.A2[step], ctx.pols.A3[step], ctx.pols.A4[step], ctx.pols.A5[step], ctx.pols.A6[step], ctx.pols.A7[step]))
            {
                zklog.error("Arith_verify() failed calling fea2scalar(A)");
                return ZKR_SM_MAIN_FEA2SCALAR;
            }
            if (!fea2scalar(fr, B, ctx.pols.B0[step], ctx.pols.B1[step], ctx.pols.B2[step], ctx.pols.B3[step], ctx.pols.B4[step], ctx.pols.B5[step], ctx.pols.B6[step], ctx.pols.B7[step]))
            {
                zklog.error("Arith_verify() failed calling fea2scalar(A)");
                return ZKR_SM_MAIN_FEA2SCALAR;
            }
            if (!fea2scalar(fr, C, ctx.pols.C0[step], ctx.pols.C1[step], ctx.pols.C2[step], ctx.pols.C3[step], ctx.pols.C4[step], ctx.pols.C5[step], ctx.pols.C6[step], ctx.pols.C7[step]))
            {
                zklog.error("Arith_verify() failed calling fea2scalar(C)");
                return ZKR_SM_MAIN_FEA2SCALAR;
            }
            if (!fea2scalar(fr, D, ctx.pols.D0[step], ctx.pols.D1[step], ctx.pols.D2[step], ctx.pols.D3[step], ctx.pols.D4[step], ctx.pols.D5[step], ctx.pols.D6[step], ctx.pols.D7[step]))
            {
                zklog.error("Arith_verify() failed calling fea2scalar(D)");
                return ZKR_SM_MAIN_FEA2SCALAR;
            }
            if (!fea2scalar(fr, op, op0, op1, op2, op3, op4, op5, op6, op7))
            {
                zklog.error("Arith_verify() failed calling fea2scalar(op)");
                return ZKR_SM_MAIN_FEA2SCALAR;
            }
        }

        mpz_class left, right;
        if (arithEquation == ARITH_BASE)
        {
            left = A*B + C;
            right = (D<<256) + op;
        }
        else
        {
            if (D == 0)
            {
                zklog.error("Arith_verify() Modular arithmetic is undefined when D is zero arithEquation=" + arith2string(arithEquation));
                return ZKR_SM_MAIN_ARITH_MISMATCH;
            }
            left = (A*B + C)%D;
            right = op;
        }

        if (left != right)
        {
            zklog.error("Arith_verify() equation=" + arith2string(arithEquation) + " does not match, A=" + A.get_str(16) + " B=" + B.get_str(16) + " C=" + C.get_str(16) + " D=" + D.get_str(16) + " op=" + op.get_str(16) + " left=" + left.get_str(16) + " right=" + right.get_str(16));
            return ZKR_SM_MAIN_ARITH_MISMATCH;
        }

#ifdef USE_REQUIRED
        if (required != NULL)
        {
            ArithAction arithAction;

            arithAction.x1[0] = ctx.pols.A0[step];
            arithAction.x1[1] = ctx.pols.A1[step];
            arithAction.x1[2] = ctx.pols.A2[step];
            arithAction.x1[3] = ctx.pols.A3[step];
            arithAction.x1[4] = ctx.pols.A4[step];
            arithAction.x1[5] = ctx.pols.A5[step];
            arithAction.x1[6] = ctx.pols.A6[step];
            arithAction.x1[7] = ctx.pols.A7[step];

            arithAction.y1[0] = ctx.pols.B0[step];
            arithAction.y1[1] = ctx.pols.B1[step];
            arithAction.y1[2] = ctx.pols.B2[step];
            arithAction.y1[3] = ctx.pols.B3[step];
            arithAction.y1[4] = ctx.pols.B4[step];
            arithAction.y1[5] = ctx.pols.B5[step];
            arithAction.y1[6] = ctx.pols.B6[step];
            arithAction.y1[7] = ctx.pols.B7[step];

            arithAction.x2[0] = ctx.pols.C0[step];
            arithAction.x2[1] = ctx.pols.C1[step];
            arithAction.x2[2] = ctx.pols.C2[step];
            arithAction.x2[3] = ctx.pols.C3[step];
            arithAction.x2[4] = ctx.pols.C4[step];
            arithAction.x2[5] = ctx.pols.C5[step];
            arithAction.x2[6] = ctx.pols.C6[step];
            arithAction.x2[7] = ctx.pols.C7[step];

            arithAction.y2[0] = ctx.pols.D0[step];
            arithAction.y2[1] = ctx.pols.D1[step];
            arithAction.y2[2] = ctx.pols.D2[step];
            arithAction.y2[3] = ctx.pols.D3[step];
            arithAction.y2[4] = ctx.pols.D4[step];
            arithAction.y2[5] = ctx.pols.D5[step];
            arithAction.y2[6] = ctx.pols.D6[step];
            arithAction.y2[7] = ctx.pols.D7[step];

            arithAction.x3[0] = fr.zero();
            arithAction.x3[1] = fr.zero();
            arithAction.x3[2] = fr.zero();
            arithAction.x3[3] = fr.zero();
            arithAction.x3[4] = fr.zero();
            arithAction.x3[5] = fr.zero();
            arithAction.x3[6] = fr.zero();
            arithAction.x3[7] = fr.zero();

            arithAction.y3[0] = op0;
            arithAction.y3[1] = op1;
            arithAction.y3[2] = op2;
            arithAction.y3[3] = op3;
            arithAction.y3[4] = op4;
            arithAction.y3[5] = op5;
            arithAction.y3[6] = op6;
            arithAction.y3[7] = op7;

            arithAction.equation = arithEquation;

            required->Arith.push_back(arithAction);
        }
#endif

    }
    else if (((arithEquation >= ARITH_ECADD_DIFFERENT) && (arithEquation <= ARITH_BN254_SUBFP2)) ||
             ((arithEquation >= ARITH_BLS12381_MULFP2) && (arithEquation <= ARITH_BLS12381_SUBFP2)))
    {   
        const bool dbl = (arithEquation == ARITH_ECADD_SAME);
        mpz_class x1, y1, x2, y2, x3, y3;
        if (is384)
        {
            if (!fea384ToScalar(fr, x1, ctx.pols.A0[step], ctx.pols.A1[step], ctx.pols.A2[step], ctx.pols.A3[step], ctx.pols.A4[step], ctx.pols.A5[step], ctx.pols.A6[step], ctx.pols.A7[step]))
            {
                zklog.error("Arith_verify() failed calling fea384ToScalar(A)");
                return ZKR_SM_MAIN_FEA2SCALAR;
            }
            if (!fea384ToScalar(fr, y1, ctx.pols.B0[step], ctx.pols.B1[step], ctx.pols.B2[step], ctx.pols.B3[step], ctx.pols.B4[step], ctx.pols.B5[step], ctx.pols.B6[step], ctx.pols.B7[step]))
            {
                zklog.error("Arith_verify() failed calling fea384ToScalar(A)");
                return ZKR_SM_MAIN_FEA2SCALAR;
            }
            if (dbl)
            {
                x2 = x1;
                y2 = y1;
            }
            else
            {
                if (!fea384ToScalar(fr, x2, ctx.pols.C0[step], ctx.pols.C1[step], ctx.pols.C2[step], ctx.pols.C3[step], ctx.pols.C4[step], ctx.pols.C5[step], ctx.pols.C6[step], ctx.pols.C7[step]))
                {
                    zklog.error("Arith_verify() failed calling fea384ToScalar(C)");
                    return ZKR_SM_MAIN_FEA2SCALAR;
                }
                if (!fea384ToScalar(fr, y2, ctx.pols.D0[step], ctx.pols.D1[step], ctx.pols.D2[step], ctx.pols.D3[step], ctx.pols.D4[step], ctx.pols.D5[step], ctx.pols.D6[step], ctx.pols.D7[step]))
                {
                    zklog.error("Arith_verify() failed calling fea384ToScalar(D)");
                    return ZKR_SM_MAIN_FEA2SCALAR;
                }
            }
            if (!fea384ToScalar(fr, x3, ctx.pols.E0[step], ctx.pols.E1[step], ctx.pols.E2[step], ctx.pols.E3[step], ctx.pols.E4[step], ctx.pols.E5[step], ctx.pols.E6[step], ctx.pols.E7[step]))
            {
                zklog.error("Arith_verify() failed calling fea384ToScalar(A)");
                return ZKR_SM_MAIN_FEA2SCALAR;
            }
            if (!fea384ToScalar(fr, y3, op0, op1, op2, op3, op4, op5, op6, op7))
            {
                zklog.error("Arith_verify() failed calling fea384ToScalar(op)");
                return ZKR_SM_MAIN_FEA2SCALAR;
            }
        }
        else
        {
            if (!fea2scalar(fr, x1, ctx.pols.A0[step], ctx.pols.A1[step], ctx.pols.A2[step], ctx.pols.A3[step], ctx.pols.A4[step], ctx.pols.A5[step], ctx.pols.A6[step], ctx.pols.A7[step]))
            {
                zklog.error("Arith_verify() failed calling fea2scalar(A)");
                return ZKR_SM_MAIN_FEA2SCALAR;
            }
            if (!fea2scalar(fr, y1, ctx.pols.B0[step], ctx.pols.B1[step], ctx.pols.B2[step], ctx.pols.B3[step], ctx.pols.B4[step], ctx.pols.B5[step], ctx.pols.B6[step], ctx.pols.B7[step]))
            {
                zklog.error("Arith_verify() failed calling fea2scalar(A)");
                return ZKR_SM_MAIN_FEA2SCALAR;
            }
            if (dbl)
            {
                x2 = x1;
                y2 = y1;
            }
            else
            {
                if (!fea2scalar(fr, x2, ctx.pols.C0[step], ctx.pols.C1[step], ctx.pols.C2[step], ctx.pols.C3[step], ctx.pols.C4[step], ctx.pols.C5[step], ctx.pols.C6[step], ctx.pols.C7[step]))
                {
                    zklog.error("Arith_verify() failed calling fea2scalar(C)");
                    return ZKR_SM_MAIN_FEA2SCALAR;
                }
                if (!fea2scalar(fr, y2, ctx.pols.D0[step], ctx.pols.D1[step], ctx.pols.D2[step], ctx.pols.D3[step], ctx.pols.D4[step], ctx.pols.D5[step], ctx.pols.D6[step], ctx.pols.D7[step]))
                {
                    zklog.error("Arith_verify() failed calling fea2scalar(D)");
                    return ZKR_SM_MAIN_FEA2SCALAR;
                }
            }
            if (!fea2scalar(fr, x3, ctx.pols.E0[step], ctx.pols.E1[step], ctx.pols.E2[step], ctx.pols.E3[step], ctx.pols.E4[step], ctx.pols.E5[step], ctx.pols.E6[step], ctx.pols.E7[step]))
            {
                zklog.error("Arith_verify() failed calling fea2scalar(A)");
                return ZKR_SM_MAIN_FEA2SCALAR;
            }
            if (!fea2scalar(fr, y3, op0, op1, op2, op3, op4, op5, op6, op7))
            {
                zklog.error("Arith_verify() failed calling fea2scalar(op)");
                return ZKR_SM_MAIN_FEA2SCALAR;
            }
        }

        RawFec::Element s;
        if ((arithEquation == ARITH_ECADD_DIFFERENT) || (arithEquation == ARITH_ECADD_SAME))
        {
            if (dbl)
            {
                // Convert to RawFec::Element
                RawFec::Element fecX1, fecY1, fecX2, fecY2;
                fec.fromMpz(fecX1, x1.get_mpz_t());
                fec.fromMpz(fecY1, y1.get_mpz_t());
                fec.fromMpz(fecX2, x2.get_mpz_t());
                fec.fromMpz(fecY2, y2.get_mpz_t());

                // Division by zero must be managed by ROM before call ARITH
                RawFec::Element divisor;
                divisor = fec.add(fecY1, fecY1);

                same12 = 1;
                useCD = 0;

                if (fec.isZero(divisor))
                {
                    zklog.info("Arith_verify() Invalid arithmetic op, DivisionByZero arithEquation=" + arith2string(arithEquation));
                    return ZKR_SM_MAIN_ARITH_ECRECOVER_DIVIDE_BY_ZERO;
                }

                RawFec::Element fecThree;
                fec.fromUI(fecThree, 3);
                fec.div(s, fec.mul(fecThree, fec.mul(fecX1, fecX1)), divisor);
            }
            else
            {
                // Convert to RawFec::Element
                RawFec::Element fecX1, fecY1, fecX2, fecY2;
                fec.fromMpz(fecX1, x1.get_mpz_t());
                fec.fromMpz(fecY1, y1.get_mpz_t());
                fec.fromMpz(fecX2, x2.get_mpz_t());
                fec.fromMpz(fecY2, y2.get_mpz_t());

                // Division by zero must be managed by ROM before call ARITH
                RawFec::Element deltaX = fec.sub(fecX2, fecX1);
                if (fec.isZero(deltaX))
                {
                    zklog.info("Arith_verify() Invalid arithmetic op, DivisionByZero arithEquation=" + arith2string(arithEquation));
                    return ZKR_SM_MAIN_ARITH_ECRECOVER_DIVIDE_BY_ZERO;
                }
                fec.div(s, fec.sub(fecY2, fecY1), deltaX);
            }
        }

        mpz_class _x3,_y3;

        switch (arithEquation)
        {
            case ARITH_ECADD_DIFFERENT:
            {
                // Convert to RawFec::Element
                RawFec::Element fecX1, fecY1, fecX2, fecY2;
                fec.fromMpz(fecX1, x1.get_mpz_t());
                fec.fromMpz(fecY1, y1.get_mpz_t());
                fec.fromMpz(fecX2, x2.get_mpz_t());
                fec.fromMpz(fecY2, y2.get_mpz_t());

                RawFec::Element _fecX3;
                _fecX3 = fec.sub( fec.mul(s, s), fec.add(fecX1, fecX2) );
                fec.toMpz(_x3.get_mpz_t(), _fecX3);

                RawFec::Element _fecY3;
                _fecY3 = fec.sub( fec.mul(s, fec.sub(fecX1, _fecX3)), fecY1);
                fec.toMpz(_y3.get_mpz_t(), _fecY3);
                
                break;
            }
            case ARITH_ECADD_SAME:
            {
                // Convert to RawFec::Element
                RawFec::Element fecX1, fecY1;
                fec.fromMpz(fecX1, x1.get_mpz_t());
                fec.fromMpz(fecY1, y1.get_mpz_t());

                RawFec::Element _fecX3;
                _fecX3 = fec.sub( fec.mul(s, s), fec.add(fecX1, fecX1) );
                fec.toMpz(_x3.get_mpz_t(), _fecX3);

                RawFec::Element _fecY3;
                _fecY3 = fec.sub( fec.mul(s, fec.sub(fecX1, _fecX3)), fecY1);
                fec.toMpz(_y3.get_mpz_t(), _fecY3);

                break;
            }
            case ARITH_BN254_MULFP2:
            case ARITH_BLS12381_MULFP2:
            {
                if (is384) // BLS12_381_384
                {
                    // Convert to RawFec::Element
                    RawBLS12_381_384::Element fpX1, fpY1, fpX2, fpY2;
                    bls12_381_384.fromMpz(fpX1, x1.get_mpz_t());
                    bls12_381_384.fromMpz(fpY1, y1.get_mpz_t());
                    bls12_381_384.fromMpz(fpX2, x2.get_mpz_t());
                    bls12_381_384.fromMpz(fpY2, y2.get_mpz_t());

                    RawBLS12_381_384::Element _fpX3;
                    _fpX3 = bls12_381_384.sub( bls12_381_384.mul(fpX1, fpX2), bls12_381_384.mul(fpY1, fpY2));
                    bls12_381_384.toMpz(_x3.get_mpz_t(), _fpX3);

                    RawBLS12_381_384::Element _fpY3;
                    _fpY3 = bls12_381_384.add( bls12_381_384.mul(fpY1, fpX2), bls12_381_384.mul(fpX1, fpY2));
                    bls12_381_384.toMpz(_y3.get_mpz_t(), _fpY3);
                }
                else // BN256
                {
                    // Convert to RawFec::Element
                    RawFq::Element fpX1, fpY1, fpX2, fpY2;
                    bn254.fromMpz(fpX1, x1.get_mpz_t());
                    bn254.fromMpz(fpY1, y1.get_mpz_t());
                    bn254.fromMpz(fpX2, x2.get_mpz_t());
                    bn254.fromMpz(fpY2, y2.get_mpz_t());

                    RawFq::Element _fpX3;
                    _fpX3 = bn254.sub( bn254.mul(fpX1, fpX2), bn254.mul(fpY1, fpY2)) ;
                    bn254.toMpz(_x3.get_mpz_t(), _fpX3);

                    RawFq::Element _fpY3;
                    _fpY3 = bn254.add( bn254.mul(fpY1, fpX2), bn254.mul(fpX1, fpY2) );
                    bn254.toMpz(_y3.get_mpz_t(), _fpY3);                   
                }
                break;
            }
            case ARITH_BN254_ADDFP2:
            case ARITH_BLS12381_ADDFP2:
            {
                if (is384) // BLS12_381_384
                {
                    // Convert to RawFec::Element
                    RawBLS12_381_384::Element fpX1, fpY1, fpX2, fpY2;
                    bls12_381_384.fromMpz(fpX1, x1.get_mpz_t());
                    bls12_381_384.fromMpz(fpY1, y1.get_mpz_t());
                    bls12_381_384.fromMpz(fpX2, x2.get_mpz_t());
                    bls12_381_384.fromMpz(fpY2, y2.get_mpz_t());

                    RawBLS12_381_384::Element _fpX3;
                    _fpX3 = bls12_381_384.add(fpX1, fpX2);
                    bls12_381_384.toMpz(_x3.get_mpz_t(), _fpX3);

                    RawBLS12_381_384::Element _fpY3;
                    _fpY3 = bls12_381_384.add(fpY1, fpY2);
                    bls12_381_384.toMpz(_y3.get_mpz_t(), _fpY3);
                }
                else // BN256
                {
                    // Convert to RawFec::Element
                    RawFq::Element fpX1, fpY1, fpX2, fpY2;
                    bn254.fromMpz(fpX1, x1.get_mpz_t());
                    bn254.fromMpz(fpY1, y1.get_mpz_t());
                    bn254.fromMpz(fpX2, x2.get_mpz_t());
                    bn254.fromMpz(fpY2, y2.get_mpz_t());

                    RawFq::Element _fpX3;
                    _fpX3 = bn254.add(fpX1, fpX2);
                    bn254.toMpz(_x3.get_mpz_t(), _fpX3);

                    RawFq::Element _fpY3;
                    _fpY3 = bn254.add(fpY1, fpY2);
                    bn254.toMpz(_y3.get_mpz_t(), _fpY3);
                }
                break;
            }
            case ARITH_BN254_SUBFP2:
            case ARITH_BLS12381_SUBFP2:
            {
                if (is384) // BLS12_381_384
                {
                    // Convert to RawFec::Element
                    RawBLS12_381_384::Element fpX1, fpY1, fpX2, fpY2;
                    bls12_381_384.fromMpz(fpX1, x1.get_mpz_t());
                    bls12_381_384.fromMpz(fpY1, y1.get_mpz_t());
                    bls12_381_384.fromMpz(fpX2, x2.get_mpz_t());
                    bls12_381_384.fromMpz(fpY2, y2.get_mpz_t());

                    RawBLS12_381_384::Element _fpX3;
                    _fpX3 = bls12_381_384.sub(fpX1, fpX2);
                    bls12_381_384.toMpz(_x3.get_mpz_t(), _fpX3);

                    RawBLS12_381_384::Element _fpY3;
                    _fpY3 = bls12_381_384.sub(fpY1, fpY2);
                    bls12_381_384.toMpz(_y3.get_mpz_t(), _fpY3);
                }
                else // BN256
                {
                    // Convert to RawFec::Element
                    RawFq::Element fpX1, fpY1, fpX2, fpY2;
                    bn254.fromMpz(fpX1, x1.get_mpz_t());
                    bn254.fromMpz(fpY1, y1.get_mpz_t());
                    bn254.fromMpz(fpX2, x2.get_mpz_t());
                    bn254.fromMpz(fpY2, y2.get_mpz_t());

                    RawFq::Element _fpX3;
                    _fpX3 = bn254.sub(fpX1, fpX2);
                    bn254.toMpz(_x3.get_mpz_t(), _fpX3);

                    RawFq::Element _fpY3;
                    _fpY3 = bn254.sub(fpY1, fpY2);
                    bn254.toMpz(_y3.get_mpz_t(), _fpY3);
                }
                break;
            }
            default:
            {
                zklog.error("Arith_verify() invalid arithEquation=" + arith2string(arithEquation));
                exitProcess();
            }
        }

        bool x3eq = (x3 == _x3);
        bool y3eq = (y3 == _y3);

        if (!x3eq || !y3eq)
        {
            zklog.error("Arith_verify() Arithmetic point does not match arithEquation=" + arith2string(arithEquation) +
                " x3=" + x3.get_str(16) +
                " _x3=" + _x3.get_str(16) +
                " y3=" + y3.get_str(16) +
                " _y3=" + _y3.get_str(16));
            return ZKR_SM_MAIN_ARITH_MISMATCH;
        }

#ifdef USE_REQUIRED
        if (required != NULL)
        {
            ArithAction arithAction;

            arithAction.x1[0] = ctx.pols.A0[step];
            arithAction.x1[1] = ctx.pols.A1[step];
            arithAction.x1[2] = ctx.pols.A2[step];
            arithAction.x1[3] = ctx.pols.A3[step];
            arithAction.x1[4] = ctx.pols.A4[step];
            arithAction.x1[5] = ctx.pols.A5[step];
            arithAction.x1[6] = ctx.pols.A6[step];
            arithAction.x1[7] = ctx.pols.A7[step];

            arithAction.y1[0] = ctx.pols.B0[step];
            arithAction.y1[1] = ctx.pols.B1[step];
            arithAction.y1[2] = ctx.pols.B2[step];
            arithAction.y1[3] = ctx.pols.B3[step];
            arithAction.y1[4] = ctx.pols.B4[step];
            arithAction.y1[5] = ctx.pols.B5[step];
            arithAction.y1[6] = ctx.pols.B6[step];
            arithAction.y1[7] = ctx.pols.B7[step];

            arithAction.x2[0] = dbl ? ctx.pols.A0[step] : ctx.pols.C0[step];
            arithAction.x2[1] = dbl ? ctx.pols.A1[step] : ctx.pols.C1[step];
            arithAction.x2[2] = dbl ? ctx.pols.A2[step] : ctx.pols.C2[step];
            arithAction.x2[3] = dbl ? ctx.pols.A3[step] : ctx.pols.C3[step];
            arithAction.x2[4] = dbl ? ctx.pols.A4[step] : ctx.pols.C4[step];
            arithAction.x2[5] = dbl ? ctx.pols.A5[step] : ctx.pols.C5[step];
            arithAction.x2[6] = dbl ? ctx.pols.A6[step] : ctx.pols.C6[step];
            arithAction.x2[7] = dbl ? ctx.pols.A7[step] : ctx.pols.C7[step];

            arithAction.y2[0] = dbl ? ctx.pols.B0[step] : ctx.pols.D0[step];
            arithAction.y2[1] = dbl ? ctx.pols.B1[step] : ctx.pols.D1[step];
            arithAction.y2[2] = dbl ? ctx.pols.B2[step] : ctx.pols.D2[step];
            arithAction.y2[3] = dbl ? ctx.pols.B3[step] : ctx.pols.D3[step];
            arithAction.y2[4] = dbl ? ctx.pols.B4[step] : ctx.pols.D4[step];
            arithAction.y2[5] = dbl ? ctx.pols.B5[step] : ctx.pols.D5[step];
            arithAction.y2[6] = dbl ? ctx.pols.B6[step] : ctx.pols.D6[step];
            arithAction.y2[7] = dbl ? ctx.pols.B7[step] : ctx.pols.D7[step];

            arithAction.x3[0] = ctx.pols.E0[step];
            arithAction.x3[1] = ctx.pols.E1[step];
            arithAction.x3[2] = ctx.pols.E2[step];
            arithAction.x3[3] = ctx.pols.E3[step];
            arithAction.x3[4] = ctx.pols.E4[step];
            arithAction.x3[5] = ctx.pols.E5[step];
            arithAction.x3[6] = ctx.pols.E6[step];
            arithAction.x3[7] = ctx.pols.E7[step];

            arithAction.y3[0] = op0;
            arithAction.y3[1] = op1;
            arithAction.y3[2] = op2;
            arithAction.y3[3] = op3;
            arithAction.y3[4] = op4;
            arithAction.y3[5] = op5;
            arithAction.y3[6] = op6;
            arithAction.y3[7] = op7;

            arithAction.equation = arithEquation;

            required->Arith.push_back(arithAction);
        }
#endif

    } 
    else if (arithEquation == ARITH_256TO384)
    {
        mpz_class A, B, op;
        if (!fea2scalar(fr, A, ctx.pols.A0[step], ctx.pols.A1[step], ctx.pols.A2[step], ctx.pols.A3[step], ctx.pols.A4[step], ctx.pols.A5[step], ctx.pols.A6[step], ctx.pols.A7[step]))
        {
            zklog.error("Arith_verify() failed calling fea2scalar(A)");
            return ZKR_SM_MAIN_FEA2SCALAR;
        }
        if (!fea2scalar(fr, B, ctx.pols.B0[step], ctx.pols.B1[step], ctx.pols.B2[step], ctx.pols.B3[step], ctx.pols.B4[step], ctx.pols.B5[step], ctx.pols.B6[step], ctx.pols.B7[step]))
        {
            zklog.error("Arith_verify() failed calling fea2scalar(A)");
            return ZKR_SM_MAIN_FEA2SCALAR;
        }
        if (!fea384ToScalar(fr, op, op0, op1, op2, op3, op4, op5, op6, op7))
        {
            zklog.error("Arith_verify() failed calling fea384ToScalar(op)");
            return ZKR_SM_MAIN_FEA2SCALAR;
        }

        if (B > ScalarMask128)
        {
            zklog.error("Arith_verify() ARITH_256TO384 B is too big B=" + B.get_str(16) + " > 128 bits ");
            return ZKR_SM_MAIN_ARITH_MISMATCH;
        }

        mpz_class expected = A + (B << 256);

        if (op != expected)
        {
            zklog.error("Arith_verify() Arithmetic ARITH_256TO384 point does not match op=" + op.get_str(16) + " expected=" + expected.get_str(16) + " A=" + A.get_str(16) + " B=" + B.get_str(16));
            return ZKR_SM_MAIN_ARITH_MISMATCH;
        }

        useCD = 0;
        useE = 0;

#ifdef USE_REQUIRED
        if (required != NULL)
        {
            ArithAction arithAction;

            arithAction.x1[0] = ctx.pols.A0[step];
            arithAction.x1[1] = ctx.pols.A1[step];
            arithAction.x1[2] = ctx.pols.A2[step];
            arithAction.x1[3] = ctx.pols.A3[step];
            arithAction.x1[4] = ctx.pols.A4[step];
            arithAction.x1[5] = ctx.pols.A5[step];
            arithAction.x1[6] = ctx.pols.A6[step];
            arithAction.x1[7] = ctx.pols.A7[step];

            arithAction.y1[0] = ctx.pols.B0[step];
            arithAction.y1[1] = ctx.pols.B1[step];
            arithAction.y1[2] = ctx.pols.B2[step];
            arithAction.y1[3] = ctx.pols.B3[step];
            arithAction.y1[4] = ctx.pols.B4[step];
            arithAction.y1[5] = ctx.pols.B5[step];
            arithAction.y1[6] = ctx.pols.B6[step];
            arithAction.y1[7] = ctx.pols.B7[step];

            arithAction.x2[0] = fr.zero();
            arithAction.x2[1] = fr.zero();
            arithAction.x2[2] = fr.zero();
            arithAction.x2[3] = fr.zero();
            arithAction.x2[4] = fr.zero();
            arithAction.x2[5] = fr.zero();
            arithAction.x2[6] = fr.zero();
            arithAction.x2[7] = fr.zero();

            arithAction.y2[0] = fr.zero();
            arithAction.y2[1] = fr.zero();
            arithAction.y2[2] = fr.zero();
            arithAction.y2[3] = fr.zero();
            arithAction.y2[4] = fr.zero();
            arithAction.y2[5] = fr.zero();
            arithAction.y2[6] = fr.zero();
            arithAction.y2[7] = fr.zero();

            arithAction.x3[0] = fr.zero();
            arithAction.x3[1] = fr.zero();
            arithAction.x3[2] = fr.zero();
            arithAction.x3[3] = fr.zero();
            arithAction.x3[4] = fr.zero();
            arithAction.x3[5] = fr.zero();
            arithAction.x3[6] = fr.zero();
            arithAction.x3[7] = fr.zero();

            arithAction.y3[0] = op0;
            arithAction.y3[1] = op1;
            arithAction.y3[2] = op2;
            arithAction.y3[3] = op3;
            arithAction.y3[4] = op4;
            arithAction.y3[5] = op5;
            arithAction.y3[6] = op6;
            arithAction.y3[7] = op7;

            arithAction.equation = arithEquation;
            
            required->Arith.push_back(arithAction);
        }
#endif
    }
    else
    {
        zklog.error("Arith_verify() invalid arithEquation=" + arith2string(arithEquation));
        exitProcess();
    }

    return ZKR_SUCCESS;
}

uint64_t sign_BLS12_381p (mpz_class &a)
{
    return (a > ((BLS12_381p_prime - ScalarOne) / ScalarTwo)) ? 1 : 0;
}

// Returns the square root of the input scalar in the BLS12-381 base field or 2^384-1 if the input scalar is not a square
void eval_fpBLS12_381_sqrt (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 2)
    {
        zklog.error("eval_fpBLS12_381_sqrt() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get index by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_fpBLS12_381_sqrt() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    RawBLS12_381_384::Element a;
    BLS12_381p.fromMpz(a, cr.scalar.get_mpz_t());

    // Get level by executing cmd.params[1]
    evalCommand(ctx, *cmd.params[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_fpBLS12_381_sqrt() 1 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    uint64_t sign = cr.scalar.get_ui(); // Also knows as "parity"

    cr.type = crt_scalar;

    if (BLS12_381p.isZero(a))
    {
        cr.scalar = 0;
        return;
    }

    // Get the exponent
    mpz_class exponent;
    exponent = (BLS12_381p_prime - ScalarOne) / ScalarTwo;
    string exponentBa;
    exponentBa = scalar2ba(exponent);

    // Check if it is a square in this finite field
    RawBLS12_381_384::Element sqrt;
    BLS12_381p.exp(sqrt, a, (uint8_t *)exponentBa.c_str(), exponentBa.size());

    if (!BLS12_381p.eq(sqrt, BLS12_381p.one()))
    {
        // a is not a square in Fp
        // return 2^384-1, the maximum allowed value that can be represented
        cr.scalar = ScalarMask384;
        return;
    }

    // You don't need to apply the standard Tonelli-Shanks algorithm because p = 3 mod 4
    // Get the exponent
    exponent = (BLS12_381p_prime + ScalarOne) / ScalarFour;
    BLS12_381p.exp(sqrt, a, (uint8_t *)exponentBa.c_str(), exponentBa.size());

    mpz_class sqrtScalar;
    BLS12_381p.toMpz(sqrtScalar.get_mpz_t(), sqrt);

    // If sign/parity does not match, negate to get the other sqrt
    if (sign_BLS12_381p(sqrtScalar) != sign)
    {
        BLS12_381p.neg(sqrt);
        BLS12_381p.toMpz(sqrtScalar.get_mpz_t(), sqrt);
    }

    cr.scalar = sqrtScalar;
    return;
}

// Returns the length of the binary representation of the input scalar. If there are multiple input scalars, it returns the maximum length.
void eval_lenBinDecomp (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 1)
    {
        zklog.error("eval_lenBinDecomp() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get index by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_lenBinDecomp() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    mpz_class k = cr.scalar;

    // Count the k bits
    cr.type = crt_scalar;
    uint64_t len = 0;
    while (k > 0)
    {
        k >>= 1;
        len++;
    }

    // Return the length
    cr.type = crt_scalar;
    cr.scalar = len;
}

//Computes the inverse of the given element of the BLS12-381 scalar field
void eval_frBLS12_381_inv (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 1)
    {
        zklog.error("eval_frBLS12_381_inv() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get index by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_frBLS12_381_inv() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    RawBLS12_381::Element a;
    BLS12_381r.fromMpz(a, cr.scalar.get_mpz_t());

    // Calculate the inverse
    RawBLS12_381::Element aInv;
    BLS12_381r.inv(aInv, a);

    // Return the result
    cr.type = crt_scalar;
    BLS12_381r.toMpz(cr.scalar.get_mpz_t(), aInv);
}

//Computes the inverse of the given element of the BLS12-381 base field
void eval_fpBLS12_381_inv (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 2)
    {
        zklog.error("eval_fpBLS12_381_inv() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get index by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_fpBLS12_381_inv() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    RawBLS12_381_384::Element a;
    BLS12_381p.fromMpz(a, cr.scalar.get_mpz_t());

    // Calculate the inverse
    RawBLS12_381_384::Element aInv;
    BLS12_381p.inv(aInv, a);

    // Return the result
    cr.type = crt_scalar;
    BLS12_381p.toMpz(cr.scalar.get_mpz_t(), aInv);
}

// Computes the "real" part of the inverse of the given Fp2 element
void eval_fp2BLS12_381_inv_x (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 1)
    {
        zklog.error("eval_fp2BLS12_381_inv_x() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get index by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_fp2BLS12_381_inv_x() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    RawBLS12_381_384::Element a;
    BLS12_381p.fromMpz(a, cr.scalar.get_mpz_t());

    // Get index by executing cmd.params[1]
    evalCommand(ctx, *cmd.params[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_fp2BLS12_381_inv_x() 1 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    RawBLS12_381_384::Element b;
    BLS12_381p.fromMpz(b, cr.scalar.get_mpz_t());

    // Calculate the denominator
    RawBLS12_381_384::Element den;
    den = BLS12_381p.add(BLS12_381p.mul(a, a), BLS12_381p.mul(b, b));

    // Calculate the result
    RawBLS12_381_384::Element result;
    BLS12_381p.div(result, a, den);

    cr.type = crt_scalar;
    BLS12_381p.toMpz(cr.scalar.get_mpz_t(), result);
}

// Computes the "imaginary" part of the inverse of the given Fp2 element
void eval_fp2BLS12_381_inv_y (Context &ctx, const RomCommand &cmd, CommandResult &cr)
{
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    // Check parameters list size
    if (cmd.params.size() != 2)
    {
        zklog.error("eval_fp2BLS12_381_inv_y() invalid number of parameters=" + to_string(cmd.params.size()) + " function " + function2String(cmd.function) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif

    // Get index by executing cmd.params[0]
    evalCommand(ctx, *cmd.params[0], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_fp2BLS12_381_inv_y() 0 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    RawBLS12_381_384::Element a;
    BLS12_381p.fromMpz(a, cr.scalar.get_mpz_t());

    // Get index by executing cmd.params[1]
    evalCommand(ctx, *cmd.params[1], cr);
    if (cr.zkResult != ZKR_SUCCESS)
    {
        return;
    }
#ifdef CHECK_EVAL_COMMAND_PARAMETERS
    if (cr.type != crt_scalar)
    {
        zklog.error("eval_fp2BLS12_381_inv_y() 1 unexpected command result type: " + to_string(cr.type) + " step=" + to_string(*ctx.pStep) + " zkPC=" + to_string(*ctx.pZKPC) + " line=" + ctx.rom.line[*ctx.pZKPC].toString(ctx.fr) + " uuid=" + ctx.proverRequest.uuid);
        exitProcess();
    }
#endif
    RawBLS12_381_384::Element b;
    BLS12_381p.fromMpz(b, cr.scalar.get_mpz_t());

    // Calculate the denominator
    RawBLS12_381_384::Element den;
    den = BLS12_381p.add(BLS12_381p.mul(a, a), BLS12_381p.mul(b, b));

    // Calculate the result
    RawBLS12_381_384::Element negB;
    negB = BLS12_381p.neg(b);
    RawBLS12_381_384::Element result;
    BLS12_381p.div(result, negB, den);

    cr.type = crt_scalar;
    BLS12_381p.toMpz(cr.scalar.get_mpz_t(), result);
}

void CommandResult::toFea (Context &ctx, Goldilocks::Element &fi0, Goldilocks::Element &fi1, Goldilocks::Element &fi2, Goldilocks::Element &fi3, Goldilocks::Element &fi4, Goldilocks::Element &fi5, Goldilocks::Element &fi6, Goldilocks::Element &fi7)
{
    // Copy fi=command result, depending on its type 
    switch (type)
    {
    case crt_fea:
        fi0 = fea0;
        fi1 = fea1;
        fi2 = fea2;
        fi3 = fea3;
        fi4 = fea4;
        fi5 = fea5;
        fi6 = fea6;
        fi7 = fea7;
        break;
    case crt_fe:
        fi0 = fe;
        fi1 = fr.zero();
        fi2 = fr.zero();
        fi3 = fr.zero();
        fi4 = fr.zero();
        fi5 = fr.zero();
        fi6 = fr.zero();
        fi7 = fr.zero();
        break;
    case crt_scalar:
        ctx.scalarToMultiBaseFea(fr, scalar, fi0, fi1, fi2, fi3, fi4, fi5, fi6, fi7);
        break;
    case crt_u16:
        fi0 = fr.fromU64(u16);
        fi1 = fr.zero();
        fi2 = fr.zero();
        fi3 = fr.zero();
        fi4 = fr.zero();
        fi5 = fr.zero();
        fi6 = fr.zero();
        fi7 = fr.zero();
        break;
    case crt_u32:
        fi0 = fr.fromU64(u32);
        fi1 = fr.zero();
        fi2 = fr.zero();
        fi3 = fr.zero();
        fi4 = fr.zero();
        fi5 = fr.zero();
        fi6 = fr.zero();
        fi7 = fr.zero();
        break;
    case crt_u64:
        fi0 = fr.fromU64(u64);
        fi1 = fr.zero();
        fi2 = fr.zero();
        fi3 = fr.zero();
        fi4 = fr.zero();
        fi5 = fr.zero();
        fi6 = fr.zero();
        fi7 = fr.zero();
        break;
    default:
        zklog.error("CommandResult::toFea() Unexpected command result type: " + to_string(type));
        exitProcess();
    }
}

} // namespace