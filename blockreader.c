#include "merkel.h"

long chartoint(char a){
	long b;
	b=a-'0';
	return b;}

long length;
void cut(long* thing){
	long n=0;
	while(thing[n]!=-48){
		n++;}
	length=n;}

void convert(unsigned char thing[1024][64], int k){
	int n,m;
	for(n=0;n<k;n++){
		for(m=0;m<64;m++){
			if(thing[n][m]>40){
				thing[n][m]=thing[n][m]-39;}
			else if (thing[n][m]>10){
				thing[n][m]=thing[n][m]-7;}}}}

int main(int argc, char **argv){
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
	FILE *input_file;
	input_file=fopen("raw.txt","r+");
		for(i=0;i<262144;i++){
			fscanf(input_file,"%c",&in[i]);
			input[i]=chartoint(in[i]);}
	fclose(input_file);

	cut(input);
	for(j=0;j<length;j++){
		if(input[j]==75 && input[j+1]==-38 && input[j+8]==-14){
			for(i=0;i<64;i++){
				transactions[number][i]=input[j+16+i];}
			number=number+1;}}
	convert(transactions, number);
	FILE *output_file1;
	output_file1=fopen("blocktransactions.txt", "w");
		for(i=0;i<number;i++){
			for(j=0;j<64;j++){
				fprintf(output_file1,"%x",transactions[i][j]);}
			if(i!=number-1){
				fprintf(output_file1,"\n");}}
	fclose(output_file1);

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
	FILE *output_file2;
	output_file2=fopen("blockdata.txt", "w");
		fprintf(output_file2,"Transactions: %d\n",number);
		fprintf(output_file2,"Root: ");
		for(i=0;i<64;i++){
			fprintf(output_file2,"%x",root[i]);}
		fprintf(output_file2,"\n");
		fprintf(output_file2,"Time: ");
		for(i=0;i<tlen;i++){
			fprintf(output_file2,"%d",time[i]);}
		fprintf(output_file2,"\n");
		fprintf(output_file2,"Bits: ");
		for(i=0;i<blen;i++){
			fprintf(output_file2,"%d",bit[i]);}
		fprintf(output_file2,"\n");
		fprintf(output_file2,"Nonce: ");
		for(i=0;i<nlen;i++){
			fprintf(output_file2,"%d",nonce[i]);}
		fprintf(output_file2,"\n");
		fprintf(output_file2,"Size: ");
		for(i=0;i<slen;i++){
			fprintf(output_file2,"%d",size[i]);}
	fclose(output_file2);

	return 0;}
