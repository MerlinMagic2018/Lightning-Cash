// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <pow.h>

#include <arith_uint256.h>
#include <chain.h>
#include <primitives/block.h>
#include <uint256.h>
#include <util.h>
#include <core_io.h>            // LightningCash Gold: Hive
#include <script/standard.h>    // LightningCash Gold: Hive
#include <base58.h>             // LightningCash Gold: Hive
#include <pubkey.h>             // LightningCash Gold: Hive
#include <hash.h>               // LightningCash Gold: Hive
#include <sync.h>               // LightningCash Gold: Hive
#include <validation.h>         // LightningCash Gold: Hive
#include <utilstrencodings.h>   // LightningCash Gold: Hive

BeePopGraphPoint beePopGraph[1024*40];       // LightningCash Gold: Hive

CAmount totalMatureBees;
//int deadmatureBees;
int multicount;
int toti;
int tata;
int calisse;
int pet;
int thematurebees;
int deadBees;
//int zaza;
//int zozo;
//int zazaza;
//int zozozo;

// LightningCash Gold: DarkGravity V3 (https://github.com/dashpay/dash/blob/master/src/pow.cpp#L82)
// By Evan Duffield <evan@dash.org>
unsigned int DarkGravityWave(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);   // LightningCash: Note we use the Scrypt pow limit here!
    int64_t nPastBlocks = 24;

    // LightningCash Gold: Allow minimum difficulty blocks if we haven't seen a block for ostensibly 10 blocks worth of time
    if (params.fPowAllowMinDifficultyBlocks && pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacing * 10)
        return bnPowLimit.GetCompact();

    // LightningCash Gold: Make sure we have at least (nPastBlocks + 1) blocks since the fork, otherwise just return powLimitSHA
    if (!pindexLast || pindexLast->nHeight - params.lastScryptBlock < nPastBlocks)
        return bnPowLimit.GetCompact();

    const CBlockIndex *pindex = pindexLast;
    arith_uint256 bnPastTargetAvg;

    for (unsigned int nCountBlocks = 1; nCountBlocks <= nPastBlocks; nCountBlocks++) {
        // LightningCash Gold: Hive: Skip over Hivemined blocks; we only want to consider PoW blocks
        while (pindex->GetBlockHeader().IsHiveMined(params)) {
            //LogPrintf("DarkGravityWave: Skipping hivemined block at %i\n", pindex->nHeight);
            assert(pindex->pprev); // should never fail
            pindex = pindex->pprev;
        }

        arith_uint256 bnTarget = arith_uint256().SetCompact(pindex->nBits);
        if (nCountBlocks == 1) {
            bnPastTargetAvg = bnTarget;
        } else {
            // NOTE: that's not an average really...
            bnPastTargetAvg = (bnPastTargetAvg * nCountBlocks + bnTarget) / (nCountBlocks + 1);
        }

        if(nCountBlocks != nPastBlocks) {
            assert(pindex->pprev); // should never fail
            pindex = pindex->pprev;
        }
    }

    arith_uint256 bnNew(bnPastTargetAvg);

    int64_t nActualTimespan = pindexLast->GetBlockTime() - pindex->GetBlockTime();
    // NOTE: is this accurate? nActualTimespan counts it for (nPastBlocks - 1) blocks only...
    int64_t nTargetTimespan = nPastBlocks * params.nPowTargetSpacing;

    if (nActualTimespan < nTargetTimespan/3)
        nActualTimespan = nTargetTimespan/3;
    if (nActualTimespan > nTargetTimespan*3)
        nActualTimespan = nTargetTimespan*3;

    // Retarget
    bnNew *= nActualTimespan;
    bnNew /= nTargetTimespan;

    // LightningCash Gold : Limit "High Hash" Attacks... Progressively lower mining difficulty if too high...
    if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacing * 30){
	//LogPrintf("DarkGravityWave: 30 minutes without a block !! Resetting difficulty ! OLD Target = %s\n", bnNew.ToString());
	bnNew = bnPowLimit;
	//LogPrintf("DarkGravityWave: 30 minutes without a block !! Resetting difficulty ! NEW Target = %s\n", bnNew.ToString());
    }

    else if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacing * 25){
	//LogPrintf("DarkGravityWave: 25 minutes without a block. OLD Target = %s\n", bnNew.ToString());
	bnNew *= 100000;
	//LogPrintf("DarkGravityWave: 25 minutes without a block. NEW Target = %s\n", bnNew.ToString());
    }

    else if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacing * 20){
	//LogPrintf("DarkGravityWave: 20 minutes without a block. OLD Target = %s\n", bnNew.ToString());
	bnNew *= 10000;
	//LogPrintf("DarkGravityWave: 20 minutes without a block. NEW Target = %s\n", bnNew.ToString());
    }

    else if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacing * 15){
	//LogPrintf("DarkGravityWave: 15 minutes without a block. OLD Target = %s\n", bnNew.ToString());
	bnNew *= 1000;
	//LogPrintf("DarkGravityWave: 15 minutes without a block. NEW Target = %s\n", bnNew.ToString());
    }

    else if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacing * 10){
	//LogPrintf("DarkGravityWave: 10 minutes without a block. OLD Target = %s\n", bnNew.ToString());
	bnNew *= 100;
	//LogPrintf("DarkGravityWave: 10 minutes without a block. NEW Target = %s\n", bnNew.ToString());
    }

    else {
	bnNew = bnNew;
	//LogPrintf("DarkGravityWave: no stale tip over 10m detected yet so target = %s\n", bnNew.ToString());
    }

    if (bnNew > bnPowLimit) {
        bnNew = bnPowLimit;
	//LogPrintf("DarkGravityWave: target too low, so target is minimum which is = %s\n", bnNew.ToString());
    }

    return bnNew.GetCompact();
}

unsigned int GetNextWorkRequiredLTC(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    assert(pindexLast != nullptr);
    unsigned int nProofOfWorkLimit = UintToArith256(params.powLimit).GetCompact();

    // Only change once per difficulty adjustment interval
    if ((pindexLast->nHeight+1) % params.DifficultyAdjustmentInterval() != 0)
    {
        if (params.fPowAllowMinDifficultyBlocks)
        {
            // Special difficulty rule for testnet:
            // If the new block's timestamp is more than 2* 10 minutes
            // then allow mining of a min-difficulty block.
            if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacing*2)
                return nProofOfWorkLimit;
            else
            {
                // Return the last non-special-min-difficulty-rules-block
                const CBlockIndex* pindex = pindexLast;
                while (pindex->pprev && pindex->nHeight % params.DifficultyAdjustmentInterval() != 0 && pindex->nBits == nProofOfWorkLimit)
                    pindex = pindex->pprev;
                return pindex->nBits;
            }
        }
        return pindexLast->nBits;
    }

    // Go back by what we want to be 14 days worth of blocks
    // LightningCash Gold: This fixes an issue where a 51% attack can change difficulty at will.
    // Go back the full period unless it's the first retarget after genesis. Code courtesy of Art Forz
    int blockstogoback = params.DifficultyAdjustmentInterval()-1;
    if ((pindexLast->nHeight+1) != params.DifficultyAdjustmentInterval())
        blockstogoback = params.DifficultyAdjustmentInterval();

    // Go back by what we want to be 14 days worth of blocks
    const CBlockIndex* pindexFirst = pindexLast;
    for (int i = 0; pindexFirst && i < blockstogoback; i++)
        pindexFirst = pindexFirst->pprev;

    assert(pindexFirst);

    return CalculateNextWorkRequired(pindexLast, pindexFirst->GetBlockTime(), params);
}

unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    assert(pindexLast != nullptr);

    // LitecoinCash: If past fork time, use Dark Gravity Wave
    if (pindexLast->nHeight >= params.lastScryptBlock)
        return DarkGravityWave(pindexLast, pblock, params);
    else
        return GetNextWorkRequiredLTC(pindexLast, pblock, params);
}



unsigned int CalculateNextWorkRequired(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params& params)
{
    if (params.fPowNoRetargeting)
        return pindexLast->nBits;

    // Limit adjustment step
    int64_t nActualTimespan = pindexLast->GetBlockTime() - nFirstBlockTime;
    if (nActualTimespan < params.nPowTargetTimespan/4)
        nActualTimespan = params.nPowTargetTimespan/4;
    if (nActualTimespan > params.nPowTargetTimespan*4)
        nActualTimespan = params.nPowTargetTimespan*4;

    // Retarget
    arith_uint256 bnNew;
    arith_uint256 bnOld;
    bnNew.SetCompact(pindexLast->nBits);
    bnOld = bnNew;
    // LightningCash Gold: intermediate uint256 can overflow by 1 bit
    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
    bool fShift = bnNew.bits() > bnPowLimit.bits() - 1;
    if (fShift)
        bnNew >>= 1;
    bnNew *= nActualTimespan;
    bnNew /= params.nPowTargetTimespan;
    if (fShift)
        bnNew <<= 1;

    if (bnNew > bnPowLimit)
        bnNew = bnPowLimit;

    return bnNew.GetCompact();
}

bool CheckProofOfWork(uint256 hash, unsigned int nBits, const Consensus::Params& params)
{
    bool fNegative;
    bool fOverflow;
    arith_uint256 bnTarget;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(params.powLimit))
        return false;

    // Check proof of work matches claimed amount
    if (UintToArith256(hash) > bnTarget)
        return false;

    return true;
}

// LightningCash Gold: Hive: Get the current Bee Hash Target
unsigned int GetNextHiveWorkRequired(const CBlockIndex* pindexLast, const Consensus::Params& params) {
    const arith_uint256 bnPowLimit = UintToArith256(params.powLimitHive);
    const arith_uint256 bnImpossible = 0;
    arith_uint256 beeHashTarget;

    //LogPrintf("GetNextHiveWorkRequired: Height     = %i\n", pindexLast->nHeight);

    int numPowBlocks = 0;
    CBlockHeader block;
    while (true) {
        if (!pindexLast->pprev || pindexLast->nHeight < params.minHiveCheckBlock) {   // Ran out of blocks without finding a Hive block? Return min target
            LogPrintf("GetNextHiveWorkRequired: No hivemined blocks found in history\n");
            //LogPrintf("GetNextHiveWorkRequired: This target= %s\n", bnPowLimit.ToString());
            return bnPowLimit.GetCompact();
        }

        block = pindexLast->GetBlockHeader();
        if (block.IsHiveMined(params)) {  // Found the last Hive block; pick up its bee hash target
            beeHashTarget.SetCompact(block.nBits);
            break;
        }

        pindexLast = pindexLast->pprev;
        numPowBlocks++;
    }

    //LogPrintf("GetNextHiveWorkRequired: powBlocks  = %i\n", numPowBlocks);
    if (numPowBlocks == 0)
        return bnImpossible.GetCompact();

    //LogPrintf("GetNextHiveWorkRequired: Last target= %s\n", beeHashTarget.ToString());

	// Apply EMA
	int interval = params.hiveTargetAdjustAggression / params.hiveBlockSpacingTarget;
	beeHashTarget *= (interval - 1) * params.hiveBlockSpacingTarget + numPowBlocks + numPowBlocks;
	beeHashTarget /= (interval + 1) * params.hiveBlockSpacingTarget;

	// Clamp to min difficulty
	if (beeHashTarget > bnPowLimit)
		beeHashTarget = bnPowLimit;

    LogPrintf("GetNextHiveWorkRequired: This target= %s\n", beeHashTarget.ToString());

    return beeHashTarget.GetCompact();
}


// LightningCash Gold: Hive: Get count of all live and gestating BCTs on the network
bool GetNetworkHiveInfo(int& immatureBees, int& immatureBCTs, int& matureBees, int& matureBCTs, CAmount& potentialLifespanRewards, const Consensus::Params& consensusParams, bool recalcGraph) {
    int totalBeeLifespan = consensusParams.beeLifespanBlocks + consensusParams.beeGestationBlocks;
    int bordel = consensusParams.beeGestationBlocks;
    
    immatureBees = immatureBCTs = matureBees = matureBCTs = totalMatureBees = deadBees = thematurebees = 0;
    
    //tata = 1551819029;
    //tata = ((chainActive.Genesis())->GetBlockTime());
    CBlockIndex* pindexStart = chainActive.Genesis();
    CBlockIndex* pindexPrev = chainActive.Genesis(); // doit compter en ordre croissant, donc part au bloc genese
//    CBlockIndex* pindexCaca = chainActive.Genesis(); // sert a rien...
//    int patatesale = ((chainActive.Back(pindexPrev))->nTime); // 360 Blocs avant celui qu'on check...
    CBlockIndex* pindexLast = chainActive.Tip();
    CBlockIndex* pindexCoco = chainActive.Genesis();
    
//    int patatesale = pindexCoco->nTime; // 360 Blocs avant celui qu'on check...
    assert(pindexPrev != nullptr);

    int tipHeight = pindexLast->nHeight; // Last mined block height

    int testing = pindexStart->nHeight; // start from Block 0    

    potentialLifespanRewards = (consensusParams.beeLifespanBlocks * GetBlockSubsidy(pindexLast->nHeight, consensusParams)) / consensusParams.hiveBlockSpacingTarget;

    if (recalcGraph) {
        for (int i = 0; i < totalBeeLifespan; i++) {
            beePopGraph[i].immaturePop = 0;
            beePopGraph[i].maturePop = 0;
	    totalMatureBees = 0;
        }
    }

    if (IsInitialBlockDownload())   // Refuse if we're downloading
        return false;



    // Count bees in next blockCount blocks
    CBlock block;
    CScript scriptPubKeyBCF = GetScriptForDestination(DecodeDestination(consensusParams.beeCreationAddress));
    CScript scriptPubKeyCF = GetScriptForDestination(DecodeDestination(consensusParams.hiveCommunityAddress));



    for (int i = testing; i < tipHeight; i++) {    // lets reverse that !!

	int currentCheckedHeight = pindexPrev->nHeight;
        if ((fHavePruned && !(pindexPrev->nStatus & BLOCK_HAVE_DATA) && pindexPrev->nTx > 0) || ((i > (totalBeeLifespan + 125)) && (fHavePruned && !(pindexCoco->nStatus & BLOCK_HAVE_DATA) && pindexCoco->nTx > 0))) {
            LogPrintf("! GetNetworkHiveInfo: Warn: Block not available (pruned data); can't calculate network bee count.");
            return false;
        }

        if ((!pindexPrev->GetBlockHeader().IsHiveMined(consensusParams))) {  // Don't check Hivemined blocks (no BCTs will be found in them)
            if (!ReadBlockFromDisk(block, pindexPrev, consensusParams)) {
                LogPrintf("! GetNetworkHiveInfo: Warn: Block not available (not found on disk); can't calculate network bee count.");
                return false;
            }
            /*if ((!ReadBlockFromDisk(block, pindexCoco, consensusParams)) && (i >= 360)) {
                LogPrintf("! GetNetworkHiveInfo: Warn: Block not available (not found on disk); can't calculate network bee count.");
                return false;
            }*/ // Useless ??

            int blockHeight = pindexPrev->nHeight; 
	    
            if (block.vtx.size() > 0) {
                for(const auto& tx : block.vtx) {
                    CAmount beeFeePaid;          
                    if (tx->IsBCT(consensusParams, scriptPubKeyBCF, &beeFeePaid)) {                 // If it's a BCT, total its bees
                        if (tx->vout.size() > 1 && tx->vout[1].scriptPubKey == scriptPubKeyCF) {    // If it has a community fund contrib...
                            CAmount donationAmount = tx->vout[1].nValue;
                            CAmount expectedDonationAmount = (beeFeePaid + donationAmount) / consensusParams.communityContribFactor;  // ...check for valid donation amount
                            if (donationAmount != expectedDonationAmount)
                                continue;
                            beeFeePaid += donationAmount;                                           // Add donation amount back to total paid
                        }
			
			int maudit;
                        int maudit2; // time of creation for dead bees...
                        int maudit3;
			
			CAmount beeCost; // PROBLEM ---> RESOLVED !!!!
                        CAmount beeCost2; // PROBLEM ---> RESOLVED !!!!!
                        CAmount beeCost3;
		
			toti = calisse;	// This is to remember switch times for each loop...
                        //zaza = zazaza;
                        if (!tata)
                            tata = ((chainActive.Genesis())->GetBlockTime());
                        else    
                            tata = pet; // This is to remember switch times for each loop...
                        //zozo = zozozo;
                        
                        int beeCount;
                        int beeCount2;
                        int beeCount3;
			
                        
                            
			
			if ((tipHeight) < (i + bordel)) { // immature bees count here
                            
                            maudit = pindexPrev->GetBlockTime();
                            
                            if (((maudit > tata) && (tata > toti)) || ((toti > tata) && ((maudit > tata) && (maudit < toti))) || (!(toti))){ // toti can be known before we check beeCost here, so must check if TX is after tata AND before toti !!!
				beeCost = 0.0004*(GetBlockSubsidy(pindexPrev->nHeight, consensusParams));
				
                            }
                            else {
				beeCost = 0.0008*(GetBlockSubsidy(pindexPrev->nHeight, consensusParams));
                            }
                            
                            beeCount = beeFeePaid / beeCost;
                            
                            immatureBees += beeCount;
                            //LogPrintf("immatureBees = %i \n", immatureBees);
                            immatureBCTs++;
                            if (beeCount > 0) {
                                LogPrintf("                                 \n");
                                LogPrintf(" %i ----> le temps du BLOC qu'on check en ce moment \n", maudit);
                                LogPrintf(" Immature \n");
                                LogPrintf(" BeeCost : %i \n", beeCost);
                                LogPrintf("beeCount in pow.cpp  = + %i \n", beeCount);
                                LogPrintf("Height = %i ----> la hauteur du BLOC qu'on check en ce moment \n", currentCheckedHeight);
                            }
                        }

                        if ((tipHeight) >= (i + bordel)) { // mature bees count here
                            
                            
                            maudit3 = (pindexPrev->GetBlockTime());
                            
                            
                            if (((maudit3 > tata) && (tata > toti)) || ((toti > tata) && ((maudit3 > tata) && (maudit3 < toti))) || (!(toti))){ // toti can be known before we check beeCost here, so must check if TX is after tata AND before toti !!!
				beeCost3 = 0.0004*(GetBlockSubsidy(pindexPrev->nHeight, consensusParams));
				
                            }
                            else {
				beeCost3 = 0.0008*(GetBlockSubsidy(pindexPrev->nHeight, consensusParams));
                            }
                            
                            beeCount3 = beeFeePaid / beeCost3;
                            
                            matureBees += beeCount3;
			    //LogPrintf("matureBees = %i \n", matureBees);	

			    totalMatureBees = matureBees;
			    thematurebees = matureBees;
                            matureBCTs++;
                            if (beeCount3 > 0) {
                                LogPrintf("                                 \n");
                                LogPrintf(" %i ----> le temps du BLOC qu'on check en ce moment  \n", maudit3);
                                LogPrintf(" Mature, excluding EXPIRED... \n");
                                LogPrintf(" BeeCost : %i \n", beeCost3);
                                LogPrintf("beeCount in pow.cpp  = + %i \n", beeCount3);
                                LogPrintf("Height = %i ----> la hauteur du BLOC qu'on check en ce moment  \n", currentCheckedHeight);
                            }
                        }
                        //for (int k = (totalBeeLifespan + 125); k < tipHeight; k++) {
                        
                            //if (((tipHeight) >= (i + totalBeeLifespan)) && (i > (totalBeeLifespan + 125))) {
                        if (i > (totalBeeLifespan + 125)) { // dead bees count here
                            LogPrintf(" %i ----> La hauteur quon commence a checker pour les abeilles mortes  \n", currentCheckedHeight);
                            maudit2 = (chainActive.Back(pindexCoco))->GetBlockTime(); // Time of bloc when this dying BCT was created !!
                            LogPrintf(" %i ----> The block TIME that deadbees were created...  \n", maudit2);
                            //maudit2 = patatesale;

                            if (((maudit2 > tata) && (tata > toti)) || ((toti > tata) && ((maudit2 > tata) && (maudit2 <= toti))) || (!(toti))){
                                    beeCost2 = 0.0004*(GetBlockSubsidy(pindexCoco->nHeight, consensusParams));

                            }
                            else {
                                    beeCost2 = 0.0008*(GetBlockSubsidy(pindexCoco->nHeight, consensusParams));
                            }


                            int beeCount2 = beeFeePaid / beeCost2; // PROBLEM
                            deadBees += beeCount2;

                            if (beeCount2 > 0) {
                                    LogPrintf("                                 \n");
                                    LogPrintf(" %i ----> time of creation   \n", maudit2);
                                    LogPrintf(" Expired \n");
                                    LogPrintf(" BeeCost : %i \n", beeCost2);
                                    LogPrintf("beeCount in pow.cpp  = - %i \n", beeCount2);
                                    LogPrintf("Height = %i ----> la hauteur du BLOC qu'on check en ce moment, et non celui 360 BLOCs avant...  \n", currentCheckedHeight);
                            }

                        }
                        //}

                        // Add these bees to pop graph
                        if (recalcGraph) {
                            /*
                            int beeStart = blockHeight + consensusParams.beeGestationBlocks;
                            int beeStop = beeStart + consensusParams.beeLifespanBlocks;
                            beeStart -= tipHeight;
                            beeStop -= tipHeight;
                            for (int j = beeStart; j < beeStop; j++) {
                                if (j > 0 && j < totalBeeLifespan) {
                                    if (i < consensusParams.beeGestationBlocks) // THIS IS WRONG
                                        beePopGraph[j].immaturePop += beeCount;
                                    else
                                        beePopGraph[j].maturePop += beeCount;
                                }           int beeDiesBlockX = blockHeight + consensusParams.beeGestationBlocks + consensusParams.beeLifespanBlocks;
                            }*/
                            int beeBornBlock = blockHeight;
                            int beeMaturesBlock = beeBornBlock + consensusParams.beeGestationBlocks;
                            int beeDiesBlock = beeMaturesBlock + consensusParams.beeLifespanBlocks;
                            for (int j = beeBornBlock; j < beeDiesBlock; j++) {
                                int graphPos = j - tipHeight;
                                if (graphPos > 0 && graphPos < totalBeeLifespan) {
                                    if (j < beeMaturesBlock)
                                        beePopGraph[graphPos].immaturePop += beeCount;
                                    else
                                        beePopGraph[graphPos].maturePop += beeCount;
					//LogPrintf("maturePop in recalcgraph... = %i\n", beePopGraph[graphPos].maturePop);
					//LogPrintf("beeCount at the end of loop in pow.cpp  = %i \n", beeCount);  
                                }
                            }
                        }
                    }
                }
            }
        }
                            // check switch time to know beeCost for every BCT !!
                            int coucou = (matureBees - deadBees);
 
			    if (((coucou) > 378000) && (!(multicount % 2))){ // gets over 90 and multicount is pair
				multicount++;

				toti = (chainActive.Nono(pindexPrev))->GetBlockTime(); // Time of bloc when this BCT will mature
                                //zaza = ((pindexPrev->nHeight) + 24);
				calisse = toti;
                                //zazaza = zaza;

                                int switchHeight = ((pindexPrev->nHeight) + 24); // height when this switching BCT will matures
				LogPrintf(" %i %i SWITCH ------------------------------------------- ( TOTI ) = %i \n", coucou, switchHeight, toti);

			    }
                            int volvo = pindexPrev->GetBlockTime();

			    if  (((coucou) <= 378000) && (multicount % 2)){
				tata = pindexPrev->GetBlockTime(); // Time of block when bees DIES ( no "maturity time" for death... ) lol
                                

				multicount++;
                                
                                pet = tata;

                                int switchHeight2 = pindexPrev->nHeight;
				LogPrintf(" %i %i SWITCH ------------------------------------------- ( tata ) = %i \n", coucou, switchHeight2, tata);

				
			    }
             
            
            

	
	

        if ((!chainActive.Next(pindexPrev)) || (!chainActive.Next(pindexCoco)))    // Check we didn't run out of blocks
            return true;

        pindexPrev = chainActive.Next(pindexPrev);
        pindexCoco = chainActive.Next(pindexCoco);
    }

    return true;
}

// LightningCash Gold: Hive: Check the hive proof for given block
bool CheckHiveProof(const CBlock* pblock, const Consensus::Params& consensusParams) {
    bool verbose = LogAcceptCategory(BCLog::HIVE);

    if (verbose)
        LogPrintf("********************* Hive: CheckHiveProof *********************\n");

    // Get height (a CBlockIndex isn't always available when this func is called, eg in reads from disk)
    int blockHeight;
    CBlockIndex* pindexPrev;
    {
        LOCK(cs_main);
        pindexPrev = mapBlockIndex[pblock->hashPrevBlock];
        blockHeight = pindexPrev->nHeight + 1;
    }
    if (!pindexPrev) {
        LogPrintf("CheckHiveProof: Couldn't get previous block's CBlockIndex!\n");
        return false;
    }
    if (verbose)
        LogPrintf("CheckHiveProof: nHeight             = %i\n", blockHeight);

    // Check hive is enabled on network
    if (!IsHiveEnabled(pindexPrev, consensusParams)) {
        LogPrintf("CheckHiveProof: Can't accept a Hive block; Hive is not yet enabled on the network.\n");
        return false;
    }

    // Check previous block wasn't hivemined
    if (pindexPrev->GetBlockHeader().IsHiveMined(consensusParams)) {
        LogPrintf("CheckHiveProof: Hive block must follow a POW block.\n");
        return false;
    }

    // Block mustn't include any BCTs
    CScript scriptPubKeyBCF = GetScriptForDestination(DecodeDestination(consensusParams.beeCreationAddress));
    if (pblock->vtx.size() > 1)
        for (unsigned int i=1; i < pblock->vtx.size(); i++)
            if (pblock->vtx[i]->IsBCT(consensusParams, scriptPubKeyBCF)) {
                LogPrintf("CheckHiveProof: Hivemined block contains BCTs!\n");
                return false;                
            }
    
    // Coinbase tx must be valid
    CTransactionRef txCoinbase = pblock->vtx[0];
    //LogPrintf("CheckHiveProof: Got coinbase tx: %s\n", txCoinbase->ToString());
    if (!txCoinbase->IsCoinBase()) {
        LogPrintf("CheckHiveProof: Coinbase tx isn't valid!\n");
        return false;
    }

    // Must have exactly 2 or 3 outputs
    if (txCoinbase->vout.size() < 2 || txCoinbase->vout.size() > 3) {
        LogPrintf("CheckHiveProof: Didn't expect %i vouts!\n", txCoinbase->vout.size());
        return false;
    }

    // vout[0] must be long enough to contain all encodings
    if (txCoinbase->vout[0].scriptPubKey.size() < 144) {
        LogPrintf("CheckHiveProof: vout[0].scriptPubKey isn't long enough to contain hive proof encodings\n");
        return false;
    }

    // vout[1] must start OP_RETURN OP_BEE (bytes 0-1)
    if (txCoinbase->vout[0].scriptPubKey[0] != OP_RETURN || txCoinbase->vout[0].scriptPubKey[1] != OP_BEE) {
        LogPrintf("CheckHiveProof: vout[0].scriptPubKey doesn't start OP_RETURN OP_BEE\n");
        return false;
    }

    // Grab the bee nonce (bytes 3-6; byte 2 has value 04 as a size marker for this field)
    uint32_t beeNonce = ReadLE32(&txCoinbase->vout[0].scriptPubKey[3]);
    if (verbose)
        LogPrintf("CheckHiveProof: beeNonce            = %i\n", beeNonce);

    // Grab the bct height (bytes 8-11; byte 7 has value 04 as a size marker for this field)
    uint32_t bctClaimedHeight = ReadLE32(&txCoinbase->vout[0].scriptPubKey[8]);
    if (verbose)
        LogPrintf("CheckHiveProof: bctHeight           = %i\n", bctClaimedHeight);

    // Get community contrib flag (byte 12)
    bool communityContrib = txCoinbase->vout[0].scriptPubKey[12] == OP_TRUE;
    if (verbose)
        LogPrintf("CheckHiveProof: communityContrib    = %s\n", communityContrib ? "true" : "false");

    // Grab the txid (bytes 14-78; byte 13 has val 64 as size marker)
    std::vector<unsigned char> txid(&txCoinbase->vout[0].scriptPubKey[14], &txCoinbase->vout[0].scriptPubKey[14 + 64]);
    std::string txidStr = std::string(txid.begin(), txid.end());
    if (verbose)
        LogPrintf("CheckHiveProof: bctTxId             = %s\n", txidStr);

    // Check bee hash against target
    std::string deterministicRandString = GetDeterministicRandString(pindexPrev);
    if (verbose)
        LogPrintf("CheckHiveProof: detRandString       = %s\n", deterministicRandString);
    arith_uint256 beeHashTarget;
    beeHashTarget.SetCompact(GetNextHiveWorkRequired(pindexPrev, consensusParams));
    if (verbose)
        LogPrintf("CheckHiveProof: beeHashTarget       = %s\n", beeHashTarget.ToString());
    std::string hashHex = (CHashWriter(SER_GETHASH, 0) << deterministicRandString << txidStr << beeNonce).GetHash().GetHex();
    arith_uint256 beeHash = arith_uint256(hashHex);
    if (verbose)
        LogPrintf("CheckHiveProof: beeHash             = %s\n", hashHex);
    if (beeHash >= beeHashTarget) {
        LogPrintf("CheckHiveProof: Bee does not meet hash target!\n");
        return false;
    }

    // Grab the message sig (bytes 79-end; byte 78 is size)
    std::vector<unsigned char> messageSig(&txCoinbase->vout[0].scriptPubKey[79], &txCoinbase->vout[0].scriptPubKey[79 + 65]);
    if (verbose)
        LogPrintf("CheckHiveProof: messageSig          = %s\n", HexStr(&messageSig[0], &messageSig[messageSig.size()]));
    
    // Grab the honey address from the honey vout
    CTxDestination honeyDestination;
    if (!ExtractDestination(txCoinbase->vout[1].scriptPubKey, honeyDestination)) {
        LogPrintf("CheckHiveProof: Couldn't extract honey address\n");
        return false;
    }
    if (!IsValidDestination(honeyDestination)) {
        LogPrintf("CheckHiveProof: Honey address is invalid\n");
        return false;
    }
    if (verbose)
        LogPrintf("CheckHiveProof: honeyAddress        = %s\n", EncodeDestination(honeyDestination));

    // Verify the message sig
    const CKeyID *keyID = boost::get<CKeyID>(&honeyDestination);
    if (!keyID) {
        LogPrintf("CheckHiveProof: Can't get pubkey for honey address\n");
        return false;
    }
    CHashWriter ss(SER_GETHASH, 0);
    ss << deterministicRandString;
    uint256 mhash = ss.GetHash();
    CPubKey pubkey;
    if (!pubkey.RecoverCompact(mhash, messageSig)) {
        LogPrintf("CheckHiveProof: Couldn't recover pubkey from hash\n");
        return false;
    }
    if (pubkey.GetID() != *keyID) {
        LogPrintf("CheckHiveProof: Signature mismatch! GetID() = %s, *keyID = %s\n", pubkey.GetID().ToString(), (*keyID).ToString());
        return false;
    }

    // Grab the BCT utxo
    bool deepDrill = false;
    uint32_t bctFoundHeight;
    CAmount bctValue;
    CScript bctScriptPubKey;
    {
        LOCK(cs_main);

        COutPoint outBeeCreation(uint256S(txidStr), 0);
        COutPoint outCommFund(uint256S(txidStr), 1);
        Coin coin;
        CTransactionRef bct = nullptr;
        CBlockIndex foundAt;

        if (pcoinsTip && pcoinsTip->GetCoin(outBeeCreation, coin)) {        // First try the UTXO set (this pathway will hit on incoming blocks)
            if (verbose)
                LogPrintf("CheckHiveProof: Using UTXO set for outBeeCreation\n");
            bctValue = coin.out.nValue;
            bctScriptPubKey = coin.out.scriptPubKey;
            bctFoundHeight = coin.nHeight;
        } else {                                                            // UTXO set isn't available when eg reindexing, so drill into block db (not too bad, since Alice put her BCT height in the coinbase tx)
            if (verbose)
                LogPrintf("! CheckHiveProof: Warn: Using deep drill for outBeeCreation\n");
            if (!GetTxByHashAndHeight(uint256S(txidStr), bctClaimedHeight, bct, foundAt, pindexPrev, consensusParams)) {
                LogPrintf("CheckHiveProof: Couldn't locate indicated BCT\n");
                return false;
            }
            deepDrill = true;
            bctFoundHeight = foundAt.nHeight;
            bctValue = bct->vout[0].nValue;
            bctScriptPubKey = bct->vout[0].scriptPubKey;
        }

        if (communityContrib) {
            CScript scriptPubKeyCF = GetScriptForDestination(DecodeDestination(consensusParams.hiveCommunityAddress));
            CAmount donationAmount;

            if(bct == nullptr) {                                                                // If we dont have a ref to the BCT
                if (pcoinsTip && pcoinsTip->GetCoin(outCommFund, coin)) {                       // First try UTXO set
                    if (verbose)
                        LogPrintf("CheckHiveProof: Using UTXO set for outCommFund\n");
                    if (coin.out.scriptPubKey != scriptPubKeyCF) {                              // If we find it, validate the scriptPubKey and store amount
                        LogPrintf("CheckHiveProof: Community contrib was indicated but not found\n");
                        return false;
                    }
                    donationAmount = coin.out.nValue;
                } else {                                                                        // Fallback if we couldn't use UTXO set
                    if (verbose)
                        LogPrintf("! CheckHiveProof: Warn: Using deep drill for outCommFund\n");
                    if (!GetTxByHashAndHeight(uint256S(txidStr), bctClaimedHeight, bct, foundAt, pindexPrev, consensusParams)) {
                        LogPrintf("CheckHiveProof: Couldn't locate indicated BCT\n");           // Still couldn't find it
                        return false;
                    }
                    deepDrill = true;
                }
            }
            if(bct != nullptr) {                                                                // We have the BCT either way now (either from first or second drill). If got from UTXO set bct == nullptr still.
                if (bct->vout.size() < 2 || bct->vout[1].scriptPubKey != scriptPubKeyCF) {      // So Validate the scriptPubKey and store amount
                    LogPrintf("CheckHiveProof: Community contrib was indicated but not found\n");
                    return false;
                }
                donationAmount = bct->vout[1].nValue;
            }

            // Check for valid donation amount
            CAmount expectedDonationAmount = (bctValue + donationAmount) / consensusParams.communityContribFactor;
            if (donationAmount != expectedDonationAmount) {
                LogPrintf("CheckHiveProof: BCT pays community fund incorrect amount %i (expected %i)\n", donationAmount, expectedDonationAmount);
                return false;
            }

            // Update amount paid
            bctValue += donationAmount;
        }
    }

    if (bctFoundHeight != bctClaimedHeight) {
        LogPrintf("CheckHiveProof: Claimed BCT height of %i conflicts with found height of %i\n", bctClaimedHeight, bctFoundHeight);
        return false;
    }

    // Check bee maturity
    int bctDepth = blockHeight - bctFoundHeight;
    if (bctDepth < consensusParams.beeGestationBlocks) {
        LogPrintf("CheckHiveProof: Indicated BCT is immature.\n");
        return false;
    }
    if (bctDepth > consensusParams.beeGestationBlocks + consensusParams.beeLifespanBlocks) {
        LogPrintf("CheckHiveProof: Indicated BCT is too old.\n");
        return false;
    }

    // Check for valid bee creation script and get honey scriptPubKey from BCT
    CScript scriptPubKeyHoney;
    if (!CScript::IsBCTScript(bctScriptPubKey, scriptPubKeyBCF, &scriptPubKeyHoney)) {
        LogPrintf("CheckHiveProof: Indicated utxo is not a valid BCT script\n");
        return false;
    }

    CTxDestination honeyDestinationBCT;
    if (!ExtractDestination(scriptPubKeyHoney, honeyDestinationBCT)) {
        LogPrintf("CheckHiveProof: Couldn't extract honey address from BCT UTXO\n");
        return false;
    }

    // Check BCT's honey address actually matches the claimed honey address
    if (honeyDestination != honeyDestinationBCT) {
        LogPrintf("CheckHiveProof: BCT's honey address does not match claimed honey address!\n");
        return false;
    }

    // Find bee count
    CAmount beeCost;
    CBlockIndex foundAt;
    int maudit3 = foundAt.GetBlockTime();
	
    if ((!(multicount % 2)) || ((multicount % 2) && (maudit3 <= toti))){ // if multicount is pair
	beeCost = 0.0004*(GetBlockSubsidy(pindexPrev->nHeight, consensusParams));
	//LogPrintf("beecost for Hive Proof = %d \n", beeCost);
    }
    else{                   // multicount is impair
	beeCost = 0.0008*(GetBlockSubsidy(pindexPrev->nHeight, consensusParams));
	//LogPrintf("beecost for Hive Proof = %d \n", beeCost);
    }
    LogPrintf("decided beeCost in checkHiveProof() (pow.cpp) = %d \n", beeCost);
    //CAmount beeCost = GetBeeCost(bctFoundHeight, consensusParams); // PROBLEM
    // CAmount beeCost = 0.0004*(GetBlockSubsidy(pindexPrev->nHeight, consensusParams));
    if (bctValue < consensusParams.minBeeCost) {
        LogPrintf("CheckHiveProof: BCT fee is less than the minimum possible bee cost\n");
        return false;
    }
    if (bctValue < beeCost) {
        LogPrintf("CheckHiveProof: BCT fee is less than the cost for a single bee\n");
        return false;
    }
    unsigned int beeCount = bctValue / beeCost;
    if (verbose) {
        LogPrintf("CheckHiveProof: bctValue            = %i\n", bctValue);
        LogPrintf("CheckHiveProof: beeCost             = %i\n", beeCost);
        LogPrintf("CheckHiveProof: beeCount            = %i\n", beeCount);
    }
    
    // Check enough bees were bought to include claimed beeNonce
    if (beeNonce >= beeCount) {
        LogPrintf("CheckHiveProof: BCT did not create enough bees for claimed nonce!\n");
        return false;
    }

    LogPrintf("CheckHiveProof: Pass at %i%s\n", blockHeight, deepDrill ? " (used deepdrill)" : "");
    return true;
}
