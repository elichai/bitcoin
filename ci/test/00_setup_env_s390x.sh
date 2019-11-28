#!/usr/bin/env bash
#
# Copyright (c) 2019 The Bitcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

export LC_ALL=C.UTF-8

# The host arch is unknown, so we run the tests through qemu.
# If the host is s390x and wants to run the tests natively, it can set QEMU_USER_CMD to the empty string.
export QEMU_USER_CMD="${QEMU_USER_CMD:"qemu-s390x"}"
export PACKAGES="clang llvm python3-zmq qtbase5-dev qttools5-dev-tools libssl1.0-dev libevent-dev bsdmainutils libboost-system-dev libboost-filesystem-dev libboost-chrono-dev libboost-test-dev libboost-thread-dev libdb5.3++-dev libminiupnpc-dev libzmq3-dev libqrencode-dev"
export NO_DEPENDS=1
export RUN_UNIT_TESTS=true
export RUN_FUNCTIONAL_TESTS=true
export GOAL="install"
export BITCOIN_CONFIG="--enable-reduce-exports --with-incompatible-bdb"
