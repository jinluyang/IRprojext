#include "gammaCode.hpp"

void GammaEncoder::vec2Array(std::vector<uint8_t> vec) {
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

void GammaEncoder::encodeForOne(uint32_t value) {
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

uint8_t* GammaEncoder::encode() {
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

uint32_t GammaEncoder::getCodesSize() {
  return mAftSize;
}

void GammaDecoder::vec2Array(std::vector<uint32_t> vec) {
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

uint32_t* GammaDecoder::decode() {
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
  // deal with last interval(if it is 1)
  if (isOffset) {
    mValues.push_back(pre_value+1);
    mAftNum++;
  }
  assert(mValues.size() == mAftNum);
  vec2Array(mValues);
  return mAftAddr;
}

uint32_t GammaDecoder::getValuesNum() {
  return mAftNum;
}
