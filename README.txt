tex file írása (Réka, Máté)
txt -ből való tetszőleseg sok karakterbeolvasás amiket használható hex-é alakítunk (Máté)
funkciók
	adott tranzakció adatai (Balázs)
		adott címről-címre való utalás van-e
			az a baj, hogy a raw blokkban nincsenek címek, csak publikus kulcsok, így ott cím szerint nem tudok keresni
		mekkor az adott címről-címre való utalások összege
			a probléma itt is a cím hiánya
	block ellenőrzés
		első tranzakció coinbase (Balázs&Réka)
		a többi nem az (Balázs&Réka)
		proof of work, azaz a hash tényleg a block hash -je-e
		timestamp jó-e, azaz közel van az előző timestamphoz