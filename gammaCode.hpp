#ifndef _GAMMACODE_HPP_
#define _GAMMACODE_HPP_

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <stdint.h>
#include <vector>
#include <cassert>

#define _GAMMA_CODE_DEBUG_

class GammaEncoder {
private:
  // Assume docID is represented by unit32_t.
  uint32_t *mBefAddr;  // the address of posting before encoding
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
  GammaEncoder(uint32_t* addr, uint32_t num): mBefAddr(addr), mBefNum(num),
    mAftAddr(NULL), mAftSize(0), mCodes(), mCode(0), mCodeBitn(0), mLength(0), 
    mLenCode(0), mOffCode(0) {
  }
  void vec2Array(std::vector<uint8_t> vec) {
    if (vec.size() == 0) {
      return ;
    }
    mAftAddr = (uint8_t*)malloc(vec.size());
    uint32_t i = 0;
    for (std::vector<uint8_t>::iterator it = vec.begin(),
         ie = vec.end(); it != ie; ++it) {
      mAftAddr[i] = *it;
      i++;
    }
  }
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
      return ;
    }
    int8_t shiftn = mCodeBitn - 8;
    uint64_t mask = 0xFF << shiftn;
    while (mCodeBitn >= 8) {
      assert(shiftn >= 0);
      mCodes.push_back((mCode & mask) >> shiftn);
      mCode &= (~mask);
      mask >>= 8;
      mCodeBitn -= 8;
      shiftn -= 8;
      mAftSize++;
    }
  }
  void encodeForOne(uint32_t value) {
    // make sure mCodeBitn < 8
    assert(mCodeBitn < 8);
    if (value <= 1) {
      mCode <<= 1;
      mCodeBitn += 1;
      if (mCodeBitn >= 8) {
        add2Vec();
      }
    } else {
      mLength = getLength(value);
      mLenCode = (2 << mLength) - 2;
      mOffCode = value ^ (1 << mLength);
#ifdef _GAMMA_CODE_DEBUG_
      printf("%0x(%u) --> %0lx\n", value, value, 
             ((uint64_t)mLenCode << mLength) | mOffCode);
#endif
      mCode <<= mLength;
      mCode <<= 1;
      mCode |= mLenCode;
      mCodeBitn += (mLength + 1);
      if (mCodeBitn >= 8) {
        add2Vec();
      }
      mCode = (mCode << mLength) | mOffCode;
      mCodeBitn += mLength;
      if (mCodeBitn >= 8) {
        add2Vec();
      }
    }
  }
  uint8_t* encode() {
    uint32_t docid;
    if (mBefNum > 0) {
      docid = mBefAddr[0];
      encodeForOne(docid+1);  // first id plus 1
    }
    for (uint32_t oi = 1; oi < mBefNum; ++oi) {
      encodeForOne(mBefAddr[oi] - docid);
      docid = mBefAddr[oi];
    }
    // deal last code byte
    if (mCodeBitn != 0) {
      assert(mCodeBitn < 8);
      int8_t shiftn = 8 - mCodeBitn;
      mCode <<= shiftn;
      mCode |= ((1 << shiftn) - 1);  // fill bit 1
      mCodes.push_back(mCode & 0xFF);
      mAftSize++;
      mCode = 0;
      mCodeBitn = 0;
    }
    assert(mCodes.size() == mAftSize);
    vec2Array(mCodes);
    return mAftAddr;
  }
  uint32_t getCodesSize() {
    return mAftSize;
  }
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
    mAftAddr(NULL), mAftNum(0) {
  }
  void vec2Array(std::vector<uint32_t> vec) {
    if (vec.size() == 0) {
      return ;
    }
    mAftAddr = (uint32_t*)malloc(vec.size() * sizeof(uint32_t));
    uint32_t i = 0;
    for (std::vector<uint32_t>::iterator it = vec.begin(),
         ie = vec.end(); it != ie; ++it) {
      mAftAddr[i] = *it;
      i++;
    }
  }
  uint32_t* decode() {
    uint8_t length = 0;
    uint32_t offset = 0;
    bool isOffset = false;
    bool first_value = true;
    uint8_t code;
    uint32_t pre_value = 0;
    uint32_t value_interval;
    for (uint32_t oi = 0; oi < mBefSize; oi++) {
      code = mBefAddr[oi];
      for (int8_t ci = 0; ci < 8; ci++) {
        if (!isOffset) {
          if (code & 0x80) {
            length++;
          } else {
            value_interval = 1;
            isOffset = true;
          }
          code <<= 1;
        } else {
          if (length == 0) {
            value_interval = 1;
            if (first_value == true) {  // first value of the posting
              first_value = false;
              pre_value = 0;
            } else {
              pre_value += 1;
            }
            mValues.push_back(pre_value);
            mAftNum++;
            isOffset = false;
            ci--;
          } else {
            value_interval <<= 1;
            value_interval |= ((code & 0x80) >> 7);
            length--;
            if (length == 0) {
              isOffset = false;
              pre_value += value_interval;
              if (first_value) {
                first_value = false;
                pre_value--;
              }
              mValues.push_back(pre_value);
              mAftNum++;
            }
            code <<= 1;
          }
        }
      }
    }
    assert(mValues.size() == mAftNum);
    vec2Array(mValues);
    return mAftAddr;
  }
  uint32_t getValuesNum() {
    return mAftNum;
  }
};
#endif
