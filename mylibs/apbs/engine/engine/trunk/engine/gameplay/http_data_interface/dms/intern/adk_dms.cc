/**
 * \file adk_dms.cc
 * \ingroup adkgrp_dms
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *  \brief Data management system 実装部
 */

#include <gameplay/http_data_interface/dms/adk_dms.h>

#include <core/library/lib_file_operations.h>
#include <core/library/lib_path_util.h>
#include <core/library/lib_string.h>
#include <debug_profiling/error/result.h>
#include <debug_profiling/error/err_dms_result_types.h>
#include <gameplay/http_data_interface/dms/intern/dms_schema_json.h>

ADK_BEGIN_NAMESPACE

/**
 * \~english
 * \~japanese
 *  \brief ADKスキーマファイルの拡張子
 */
#define ADK_SCHEMA_FILE_EXT (".adkschema")
/**
 * \~english
 * \~japanese
 *  \brief 共通レスポンスファイル名
 */
#define COMMON_RESPONSE_FILE_NAME ("common_response")

/* class implementation */
/* constructor & destructor */
DMS::DMS() { memset(&schema_, 0, sizeof(schema_)); }
DMS::~DMS() {}

/* initialize & terminate */
Result DMS::Initialize() {
  Result ret = ResultSuccess;
  Char root[FILE_NAME_MAX_LEN];
  size_t rootlen;
  LIB_TDirEntry *entrys = 0;
  S32 entrycount;

  LIB_GetResourceDir(root, FILE_NAME_MAX_LEN);
  rootlen = strnlen(root, FILE_NAME_MAX_LEN);
  sprintf(&root[rootlen], "/adk/schema/");
  rootlen = strnlen(root, FILE_NAME_MAX_LEN);
  root[rootlen] = '\0';

  entrycount = LIB_DirContents(&entrys, root);
  if (entrycount == 0xffffffff) {
    ret =
        ADK_RESULT_PROGRAM(ResultUnknown, kDmsComponent, ERR_DMS_INITIALIZE, 0);
    goto initerr;
  }

  if (entrycount > 0) {
    const U32 kDirLen = FILE_NAME_MAX_LEN;
    const U32 kFileLen = 64;
    const U32 kExtLen = 16;
    U32 schemacount = 0;
    LIB_TDirEntry *entry;
    Char dir[kDirLen], file[kFileLen], ext[kExtLen];

    entry = entrys;
    for (S32 index = 0; index < entrycount; ++index, ++entry) {
      LIB_SplitPathDirFileExt(dir, file, ext, kDirLen, kFileLen, kExtLen,
                              entry->relname);
      if (LIB_strcmp(ext, ADK_SCHEMA_FILE_EXT) != 0) {
        continue;
      }
      ++schemacount;
    }
    if (schemacount > 0) {
      DMS_SchemaJson *schema;

      schema_.count = schemacount;
      schema_.schemas = new DMS_SchemaJson[schema_.count];
      if (schema_.schemas == 0) {
        ret = ADK_RESULT_PROGRAM(ResultOutOfResource, kDmsComponent,
                                 ERR_DMS_INITIALIZE, 1);
        goto initerr;
      }

      schema = (DMS_SchemaJson *)schema_.schemas;
      entry = entrys;
      for (S32 index = 0; index < entrycount; ++index, ++entry) {
        LIB_SplitPathDirFileExt(dir, file, ext, kDirLen, kFileLen, kExtLen,
                                entry->relname);
        if (LIB_strcmp(ext, ADK_SCHEMA_FILE_EXT) != 0) {
          continue;
        }
        ret = schema->Initialize(entry->path);
        if (ret.IsSuccess() == false) {
          goto initerr;
        } else {
          if (LIB_strcmp(file, COMMON_RESPONSE_FILE_NAME) == 0) {
            schema_.common_response = schema;
          }
#if 0
          if (LIB_strcmp(file,"api_draw_gacha")==0) {
#if 0
            const Char  *newFlg= "newFlg";
            /* request */
            typedef struct {
              S32 unitId;
              Byte padding0[0x4];
              S64 patternId;
              const Char* newFlg;
              S32 dummyUnitId;
            }TRequestParam;
            typedef struct {
              S32 unitId;
              Byte padding0[0x4];
              S64 patternId;
              const Char* newFlg;
              Byte padding1[0x4];
              TRequestParam param;
            }TRequestData;
            TRequestData request = {
              (S32)0xDEADC0DE,
              {0},
              9876543210,
              newFlg,
              {0},
              {
                (S32)0xDEADC0DE,
                {0},
                9876543210,
                newFlg,
                (S32)0xDEADC0DE
              }
            };
            ((DMS_Schema*)schema)->Request((const Char*)&request,0);
#endif
            /* response */
#if 0
            static const Char *json_test = 
"{"
"	\"data\":{"
"		\"friendPoint\":1610,"
"		\"magicalStonePurchase\":100,"
"		\"magicalStoneGiven\":100,"
"		\"gachaDrawList\":["
"			{"
"				\"unitId\":11,"
"				\"userUnit\":{"
"					\"userUnitId\":13939035528051,"
"					\"unitId\":11,"
"					\"rarity\":\"NORMAL\","
"					\"unitType\":\"NORMAL\","
"					\"level\":1,"
"					\"experiencePoint\":0,"
"					\"leaderSkillId\":null,"
"					\"leaderSkillLevel\":null,"
"					\"subSkillId\":null,"
"					\"subSkillLevel\":null,"
"					\"hp\":600,"
"					\"attack\":240,"
"					\"defense\":30,"
"					\"toughness\":240,"
"					\"favoriteFlg\":\"CANCELED\""
"				},"
"				\"newFlg\":false,"
"				\"dummyUnitId\":null,"
"				\"patternId\":1"
"			},"
"			{"
"				\"unitId\":13,"
"				\"userUnit\":{"
"					\"userUnitId\":13939035528052,"
"					\"unitId\":13,"
"					\"rarity\":\"NORMAL\","
"					\"unitType\":\"NORMAL\","
"					\"level\":1,"
"					\"experiencePoint\":0,"
"					\"leaderSkillId\":null,"
"					\"leaderSkillLevel\":null,"
"					\"subSkillId\":null,"
"					\"subSkillLevel\":null,"
"					\"hp\":600,"
"					\"attack\":200,"
"					\"defense\":30,"
"					\"toughness\":240,"
"					\"favoriteFlg\":\"CANCELED\""
"				},"
"				\"newFlg\":false,"
"				\"dummyUnitId\":null,"
"				\"patternId\":1"
"			},"
"			{"
"				\"unitId\":9,"
"				\"userUnit\":{"
"					\"userUnitId\":13939035528053,"
"					\"unitId\":9,"
"					\"rarity\":\"NORMAL\","
"					\"unitType\":\"NORMAL\","
"					\"level\":1,"
"					\"experiencePoint\":0,"
"					\"leaderSkillId\":null,"
"					\"leaderSkillLevel\":null,"
"					\"subSkillId\":1005,"
"					\"subSkillLevel\":1,"
"					\"hp\":600,"
"					\"attack\":120,"
"					\"defense\":30,"
"					\"toughness\":240,"
"					\"favoriteFlg\":\"CANCELED\""
"				},"
"				\"newFlg\":false,"
"				\"dummyUnitId\":null,"
"				\"patternId\":1"
"			},"
"			{"
"				\"unitId\":17,"
"				\"userUnit\":{"
"					\"userUnitId\":13939035528054,"
"					\"unitId\":17,"
"					\"rarity\":\"NORMAL\","
"					\"unitType\":\"NORMAL\","
"					\"level\":1,"
"					\"experiencePoint\":0,"
"					\"leaderSkillId\":null,"
"					\"leaderSkillLevel\":null,"
"					\"subSkillId\":1009,"
"					\"subSkillLevel\":1,"
"					\"hp\":1200,"
"					\"attack\":170,"
"					\"defense\":30,"
"					\"toughness\":480,"
"					\"favoriteFlg\":\"CANCELED\""
"				},"
"				\"newFlg\":false,"
"				\"dummyUnitId\":null,"
"				\"patternId\":1"
"			},"
"			{"
"				\"unitId\":11,"
"				\"userUnit\":{"
"					\"userUnitId\":13939035528055,"
"					\"unitId\":11,"
"					\"rarity\":\"NORMAL\","
"					\"unitType\":\"NORMAL\","
"					\"level\":1,"
"					\"experiencePoint\":0,"
"					\"leaderSkillId\":null,"
"					\"leaderSkillLevel\":null,"
"					\"subSkillId\":null,"
"					\"subSkillLevel\":null,"
"					\"hp\":600,"
"					\"attack\":240,"
"					\"defense\":30,"
"					\"toughness\":240,"
"					\"favoriteFlg\":\"CANCELED\""
"				},"
"				\"newFlg\":false,"
"				\"dummyUnitId\":null,"
"				\"patternId\":1"
"			},"
"			{"
"				\"unitId\":7,"
"				\"userUnit\":{"
"					\"userUnitId\":13939035528056,"
"					\"unitId\":7,"
"					\"rarity\":\"NORMAL\","
"					\"unitType\":\"NORMAL\","
"					\"level\":1,"
"					\"experiencePoint\":0,"
"					\"leaderSkillId\":null,"
"					\"leaderSkillLevel\":null,"
"					\"subSkillId\":1004,"
"					\"subSkillLevel\":1,"
"					\"hp\":600,"
"					\"attack\":130,"
"					\"defense\":45,"
"					\"toughness\":240,"
"					\"favoriteFlg\":\"CANCELED\""
"				},"
"				\"newFlg\":false,"
"				\"dummyUnitId\":null,"
"				\"patternId\":1"
"			},"
"			{"
"				\"unitId\":23,"
"				\"userUnit\":{"
"					\"userUnitId\":13939035528057,"
"					\"unitId\":23,"
"					\"rarity\":\"NORMAL\","
"					\"unitType\":\"NORMAL\","
"					\"level\":1,"
"					\"experiencePoint\":0,"
"					\"leaderSkillId\":null,"
"					\"leaderSkillLevel\":null,"
"					\"subSkillId\":1012,"
"					\"subSkillLevel\":1,"
"					\"hp\":1800,"
"					\"attack\":170,"
"					\"defense\":75,"
"					\"toughness\":840,"
"					\"favoriteFlg\":\"CANCELED\""
"				},"
"				\"newFlg\":false,"
"				\"dummyUnitId\":null,"
"				\"patternId\":1"
"			},"
"			{"
"				\"unitId\":13,"
"				\"userUnit\":{"
"					\"userUnitId\":13939035528058,"
"					\"unitId\":13,"
"					\"rarity\":\"NORMAL\","
"					\"unitType\":\"NORMAL\","
"					\"level\":1,"
"					\"experiencePoint\":0,"
"					\"leaderSkillId\":null,"
"					\"leaderSkillLevel\":null,"
"					\"subSkillId\":null,"
"					\"subSkillLevel\":null,"
"					\"hp\":600,"
"					\"attack\":200,"
"					\"defense\":30,"
"					\"toughness\":240,"
"					\"favoriteFlg\":\"CANCELED\""
"				},"
"				\"newFlg\":false,"
"				\"dummyUnitId\":null,"
"				\"patternId\":1"
"			},"
"			{"
"				\"unitId\":13,"
"				\"userUnit\":{"
"					\"userUnitId\":13939035528059,"
"					\"unitId\":13,"
"					\"rarity\":\"NORMAL\","
"					\"unitType\":\"NORMAL\","
"					\"level\":1,"
"					\"experiencePoint\":0,"
"					\"leaderSkillId\":null,"
"					\"leaderSkillLevel\":null,"
"					\"subSkillId\":null,"
"					\"subSkillLevel\":null,"
"					\"hp\":600,"
"					\"attack\":200,"
"					\"defense\":30,"
"					\"toughness\":240,"
"					\"favoriteFlg\":\"CANCELED\""
"				},"
"				\"newFlg\":false,"
"				\"dummyUnitId\":null,"
"				\"patternId\":1"
"			},"
"			{"
"				\"unitId\":3,"
"				\"userUnit\":{"
"					\"userUnitId\":139390355280510,"
"					\"unitId\":3,"
"					\"rarity\":\"NORMAL\","
"					\"unitType\":\"NORMAL\","
"					\"level\":1,"
"					\"experiencePoint\":0,"
"					\"leaderSkillId\":null,"
"					\"leaderSkillLevel\":null,"
"					\"subSkillId\":1002,"
"					\"subSkillLevel\":1,"
"					\"hp\":1200,"
"					\"attack\":120,"
"					\"defense\":45,"
"					\"toughness\":480,"
"					\"favoriteFlg\":\"CANCELED\""
"				},"
"				\"newFlg\":false,"
"				\"dummyUnitId\":null,"
"				\"patternId\":1"
"			}"
"		]"
"	}"
"}";
            Char *json_text = (Char*)ADK_Malloc(1024*10,"jsontext");
            strcpy(json_text,json_test);
            ((DMS_Schema*)schema)->ParseResponseData((const Byte*)json_text,
                                      strlen(json_text)+1);
#endif
            /* get */
typedef struct {
  U64 userUnitId;
  U32 unitId;
  const Char* rarity;
  const Char* unitType;
  U32 level;
  U32 experiencePoint;
  U32 leaderSkillId;
  U32 leaderSkillLevel;
  U32 subSkillId;
  U32 subSkillLevel;
  U32 hp;
  U32 attack;
  U32 defense;
  U32 toughness;
  const Char* favoriteFlg;
}TDataGachaDrawListUserUnit;
/**
 * \~english
 * \~japanese
 *  \brief gachaDrawList 情報構造体
 */
typedef struct {
  U32 unitId;
  Byte padding0[0x4];
  TDataGachaDrawListUserUnit userUnit;
  Bool newFlg;
  U32 dummyUnitId;
  U32 patternId;
  Byte end_padding[0x4];
}TDataGachaDrawList;
/**
 * \~english
 * \~japanese
 *  \brief data 情報構造体
 */
typedef struct {
  U32 friendPoint;
  U32 magicalStonePurchase;
  U32 magicalStoneGiven;
  U32 gachaDrawListCount;
  TDataGachaDrawList *gachaDrawLists;
  Byte end_padding[0x4];
}TData;
printf("%d\n",sizeof(TDataGachaDrawList));
            Result result;
            TData *data = (TData*)((DMS_Schema*)schema)->GetObject("data",&result);
            if (result.IsSuccess()) {
              for (S32 index=0;index<data->gachaDrawListCount;++index) {
                TDataGachaDrawList *list = &data->gachaDrawLists[index];
                printf("%lu\n",list->userUnit.userUnitId);
              }
            }
          }
#endif
        }
        ++schema;
      }
    }
  }

initerr:

  if (entrys != 0) {
    LIB_FreeDirContents(entrys, entrycount);
  }

  return ret;
}
Result DMS::Terminate() {
  Result ret = ResultSuccess;

  if (schema_.schemas != 0) {
    DMS_SchemaJson *schema = (DMS_SchemaJson *)schema_.schemas;
    for (S32 index = 0; index < schema_.count; ++index, ++schema) {
      schema->Terminate();
    }
    delete[]((DMS_SchemaJson *)schema_.schemas), schema_.schemas = 0;
  }

  return ret;
}

/* schema managment functionally */
DMS_Schema *DMS::FindSchema(const Char *url) {
  DMS_Schema *ret = 0;
  DMS_SchemaJson *schema = (DMS_SchemaJson *)schema_.schemas;

  for (S32 index = 0; index < schema_.count; ++index, ++schema) {
    if (LIB_strcmp(schema->GetAccessUrl(), url) != 0) {
      continue;
    }
    ret = (DMS_Schema *)schema;
    break;
  }

  return ret;
}
void DMS::ResetRequestSchema() {
  DMS_SchemaJson *schema = (DMS_SchemaJson *)schema_.schemas;

  for (S32 index = 0; index < schema_.count; ++index, ++schema) {
    schema->ResetRequest();
    break;
  }
}

ADK_END_NAMESPACE

