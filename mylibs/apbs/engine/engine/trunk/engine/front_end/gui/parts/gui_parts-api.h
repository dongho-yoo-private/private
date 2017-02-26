/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file parts_api.h
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief パーツに関するインタフェース定義部
 */

#ifndef PARTS_API_H_
#define PARTS_API_H_

#include <core/math/math_library.h>
#include <core/types.h>
#include <debug_profiling/error/result.h>
#include <front_end/gui/parts/gui_parts_types.h>

ADK_BEGIN_NAMESPACE

/* forward declaration */
class IGuiParts;
class IGuiPartsSprite;
class IGuiPartsSpriteFrame;
class IGuiPartsTexture;

/**
 * \~english
 * \~japanese
 *  \brief パーツシステムを初期化し使用できる状態にします
 *  \return 初期化の成功判定。以下の値を返します
 *  \retval ResultSuccess 初期化に成功したときに返ります
 *  \retval ResultDoAlready 既に初期化されていたときに返ります
 *  \retval ResultOutOfResource システムリソースが不足していたときに返ります
 */
extern Result InitializeGuiPartsSystem();
/**
 * \~english
 * \~japanese
 *  \brief パーツシステムを終了します
 *  \return 終了の成功判定。以下の値を返します
 *  \retval ResultSuccess 終了に成功したときに返ります
 *  \retval ResultNotInitialized 初期化されてないのに呼び出されたときに返ります
 */
extern Result TerminateGuiPartsSystem();

/**
 * \~english
 * \~japanese
 *  \brief ファイルから \ref IGuiParts を作成します
 *  \param[in] filename パーツファイル名
 *  \return 作成の成功判定。以下の値が返ります
 *  \retval \ref IGuiParts 作成に成功したときに返ります
 *  \retval NULL 作成に失敗したときに返ります
 */
extern IGuiParts *CreateGuiPartsWithFile(const char *filename,
                                         const TGuiPartsInfo *parts,
                                         S32 parts_count);
/**
 * \~english
 * \~japanese
 *  \brief 指定した \ref TGuiPartsType の \ref IGuiParts を作成する
 *  \param[in] type \ref TGuiPartsType
 *  \return 作成の成功判定。以下の値が返ります
 *  \retval \ref IGuiParts 作成に成功したときに返ります
 *  \retval NULL 作成に失敗したときに返ります
 */
extern IGuiParts *CreateGuiPartsWithType(TGuiPartsType type);
/**
 * \~english
 * \~japanese
 *  \brief スプライトラッパー系
 */
extern IGuiPartsSprite* CreateGuiPartsSprite();
extern IGuiPartsSprite* CreateGuiPartsSpriteWithFile(const char* filename);
extern IGuiPartsSprite* CreateGuiPartsSpriteWithFile(const char* filename,
                                                     const TRect& rect);
extern IGuiPartsSprite* CreateGuiPartsSpriteWithTexture(
    IGuiPartsTexture* pTexture);
extern IGuiPartsSprite* CreateGuiPartsSpriteWithTexture(
    IGuiPartsTexture* pTexture, const TRect& rect);
#if 0
extern IGuiPartsSprite* CreateGuiPartsSpriteWithSpriteFrame(
    IGuiPartsSpriteFrame* pSpriteFrame);
extern IGuiPartsSprite* CreateGuiPartsSpriteWithSpriteFrameName(
    const char* pszSpriteFrameName);
#endif
/**
 * \~english
 * \~japanese
 *  \brief \ref IGuiParts を削除します
 *  \param[in] parts \ref IGuiParts
 *  \return 削除の成功判定。以下の値が返ります
 *  \retval ResultSuccess 削除に成功したときに返ります
 *  \retval ResultInvalidVariable 不正な引数が設定されたときに返ります
 */
extern Result DisposeGuiParts(IGuiParts *parts);
/**
 * \~english
 * \~japanese
 *  \brief \ref IGuiParts をシステムに追加します
 *  \param[in] parts \ref IGuiParts
 *  \return 追加の成功判定。以下の値が返ります
 *  \retval ResultSuccess 追加に成功したときに返ります
 *  \retval ResultInvalidVariable 不正な引数が設定されたときに返ります
 */
extern Result AddGuiParts(IGuiParts *parts);
/**
 * \~english
 * \~japanese
 *  \brief \ref IGuiParts をシステムから切り離します
 *  \param[in] parts \ref IGuiParts
 *  \return 切り離しの成功判定。以下の値が返ります
 *  \retval ResultSuccess 切り離しに成功したときに返ります
 *  \retval ResultInvalidVariable 不正な引数が設定されたときに返ります
 */
extern Result RemoveGuiParts(IGuiParts *parts);

ADK_END_NAMESPACE

#endif /* PARTS_API_H_ */

