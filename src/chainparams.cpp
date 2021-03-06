// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "assert.h"

#include "chainparams.h"
#include "main.h"
#include "util.h"

#include <boost/assign/list_of.hpp>

using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

void MineGenesis(CBlock genesis){
    // This will figure out a valid hash and Nonce if you're creating a different genesis block:
    uint256 hashTarget = CBigNum().SetCompact(Params().ProofOfWorkLimit().GetCompact()).getuint256();
    printf("Target: %s\n", hashTarget.GetHex().c_str());
    uint256 newhash = genesis.GetHash();
    uint256 besthash;
    memset(&besthash,0xFF,32);
    while (newhash > hashTarget) {
    	++genesis.nNonce;
        if (genesis.nNonce == 0){
            printf("NONCE WRAPPED, incrementing time");
            ++genesis.nTime;
        }
	newhash = genesis.GetHash();
	if(newhash < besthash){
	    besthash=newhash;
	    printf("New best: %s\n", newhash.GetHex().c_str());
	}
    }
    printf("Found Genesis, Nonce: %u, Hash: %s\n", genesis.nNonce, genesis.GetHash().GetHex().c_str());
    printf("Gensis Hash Merkle: %s\n", genesis.hashMerkleRoot.ToString().c_str());
}

//
// Main network
//

// Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress> &vSeedsOut, const SeedSpec6 *data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7*24*60*60;
    for (unsigned int i = 0; i < count; i++)
    {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

class CMainParams : public CChainParams {
public:
    CMainParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0xf5;
        pchMessageStart[1] = 0xc1;
        pchMessageStart[2] = 0xaf;
        pchMessageStart[3] = 0xca;
        vAlertPubKey = ParseHex("0423f28ebab52a8db8da4e810d963133ce98a246c52f6b8eb37ab6866a47bb8f7267827fb8dfb1cc98fb311862f8d18eda4fc13c726a5f15f21493df4c7c88ec86");
        nDefaultPort = 11225;
        nRPCPort = 11226;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 19);

        // Build the genesis block. Note that the output of the genesis coinbase cannot
        // be spent as it did not originally exist in the database.
        //
        //CBlock(hash=000001faef25dec4fbcf906e6242621df2c183bf232f263d0ba5b101911e4563, ver=1, hashPrevBlock=0000000000000000000000000000000000000000000000000000000000000000, hashMerkleRoot=12630d16a97f24b287c8c2594dda5fb98c9e6c70fc61d44191931ea2aa08dc90, nTime=1393221600, nBits=1e0fffff, nNonce=164482, vtx=1, vchBlockSig=)
        //  Coinbase(hash=12630d16a9, nTime=1393221600, ver=1, vin.size=1, vout.size=1, nLockTime=0)
        //    CTxIn(COutPoint(0000000000, 4294967295), coinbase 00012a24323020466562203230313420426974636f696e2041544d7320636f6d6520746f20555341)
        //    CTxOut(empty)
        //  vMerkleTree: 12630d16a9
        const char* pszTimestamp = "Reuters-Mar/26/2018-U.S.,EU to expel more than 100 Russian diplomats over UK nerve attack";
        std::vector<CTxIn> vin;
        vin.resize(1);
        vin[0].scriptSig = CScript() << 0 << CBigNum(42) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        std::vector<CTxOut> vout;
        vout.resize(1);
        vout[0].SetEmpty();
        CTransaction txNew(1, 1522091847, vin, vout, 0);
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime    = 1522091847;
        genesis.nBits    = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce   = 7407050;

        hashGenesisBlock = genesis.GetHash();

        //MineGenesis(genesis);
        assert(hashGenesisBlock         ==    uint256("0x00001123368370feb0997f471423e4445be205b9947e7053c762886317274d2a"));
        assert(genesis.hashMerkleRoot   ==    uint256("0xb73886f7ac4c55766d661049b166475eb1946f5b9c0ed7a566e0c13d9221d471"));

        vFixedSeeds.clear();
        vSeeds.clear();

        vSeeds.push_back(CDNSSeedData("Seednode1", "d20.dnotescoin.com"));
        vSeeds.push_back(CDNSSeedData("Seednode1", "d21.dnotescoin.com"));
        vSeeds.push_back(CDNSSeedData("Seednode1", "d22.dnotescoin.com"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 63);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 125);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1, (63+128));
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xC2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0xDD).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        nLastPOWBlock = 2000;

        nCRISPPayoutInterval = 60 * 24 * 30; //43200 minutes in 30 days
        nCRISPPayoutLag = 60 * 24 * 7; //10080 minutes in 30 days
        nMaxCoinBaseOutputsPerBlock = 10000;
        nMaxAddressBalancesPerBlock = 10000;
        nMaxTransactionsPerBlock = 100;
        nMaxInputsAndOutputsPerBlock = 300;
        nMaxInputsAndOutputsPerTransaction = 50;
        nCRISPPayoutPercentage = .005; //half a percent, paid out roughly monthly
     }

    virtual const CBlock& GenesisBlock() const { return genesis; }
    virtual Network NetworkID() const { return CChainParams::MAIN; }

    virtual const vector<CAddress>& FixedSeeds() const {
        return vFixedSeeds;
    }
protected:
    CBlock genesis;
    vector<CAddress> vFixedSeeds;
};
static CMainParams mainParams;


//
// Testnet
//

class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0xf6;
        pchMessageStart[1] = 0xc2;
        pchMessageStart[2] = 0xb0;
        pchMessageStart[3] = 0xcb;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 16);
        vAlertPubKey = ParseHex("04d1c5e86e120a1db34ab9013b77659cca1e0b3f8c435fa5b679b2133d6ab22c1c7da922a026f409862a3cc0dd9f51153c1a0d8e7927dc34b09501bd2d0bbfa217");
        nDefaultPort = 26178;
        nRPCPort = 26174;

        strDataDir = "testnet";
        // Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nBits  = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce = 4509945;
        genesis.nTime    = 1493909211;
  
        hashGenesisBlock = genesis.GetHash();
        
        //MineGenesis(genesis);
        assert(hashGenesisBlock == uint256("0x00000089c09ef97712b7bde5994e6eb1c6f237cc119a86b77b92c28ef411e2f2"));

        vFixedSeeds.clear();
        vSeeds.clear();

        vSeeds.push_back(CDNSSeedData("Seednode1", "dnotesdevlinux1.southcentralus.cloudapp.azure.com"));
        vSeeds.push_back(CDNSSeedData("Seednode1", "dnotesdevlinux4.southcentralus.cloudapp.azure.com"));
        
        //vSeeds.push_back(CDNSSeedData("stratisplatform.com", "testnode1.stratisplatform.com"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 65); // stratis test net start with T
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 196);
        base58Prefixes[SECRET_KEY]     = std::vector<unsigned char>(1, 65 + 128);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        nLastPOWBlock = 0x7fffffff;// max signed int value
        //nLastPOWBlock = 300;
        nCRISPPayoutInterval = 60;
        nCRISPPayoutLag = 6;

        //for testing
        nMaxAddressBalancesPerBlock = 10;
        nMaxCoinBaseOutputsPerBlock = 10;
    }
    virtual Network NetworkID() const { return CChainParams::TESTNET; }
};
static CTestNetParams testNetParams;


//
// Regression test
//
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        pchMessageStart[0] = 0xf7;
        pchMessageStart[1] = 0xc3;
        pchMessageStart[2] = 0xb2;
        pchMessageStart[3] = 0xcc;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 1);
        genesis.nTime = 1411111111;
        genesis.nBits  = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce = 3349636;
        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 18444;
        strDataDir = "regtest";
        
        //MineGenesis(genesis);
        assert(hashGenesisBlock == uint256("0x00000ee61e8f9ce29956ecc49e8e9543c0c02262f5ae631e5d86603b9f3277f1"));

        vSeeds.clear();  // Regtest mode doesn't have any DNS seeds.

        nCRISPPayoutInterval = 100;
        nCRISPPayoutLag = 25;
        fPOWNoRetargeting = true;

        //for testing
        nMaxAddressBalancesPerBlock = 5;
        nMaxCoinBaseOutputsPerBlock = 5;
    }

    virtual bool RequireRPCPassword() const { return false; }
    virtual Network NetworkID() const { return CChainParams::REGTEST; }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = &mainParams;

const CChainParams &Params() {
    return *pCurrentParams;
}

void SelectParams(CChainParams::Network network) {
    switch (network) {
        case CChainParams::MAIN:
            pCurrentParams = &mainParams;
            break;
        case CChainParams::TESTNET:
            pCurrentParams = &testNetParams;
            break;
        case CChainParams::REGTEST:
            pCurrentParams = &regTestParams;
            break;
        default:
            assert(false && "Unimplemented network");
            return;
    }
}

bool SelectParamsFromCommandLine() {
    bool fRegTest = GetBoolArg("-regtest", false);
    bool fTestNet = GetBoolArg("-testnet", false);

    if (fTestNet && fRegTest) {
        return false;
    }

    if (fRegTest) {
        SelectParams(CChainParams::REGTEST);
    } else if (fTestNet) {
        SelectParams(CChainParams::TESTNET);
    } else {
        SelectParams(CChainParams::MAIN);
    }
    return true;
}
