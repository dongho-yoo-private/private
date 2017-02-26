/** 
 * Copyright 2014 Applibot Inc.
 *
 * Author: Dongho.Yoo.
*/

#ifndef _error_handler_types_h_
#define _error_handler_types_h_

/**\brief key for json */
#define KEY_SATUS "status"


/**\brief macros */
#define SERVER_STATUS_SUCCESS (200)
#define MAX_STR_ERROR_DETAIL_LEN (255)


/** \brief WAS側の共通のエラーコード*/
typedef enum {
    kNO_ERROR = 0,
    /**\breif メンテナンス中です。*/
    kALL_MAINTENANCE_CODE             = 9999,
    /**\breif 機能制限中です。*/
    kRESTRICTION_FUNCTION_CODE        = 9998,
    /**\breif 新しいバージョンがあります。*/
    kUPDATE_CODE                      = 9990,
    /**\breif 新しいバージョンがあります。*/
    kAPP_UPDATE_FORCE_CODE            = 9980,
    /**\breif */
    kAPP_UPDATE_OPTIONAL_CODE         = 9979,
    /**\breif */
    kACCOUNT_STATE_FREEZE_LV2_CODE    = 9969,
    /**\breif */
    kACCOUNT_STATE_FREEZE_LV1_CODE    = 9968,
    /**\breif */
    kACCOUNT_STATE_WARNING_CODE       = 9967,
    /**\breif 論理的なアプリケーションエラー*/
    kAPP_COMMON_ERROR_CODE            = 9960
} TApplicationErrorCode;

typedef enum {
  /**\brief エラーなし*/
  kSystemErrorSuccess = 0,
  /**\brief 通信エラー*/
  kSystemErrorCodeNetwork,
  /**\brief アプリケーション共通エラー*/
  kSystemErrorCodeApplicationCommon,
  /**\brief アプリケーションエラー*/
  kSystemErrorCodeApplication,
  /**\brief システムエラー*/
  kSystemErrorCodeSystem

} TSystemErrorCode;


/**\brief エラー情報*/
typedef struct {
  /**\brief システムエラーコード*/
  TSystemErrorCode system_error_code;
  /**\brief システムエラーコードがkSystemErrorCodeApplicationの場合は以下にエラーコードが入ります。*/
  TApplicationErrorCode application_error_code;
  /**\brief サーバからの文字列のエラー詳細*/
  Char error_detail[MAX_STR_ERROR_DETAIL_LEN+1];
} TErrorInfo;

/**\brief エラー情報*/
typedef struct {
  /**\brief システムエラーコードがkSystemErrorCodeApplicationの場合は以下にエラーコードが入ります。*/
  TApplicationErrorCode application_error_code;
  /**\brief サーバからの文字列のエラー詳細*/
  Char error_detail[MAX_STR_ERROR_DETAIL_LEN+1];
  /**\brief エラーの付加情報*/
  S32 extra_param;
} TApplicationErrorInfo;


#endif
