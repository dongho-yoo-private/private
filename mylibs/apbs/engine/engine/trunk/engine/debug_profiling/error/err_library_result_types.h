/**
 * \file err_library_result_types.h
 * \~english
 * \~japanese
 *  \brief ライブラリリザルト定義部
 */


#ifndef ERR_LIBRARY_RESULT_TYPES_H__
#define ERR_LIBRARY_RESULT_TYPES_H__


typedef enum {
  kLibBase64,
  kLibDevice,
  kLibFileOperations,
  kLibFileTypes,
  kLibIObject,
  kLibJsonDocument,
  kLibJsonValue,
  kLibList,
  kLibObjectList,
  kLibPathUtil,
  kLibProfile,
  kLibStack,
  kLibString,
  kLibThread,
  kLibTime,
  kLibUrlEncode,
  kLibZlib
} TLibraryResultComponent;


/* kLibBase64 */
/* kLibDevice */
/* kLibFileOperations */
/* kLibFileTypes */
/* kLibIObject */
/* kLibJsonDocument */
#define ERR_LIB_JDOC_PARSE_INVALID_VARIABLE (0)
#define ERR_LIB_JDOC_CREATE_STRING_OUT_OF_RESOURCE (1)
#define ERR_LIB_JDOC_CREATE_STRING_INTERNAL_OUT_OF_RESOURCE (2)
#define ERR_LIB_JDOC_READER_PARSE_JSON_IS_NOT_PRESENT_IN_THE_TEXT (3)
#define ERR_LIB_JDOC_READER_PARSE_CHARS_THAT_YOU_EXPECT_TO_ROOT_DOES_NOT_EXIST (4)
#define ERR_LIB_JDOC_READER_PARSE_NOTHING_SHOULD_FOLLOW_THE_ROOT_OBJECT_OR_ARRAY (5)
/* kLibJsonValue */
#define ERR_LIB_JVALUE_GET_NUMBER_INVALID_VARIABLE (0)
#define ERR_LIB_JVALUE_GET_NUMBER_INVALID_VARIABLE_TYPE (1)
/* kLibList */
/* kLibObjectList */
/* kLibPathUtil */
/* kLibProfile */
/* kLibStack */
/* kLibString */
/* kLibThread */
/* kLibTime */
/* kLibUrlEncode */
#define ERR_URL_ENCODE_ENCODE_INVALID_VARIABLE_STR (0)
#define ERR_URL_ENCODE_ENCODE_OUT_OF_RESOURCE (1)
#define ERR_URL_ENCODE_DISPOSE_INVALID_VARIABLE (2)
/* kLibZlib */


#endif 


