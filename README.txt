tex file írása
p2p -ból való blockszerzés
txt -ből való tetszőleseg sok karakterbeolvasás amiket használható hex-é alakítunk
funkciók:
	adott tranzakció adatai
		adott címről-címre való utalás van-e
		mekkor az adott címről-címre való utalások összege
		merkele branch-je a tranzakciónak
	block ellenőrzés:
		első tranzakció coinbase
		a többi nem az
		proof of work, azaz a hash tényleg a block hash -je-e
		timestamp jó-e, azaz közel van az előző timestamphoz