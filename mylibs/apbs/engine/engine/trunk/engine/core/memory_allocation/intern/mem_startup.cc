/**
 * \file mem_startup.cc
 * \ingroup adkgrp_guardalloc
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *  \brief ガードアロケータの起動情報に関する実装部
 */

#include <core/memory_allocation/intern/mem_startup.h>

#include <core/library/lib_file_operations.h>
#include <core/library/lib_json_document.h>
#include <core/library/lib_path_util.h>
#include <core/memory_allocation/intern/mem_internal_alloc.h>

/**
 * \~english
 * \~japanese
 *  \brief ガードアロケータの起動情報ファイル名
 */
#define STARTUP_FILENAME ("adk/adk_galloc_info.txt")

/* startup type */
/**
 * \~english
 * \~japanese
 *  \brief 起動情報
 */
struct Startup {
  struct {
    Char *bin;
    adk::LIB_JsonDocument *doc;
  } json;
} gStartup;

/* extern function implementation */
Bool MEM_InitStartup() {
  Bool ret;
  static const size_t kMaxLen = 256;
  Char path[kMaxLen], filename[kMaxLen];

  if (adk::LIB_GetResourceDir(path, kMaxLen) == false) {
    return false;
  } else {
    sprintf(filename, "%s/%s", path, STARTUP_FILENAME);
  }

  if (adk::LIB_FileExists(filename) == false) {
    ret = false;
  } else {
    adk::LIB_TFileObject *fo =
        adk::LIB_FileOpen(filename, adk::LIB_kFileTypeRead);
    if (fo == 0) {
      ret = false;
    } else {
      const size_t filesize = adk::LIB_FileSize(fo);
      gStartup.json.bin = static_cast<Char *>(MEM_InternalMalloc(filesize + 1));
      ADK_ASSERT_MSG(gStartup.json.bin,
                     "ADK_GA: MEM_InitStartup(): Out of resource.");
      adk::LIB_FileRead(gStartup.json.bin, fo, filesize);
      gStartup.json.bin[filesize] = '\0';
      adk::LIB_FileClose(fo);
      fo = 0;

      gStartup.json.doc = new adk::LIB_JsonDocument;
      if (gStartup.json.doc == 0) {
        ret = false;
      } else {
        adk::LIB_JsonDocument &jdoc = *gStartup.json.doc;
        /* 起動情報のパースを行う */
        jdoc.Parse(gStartup.json.bin, false, 0);
      }

      ret = true;
    }
  }

  return ret;
}
Bool MEM_TermStartup() {
  Bool ret;

  if (gStartup.json.doc == 0) {
    ret = false;
  } else {
    gStartup.json.doc->Release();
    delete gStartup.json.doc, gStartup.json.doc = 0;
    ret = true;
  }

  if (gStartup.json.bin != 0) {
    MEM_InternalFree(gStartup.json.bin), gStartup.json.bin = 0;
  }

  return ret;
}

Bool MEM_IsUse() {
  Bool ret;
  const adk::LIB_JsonDocument &jdoc = *gStartup.json.doc;
  const adk::LIB_TJsonMember &use = jdoc["use"];

  ret = use.data.IsNull() != false ? false : use.data.GetBool();

  return ret;
}
S32 MEM_StartupGetArenaCount() {
  S32 ret = 0;
  const adk::LIB_JsonDocument &jdoc = *gStartup.json.doc;
  const adk::LIB_TJsonMember &sa = jdoc["small"];
  const adk::LIB_TJsonMember &la = jdoc["large"];

  if (sa.name.IsNull() == false) {
    ret += sa.data.Size();
  }

  if (la.name.IsNull() == false) {
    ret += la.data.Size();
  }

  return ret;
}
Bool MEM_StartupGetArena(MEM_TStartupArena *arena, S32 index) {
  Bool ret = false;
  const adk::LIB_JsonDocument &jdoc = *gStartup.json.doc;
  const adk::LIB_TJsonMember &sa = jdoc["small"];
  const adk::LIB_TJsonMember &la = jdoc["large"];
  S32 saCount = 0;
  size_t saMaxSize;

  ret = true;
  if (sa.name.IsNull() == false) {
    saCount = sa.data.Size();
    if (index < saCount) {
      const adk::LIB_JsonValue &val = sa.data[index];
      arena->min = val["min"].data.GetS32();
      arena->max = val["max"].data.GetS32();
      arena->size = ADK_ALIGN(val["size"].data.GetS32(), 4);
      snprintf(arena->name, MEM_STARTUP_ARENA_NAME_LEN, "%zu_%zu", arena->min,
               arena->max);
      ret = true;
      goto setting;
    }

    saMaxSize = 0;
    for (S32 idx = 0; idx < saCount; ++idx) {
      const adk::LIB_JsonValue &val = sa.data[idx];
      if (val["max"].data.GetS32() <= saMaxSize) {
        continue;
      }
      saMaxSize = val["max"].data.GetS32();
    }
  }

  if (la.name.IsNull() == false) {
    const adk::LIB_JsonValue &val = la.data[index - saCount];
    arena->min = saMaxSize;
    arena->max = 0xffffffff;
    arena->size = ADK_ALIGN(val["size"].data.GetS32(), 4);
    snprintf(arena->name, MEM_STARTUP_ARENA_NAME_LEN, "large");
    ret = true;
    goto setting;
  }

setting:

  return ret;
}

