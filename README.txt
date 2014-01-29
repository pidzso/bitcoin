The program cannot communicate the P2P network, so currently it gets the raw block from a txt file. 
It can check the validity of the block, by
	checking the Merkle root
		it prints the root from the block
		it generate and prints the root from the transactions
		do not compare the two value
	checking the coinbase transaction
		count how many are in the block
		do not check that the first one is a coinbase 
	checking the blockhash
		it prints the block hash from the raw block
		it does not hash the header
It can check a transaction by
	generating its Merkle branch for it

There are no error propagation at all. 