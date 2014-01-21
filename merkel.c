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

//beolvas fileból két hash értéket
void read(FILE *in, unsigned char x0[64], unsigned char x1[64], unsigned char x2[64], unsigned char x3[64], unsigned char x4[64], unsigned char z[1024][32]){
	in=fopen("input.txt", "r");
		fscanf(in,"%s\n%s\n%s\n%s\n%s",x0, x1, x2, x3, x4);
		const char *src0 = x0, *src1 = x1, *src2 = x2, *src3 = x3, *src4 = x4;
		unsigned char buffer0[32], buffer1[32], buffer2[32], buffer3[32], buffer4[32];
		unsigned char *dst0 = buffer0, *dst1 = buffer1, *dst2 = buffer2, *dst3 = buffer3, *dst4 = buffer4;
		unsigned char *end0 = buffer0 + sizeof(buffer0), *end1 = buffer1 + sizeof(buffer1), *end2 = buffer2 + sizeof(buffer2), *end3 = buffer3 + sizeof(buffer3), *end4 = buffer4 + sizeof(buffer4);
 		unsigned int u0, u1, u2, u3, u4;
		while (dst0 < end0 && sscanf(src0, "%02x", &u0) == 1){
 			*dst0++ = u0;
        	src0 += 2;}
  		while (dst1 < end1 && sscanf(src1, "%02x", &u1) == 1){
 			*dst1++ = u1;
        	src1 += 2;}
		while (dst2 < end2 && sscanf(src2, "%02x", &u2) == 1){
 			*dst2++ = u2;
        	src2 += 2;}
        while (dst3 < end3 && sscanf(src3, "%02x", &u3) == 1){
 			*dst3++ = u3;
        	src3 += 2;}
        while (dst4 < end4 && sscanf(src4, "%02x", &u4) == 1){
 			*dst4++ = u4;
        	src4 += 2;}
		int i=0;
		for (dst0 = buffer0; dst0 < end0; dst0++){
			z[0][i]=*dst0;
        	i++;}
		i=0;
        for (dst1 = buffer1; dst1 < end1; dst1++){
        	z[1][i]=*dst1;
        	i++;}
		i=0;
        for (dst2 = buffer2; dst2 < end2; dst2++){
        	z[2][i]=*dst2;
        	i++;}
        i=0;
        for (dst3 = buffer3; dst3 < end3; dst3++){
        	z[3][i]=*dst3;
        	i++;}
        i=0;
        for (dst4 = buffer4; dst4 < end4; dst4++){
        	z[4][i]=*dst4;
        	i++;}
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
	int a,b;
	printf("Welcome in EIT-BTC cliens!\nWhat do you want?\n");
stage1:
    printf("\tPress 1 to check the validity of a block!\n");
    printf("\tPress 2 to check transactions!\n");
	printf("\tPress 0 to quit!\n");
	a=getch();
	if(a=='0'){
		goto quit;}
	if(a=='1'){
		goto stage2a;}
	else if(a=='2'){
		goto stage2b;}
	else {
		printf("You choosen poorly!\n");
		goto stage1;}
stage2a:
	printf("Which block would you like to verify?\nPlease insert the raw block to the raw.txt!\n");
	printf("If you done with it, please press anything!\n");
	a=getch();
	goto verify;
stage2b:
	printf("What would you like to know?\n");
	printf("\tThe incoming transactions for an address! (1)\n");
	printf("\tThe outgoing transactions for an address! (2)\n");
	printf("\tThe merkel branch for a transaction! (3)\n");
	a=getch();
	if(a=='3'){
		printf("Please put the hash of the transaction into trans.txt!\n");
		printf("If you done with it, please press anything!\n");
		b=getch();
		goto mbranch;}
	else if(a=='1'){
		printf("Please put the address into the address.txt!\n");
		printf("If you done with it, please press anything!\n");
		b=getch();
		goto incoming;}
	else if(a=='2'){
		printf("Please put the address into the address.txt!\n");
		printf("If you done with it, please press anything!\n");
		b=getch();
		goto outgoing;}
	else {
		printf("This option was not offered!\n");
		goto stage2b;}

verify:
	printf("Verification in progress...\n");
	
	goto stage1;
mbranch:
	printf("Branch generation in progress...\n");
	
	goto stage1;
incoming:
	printf("Checking in progress...\n");
	
	goto stage1;
outgoing:
	printf("Checking in progress...\n");
	
	goto stage1;

	unsigned char input[1024][32];
    unsigned char input0[64];
	unsigned char input1[65];
    unsigned char input2[65];
    unsigned char input3[65];
    unsigned char input4[65];
	unsigned char hash[32];
    FILE *input_file;
	read(input_file, input0, input1, input2, input3, input4, input);

	reverse(input[0]);
	reverse(input[1]);
	reverse(input[2]);
	reverse(input[3]);
	reverse(input[4]);
	
	mroot(input,5,hash);
	reverse(hash);
	print_hash(hash);
quit:
	return 0;}
