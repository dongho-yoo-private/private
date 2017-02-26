/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file llr_types.h
 * \ingroup llr
 * \~english
 * \~japanese
 *  \brief low level renderer に必要な定義部
 */

#ifndef LLR_TYPES_H_
#define LLR_TYPES_H_

#include <core/types.h>

ADK_BEGIN_NAMESPACE

/**
 * \~english
 * \~japanese
 *  \brief Cocos2d-x間に合わせ定義(場所命名は後で変更の可能性有り)
 */
typedef struct {
  S32 src;
  S32 dst;
}TBlendFunc;
/**
 * \~english
 * \~japanese
 *  \brief とりあえずCocos2d-xに間に合わせ定義(この辺りはグラフィック機能実装する際に最適化する)
 */
typedef enum {

    //! 32-bit texture: RGBA8888
    kTexturePixelFormat_RGBA8888,
    //! 24-bit texture: RGBA888
    kTexturePixelFormat_RGB888,
    //! 16-bit texture without Alpha channel
    kTexturePixelFormat_RGB565,
    //! 8-bit textures used as masks
    kTexturePixelFormat_A8,
    //! 8-bit intensity texture
    kTexturePixelFormat_I8,
    //! 16-bit textures used as masks
    kTexturePixelFormat_AI88,
    //! 16-bit textures: RGBA4444
    kTexturePixelFormat_RGBA4444,
    //! 16-bit textures: RGB5A1
    kTexturePixelFormat_RGB5A1,    
    //! 4-bit PVRTC-compressed texture: PVRTC4
    kTexturePixelFormat_PVRTC4,
    //! 2-bit PVRTC-compressed texture: PVRTC2
    kTexturePixelFormat_PVRTC2,


    //! Default texture format: RGBA8888
    kTexturePixelFormat_Default = kTexturePixelFormat_RGBA8888,

    // backward compatibility stuff
    kTexture2DPixelFormat_RGBA8888 = kTexturePixelFormat_RGBA8888,
    kTexture2DPixelFormat_RGB888 = kTexturePixelFormat_RGB888,
    kTexture2DPixelFormat_RGB565 = kTexturePixelFormat_RGB565,
    kTexture2DPixelFormat_A8 = kTexturePixelFormat_A8,
    kTexture2DPixelFormat_RGBA4444 = kTexturePixelFormat_RGBA4444,
    kTexture2DPixelFormat_RGB5A1 = kTexturePixelFormat_RGB5A1,
    kTexture2DPixelFormat_Default = kTexturePixelFormat_Default
} TTexturePixelFormat;
/**
 * \~english
 * \~japanese
 *  \brief グラフィックライブラリ作成時に、これも名前返る
 */
typedef struct {
  U32 minFilter;
  U32 magFilter;
  U32 wrapS;
  U32 wrapT;
} TTexParams;

ADK_END_NAMESPACE

#endif  /* LLR_TYPES_H_ */

