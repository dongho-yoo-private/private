//
//  binary_data.h
//  Chaos
//
//  Created by CJ0048 on 2014/04/04.
//
//

#ifndef __Chaos__BinaryData__
#define __Chaos__BinaryData__

#include <core/types.h>
#include "application/application_manager.h"

CHAOS_BEGIN_NAMESPACE

class BinaryData{
 public:
  /**
   * \~english
   * \~japanese
   *   \brief デフォルトコンストラクタ
   */
  BinaryData();
  /**
   * \~english
   * \~japanese
   *   \brief コンストラクタ
   *   \param size [in] データサイズを設定します
   */
  BinaryData(U32 size);
  /**
   * \~english
   * \~japanese
   *   \brief デストラクタ
   */
  virtual ~BinaryData();
  
  /**
   * \~english
   * \~japanese
   *   \brief データサイズを取得します。
   *   \return データサイズ
   */
  U32 GetSize() const
  {
    return size_;
  }
  
  /**
   * \~english
   * \~japanese
   *   \brief データサイズを設定します。
   *   \param size [in] データサイズ
   */
  ADK_INLINE void SetSize(U32 size)
  {
    size_ = size;
  }
  
  /**
   * \~english
   * \~japanese
   *   \brief データへのポインタを取得します。
   *   \return データへのポインタ
   */
  ADK_INLINE const void* GetData() const
  {
    return data_;
  }
  /**
   * \~english
   * \~japanese
   *   \brief データへのポインタを取得します。
   *   \return データへのポインタ
   */
  ADK_INLINE void* GetBuffer()
  {
    return data_;
  }
  
  /**
   * \~english
   * \~japanese
   *   \brief データを設定します。この場合は、データを参照するのみです。\brief
   *   このメソッドで設定されたデータの削除は行いません。
   *   \param data [in] データへのポインタ
   *   \param size [in] データサイズ
   */
   ADK_INLINE void SetData(void* data, U32 size)
   {
      ADK_ASSERT(!data_);
      data_=data;
      size_ = size;
      is_reference_=true;
   }

  /**
   * \~english
   * \~japanese
   *   \brief データをコピーします。
   *   \param data [in] データへのポインタ
   *   \param size [in] データサイズ
   */
  void CopyData(const void* data, U32 size);
 
  /**
   * \~english
   * \~japanese
   *   \brief レファレンスカウンタを１加算します。\n
   * Retainが呼び出される事により、Releaseが呼び出されるまで、データが保証されます。\n
   * Retain/Releaseを使わない場合は、new/deleteを使ってください。
   */
  void Retain();
  /**
   * \~english
   * \~japanese
   *   \brief レファレンスカウンタを１減算し、リファレンスカウンタが0の場合はデータを解放します。\n
   * Retainが呼び出された際には必ず、deleteではなく、Releaseで解放してください。
   */
  void Release();
 
 private:
  /** \brief データサイズ　*/
  U32 size_;
  
  /** \brief データへのポインタ　*/
  void* data_;
  
  /** \brief リファレンスカウンタ */
  S32 reference_count_;
 
  /** \brief 内部的に参照しているかどうか*/
  Bool is_reference_;
  
  /**
  * \~english
  * \~japanese
  *   \brief データを解放します。
  */
  void DeleteData();
  

};

CHAOS_END_NAMESPACE

#endif /* defined(__Chaos__BinaryData__) */
