#pragma once
typedef struct
{
	unsigned char key[32];
	unsigned char nonce[16];
	unsigned char assoc[48];
	unsigned int assoc_len;
	unsigned char p[80];
	unsigned int p_len;
	unsigned char c[80];
	unsigned char tag[16];
}LEA_GCM_AE;

