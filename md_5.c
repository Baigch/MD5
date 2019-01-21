//
//  md_5.c
//  MD5
//
//  Created by 白光程 on 2018/12/1.
//  Copyright © 2018 白光程. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "md_5.h"

//迭代运算中迭代运算采用的做循环移位的s值
unsigned int s[64] = {
    7,12,17,22,7,12,17,22,7,12,17,22,7,12,17,22,
    5,9,14,20,5,9,14,20,5,9,14,20,5,9,14,20,
    4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23,
    6,10,15,21,6,10,15,21,6,10,15,21,6,10,15,21
};
//各轮循环中使用的T值
unsigned int t[] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee ,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501 ,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be ,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821 ,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa ,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8 ,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed ,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a ,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c ,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70 ,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05 ,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665 ,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039 ,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1 ,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1 ,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 };
//第一位为1，剩余为0，用于补足
unsigned char PADDING[64]={0x80};

void md5_Init(MD5_CTX *context)
{
    context->count[0] = 0;
    context->count[1] = 0;
    context->state[0] = 0x67452301;
    context->state[1] = 0xEFCDAB89;
    context->state[2] = 0x98BADCFE;
    context->state[3] = 0x10325476;
}
void md5_Update(MD5 *context,unsigned char *input,unsigned int inputlen)
{
    unsigned int index = 0, i=0;
    unsigned int addlen = 0;
    //当前状态的位数
    index = context->count[0] / 8 % 64;
    //需要补齐的位数
    addlen = 64 - index;
    
    //当一个unsignde int 无法解决存储极大数据导致溢出的问题时，将两个16位的数连在一起，生成一个32位2	  //进制数，扩大存储范围
    context->count[0] += inputlen * 8;
    if(context->count[0] < (inputlen * 8))
        context->count[1]++;
    context->count[1] += inputlen >> 29;
    
   //补足并加密的判断：当输入的字节数大于其可以补足64字节的字节数时，进行补足
    if(inputlen >= addlen){
        //向buffer中补足addlen个字节，使其达到64字节
        memcpy(&context->buffer[index], input, addlen);
        //buffer达到64字节时（512位），将其作为一组进行运算
        md5_Transform(context->state, context->buffer);
		//输入数据若可以组成多组512位时，继续进行运算
        for (i = addlen; i+64 <= inputlen; i += 64) {
            md5_Transform(context->state, &input[i]);
        }
        //一组加密结束，恢复0值，开始新一轮
        index = 0;
    }
    //条件不满足时，将输入数据按顺序放在原数据的后面
    else{
        i=0;
    }
    //剩余数据的处理，放入buffer
    memcpy(&context->buffer[index], &input[i], inputlen-i);
}
void md5_Final(MD5 *context,unsigned char digest[16])
{
    unsigned int index = 0,addlen = 0;
    unsigned char bits[8];
    //当前状态的位数
    index = context->count[0] / 8 % 64;
    //所需填充的字节数，由于位长要满足对512求余结果为448，当小于448位时，填充56-index字节，否则再向
    //后取512位，填充120-index字节
    if (index < 56) {
        addlen = 56 - index;
    }else
        addlen = 120 - index;
    //将填充前数据数据长度转换成char类型后放入bits中
    md5_toChar(bits,context->count,8);
    //由已经存储好的数组PADDING，在信息后面填充一个1和数个0，直到满足条件，再进行加密，加密后剩384位
    md5_Update(context,PADDING,addlen);
    //为再凑成一组448位的数据，再添加64位（8字节）长的数据，进行一次加密
    md5_Update(context,bits,8);
    //将加密结果以字符形式输出，128位（16字节）
    md5_toChar(digest,context->state,16);
}
void md5_toChar(unsigned char *output,unsigned int *input,unsigned int len)
{
    unsigned int i = 0,j = 0;
    for (i=0,j=0; j<len; i++,j+=4) {
        output[j] = input[i] & 0xFF;
        output[j+1] = (input[i] >> 8) & 0xFF;
        output[j+2] = (input[i] >> 16) & 0xFF;
        output[j+3] = (input[i] >> 24) & 0xFF;
    }
}

void md5_toInt(unsigned int *output,unsigned char *input,unsigned int len)
{
    unsigned int i = 0,j = 0;
    for (i=0,j=0; j<len; i++,j+=4){
        output[i] = (input[j]) |
        (input[j+1] << 8) |
        (input[j+2] << 16) |
        (input[j+3] << 24);
    }
}
void md5_Transform(unsigned int state[4],unsigned char block[64])
{
    //初始化，每次继承上一组的加密结果
    unsigned int a = state[0];
    unsigned int b = state[1];
    unsigned int c = state[2];
    unsigned int d = state[3];
    unsigned int x[64], i, j;
    //将字符转换为数字
    md5_toInt(x,block,64);
    //四轮压缩64次迭代运算
    for (i=0; i<64; i ++) {
        //第一轮迭代
        if (i<16){
            j = i;
            FF(a, b, c, d, x[j], s[i], t[i]);
        }
        //第二轮迭代
        else if(i < 32){
            j = (5*i+1) % 16;
            GG(a, b, c, d, x[j], s[i], t[i]);
        }
        //第三轮迭代
        else if(i < 48){
            j = (3*i + 5) % 16;
            HH(a, b, c, d, x[j], s[i], t[i]);
        }
        //第四轮迭代
        else {
            j = (7*i) % 16;
            II(a, b, c, d, x[j], s[i], t[i]);
        }
        //每次迭代运算后，a，b，c，d轮换顺序
        unsigned int temp = a, temp1 = b, temp2 = c;
        a = d;
        b = temp;
        c = temp1;
        d = temp2;
    }
    //更新原来的结果，在原基础上加上新的部分
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
}

int main(int argc, char *argv[])
{
    int i;
    unsigned char encrypt[1000] ;
    unsigned char decrypt[16];
    MD5 md5;
    md5_Init(&md5);
    printf("请输入需要加密的明文：");
    scanf("%s", &encrypt);
    
    md5_Update(&md5,encrypt,strlen((char *)encrypt));
    md5_Final(&md5,decrypt);
    printf("\n加密结果:");
    for(i=0;i<16;i++)
    {
        printf("%02x",decrypt[i]);
    }
    
    printf("\n\n\n");
    return 0;
}

