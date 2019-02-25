//
//  md_5.h
//  MD5
//
//  Created by Baigch on 2018/12/1.
//  Copyright © 2018 Baigch. All rights reserved.
//
#ifndef md_5_h
#define md_5_h

//用于存储一个MD5 message信息
typedef struct
{
    unsigned int count[2];//记录当前数据的位数
    unsigned int state[4];//记录对512-bit信息加密的中间结果
    unsigned char buffer[64];//缓冲区
}MD5;

//四轮压缩函数
#define F(x,y,z) ((x & y) | (~x & z))
#define G(x,y,z) ((x & z) | (y & ~z))
#define H(x,y,z) (x^y^z)
#define I(x,y,z) (y ^ (x | ~z))
//循环左移n个单位
#define CLS(x,n) ((x << n) | (x >> (32-n)))
//每一轮循环中的迭代运算函数
#define FF(a,b,c,d,x,s,t) {a += F(b,c,d) + x + t; a = CLS(a,s); a += b;}
#define GG(a,b,c,d,x,s,t) {a += G(b,c,d) + x + t; a = CLS(a,s); a += b;}
#define HH(a,b,c,d,x,s,t) {a += H(b,c,d) + x + t; a = CLS(a,s); a += b;}
#define II(a,b,c,d,x,s,t) {a += I(b,c,d) + x + t; a = CLS(a,s); a += b;}
//函数申明区
void md5_Init(MD5 *context);
void md5_Update(MD5 *context,unsigned char *input,unsigned int inputlen);
void md5_Final(MD5 *context,unsigned char digest[16]);
void md5_Transform(unsigned int state[4],unsigned char block[64]);
void md5_toChar(unsigned char *output,unsigned int *input,unsigned int len);
void md5_toInt(unsigned int *output,unsigned char *input,unsigned int len);

#endif /* md_5_h */
