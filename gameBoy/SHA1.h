#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<cstdint>

uint32_t leftRotate(uint32_t n, unsigned int d,unsigned char INT_BITS=32) 
{ 
    /* In n<<d, last d bits are 0. To 
     put first 3 bits of n at  
    last, do bitwise or of n<<d  
    with n >>(INT_BITS - d) */
    return (n << d)|(n >> (INT_BITS - d)); 
}
void SHA1(char * orgMessage){
    uint32_t h0 = 0x67452301;
uint32_t h1 = 0xEFCDAB89;
uint32_t h2 = 0x98BADCFE;
uint32_t h3 = 0x10325476;
uint32_t h4 = 0xC3D2E1F0;

int orgMessageLengthBytes=strlen(orgMessage);
int orgMessageLengthBits=strlen(orgMessage)*8;
//padding(1 bit and zeros).
unsigned int paddingLengthBits =(unsigned int)(448-orgMessageLengthBits)%512;
int hashMessageLengthBytes=(orgMessageLengthBits+paddingLengthBits+64)/8;
unsigned char* hashMessage=(unsigned char*)calloc(hashMessageLengthBytes,1);
//unsigned char hashMessage[64];
memcpy(hashMessage,orgMessage,orgMessageLengthBytes);
//printf("%d\n", hashMessageLengthBytes);
int paddingLengthBytes=(paddingLengthBits)/8;
for(int i=orgMessageLengthBytes;i<hashMessageLengthBytes;i++){
    hashMessage[i]=0x00;
}
hashMessage[orgMessageLengthBytes]=0x80;
uint64_t d=orgMessageLengthBits;
//little endian to big endian.
uint64_t orgMessaggeLength=(d&0xff)<<56|(d>>8&0xff)<<48|(d>>16&0xff)<<40|(d>>24&0xff)<<32|(d&0xff>>32)<<24|(d&0xff>>40)<<16|(d&0xff>>48)<<8|(d&0xff>>56);
//uint64_t orgMessaggeLength = d;
//set orgMessaggeLength in 8 last bytes(64 bits)
*(uint64_t*)(hashMessage+orgMessageLengthBytes+paddingLengthBytes)=orgMessaggeLength;
//break to chunk of 512 bits(64 bytes)
for(int index=0;index<hashMessageLengthBytes;index+=64){
    uint32_t w[80];
    for(int i=index;i<index+64;i+=4){
        w[(i-index)/4]=(hashMessage[i]<<24)|(hashMessage[i+1]<<16)|(hashMessage[i+2]<<8)|hashMessage[i+3];
    }
     for(int i=16;i<80;i++){
       w[i] = (w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16]);
       w[i]=leftRotate(w[i],1);
    }
     printf("w14:%u w15:%u\n", w[14], w[15]);
    uint32_t a = h0;
    uint32_t b = h1;
    uint32_t c = h2;
    uint32_t d = h3;
    uint32_t e = h4;
    uint32_t f=0;
    uint32_t k=0;
    uint32_t temp=0;
    
    for(int i=0;i<80;i++){
         
        if (0 <= i&&i < 20){
            f = (b & c)|((b^0xffffffff) & d);
            k = 0x5A827999;
           
        }else if(20 <=i&&i < 40){
            f = b  ^ c ^ d;
            k = 0x6ED9EBA1;
        }else if (40<= i&&i<60){
            f = (b & c) | (b & d) | (c & d) ;
            k = 0x8F1BBCDC;
        }else if(60<= i&&i <80){
            f = b ^ c ^ d;
            k = 0xCA62C1D6;
        }
        temp = leftRotate(a,5) + f + e + k + w[i];
        e = d;
        d = c;
        c = leftRotate(b,30);
        b = a;
        a = temp;
    }
       h0 += a;
        h1 += b; 
        h2 += c;
        h3 += d;
        h4 += e;
}
printf("\nhash:%04x%04x%04x%04x%04x\n",h0,h1,h2,h3,h4);
}
/*int main()
{
    char t[3]={'a','v','i'};
    SHA1(t);
return 0;
}*/