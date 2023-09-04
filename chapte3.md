# 第三章 “张江壹号”的全同态计算库——Poseidon

“张江”支持主流算数逻辑运算的全同态加密算法，包括 CKKS、BFV、BGV。以下内容分别对每种算法的用户编程接口函数进行描述。

## 1 CKKS

### 1.1 参数数据结构

支持的参数数据结构包括：
| 序号 | 类名  | 参数  |
|---|---|---|
| 1  | 内存地址管理类  | MemoryPool  |
| 2  |  CKKS加密方案的参数类 | CKKSParametersLiteralDefault  |
| 3  |  上下文信息管理类 | PoseidonContext  |
| 4  |  生成伪随机数的类 | Blake2xbPRNGFactory  |
| 5  | 明文类  |  Plaintext |
|  6 | 密文类  | Ciphertext  |
|   7| 公钥类  |  PublicKey |
|  8 | 重现性化密钥类  | RelinKeys  |
|  9 |  伽罗瓦密钥类 | GaloisKeys  |
|10  | CKKS 加密方案的编解码类  | CKKSEncoder  |
|  11 |表示明文矩阵的类   |  MatrixPlain |
|   12|密钥生成类   |  KeyGenerator |
|   13| 加密类  |  Encryptor |
|14   | 解密类  |  Decryptor |
|15   |  运算库类 | EnvaluatorFactory  |

<table>
  <tbody>
    <tr> <th colspan = "2">
    <font color="Red">
      MemoryPool
    </font>
      (DegreeType degree_type);
    </th></tr>

  <tr>
    <th>
      描述
    </th>
    <td>
      CKKSParametersLiteralDefault 是一个用于初始化加密参数的类。
    </td>
  </tr>

  <tr>
    <th>
      参数
    </th>
    <td>
      degree_type：DegreeType 类型，表示多项式的次数，可选项有 degree_2048,degree_4096，degree_8192，degree_16384，degree_32768。
    </td>
  </tr>
  <tr>
    <th>
      成员  函数
    </th>
    <td>
      getInstance(DegreeType degree_type)：用于创建 Memorypool 的类。
    </td>
  </tr>

 </tbody> 
</table>


<table>
  <tbody>
    <tr> <th colspan = "2">
    <font color="Red">
      CKKSParametersLiteralDefault
    </font>
      (DegreeType degree_type);<br>
    <font color="Red">
      CKKSParametersLiteralDefault
    </font>
 (SchemeType Type, uint32_t LogN, uint32_t
LogSlots, const vector<uint32_t>& LogQ, const vector<uint32_t>& LogP,uint32_t LogScale, uint32_t H,uint32_t T = 0,int q0_level = 0);
    </th></tr>

  <tr>
    <th>
      描述
    </th>
    <td>
      MemoryPool 是一个用于管理地址和内存空间的类。
    </td>
  </tr>

  <tr>
    <th>
      参数
    </th>
    <td>
      degreeType：DegreeType 类型，表示多项式的次数。

Type: SchemeType 类型，表示加密方案的类型。

LogN: uint32_t 类型，表示环度的对数。

LogSlots: uint32_t 类型，表示槽位的对数。

LogQ: vector<uint32_t> 类型，表示密文素数模数的对数。

LogP: vector<uint32_t> 类型，表示密钥切换辅助素数模数的对数。

LogScale: uint32_t 类型，表示缩放因子的对数。

H: uint32_t 类型，表示 Hamming 权重。


T: uint32_t 类型，表示明文模数。

q0_level: int 类型，表示 q0 的级别。
    </td>
  </tr>
 <tr>
    <th>
      成员  函数
    </th>
    <td>
      成员函数仅有构造函数。
    </td>
  </tr>

 </tbody> 
</table>




MemoryPool (DegreeType degree\_type);

描述

参数

MemoryPool 是一个用于管理地址和内存空间的类。

degree\_type：DegreeType 类型，表示多项式的次数，可选项有 degree\_2048，

degree\_4096，degree\_8192，degree\_16384，degree\_32768。

成员函数 getInstance(DegreeType degree\_type)：用于创建 Memorypool 的类。

CKKSParametersLiteralDefault(DegreeType degreeType);

CKKSParametersLiteralDefault (SchemeType Type, uint32\_t LogN, uint32\_t

LogSlots, const vector<uint32\_t>& LogQ, const vector<uint32\_t>& LogP,

uint32\_t LogScale, uint32\_t H,uint32\_t T = 0,int q0\_level = 0);

描述

参数

CKKSParametersLiteralDefault 是一个用于初始化加密参数的类。

degreeType：DegreeType 类型，表示多项式的次数。

Type: SchemeType 类型，表示加密方案的类型。

LogN: uint32\_t 类型，表示环度的对数。

LogSlots: uint32\_t 类型，表示槽位的对数。

LogQ: vector<uint32\_t> 类型，表示密文素数模数的对数。

LogP: vector<uint32\_t> 类型，表示密钥切换辅助素数模数的对数。

LogScale: uint32\_t 类型，表示缩放因子的对数。

H: uint32\_t 类型，表示 Hamming 权重。



<a name="br9"></a> 

T: uint32\_t 类型，表示明文模数。

q0\_level: int 类型，表示 q0 的级别。

成员函数 成员函数仅有构造函数。

PoseidonContext(const ParametersLiteral& paramLiteral);

描述

参数

PoseidonContext 是一个用于生成并管理上下文信息的类。

paramLiteral：ParametersLiteral 类型，表示用于计算上下文信息的加密参数。

成员函数 const uint32\_t poly\_degree()：返回多项式次数。

const mpf\_class scaling\_factor() const：返回 scaling\_factor。

void set\_random\_generator

(std::shared\_ptr<UniformRandomGeneratorFactory>

random\_generator)：用于设置随机数生成器。

Blake2xbPRNGFactory();

描述

参数

Blake2xbPRNGFactory 是一个用于生成伪随机数的类。

无

成员函数 仅有构造函数。

Plaintext();

描述

参数

Plaintext 是一个用于存储明文信息的类。

无

成员函数 RNSPolynomial\* poly() const: 用于获取多项式指针。

MetaData\* metaData() const: 用于获取元数据指针的函数。

int newMetaData(const mpf\_class &scaling\_factor,bool isNTT,int level,int



<a name="br10"></a> 

poly\_degree): 用于创建新的元数据的函数。

int newPoly(const PoseidonContext& context,int level): 用于创建新的多项

式的函数。

Ciphertext ();

描述

参数

Ciphertext 是一个用于存储密文信息的类。

无

成员函数 void newPoly(const PoseidonContext& context,int rns\_num\_q,int

rns\_num\_p): 用于创建新的多项式的函数。

void newMetaData(const mpf\_class &scaling\_factor,bool isNTT,int

level,int poly\_degree): 用于创建新的元数据的函数。

bool isValid() const: 用于检查密文多项式是否有效。

RNSPolynomial\* c0() const: 用于获取第一部分多项式指针的函数。

RNSPolynomial\* c1() const: 用于获取第二部分多项式指针的函数。

MetaData\* metaData() const: 用于获取元数据指针的函数。

PublicKey ();

描述

参数

PublicKey 是一个用于存储公钥信息的类。

无

成员函数 inline Ciphertext data() const noexcept: 用于获取密钥的函数。

RelinKeys ();

描述

参数

RelinKeys 是一个用于存储重现性化密钥信息的类。

无



<a name="br11"></a> 

成员函数 inline std::size\_t size() const noexcept: 用于获取密钥数量的函数。

inline auto &data() noexcept：用于获取密钥数据地址的函数。

GaloisKeys ();

描述

参数

GaloisKeys 是一个用于存储伽罗瓦密钥信息的类。

无

成员函数 static inline std::size\_t get\_index(std::uint32\_t galois\_elt):用于根据一个伽罗

瓦元素来获取其对应索引的函数。

inline bool has\_key(std::uint32\_t galois\_elt) const:用于判断是否存在给定的

伽罗瓦元素的函数。

MatrixPlain ();

描述

参数

MatrixPlain 是一个用于存储明文矩阵信息的类。

无

成员变量 LogSlots: 表示矩阵元素的数量的 2 的对数。

N1: 表示矩阵的行数。

level: 表示矩阵所在的密文模数链的层级。

scale: 表示矩阵的缩放因子。

rot\_index:表示矩阵元素在多项式中的旋转索引。

plain\_vec:表示矩阵元素对应的多项式。

CKKSEncoder(const PoseidonContext& params);

描述

参数

CKKSEncoder 是一个用于 CKKS 加密方案编解码的类。

paramLiteral：ParametersLiteral 类型，表示用于计算上下文信息的加密参数。



<a name="br12"></a> 

成员函数 int encode(vector<complex<double>> vec, Plaintext &plain, const

mpf\_class scaling\_factor): 用于将复数向量编码为明文多项式的函数。

int decode(const Plaintext &plain, vector<complex<double>>& vec): 用

于将明文多项式解码为复数向量的函数。

KeyGenerator(const PoseidonContext& params):

描述

参数

KeyGenerator 是一个用于生成密钥的类。

paramLiteral：ParametersLiteral 类型，表示用于计算上下文信息的加密参数。

成员函数 inline void create\_public\_key(PublicKey &destination) const: 用于创建公

钥的函数。

inline void create\_relin\_keys(RelinKeys &destination):用于创建重线性化密

钥的函数。

inline void create\_galois\_keys(const std::vector<int> &steps, GaloisKeys

&destination):用于根据给定的旋转步长向量来创建旋转密钥的函数。

inline void create\_conj\_keys(GaloisKeys &destination): 用于创建共轭旋转

密钥的函数。

Encryptor(const PoseidonContext &context, const SecretKey &secret\_key);

描述

参数

Encryptor 是一个用于对明文加密的类。

paramLiteral：ParametersLiteral 类型，表示用于计算上下文信息的加密参数。

secret\_key：SecretKey 类型，表示参与加密运算的密钥。

成员函数 void encrypt(const Plaintext &plain, Ciphertext &destination) const：用

于对明文加密的函数。



<a name="br13"></a> 

Decryptor (const PoseidonContext &context, const SecretKey &secret\_key);

描述

参数

Decryptor 是一个用于对明文解密的类。

paramLiteral：ParametersLiteral 类型，表示用于计算上下文信息的加密参数。

secret\_key：SecretKey 类型，表示参与加密运算的密钥。

成员函数 void decrypt (const Plaintext &plain, Ciphertext &destination) const：用

于对明文解密的函数。

EnvaluatorFactory();

描述

参数

EnvaluatorFactory 是一个用于创建 Poseidon 算法库的类。

无

成员函数 auto create(const PoseidonContext &context) ->

std::shared\_ptr<Envaluator>: 用于根据给定的 PoseidonContext 对象来创

建一个 Envaluator 对象的函数，可以选择所采用的全同态加密算法如 BFV、

CKKS 以及其对应的硬件或者软件算法库。

具体算法支持详见下一节。

1\.2 API

支持的 API 包括：

1

2

3

4

密文与密文加法

密文与明文加法

密文与密文减法

密文与密文乘法

add

add\_plain

sub

multiply



<a name="br14"></a> 

5

密文明文乘法

重缩放

multiply\_plain

rescale

6

7

密文旋转

rotate

8

求共轭

conjugate

9

密文明文矩阵乘

系数放入明文时隙

明文时隙放入系数

快速傅里叶变换

快速傅里叶逆变换

从加速卡上读取密文信息

密文与复数常数乘

密文与复数常数加

对密文进行离散傅里叶变换

自动重缩放

multiplyByDiagMatrixBSGS

coeff\_to\_slot

slot\_to\_coeff

ftt\_fwd

10

11

12

13

14

15

16

17

18

19

20

21

22

ftt\_inv

read

multiply\_const

add\_const

dft

rescale\_dynamic

evaluatePolyVector

eval\_mod

多项式评估

密文向量模运算

自举

bootstrap

密钥转换

switch\_key

void add( Ciphertext &ciph1, Ciphertext &ciph2,Ciphertext &result) override;

函数描述 函数用于对两个密文进行全同态加法。



<a name="br15"></a> 

参数

ciph1: 一个 Ciphertext 对象的引用，表示一个密文。

ciph2: 一个 Ciphertext 对象的引用，表示另一个密文。

result: 一个 Ciphertext 对象的引用，用于存储计算结果。

void add\_plain( Ciphertext &ciph, Plaintext &plain,Ciphertext &result) override;

函数描述 函数用于对一个密文和一个明文进行全同态加法。

参数

ciph: 一个 Ciphertext 对象的引用，表示一个密文。

plain: 一个 Plaintext 对象的引用，表示一个明文。

result: 一个 Ciphertext 对象的引用，用于存储计算结果。

void sub( Ciphertext &ciph1, Ciphertext &ciph2,Ciphertext &result) override;

函数描述 函数用于对两个密文进行全同态减法。

参数

ciph1: 一个 Ciphertext 对象的引用，表示被减数。

ciph2: 一个 Ciphertext 对象的引用，表示减数。

result: 一个 Ciphertext 对象的引用，用于存储计算结果。

void multiply( Ciphertext &ciph0, Ciphertext &ciph1,Ciphertext &result, const

RelinKeys &relin\_key) override;

函数描述 函数用于对两个密文进行全同态乘法，并使用重线性化密钥来降低密文规模。

参数

ciph0: 一个 Ciphertext 对象的引用，表示一个密文。

ciph1: 一个 Ciphertext 对象的引用，表示另一个密文。

result: 一个 Ciphertext 对象的引用，用于存储计算结果。

relin\_key: 一个 RelinKeys 对象的常量引用，表示重线性化密钥。

void multiply\_plain( Ciphertext &ciph, Plaintext &plain,Ciphertext &result)



<a name="br16"></a> 

override;

函数描述 函数用于对一个密文和一个明文进行全同态乘法。

参数

ciph: 一个 Ciphertext 对象的引用，表示一个密文。

plain: 一个 Plaintext 对象的引用，表示一个明文。

result: 一个 Ciphertext 对象的引用，用于存储计算结果。

void rescale(Ciphertext &ciph) override;

函数描述 函数用于对一个密文进行重缩放操作。

参数

ciph: 一个 Ciphertext 对象的引用，表示要重缩放的密文。

void rotate( Ciphertext &ciph, Ciphertext &result,const GaloisKeys &rot\_key, int

k) override;

函数描述 函数用于对一个密文进行旋转操作。

参数

ciph: 一个 Ciphertext 对象的引用，表示一个密文。

result: 一个 Ciphertext 对象的引用，用于存储旋转后的密文。

rot\_key: 一个 GaloisKeys 对象的常量引用，表示用于旋转的加密密钥。

k: 一个整数，表示旋转的步长，正数表示向右旋转，负数表示向左旋转。

void conjugate( Ciphertext &ciph,Ciphertext &result ,const GaloisKeys

&conj\_key) override;

函数描述 函数用于对一个密文进行共轭操作，即将其每个元素变为其复共轭。

参数

ciph: 一个 Ciphertext 对象的引用，表示要共轭的密文。

result: 一个 Ciphertext 对象的引用，用于存储共轭后的密文。

conj\_key: 一个 GaloisKeys 对象的常量引用，表示用于共轭的加密密钥。



<a name="br17"></a> 

void multiplyByDiagMatrixBSGS( Ciphertext &ciph, MatrixPlain&

plain\_mat,Ciphertext &result,const GaloisKeys &rot\_key) override;

函数描述 函数用于对一个密文和一个明文矩阵进行全同态乘法，使用了 BSGS 算法来加速

旋转操作。

参数

ciph: 一个 Ciphertext 对象的引用，表示一个密文。

plain\_mat: 一个 MatrixPlain 对象的引用，表示一个明文矩阵。

result: 一个 Ciphertext 对象的引用，用于存储计算结果。

rot\_key: 一个 GaloisKeys 对象的常量引用，表示用于旋转的加密密钥。

void coeff\_to\_slot( Ciphertext &ciph, LinearMatrixGroup&

matrix\_group,Ciphertext &result\_real,Ciphertext &result\_imag, const GaloisKeys

&rot\_key,const GaloisKeys &conj\_key,CKKSEncoder &encoder) override;

函数描述 函数用于将一个密文从系数域转换到槽域。

参数

ciph: 一个 Ciphertext 对象的引用，表示要转换的密文。

matrix\_group: 一个 LinearMatrixGroup 对象的引用，表示用于转换的线性矩

阵组。

result\_real: 一个 Ciphertext 对象的引用，用于存储转换后的密文的实部。

result\_imag: 一个 Ciphertext 对象的引用，用于存储转换后的密文的虚部。

rot\_key: 一个 GaloisKeys 对象的常量引用，表示用于旋转的加密密钥。

conj\_key: 一个 GaloisKeys 对象的常量引用，表示用于共轭的加密密钥。

encoder: 一个 CKKSEncoder 对象的引用，表示用于编码和解码的对象。

void slot\_to\_coeff( Ciphertext &ciph\_real, Ciphertext &ciph\_imag,



<a name="br18"></a> 

LinearMatrixGroup& matrix\_group,Ciphertext &result, const GaloisKeys

&rot\_key,const GaloisKeys &conj\_key,CKKSEncoder &encoder) override;

函数描述 函数用于将一个密文从槽域转换到系数域。

参数

ciph\_real: 一个 Ciphertext 对象的引用，表示要转换的密文的实部。

ciph\_imag: 一个 Ciphertext 对象的引用，表示要转换的密文的虚部。

matrix\_group: 一个 LinearMatrixGroup 对象的引用，表示用于转换的线性矩

阵组。

result: 一个 Ciphertext 对象的引用，用于存储转换后的密文。

rot\_key: 一个 GaloisKeys 对象的常量引用，表示用于旋转的加密密钥。

conj\_key: 一个 GaloisKeys 对象的常量引用，表示用于共轭的加密密钥。

encoder: 一个 CKKSEncoder 对象的引用，表示用于编码和解码的对象。

void ftt\_fwd( Plaintext &plain ,Plaintext &result)

void ftt\_fwd( Ciphertext &ciph, Ciphertext &result) override;

函数描述 函数用于对一个明文或密文进行快速傅里叶变换（FFT）。

参数

plain: 一个 Plaintext 对象的引用，表示一个明文。

Ciph：一个 Ciphertext 对象的引用，表示一个密文。

result: 一个 Plaintext 或 Ciphertext 对象的引用，用于存储变换后的明文或密

文。

void ftt\_inv( Plaintext &plain ,Plaintext &result) override;

void ftt\_inv( Ciphertext &ciph ,Ciphertext &result) override;

函数描述 函数用于对一个密文或明文进行快速傅里叶逆变换（IFFT）。



<a name="br19"></a> 

参数

plain: 一个 Plaintext 对象的引用，表示一个明文。

ciph: 一个 Ciphertext 对象的引用，表示一个密文。

result: 一个 Plaintext 或 Ciphertext 对象的引用，用于存储逆变换后的密文或

明文。

void read(Ciphertext &ciph) override;

函数描述 函数用于从加速卡上读取密文中的数据。

参数

ciph: 一个 Ciphertext 对象的引用，表示要读取的密文。

void multiply\_const( Ciphertext &ciph,complex<double> constData,Ciphertext

&result,CKKSEncoder &encoder,bool isDirect = false) override;

函数描述 函数用于对一个密文和一个复数常数进行全同态乘法。

参数

ciph: 一个 Ciphertext 对象的引用，表示一个密文。

constData: 一个 complex<double>对象，表示一个复数常数。

result: 一个 Ciphertext 对象的引用，用于存储计算结果。

encoder: 一个 CKKSEncoder 对象的引用，表示用于编码和解码的对象。

isDirect: 一个布尔值，表示是否直接对密文进行乘法操作，而不是先将常数编

码为明文再进行乘法。

void add\_const( Ciphertext &ciph,double constData,Ciphertext &result)

override;

函数描述 函数用于对一个密文和一个实数常数进行全同态加法。

参数

ciph: 一个 Ciphertext 对象的引用，表示一个密文。

constData: 一个 double 类型的值，表示一个实数常数。



<a name="br20"></a> 

result: 一个 Ciphertext 对象的引用，用于存储计算结果。

void dft( Ciphertext &ciph, LinearMatrixGroup& matrix\_group,Ciphertext

&result,const GaloisKeys &rot\_key) override;

函数描述 函数用于对一个密文进行离散傅里叶变换（DFT）。

参数

ciph: 一个 Ciphertext 对象的引用，表示一个密文。

matrix\_group: 一个 LinearMatrixGroup 对象的引用，表示用于 DFT 的线性矩

阵组。

result: 一个 Ciphertext 对象的引用，用于存储变换后的密文。

rot\_key: 一个 GaloisKeys 对象的常量引用，表示用于旋转的加密密钥。

void rescale\_dynamic(Ciphertext &ciph,mpf\_class scale);

函数描述 函数用于对一个密文进行动态重缩放。

参数

ciph: 一个 Ciphertext 对象的引用，表示一个密文。

scale: 一个 mpf\_class 对象，表示给定的缩放因子。

void evaluatePolyVector( Ciphertext &ciph,Ciphertext &destination,const

PolynomialVector &polys,mpf\_class scalingfactor,const RelinKeys

&relin\_key,CKKSEncoder &encoder) override;

void evaluatePolyVector( Ciphertext &ciph,Ciphertext &destination,const

PolynomialVector &polys,mpf\_class scalingfactor,const RelinKeys

&relin\_key,CKKSEncoder &encoder,Decryptor &dec);

函数描述 多项式评估，预先计算出切比雪夫或者泰勒的多项式系数，然后进行多项式的计

算。



<a name="br21"></a> 

参数

ciph: 一个 Ciphertext 对象的引用，表示一个密文。

destination: 一个 Ciphertext 对象的引用，用于存储评估后的密文。

polys: 一个 PolynomialVector 对象的常量引用，表示给定的多项式向量。

scalingfactor: 一个 mpf\_class 对象，表示给定的缩放因子。

relin\_key: 一个 RelinKeys 对象的常量引用，表示用于重线性化的加密密钥。

encoder: 一个 CKKSEncoder 对象的引用，表示用于编码和解码的编码器。

dec: 一个 Decryptor 对象的引用，表示用于解密的解密器。

void eval\_mod( Ciphertext &ciph,Ciphertext &result,const EvalModPoly

&eva\_poly,const RelinKeys &relin\_key,CKKSEncoder &encoder) override;

函数描述 用于对一个密文进行模运算。

参数

ciph: 一个 Ciphertext 对象的引用，表示一个密文。

result: 一个 Ciphertext 对象的引用，用于存储模运算后的密文。

eva\_poly: 一个 EvalModPoly 对象的常量引用，表示给定的模多项式。

relin\_key: 一个 RelinKeys 对象的常量引用，表示用于重线性化的加密密钥。

encoder: 一个 CKKSEncoder 对象的引用，表示用于编码和解码的编码器。

void bootstrap(Ciphertext &ciph,Ciphertext &result,const EvalModPoly

&eva\_poly,LinearMatrixGroup& matrix\_group0,LinearMatrixGroup

&matrix\_group1, const RelinKeys &relin\_key, const GaloisKeys &rot\_key,const

GaloisKeys &conj\_key,CKKSEncoder &encoder) override;

函数描述 对一个密文进行自举。

参数

ciph: 一个 Ciphertext 对象的引用，表示一个密文。



<a name="br22"></a> 

result: 一个 Ciphertext 对象的引用，用于存储自举后的密文。

eva\_poly: 一个 EvalModPoly 对象的常量引用，表示给定的模多项式。

matrix\_group0: 一个 LinearMatrixGroup 对象的引用，表示用于自举的第一

个线性矩阵组。

matrix\_group1: 一个 LinearMatrixGroup 对象的引用，表示用于自举的第二

个线性矩阵组。

relin\_key: 一个 RelinKeys 对象的常量引用，表示用于重线性化的加密密钥。

rot\_key: 一个 GaloisKeys 对象的常量引用，表示用于旋转的加密密钥。

conj\_key: 一个 GaloisKeys 对象的常量引用，表示用于共轭的加密密钥。

encoder: 一个 CKKSEncoder 对象的引用，表示用于编码和解码的编码器。

void switch\_key(Ciphertext& ciph, Ciphertext &result, const

vector<PublicKey>& switch\_key);

函数描述 对密文进行密钥转换。

参数

ciph: 一个 Ciphertext 对象的引用，表示一个密文。

result: 一个 Ciphertext 对象的引用，用于存储密钥转换后的密文。

switch\_key: 一个 PublicKey 对象的向量的常量引用，表示给定的一组公钥。

2 BFV

2\.1 参数数据结构

支持的参数数据结构包括：



<a name="br23"></a> 

1

内存地址管理类

BFV 加密方案的参数类

上下文信息管理类

生成伪随机数的类

明文类

MemoryPool

BFVParametersLiteralDefault

PoseidonContext

Blake2xbPRNGFactory

Plaintext

2

3

4

5

6

密文类

Ciphertext

7

公钥类

PublicKey

8

重现性化密钥类

伽罗瓦密钥类

BFV 加密方案的编解码类

表示明文矩阵的类

密钥生成类

RelinKeys

9

GaloisKeys

10

11

12

13

14

15

BatchEncoder

MatrixPlain

KeyGenerator

Encryptor

加密类

解密类

Decryptor

运算库类

EnvaluatorFactory

除以下两个数据结构外，其余均与 CKKS 部分一致。

BFVParametersLiteralDefault(DegreeType degreeType);

描述

参数

BFVParametersLiteralDefault 是一个用于初始化加密参数的类。

degreeType：DegreeType 类型，表示多项式的次数。

成员函数 成员函数仅有构造函数。



<a name="br24"></a> 

BatchEncoder(const PoseidonContext& params);

描述

参数

BatchEncoder 是一个用于 BFV 加密方案编解码的类。

paramLiteral：ParametersLiteral 类型，表示用于计算上下文信息的加密参数。

成员函数 int encode(vector<complex<double>> vec, Plaintext &plain, const

mpf\_class scaling\_factor): 用于将复数向量编码为明文多项式的函数。

int decode(const Plaintext &plain, vector<complex<double>>& vec): 用

于将明文多项式解码为复数向量的函数。

2\.2 API

支持的 API 包括：

1

2

3

4

5

6

7

8

9

10

密文与密文加法

密文与明文加法

密文与密文减法

密文与密文乘法

密文明文乘法

重缩放

add

add\_plain

sub

multiply

multiply\_plain

rescale

密文行旋转

rotate\_row

rotate\_col

switch\_key

multiplyByDiagMatrixBSGS

密文列旋转

密钥转换

密文明文矩阵乘

除以下两个函数外，其余函数均与 CKKS 部分一致。



<a name="br25"></a> 

void rotate\_row(Ciphertext &encrypted, int rot\_step, const GaloisKeys

&galois\_keys, Ciphertext &destination);

函数描述 函数用于对一个密文进行行旋转操作。

参数

encrypted: 一个 Ciphertext 对象的引用，表示一个密文。

rot\_step: 一个整数，表示旋转的步长，正数表示向右旋转，负数表示向左旋转。

galois\_keys: 一个 GaloisKeys 对象的常量引用，表示用于行旋转的加密密钥。

destination: 一个 Ciphertext 对象的引用，用于存储旋转后的密文。

void rotate\_col(Ciphertext &encrypted, const GaloisKeys &galois\_keys,

Ciphertext &destination);

函数描述 函数用于对一个密文进行列旋转操作。

参数

encrypted: 一个 Ciphertext 对象的引用，表示一个密文。

galois\_keys: 一个 GaloisKeys 对象的常量引用，表示用于列旋转的加密密钥。

destination: 一个 Ciphertext 对象的引用，用于存储旋转后的密文。

版本更新记录

第一版创建

2022\.12.01

2023\.04.01

2023\.06.01

2023\.07.01

更新 CKKS API

第二版创建

更新 BFV API

