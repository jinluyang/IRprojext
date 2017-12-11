#ifndef _GAMMACODE_HPP_
#define _GAMMACODE_HPP_

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <stdint.h>
#include <vector>
#include <cassert>

//#define _GAMMA_CODE_DEBUG_

class GammaEncoder {
private:
  // Assume docID is represented by unit32_t.
  const uint32_t *mBefAddr;  // the address of posting before encoding
  uint8_t  *mAftAddr;  // the address of posting after encoding
  uint32_t  mBefNum;   // the number of docIDs of posting before encoding
  uint32_t  mAftSize;  // the number of bytes of posting after encoding
  std::vector<uint8_t> mCodes;  // store gamma codes of posting temporarily
  uint64_t mCode;      // store gamma code within 1 byte
  uint8_t mCodeBitn;   // bit number of code
  uint8_t mLength;
  uint32_t mLenCode;   // unary code of length
  uint32_t mOffCode;
public:
  GammaEncoder(const uint32_t* addr, uint32_t num): mBefAddr(addr), mBefNum(num),
    mAftAddr(NULL), mAftSize(0), mCodes(), mCode(0), mCodeBitn(0), mLength(0), 
    mLenCode(0), mOffCode(0) {
  }

  void vec2Array(std::vector<uint8_t> vec);

  inline uint8_t getLength(uint32_t value) {
    assert(value >= 1);
    uint8_t ret = 0;
    value >>= 1;
    while (value > 0) {
      ret += 1;
      value >>= 1;
    }
    return ret;
  }
  inline void add2Vec() {
    assert(mCodeBitn >= 8);
    if (mCodeBitn == 8) {
      mCodes.push_back(mCode & 0xFF);
      mAftSize++;
      mCodeBitn -= 8;
      return ;
    }
    int8_t shiftn = mCodeBitn - 8;
    uint64_t mask = 0xFF << shiftn;
    while (mCodeBitn >= 8) {
      assert(shiftn >= 0);
      mCodes.push_back((uint8_t)((mCode & mask) >> shiftn));
      mCode &= (~mask);
      mask >>= 8;
      mCodeBitn -= 8;
      shiftn -= 8;
      mAftSize++;
    }
  }
  void encodeForOne(uint32_t value);

  uint8_t* encode();

  uint32_t getCodesSize();
};

class GammaDecoder {
private:
  // Assume docID is represented by uint32_t.
  uint8_t *mBefAddr;   // the address of posting before decoding
  uint32_t *mAftAddr;  // the address of posting after encoding
  int32_t mBefSize;    // the number of bytes of posting before decoding
  uint32_t mAftNum;    // the number of docIDs of posting after decoding
  std::vector<uint32_t> mValues;  // store values of gamma codes temporarily
public:
  GammaDecoder(uint8_t* addr, uint32_t size): mBefAddr(addr), mBefSize(size),
    mAftAddr(NULL), mAftNum(0), mValues() {
  }
  void vec2Array(std::vector<uint32_t> vec);

  uint32_t* decode();

  uint32_t getValuesNum();
};
#endif
