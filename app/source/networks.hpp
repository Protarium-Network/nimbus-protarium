#pragma once

#include <string>
#include "common.hpp"

// Where the release zip puts the per-network patch sets: /3ds/nimbus/variants/<nintendo|pretendo|protarium>/
#define NIMBUS_VARIANTS_PATH "/3ds/nimbus/variants/"
#define NIMBUS_ACTIVE_NETWORK_PATH "/3ds/nimbus/active-network.txt"

// Human readable ("Protarium") and directory/file ("protarium") names of a network
const char *networkName(Network network);
const char *networkDir(Network network);

// Pretendo and Protarium both run on the Pretendo friend account (slot 2) and the "test" NASC environment,
// so the console cannot tell them apart. The installed network is remembered in active-network.txt.
bool activeNetworkIsProtarium();

// Installs (or, for Nintendo, removes) the Luma IPS patches and CA of a network, then records it in
// active-network.txt. On failure returns false and fills networkPatchError with "<step>: <path> (err <errno>)".
bool installNetworkPatches(Network network);

extern std::string networkPatchError;
