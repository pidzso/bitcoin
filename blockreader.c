#include "merkel.h"

long length;
void cut(long* thing){
	long n=0;
	while(thing[n]!=-48){
		n++;}
	length=n;}

readraw(){

  	FILE *rawf;
    FILE *transf;
    FILE *headf;
unsigned char transactions[1024][64];
	unsigned char root[64];
	unsigned char size[16];
	unsigned char nonce[16];
	unsigned char bit[16];
	unsigned char time[16];
	int tlen=0,blen=0,nlen=0,slen=0;
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
		while(input[j+106+tlen+blen+nlen+slen+25]!=-4){
			size[slen]=input[j+106+tlen+blen+nlen+slen+25];
			slen++;}
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
		fprintf(headf,"\nSize: ");
		for(i=0;i<slen;i++){
			fprintf(headf,"%d",size[i]);}
	fclose(headf);}

int main(int argc, char **argv){
	readraw();
	return 0;}
