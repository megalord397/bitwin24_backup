// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018 The MAC developers
// Copyright (c) 2019 The BITWIN24 developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "libzerocoin/Params.h"
#include "chainparams.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
    (0, uint256("0x000000772114d8a6c2b9a36be07cc74b05a67db48bdacb1cac16aa96b3f29308"));
static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1540617434, // * UNIX timestamp of last checkpoint block
    1,          // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    1000        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1740710,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1540617434,
    0,
    100};

libzerocoin::ZerocoinParams* CChainParams::Zerocoin_Params(bool useModulusV1) const
{
    assert(this);
    static CBigNum bnHexModulus = 0;
    if (!bnHexModulus)
        bnHexModulus.SetHex(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParamsHex = libzerocoin::ZerocoinParams(bnHexModulus);
    static CBigNum bnDecModulus = 0;
    if (!bnDecModulus)
        bnDecModulus.SetDec(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParamsDec = libzerocoin::ZerocoinParams(bnDecModulus);

    if (useModulusV1)
        return &ZCParamsHex;

    return &ZCParamsDec;
}

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0x1b;
        pchMessageStart[1] = 0xee;
        pchMessageStart[2] = 0x2c;
        pchMessageStart[3] = 0x6e;
        vAlertPubKey = ParseHex("0457cbd0e4325dbcdf526f4b27d459383b7ddca9bb4bdfa61ada823a3f8522b6f9c7afbc4375f51ed22989de3eced8612dd99f110936fe313938d22b5f838c775a");
        nDefaultPort = 24072;
        bnProofOfWorkLimit = (~uint256(0) >> 6);
        nSubsidyHalvingInterval = 210000;
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // BITWIN24: 1 day
        nTargetSpacing = 1 * 60;  // BITWIN24: 1 minute
        nMaturity = 100;
        nMasternodeCountDrift = 20;
        nBlockReward = 5 * COIN;
        nBlockReward2 = 0.5 * COIN;
        nMaxSupply = 21000000 * COIN;
        nBlocksPerYear = 365 * 24 * 60;
        nMasternodeTolerance = 2;

        // The mainnet swap amount of 37334000 was determined based on money supply 36757502.29915683 BITWIN24 @ block 416959.
        // 576500 BITWIN24 were added to this amount to represent the expected POW production at a rate of 960 blocks/day
        // with 100 BITWIN24 reward for 6 days along with POS production at 7% per stake.
        nSwapAmount = 6998995 * COIN; // Amount of coins generated for the swap.
        nSwapPoWBlocks = 10; // Number of PoW blocks used to generate the Swap amount.
        nSwapCoinbaseValue = nSwapAmount / nSwapPoWBlocks; // The swap amount will be distributed uniformly through PoW coinbase.
   
        /** Height or Time Based Activations **/

        // Start PoS/Staking when the PoW coinbase has completed a confirmation cycle.
        // Excluding Swap blocks.
        nLastPOWBlock = nMaturity * 2 + nSwapPoWBlocks + 1;
        nModifierUpdateBlock = 1;

        nMaxMoneyOut = nSwapAmount;

        // Disabling zerocoin for now.
        nZerocoinStartHeight = std::numeric_limits<int>::max();
        nZerocoinStartTime = std::numeric_limits<int>::max();
        nBlockZerocoinV2 = std::numeric_limits<int>::max();
        nBlockRecalculateAccumulators = std::numeric_limits<int>::max();
        nBlockEnforceSerialRange = std::numeric_limits<int>::max();
        nBlockFirstFraudulent = std::numeric_limits<int>::max();
        nBlockLastGoodCheckpoint = std::numeric_limits<int>::max();
        nBlockEnforceInvalidUTXO = std::numeric_limits<int>::max();
        nInvalidAmountFiltered = 0; //Amount of invalid coins filtered through exchanges, that should be considered valid
        nEnforceNewSporkKey = 1525158000; //!> Sporks signed after (GMT): Tuesday, May 1, 2018 7:00:00 AM GMT must use the new spork key
        nRejectOldSporkKey = 1527811200; //!> Fully reject old spork key after (GMT): Friday, June 1, 2018 12:00:00 AM

        /**
         * Build the genesis block. Note that the output of the genesis coinbase cannot
         * be spent as it did not originally exist in the database.
         *
         * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
         *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
         *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
         *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
         *   vMerkleTree: e0028e
         */
        const char* pszTimestamp = "Bitcoin is 10 years old today — here's a look back at its crazy history";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 0 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("045777AA773E88BBBF2B31FB859D4E3C73B527B6F1FB12FFFDD6B331AB585C1CBD0CCBAF0E40B947235A49B04A806AE3C38FBC23BAB96CCF3252A312BE0BB0E61C") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = 1572494400;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 92133373;

        hashGenesisBlock = genesis.GetHash();
        string strHexHash = genesis.GetHash().GetHex();
        string strmerkle = genesis.hashMerkleRoot.GetHex();
        string test = genesis.ToString();
        assert(hashGenesisBlock == uint256("0x000000772114d8a6c2b9a36be07cc74b05a67db48bdacb1cac16aa96b3f29308"));
        assert(genesis.hashMerkleRoot == uint256("0x59b032829f89c69e4e3f4f378b46aed9f6898d4c4ea1a4786e05e640c2a53b9c"));

        vSeeds.push_back(CDNSSeedData("134.209.244.29", "134.209.244.29"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 38); // Start with 'G'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 15); // Start with '7'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 142); // start with 'z'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x02)(0x2D)(0x25)(0x33).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x02)(0x21)(0x31)(0x2B).convert_to_container<std::vector<unsigned char> >();
        // 	BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x77).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;
        fZeroCoinEnabled = false;

        nPoolMaxTransactions = 3;
        strSporkKey = "04955AD7A25E39ED4A9DDC0977972909E6221092EB706F380687CDDF4A520DCE4E87E2576E2353615075FF0CFFBD0777A04AA51834FF64D58622ACB37CB7FAD425";
        strSporkKeyOld = "043306b8b174192b112b453536cc899e862351b8b408640de8a150876447c9a62eb7f4125ae9ecd965f45ecf2e5bb259e32fcf7cec84cedbb3fb67ecdab15a2081";
        strObfuscationPoolDummyAddress = "GSkW1Z9n94rdjfHd1VWVXfDgcDkMXhYNWr";
        nStartMasternodePayments = genesis.nTime;

        /** Zerocoin */
        zerocoinModulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
            "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
            "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
            "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
            "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
            "31438167899885040445364023527381951378636564391212010397122822120720357";

        nMaxZerocoinSpendsPerTransaction = 7; // Assume about 20kb each
        nMinZerocoinMintFee = 1 * CENT; //high fee required for zerocoin mints
        nMintRequiredConfirmations = 20; //the maximum amount of confirmations until accumulated in 19
        nRequiredAccumulation = 1;
        nDefaultSecurityLevel = 100; //full security level for accumulators
        nZerocoinHeaderVersion = 4; //Block headers must be this version once zerocoin is active
        nZerocoinRequiredStakeDepth = 200; //The required confirmations for a zbwi to be stakable

        nBudget_Fee_Confirmations = 6; // Number of confirmations for the finalization fee
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0x26;
        pchMessageStart[1] = 0xb7;
        pchMessageStart[2] = 0xdc;
        pchMessageStart[3] = 0xf9;
        vAlertPubKey = ParseHex("042abb9fbdbdad29ed411ffc04c465f3f3a0e0f0ae183b4777886f771bdc143481eb0ad3c5fb67ecbcd0568f5f602039fde73015975d998f242d7fe5c3d2a059ce");
        nDefaultPort = 17174;
        nBlockReward = 5 * COIN;
        nMaxSupply = 400000000 * COIN;

        nSwapAmount = 6999005 * COIN; // Amount of coins generated for the swap.
        nSwapPoWBlocks = 10; // Number of PoW blocks used to generate the Swap amount.
        nSwapCoinbaseValue = nSwapAmount / nSwapPoWBlocks; // The swap amount will be distributed uniformly through PoW coinbase.

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1540944001;
        genesis.nNonce = 2376021;

        hashGenesisBlock = genesis.GetHash();
//        assert(hashGenesisBlock == uint256("0x00000cf101fc4c4a5e609584707447bba50392041314bf6e2476b4a815904b0c"));

        vFixedSeeds.clear();
        vSeeds.clear();
//        vSeeds.push_back(CDNSSeedData("BitWin24.io", "nakamoto.BitWin24.io"));
//        vSeeds.push_back(CDNSSeedData("litemint.com", "nakamoto.litemint.com"));
//        vSeeds.push_back(CDNSSeedData("35.241.249.95", "35.241.249.95"));
//        vSeeds.push_back(CDNSSeedData("35.227.76.49", "35.227.76.49"));
//        vSeeds.push_back(CDNSSeedData("35.190.191.73", "35.190.191.73"));
        
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 65); // Testnet bitwin24 addresses start with 'T'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 10);  // Testnet bitwin24 script addresses start with '5'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 135);     // Testnet private keys start with 'w'
        // Testnet bitwin24 BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x3a)(0x80)(0x61)(0xa0).convert_to_container<std::vector<unsigned char> >();
        // Testnet bitwin24 BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x3a)(0x80)(0x58)(0x37).convert_to_container<std::vector<unsigned char> >();
        // Testnet bitwin24 BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strSporkKey = "045b8dc72b3429ef0ab12d80c26bb1025d2eee81abe3f284face90df0cdd341b1ab0b28d3741d6fb876c33b4355b9d4b15e1747c805b2416d0016c9546e9094f03";
        strSporkKeyOld = "045b8dc72b3429ef0ab12d80c26bb1025d2eee81abe3f284face90df0cdd341b1ab0b28d3741d6fb876c33b4355b9d4b15e1747c805b2416d0016c9546e9094f03";
        strObfuscationPoolDummyAddress = "TSegSsewERsEduCNCNnenuNDU2tYNrAj8Y";
        nStartMasternodePayments = genesis.nTime;
        nBudget_Fee_Confirmations = 3; // Number of confirmations for the finalization fee. We have to make this very short
                                       // here because we only have a 8 block finalization window on testnet
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        strNetworkID = "regtest";
        pchMessageStart[0] = 0xa1;
        pchMessageStart[1] = 0xb7;
        pchMessageStart[2] = 0xdc;
        pchMessageStart[3] = 0xac;
        nSubsidyHalvingInterval = 150;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 24 * 60 * 60; // BITWIN24: 1 day
        nTargetSpacing = 1 * 60;        // BITWIN24: 1 minutes
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        nBlockReward = 5 * COIN;
        nMaxSupply = 400000000 * COIN;

        genesis.nTime = 1540944002;
        genesis.nNonce = 2;

        hashGenesisBlock = genesis.GetHash();
        string strHexHash = genesis.GetHash().GetHex();
        nDefaultPort = 24072;
//        assert(hashGenesisBlock == uint256("0xed556f3925965379bf46d9c15375cab46741dc47dc9113c6bf5b569ea316ca34"));

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 51478;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval) { nSubsidyHalvingInterval = anSubsidyHalvingInterval; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    case CBaseChainParams::UNITTEST:
        return unitTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
