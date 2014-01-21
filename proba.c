#include "merkel.h"

//adott számhoz (levelek száma) megmondja mennyi a fa mélysége
int deep;
void deepness(int transnum){
	int d=0;
	while(pow(2,d)<transnum){
		d++;}
  deep=d;}

//kettõ hast összerak egy kifejezéssé
void melt(unsigned char x1[32], unsigned char x2[32], unsigned char y[64]){
	int i;
	for(i=0;i<32;i++){
		y[i]=x1[i];
		y[i+32]=x2[i];}}

//egy adott szintrõl való hasheket fûzi össze, és hasheli meg újra, legyártva ezzel a következõ szintet
int count;
void leveljump(unsigned char level0[1024][32], unsigned char level1[1024][32], int db){
	int i=0,j=0,ind=0;
	unsigned char help[1024][64];
	if(db%2==1){
		db=db-1;
		ind=1;
		melt(level0[db], level0[db], help[db/2]);}
    while(i<db){
		melt(level0[i],level0[i+1],help[i/2]);
		i=i+2;}
	for(j=0;j<db/2;j++){
		DSHA256(help[j], level1[j],64);}
	if (ind==1){
		DSHA256(help[db/2], level1[db/2],64);}
	count=db/2+ind;}

/*
//a tranzakciókat hasheli össze
void transtohash(unsigned char trans[], unsigned char hash[1024][32]){
	int i;
	for(i=0;i<x;i++){
		SHA256(trans[i], hash[i]);}}
*/

//kiszámolja az adott tranzakciókhoz tartozó hashgyökeret
void mroot(unsigned char hashes[1024][32], int db, unsigned char root[32]){
    deepness(db);
    unsigned char help[deep+1][1024][32];
	int i,j;
	int ind=0;
	for(i=0;i<db;i++){
		for(j=0;j<32;j++){
			help[deep][i][j]=hashes[i][j];}}
	count=db;
    while(deep!=0){
		leveljump(help[deep], help[deep-1], count);
        db=ceil(db/2);
        deep=deep-1;}
    for(i=0;i<32;i++){
		root[i]=help[0][0][i];}}

void read(FILE *in, unsigned char x[1024][65], int db, unsigned char z[1024][32]){
	in=fopen("input.txt", "r");
		const char *src = x;
		unsigned char buffer[32];
		unsigned char *dst = buffer;
		unsigned char *end = buffer + sizeof(buffer);
 		unsigned int u;
		int j=0,i=0;
		
		x[0]={"1","c","9","8","7","0","b","0","6","e","5","a","2","4","5","6","f","9","b","a","3","7","b","0","6","0","d","2","9","d","6","0","f","d","6","c","1","3","0","6","2","2","8","9","a","2","2","c","3","5","d","c","0","5","5","1","3","5","0","9","3","6","2","8"};
		x[1]={"6","f","4","c","e","4","1","b","2","0","1","3","4","f","3","6","7","8","6","e","1","1","3","e","0","b","b","b","1","1","c","5","2","4","5","2","6","0","1","4","7","3","1","2","c","5","d","7","b","3","8","0","5","d","1","f","c","5","4","0","f","5","f","5"};
//		x[2]={"295970c90c282fb5af692d8b7aed98b096955f6e53f8ba0a6e7d12c287c25096"};
//		x[3]={"e9542e596e7263a6e6783c18c2ef51f3f9ab9769c7300820c100e0f6d2bc3974"};
//		x[4]={"0d103c28e90bcb16c7a757db15f21fd310fe217d5d592173a18380eba3f126f2"};
		//readhiány
		for(j=0;j<db;j++){
			*dst = buffer;
			end = buffer + sizeof(buffer);
			while (dst < end && sscanf(src, "%02x", &u) == 1){
				*dst++ = u;
				src += 2;}
			i=0;
			for (dst = buffer; dst < end; dst++){
				z[j][i]=*dst;
				i++;}}
		fclose(in);}

//bitreverser (little-big endian)
void reverse(unsigned char bit[32]){
	unsigned char help[32];
	int i;
	for(i=0;i<32;i++){
		help[31-i]=bit[i];}
	for(i=0;i<32;i++){
		bit[i]=help[i];}}

int main(int argc, char **argv){
    unsigned char input[1024][32];
    unsigned char in[1024][65];
	unsigned char hash[32];
    FILE *input_file;
	read(input_file, in, 5,input);

	print_hash(input[0]);
	print_hash(input[1]);
	print_hash(input[2]);
	print_hash(input[3]);
	print_hash(input[4]);

	reverse(input[0]);
	reverse(input[1]);
	reverse(input[2]);
	reverse(input[3]);
	reverse(input[4]);
	
	mroot(input,5,hash);
	reverse(hash);
	print_hash(hash);
	return 0;}
