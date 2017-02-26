/**
 * \file lib_file_types.h
 * \ingroup adkgrp_library
 * \~english
 * \~japanese
 *   \brief ファイル IO 定義群
 */


#ifndef LIB_FILE_TYPES_H_
#define LIB_FILE_TYPES_H_


#include <sys/stat.h>

#include <core/types.h>


ADK_BEGIN_NAMESPACE


/* stats */
/* stat mode */
#define LIB_TFileStatusIsReg  (0x01)  /* 通常のファイル */
#define LIB_TFileStatusIsDir  (0x02)  /* ディレクトリ */

/**
 * \~english
 * \~japanese
 *   ファイル/ディレクトリのステータス
 */
typedef struct {
  /**
   * \~english
   * \~japanese
   *   ファイルの種類とアクセス保護モード
   */
  S32  mode;
  /**
   * \~english
   * \~japanese
   *   バイト単位のサイズ
   */
  U64  size;
}LIB_TFileStatus;


/* directories */
/**
 * \~english
 * \~japanese
 *   ディレクトリエントリ
 */
typedef struct _LIB_TFileDirEntry  FILE_DirEntry;


/* files */
#if defined(WITH_WIN32) || defined(WITH_IOS) || defined(WITH_COCOA) || defined(WITH_CARBON) || defined(WITH_X11) || defined(WITH_ANDROID)
  /**
   * \~english
   * \~japanese
   *   ファイル名の最大文字数(null 含まず)
   */
  #define FILE_NAME_MAX_LEN  (512)
  #define FILE_PATH_MAX_LEN  (512)
#else
  /**
   * \~english
   * \~japanese
   *   ファイル名の最大文字数(null 含まず)
   */
  #define FILE_FILE_NAME_MAX_LEN  (128)
  #define FILE_FILE_PATH_MAX_LEN  (512)
#endif
/**
 * \~english
 * \~japanese
 *   ファイル拡張子の最大文字数(null 含まず)
 */
#define FILE_FILE_EXT_MAX_LEN  (3)

/* file type */
/**
 * \~english
 * \~japanese
 *   ファイルタイプ
 */
typedef S32  LIB_TFileType;
static const LIB_TFileType  LIB_kFileTypeRead = 0x01;
static const LIB_TFileType  LIB_kFileTypeWrite = 0x02;
static const LIB_TFileType  LIB_kFileTypeAppend = 0x04;

/**
 * \~english
 * \~japanese
 *   ファイル情報
 */
typedef struct _LIB_TFileObject  LIB_TFileObject;

/**
 * \~english
 * \~japanese
 *  認証済みのファイル情報
 */
typedef struct _LIB_TFileKey    LIB_TFileKey;

/**
 * シークをどこから行うか
 */
typedef enum {
  LIB_kFileSet,
  LIB_kFileCur,
  LIB_kFileEnd
}LIB_TFileWhence;

/**
 * \~english
 * \~japanese
 *   \brief ディレクトリエントリ
 */
typedef struct LIB_TDirEntry {
  mode_t type;
  Char *relname;
  Char *path;
  struct stat s;
  U32 flags;
}LIB_TDirEntry;

/**
 * \~english
 * \~japanese
 *   \brief ディレクトリリンク
 */
typedef struct LIB_TDirLink {
  struct LIB_TDirLink  *next,
                       *prev;
  Char *name;
}LIB_TDirLink;


ADK_END_NAMESPACE


#endif  /* LIB_FILE_TYPES_H_ */


