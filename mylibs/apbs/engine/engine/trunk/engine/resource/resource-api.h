/**
 * \file resource-api.h
 * \ingroup adkgrp_api
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *  \brief リソース周りのアプリケーションインタフェース
 */

#ifndef RESOURCE_API_H_
#define RESOURCE_API_H_

#include <core/types.h>

ADK_BEGIN_NAMESPACE

/**
 * \~english
 * \~japanese
 *  \brief リソースが探し出すルートパスを設定する。
 *  \attention この関数はCocos2d-xのCCFilUtilのためのもので、
 *             エンジン設計とは別物。突貫工事的な
 */
extern Bool AddResourceSearchPath(const char *path);
extern Bool AddSearchResolutionsOrder(const char *path);

ADK_END_NAMESPACE

#endif /* RESOURCE_API_H_ */

