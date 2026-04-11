#pragma once

enum BeatArguments
{
    eBeatArgument_txnIn_NONE = -1,
    eBeatArgument_txnIn_Default = 0,
    eBeatArgument_txnIn_ReferTo = 1,
    eBeatArgument_txnIn_Redirect = 2,
    eBeatArgument_txnIn_InTripletAffinitySwitch = 3,
    eBeatArgument_txnOut_T2ToT3Transition = 4,
};