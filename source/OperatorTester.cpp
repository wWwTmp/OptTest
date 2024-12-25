#include"OperatorTester.h"

CryptoPP::byte* GenRandomAesCipher(int len) {
    // 创建随机数生成器
    CryptoPP::AutoSeededRandomPool rng;
    CryptoPP::byte * res= new CryptoPP::byte[len];
    // 生成随机字节
    rng.GenerateBlock(res, len);
    return res;
}

// class OperatorTester{
// private:
//     int testTimes;
//     Pairing e;
//     std::vector<G1> G1Args;
//     std::vector<string> strArgs;
//     std::vector<Zr> ZrArgs;
// public:
//     OperatorTester(int times, FILE* paramFile);
//     ~OperatorTester();
//     void TestPointMul();
//     void TestPointAdd();
//     void TestHash();
//     void TestSymmericEnc();
//     void TestEccEnc();
//     void TestEccDec();
// };

string OperatorTester::getRandomString(int len)
{
    static const std::string characters = 
        "0123456789"  // 数字
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"  // 大写字母
        "abcdefghijklmnopqrstuvwxyz";
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(0, characters.size() - 1);
    std::string randomString;
    for (int i = 0; i < len;i++){
        randomString += characters[distribution(generator)];
    }
    return randomString;
}

void OperatorTester::PointMul(int i)
{
    auto res = G1Args[i] ^ ZrArgs[i];
}

void OperatorTester::PointAdd(int i)
{
    auto res = G1Args[i] * G1Args[(i + 1) % testTimes];
}

void OperatorTester::Hash(int i)
{
    CryptoPP::SHA256 hash;
    std::string digest;
    CryptoPP::StringSource(strArgs[i], true,
                           new CryptoPP::HashFilter(hash,
                                                    new CryptoPP::HexEncoder(new CryptoPP::StringSink(digest))));
}

void OperatorTester::SymEnc(int i)
{
    using namespace CryptoPP;
    CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption enc;
    enc.SetKeyWithIV(key, sizeof(key), iv);
    CryptoPP::byte* cipher = new CryptoPP::byte[2048];
    StringSource ss1(byteBufferArgs[i], true,
                     new StreamTransformationFilter(enc,
                                                    new ArraySink(cipher,2048)));
    AesCiphers.emplace_back(cipher);
}

void OperatorTester::SymDec(int i)
{
    using namespace CryptoPP;
    CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption dec;
    dec.SetKeyWithIV(key, sizeof(key), iv);
    string plaintex;
    StringSource ss1(AesCiphers[i], true,
                     new StreamTransformationFilter(dec,
                                                    new StringSink(plaintex)));
}

void OperatorTester::RsaEnc(int i)
{
    using namespace CryptoPP;
    AutoSeededRandomPool rng;
    //rsa用2048位的密钥，密文长度位256字节
    CryptoPP::byte *cipher = new CryptoPP::byte[2048];
    RSAES_OAEP_SHA_Encryptor encryptor(pk);
    StringSource ss1(byteBufferArgs[i], true,
                     new PK_EncryptorFilter(rng, encryptor,
                                            new ArraySink(cipher,2048)));
    RsaCiphers.emplace_back(cipher);
}

void OperatorTester::RsaDec(int i)
{
    using namespace CryptoPP;
    AutoSeededRandomPool rng;
    string decTex;
    RSAES_OAEP_SHA_Decryptor decryptor(sk);
    StringSource ss2(RsaCiphers[i], true,
        new PK_DecryptorFilter(rng, decryptor,
            new StringSink(decTex)
        )
    );
}

void OperatorTester::EccPairing(int i)
{
    auto res=e(G1Args[i], G1Args[(i + 1) % testTimes]);
}

auto OperatorTester::TestAlg(std::function<void(int)> f)
    {
        auto start = chrono::high_resolution_clock::now();
        for (int i = 0; i < testTimes; i++)
        {
            f(i);
        }
        auto end = chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;
        return duration.count() / testTimes;
    }

OperatorTester::OperatorTester(int times, FILE *paramFile)
    : e(paramFile), testTimes(times)
{
    for (int i = 0; i < times;i++){
        G1Args.emplace_back(e, false);
        ZrArgs.emplace_back(e, false);
        strArgs.emplace_back(getRandomString(128));
        byteBufferArgs.emplace_back(GenRandomAesCipher(2048));
    }
    //AES key
    CryptoPP::AutoSeededRandomPool rng;
    rng.GenerateBlock(key, sizeof(key));
    rng.GenerateBlock(iv, sizeof(iv));
    //RSA key
    CryptoPP::InvertibleRSAFunction params;
    params.GenerateRandomWithKeySize(rng, 2048);
    sk = params;
    pk = params;
}

OperatorTester::~OperatorTester()
{
    for (int i = 0; i < testTimes;i++){
        delete[] byteBufferArgs[i];
    }
}

void OperatorTester::run()
{
    vector < function<void(int)>> testArr;
    vector<string> optName = {"PointMul", "PointAdd", "EccPairing", "Hash", "SymEnc", "SymDec", "RsaEnc", "RsaDec"};
    testArr.emplace_back(bind(&OperatorTester::PointMul, this, placeholders::_1));
    testArr.emplace_back(bind(&OperatorTester::PointAdd,this,placeholders::_1));
    testArr.emplace_back(bind(&OperatorTester::EccPairing,this,placeholders::_1));
    testArr.emplace_back(bind(&OperatorTester::Hash,this,placeholders::_1));
    testArr.emplace_back(bind(&OperatorTester::SymEnc,this,placeholders::_1));
    testArr.emplace_back(bind(&OperatorTester::SymDec,this,placeholders::_1));
    testArr.emplace_back(bind(&OperatorTester::RsaEnc,this,placeholders::_1));
    testArr.emplace_back(bind(&OperatorTester::RsaDec,this,placeholders::_1));
    for (int i = 0; i < testArr.size();i++){
        cout<<"Opt "<<optName[i]<<"  "<<" Time cost="<<TestAlg(testArr[i])<<"ms"<<endl;
    }
}
