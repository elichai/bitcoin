#!/usr/bin/env python3
# Copyright (c) 2018-2019 The Bitcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.
"""Test the Partially Signed Transaction RPCs.
"""

from test_framework.address import program_to_witness
from test_framework.key import ECKey, ECPubKey
from test_framework.script import (
    CScript,
    CScriptOp,
    OP_CHECKSIG,
    OP_DUP,
    OP_EQUALVERIFY,
    OP_HASH160,
    hash160,
)
from test_framework.script import taproot_construct
from test_framework.test_framework import BitcoinTestFramework
from test_framework.util import (
    assert_equal,
    find_output,
)
from test_framework.descriptors import descsum_create


# Create one-input, one-output, no-fee transaction:
class PSBTTest(BitcoinTestFramework):

    def set_test_params(self):
        self.setup_clean_chain = False
        self.num_nodes = 2

    def skip_test_if_missing_module(self):
        self.skip_if_no_wallet()

    def run_test(self):
        invalid_point = ECPubKey()
        invalid_point.hash_to_curve()
        invalid_point_hex = invalid_point.get_bytes().hex()
        sec1 = ECKey()
        sec1.generate()
        sec1_wif = sec1.get_wif()
        pub1 = sec1.get_pubkey()
        pub1_hash = hash160(pub1.get_bytes())
        script = CScript(
            [CScriptOp(OP_DUP), CScriptOp(OP_HASH160), pub1_hash, CScriptOp(OP_EQUALVERIFY),
             CScriptOp(OP_CHECKSIG)])
        script_desc = 'pkh('+ sec1_wif + ')'
        taproot_scriptpubkey, taproot_tweak, taproot_control_block = taproot_construct(pub1, [script])
        desc = 'tap('+ invalid_point_hex +','+ script_desc + ')'

        taproot_address = program_to_witness(1, taproot_scriptpubkey[2:])

        txid1 = self.nodes[0].sendtoaddress(taproot_address, 33)
        vout1 = find_output(self.nodes[0], txid1, 33)
        self.sync_all()

        def test_psbt_input_keys(psbt_input, keys):
            """Check that the psbt input has only the expected keys."""
            assert_equal(set(keys), set(psbt_input.keys()))

        # Create a PSBT. None of the inputs are filled initially
        psbt = self.nodes[1].createpsbt([{"txid": txid1, "vout": vout1}],
                                        {self.nodes[0].getnewaddress(): 32.999})
        decoded = self.nodes[1].decodepsbt(psbt)
        test_psbt_input_keys(decoded['inputs'][0], [])
        updated = self.nodes[1].utxoupdatepsbt(psbt=psbt, descriptors=[desc])
        decoded = self.nodes[1].decodepsbt(updated)
        print(decoded)
        print('\n')
        res = self.nodes[1].importmulti([{"desc": descsum_create(desc), "timestamp": 0, "watchonly": True}])
        assert res[0]['success']
        processed = self.nodes[1].walletprocesspsbt(updated)



if __name__ == '__main__':
    PSBTTest().main()
