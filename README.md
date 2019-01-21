# MD5
md5算法的c实现

## 算法原理概述

MD5算法是一种输入任意不定长度的信息，通过求余、取余、调整长度、与链接变量进行循环运算，最终得出固定长度128-bits的散列值。这是一种广泛使用的Hash算法，用于确保信息传输的完整性和一致性。从MD5算法发明至今，还未真正找到两个不同的消息拥有相同的MD5Hash值。

## 总体结构

**MD5**算法的基本过程是填充（padding）、分块、缓冲区初始化、循环压缩（H<sub>MD5</sub>)、得出结果；

### 填充:

 这一部分是在长度为 <font color=red>K</font> bits 的原始消息数据尾部填充长度为 <font color=red>P</font> bits 的标识100...0，$1<=P<=512$ (即至少要填充1个bit)，使得填充后的消息位数为：<font color=red>K</font> + <font color = red>P</font> $\equiv 448(mod \ 512)$ ；之后，再向上述填充好的消息尾部附加<font color=red>K</font>值的低64位(即<font color=red>K</font> $mod$ 2<sup>64</sup>)，最后得到一个长度位数为 <font color=red>K</font> + <font color = red>P</font> + 64 $\equiv0\ (mod\ 512)$的消息。

### 分块:

 这部分是将填充后的消息结果分割为 ***<font color=blue>L</font>*** 个 **512-bit**分组：***<font color=blue>Y<sub>0</sub>, Y<sub>1</sub>, ..., Y<sub>L-1</sub></font>*** 。

### 初始化:

 初始化一个**128-bit**的**MD**缓冲区，记为表示成4个32-bit寄存器（A，B，C，D)；**<font color=red>CV<sub>0</sub></font> = <font color=red>IV</font>**。迭代在**MD**缓冲区进行，最后一步的**128-bit**输出即为算法结果。

寄存器（**<font color=red>A</font>，<font color=red>B</font>，<font color=red>C</font>，<font color=red>D</font>**)置16进制初值作为初始向量**<font color=red>IV</font>**，并采用小端存储的存储结构.

### 循环压缩(H<sub>MD5</sub>):

- **<font color=blue>H<sub>MD5</sub></font>** 从 **<font color=red>CV</font>** 输入128位，从消息分组输入512位，完成4大轮循环后，输出128位，用于下一轮输入的**<font color=red>CV</font>**值；

- 每轮循环分别固定不同的生成函数 F, G, H, I，这四个函数是32位非线性逻辑函数，再结合指定的 T 表元素 T[] 和消息分组的不同部分 X[] 做16次迭代运算，生成下一轮循环的输入；

- 4轮循环总共有64次迭代运算；

- 四轮压缩函数：

  轮函数**g**是32位的非线性逻辑函数

- 迭代运算：

  每轮循环中的一次迭代运算逻辑：

  对**<font color=red>A</font>**迭代： **<font color=red>a</font> $\longleftarrow$ <font color=red>b</font> + ((<font color=red>a</font> + <font color=blue>g</font>(<font color=red>b</font>,<font color=red>c</font>,<font color=red>d</font>) + <font color=blue>X[k]</font> + <font color=red>T[i]</font> <<< <font color=red>s</font>)*** 

​	缓冲区**（<font color=red>B</font>，<font color=red>C</font>，<font color=red>D</font>，<font color=red>A</font>）$\longleftarrow$ (<font color=red>A</font>，<font color=red>B</font>，<font color=red>C</font>，<font color=red>D</font>)** 

​	◌  <font color=red>a, b, c, d</font>: MD缓冲区 (<font color=red>A</font>，<font color=red>B</font>，<font color=red>C</font>，<font color=red>D</font>)的当前值。 

​	◌  <font color=blue>g</font> :轮函数(<font color=blue>F</font>,<font color=blue>G</font>,<font color=blue>H</font>,<font color=blue>I</font>中的一个)。 

​	◌  <<<<font color=red>s</font>:将32位输入循环左移(CLS)s位。 

​	◌  <font color=blue>X[k]</font> : 当前处理消息分组的第 <font color=blue>k</font> 个 (<font color=blue>k</font> = 0..15) 32位字，即 <font color=blue>M<sub>q*16+k</sub></font>。 

​	◌  <font color=red>T[i]</font> : <font color=red>T</font> 表的第 <font color=red>i</font> 个元素，32位字;<font color=red>T</font> 表总共有64个元素，也称为加法常数。 

​	**各轮循环中第<font color=red>i</font>次迭代（<font color=red>i</font> = 1...16）使用的<font color=blue>X[k]</font>的确定：**

​		设***<font color=red>j</font> = <font color=red>i</font>*** - 1:

​		◌ 第一轮迭代：***<font color=blue>k</font> = <font color=red>j</font>***.

​			• 顺序使用 ***<font color=blue>X</font>[0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15]***

​		◌ 第2轮迭代:***<font color=blue>k</font> = (1 + 5<font color=red>j</font>) mod 16***.
​			 • 顺序使用 ***<font color=blue>X</font>[1, 6,11, 0, 5,10,15, 4, 9,14, 3, 8,13, 2, 7,12]***

​		◌ 第3轮迭代:***<font color=blue>k</font> = (5 + 3<font color=red>j</font>) mod 16***.
​			 • 顺序使用 ***<font color=blue>X</font>[5, 8,11,14, 1, 4, 7,10,13, 0, 3, 6, 9,12,15, 2]***

​		◌ 第4轮迭代:***<font color=blue>k</font> = 7<font color=red>j</font> mod 16***.
​			 • 顺序使用 ***<font color=blue>X</font>[0, 7,14, 5,12, 3,10, 1, 8,15, 6,13, 4,11, 2, 9]***

​	**各轮循环中使用的T值：**
​		 ◌ ***<font color=red>T[i]</font>= int(2<sup>32</sup>x|sin(<font color=red>i</font>)|)*** 

​			• int取整函数，sin正弦函数，以<font color=red>i</font>作为弧度输入

​		<font color=red>T</font>[ 1.. 4] = { 0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee }

​		<font color=red>T</font>[ 5.. 8] = { 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501 }

​		<font color=red>T</font>[ 9..12] = { 0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be }

​		<font color=red>T</font>[13..16] = { 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821 }

​		<font color=red>T</font>[17..20] = { 0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa }

​		<font color=red>T</font>[21..24] = { 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8 }

​		<font color=red>T</font>[25..28] = { 0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed }

​		<font color=red>T</font>[29..32] = { 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a }	

​		<font color=red>T</font>[33..36] = { 0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c }

​		<font color=red>T</font>[37..40] = { 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70 }

​		<font color=red>T</font>[41..44] = { 0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05 }

​		<font color=red>T</font>[45..48] = { 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665 }

​		<font color=red>T</font>[49..52] = { 0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039 }

​		<font color=red>T</font>[53..56] = { 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1 }

​		<font color=red>T</font>[57..60] = { 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1 }

​		<font color=red>T</font>[61..64] = { 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 }

​	**各轮迭代运算采用的做循环移位的s值：**

​		<font color=red>s</font>[ 1..16] = { 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22 }

​		<font color=red>s</font>[17..32] = { 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20 }

​		<font color=red>s</font>[33..48] = { 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23 }

​		<font color=red>s</font>[49..64] = { 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21 }	

### 得出结果:

完成以上所有操作后，输出A，B，C，D的级联。
