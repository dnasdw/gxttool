#ifndef GXT_H_
#define GXT_H_

#include <sdw.h>

namespace pvrtexture
{
	class CPVRTexture;
}

// /target/include_common/gxt.h

#define SCE_GXT_TAG								0x00545847UL		/* 'GXT\0'	*/
#define SCE_GXT_VERSION							0x10000003UL

#define SCE_GXT_PALETTE_SIZE_P4					64U
#define SCE_GXT_PALETTE_SIZE_P8					1024U

// /target/include_common/gxm/defs.h

/** The minimum alignment of texture data that satisfies the
	alignment requirements of all supported texture formats.

	When a specific texture format is being used, the required
	alignment may be less than this value. For details on
	each format¡¯s alignment requirement, please see the
	<em>GPU User¡¯s Guide</em>.

	@ingroup render
*/
#define SCE_GXM_TEXTURE_ALIGNMENT					16U

/** The minimum alignment of the space between cube map faces when
	each face has a mip chain, and the length of the top level mip of
	each face is above the ¡°no align¡± size.

	The ¡°no align¡± size will be either 
	#SCE_GXM_TEXTURE_CUBE_NO_ALIGN_SIZE_8BPP or
	#SCE_GXM_TEXTURE_CUBE_NO_ALIGN_SIZE_16_32BPP or 
	#SCE_GXM_TEXTURE_CUBE_NO_ALIGN_SIZE_64BPP depending on the texture
	format of the cube map data.

	There is no padding between cube map faces for cubemaps less than
	or equal to the ¡°no align¡± size.

	@ingroup render
*/
#define SCE_GXM_TEXTURE_FACE_ALIGNMENT				2048U

/** The largest cube face size where the space between each cube map face
	mip chain does <b>not</b> need to be aligned to 
	#SCE_GXM_TEXTURE_FACE_ALIGNMENT bytes.

	This size applies to cube maps of 8 bits per pixel, or cube maps that 
	use block compression.

	@ingroup render
*/
#define SCE_GXM_TEXTURE_CUBE_NO_ALIGN_SIZE_8BPP		16U

/** The largest cube face size where the space between each cube map face
	mip chain does <b>not</b> need to be aligned to 
	#SCE_GXM_TEXTURE_FACE_ALIGNMENT bytes.

	This size applies to cube maps of 16 or 32 bits per pixel.

	@ingroup render
*/
#define SCE_GXM_TEXTURE_CUBE_NO_ALIGN_SIZE_16_32BPP	8U

/** The largest cube face size where the space between each cube map face
	mip chain does <b>not</b> need to be aligned to 
	#SCE_GXM_TEXTURE_FACE_ALIGNMENT bytes.

	This size applies to cube maps of 64 bits per pixel.

	@ingroup render
*/
#define SCE_GXM_TEXTURE_CUBE_NO_ALIGN_SIZE_64BPP	4U

/** When using linear textures with implicit stride, the stride is formed
	by aligning the width to this value.

	@ingroup render
*/
#define SCE_GXM_TEXTURE_IMPLICIT_STRIDE_ALIGNMENT	8U

/** A mask used to extract the base texture format from a #SceGxmTextureFormat
	value. 

	@ingroup render
*/
#define SCE_GXM_TEXTURE_BASE_FORMAT_MASK			0x9f000000U

// /host_tools/graphics/src/sce_texture/texture_libraries/sce_texture_core/common/common.h

// Power of two alignment
#define SCE_ALIGN(VALUE, ALIGNMENT)											(((size_t)(VALUE) + ((ALIGNMENT)-1)) & ~((ALIGNMENT)-1))

#define SCE_IS_POW2(VALUE)													((((VALUE) & ((VALUE) - 1)) == 0) && (VALUE))

#include SDW_MSC_PUSH_PACKED
// /target/include_common/gxt.h

/**	A header for a GXT file.

	This structure should be located right at the start of the GXT file.
*/
typedef struct SceGxtHeader
{
	uint32_t	tag;				///< The GXT identifier.
	uint32_t	version;			///< The version number.
	uint32_t	numTextures;		///< The number of textures.
	uint32_t	dataOffset;			///< The offset to the texture data.
	uint32_t	dataSize;			///< The total size of the texture data.
	uint32_t	numP4Palettes;		///< The number of 16 entry palettes.
	uint32_t	numP8Palettes;		///< The number of 256 entry palettes.
	uint32_t	pad;				///< Padding.
} SceGxtHeader;
#include SDW_MSC_POP_PACKED

/**	Contains information about each texture in the GXT file.

	An array of these structures should immediately follow the initial SceGxtHeader in the GXT file.

	Please refer to the <em>libgxm Reference</em> for a description of <c>SceGxmTextureType</c> and <c>SceGxmTextureFormat</c>.
*/
typedef struct SceGxtTextureInfo
{
	uint32_t	dataOffset;			///< The offset to the texture data.
	uint32_t	dataSize;			///< The size of the texture data.
	uint32_t	paletteIndex;		///< The index of the palette.
	uint32_t	flags;				///< Texture flags (#SceGxtTextureFlags).
	uint32_t	type;				///< The texture type (<c>SceGxmTextureType</c>).
	uint32_t	format;				///< The texture format (<c>SceGxmTextureFormat</c>).
	uint16_t	width;				///< The texture width.
	uint16_t	height;				///< The texture height.
	uint8_t		mipCount;			///< The number of mipmaps.
	uint8_t		pad[3];				///< Padding.
} SceGxtTextureInfo;

/** A set of flags that describe a texture.
*/
typedef enum SceGxtTextureFlags {
	SCE_GXT_TEXTURE_FLAG_HAS_BORDER_DATA		= 0x00000001U		///< The texture contains border data.
} SceGxtTextureFlags;

// /target/include_common/gxm/constants.h

/** Defines the layout of 4-component texture formats which can be swizzled.

	@ingroup render
*/
typedef enum SceGxmTextureSwizzle4Mode {
	SCE_GXM_TEXTURE_SWIZZLE4_ABGR			= 0x00000000U,	///< Texture format read in ABGR order
	SCE_GXM_TEXTURE_SWIZZLE4_ARGB			= 0x00001000U,	///< Texture format read in ARGB order
	SCE_GXM_TEXTURE_SWIZZLE4_RGBA			= 0x00002000U,	///< Texture format read in RGBA order
	SCE_GXM_TEXTURE_SWIZZLE4_BGRA			= 0x00003000U,	///< Texture format read in BGRA order
	SCE_GXM_TEXTURE_SWIZZLE4_1BGR			= 0x00004000U,	///< Texture format read in ABGR order, and A is forced to 1.0
	SCE_GXM_TEXTURE_SWIZZLE4_1RGB			= 0x00005000U,	///< Texture format read in ARGB order, and A is forced to 1.0
	SCE_GXM_TEXTURE_SWIZZLE4_RGB1			= 0x00006000U,	///< Texture format read in RGBA order, and A is forced to 1.0
	SCE_GXM_TEXTURE_SWIZZLE4_BGR1			= 0x00007000U	///< Texture format read in BGRA order, and A is forced to 1.0
} SceGxmTextureSwizzle4Mode;

/** Defines the layout of 3-component texture formats which can be swizzled.

	@ingroup render
*/
typedef enum SceGxmTextureSwizzle3Mode {
	SCE_GXM_TEXTURE_SWIZZLE3_BGR			= 0x00000000U,	///< Texture format read in BGR order
	SCE_GXM_TEXTURE_SWIZZLE3_RGB			= 0x00001000U	///< Texture format read in RGB order
} SceGxmTextureSwizzle3Mode;

/** Defines the result layout of 2-component texture formats which can be swizzled.

	@ingroup render
*/
typedef enum SceGxmTextureSwizzle2Mode {
	SCE_GXM_TEXTURE_SWIZZLE2_GR				= 0x00000000U,	///< The texture format is swizzled to ABGR form as XXGR (where X is undefined)
	SCE_GXM_TEXTURE_SWIZZLE2_00GR			= 0x00001000U,	///< The texture format is swizzled to ABGR form as 00GR 
	SCE_GXM_TEXTURE_SWIZZLE2_GRRR			= 0x00002000U,	///< The texture format is swizzled to ABGR form as GRRR
	SCE_GXM_TEXTURE_SWIZZLE2_RGGG			= 0x00003000U,	///< The texture format is swizzled to ABGR form as RGGG 
	SCE_GXM_TEXTURE_SWIZZLE2_GRGR			= 0x00004000U,	///< The texture format is swizzled to ABGR form as GRGR 
	SCE_GXM_TEXTURE_SWIZZLE2_00RG			= 0x00005000U	///< The texture format is swizzled to ABGR form as 00RG 
} SceGxmTextureSwizzle2Mode;

/** Defines the result layout of 2-component texture formats which can be swizzled.

	@ingroup render
*/
typedef enum SceGxmTextureSwizzle2ModeAlt {
	SCE_GXM_TEXTURE_SWIZZLE2_SD				= 0x00000000U,	///< The depth/stencil texture format read in SD order
	SCE_GXM_TEXTURE_SWIZZLE2_DS				= 0x00001000U	///< The depth/stencil texture format read in DS order
} SceGxmTextureSwizzle2ModeAlt;

/** Defines the result layout of 1-component texture formats which can be swizzled.

	@ingroup render
*/
typedef enum SceGxmTextureSwizzle1Mode {
	SCE_GXM_TEXTURE_SWIZZLE1_R				= 0x00000000U,	///< Texture format is swizzled to ABGR form as XXXR (where X is undefined)
	SCE_GXM_TEXTURE_SWIZZLE1_000R			= 0x00001000U,	///< Texture format is swizzled to ABGR form as 000R 
	SCE_GXM_TEXTURE_SWIZZLE1_111R			= 0x00002000U,	///< Texture format is swizzled to ABGR form as 111R 
	SCE_GXM_TEXTURE_SWIZZLE1_RRRR			= 0x00003000U,	///< Texture format is swizzled to ABGR form as RRRR 
	SCE_GXM_TEXTURE_SWIZZLE1_0RRR			= 0x00004000U,	///< Texture format is swizzled to ABGR form as 0RRR 
	SCE_GXM_TEXTURE_SWIZZLE1_1RRR			= 0x00005000U,	///< Texture format is swizzled to ABGR form as 1RRR 
	SCE_GXM_TEXTURE_SWIZZLE1_R000			= 0x00006000U,	///< Texture format is swizzled to ABGR form as R000 
	SCE_GXM_TEXTURE_SWIZZLE1_R111			= 0x00007000U	///< Texture format is swizzled to ABGR form as R111 
} SceGxmTextureSwizzle1Mode;

/** Defines the memory layout of YUV422 texture formats.

	@ingroup render
*/
typedef enum SceGxmTextureSwizzleYUV422Mode {
	SCE_GXM_TEXTURE_SWIZZLE_YUYV_CSC0		= 0x00000000U,	///< Texture format read in Y1UY0V order, using CSC matrix 0
	SCE_GXM_TEXTURE_SWIZZLE_YVYU_CSC0		= 0x00001000U,	///< Texture format read in Y1VY0U order, using CSC matrix 0
	SCE_GXM_TEXTURE_SWIZZLE_UYVY_CSC0		= 0x00002000U,	///< Texture format read in UY1VY0 order, using CSC matrix 0
	SCE_GXM_TEXTURE_SWIZZLE_VYUY_CSC0		= 0x00003000U,	///< Texture format read in VY1UY0 order, using CSC matrix 0
	SCE_GXM_TEXTURE_SWIZZLE_YUYV_CSC1		= 0x00004000U,	///< Texture format read in Y1UY0V order, using CSC matrix 1
	SCE_GXM_TEXTURE_SWIZZLE_YVYU_CSC1		= 0x00005000U,	///< Texture format read in Y1VY0U order, using CSC matrix 1
	SCE_GXM_TEXTURE_SWIZZLE_UYVY_CSC1		= 0x00006000U,	///< Texture format read in UY1VY0 order, using CSC matrix 1
	SCE_GXM_TEXTURE_SWIZZLE_VYUY_CSC1		= 0x00007000U	///< Texture format read in VY1UY0 order, using CSC matrix 1
} SceGxmTextureSwizzleYUV422Mode;

/** Defines the memory layout of YUV420 texture formats.

	@ingroup render
*/
typedef enum SceGxmTextureSwizzleYUV420Mode {
	SCE_GXM_TEXTURE_SWIZZLE_YUV_CSC0		= 0x00000000U,	///< Texture planes read in YUV order, using CSC matrix 0
	SCE_GXM_TEXTURE_SWIZZLE_YVU_CSC0		= 0x00001000U,	///< Texture planes read in YVU order, using CSC matrix 0
	SCE_GXM_TEXTURE_SWIZZLE_YUV_CSC1		= 0x00002000U,	///< Texture planes read in YUV order, using CSC matrix 1
	SCE_GXM_TEXTURE_SWIZZLE_YVU_CSC1		= 0x00003000U	///< Texture planes read in YVU order, using CSC matrix 1
} SceGxmTextureSwizzleYUV420Mode;

/** The base formats for textures.  A texture format is made from (bitwise) combining 
	a base format with a compatible swizzle.

	@ingroup render
*/
typedef enum SceGxmTextureBaseFormat {
	SCE_GXM_TEXTURE_BASE_FORMAT_U8					= 0x00000000U,	///< 8-bit format, 8-bit unsigned integer
	SCE_GXM_TEXTURE_BASE_FORMAT_S8					= 0x01000000U,	///< 8-bit format, 8-bit signed integer
	SCE_GXM_TEXTURE_BASE_FORMAT_U4U4U4U4			= 0x02000000U,	///< 16-bit format, 4x 4-bit unsigned integer
	SCE_GXM_TEXTURE_BASE_FORMAT_U8U3U3U2			= 0x03000000U,	///< 16-bit format, 8-bit unsigned, 3-bit unsigned, 3-bit unsigned and 2-bit unsigned integer
	SCE_GXM_TEXTURE_BASE_FORMAT_U1U5U5U5			= 0x04000000U,	///< 16-bit format, 1-bit unsigned and 3x 5-bit unsigned integer
	SCE_GXM_TEXTURE_BASE_FORMAT_U5U6U5				= 0x05000000U,	///< 16-bit format, 5-bit unsigned, 6-bit unsigned and 5-bit unsigned integer		
	SCE_GXM_TEXTURE_BASE_FORMAT_S5S5U6				= 0x06000000U,	///< 16-bit format, 5-bit signed, 5-bit signed and 6-bit unsigned integer
	SCE_GXM_TEXTURE_BASE_FORMAT_U8U8				= 0x07000000U,	///< 16-bit format, 2x 8-bit unsigned integer
	SCE_GXM_TEXTURE_BASE_FORMAT_S8S8				= 0x08000000U,	///< 16-bit format, 2x 8-bit signed integer
	SCE_GXM_TEXTURE_BASE_FORMAT_U16					= 0x09000000U,	///< 16-bit format, 16-bit unsigned integer
	SCE_GXM_TEXTURE_BASE_FORMAT_S16					= 0x0a000000U,	///< 16-bit format, 16-bit signed integer
	SCE_GXM_TEXTURE_BASE_FORMAT_F16					= 0x0b000000U,	///< 16-bit format, 16-bit s1e5m10 floating point
	SCE_GXM_TEXTURE_BASE_FORMAT_U8U8U8U8			= 0x0c000000U,	///< 32-bit format, 4x 8-bit unsigned integer
	SCE_GXM_TEXTURE_BASE_FORMAT_S8S8S8S8			= 0x0d000000U,	///< 32-bit format, 4x 8-bit signed integer
	SCE_GXM_TEXTURE_BASE_FORMAT_U2U10U10U10			= 0x0e000000U,	///< 32-bit format, 2-bit unsigned and 3x 10-bit unsigned integer
	SCE_GXM_TEXTURE_BASE_FORMAT_U16U16				= 0x0f000000U,	///< 32-bit format, 2x 16-bit unsigned integer
	SCE_GXM_TEXTURE_BASE_FORMAT_S16S16				= 0x10000000U,	///< 32-bit format, 2x 16-bit signed integer
	SCE_GXM_TEXTURE_BASE_FORMAT_F16F16				= 0x11000000U,	///< 32-bit format, 2x 16-bit s1e5m10 floating point
	SCE_GXM_TEXTURE_BASE_FORMAT_F32					= 0x12000000U,	///< 32-bit format, 32-bit floating point
	SCE_GXM_TEXTURE_BASE_FORMAT_F32M				= 0x13000000U,	///< 32-bit format, 32-bit floating point with sign bit masked off
	SCE_GXM_TEXTURE_BASE_FORMAT_X8S8S8U8			= 0x14000000U,	///< 32-bit format, 8-bit unused, 8-bit signed, 8-bit signed and 8-bit unsigned integer
	SCE_GXM_TEXTURE_BASE_FORMAT_X8U24				= 0x15000000U,	///< 32-bit format, 8-bit unused, 24-bit unsigned integer
	SCE_GXM_TEXTURE_BASE_FORMAT_U32					= 0x17000000U,	///< 32-bit format, 32-bit unsigned integer
	SCE_GXM_TEXTURE_BASE_FORMAT_S32					= 0x18000000U,	///< 32-bit format, 32-bit signed integer
	SCE_GXM_TEXTURE_BASE_FORMAT_SE5M9M9M9			= 0x19000000U,	///< 32-bit format, 5-bit shared exponent and 3x 9-bit floating point mantissa
	SCE_GXM_TEXTURE_BASE_FORMAT_F11F11F10			= 0x1a000000U,	///< 32-bit format, 2x 11-bit s0e5m6 floating point and 10-bit s0e5m5 floating point
	SCE_GXM_TEXTURE_BASE_FORMAT_F16F16F16F16		= 0x1b000000U,	///< 64-bit format, 4x 16-bit s1e5m10 floating point
	SCE_GXM_TEXTURE_BASE_FORMAT_U16U16U16U16		= 0x1c000000U,	///< 64-bit format, 4x 16-bit unsigned integer
	SCE_GXM_TEXTURE_BASE_FORMAT_S16S16S16S16		= 0x1d000000U,	///< 64-bit format, 4x 16-bit signed integer
	SCE_GXM_TEXTURE_BASE_FORMAT_F32F32				= 0x1e000000U,	///< 64-bit format, 2x 32-bit floating point
	SCE_GXM_TEXTURE_BASE_FORMAT_U32U32				= 0x1f000000U,	///< 64-bit format, 2x 32-bit unsigned integer
	SCE_GXM_TEXTURE_BASE_FORMAT_PVRT2BPP			= 0x80000000U,	///< Block compressed PVRT1, 2 bits per pixel mode
	SCE_GXM_TEXTURE_BASE_FORMAT_PVRT4BPP			= 0x81000000U,	///< Block compressed PVRT1, 4 bits per pixel mode
	SCE_GXM_TEXTURE_BASE_FORMAT_PVRTII2BPP			= 0x82000000U,	///< Block compressed PVRT2, 2 bits per pixel mode
	SCE_GXM_TEXTURE_BASE_FORMAT_PVRTII4BPP			= 0x83000000U,	///< Block compressed PVRT2, 4 bits per pixel mode
	SCE_GXM_TEXTURE_BASE_FORMAT_UBC1				= 0x85000000U,	///< Block compressed UBC1 (aka DXT1), 4 bits per pixel 
	SCE_GXM_TEXTURE_BASE_FORMAT_UBC2				= 0x86000000U,	///< Block compressed UBC2 (aka DXT3), 8 bits per pixel
	SCE_GXM_TEXTURE_BASE_FORMAT_UBC3				= 0x87000000U,	///< Block compressed UBC3 (aka DXT5), 8 bits per pixel
	SCE_GXM_TEXTURE_BASE_FORMAT_UBC4				= 0x88000000U,	///< Block compressed UBC4, 4 bits per pixel
	SCE_GXM_TEXTURE_BASE_FORMAT_SBC4				= 0x89000000U,	///< Block compressed SBC4, 4 bits per pixel
	SCE_GXM_TEXTURE_BASE_FORMAT_UBC5				= 0x8A000000U,	///< Block compressed UBC5, 8 bits per pixel
	SCE_GXM_TEXTURE_BASE_FORMAT_SBC5				= 0x8B000000U,	///< Block compressed SBC5, 8 bits per pixel
	SCE_GXM_TEXTURE_BASE_FORMAT_YUV420P2			= 0x90000000U,	///< Y plane, interleaved UV plane
	SCE_GXM_TEXTURE_BASE_FORMAT_YUV420P3			= 0x91000000U,	///< Y plane, U plane, V plane
	SCE_GXM_TEXTURE_BASE_FORMAT_YUV422				= 0x92000000U,	///< Interleaved YUV
	SCE_GXM_TEXTURE_BASE_FORMAT_P4					= 0x94000000U,	///< Palettized format, 4-bit palette index per pixel
	SCE_GXM_TEXTURE_BASE_FORMAT_P8					= 0x95000000U,	///< Palettized format, 8-bit palette index per pixel
	SCE_GXM_TEXTURE_BASE_FORMAT_U8U8U8				= 0x98000000U,	///< 24-bit packed format, 3x 8-bit unsigned integer
	SCE_GXM_TEXTURE_BASE_FORMAT_S8S8S8				= 0x99000000U,	///< 24-bit packed format, 3x 8-bit signed integer
	SCE_GXM_TEXTURE_BASE_FORMAT_U2F10F10F10			= 0x9a000000U	///< 32-bit format, 2-bit unsigned integer and 3x 10-bit s0e5m5 floating point
} SceGxmTextureBaseFormat;

/** The texture formats.

	These are split into two sections: the full list of all texture formats 
	supported by the hardware, followed by some legacy defines for convenience.
	The full list uses a standard syntax of <c>FORMAT_SWIZZLE</c>.

	The format part of the name is written for high-to-low bit ordering assuming
	the value is in a register.  Note that registers are stored in memory in a 
	little-endian format.

	For 4 and 3-component formats in memory, the swizzle part of the name is the
	component ordering in the value loaded from memory.  For example, a texel of
	format #SCE_GXM_TEXTURE_FORMAT_U4U4U4U4_ABGR would have A in the high 4 bits
	and R in the low 4 bits if the 16-bit value was loaded into a register.

	For 2 and 1-component formats in memory, the format in memory is always GR or 
	R and the swizzle represents the mapping to an ABGR (or WZYX) result in the 
	shader code.  For example, the format #SCE_GXM_TEXTURE_FORMAT_U8_000R would 
	return zero in the ABG components and the U8 value in the R component.
	
	The depth/stencil format X8U24 is an exception and behaves like the 4 and
	3-component formats: the swizzle part of the name is the component ordering
	in memory, and the result is always returned with D in the x component.  Because of this,
	the format can only be used with single-component query formats.

	For a full table of all texture base formats, swizzles and supported query
	formats please refer to the <em>GPU User¡¯s Guide</em>, Appendix A.

	@ingroup render
*/
typedef enum SceGxmTextureFormat {
	// Full list of supported texture formats

	SCE_GXM_TEXTURE_FORMAT_U8_000R				= SCE_GXM_TEXTURE_BASE_FORMAT_U8 | SCE_GXM_TEXTURE_SWIZZLE1_000R,			///< The U8 value is swizzled to 000R (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_U8_111R				= SCE_GXM_TEXTURE_BASE_FORMAT_U8 | SCE_GXM_TEXTURE_SWIZZLE1_111R,			///< The U8 value is swizzled to 111R (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_U8_RRRR				= SCE_GXM_TEXTURE_BASE_FORMAT_U8 | SCE_GXM_TEXTURE_SWIZZLE1_RRRR,			///< The U8 value is swizzled to RRRR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_U8_0RRR				= SCE_GXM_TEXTURE_BASE_FORMAT_U8 | SCE_GXM_TEXTURE_SWIZZLE1_0RRR,			///< The U8 value is swizzled to 0RRR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_U8_1RRR				= SCE_GXM_TEXTURE_BASE_FORMAT_U8 | SCE_GXM_TEXTURE_SWIZZLE1_1RRR,			///< The U8 value is swizzled to 1RRR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_U8_R000				= SCE_GXM_TEXTURE_BASE_FORMAT_U8 | SCE_GXM_TEXTURE_SWIZZLE1_R000,			///< The U8 value is swizzled to R000 (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_U8_R111				= SCE_GXM_TEXTURE_BASE_FORMAT_U8 | SCE_GXM_TEXTURE_SWIZZLE1_R111,			///< The U8 value is swizzled to R111 (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_U8_R					= SCE_GXM_TEXTURE_BASE_FORMAT_U8 | SCE_GXM_TEXTURE_SWIZZLE1_R,				///< The U8 value is returned as a single component result.

	SCE_GXM_TEXTURE_FORMAT_S8_000R				= SCE_GXM_TEXTURE_BASE_FORMAT_S8 | SCE_GXM_TEXTURE_SWIZZLE1_000R,			///< The S8 value is swizzled to 000R (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_S8_111R				= SCE_GXM_TEXTURE_BASE_FORMAT_S8 | SCE_GXM_TEXTURE_SWIZZLE1_111R,			///< The S8 value is swizzled to 111R (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_S8_RRRR				= SCE_GXM_TEXTURE_BASE_FORMAT_S8 | SCE_GXM_TEXTURE_SWIZZLE1_RRRR,			///< The S8 value is swizzled to RRRR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_S8_0RRR				= SCE_GXM_TEXTURE_BASE_FORMAT_S8 | SCE_GXM_TEXTURE_SWIZZLE1_0RRR,			///< The S8 value is swizzled to 0RRR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_S8_1RRR				= SCE_GXM_TEXTURE_BASE_FORMAT_S8 | SCE_GXM_TEXTURE_SWIZZLE1_1RRR,			///< The S8 value is swizzled to 1RRR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_S8_R000				= SCE_GXM_TEXTURE_BASE_FORMAT_S8 | SCE_GXM_TEXTURE_SWIZZLE1_R000,			///< The S8 value is swizzled to R000 (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_S8_R111				= SCE_GXM_TEXTURE_BASE_FORMAT_S8 | SCE_GXM_TEXTURE_SWIZZLE1_R111,			///< The S8 value is swizzled to R111 (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_S8_R					= SCE_GXM_TEXTURE_BASE_FORMAT_S8 | SCE_GXM_TEXTURE_SWIZZLE1_R,	 			///< The S8 value is returned as a single component result.

	SCE_GXM_TEXTURE_FORMAT_U4U4U4U4_ABGR		= SCE_GXM_TEXTURE_BASE_FORMAT_U4U4U4U4 | SCE_GXM_TEXTURE_SWIZZLE4_ABGR,		///< The U4U4U4U4 data is read in ABGR order from memory.
	SCE_GXM_TEXTURE_FORMAT_U4U4U4U4_ARGB		= SCE_GXM_TEXTURE_BASE_FORMAT_U4U4U4U4 | SCE_GXM_TEXTURE_SWIZZLE4_ARGB,		///< The U4U4U4U4 data is read in ARGB order from memory.
	SCE_GXM_TEXTURE_FORMAT_U4U4U4U4_RGBA		= SCE_GXM_TEXTURE_BASE_FORMAT_U4U4U4U4 | SCE_GXM_TEXTURE_SWIZZLE4_RGBA,		///< The U4U4U4U4 data is read in RGBA order from memory.
	SCE_GXM_TEXTURE_FORMAT_U4U4U4U4_BGRA		= SCE_GXM_TEXTURE_BASE_FORMAT_U4U4U4U4 | SCE_GXM_TEXTURE_SWIZZLE4_BGRA,		///< The U4U4U4U4 data is read in BGRA order from memory.
	SCE_GXM_TEXTURE_FORMAT_X4U4U4U4_1BGR		= SCE_GXM_TEXTURE_BASE_FORMAT_U4U4U4U4 | SCE_GXM_TEXTURE_SWIZZLE4_1BGR,		///< The U4U4U4U4 data is read in ABGR order from memory, A is replaced with 0xf.
	SCE_GXM_TEXTURE_FORMAT_X4U4U4U4_1RGB		= SCE_GXM_TEXTURE_BASE_FORMAT_U4U4U4U4 | SCE_GXM_TEXTURE_SWIZZLE4_1RGB,		///< The U4U4U4U4 data is read in ARGB order from memory, A is replaced with 0xf.
	SCE_GXM_TEXTURE_FORMAT_U4U4U4X4_RGB1		= SCE_GXM_TEXTURE_BASE_FORMAT_U4U4U4U4 | SCE_GXM_TEXTURE_SWIZZLE4_RGB1,		///< The U4U4U4U4 data is read in RGBA order from memory, A is replaced with 0xf.
	SCE_GXM_TEXTURE_FORMAT_U4U4U4X4_BGR1		= SCE_GXM_TEXTURE_BASE_FORMAT_U4U4U4U4 | SCE_GXM_TEXTURE_SWIZZLE4_BGR1,		///< The U4U4U4U4 data is read in BGRA order from memory, A is replaced with 0xf.

	SCE_GXM_TEXTURE_FORMAT_U8U3U3U2_ARGB		= SCE_GXM_TEXTURE_BASE_FORMAT_U8U3U3U2,										///< The U8U3U3U2 data is read in ARGB order from memory.

	SCE_GXM_TEXTURE_FORMAT_U1U5U5U5_ABGR		= SCE_GXM_TEXTURE_BASE_FORMAT_U1U5U5U5 | SCE_GXM_TEXTURE_SWIZZLE4_ABGR,		///< The U1U5U5U5 data is read in ABGR order from memory.
	SCE_GXM_TEXTURE_FORMAT_U1U5U5U5_ARGB		= SCE_GXM_TEXTURE_BASE_FORMAT_U1U5U5U5 | SCE_GXM_TEXTURE_SWIZZLE4_ARGB,		///< The U1U5U5U5 data is read in ARGB order from memory.
	SCE_GXM_TEXTURE_FORMAT_U5U5U5U1_RGBA		= SCE_GXM_TEXTURE_BASE_FORMAT_U1U5U5U5 | SCE_GXM_TEXTURE_SWIZZLE4_RGBA,		///< The U5U5U5U1 data is read in RGBA order from memory.
	SCE_GXM_TEXTURE_FORMAT_U5U5U5U1_BGRA		= SCE_GXM_TEXTURE_BASE_FORMAT_U1U5U5U5 | SCE_GXM_TEXTURE_SWIZZLE4_BGRA,		///< The U5U5U5U1 data is read in BGRA order from memory.
	SCE_GXM_TEXTURE_FORMAT_X1U5U5U5_1BGR		= SCE_GXM_TEXTURE_BASE_FORMAT_U1U5U5U5 | SCE_GXM_TEXTURE_SWIZZLE4_1BGR,		///< The U1U5U5U5 data is read in ABGR order from memory, A is replaced with 1.
	SCE_GXM_TEXTURE_FORMAT_X1U5U5U5_1RGB		= SCE_GXM_TEXTURE_BASE_FORMAT_U1U5U5U5 | SCE_GXM_TEXTURE_SWIZZLE4_1RGB,		///< The U1U5U5U5 data is read in ARGB order from memory, A is replaced with 1.
	SCE_GXM_TEXTURE_FORMAT_U5U5U5X1_RGB1		= SCE_GXM_TEXTURE_BASE_FORMAT_U1U5U5U5 | SCE_GXM_TEXTURE_SWIZZLE4_RGB1,		///< The U5U5U5U1 data is read in RGBA order from memory, A is replaced with 1.
	SCE_GXM_TEXTURE_FORMAT_U5U5U5X1_BGR1		= SCE_GXM_TEXTURE_BASE_FORMAT_U1U5U5U5 | SCE_GXM_TEXTURE_SWIZZLE4_BGR1,		///< The U5U5U5U1 data is read in BGRA order from memory, A is replaced with 1.

	SCE_GXM_TEXTURE_FORMAT_U5U6U5_BGR			= SCE_GXM_TEXTURE_BASE_FORMAT_U5U6U5 | SCE_GXM_TEXTURE_SWIZZLE3_BGR,		///< The U5U6U5 data is read in BGR order from memory, A is implicit and assigned 1.
	SCE_GXM_TEXTURE_FORMAT_U5U6U5_RGB			= SCE_GXM_TEXTURE_BASE_FORMAT_U5U6U5 | SCE_GXM_TEXTURE_SWIZZLE3_RGB,		///< The U5U6U5 data is read in RGB order from memory, A is implicit and assigned 1.

	SCE_GXM_TEXTURE_FORMAT_U6S5S5_BGR			= SCE_GXM_TEXTURE_BASE_FORMAT_S5S5U6 | SCE_GXM_TEXTURE_SWIZZLE3_BGR,		///< The U6S5S5 data is read in BGR order from memory, A is implicit and assigned 1.
	SCE_GXM_TEXTURE_FORMAT_S5S5U6_RGB			= SCE_GXM_TEXTURE_BASE_FORMAT_S5S5U6 | SCE_GXM_TEXTURE_SWIZZLE3_RGB,		///< The S5S5U6 data is read in RGB order from memory, A is implicit and assigned 1.

	SCE_GXM_TEXTURE_FORMAT_U8U8_00GR			= SCE_GXM_TEXTURE_BASE_FORMAT_U8U8 | SCE_GXM_TEXTURE_SWIZZLE2_00GR,			///< The U8U8 (GR) value is swizzled to 00GR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_U8U8_GRRR			= SCE_GXM_TEXTURE_BASE_FORMAT_U8U8 | SCE_GXM_TEXTURE_SWIZZLE2_GRRR,			///< The U8U8 (GR) value is swizzled to GRRR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_U8U8_RGGG			= SCE_GXM_TEXTURE_BASE_FORMAT_U8U8 | SCE_GXM_TEXTURE_SWIZZLE2_RGGG,			///< The U8U8 (GR) value is swizzled to RGGG (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_U8U8_GRGR			= SCE_GXM_TEXTURE_BASE_FORMAT_U8U8 | SCE_GXM_TEXTURE_SWIZZLE2_GRGR,			///< The U8U8 (GR) value is swizzled to GRGR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_U8U8_00RG			= SCE_GXM_TEXTURE_BASE_FORMAT_U8U8 | SCE_GXM_TEXTURE_SWIZZLE2_00RG,			///< The U8U8 (GR) value is swizzled to 00RG (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_U8U8_GR				= SCE_GXM_TEXTURE_BASE_FORMAT_U8U8 | SCE_GXM_TEXTURE_SWIZZLE2_GR,			///< The U8U8 (GR) value is returned as a 2-component result.

	SCE_GXM_TEXTURE_FORMAT_S8S8_00GR			= SCE_GXM_TEXTURE_BASE_FORMAT_S8S8 | SCE_GXM_TEXTURE_SWIZZLE2_00GR,			///< The S8S8 (GR) value is swizzled to 00GR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_S8S8_GRRR			= SCE_GXM_TEXTURE_BASE_FORMAT_S8S8 | SCE_GXM_TEXTURE_SWIZZLE2_GRRR,			///< The S8S8 (GR) value is swizzled to GRRR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_S8S8_RGGG			= SCE_GXM_TEXTURE_BASE_FORMAT_S8S8 | SCE_GXM_TEXTURE_SWIZZLE2_RGGG,			///< The S8S8 (GR) value is swizzled to RGGG (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_S8S8_GRGR			= SCE_GXM_TEXTURE_BASE_FORMAT_S8S8 | SCE_GXM_TEXTURE_SWIZZLE2_GRGR,			///< The S8S8 (GR) value is swizzled to GRGR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_S8S8_00RG			= SCE_GXM_TEXTURE_BASE_FORMAT_S8S8 | SCE_GXM_TEXTURE_SWIZZLE2_00RG,			///< The S8S8 (GR) value is swizzled to 00RG (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_S8S8_GR				= SCE_GXM_TEXTURE_BASE_FORMAT_S8S8 | SCE_GXM_TEXTURE_SWIZZLE2_GR,   		///< The S8S8 (GR) value is returned as a 2-component result.

	SCE_GXM_TEXTURE_FORMAT_U16_000R				= SCE_GXM_TEXTURE_BASE_FORMAT_U16 | SCE_GXM_TEXTURE_SWIZZLE1_000R,			///< The U16 value is swizzled to 000R (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_U16_111R				= SCE_GXM_TEXTURE_BASE_FORMAT_U16 | SCE_GXM_TEXTURE_SWIZZLE1_111R,			///< The U16 value is swizzled to 111R (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_U16_RRRR				= SCE_GXM_TEXTURE_BASE_FORMAT_U16 | SCE_GXM_TEXTURE_SWIZZLE1_RRRR,			///< The U16 value is swizzled to RRRR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_U16_0RRR				= SCE_GXM_TEXTURE_BASE_FORMAT_U16 | SCE_GXM_TEXTURE_SWIZZLE1_0RRR,			///< The U16 value is swizzled to 0RRR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_U16_1RRR				= SCE_GXM_TEXTURE_BASE_FORMAT_U16 | SCE_GXM_TEXTURE_SWIZZLE1_1RRR,			///< The U16 value is swizzled to 1RRR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_U16_R000				= SCE_GXM_TEXTURE_BASE_FORMAT_U16 | SCE_GXM_TEXTURE_SWIZZLE1_R000,			///< The U16 value is swizzled to R000 (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_U16_R111				= SCE_GXM_TEXTURE_BASE_FORMAT_U16 | SCE_GXM_TEXTURE_SWIZZLE1_R111,			///< The U16 value is swizzled to R111 (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_U16_R				= SCE_GXM_TEXTURE_BASE_FORMAT_U16 | SCE_GXM_TEXTURE_SWIZZLE1_R,	  			///< The U16 value is returned as a single component result.

	SCE_GXM_TEXTURE_FORMAT_S16_000R				= SCE_GXM_TEXTURE_BASE_FORMAT_S16 | SCE_GXM_TEXTURE_SWIZZLE1_000R,			///< The S16 value is swizzled to 000R (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_S16_111R				= SCE_GXM_TEXTURE_BASE_FORMAT_S16 | SCE_GXM_TEXTURE_SWIZZLE1_111R,			///< The S16 value is swizzled to 111R (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_S16_RRRR				= SCE_GXM_TEXTURE_BASE_FORMAT_S16 | SCE_GXM_TEXTURE_SWIZZLE1_RRRR,			///< The S16 value is swizzled to RRRR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_S16_0RRR				= SCE_GXM_TEXTURE_BASE_FORMAT_S16 | SCE_GXM_TEXTURE_SWIZZLE1_0RRR,			///< The S16 value is swizzled to 0RRR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_S16_1RRR				= SCE_GXM_TEXTURE_BASE_FORMAT_S16 | SCE_GXM_TEXTURE_SWIZZLE1_1RRR,			///< The S16 value is swizzled to 1RRR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_S16_R000				= SCE_GXM_TEXTURE_BASE_FORMAT_S16 | SCE_GXM_TEXTURE_SWIZZLE1_R000,			///< The S16 value is swizzled to R000 (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_S16_R111				= SCE_GXM_TEXTURE_BASE_FORMAT_S16 | SCE_GXM_TEXTURE_SWIZZLE1_R111,			///< The S16 value is swizzled to R111 (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_S16_R				= SCE_GXM_TEXTURE_BASE_FORMAT_S16 | SCE_GXM_TEXTURE_SWIZZLE1_R,	  			///< The S16 value is returned as a single component result.

	SCE_GXM_TEXTURE_FORMAT_F16_000R				= SCE_GXM_TEXTURE_BASE_FORMAT_F16 | SCE_GXM_TEXTURE_SWIZZLE1_000R,			///< The F16 value is swizzled to 000R (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_F16_111R				= SCE_GXM_TEXTURE_BASE_FORMAT_F16 | SCE_GXM_TEXTURE_SWIZZLE1_111R,			///< The F16 value is swizzled to 111R (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_F16_RRRR				= SCE_GXM_TEXTURE_BASE_FORMAT_F16 | SCE_GXM_TEXTURE_SWIZZLE1_RRRR,			///< The F16 value is swizzled to RRRR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_F16_0RRR				= SCE_GXM_TEXTURE_BASE_FORMAT_F16 | SCE_GXM_TEXTURE_SWIZZLE1_0RRR,			///< The F16 value is swizzled to 0RRR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_F16_1RRR				= SCE_GXM_TEXTURE_BASE_FORMAT_F16 | SCE_GXM_TEXTURE_SWIZZLE1_1RRR,			///< The F16 value is swizzled to 1RRR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_F16_R000				= SCE_GXM_TEXTURE_BASE_FORMAT_F16 | SCE_GXM_TEXTURE_SWIZZLE1_R000,			///< The F16 value is swizzled to R000 (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_F16_R111				= SCE_GXM_TEXTURE_BASE_FORMAT_F16 | SCE_GXM_TEXTURE_SWIZZLE1_R111,			///< The F16 value is swizzled to R111 (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_F16_R				= SCE_GXM_TEXTURE_BASE_FORMAT_F16 | SCE_GXM_TEXTURE_SWIZZLE1_R,	  			///< The F16 value is returned as a single component result.

	SCE_GXM_TEXTURE_FORMAT_U8U8U8U8_ABGR		= SCE_GXM_TEXTURE_BASE_FORMAT_U8U8U8U8 | SCE_GXM_TEXTURE_SWIZZLE4_ABGR,		///< The U8U8U8U8 data is read in ABGR order from memory.
	SCE_GXM_TEXTURE_FORMAT_U8U8U8U8_ARGB		= SCE_GXM_TEXTURE_BASE_FORMAT_U8U8U8U8 | SCE_GXM_TEXTURE_SWIZZLE4_ARGB,		///< The U8U8U8U8 data is read in ARGB order from memory.
	SCE_GXM_TEXTURE_FORMAT_U8U8U8U8_RGBA		= SCE_GXM_TEXTURE_BASE_FORMAT_U8U8U8U8 | SCE_GXM_TEXTURE_SWIZZLE4_RGBA,		///< The U8U8U8U8 data is read in RGBA order from memory.
	SCE_GXM_TEXTURE_FORMAT_U8U8U8U8_BGRA		= SCE_GXM_TEXTURE_BASE_FORMAT_U8U8U8U8 | SCE_GXM_TEXTURE_SWIZZLE4_BGRA,		///< The U8U8U8U8 data is read in BGRA order from memory.
	SCE_GXM_TEXTURE_FORMAT_X8U8U8U8_1BGR		= SCE_GXM_TEXTURE_BASE_FORMAT_U8U8U8U8 | SCE_GXM_TEXTURE_SWIZZLE4_1BGR,		///< The U8U8U8U8 data is read in ABGR order from memory, A is replaced with 0xff.
	SCE_GXM_TEXTURE_FORMAT_X8U8U8U8_1RGB		= SCE_GXM_TEXTURE_BASE_FORMAT_U8U8U8U8 | SCE_GXM_TEXTURE_SWIZZLE4_1RGB,		///< The U8U8U8U8 data is read in ARGB order from memory, A is replaced with 0xff.
	SCE_GXM_TEXTURE_FORMAT_U8U8U8X8_RGB1		= SCE_GXM_TEXTURE_BASE_FORMAT_U8U8U8U8 | SCE_GXM_TEXTURE_SWIZZLE4_RGB1,		///< The U8U8U8U8 data is read in RGBA order from memory, A is replaced with 0xff.
	SCE_GXM_TEXTURE_FORMAT_U8U8U8X8_BGR1		= SCE_GXM_TEXTURE_BASE_FORMAT_U8U8U8U8 | SCE_GXM_TEXTURE_SWIZZLE4_BGR1,		///< The U8U8U8U8 data is read in BGRA order from memory, A is replaced with 0xff.

	SCE_GXM_TEXTURE_FORMAT_S8S8S8S8_ABGR		= SCE_GXM_TEXTURE_BASE_FORMAT_S8S8S8S8 | SCE_GXM_TEXTURE_SWIZZLE4_ABGR,		///< The S8S8S8S8 data is read in ABGR order from memory.
	SCE_GXM_TEXTURE_FORMAT_S8S8S8S8_ARGB		= SCE_GXM_TEXTURE_BASE_FORMAT_S8S8S8S8 | SCE_GXM_TEXTURE_SWIZZLE4_ARGB,		///< The S8S8S8S8 data is read in ARGB order from memory.
	SCE_GXM_TEXTURE_FORMAT_S8S8S8S8_RGBA		= SCE_GXM_TEXTURE_BASE_FORMAT_S8S8S8S8 | SCE_GXM_TEXTURE_SWIZZLE4_RGBA,		///< The S8S8S8S8 data is read in RGBA order from memory.
	SCE_GXM_TEXTURE_FORMAT_S8S8S8S8_BGRA		= SCE_GXM_TEXTURE_BASE_FORMAT_S8S8S8S8 | SCE_GXM_TEXTURE_SWIZZLE4_BGRA,		///< The S8S8S8S8 data is read in BGRA order from memory.
	SCE_GXM_TEXTURE_FORMAT_X8S8S8S8_1BGR		= SCE_GXM_TEXTURE_BASE_FORMAT_S8S8S8S8 | SCE_GXM_TEXTURE_SWIZZLE4_1BGR,		///< The S8S8S8S8 data is read in ABGR order from memory, A is replaced with 0x7f.
	SCE_GXM_TEXTURE_FORMAT_X8S8S8S8_1RGB		= SCE_GXM_TEXTURE_BASE_FORMAT_S8S8S8S8 | SCE_GXM_TEXTURE_SWIZZLE4_1RGB,		///< The S8S8S8S8 data is read in ARGB order from memory, A is replaced with 0x7f.
	SCE_GXM_TEXTURE_FORMAT_S8S8S8X8_RGB1		= SCE_GXM_TEXTURE_BASE_FORMAT_S8S8S8S8 | SCE_GXM_TEXTURE_SWIZZLE4_RGB1,		///< The S8S8S8S8 data is read in RGBA order from memory, A is replaced with 0x7f.
	SCE_GXM_TEXTURE_FORMAT_S8S8S8X8_BGR1		= SCE_GXM_TEXTURE_BASE_FORMAT_S8S8S8S8 | SCE_GXM_TEXTURE_SWIZZLE4_BGR1,		///< The S8S8S8S8 data is read in BGRA order from memory, A is replaced with 0x7f.

	SCE_GXM_TEXTURE_FORMAT_U2U10U10U10_ABGR		= SCE_GXM_TEXTURE_BASE_FORMAT_U2U10U10U10 | SCE_GXM_TEXTURE_SWIZZLE4_ABGR,	///< The U2U10U10U10 data is read in ABGR order from memory.	
	SCE_GXM_TEXTURE_FORMAT_U2U10U10U10_ARGB		= SCE_GXM_TEXTURE_BASE_FORMAT_U2U10U10U10 | SCE_GXM_TEXTURE_SWIZZLE4_ARGB,	///< The U2U10U10U10 data is read in ARGB order from memory.
	SCE_GXM_TEXTURE_FORMAT_U10U10U10U2_RGBA		= SCE_GXM_TEXTURE_BASE_FORMAT_U2U10U10U10 | SCE_GXM_TEXTURE_SWIZZLE4_RGBA,	///< The U10U10U10U2 data is read in RGBA order from memory.
	SCE_GXM_TEXTURE_FORMAT_U10U10U10U2_BGRA		= SCE_GXM_TEXTURE_BASE_FORMAT_U2U10U10U10 | SCE_GXM_TEXTURE_SWIZZLE4_BGRA,	///< The U10U10U10U2 data is read in BGRA order from memory.
	SCE_GXM_TEXTURE_FORMAT_X2U10U10U10_1BGR		= SCE_GXM_TEXTURE_BASE_FORMAT_U2U10U10U10 | SCE_GXM_TEXTURE_SWIZZLE4_1BGR,	///< The U2U10U10U10 data is read in ABGR order from memory, A is replaced with 0x3.
	SCE_GXM_TEXTURE_FORMAT_X2U10U10U10_1RGB		= SCE_GXM_TEXTURE_BASE_FORMAT_U2U10U10U10 | SCE_GXM_TEXTURE_SWIZZLE4_1RGB,	///< The U2U10U10U10 data is read in ARGB order from memory, A is replaced with 0x3.
	SCE_GXM_TEXTURE_FORMAT_U10U10U10X2_RGB1		= SCE_GXM_TEXTURE_BASE_FORMAT_U2U10U10U10 | SCE_GXM_TEXTURE_SWIZZLE4_RGB1,	///< The U10U10U10U2 data is read in RGBA order from memory, A is replaced with 0x3.
	SCE_GXM_TEXTURE_FORMAT_U10U10U10X2_BGR1		= SCE_GXM_TEXTURE_BASE_FORMAT_U2U10U10U10 | SCE_GXM_TEXTURE_SWIZZLE4_BGR1,	///< The U10U10U10U2 data is read in BGRA order from memory, A is replaced with 0x3.

	SCE_GXM_TEXTURE_FORMAT_U16U16_00GR			= SCE_GXM_TEXTURE_BASE_FORMAT_U16U16 | SCE_GXM_TEXTURE_SWIZZLE2_00GR,		///< The U16U16 (GR) value is swizzled to 00GR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_U16U16_GRRR			= SCE_GXM_TEXTURE_BASE_FORMAT_U16U16 | SCE_GXM_TEXTURE_SWIZZLE2_GRRR,		///< The U16U16 (GR) value is swizzled to GRRR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_U16U16_RGGG			= SCE_GXM_TEXTURE_BASE_FORMAT_U16U16 | SCE_GXM_TEXTURE_SWIZZLE2_RGGG,		///< The U16U16 (GR) value is swizzled to RGGG (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_U16U16_GRGR			= SCE_GXM_TEXTURE_BASE_FORMAT_U16U16 | SCE_GXM_TEXTURE_SWIZZLE2_GRGR,		///< The U16U16 (GR) value is swizzled to GRGR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_U16U16_00RG			= SCE_GXM_TEXTURE_BASE_FORMAT_U16U16 | SCE_GXM_TEXTURE_SWIZZLE2_00RG,		///< The U16U16 (GR) value is swizzled to 00RG (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_U16U16_GR			= SCE_GXM_TEXTURE_BASE_FORMAT_U16U16 | SCE_GXM_TEXTURE_SWIZZLE2_GR,	  		///< The U16U16 (GR) value is returned as a 2-component result.

	SCE_GXM_TEXTURE_FORMAT_S16S16_00GR			= SCE_GXM_TEXTURE_BASE_FORMAT_S16S16 | SCE_GXM_TEXTURE_SWIZZLE2_00GR,		///< The S16S16 (GR) value is swizzled to 00GR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_S16S16_GRRR			= SCE_GXM_TEXTURE_BASE_FORMAT_S16S16 | SCE_GXM_TEXTURE_SWIZZLE2_GRRR,		///< The S16S16 (GR) value is swizzled to GRRR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_S16S16_RGGG			= SCE_GXM_TEXTURE_BASE_FORMAT_S16S16 | SCE_GXM_TEXTURE_SWIZZLE2_RGGG,		///< The S16S16 (GR) value is swizzled to RGGG (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_S16S16_GRGR			= SCE_GXM_TEXTURE_BASE_FORMAT_S16S16 | SCE_GXM_TEXTURE_SWIZZLE2_GRGR,		///< The S16S16 (GR) value is swizzled to GRGR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_S16S16_00RG			= SCE_GXM_TEXTURE_BASE_FORMAT_S16S16 | SCE_GXM_TEXTURE_SWIZZLE2_00RG,		///< The S16S16 (GR) value is swizzled to 00RG (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_S16S16_GR			= SCE_GXM_TEXTURE_BASE_FORMAT_S16S16 | SCE_GXM_TEXTURE_SWIZZLE2_GR,	  		///< The S16S16 (GR) value is returned as a 2-component result.

	SCE_GXM_TEXTURE_FORMAT_F16F16_00GR			= SCE_GXM_TEXTURE_BASE_FORMAT_F16F16 | SCE_GXM_TEXTURE_SWIZZLE2_00GR,		///< The F16F16 (GR) value is swizzled to 00GR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_F16F16_GRRR			= SCE_GXM_TEXTURE_BASE_FORMAT_F16F16 | SCE_GXM_TEXTURE_SWIZZLE2_GRRR,		///< The F16F16 (GR) value is swizzled to GRRR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_F16F16_RGGG			= SCE_GXM_TEXTURE_BASE_FORMAT_F16F16 | SCE_GXM_TEXTURE_SWIZZLE2_RGGG,		///< The F16F16 (GR) value is swizzled to RGGG (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_F16F16_GRGR			= SCE_GXM_TEXTURE_BASE_FORMAT_F16F16 | SCE_GXM_TEXTURE_SWIZZLE2_GRGR,		///< The F16F16 (GR) value is swizzled to GRGR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_F16F16_00RG			= SCE_GXM_TEXTURE_BASE_FORMAT_F16F16 | SCE_GXM_TEXTURE_SWIZZLE2_00RG,		///< The F16F16 (GR) value is swizzled to 00RG (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_F16F16_GR			= SCE_GXM_TEXTURE_BASE_FORMAT_F16F16 | SCE_GXM_TEXTURE_SWIZZLE2_GR,	  		///< The F16F16 (GR) value is returned as a 2-component result.

	SCE_GXM_TEXTURE_FORMAT_F32_000R				= SCE_GXM_TEXTURE_BASE_FORMAT_F32 | SCE_GXM_TEXTURE_SWIZZLE1_000R,			///< The F32 value is swizzled to 000R (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_F32_111R				= SCE_GXM_TEXTURE_BASE_FORMAT_F32 | SCE_GXM_TEXTURE_SWIZZLE1_111R,			///< The F32 value is swizzled to 111R (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_F32_RRRR				= SCE_GXM_TEXTURE_BASE_FORMAT_F32 | SCE_GXM_TEXTURE_SWIZZLE1_RRRR,			///< The F32 value is swizzled to RRRR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_F32_0RRR				= SCE_GXM_TEXTURE_BASE_FORMAT_F32 | SCE_GXM_TEXTURE_SWIZZLE1_0RRR,			///< The F32 value is swizzled to 0RRR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_F32_1RRR				= SCE_GXM_TEXTURE_BASE_FORMAT_F32 | SCE_GXM_TEXTURE_SWIZZLE1_1RRR,			///< The F32 value is swizzled to 1RRR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_F32_R000				= SCE_GXM_TEXTURE_BASE_FORMAT_F32 | SCE_GXM_TEXTURE_SWIZZLE1_R000,			///< The F32 value is swizzled to R000 (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_F32_R111				= SCE_GXM_TEXTURE_BASE_FORMAT_F32 | SCE_GXM_TEXTURE_SWIZZLE1_R111,			///< The F32 value is swizzled to R111 (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_F32_R				= SCE_GXM_TEXTURE_BASE_FORMAT_F32 | SCE_GXM_TEXTURE_SWIZZLE1_R,	  			///< The F32 value is returned as a single component result.

	SCE_GXM_TEXTURE_FORMAT_F32M_000R			= SCE_GXM_TEXTURE_BASE_FORMAT_F32M | SCE_GXM_TEXTURE_SWIZZLE1_000R,			///< The F32M value is swizzled to 000R (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_F32M_111R			= SCE_GXM_TEXTURE_BASE_FORMAT_F32M | SCE_GXM_TEXTURE_SWIZZLE1_111R,			///< The F32M value is swizzled to 111R (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_F32M_RRRR			= SCE_GXM_TEXTURE_BASE_FORMAT_F32M | SCE_GXM_TEXTURE_SWIZZLE1_RRRR,			///< The F32M value is swizzled to RRRR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_F32M_0RRR			= SCE_GXM_TEXTURE_BASE_FORMAT_F32M | SCE_GXM_TEXTURE_SWIZZLE1_0RRR,			///< The F32M value is swizzled to 0RRR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_F32M_1RRR			= SCE_GXM_TEXTURE_BASE_FORMAT_F32M | SCE_GXM_TEXTURE_SWIZZLE1_1RRR,			///< The F32M value is swizzled to 1RRR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_F32M_R000			= SCE_GXM_TEXTURE_BASE_FORMAT_F32M | SCE_GXM_TEXTURE_SWIZZLE1_R000,			///< The F32M value is swizzled to R000 (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_F32M_R111			= SCE_GXM_TEXTURE_BASE_FORMAT_F32M | SCE_GXM_TEXTURE_SWIZZLE1_R111,			///< The F32M value is swizzled to R111 (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_F32M_R				= SCE_GXM_TEXTURE_BASE_FORMAT_F32M | SCE_GXM_TEXTURE_SWIZZLE1_R,   			///< The F32M value is returned as a single component result.

	SCE_GXM_TEXTURE_FORMAT_X8S8S8U8_1BGR		= SCE_GXM_TEXTURE_BASE_FORMAT_X8S8S8U8 | SCE_GXM_TEXTURE_SWIZZLE3_BGR,		///< The U8S8S8U8 data is read in ABGR order from memory, A is replaced with 0xff.
	SCE_GXM_TEXTURE_FORMAT_X8U8S8S8_1RGB		= SCE_GXM_TEXTURE_BASE_FORMAT_X8S8S8U8 | SCE_GXM_TEXTURE_SWIZZLE3_RGB,		///< The U8U8S8S8 data is read in ARGB order from memory, A is replaced with 0xff.

	SCE_GXM_TEXTURE_FORMAT_X8U24_SD				= SCE_GXM_TEXTURE_BASE_FORMAT_X8U24 | SCE_GXM_TEXTURE_SWIZZLE2_SD,			///< The U8U24 data is read in SD order, D is returned as a single component result.
	SCE_GXM_TEXTURE_FORMAT_U24X8_DS				= SCE_GXM_TEXTURE_BASE_FORMAT_X8U24 | SCE_GXM_TEXTURE_SWIZZLE2_DS,			///< The U24U8 data is read in DS order, D is returned as a single component result.

	SCE_GXM_TEXTURE_FORMAT_U32_000R				= SCE_GXM_TEXTURE_BASE_FORMAT_U32 | SCE_GXM_TEXTURE_SWIZZLE1_000R,			///< The U32 value is swizzled to 000R (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_U32_111R				= SCE_GXM_TEXTURE_BASE_FORMAT_U32 | SCE_GXM_TEXTURE_SWIZZLE1_111R,			///< The U32 value is swizzled to 111R (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_U32_RRRR				= SCE_GXM_TEXTURE_BASE_FORMAT_U32 | SCE_GXM_TEXTURE_SWIZZLE1_RRRR,			///< The U32 value is swizzled to RRRR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_U32_0RRR				= SCE_GXM_TEXTURE_BASE_FORMAT_U32 | SCE_GXM_TEXTURE_SWIZZLE1_0RRR,			///< The U32 value is swizzled to 0RRR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_U32_1RRR				= SCE_GXM_TEXTURE_BASE_FORMAT_U32 | SCE_GXM_TEXTURE_SWIZZLE1_1RRR,			///< The U32 value is swizzled to 1RRR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_U32_R000				= SCE_GXM_TEXTURE_BASE_FORMAT_U32 | SCE_GXM_TEXTURE_SWIZZLE1_R000,			///< The U32 value is swizzled to R000 (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_U32_R111				= SCE_GXM_TEXTURE_BASE_FORMAT_U32 | SCE_GXM_TEXTURE_SWIZZLE1_R111,			///< The U32 value is swizzled to R111 (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_U32_R				= SCE_GXM_TEXTURE_BASE_FORMAT_U32 | SCE_GXM_TEXTURE_SWIZZLE1_R,	  			///< The U32 value is returned as a single component result.

	SCE_GXM_TEXTURE_FORMAT_S32_000R				= SCE_GXM_TEXTURE_BASE_FORMAT_S32 | SCE_GXM_TEXTURE_SWIZZLE1_000R,			///< The S32 value is swizzled to 000R (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_S32_111R				= SCE_GXM_TEXTURE_BASE_FORMAT_S32 | SCE_GXM_TEXTURE_SWIZZLE1_111R,			///< The S32 value is swizzled to 111R (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_S32_RRRR				= SCE_GXM_TEXTURE_BASE_FORMAT_S32 | SCE_GXM_TEXTURE_SWIZZLE1_RRRR,			///< The S32 value is swizzled to RRRR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_S32_0RRR				= SCE_GXM_TEXTURE_BASE_FORMAT_S32 | SCE_GXM_TEXTURE_SWIZZLE1_0RRR,			///< The S32 value is swizzled to 0RRR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_S32_1RRR				= SCE_GXM_TEXTURE_BASE_FORMAT_S32 | SCE_GXM_TEXTURE_SWIZZLE1_1RRR,			///< The S32 value is swizzled to 1RRR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_S32_R000				= SCE_GXM_TEXTURE_BASE_FORMAT_S32 | SCE_GXM_TEXTURE_SWIZZLE1_R000,			///< The S32 value is swizzled to R000 (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_S32_R111				= SCE_GXM_TEXTURE_BASE_FORMAT_S32 | SCE_GXM_TEXTURE_SWIZZLE1_R111,			///< The S32 value is swizzled to R111 (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_S32_R				= SCE_GXM_TEXTURE_BASE_FORMAT_S32 | SCE_GXM_TEXTURE_SWIZZLE1_R,	  			///< The S32 value is returned as a single component result.

	SCE_GXM_TEXTURE_FORMAT_SE5M9M9M9_BGR		= SCE_GXM_TEXTURE_BASE_FORMAT_SE5M9M9M9 | SCE_GXM_TEXTURE_SWIZZLE3_BGR,		///< The SE5M9M9M9 data is read in EBGR order from memory, A is implicit and assigned 1.
	SCE_GXM_TEXTURE_FORMAT_SE5M9M9M9_RGB		= SCE_GXM_TEXTURE_BASE_FORMAT_SE5M9M9M9 | SCE_GXM_TEXTURE_SWIZZLE3_RGB,		///< The SE5M9M9M9 data is read in ERGB order from memory, A is implicit and assigned 1.

	SCE_GXM_TEXTURE_FORMAT_F10F11F11_BGR		= SCE_GXM_TEXTURE_BASE_FORMAT_F11F11F10 | SCE_GXM_TEXTURE_SWIZZLE3_BGR,		///< The F10F11F11 data is read in BGR order from memory, A is implicit and assigned 1.
	SCE_GXM_TEXTURE_FORMAT_F11F11F10_RGB		= SCE_GXM_TEXTURE_BASE_FORMAT_F11F11F10 | SCE_GXM_TEXTURE_SWIZZLE3_RGB,		///< The F11F11F10 data is read in RGB order from memory, A is implicit and assigned 1.

	SCE_GXM_TEXTURE_FORMAT_F16F16F16F16_ABGR	= SCE_GXM_TEXTURE_BASE_FORMAT_F16F16F16F16 | SCE_GXM_TEXTURE_SWIZZLE4_ABGR,	///< The F16F16F16F16 data is read in ABGR order from memory.
	SCE_GXM_TEXTURE_FORMAT_F16F16F16F16_ARGB	= SCE_GXM_TEXTURE_BASE_FORMAT_F16F16F16F16 | SCE_GXM_TEXTURE_SWIZZLE4_ARGB,	///< The F16F16F16F16 data is read in ARGB order from memory.
	SCE_GXM_TEXTURE_FORMAT_F16F16F16F16_RGBA	= SCE_GXM_TEXTURE_BASE_FORMAT_F16F16F16F16 | SCE_GXM_TEXTURE_SWIZZLE4_RGBA,	///< The F16F16F16F16 data is read in RGBA order from memory.
	SCE_GXM_TEXTURE_FORMAT_F16F16F16F16_BGRA	= SCE_GXM_TEXTURE_BASE_FORMAT_F16F16F16F16 | SCE_GXM_TEXTURE_SWIZZLE4_BGRA,	///< The F16F16F16F16 data is read in BGRA order from memory.
	SCE_GXM_TEXTURE_FORMAT_X16F16F16F16_1BGR	= SCE_GXM_TEXTURE_BASE_FORMAT_F16F16F16F16 | SCE_GXM_TEXTURE_SWIZZLE4_1BGR,	///< The F16F16F16F16 data is read in ABGR order from memory, A is replaced with 0x3c00.
	SCE_GXM_TEXTURE_FORMAT_X16F16F16F16_1RGB	= SCE_GXM_TEXTURE_BASE_FORMAT_F16F16F16F16 | SCE_GXM_TEXTURE_SWIZZLE4_1RGB,	///< The F16F16F16F16 data is read in ARGB order from memory, A is replaced with 0x3c00.
	SCE_GXM_TEXTURE_FORMAT_F16F16F16X16_RGB1	= SCE_GXM_TEXTURE_BASE_FORMAT_F16F16F16F16 | SCE_GXM_TEXTURE_SWIZZLE4_RGB1,	///< The F16F16F16F16 data is read in RGBA order from memory, A is replaced with 0x3c00.
	SCE_GXM_TEXTURE_FORMAT_F16F16F16X16_BGR1	= SCE_GXM_TEXTURE_BASE_FORMAT_F16F16F16F16 | SCE_GXM_TEXTURE_SWIZZLE4_BGR1,	///< The F16F16F16F16 data is read in BGRA order from memory, A is replaced with 0x3c00.

	SCE_GXM_TEXTURE_FORMAT_U16U16U16U16_ABGR	= SCE_GXM_TEXTURE_BASE_FORMAT_U16U16U16U16 | SCE_GXM_TEXTURE_SWIZZLE4_ABGR,	///< The U16U16U16U16 data is read in ABGR order from memory.
	SCE_GXM_TEXTURE_FORMAT_U16U16U16U16_ARGB	= SCE_GXM_TEXTURE_BASE_FORMAT_U16U16U16U16 | SCE_GXM_TEXTURE_SWIZZLE4_ARGB,	///< The U16U16U16U16 data is read in ARGB order from memory.
	SCE_GXM_TEXTURE_FORMAT_U16U16U16U16_RGBA	= SCE_GXM_TEXTURE_BASE_FORMAT_U16U16U16U16 | SCE_GXM_TEXTURE_SWIZZLE4_RGBA,	///< The U16U16U16U16 data is read in RGBA order from memory.
	SCE_GXM_TEXTURE_FORMAT_U16U16U16U16_BGRA	= SCE_GXM_TEXTURE_BASE_FORMAT_U16U16U16U16 | SCE_GXM_TEXTURE_SWIZZLE4_BGRA,	///< The U16U16U16U16 data is read in BGRA order from memory.
	SCE_GXM_TEXTURE_FORMAT_X16U16U16U16_1BGR	= SCE_GXM_TEXTURE_BASE_FORMAT_U16U16U16U16 | SCE_GXM_TEXTURE_SWIZZLE4_1BGR,	///< The U16U16U16U16 data is read in ABGR order from memory, A is replaced with 0xffff.
	SCE_GXM_TEXTURE_FORMAT_X16U16U16U16_1RGB	= SCE_GXM_TEXTURE_BASE_FORMAT_U16U16U16U16 | SCE_GXM_TEXTURE_SWIZZLE4_1RGB,	///< The U16U16U16U16 data is read in ARGB order from memory, A is replaced with 0xffff.
	SCE_GXM_TEXTURE_FORMAT_U16U16U16X16_RGB1	= SCE_GXM_TEXTURE_BASE_FORMAT_U16U16U16U16 | SCE_GXM_TEXTURE_SWIZZLE4_RGB1,	///< The U16U16U16U16 data is read in RGBA order from memory, A is replaced with 0xffff.
	SCE_GXM_TEXTURE_FORMAT_U16U16U16X16_BGR1	= SCE_GXM_TEXTURE_BASE_FORMAT_U16U16U16U16 | SCE_GXM_TEXTURE_SWIZZLE4_BGR1,	///< The U16U16U16U16 data is read in BGRA order from memory, A is replaced with 0xffff.

	SCE_GXM_TEXTURE_FORMAT_S16S16S16S16_ABGR	= SCE_GXM_TEXTURE_BASE_FORMAT_S16S16S16S16 | SCE_GXM_TEXTURE_SWIZZLE4_ABGR,	///< The S16S16S16S16 data is read in ABGR order from memory.
	SCE_GXM_TEXTURE_FORMAT_S16S16S16S16_ARGB	= SCE_GXM_TEXTURE_BASE_FORMAT_S16S16S16S16 | SCE_GXM_TEXTURE_SWIZZLE4_ARGB,	///< The S16S16S16S16 data is read in ARGB order from memory.
	SCE_GXM_TEXTURE_FORMAT_S16S16S16S16_RGBA	= SCE_GXM_TEXTURE_BASE_FORMAT_S16S16S16S16 | SCE_GXM_TEXTURE_SWIZZLE4_RGBA,	///< The S16S16S16S16 data is read in RGBA order from memory.
	SCE_GXM_TEXTURE_FORMAT_S16S16S16S16_BGRA	= SCE_GXM_TEXTURE_BASE_FORMAT_S16S16S16S16 | SCE_GXM_TEXTURE_SWIZZLE4_BGRA,	///< The S16S16S16S16 data is read in BGRA order from memory.
	SCE_GXM_TEXTURE_FORMAT_X16S16S16S16_1BGR	= SCE_GXM_TEXTURE_BASE_FORMAT_S16S16S16S16 | SCE_GXM_TEXTURE_SWIZZLE4_1BGR,	///< The S16S16S16S16 data is read in ABGR order from memory, A is replaced with 0x7fff.
	SCE_GXM_TEXTURE_FORMAT_X16S16S16S16_1RGB	= SCE_GXM_TEXTURE_BASE_FORMAT_S16S16S16S16 | SCE_GXM_TEXTURE_SWIZZLE4_1RGB,	///< The S16S16S16S16 data is read in ARGB order from memory, A is replaced with 0x7fff.
	SCE_GXM_TEXTURE_FORMAT_S16S16S16X16_RGB1	= SCE_GXM_TEXTURE_BASE_FORMAT_S16S16S16S16 | SCE_GXM_TEXTURE_SWIZZLE4_RGB1,	///< The S16S16S16S16 data is read in RGBA order from memory, A is replaced with 0x7fff.
	SCE_GXM_TEXTURE_FORMAT_S16S16S16X16_BGR1	= SCE_GXM_TEXTURE_BASE_FORMAT_S16S16S16S16 | SCE_GXM_TEXTURE_SWIZZLE4_BGR1,	///< The S16S16S16S16 data is read in BGRA order from memory, A is replaced with 0x7fff.

	SCE_GXM_TEXTURE_FORMAT_F32F32_00GR			= SCE_GXM_TEXTURE_BASE_FORMAT_F32F32 | SCE_GXM_TEXTURE_SWIZZLE2_00GR,		///< The F32F32 (GR) value is swizzled to 00GR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_F32F32_GRRR			= SCE_GXM_TEXTURE_BASE_FORMAT_F32F32 | SCE_GXM_TEXTURE_SWIZZLE2_GRRR,		///< The F32F32 (GR) value is swizzled to GRRR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_F32F32_RGGG			= SCE_GXM_TEXTURE_BASE_FORMAT_F32F32 | SCE_GXM_TEXTURE_SWIZZLE2_RGGG,		///< The F32F32 (GR) value is swizzled to RGGG (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_F32F32_GRGR			= SCE_GXM_TEXTURE_BASE_FORMAT_F32F32 | SCE_GXM_TEXTURE_SWIZZLE2_GRGR,		///< The F32F32 (GR) value is swizzled to GRGR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_F32F32_00RG			= SCE_GXM_TEXTURE_BASE_FORMAT_F32F32 | SCE_GXM_TEXTURE_SWIZZLE2_00RG,		///< The F32F32 (GR) value is swizzled to 00RG (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_F32F32_GR			= SCE_GXM_TEXTURE_BASE_FORMAT_F32F32 | SCE_GXM_TEXTURE_SWIZZLE2_GR,			///< The F32F32 (GR) value is returned as a 2-component result.

	SCE_GXM_TEXTURE_FORMAT_U32U32_00GR			= SCE_GXM_TEXTURE_BASE_FORMAT_U32U32 | SCE_GXM_TEXTURE_SWIZZLE2_00GR,		///< The U32U32 (GR) value is swizzled to 00GR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_U32U32_GRRR			= SCE_GXM_TEXTURE_BASE_FORMAT_U32U32 | SCE_GXM_TEXTURE_SWIZZLE2_GRRR,		///< The U32U32 (GR) value is swizzled to GRRR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_U32U32_RGGG			= SCE_GXM_TEXTURE_BASE_FORMAT_U32U32 | SCE_GXM_TEXTURE_SWIZZLE2_RGGG,		///< The U32U32 (GR) value is swizzled to RGGG (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_U32U32_GRGR			= SCE_GXM_TEXTURE_BASE_FORMAT_U32U32 | SCE_GXM_TEXTURE_SWIZZLE2_GRGR,		///< The U32U32 (GR) value is swizzled to GRGR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_U32U32_00RG			= SCE_GXM_TEXTURE_BASE_FORMAT_U32U32 | SCE_GXM_TEXTURE_SWIZZLE2_00RG,		///< The U32U32 (GR) value is swizzled to 00RG (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_U32U32_GR			= SCE_GXM_TEXTURE_BASE_FORMAT_U32U32 | SCE_GXM_TEXTURE_SWIZZLE2_GR,			///< The U32U32 (GR) value is returned as a 2-component result.

	SCE_GXM_TEXTURE_FORMAT_PVRT2BPP_ABGR		= SCE_GXM_TEXTURE_BASE_FORMAT_PVRT2BPP | SCE_GXM_TEXTURE_SWIZZLE4_ABGR,		///< The PVRT2BPP data is decoded into ABGR.
	SCE_GXM_TEXTURE_FORMAT_PVRT2BPP_1BGR		= SCE_GXM_TEXTURE_BASE_FORMAT_PVRT2BPP | SCE_GXM_TEXTURE_SWIZZLE4_1BGR,		///< The PVRT2BPP data is decoded into ABGR, A is replaced with 0xff.

	SCE_GXM_TEXTURE_FORMAT_PVRT4BPP_ABGR		= SCE_GXM_TEXTURE_BASE_FORMAT_PVRT4BPP | SCE_GXM_TEXTURE_SWIZZLE4_ABGR,		///< The PVRT4BPP data is decoded into ABGR.
	SCE_GXM_TEXTURE_FORMAT_PVRT4BPP_1BGR		= SCE_GXM_TEXTURE_BASE_FORMAT_PVRT4BPP | SCE_GXM_TEXTURE_SWIZZLE4_1BGR,		///< The PVRT4BPP data is decoded into ABGR, A is replaced with 0xff.

	SCE_GXM_TEXTURE_FORMAT_PVRTII2BPP_ABGR		= SCE_GXM_TEXTURE_BASE_FORMAT_PVRTII2BPP | SCE_GXM_TEXTURE_SWIZZLE4_ABGR,	///< The PVRTII2BPP data is decoded into ABGR.
	SCE_GXM_TEXTURE_FORMAT_PVRTII2BPP_1BGR		= SCE_GXM_TEXTURE_BASE_FORMAT_PVRTII2BPP | SCE_GXM_TEXTURE_SWIZZLE4_1BGR,	///< The PVRTII2BPP data is decoded into ABGR, A is replaced with 0xff.

	SCE_GXM_TEXTURE_FORMAT_PVRTII4BPP_ABGR		= SCE_GXM_TEXTURE_BASE_FORMAT_PVRTII4BPP | SCE_GXM_TEXTURE_SWIZZLE4_ABGR,	///< The PVRTII4BPP data is decoded into ABGR.
	SCE_GXM_TEXTURE_FORMAT_PVRTII4BPP_1BGR		= SCE_GXM_TEXTURE_BASE_FORMAT_PVRTII4BPP | SCE_GXM_TEXTURE_SWIZZLE4_1BGR,	///< The PVRTII4BPP data is decoded into ABGR, A is replaced with 0xff.

	SCE_GXM_TEXTURE_FORMAT_UBC1_ABGR			= SCE_GXM_TEXTURE_BASE_FORMAT_UBC1 | SCE_GXM_TEXTURE_SWIZZLE4_ABGR,			///< The UBC1 data is decoded into ABGR.
	SCE_GXM_TEXTURE_FORMAT_UBC1_1BGR			= SCE_GXM_TEXTURE_BASE_FORMAT_UBC1 | SCE_GXM_TEXTURE_SWIZZLE4_1BGR,			///< The UBC1 data is decoded into ABGR, A is replaced with 0xff.

	SCE_GXM_TEXTURE_FORMAT_UBC2_ABGR			= SCE_GXM_TEXTURE_BASE_FORMAT_UBC2 | SCE_GXM_TEXTURE_SWIZZLE4_ABGR,			///< The UBC2 data is decoded into ABGR.
	SCE_GXM_TEXTURE_FORMAT_UBC2_1BGR			= SCE_GXM_TEXTURE_BASE_FORMAT_UBC2 | SCE_GXM_TEXTURE_SWIZZLE4_1BGR,			///< The UBC2 data is decoded into ABGR, A is replaced with 0xff.

	SCE_GXM_TEXTURE_FORMAT_UBC3_ABGR			= SCE_GXM_TEXTURE_BASE_FORMAT_UBC3 | SCE_GXM_TEXTURE_SWIZZLE4_ABGR,			///< The UBC3 data is decoded into ABGR.
	SCE_GXM_TEXTURE_FORMAT_UBC3_1BGR			= SCE_GXM_TEXTURE_BASE_FORMAT_UBC3 | SCE_GXM_TEXTURE_SWIZZLE4_1BGR,			///< The UBC3 data is decoded into ABGR, A is replaced with 0xff.

	SCE_GXM_TEXTURE_FORMAT_UBC4_000R			= SCE_GXM_TEXTURE_BASE_FORMAT_UBC4 | SCE_GXM_TEXTURE_SWIZZLE1_000R,			///< The decoded UBC4 (R) value is swizzled to 000R (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_UBC4_111R			= SCE_GXM_TEXTURE_BASE_FORMAT_UBC4 | SCE_GXM_TEXTURE_SWIZZLE1_111R,			///< The decoded UBC4 (R) value is swizzled to 111R (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_UBC4_RRRR			= SCE_GXM_TEXTURE_BASE_FORMAT_UBC4 | SCE_GXM_TEXTURE_SWIZZLE1_RRRR,			///< The decoded UBC4 (R) value is swizzled to RRRR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_UBC4_0RRR			= SCE_GXM_TEXTURE_BASE_FORMAT_UBC4 | SCE_GXM_TEXTURE_SWIZZLE1_0RRR,			///< The decoded UBC4 (R) value is swizzled to 0RRR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_UBC4_1RRR			= SCE_GXM_TEXTURE_BASE_FORMAT_UBC4 | SCE_GXM_TEXTURE_SWIZZLE1_1RRR,			///< The decoded UBC4 (R) value is swizzled to 1RRR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_UBC4_R000			= SCE_GXM_TEXTURE_BASE_FORMAT_UBC4 | SCE_GXM_TEXTURE_SWIZZLE1_R000,			///< The decoded UBC4 (R) value is swizzled to R000 (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_UBC4_R111			= SCE_GXM_TEXTURE_BASE_FORMAT_UBC4 | SCE_GXM_TEXTURE_SWIZZLE1_R111,			///< The decoded UBC4 (R) value is swizzled to R111 (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_UBC4_R				= SCE_GXM_TEXTURE_BASE_FORMAT_UBC4 | SCE_GXM_TEXTURE_SWIZZLE1_R,			///< The decoded UBC4 (R) value is returned as a single component result.

	SCE_GXM_TEXTURE_FORMAT_SBC4_000R			= SCE_GXM_TEXTURE_BASE_FORMAT_SBC4 | SCE_GXM_TEXTURE_SWIZZLE1_000R,			///< The decoded SBC4 (R) value is swizzled to 000R (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_SBC4_111R			= SCE_GXM_TEXTURE_BASE_FORMAT_SBC4 | SCE_GXM_TEXTURE_SWIZZLE1_111R,			///< The decoded SBC4 (R) value is swizzled to 111R (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_SBC4_RRRR			= SCE_GXM_TEXTURE_BASE_FORMAT_SBC4 | SCE_GXM_TEXTURE_SWIZZLE1_RRRR,			///< The decoded SBC4 (R) value is swizzled to RRRR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_SBC4_0RRR			= SCE_GXM_TEXTURE_BASE_FORMAT_SBC4 | SCE_GXM_TEXTURE_SWIZZLE1_0RRR,			///< The decoded SBC4 (R) value is swizzled to 0RRR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_SBC4_1RRR			= SCE_GXM_TEXTURE_BASE_FORMAT_SBC4 | SCE_GXM_TEXTURE_SWIZZLE1_1RRR,			///< The decoded SBC4 (R) value is swizzled to 1RRR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_SBC4_R000			= SCE_GXM_TEXTURE_BASE_FORMAT_SBC4 | SCE_GXM_TEXTURE_SWIZZLE1_R000,			///< The decoded SBC4 (R) value is swizzled to R000 (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_SBC4_R111			= SCE_GXM_TEXTURE_BASE_FORMAT_SBC4 | SCE_GXM_TEXTURE_SWIZZLE1_R111,			///< The decoded SBC4 (R) value is swizzled to R111 (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_SBC4_R				= SCE_GXM_TEXTURE_BASE_FORMAT_SBC4 | SCE_GXM_TEXTURE_SWIZZLE1_R,			///< The decoded SBC4 (R) value is returned as a single component result.

	SCE_GXM_TEXTURE_FORMAT_UBC5_00GR			= SCE_GXM_TEXTURE_BASE_FORMAT_UBC5 | SCE_GXM_TEXTURE_SWIZZLE2_00GR,			///< The decoded UBC5 (GR) value is swizzled to 00GR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_UBC5_GRRR			= SCE_GXM_TEXTURE_BASE_FORMAT_UBC5 | SCE_GXM_TEXTURE_SWIZZLE2_GRRR,			///< The decoded UBC5 (GR) value is swizzled to GRRR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_UBC5_RGGG			= SCE_GXM_TEXTURE_BASE_FORMAT_UBC5 | SCE_GXM_TEXTURE_SWIZZLE2_RGGG,			///< The decoded UBC5 (GR) value is swizzled to RGGG (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_UBC5_GRGR			= SCE_GXM_TEXTURE_BASE_FORMAT_UBC5 | SCE_GXM_TEXTURE_SWIZZLE2_GRGR,			///< The decoded UBC5 (GR) value is swizzled to GRGR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_UBC5_00RG			= SCE_GXM_TEXTURE_BASE_FORMAT_UBC5 | SCE_GXM_TEXTURE_SWIZZLE2_00RG,			///< The decoded UBC5 (GR) value is swizzled to 00RG (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_UBC5_GR				= SCE_GXM_TEXTURE_BASE_FORMAT_UBC5 | SCE_GXM_TEXTURE_SWIZZLE2_GR,			///< The decoded UBC5 (GR) value is returned as a 2-component result.

	SCE_GXM_TEXTURE_FORMAT_SBC5_00GR			= SCE_GXM_TEXTURE_BASE_FORMAT_SBC5 | SCE_GXM_TEXTURE_SWIZZLE2_00GR,			///< The decoded SBC5 (GR) value is swizzled to 00GR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_SBC5_GRRR			= SCE_GXM_TEXTURE_BASE_FORMAT_SBC5 | SCE_GXM_TEXTURE_SWIZZLE2_GRRR,			///< The decoded SBC5 (GR) value is swizzled to GRRR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_SBC5_RGGG			= SCE_GXM_TEXTURE_BASE_FORMAT_SBC5 | SCE_GXM_TEXTURE_SWIZZLE2_RGGG,			///< The decoded SBC5 (GR) value is swizzled to RGGG (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_SBC5_GRGR			= SCE_GXM_TEXTURE_BASE_FORMAT_SBC5 | SCE_GXM_TEXTURE_SWIZZLE2_GRGR,			///< The decoded SBC5 (GR) value is swizzled to GRGR (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_SBC5_00RG			= SCE_GXM_TEXTURE_BASE_FORMAT_SBC5 | SCE_GXM_TEXTURE_SWIZZLE2_00RG,			///< The decoded SBC5 (GR) value is swizzled to 00RG (in ABGR notation).
	SCE_GXM_TEXTURE_FORMAT_SBC5_GR				= SCE_GXM_TEXTURE_BASE_FORMAT_SBC5 | SCE_GXM_TEXTURE_SWIZZLE2_GR,			///< The decoded SBC5 (GR) value is returned as a 2-component result.

	SCE_GXM_TEXTURE_FORMAT_YUV420P2_CSC0		= SCE_GXM_TEXTURE_BASE_FORMAT_YUV420P2 | SCE_GXM_TEXTURE_SWIZZLE_YUV_CSC0,	///< The Y plane and UV plane is converted to ABGR using CSC matrix 0, A is implicit and assigned 1.
	SCE_GXM_TEXTURE_FORMAT_YVU420P2_CSC0		= SCE_GXM_TEXTURE_BASE_FORMAT_YUV420P2 | SCE_GXM_TEXTURE_SWIZZLE_YVU_CSC0,	///< The Y plane and VU plane is converted to ABGR using CSC matrix 0, A is implicit and assigned 1.		
	SCE_GXM_TEXTURE_FORMAT_YUV420P2_CSC1		= SCE_GXM_TEXTURE_BASE_FORMAT_YUV420P2 | SCE_GXM_TEXTURE_SWIZZLE_YUV_CSC1,	///< The Y plane and UV plane is converted to ABGR using CSC matrix 1, A is implicit and assigned 1.		
	SCE_GXM_TEXTURE_FORMAT_YVU420P2_CSC1		= SCE_GXM_TEXTURE_BASE_FORMAT_YUV420P2 | SCE_GXM_TEXTURE_SWIZZLE_YVU_CSC1,	///< The Y plane and VU plane is converted to ABGR using CSC matrix 1, A is implicit and assigned 1.		

	SCE_GXM_TEXTURE_FORMAT_YUV420P3_CSC0		= SCE_GXM_TEXTURE_BASE_FORMAT_YUV420P3 | SCE_GXM_TEXTURE_SWIZZLE_YUV_CSC0,	///< The Y plane, U plane and V plane is converted to ABGR using CSC matrix 0, A is implicit and assigned 1.
	SCE_GXM_TEXTURE_FORMAT_YVU420P3_CSC0		= SCE_GXM_TEXTURE_BASE_FORMAT_YUV420P3 | SCE_GXM_TEXTURE_SWIZZLE_YVU_CSC0,	///< The Y plane, V plane and U plane is converted to ABGR using CSC matrix 0, A is implicit and assigned 1.		
	SCE_GXM_TEXTURE_FORMAT_YUV420P3_CSC1		= SCE_GXM_TEXTURE_BASE_FORMAT_YUV420P3 | SCE_GXM_TEXTURE_SWIZZLE_YUV_CSC1,	///< The Y plane, U plane and V plane is converted to ABGR using CSC matrix 1, A is implicit and assigned 1.		
	SCE_GXM_TEXTURE_FORMAT_YVU420P3_CSC1		= SCE_GXM_TEXTURE_BASE_FORMAT_YUV420P3 | SCE_GXM_TEXTURE_SWIZZLE_YVU_CSC1,	///< The Y plane, V plane and U plane is converted to ABGR using CSC matrix 1, A is implicit and assigned 1.		

	SCE_GXM_TEXTURE_FORMAT_YUYV422_CSC0			= SCE_GXM_TEXTURE_BASE_FORMAT_YUV422 | SCE_GXM_TEXTURE_SWIZZLE_YUYV_CSC0,	///< Packed YUYV 2-pixel blocks are converted to ABGR using CSC matrix 0, A is implicit and assigned 1.
	SCE_GXM_TEXTURE_FORMAT_YVYU422_CSC0			= SCE_GXM_TEXTURE_BASE_FORMAT_YUV422 | SCE_GXM_TEXTURE_SWIZZLE_YVYU_CSC0,	///< Packed YVYU 2-pixel blocks are converted to ABGR using CSC matrix 0, A is implicit and assigned 1.
	SCE_GXM_TEXTURE_FORMAT_UYVY422_CSC0			= SCE_GXM_TEXTURE_BASE_FORMAT_YUV422 | SCE_GXM_TEXTURE_SWIZZLE_UYVY_CSC0,	///< Packed UYVY 2-pixel blocks are converted to ABGR using CSC matrix 0, A is implicit and assigned 1.
	SCE_GXM_TEXTURE_FORMAT_VYUY422_CSC0			= SCE_GXM_TEXTURE_BASE_FORMAT_YUV422 | SCE_GXM_TEXTURE_SWIZZLE_VYUY_CSC0,	///< Packed VYUY 2-pixel blocks are converted to ABGR using CSC matrix 0, A is implicit and assigned 1.
	SCE_GXM_TEXTURE_FORMAT_YUYV422_CSC1			= SCE_GXM_TEXTURE_BASE_FORMAT_YUV422 | SCE_GXM_TEXTURE_SWIZZLE_YUYV_CSC1,	///< Packed YUYV 2-pixel blocks are converted to ABGR using CSC matrix 1, A is implicit and assigned 1.
	SCE_GXM_TEXTURE_FORMAT_YVYU422_CSC1			= SCE_GXM_TEXTURE_BASE_FORMAT_YUV422 | SCE_GXM_TEXTURE_SWIZZLE_YVYU_CSC1,	///< Packed YVYU 2-pixel blocks are converted to ABGR using CSC matrix 1, A is implicit and assigned 1.
	SCE_GXM_TEXTURE_FORMAT_UYVY422_CSC1			= SCE_GXM_TEXTURE_BASE_FORMAT_YUV422 | SCE_GXM_TEXTURE_SWIZZLE_UYVY_CSC1,	///< Packed UYVY 2-pixel blocks are converted to ABGR using CSC matrix 1, A is implicit and assigned 1.
	SCE_GXM_TEXTURE_FORMAT_VYUY422_CSC1			= SCE_GXM_TEXTURE_BASE_FORMAT_YUV422 | SCE_GXM_TEXTURE_SWIZZLE_VYUY_CSC1,	///< Packed VYUY 2-pixel blocks are converted to ABGR using CSC matrix 1, A is implicit and assigned 1.

	SCE_GXM_TEXTURE_FORMAT_P4_ABGR				= SCE_GXM_TEXTURE_BASE_FORMAT_P4 | SCE_GXM_TEXTURE_SWIZZLE4_ABGR,			///< The U4 index looks up into a palette of U8U8U8U8 data read in ABGR order from memory.
	SCE_GXM_TEXTURE_FORMAT_P4_ARGB				= SCE_GXM_TEXTURE_BASE_FORMAT_P4 | SCE_GXM_TEXTURE_SWIZZLE4_ARGB,			///< The U4 index looks up into a palette of U8U8U8U8 data read in ARGB order from memory.
	SCE_GXM_TEXTURE_FORMAT_P4_RGBA				= SCE_GXM_TEXTURE_BASE_FORMAT_P4 | SCE_GXM_TEXTURE_SWIZZLE4_RGBA,			///< The U4 index looks up into a palette of U8U8U8U8 data read in RGBA order from memory.
	SCE_GXM_TEXTURE_FORMAT_P4_BGRA				= SCE_GXM_TEXTURE_BASE_FORMAT_P4 | SCE_GXM_TEXTURE_SWIZZLE4_BGRA,			///< The U4 index looks up into a palette of U8U8U8U8 data read in BGRA order from memory.
	SCE_GXM_TEXTURE_FORMAT_P4_1BGR				= SCE_GXM_TEXTURE_BASE_FORMAT_P4 | SCE_GXM_TEXTURE_SWIZZLE4_1BGR,			///< The U4 index looks up into a palette of U8U8U8U8 data read in ABGR order from memory, A is replaced with 0xff.
	SCE_GXM_TEXTURE_FORMAT_P4_1RGB				= SCE_GXM_TEXTURE_BASE_FORMAT_P4 | SCE_GXM_TEXTURE_SWIZZLE4_1RGB,			///< The U4 index looks up into a palette of U8U8U8U8 data read in ARGB order from memory, A is replaced with 0xff.
	SCE_GXM_TEXTURE_FORMAT_P4_RGB1				= SCE_GXM_TEXTURE_BASE_FORMAT_P4 | SCE_GXM_TEXTURE_SWIZZLE4_RGB1,			///< The U4 index looks up into a palette of U8U8U8U8 data read in RGBA order from memory, A is replaced with 0xff.
	SCE_GXM_TEXTURE_FORMAT_P4_BGR1				= SCE_GXM_TEXTURE_BASE_FORMAT_P4 | SCE_GXM_TEXTURE_SWIZZLE4_BGR1,			///< The U4 index looks up into a palette of U8U8U8U8 data read in BGRA order from memory, A is replaced with 0xff.

	SCE_GXM_TEXTURE_FORMAT_P8_ABGR				= SCE_GXM_TEXTURE_BASE_FORMAT_P8 | SCE_GXM_TEXTURE_SWIZZLE4_ABGR,			///< The U8 index looks up into a palette of U8U8U8U8 data read in ABGR order from memory.
	SCE_GXM_TEXTURE_FORMAT_P8_ARGB				= SCE_GXM_TEXTURE_BASE_FORMAT_P8 | SCE_GXM_TEXTURE_SWIZZLE4_ARGB,			///< The U8 index looks up into a palette of U8U8U8U8 data read in ARGB order from memory.
	SCE_GXM_TEXTURE_FORMAT_P8_RGBA				= SCE_GXM_TEXTURE_BASE_FORMAT_P8 | SCE_GXM_TEXTURE_SWIZZLE4_RGBA,			///< The U8 index looks up into a palette of U8U8U8U8 data read in RGBA order from memory.
	SCE_GXM_TEXTURE_FORMAT_P8_BGRA				= SCE_GXM_TEXTURE_BASE_FORMAT_P8 | SCE_GXM_TEXTURE_SWIZZLE4_BGRA,			///< The U8 index looks up into a palette of U8U8U8U8 data read in BGRA order from memory.
	SCE_GXM_TEXTURE_FORMAT_P8_1BGR				= SCE_GXM_TEXTURE_BASE_FORMAT_P8 | SCE_GXM_TEXTURE_SWIZZLE4_1BGR,			///< The U8 index looks up into a palette of U8U8U8U8 data read in ABGR order from memory, A is replaced with 0xff.
	SCE_GXM_TEXTURE_FORMAT_P8_1RGB				= SCE_GXM_TEXTURE_BASE_FORMAT_P8 | SCE_GXM_TEXTURE_SWIZZLE4_1RGB,			///< The U8 index looks up into a palette of U8U8U8U8 data read in ARGB order from memory, A is replaced with 0xff.
	SCE_GXM_TEXTURE_FORMAT_P8_RGB1				= SCE_GXM_TEXTURE_BASE_FORMAT_P8 | SCE_GXM_TEXTURE_SWIZZLE4_RGB1,			///< The U8 index looks up into a palette of U8U8U8U8 data read in RGBA order from memory, A is replaced with 0xff.
	SCE_GXM_TEXTURE_FORMAT_P8_BGR1				= SCE_GXM_TEXTURE_BASE_FORMAT_P8 | SCE_GXM_TEXTURE_SWIZZLE4_BGR1,			///< The U8 index looks up into a palette of U8U8U8U8 data read in BGRA order from memory, A is replaced with 0xff.

	SCE_GXM_TEXTURE_FORMAT_U8U8U8_BGR			= SCE_GXM_TEXTURE_BASE_FORMAT_U8U8U8 | SCE_GXM_TEXTURE_SWIZZLE3_BGR,		///< The packed 24-bit U8U8U8 data is read in BGR order from memory, A is implicit and assigned 1.
	SCE_GXM_TEXTURE_FORMAT_U8U8U8_RGB			= SCE_GXM_TEXTURE_BASE_FORMAT_U8U8U8 | SCE_GXM_TEXTURE_SWIZZLE3_RGB,		///< The packed 24-bit U8U8U8 data is read in RGB order from memory, A is implicit and assigned 1.

	SCE_GXM_TEXTURE_FORMAT_S8S8S8_BGR			= SCE_GXM_TEXTURE_BASE_FORMAT_S8S8S8 | SCE_GXM_TEXTURE_SWIZZLE3_BGR,		///< The packed 24-bit S8S8S8 data is read in BGR order from memory, A is implicit and assigned 1.
	SCE_GXM_TEXTURE_FORMAT_S8S8S8_RGB			= SCE_GXM_TEXTURE_BASE_FORMAT_S8S8S8 | SCE_GXM_TEXTURE_SWIZZLE3_RGB,		///< The packed 24-bit S8S8S8 data is read in RGB order from memory, A is implicit and assigned 1.

	SCE_GXM_TEXTURE_FORMAT_U2F10F10F10_ABGR		= SCE_GXM_TEXTURE_BASE_FORMAT_U2F10F10F10 | SCE_GXM_TEXTURE_SWIZZLE4_ABGR,	///< The U2F10F10F10 data is read in ABGR order from memory.
	SCE_GXM_TEXTURE_FORMAT_U2F10F10F10_ARGB		= SCE_GXM_TEXTURE_BASE_FORMAT_U2F10F10F10 | SCE_GXM_TEXTURE_SWIZZLE4_ARGB,	///< The U2F10F10F10 data is read in ARGB order from memory.
	SCE_GXM_TEXTURE_FORMAT_F10F10F10U2_RGBA		= SCE_GXM_TEXTURE_BASE_FORMAT_U2F10F10F10 | SCE_GXM_TEXTURE_SWIZZLE4_RGBA,	///< The F10F10F10U2 data is read in RGBA order from memory.
	SCE_GXM_TEXTURE_FORMAT_F10F10F10U2_BGRA		= SCE_GXM_TEXTURE_BASE_FORMAT_U2F10F10F10 | SCE_GXM_TEXTURE_SWIZZLE4_BGRA,	///< The F10F10F10U2 data is read in BGRA order from memory.
	SCE_GXM_TEXTURE_FORMAT_X2F10F10F10_1BGR		= SCE_GXM_TEXTURE_BASE_FORMAT_U2F10F10F10 | SCE_GXM_TEXTURE_SWIZZLE4_1BGR,	///< The U2F10F10F10 data is read in ABGR order from memory, A is replaced with 0x3.
	SCE_GXM_TEXTURE_FORMAT_X2F10F10F10_1RGB		= SCE_GXM_TEXTURE_BASE_FORMAT_U2F10F10F10 | SCE_GXM_TEXTURE_SWIZZLE4_1RGB,	///< The U2F10F10F10 data is read in ARGB order from memory, A is replaced with 0x3.
	SCE_GXM_TEXTURE_FORMAT_F10F10F10X2_RGB1		= SCE_GXM_TEXTURE_BASE_FORMAT_U2F10F10F10 | SCE_GXM_TEXTURE_SWIZZLE4_RGB1,	///< The F10F10F10U2 data is read in RGBA order from memory, A is replaced with 0x3.
	SCE_GXM_TEXTURE_FORMAT_F10F10F10X2_BGR1		= SCE_GXM_TEXTURE_BASE_FORMAT_U2F10F10F10 | SCE_GXM_TEXTURE_SWIZZLE4_BGR1,	///< The F10F10F10U2 data is read in BGRA order from memory, A is replaced with 0x3.

	// Legacy formats (defined in terms of full list above)

	SCE_GXM_TEXTURE_FORMAT_L8					= SCE_GXM_TEXTURE_FORMAT_U8_1RRR,			///< Legacy name for #SCE_GXM_TEXTURE_FORMAT_U8_1RRR.
	SCE_GXM_TEXTURE_FORMAT_A8					= SCE_GXM_TEXTURE_FORMAT_U8_R000,			///< Legacy name for #SCE_GXM_TEXTURE_FORMAT_U8_R000.
	SCE_GXM_TEXTURE_FORMAT_R8					= SCE_GXM_TEXTURE_FORMAT_U8_000R,			///< Legacy name for #SCE_GXM_TEXTURE_FORMAT_U8_000R.
	SCE_GXM_TEXTURE_FORMAT_A4R4G4B4				= SCE_GXM_TEXTURE_FORMAT_U4U4U4U4_ARGB,		///< Legacy name for #SCE_GXM_TEXTURE_FORMAT_U4U4U4U4_ARGB.
	SCE_GXM_TEXTURE_FORMAT_A1R5G5B5				= SCE_GXM_TEXTURE_FORMAT_U1U5U5U5_ARGB,		///< Legacy name for #SCE_GXM_TEXTURE_FORMAT_U1U5U5U5_ARGB.
	SCE_GXM_TEXTURE_FORMAT_R5G6B5				= SCE_GXM_TEXTURE_FORMAT_U5U6U5_RGB,		///< Legacy name for #SCE_GXM_TEXTURE_FORMAT_U5U6U5_RGB.
	SCE_GXM_TEXTURE_FORMAT_A8L8					= SCE_GXM_TEXTURE_FORMAT_U8U8_GRRR,			///< Legacy name for #SCE_GXM_TEXTURE_FORMAT_U8U8_GRRR.
	SCE_GXM_TEXTURE_FORMAT_L8A8					= SCE_GXM_TEXTURE_FORMAT_U8U8_RGGG,			///< Legacy name for #SCE_GXM_TEXTURE_FORMAT_U8U8_RGGG.
	SCE_GXM_TEXTURE_FORMAT_G8R8					= SCE_GXM_TEXTURE_FORMAT_U8U8_00GR,			///< Legacy name for #SCE_GXM_TEXTURE_FORMAT_U8U8_00GR.
	SCE_GXM_TEXTURE_FORMAT_L16					= SCE_GXM_TEXTURE_FORMAT_U16_1RRR,			///< Legacy name for #SCE_GXM_TEXTURE_FORMAT_U16_1RRR.
	SCE_GXM_TEXTURE_FORMAT_A16					= SCE_GXM_TEXTURE_FORMAT_U16_R000,			///< Legacy name for #SCE_GXM_TEXTURE_FORMAT_U16_R000.
	SCE_GXM_TEXTURE_FORMAT_R16					= SCE_GXM_TEXTURE_FORMAT_U16_000R,			///< Legacy name for #SCE_GXM_TEXTURE_FORMAT_U16_000R.
	SCE_GXM_TEXTURE_FORMAT_D16					= SCE_GXM_TEXTURE_FORMAT_U16_R,				///< Legacy name for #SCE_GXM_TEXTURE_FORMAT_U16_R.
	SCE_GXM_TEXTURE_FORMAT_LF16					= SCE_GXM_TEXTURE_FORMAT_F16_1RRR,			///< Legacy name for #SCE_GXM_TEXTURE_FORMAT_F16_1RRR.
	SCE_GXM_TEXTURE_FORMAT_AF16					= SCE_GXM_TEXTURE_FORMAT_F16_R000,			///< Legacy name for #SCE_GXM_TEXTURE_FORMAT_F16_R000.
	SCE_GXM_TEXTURE_FORMAT_RF16					= SCE_GXM_TEXTURE_FORMAT_F16_000R,			///< Legacy name for #SCE_GXM_TEXTURE_FORMAT_F16_000R.
	SCE_GXM_TEXTURE_FORMAT_A8R8G8B8				= SCE_GXM_TEXTURE_FORMAT_U8U8U8U8_ARGB,		///< Legacy name for #SCE_GXM_TEXTURE_FORMAT_U8U8U8U8_ARGB.
	SCE_GXM_TEXTURE_FORMAT_A8B8G8R8				= SCE_GXM_TEXTURE_FORMAT_U8U8U8U8_ABGR,		///< Legacy name for #SCE_GXM_TEXTURE_FORMAT_U8U8U8U8_ABGR.
	SCE_GXM_TEXTURE_FORMAT_AF16LF16				= SCE_GXM_TEXTURE_FORMAT_F16F16_GRRR,		///< Legacy name for #SCE_GXM_TEXTURE_FORMAT_F16F16_GRRR.
	SCE_GXM_TEXTURE_FORMAT_LF16AF16				= SCE_GXM_TEXTURE_FORMAT_F16F16_RGGG,		///< Legacy name for #SCE_GXM_TEXTURE_FORMAT_F16F16_RGGG.
	SCE_GXM_TEXTURE_FORMAT_GF16RF16				= SCE_GXM_TEXTURE_FORMAT_F16F16_00GR,		///< Legacy name for #SCE_GXM_TEXTURE_FORMAT_F16F16_00GR.
	SCE_GXM_TEXTURE_FORMAT_LF32M				= SCE_GXM_TEXTURE_FORMAT_F32M_1RRR,			///< Legacy name for #SCE_GXM_TEXTURE_FORMAT_F32M_1RRR.
	SCE_GXM_TEXTURE_FORMAT_AF32M				= SCE_GXM_TEXTURE_FORMAT_F32M_R000,			///< Legacy name for #SCE_GXM_TEXTURE_FORMAT_F32M_R000.
	SCE_GXM_TEXTURE_FORMAT_RF32M				= SCE_GXM_TEXTURE_FORMAT_F32M_000R,			///< Legacy name for #SCE_GXM_TEXTURE_FORMAT_F32M_000R.
	SCE_GXM_TEXTURE_FORMAT_DF32M				= SCE_GXM_TEXTURE_FORMAT_F32M_R,			///< Legacy name for #SCE_GXM_TEXTURE_FORMAT_F32M_R.
	SCE_GXM_TEXTURE_FORMAT_VYUY					= SCE_GXM_TEXTURE_FORMAT_VYUY422_CSC0,		///< Legacy name for #SCE_GXM_TEXTURE_FORMAT_VYUY422_CSC0.
	SCE_GXM_TEXTURE_FORMAT_YVYU					= SCE_GXM_TEXTURE_FORMAT_YVYU422_CSC0,		///< Legacy name for #SCE_GXM_TEXTURE_FORMAT_YVYU422_CSC0.
	SCE_GXM_TEXTURE_FORMAT_UBC1					= SCE_GXM_TEXTURE_FORMAT_UBC1_ABGR,			///< Legacy name for #SCE_GXM_TEXTURE_FORMAT_UBC1_ABGR.
	SCE_GXM_TEXTURE_FORMAT_UBC2					= SCE_GXM_TEXTURE_FORMAT_UBC2_ABGR,			///< Legacy name for #SCE_GXM_TEXTURE_FORMAT_UBC2_ABGR.
	SCE_GXM_TEXTURE_FORMAT_UBC3					= SCE_GXM_TEXTURE_FORMAT_UBC3_ABGR,			///< Legacy name for #SCE_GXM_TEXTURE_FORMAT_UBC3_ABGR.
	SCE_GXM_TEXTURE_FORMAT_PVRT2BPP				= SCE_GXM_TEXTURE_FORMAT_PVRT2BPP_ABGR,		///< Legacy name for #SCE_GXM_TEXTURE_FORMAT_PVRT2BPP_ABGR.
	SCE_GXM_TEXTURE_FORMAT_PVRT4BPP				= SCE_GXM_TEXTURE_FORMAT_PVRT4BPP_ABGR,		///< Legacy name for #SCE_GXM_TEXTURE_FORMAT_PVRT4BPP_ABGR.
	SCE_GXM_TEXTURE_FORMAT_PVRTII2BPP			= SCE_GXM_TEXTURE_FORMAT_PVRTII2BPP_ABGR,	///< Legacy name for #SCE_GXM_TEXTURE_FORMAT_PVRTII2BPP_ABGR.
	SCE_GXM_TEXTURE_FORMAT_PVRTII4BPP			= SCE_GXM_TEXTURE_FORMAT_PVRTII4BPP_ABGR	///< Legacy name for #SCE_GXM_TEXTURE_FORMAT_PVRTII4BPP_ABGR.
} SceGxmTextureFormat;

/** The texture type.

	@ingroup render
*/
typedef enum SceGxmTextureType {
	SCE_GXM_TEXTURE_SWIZZLED				= 0x00000000U,	///< The texture uses a swizzled memory layout.
	SCE_GXM_TEXTURE_CUBE					= 0x40000000U,	///< The texture uses a (implicitly swizzled) cube memory layout.
	SCE_GXM_TEXTURE_LINEAR					= 0x60000000U,	///< The texture uses a linear memory layout with implicit stride.
	SCE_GXM_TEXTURE_TILED					= 0x80000000U,	///< The texture uses a tiled memory layout.
	SCE_GXM_TEXTURE_SWIZZLED_ARBITRARY		= 0xa0000000U,	///< The texture uses a swizzled memory layout with arbitrary width and height.
	SCE_GXM_TEXTURE_LINEAR_STRIDED			= 0xc0000000U,	///< The texture uses a linear memory layout with an explicit stride value.
	SCE_GXM_TEXTURE_CUBE_ARBITRARY			= 0xe0000000U	///< The texture uses a cube memory layout (implicitly swizzled) with arbitrary width and height.
} SceGxmTextureType;

namespace sce
{
	namespace Texture
	{

		// /host_tools/graphics/src/sce_texture/texture_libraries/sce_texture_core/common/common.h

		u32 bsr(u32 a_uX);

		u32 enclosingPowerOf2(u32 a_uX);

		u32 floorLog2(u32 a_uX);

		// /host_tools/graphics/src/sce_texture/texture_libraries/sce_texture_core/common/swizzle.h

		void deSwizzleLevel(u8* a_pTgt, const u8* a_pSrc, u32 a_uWidth, u32 a_uHeight, u32 a_uBpp);

		namespace Gxt
		{

			// /host_tools/graphics/src/sce_texture/texture_libraries/sce_texture_core/common/gxt_util.h

			bool getTextureDataSize(u32& a_uSize, u32 a_uWidth, u32 a_uHeight, u32 a_uNumLevels, u32 a_uNumFaces, SceGxmTextureFormat a_eFormat, SceGxmTextureType a_eType);

			bool getBorderDataSize(u32& a_uSize, u32 a_uWidth, u32 a_uHeight, SceGxmTextureFormat a_eFormat);

			bool getBpp(u32& a_uBpp, SceGxmTextureFormat a_eFormat);

			bool isPvr(SceGxmTextureFormat a_eFormat);

			bool isUbc(SceGxmTextureFormat a_eFormat);

			bool isBlockCompressed(SceGxmTextureFormat a_eFormat);

			bool isIndexed(SceGxmTextureFormat a_eFormat);

			bool supportsBorderData(SceGxmTextureFormat a_eFormat);

			SceGxmTextureBaseFormat getBaseFormat(SceGxmTextureFormat a_eFormat);

			// /host_tools/graphics/src/sce_texture/texture_libraries/sce_texture_core/common/gxt_io.h

			struct Palette16
			{
				u8 m_data[4 * 16];
			};

			struct Palette256
			{
				uint8_t m_data[4 * 256];
			};

			struct Data
			{
				SceGxmTextureFormat m_format;
				SceGxmTextureType m_type;
				uint32_t m_width;
				uint32_t m_height;
				uint32_t m_numLevels;
				size_t m_borderSize;
				size_t m_totalSize;
				vector<uint8_t> m_data;
				Palette16* m_palette16;
				Palette256* m_palette256;
			};

			bool isValidInput(const u8* a_pSrcBuffer, size_t a_uSrcSize);

		} // namespace Gxt
	} // namespace Texture
} // namespace sce

class CGxt
{
public:
	CGxt();
	~CGxt();
	void SetFileName(const UString& a_sFileName);
	void SetDirName(const UString& a_sDirName);
	void SetVerbose(bool a_bVerbose);
	bool ExportFile();
	bool ImportFile();
	bool TestPalette();
	static bool IsGxtFile(const UString& a_sFileName);
private:
	static int decode(sce::Texture::Gxt::Data* a_pData, u8* a_pLinear, n32 a_nWidth, n32 a_nHeight, u32 a_uBpp, pvrtexture::CPVRTexture** a_pPVRTexture);
	UString m_sFileName;
	UString m_sDirName;
	bool m_bVerbose;
	vector<sce::Texture::Gxt::Data> m_vData;
	vector<sce::Texture::Gxt::Palette16> m_vPalette16;
	vector<sce::Texture::Gxt::Palette256> m_vPalette256;
};

#endif	// GXT_H_
