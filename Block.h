#include <cstdint>
#include <iostream>

using namespace std;

class Block
{

public:
     string prevHash;
     Block(u_int32_t _nIndexIn, const string &sDataIn);

     string Gethash();

     void MineBlock(uint32_t nDifficulty);

private:
     uint32_t nIndex;
     int64_t _nNonce;
     string _sData;
     string _sHash;
     time_t _tTime;

     string _CalculateHash() const;
};
