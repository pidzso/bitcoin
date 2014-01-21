#include "merkel.h"

//since this implementation reads fixed number of characters, it read after EOF as well
//this find the EOF char, and set the lenght to its position
long length;
void cut(long* thing){
	long n=0;
	while(thing[n]!=-48){
		n++;}
	length=n;}

//for a given number of transactions it sets deep to the required level of merkel tree
int deep;
void deepness(int transnum){
	int d=0;
	while(pow(2,d)<transnum){
		d++;}
  deep=d;}

//from a given level in a merkel tree it concenate the elements pairwise and hash them to get the level above
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

//finds the merkel root for the given transactions
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
        deep=deep-1;}
    for(i=0;i<32;i++){
		root[i]=help[0][0][i];}}

//read transaction hashes
void readtransactions(unsigned char x0[64], unsigned char x1[64], unsigned char x2[64], unsigned char x3[64], unsigned char x4[64], unsigned char z[1024][32]){
	FILE *in;
	in=fopen("blocktransactions.txt", "r");
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

//read the raw block
//collect the hashes of the transactions into transactions.txt
//collect the header informations into blockdata.txt
unsigned char root[64], nonce[16], bit[16], time[16];
unsigned char transactions[1024][64];
void readraw(){
	FILE *rawf;
	FILE *transf;
	FILE *headf;
	int tlen=0,blen=0,nlen=0;
	long i=0, j=0;
	long number=0;
	long input[262144];
    char in[262144];
	rawf=fopen("raw.txt","r+");
		for(i=0;i<262144;i++){
			fscanf(rawf,"%c",&in[i]);
			input[i]=chartoint(in[i]);}
	fclose(rawf);
	cut(input);
	for(j=0;j<length;j++){
		if(input[j]==75 && input[j+1]==-38 && input[j+8]==-14){
			for(i=0;i<64;i++){
				transactions[number][i]=input[j+16+i];}
			number=number+1;}}
	convert(transactions, number);
	transf=fopen("blocktransactions.txt", "w");
		for(i=0;i<number;i++){
			for(j=0;j<64;j++){
				fprintf(transf,"%x",transactions[i][j]);}
			if(i!=number-1){
				fprintf(transf,"\n");}}
	fclose(transf);
	j=0;
	while(input[j]!=66 || input[j+1]!=63 || input[j+2]!=63){
		j++;}
		for(i=0;i<64;i++){
			root[i]=input[j+7+i];}
		while(input[j+64+tlen+19]!=-4){
			time[tlen]=input[j+64+tlen+19];
			tlen++;}
		while(input[j+83+tlen+blen+11]!=-4){
			bit[blen]=input[j+83+tlen+blen+11];
			blen++;}
		while(input[j+94+tlen+blen+nlen+12]!=-4){
			nonce[nlen]=input[j+94+tlen+blen+nlen+12];
			nlen++;}
	convert(root,1);
	headf=fopen("blockdata.txt", "w");
		fprintf(headf,"Transactions: %d\n",number);
		fprintf(headf,"Root: ");
		for(i=0;i<64;i++){
			fprintf(headf,"%x",root[i]);}
		fprintf(headf,"\nTime: ");
		for(i=0;i<tlen;i++){
			fprintf(headf,"%d",time[i]);}
		fprintf(headf,"\nBits: ");
		for(i=0;i<blen;i++){
			fprintf(headf,"%d",bit[i]);}
		fprintf(headf,"\nNonce: ");
		for(i=0;i<nlen;i++){
			fprintf(headf,"%d",nonce[i]);}
	fclose(headf);}

//for a given transaction give back its merkel branch
void mbranch(unsigned char hashes[1024][32], int db, unsigned char branch[1024][32], int pos){
    deepness(db);
    int posi=pos;
    int d=db;
	unsigned char help[deep+1][1024][32];
	int i,j;
	int ind=0;
	for(i=0;i<db;i++){
		for(j=0;j<32;j++){
			help[deep][i][j]=hashes[i][j];}}
	count=db;
    while(deep!=0){
		leveljump(help[deep], help[deep-1], count);
        deep=deep-1;}
	deepness(db);
	for(i=0;i<deep;i++){
		if(posi%2==1){
			for(j=0;j<32;j++){
				branch[i][j]=help[deep-i][posi-1][j];}}
		else if(posi%2==0 && d==posi){
			for(j=0;j<32;j++){
				branch[i][j]=help[deep-i][posi][j];}}
		else {
			for(j=0;j<32;j++){
				branch[i][j]=help[deep-i][posi+1][j];}}
		d=ceil(d/2);
		posi=floor(posi/2);}}

int main(int argc, char **argv){
	unsigned char hash[32];
	unsigned char input[1024][32];
    unsigned char br[1024][32];
	unsigned char input0[64];
	unsigned char input1[65];
    unsigned char input2[65];
    unsigned char input3[65];
    unsigned char input4[65];
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
		printf("Please select 1 or 2!\n");
		goto stage1;}
stage2a:
	printf("Which block would you like to verify?\nPlease insert the raw block to the raw.txt!\n");
	printf("If you done with it, please press anything!\n");
	a=getch();
	goto verify;
stage2b:
	printf("What would you like to know?\n");
	printf("\tThe transactions for an address! (1)\n");
	printf("\tThe merkel branch for a transaction! (2)\n");
	a=getch();
	if(a=='2'){
		printf("Please put the raw bloch which contains the transaction into raw.txt and put the transaction itself into transaction.txt!\n");
		printf("If you done with it, please press anything!\n");
		b=getch();
		goto mbranch;}
	else if(a=='1'){
		printf("Please put the public key to the publickey.txt!\n");
		printf("If you done with it, please press anything!\n");
		b=getch();
		goto trans;}
	else {
		printf("This option was not offered!\n");
		goto stage2b;}

verify:
	printf("Verification in progress...\n");
	readraw();
	readtransactions(input0, input1, input2, input3, input4, input);
	reverse(input[0]);
	reverse(input[1]);
	reverse(input[2]);
	reverse(input[3]);
	reverse(input[4]);
	mroot(input,5,hash);
	reverse(hash);
	printf("The generated merkel root:\n");
	print_hash(hash);
	printf("The merkel root from the block:\n");
	int i;
	for(i=0;i<64;i++){
		printf("%x", root[i]);}
	printf("\n");
	goto stage1;
mbranch:
	printf("Branch generation in progress...\n");
	readraw();
	readtransactions(input0, input1, input2, input3, input4, input);
	reverse(input[0]);
	reverse(input[1]);
	reverse(input[2]);
	reverse(input[3]);
	reverse(input[4]);
	mbranch(input, 5, br, 2);
	for(i=0;i<3;i++){
		reverse(br[i]);
		print_hash(br[i]);}
	goto stage1;
trans:
	printf("Checking in progress...\n");
	
	goto stage1;
quit:
	return 0;}
