//
//  binary_data.cc
//  Chaos
//
//  Created by CJ0048 on 2014/04/04.
//
//

#include "file/binary_data.h"

CHAOS_BEGIN_NAMESPACE

BinaryData::BinaryData()
:size_(0), data_(0), reference_count_(0), is_reference_(false)
{
}

BinaryData::BinaryData(U32 size):size_(size), data_(0), reference_count_(0), is_reference_(false)
{
  data_ = ADK_Malloc(size, "BinaryData");
}

BinaryData::~BinaryData()
{
  DeleteData();
}

/**\brief
* データを設定します。この場合は、データをコピーします。
*/
void BinaryData::CopyData(const void* data, U32 size)
{
  data_ = (void*)malloc(size+1);
  memcpy(data_, data, size);
  Char* d = (Char*)data_;
  d[size]=0;
  size_ = size;
}

void BinaryData::Retain()
{
  ++reference_count_;
}

void BinaryData::Release()
{
  ADK_ASSERT_MSG(reference_count_, "reference count is zero!");
  
  --reference_count_;
  if(reference_count_ == 0) {
    DeleteData();
    delete this;
  }
}

void BinaryData::DeleteData()
{
  if(data_ && is_reference_==false)
  {
    ADK_Free(data_);
    data_=0;
    size_=0;
  }
}

CHAOS_END_NAMESPACE