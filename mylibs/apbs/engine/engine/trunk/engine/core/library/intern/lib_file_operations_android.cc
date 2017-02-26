/**
 * \file lib_file_operations_android.cc
 * \ingroup adkgrp_library
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief ファイル/ディレクトリ操作 for Android
 */

#include <core/library/lib_file_operations.h>

#include <errno.h>
#include <fcntl.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <core/debug/debug_assert.h>
#include <core/debug/logging.h>
#include <core/memory_allocation/guarded_alloc.h>
#include <core/library/lib_list.h>
#include <core/library/lib_path_util.h>
#include <core/library/lib_string.h>

ADK_BEGIN_NAMESPACE

/* file operation */
#ifndef S_ISREG
#define S_ISREG(value) (((value) & _S_IFREG) == _S_IFREG)
#endif
#ifndef S_ISDIR
#define S_ISDIR(value) (((value) & _S_IFDIR) == _S_IFDIR)
#endif

/* files */
struct _LIB_TFileObject {
  LIB_TFileStatus stat;
  FILE *file;
  LIB_TFileType type;
};

/*  */
static S32 FileCompare(struct LIB_TDirEntry *entry1,
                       struct LIB_TDirEntry *entry2) {
  if (S_ISDIR(entry1->type)) {
    if (S_ISDIR(entry2->type) == 0) {
      return -1;
    }
  } else {
    if (S_ISDIR(entry2->type)) {
      return 1;
    }
  }
  if (S_ISREG(entry1->type)) {
    if (S_ISREG(entry2->type) == 0) {
      return -1;
    }
  } else {
    if (S_ISREG(entry2->type)) {
      return 1;
    }
  }
  if ((entry1->type & S_IFMT) < (entry2->type & S_IFMT)) {
    return -1;
  }
  if ((entry1->type & S_IFMT) > (entry2->type & S_IFMT)) {
    return 1;
  }

  if (strcmp(entry1->relname, ".") == 0) {
    return -1;
  }
  if (strcmp(entry2->relname, ".") == 0) {
    return 1;
  }
  if (strcmp(entry1->relname, "..") == 0) {
    return -1;
  }
  if (strcmp(entry2->relname, "..") == 0) {
    return 1;
  }

  return LIB_natstrcmp(entry1->relname, entry2->relname);
}

/* extern function implementation */
/* common functionally */
Bool LIB_FileExists(const Char *name) {
  Bool ret;

  ADK_ASSERT(name);
  if (name == 0) {
    ret = false;
  } else {
    struct stat buf;
    S32 result = stat(name, &buf);
    if (result != 0) {
      ret = false;
    } else {
      ret = true;
    }
  }

  return ret;
}
Bool LIB_FileCopy(const Char *from, const Char *to) { return false; }
Bool LIB_FileRename(const Char *from, const Char *to) { return false; }
Bool LIB_FileDelete(const Char *name, Bool dir, Bool recursive) {
  if (strchr(name, '"')) {
    CRITICAL_LOG("not deleted file %s because of quote!\n", name);
  } else {
    if (recursive) {
      //      return recursive_operation(name, NULL, NULL, delete_single_file,
      //      delete_callback_post);
    } else if (dir) {
      return rmdir(name);
    } else {
      return remove(name);
    }
  }

  return -1;
}
Bool LIB_FileMove(const Char *from, const Char *to) { return false; }
Bool LIB_FileStat(LIB_TFileStatus *out, const Char *name) {
  Bool ret;

  ADK_ASSERT(name);
  if (name == 0) {
    ret = false;
  } else {
    ADK_ASSERT(out);
    if (out == 0) {
      ret = false;
    } else {
      struct stat buf;
      S32 result = stat(name, &buf);
      if (result != 0) {
        ret = false;
      } else {
        memset(out, 0, sizeof(LIB_TFileStatus));
        out->size = buf.st_size;
        if (S_ISREG(buf.st_size)) out->mode |= LIB_TFileStatusIsReg;
        if (S_ISDIR(buf.st_size)) out->mode |= LIB_TFileStatusIsDir;
        ret = true;
      }
    }
  }

  return ret;
}

/* directories management functionally */
Bool LIB_IsDir(const Char *path) {
  Bool ret;

  ADK_ASSERT(path);
  if (path == 0) {
    ret = false;
  } else {
    LIB_TFileStatus stat;
    if (LIB_FileStat(&stat, path) != true) {
      ret = false;
    } else {
      ret = stat.mode & LIB_TFileStatusIsDir ? true : false;
    }
  }

  return ret;
}
Bool LIB_FileCreateDir(const Char *dir) {
  Bool ret;

  if (mkdir(dir, 0777) == 0) {
    ret = true;
  } else {
    ret = false;
  }
  return ret;
}
U64 LIB_FileDirFreeSpace(const Char *dir) { return 0; }
LIB_TFileKey *LIB_FileAuthenticateDir(const Char *dlcID) { return 0; }
Bool LIB_FileDisposeKey(LIB_TFileKey *key) { return false; }
S32 LIB_DirContents(struct LIB_TDirEntry **filelist, const Char *dirname) {
  LIB_TListBase dirbase = {0, 0};
  S32 ret = 0, dirlen;
  DIR *dir;
  Char dirpath[FILE_PATH_MAX_LEN];

  *filelist = 0;

  dirlen = LIB_strlen(dirname);
  LIB_strncpy(dirpath, dirname, FILE_PATH_MAX_LEN);
  if (dirname[dirlen - 1] != '/' && dirname[dirlen - 1] != '\\') {
    dirpath[dirlen] = '/';
    dirpath[dirlen + 1] = '\0';
  }

  if ((dir = opendir(dirpath)) != 0) {
    const struct dirent *fname;
    while ((fname = readdir(dir)) != 0) {
      struct LIB_TDirLink *const dlink = (struct LIB_TDirLink *)ADK_Calloc(
          1, sizeof(struct LIB_TDirLink), "dlink");
      if (dlink != 0) {
        dlink->name = LIB_strdup(fname->d_name);
        LIB_ListAddHead(&dirbase, dlink);
        ret++;
      }
    }

    if (ret != 0) {
      *filelist = (struct LIB_TDirEntry *)ADK_Malloc(
          sizeof(struct LIB_TDirEntry) * ret, "LIB_TDirEntry");
      ADK_ASSERT(*filelist);
      struct LIB_TDirLink *dlink = (struct LIB_TDirLink *)dirbase.first;
      struct LIB_TDirEntry *file = &(*filelist)[0];
      ret = 0;
      while (dlink) {
        Char fullname[FILE_PATH_MAX_LEN] = {0};

        file->relname = dlink->name;
        file->path = LIB_strdupcat(dirpath, dlink->name);
        LIB_JoinDirFile(fullname, sizeof(fullname), dirpath, dlink->name);

        stat(fullname, &file->s);

        file->type = file->s.st_mode;
        file->flags = 0;
        file++;
        ret++;
        dlink = dlink->next;
      }

      LIB_ListFreeList(&dirbase);

      if (*filelist != 0) {
        qsort(*filelist, ret, sizeof(struct LIB_TDirEntry),
              (int (*)(const void *, const void *))FileCompare);
      }
    }

    closedir(dir);
  } else {
    ret = 0xffffffff;
  }

  return ret;
}
void LIB_FreeDirContents(struct LIB_TDirEntry *filelist, U32 contentCount) {
  U32 index;

  for (index = 0; index < contentCount; ++index) {
    struct LIB_TDirEntry *const entry = filelist + index;
    if (entry->path) {
      ADK_Free(entry->path), entry->path = 0;
    }
    if (entry->relname) {
      ADK_Free(entry->relname), entry->relname = 0;
    }
  }

  ADK_Free(filelist), filelist = 0;
}

/* files management functionally */
Bool LIB_IsFile(const Char *path) {
  Bool ret;

  ADK_ASSERT(path);
  if (path == 0) {
    ret = false;
  } else {
    LIB_TFileStatus stat;
    if (LIB_FileStat(&stat, path) != true) {
      ret = false;
    } else {
      ret = stat.mode & LIB_TFileStatusIsReg ? true : false;
    }
  }

  return ret;
}
LIB_TFileObject *LIB_FileOpen(const Char *filename, LIB_TFileType type) {
  LIB_TFileObject *ret = 0;
  Bool result = true;

  if (LIB_strnlen(filename, FILE_NAME_MAX_LEN) > FILE_NAME_MAX_LEN) {
    result = false;
  } else {
    char opentype[4] = {0};

    if (type & LIB_kFileTypeRead) {
      if (type & LIB_kFileTypeWrite) {
        sprintf(opentype, "rb+");
      } else {
        sprintf(opentype, "rb");
      }
    } else if (type & LIB_kFileTypeAppend) {
      sprintf(opentype, "rb+");
    } else if (type & LIB_kFileTypeWrite) {
      sprintf(opentype, "wb");
    } else {
      ADK_ASSERT_MSG(0, "LIB_FileFioWin32::Open(): Invalid file type\n");
      result = false;
    }

    if (result == true) {
      ret = (LIB_TFileObject *)ADK_Calloc(1, sizeof(LIB_TFileObject),
                                          "FILE Object");
      if (ret == 0) {
        CRITICAL_LOG("LIB_FileOpen(): Out of resource.\n");
        result = false;
      } else {
        ret->file = fopen(filename, opentype);
        if (ret->file == 0) {
          CRITICAL_LOG("LIB_FileOpen(): Failed to file open = %s\n",
                           filename);
          ADK_Free(ret);
          ret = 0;
          result = false;
        } else {
          LIB_TFileStatus stat = {0};
          if (type & (LIB_kFileTypeRead | LIB_kFileTypeAppend)) {
            if (LIB_FileStat(&stat, filename) != true) {
              result = false;
            } else {
              ret->stat = stat;
              ret->type = type;
              result = true;
            }
          } else {
            ret->stat = stat;
            ret->type = type;
            result = true;
          }
        }
      }
    }
  }

  return ret;
}
Bool LIB_FileClose(LIB_TFileObject *fileobj) {
  Bool ret;

  ADK_ASSERT(fileobj);
  if (fileobj == 0) {
    ret = false;
  } else {
    if (fileobj->file == 0) {
      ret = false;
    } else {
      fclose(fileobj->file);
      fileobj->file = 0;
      ret = true;
    }
    ADK_Free(fileobj);
  }

  return ret;
}
U64 LIB_FileRead(void *buf, LIB_TFileObject *fileobj, U64 size) {
  ADK_ASSERT(buf);
  ADK_ASSERT(fileobj);
  if (buf == 0 || fileobj == 0) {
    return 0;
  } else {
    if (fileobj->file == 0) {
      return 0;
    } else {
      size_t readnum = fread(buf, (size_t)size, 1, fileobj->file);
      return readnum * size;
    }
  }
}
Bool LIB_FileWrite(LIB_TFileObject *fileobj, const void *buf, U64 size) {
  Bool ret;

  ADK_ASSERT(buf);
  ADK_ASSERT(fileobj);
  if (buf == 0 || fileobj == 0) {
    ret = false;
  } else {
    if (fileobj->file == 0) {
      ret = false;
    } else {
#if 1
      fwrite(buf, (size_t)size, 1, fileobj->file);
#else
      size_t writesize = fwrite(buf, (size_t)size, 1, fileobj->file);
#endif
      ret = true;
    }
  }

  return ret;
}
Bool LIB_FileSeek(LIB_TFileObject *fileobj, S64 seek, LIB_TFileWhence whence) {
  Bool ret;
  S32 w = 0;

  ADK_ASSERT(fileobj);
  if (fileobj == 0) {
    ret = false;
  } else {
    if (fileobj->file == 0) {
      ret = false;
    } else {
      switch (whence) {
        case LIB_kFileSet: {
          w = SEEK_SET;
          break;
        }
        case LIB_kFileCur: {
          w = SEEK_CUR;
          break;
        }
        case LIB_kFileEnd: {
          w = SEEK_END;
          break;
        }
      }
      if (fseek(fileobj->file, (size_t)seek, w) != 0) {
        ret = false;
      } else {
        ret = true;
      }
    }
  }

  return ret;
}
U64 LIB_FileTell(LIB_TFileObject *fileobj) {
  U64 ret;

  ADK_ASSERT(fileobj);
  if (fileobj == 0) {
    ret = 0;
  } else {
    if (fileobj->file == 0) {
      ret = 0;
    } else {
      ret = ftell(fileobj->file);
    }
  }

  return ret;
}
Bool LIB_FileTruncate(LIB_TFileObject *fileobj, U64 size) {
  Bool ret;

  ADK_ASSERT(fileobj);
  if (fileobj == 0) {
    ret = false;
  } else {
    if (fileobj->file == 0) {
      ret = false;
    } else {
      int file_no = fileno(fileobj->file);
      if (ftruncate(file_no, (off_t)size) != 0) {
        ret = false;
      } else {
        ret = true;
      }
    }
  }

  return ret;
}
U64 LIB_FileSize(LIB_TFileObject *fileobj) {
  U64 ret;

  ADK_ASSERT(fileobj);
  if (fileobj == 0) {
    ret = 0;
  } else {
    ret = fileobj->stat.size;
  }

  return ret;
}

ADK_END_NAMESPACE

