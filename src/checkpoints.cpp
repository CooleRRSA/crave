// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "txdb.h"
#include "main.h"
#include "uint256.h"


static const int nCheckpointSpan = 5000;

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    //
    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    //
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
    (     0, uint256("0x00000a336bf3e2be21c2ce9a3f9bc9849c697475d0de85e201bdc3452f3c343b") )
    ( 10000, uint256("0x1cbd7af1430109200bda6ca6a0b94b5771c28819518a201ef96eb0859afca006") )
    ( 20000, uint256("0x8ca1f8beca3f6fd966cc83495f367be55ef3460e63abdf692e22e279a47fbe71") )
    ( 32712, uint256("0x15682588a31e9064596fc45aa2403558dc62eed296e709e4f577beca1669e788") )
    ( 32713, uint256("0x474619e0a58ec88c8e2516f8232064881750e87acac3a416d65b99bd61246968") )
    ( 32720, uint256("0x9bef0d350eb6b11bc0e24c611e92e8e4b7806b1c66afae3d30581c7db89512e4") )
    ( 32750, uint256("0x4f3dd45d3de3737d60da46cff2d36df0002b97c505cdac6756d2d88561840b63") )
    ( 32800, uint256("0x274996cec47b3f3e6cd48c8f0b39c32310dd7ddc8328ae37762be956b9031024") )
    ( 32900, uint256("0xfcea99cbf85e66d340b9883fd4a22f3426dc5a27e35b9cbf936d9fae3378002c") )
    ( 33000, uint256("0x081cb47222d3e076d4035041547f3d91f8b35526b8087a4e360f1470a8d541da") )
    ( 40000, uint256("0x49adb986a856eca96e5af98026f39b30d98d80cacba37ee0bae0ddbf2dbefed9") )
    ( 50000, uint256("0xaf0b1d395f56afd34d28af1f606378caa5570f942a5746b8de39470e92a2aa52") )
    ( 60000, uint256("0xc847e756dbd4d7119106faa675ab3d900c75c1070c69bb91826ed90e89f75052") )
    ( 70000, uint256("0xf7ccc8cb7e5b4075e67c1bc0f8b74d165e0642847f9d3492822dbeda3657f8a2") )
    ( 80000, uint256("0xd4a98f840a72287ddaff2a57380ca625126b5a34982d5e7d6df085ad2d0ba5f3") )
    ( 98000, uint256("0x936bcf68c8c46dab4d1078c430e252ef6372ac419566cfcaa36bcc4a97254520") )
    (100000, uint256("0xfabdddd41bf7c5e1d30cec912b5d8ece6e6cf4a4af3d55308f27ef2e38bca789") )
    (200000, uint256("0x4331fd4e2ec75ee16a269934b643c86a00f4243ce35f74eded28026d30b49100") )
    (300000, uint256("0x7e5c62b6b983a8d03181af44666b19ef88cfec633634413a5dc05cdf9a4cc553") )
    (400000, uint256("0x1f81d66b9c644af4e99ffbfd64bce1c697a8682071b14545642972a37b8ca0a8") )
    (500000, uint256("0xa5e8bbbd1b6e49991e1af745e890896eb93a9afd86fe4661dad73b368981681f") )
    (600000, uint256("0x498c103556a7ff2f3cade8d0194c4d869ecdcf055710835bed64f6af01f1c689") )
    (700000, uint256("0xbef31c6a7cbf9ed67af047dae9cfa8c81616be4c33c76099e903cc2e157a659f") )
    
    ;

    // TestNet has no checkpoints
    static MapCheckpoints mapCheckpointsTestnet;

    bool CheckHardened(int nHeight, const uint256& hash)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
    }

    int GetTotalBlocksEstimate()
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        if (checkpoints.empty())
            return 0;
        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }

    // Automatically select a suitable sync-checkpoint 
    const CBlockIndex* AutoSelectSyncCheckpoint()
    {
        const CBlockIndex *pindex = pindexBest;
        // Search backward for a block within max span and maturity window
        while (pindex->pprev && pindex->nHeight + nCheckpointSpan > pindexBest->nHeight)
            pindex = pindex->pprev;
        return pindex;
    }

    // Check against synchronized checkpoint
    bool CheckSync(int nHeight)
    {
        const CBlockIndex* pindexSync = AutoSelectSyncCheckpoint();
        if (nHeight <= pindexSync->nHeight){
            return false;
        }
        return true;
    }
}
