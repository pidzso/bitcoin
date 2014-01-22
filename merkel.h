#include <stdio.h>
#include <stdlib.h>
#include <math.h>

///////
//SHA//
///////

// Signed variables are for wimps 
#define uchar unsigned char // 8-bit byte
#define uint unsigned long // 32-bit word

// DBL_INT_ADD treats two unsigned ints a and b as one 64-bit integer and adds c to it
#define DBL_INT_ADD(a,b,c) if (a > 0xffffffff - (c)) ++b; a += c;
#define ROTLEFT(a,b) (((a) << (b)) | ((a) >> (32-(b))))
#define ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))

#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))
#define SIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))

typedef struct {
   uchar data[64];
   uint datalen;
   uint bitlen[2];
   uint state[8];
} SHA256_CTX;

uint k[64] = {
   0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
   0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
   0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
   0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
   0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
   0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
   0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
   0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2};

void sha256_transform(SHA256_CTX *ctx, uchar data[]){  
   uint a,b,c,d,e,f,g,h,i,j,t1,t2,m[64];
   for (i=0,j=0; i < 16; ++i, j += 4)
      m[i] = (data[j] << 24) | (data[j+1] << 16) | (data[j+2] << 8) | (data[j+3]);
   for ( ; i < 64; ++i)
      m[i] = SIG1(m[i-2]) + m[i-7] + SIG0(m[i-15]) + m[i-16];
   a = ctx->state[0];
   b = ctx->state[1];
   c = ctx->state[2];
   d = ctx->state[3];
   e = ctx->state[4];
   f = ctx->state[5];
   g = ctx->state[6];
   h = ctx->state[7];
   for (i = 0; i < 64; ++i) {
      t1 = h + EP1(e) + CH(e,f,g) + k[i] + m[i];
      t2 = EP0(a) + MAJ(a,b,c);
      h = g;
      g = f;
      f = e;
      e = d + t1;
      d = c;
      c = b;
      b = a;
      a = t1 + t2;}
   ctx->state[0] += a;
   ctx->state[1] += b;
   ctx->state[2] += c;
   ctx->state[3] += d;
   ctx->state[4] += e;
   ctx->state[5] += f;
   ctx->state[6] += g;
   ctx->state[7] += h;}  

void sha256_init(SHA256_CTX *ctx){  
   ctx->datalen = 0; 
   ctx->bitlen[0] = 0; 
   ctx->bitlen[1] = 0; 
   ctx->state[0] = 0x6a09e667;
   ctx->state[1] = 0xbb67ae85;
   ctx->state[2] = 0x3c6ef372;
   ctx->state[3] = 0xa54ff53a;
   ctx->state[4] = 0x510e527f;
   ctx->state[5] = 0x9b05688c;
   ctx->state[6] = 0x1f83d9ab;
   ctx->state[7] = 0x5be0cd19;}

void sha256_update(SHA256_CTX *ctx, uchar data[], uint len){  
   uint t,i;
   for (i=0; i < len; ++i) { 
      ctx->data[ctx->datalen] = data[i]; 
      ctx->datalen++; 
      if (ctx->datalen == 64) { 
         sha256_transform(ctx,ctx->data);
         DBL_INT_ADD(ctx->bitlen[0],ctx->bitlen[1],512); 
         ctx->datalen = 0; }}}

void sha256_final(SHA256_CTX *ctx, uchar hash[]){
   uint i; 
   i = ctx->datalen; 
   // Pad whatever data is left in the buffer. 
   if (ctx->datalen < 56) { 
      ctx->data[i++] = 0x80; 
      while (i < 56) 
         ctx->data[i++] = 0x00; }  
   else {
      ctx->data[i++] = 0x80; 
      while (i < 64) 
         ctx->data[i++] = 0x00; 
      sha256_transform(ctx,ctx->data);
      memset(ctx->data,0,56); }
   
   // Append to the padding the total message's length in bits and transform. 
   DBL_INT_ADD(ctx->bitlen[0],ctx->bitlen[1],ctx->datalen * 8);
   ctx->data[63] = ctx->bitlen[0]; 
   ctx->data[62] = ctx->bitlen[0] >> 8; 
   ctx->data[61] = ctx->bitlen[0] >> 16; 
   ctx->data[60] = ctx->bitlen[0] >> 24; 
   ctx->data[59] = ctx->bitlen[1]; 
   ctx->data[58] = ctx->bitlen[1] >> 8; 
   ctx->data[57] = ctx->bitlen[1] >> 16;  
   ctx->data[56] = ctx->bitlen[1] >> 24; 
   sha256_transform(ctx,ctx->data);
   
   // Since this implementation uses little endian byte ordering and SHA uses big endian,
   // reverse all the bytes when copying the final state to the output hash. 
   for (i=0; i < 4; ++i) { 
      hash[i]    = (ctx->state[0] >> (24-i*8)) & 0x000000ff; 
      hash[i+4]  = (ctx->state[1] >> (24-i*8)) & 0x000000ff; 
      hash[i+8]  = (ctx->state[2] >> (24-i*8)) & 0x000000ff;
      hash[i+12] = (ctx->state[3] >> (24-i*8)) & 0x000000ff;
      hash[i+16] = (ctx->state[4] >> (24-i*8)) & 0x000000ff;
      hash[i+20] = (ctx->state[5] >> (24-i*8)) & 0x000000ff;
      hash[i+24] = (ctx->state[6] >> (24-i*8)) & 0x000000ff;
      hash[i+28] = (ctx->state[7] >> (24-i*8)) & 0x000000ff;}}

void print_hash(unsigned char hash[32]){
   int idx;
   for (idx=0; idx < 32; idx++)
      printf("%02x",hash[idx]);
   printf("\n");}

void SHA256(unsigned char text[], unsigned char hash[32],int len){
   int idx;
   SHA256_CTX ctx;
   sha256_init(&ctx);
   sha256_update(&ctx,text,len);
   sha256_final(&ctx,hash);}

void DSHA256(unsigned char text[], unsigned char hash[32],int len){
   int idx;
   unsigned char help[32];
   SHA256_CTX ctx;
   sha256_init(&ctx);
   sha256_update(&ctx,text,len);
   sha256_final(&ctx,help);
   sha256_init(&ctx);
   sha256_update(&ctx,help,32);
   sha256_final(&ctx,hash);}

//little-big endian
void reverse(unsigned char bit[32]){
	int i;
	unsigned char help[32];
	for(i=0;i<32;i++){
		help[31-i]=bit[i];}
	for(i=0;i<32;i++){
		bit[i]=help[i];}}
		
//concenate two hash value
void melt(unsigned char x1[32], unsigned char x2[32], unsigned char y[64]){
	int i;
	for(i=0;i<32;i++){
		y[i]=x1[i];
		y[i+32]=x2[i];}}

//convert a char to an int
long chartoint(char a){
	long b;
	b=a-'0';
	return b;}

//convert a character value to the value what the character as a hex value represent
void convert(unsigned char thing[1024][64], int k){
	int n,m;
	for(n=0;n<k;n++){
		for(m=0;m<64;m++){
			if(thing[n][m]>40){
				thing[n][m]=thing[n][m]-39;}
			else if (thing[n][m]>10){
				thing[n][m]=thing[n][m]-7;}}}}

//give back the place where the transaction is
void position(unsigned char hashes[1024][32], int db, unsigned char tran[32], int p){
	int i,j;
	int help=0;
	for(i=0;i<db;i++){
		help=0;
		for(j=0;j<32;j++){
			if(hashes[i][j]==tran[j]){
				help=help+1;}
			if(help==32){
				p=i+1;}}}}

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
