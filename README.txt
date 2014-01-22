tex file írása
txt -ből való tetszőleseg sok karakterbeolvasás amiket használható hex-é alakítunk
	ezt átirni olyanra, hogy adott hosszú bemenethez gyártja le a neki megfelelő dupla hexes listát (pl (e,a,c,4,5,d)->(ea,c4,5d)) !!!
funkciók
	adott tranzakció adatai
		adott címről-címre való utalás van-e
		mekkor az adott címről-címre való utalások összege
			az a baj, hogy a raw blokkban nincsenek címek, csak publikus kulcsok
	block ellenőrzés
		első tranzakció coinbase
		a többi nem az 
		proof of work, azaz a hash tényleg a block hash -je-e
			https://en.bitcoin.it/wiki/Block_hashing_algorithm
			kéne egy olyan program, ami egy számlistához hozzárendeli a számot (KÉSZ)
				kéne egy olyan program, ami egy számhoz hozzárendeli a neki megfelelő fix hosszú (4) hexa listát (pl 3=(00,00,00,03)) !!!
			kéne egy olyan program, ami összekonkatenálja a dolgokat (KÉSZ)
			kéne egy olyan program, ami kiszámolja a hashjét midennek (KÉSZ)