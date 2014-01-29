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
//transnum is the number of transactions
//deep will be the required levels for the merkel tree
int deep;
void deepness(int transnum){
	int d=0;
	while(pow(2,d)<transnum){
		d++;}
  deep=d;}

//from a given level in a merkel tree it concenate the elements pairwise and hash them to get the level above
//level0 is the original level with db transactions
//level1 is the next level with celi(db) transactions
int count;
void leveljump(unsigned char level0[1024][32], unsigned char level1[1024][32], int db){
	int i=0,j=0,ind=0;
	unsigned char help[1024][64];
	//checkes the parity, and in case of odd it doubles the last hash
	if(db%2==1){
		db=db-1;
		ind=1;
		melt(level0[db], level0[db], help[db/2]);}
    //concatenate the hashes together pairwise
	while(i<db){
		melt(level0[i],level0[i+1],help[i/2]);
		i=i+2;}
	//double hash the concatenated hashes
	for(j=0;j<db/2;j++){
		DSHA256(help[j], level1[j],64);}
	//in case of odd number, there an additional hashing at the end
	if (ind==1){
		DSHA256(help[db/2], level1[db/2],64);}
	count=db/2+ind;}

//finds the merkel root for the given transactions
//db transactions are in hashes, the merkel root goes to root
void mroot(unsigned char hashes[1024][32], int db, unsigned char root[32]){
	//measure the deepness of the tree
	deepness(db);
    unsigned char help[deep+1][1024][32];
	int i,j;
	//change the endian style
	for(i=0;i<db;i++){
		reverse(hashes[i],32);}
	int ind=0;
	//put the original transactions to the help variable
	for(i=0;i<db;i++){
		for(j=0;j<32;j++){
			help[deep][i][j]=hashes[i][j];}}
	count=db;
    //generate from one the next one till only one remains
	while(deep!=0){
		leveljump(help[deep], help[deep-1], count);
        deep=deep-1;}
    //copy the top of the tree to root
	for(i=0;i<32;i++){
		root[i]=help[0][0][i];}
	//change back the endian style
	reverse(root,32);}

//read the raw block
//collect the hashes of the transactions into transactions.txt
//collect the header informations into blockdata.txt
	//MISSING: to actually use these informations, we need to convert them to dhex format
	//MISSING: till then it can only echo them
	//unsigned char rt[32], nnc[4], bt[4], tme[4], blkhsh[32], prvhsh[32], sz[32];
	//unsigned char trnsctns[1024][32];
	long number, version;
void readraw(unsigned char blkhash[64], unsigned char prevhash[64], unsigned char root[64], unsigned char time[16], unsigned char bit[16], unsigned char nonce[16], unsigned char size[16],  unsigned char transactions[1024][64]){
	//unsigned char root[64], nonc[16], bit[16], time[16], blkhash[64], prevhash[64], size[16];
	//unsigned char transactions[1024][64];
	//long number, version;
	FILE *rawf;
	FILE *transf;
	FILE *headf;
	int tlen=0,blen=0,nlen=0,slen=0;
	long i=0, j=0;
	long no;

	//read the raw block
	long input[262144];
    char in[262144];
	rawf=fopen("raw.txt","r+");
		for(i=0;i<262144;i++){
			fscanf(rawf,"%c",&in[i]);
			input[i]=chartoint(in[i]);}
	fclose(rawf);
	cut(input);

	//put the hashes of the transactions into transactions[1024][64] in a format of int
	//count the transactions in number
	no=0;
	for(j=0;j<length;j++){
		if(input[j]==75 && input[j+1]==-38 && input[j+8]==-14){
			for(i=0;i<64;i++){
				transactions[no][i]=input[j+16+i];}
			no=no+1;}}
	convert(transactions, no);
	number=no;

	//write the transactions into blocktransactions.txt
	transf=fopen("blocktransactions.txt", "w");
		for(i=0;i<no;i++){
			for(j=0;j<64;j++){
				fprintf(transf,"%x",transactions[i][j]);}
			if(i!=no-1){
				fprintf(transf,"\n\n");}}
	fclose(transf);

	//look for the header data
	j=0;
	//the block hash place are always at the beginning of the raw file
	for(i=12;i<76;i++){
		blkhash[i-12]=input[i];}
	//the version comes right after the blockhash
	version=input[87];
	//now look for "roo"
	while(input[j]!=66 || input[j+1]!=63 || input[j+2]!=63){
		j++;}
		//right arter "roo" comes the merkel root
		for(i=0;i<64;i++){
			root[i]=input[j+7+i];}
		//fixed places earlier in the file was the hash of the previous block
		for(i=0;i<64;i++){
			prevhash[i]=input[i+(j-75)];}
		//till ',' it reads the time (the beginning is fixed distance from 'roo')
		//save its length into tlen
		while(input[j+64+tlen+19]!=-4){
			time[tlen]=input[j+64+tlen+19];
			tlen++;}
		//till ',' it reads the bti (the beginning is fixed distance from 'roo')
		//save its length into blen
		while(input[j+83+tlen+blen+11]!=-4){
			bit[blen]=input[j+83+tlen+blen+11];
			blen++;}
		//till ',' it reads the nonce (the beginning is fixed distance from 'roo')
		//save its length into nlen
		while(input[j+94+tlen+blen+nlen+12]!=-4){
			nonce[nlen]=input[j+94+tlen+blen+nlen+12];
			nlen++;}
	//the size can be warious distance from 'roo', so we look directly for 'siz'
	j=0;
	while(input[j]!=67 || input[j+1]!=57 || input[j+2]!=74){
		j++;}
		while(input[j+slen+6]!=-4){
			size[slen]=input[j+slen+6];
			slen++;}
	//convert the hash values to int with their original value
	convert1(root, 64);
	convert1(blkhash, 64);
	convert1(prevhash, 64);

	//write the header data into blockdata.txt
	headf=fopen("blockdata.txt", "w");
		fprintf(headf,"Block hash: ");
		for(i=0;i<64;i++){
			fprintf(headf,"%x",blkhash[i]);}
		fprintf(headf,"\nVersion: %d", version);
		fprintf(headf,"\nPrevious block hash: ");
		for(i=0;i<64;i++){
			fprintf(headf,"%x",prevhash[i]);}
		fprintf(headf,"\nRoot: ");
		for(i=0;i<64;i++){
			fprintf(headf,"%x",root[i]);}
		fprintf(headf,"\nTime: ");
		for(i=0;i<tlen;i++){
			fprintf(headf,"%d",time[i]);}
		fprintf(headf,"\nBits: ");
		for(i=0;i<tlen;i++){
			fprintf(headf,"%d",bit[i]);}
		fprintf(headf,"\nNonce: ");
		for(i=0;i<nlen;i++){
			fprintf(headf,"%d",nonce[i]);}
		fprintf(headf,"\nTransactions: %d",number);
		fprintf(headf,"\nSize: ");
		for(i=0;i<slen;i++){
			fprintf(headf,"%d",size[i]);}
	fclose(headf);}

//checks the coinbase transaction
//cbnum is the number of the coinbase transactions
int cbnum;
void coinbase(){
	//read the raw block
	FILE *rawf;
	long i=0,j=0,k=0;
	long input[262144];
    char in[262144];
	rawf=fopen("raw.txt","r+");
		for(i=0;i<262144;i++){
			fscanf(rawf,"%c",&in[i]);
			input[i]=chartoint(in[i]);}
	fclose(rawf);
	cut(input);
	cbnum=0;
	//look for 64 0s in a row (the address from the coinbase generated BTCs are from)
	for(j=0;j<length;j++){
		i=0;
		while(input[j+i]==0){
			i++;}
		if(i==64){
			cbnum++;}}}

//for a given transaction give back its merkel branch
//db transactions are in hashes, pos indicates for which transaction's branch is needed
//branch will cointain the branch, while lor will indicate the direction where it should be concatenated
void mbranch(unsigned char hashes[1024][32], int db, unsigned char branch[1024][32], int lor[1024], int pos){
	//first it works as mroot
	deepness(db);
    int posi=pos;
    int d=db;
	unsigned char help[deep+1][1024][32];
	int i,j;
	for(i=0;i<db;i++){
		reverse(hashes[i],32);}
	int ind=0;
	for(i=0;i<db;i++){
		for(j=0;j<32;j++){
			help[deep][i][j]=hashes[i][j];}}
	count=db;
    while(deep!=0){
		leveljump(help[deep], help[deep-1], count);
        deep=deep-1;}
	//than depending on the position of the given transaction, it copies the required hashes from help into branch
	//it also puts the concatenation direction into lor
	deepness(db);
	for(i=0;i<deep;i++){
		if(posi%2==1){
			for(j=0;j<32;j++){
				branch[i][j]=help[deep-i][posi-1][j];}
			lor[i]=-1;}
		else if(posi%2==0 && d==posi){
			for(j=0;j<32;j++){
				branch[i][j]=help[deep-i][posi][j];}
			lor[i]=0;}
		else {
			for(j=0;j<32;j++){
				branch[i][j]=help[deep-i][posi+1][j];}
			lor[i]=1;}
		d=ceil(d/2);
		posi=floor(posi/2);
		reverse(branch[i],32);}}

int main(int argc, char **argv){
	//these are needed because readraw depend on chars instead of double hexes
	//	unsigned char root[32], nonce[4], bit[4], time[4], blkhash[32], prevhash[32], size[32];
	//	unsigned char transactions[1024][32];
	
	//the blockheader
	unsigned char blkhash[64], prevhash[64], root[64], time[16], bit[16], nonce[16], size[16], transactions[1024][64];
	//the branch from the tree
	unsigned char br[1024][32];
    //the order indicator for concatenation with the branch
	int rol[1024];
	//the position in a transactionlist for a given transaction
    int pstn;
    //unformated transactions
	unsigned char unformated[32768];
	//a hash value
	unsigned char hash[32];
	//formatted transactions
	unsigned char input[1024][32];
    //help variables for the human interaction
	int a,b;
	printf("Welcome in EIT-BTC cliens!\nWhat do you want?\n");
stage1:
	//the first stage where the user can choose what he/she wants
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
	//the block verification module
	printf("Which block would you like to verify?\nPlease insert the raw block to the raw.txt!\n");
	printf("If you done with it, please press anything!\n");
	a=getch();
	goto verify;
stage2b:
	//the transaction checking module
	printf("What would you like to know?\n");
	printf("\tThe transactions for an address! (1)\n");
	printf("\tThe merkel branch for a transaction! (2)\n");
	a=getch();
	if(a=='2'){
		printf("Please put the raw bloch which contains the transaction into raw.txt and put the transaction's hash value into transaction.txt!\n");
		printf("If you done with it, please press anything!\n");
		b=getch();
		goto mbranch;}
	else if(a=='1'){
		printf("This function is temporaty not avalible!\n");
		goto stage3;}
	else {
		printf("This option was not offered!\n");
		goto stage2b;}

verify:
	//the verification process
	printf("Merkel root checking...\n");
	//read the merkel root from the rw block file
	//generate the merkel root from the transactions from the raw block file
	readraw(blkhash, prevhash, root, time, bit, nonce, size, transactions);
	readtrans(unformated);
	separate(number,unformated,input);
	mroot(input,number,hash);
	printf("The generated merkel root:\n");
	print_hash(hash);
	printf("The merkel root from the block:\n");
	int i;
	for(i=0;i<64;i++){
		printf("%x", root[i]);}
	printf("\n");
	printf("Coinbase transaction checking...\n");
	//checked the number of the coinbase transactions in the block
	coinbase();
	printf("Number of coinbase transactions: %d\n", cbnum);
	printf("Blockhash checking...\n");
	printf("The blockhash from the raw file:\n");
	for(i=0;i<64;i++){
		printf("%x",blkhash[i]);}
	printf("\nThe generated hash file:\n");
	//MISING: hashhing the blockheader
		printf("This function is temporaty not avalible!\n");
	goto stage3;
mbranch:
	printf("Branch generation in progress...\n");
	//read the raw block and generates the branch for a given transaction
	readraw(blkhash, prevhash, root, time, bit, nonce, size, transactions);
	readtrans(unformated);
	separate(number,unformated,input);
	position(input, number, input[pstn], pstn);
	mbranch(input, number, br, rol, pstn+1);
	for(i=0;i<3;i++){
		if(rol[i]==1){
			printf("The %d step: Concatenate the following hash with the previous result!\n",i+1);}
		else {
			printf("The %d step: Concatenate the previous result with the following hash!\n",i+1);}
		print_hash(br[i]);}
	goto stage3;
trans:
	printf("Checking in progress...\n");
	//MISSING: for a given address and/or public key the BTC sum of the recieved and sent transactions
	goto stage3;
stage3:
	//after one action, the user can continue or exit
	printf("Would you like to do something else?\n\tYes (1)\n\tNo (0)\n");
	b=getch();
	if(b=='1'){
		goto stage1;}
	else if(b=='0'){
		goto quit;}
	else {
		printf("This was not an option!\n");
		goto stage3;}
error:
	//error propagation
	//MISSING: every data from a file should be verified, that it is in a right format or not
	printf("Something went wrong!\n");
	goto stage3;
quit:
	return 0;}
