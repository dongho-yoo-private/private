/**
 * \file math.h
 * \ingroup math
 * \author tomonori tanaka
 * \~english
 * \~japanese
 */

#ifndef MATH_LIBRARY_H_
#define MATH_LIBRARY_H_

#include <math.h>

#include <core/types.h>
#include <core/debug/debug_assert.h>

#if 1
#include "core/math/math_types.h"
#include "core/math/intern/math_color_ios.h"
#include "core/math/intern/math_vector_ios.h"
#else
#include "core/math/math_types.h"
#include "core/math/math_color.h"
#include "core/math/math_scalar.h"
#include "core/math/math_vector.h"
#if 0 /* とりあえず後回し */
#include "core/math/math_matrix.h"
#include "core/math/math_quat.h"
#endif
#if WITH_WIN32
# include "core/math/intern/math_color_intern_win32.h"
# include "core/math/intern/math_matrix_intern_win32.h"
# include "core/math/intern/math_quat_intern_win32.h"
# include "core/math/intern/math_scalar_intern_win32.h"
# include "core/math/intern/math_vector_intern_win32.h"
#elif WITH_COCOA
# include "core/math/intern/math_color_intern_cocoa.h"
# include "core/math/intern/math_matrix_intern_cocoa.h"
# include "core/math/intern/math_quat_intern_cocoa.h"
# include "core/math/intern/math_scalar_intern_cocoa.h"
# include "core/math/intern/math_vector_intern_cocoa.h"
#elif WITH_CARBON
# include "core/math/intern/math_color_intern_carbon.h"
# include "core/math/intern/math_matrix_intern_carbon.h"
# include "core/math/intern/math_quat_intern_carbon.h"
# include "core/math/intern/math_scalar_intern_carbon.h"
# include "core/math/intern/math_vector_intern_carbon.h"
#elif WITH_IOS
#if 1
# include "core/math/intern/math_color_intern_ios.h"
# include "core/math/intern/math_scalar_intern_ios.h"
# include "core/math/intern/math_vector_intern_ios.h"
#else
# include "core/math/intern/math_matrix_intern_ios.h"
# include "core/math/intern/math_quat_intern_ios.h"
#endif
#elif WITH_X11
# include "core/math/intern/math_color_intern_x11.h"
# include "core/math/intern/math_matrix_intern_x11.h"
# include "core/math/intern/math_quat_intern_x11.h"
# include "core/math/intern/math_scalar_intern_x11.h"
# include "core/math/intern/math_vector_intern_x11.h"
#elif WITH_ANDROID
# include "core/math/intern/math_color_intern_android.h"
# include "core/math/intern/math_matrix_intern_android.h"
# include "core/math/intern/math_quat_intern_android.h"
# include "core/math/intern/math_scalar_intern_android.h"
# include "core/math/intern/math_vector_intern_android.h"
#elif WITH_PSP
# include "core/math/intern/math_color_intern_psp.h"
# include "core/math/intern/math_matrix_intern_psp.h"
# include "core/math/intern/math_quat_intern_psp.h"
# include "core/math/intern/math_scalar_intern_psp.h"
# include "core/math/intern/math_vector_intern_psp.h"
#elif WITH_VITA
# include "core/math/intern/math_color_intern_vita.h"
# include "core/math/intern/math_matrix_intern_vita.h"
# include "core/math/intern/math_quat_intern_vita.h"
# include "core/math/intern/math_scalar_intern_vita.h"
# include "core/math/intern/math_vector_intern_vita.h"
#elif WITH_PS3
# include "core/math/intern/math_color_intern_ps3.h"
# include "core/math/intern/math_matrix_intern_ps3.h"
# include "core/math/intern/math_quat_intern_ps3.h"
# include "core/math/intern/math_scalar_intern_ps3.h"
# include "core/math/intern/math_vector_intern_ps3.h"
#elif WITH_PS4
# include "core/math/intern/math_color_intern_ps4.h"
# include "core/math/intern/math_matrix_intern_ps4.h"
# include "core/math/intern/math_quat_intern_ps4.h"
# include "core/math/intern/math_scalar_intern_ps4.h"
# include "core/math/intern/math_vector_intern_ps4.h"
#elif WITH_XBOX360
# include "core/math/intern/math_color_intern_xbox360.h"
# include "core/math/intern/math_matrix_intern_xbox360.h"
# include "core/math/intern/math_quat_intern_xbox360.h"
# include "core/math/intern/math_scalar_intern_xbox360.h"
# include "core/math/intern/math_vector_intern_xbox360.h"
#elif WITH_WII
# include "core/math/intern/math_color_intern_wii.h"
# include "core/math/intern/math_matrix_intern_wii.h"
# include "core/math/intern/math_quat_intern_wii.h"
# include "core/math/intern/math_scalar_intern_wii.h"
# include "core/math/intern/math_vector_intern_wii.h"
#elif WITH_3DS
# include "core/math/intern/math_color_intern_3ds.h"
# include "core/math/intern/math_matrix_intern_3ds.h"
# include "core/math/intern/math_quat_intern_3ds.h"
# include "core/math/intern/math_scalar_intern_3ds.h"
# include "core/math/intern/math_vector_intern_3ds.h"
#endif
#endif


#endif  /* MATH_LIBRARY_H_ */
