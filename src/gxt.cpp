#include "gxt.h"
#include <png.h>
#include <PVRTextureUtilities.h>

#define SCE_GXM_TEXTURE_MAX_SIZE			4096U

namespace sce
{
	namespace Texture
	{

		u32 bsr(u32 a_uX)
		{
			u32 uResult = 0;
			for (n32 i = 0; i < 32; i++)
			{
				if ((a_uX & SDW_BIT32(i)) != 0)
				{
					uResult = i;
				}
			}
			return uResult;
		}

		u32 enclosingPowerOf2(u32 a_uX)
		{
			return 1 << bsr(a_uX + (a_uX - 1));
		}

		u32 floorLog2(u32 a_uX)
		{
			return bsr(a_uX);
		}

		namespace Gxt
		{

			u32 getBlockWidth(SceGxmTextureFormat a_eFormat)
			{
				SceGxmTextureBaseFormat eBaseFormat = getBaseFormat(a_eFormat);
				switch (eBaseFormat)
				{
				case SCE_GXM_TEXTURE_BASE_FORMAT_PVRT2BPP:
				case SCE_GXM_TEXTURE_BASE_FORMAT_PVRTII2BPP:
					return 8;
				case SCE_GXM_TEXTURE_BASE_FORMAT_PVRT4BPP:
				case SCE_GXM_TEXTURE_BASE_FORMAT_PVRTII4BPP:
				case SCE_GXM_TEXTURE_BASE_FORMAT_UBC1:
				case SCE_GXM_TEXTURE_BASE_FORMAT_UBC2:
				case SCE_GXM_TEXTURE_BASE_FORMAT_UBC3:
				case SCE_GXM_TEXTURE_BASE_FORMAT_UBC4:
				case SCE_GXM_TEXTURE_BASE_FORMAT_UBC5:
				case SCE_GXM_TEXTURE_BASE_FORMAT_SBC4:
				case SCE_GXM_TEXTURE_BASE_FORMAT_SBC5:
					return 4;
				default:
					return 0;
				}
			}

			u32 getBlockHeight(SceGxmTextureFormat a_eFormat)
			{
				SceGxmTextureBaseFormat eBaseFormat = getBaseFormat(a_eFormat);
				switch (eBaseFormat)
				{
				case SCE_GXM_TEXTURE_BASE_FORMAT_PVRT2BPP:
				case SCE_GXM_TEXTURE_BASE_FORMAT_PVRTII2BPP:
				case SCE_GXM_TEXTURE_BASE_FORMAT_PVRT4BPP:
				case SCE_GXM_TEXTURE_BASE_FORMAT_PVRTII4BPP:
				case SCE_GXM_TEXTURE_BASE_FORMAT_UBC1:
				case SCE_GXM_TEXTURE_BASE_FORMAT_UBC2:
				case SCE_GXM_TEXTURE_BASE_FORMAT_UBC3:
				case SCE_GXM_TEXTURE_BASE_FORMAT_UBC4:
				case SCE_GXM_TEXTURE_BASE_FORMAT_UBC5:
				case SCE_GXM_TEXTURE_BASE_FORMAT_SBC4:
				case SCE_GXM_TEXTURE_BASE_FORMAT_SBC5:
					return 4;
				default:
					return 0;
				}
			}

			u32 getFaceAlignment(u32 a_uBpp, u32 a_uSize)
			{
				if (a_uBpp <= 8 && a_uSize > SCE_GXM_TEXTURE_CUBE_NO_ALIGN_SIZE_8BPP)
				{
					return SCE_GXM_TEXTURE_FACE_ALIGNMENT;
				}
				else if (a_uBpp >= 16 && a_uBpp <= 32 && a_uSize > SCE_GXM_TEXTURE_CUBE_NO_ALIGN_SIZE_16_32BPP)
				{
					return SCE_GXM_TEXTURE_FACE_ALIGNMENT;
				}
				else if (a_uBpp >= 64 && a_uSize > SCE_GXM_TEXTURE_CUBE_NO_ALIGN_SIZE_64BPP)
				{
					return SCE_GXM_TEXTURE_FACE_ALIGNMENT;
				}
				else
				{
					return 4;
				}
			}

			bool validateDimensions(u32 a_uWidth, u32 a_uHeight, SceGxmTextureType a_eType)
			{
				if (a_uWidth > SCE_GXM_TEXTURE_MAX_SIZE)
				{
					UPrintf(USTR("ERROR: width %u > %u\n\n"), a_uWidth, SCE_GXM_TEXTURE_MAX_SIZE);
					return false;
				}
				if (a_uHeight > SCE_GXM_TEXTURE_MAX_SIZE)
				{
					UPrintf(USTR("ERROR: height %u > %u\n\n"), a_uHeight, SCE_GXM_TEXTURE_MAX_SIZE);
					return false;
				}
				if (a_eType == SCE_GXM_TEXTURE_SWIZZLED || a_eType == SCE_GXM_TEXTURE_CUBE)
				{
					if (!SCE_IS_POW2(a_uWidth))
					{
						UPrintf(USTR("ERROR: width %u is not pow2\n\n"), a_uWidth);
						return false;
					}
					if (!SCE_IS_POW2(a_uHeight))
					{
						UPrintf(USTR("ERROR: height %u is not pow2\n\n"), a_uHeight);
						return false;
					}
				}
				return true;
			}

			bool getTextureDataSize(u32& a_uSize, u32 a_uWidth, u32 a_uHeight, u32 a_uNumLevels, u32 a_uNumFaces, SceGxmTextureFormat a_eFormat, SceGxmTextureType a_eType)
			{
				if (!validateDimensions(a_uWidth, a_uHeight, a_eType))
				{
					return false;
				}
				a_uSize = 0;
				u32 uBpp = 0;
				if (!sce::Texture::Gxt::getBpp(uBpp, a_eFormat))
				{
					return false;
				}
				u32 uFaceAlignment = (a_uNumFaces > 1 && a_uNumLevels > 1) ? getFaceAlignment(uBpp, a_uWidth) : 1;
				if (isBlockCompressed(a_eFormat))
				{
					u32 uBlockWidth = getBlockWidth(a_eFormat);
					u32 uBlockHeight = getBlockHeight(a_eFormat);
					for (u32 i = 0; i < a_uNumFaces; i++)
					{
						u32 uFaceOffset = a_uSize;
						u32 uMipWidth = std::max<u32>(a_uWidth, uBlockWidth);
						u32 uMipHeight = std::max<u32>(a_uHeight, uBlockHeight);
						u32 uMipWidthEx = enclosingPowerOf2(uMipWidth);
						u32 uMipHeightEx = enclosingPowerOf2(uMipHeight);
						for (u32 j = 0; j < a_uNumLevels; j++)
						{
							u32 uMipWidthBlocks = (uMipWidth + uBlockWidth - 1) / uBlockWidth;
							u32 uMipHeightBlocks = (uMipHeight + uBlockHeight - 1) / uBlockHeight;
							u32 uLevelSizeTgt = (uBpp * uMipWidthEx * uMipHeightEx) / 8;
							a_uSize += uLevelSizeTgt;
							uMipWidth = uMipWidth > uBlockWidth ? uMipWidth / 2 : uBlockWidth;
							uMipHeight = uMipHeight > uBlockHeight ? uMipHeight / 2 : uBlockHeight;
							uMipWidthEx = uMipWidthEx > uBlockWidth ? uMipWidthEx / 2 : uBlockWidth;
							uMipHeightEx = uMipHeightEx > uBlockHeight ? uMipHeightEx / 2 : uBlockHeight;
						}
						if (i < a_uNumFaces - 1)
						{
							a_uSize = uFaceOffset + SCE_ALIGN(a_uSize - uFaceOffset, uFaceAlignment);
						}
					}
				}
				else
				{
					u32 uWidthAlignment = a_eType == SCE_GXM_TEXTURE_LINEAR ? SCE_GXM_TEXTURE_IMPLICIT_STRIDE_ALIGNMENT : 1;
					for (u32 i = 0; i < a_uNumFaces; i++)
					{
						u32 uFaceOffset = a_uSize;
						u32 uMipWidth = a_uWidth;
						u32 uMipHeight = a_uHeight;
						u32 uMipWidthEx = a_uNumLevels > 1 ? enclosingPowerOf2(a_uWidth) : a_uWidth;
						u32 uMipHeightEx = a_uNumLevels > 1 ? enclosingPowerOf2(a_uHeight) : a_uHeight;
						uMipWidthEx = SCE_ALIGN(uMipWidthEx, uWidthAlignment);
						for (u32 j = 0; j < a_uNumLevels; j++)
						{
							a_uSize += uMipHeightEx * ((uMipWidthEx * uBpp + 7) / 8);
							uMipWidth = uMipWidth > 1 ? uMipWidth / 2 : 1;
							uMipHeight = uMipHeight > 1 ? uMipHeight / 2 : 1;
							uMipWidthEx = uMipWidthEx > 1 ? uMipWidthEx / 2 : 1;
							uMipHeightEx = uMipHeightEx > 1 ? uMipHeightEx / 2 : 1;
							uMipWidthEx = SCE_ALIGN(uMipWidthEx, uWidthAlignment);
						}
						if (i < a_uNumFaces - 1)
						{
							a_uSize = uFaceOffset + SCE_ALIGN(a_uSize - uFaceOffset, uFaceAlignment);
						}
					}
				}
				a_uSize = SCE_ALIGN(a_uSize, SCE_GXM_TEXTURE_ALIGNMENT);
				return true;
			}

			bool getBorderDataSize(u32& a_uSize, u32 a_uWidth, u32 a_uHeight, SceGxmTextureFormat a_eFormat)
			{
				if (!supportsBorderData(a_eFormat))
				{
					UPrintf(USTR("ERROR: format %08X do not support border\n\n"), a_eFormat);
					return false;
				}
				u32 uBpp = 0;
				if (!getBpp(uBpp, a_eFormat))
				{
					return false;
				}
				// taken from GPU Data Formats Guide Table 59
				static u32 uSizeTable[] =
				{
					56, 64, 80, 112, 176, 304, 560, 1072, 2096, 4144, 8240, 16432, 32816
				};
				u32 uMaxDim = std::max<u32>(a_uWidth, a_uHeight);
				u32 uMaxDimLog2 = floorLog2(uMaxDim);
				if (uMaxDimLog2 >= SDW_ARRAY_COUNT(uSizeTable))
				{
					return false;
				}
				a_uSize = (uSizeTable[uMaxDimLog2] * uBpp) / 8;
				return true;
			}

			bool getBpp(u32& a_uBpp, SceGxmTextureFormat a_eFormat)
			{
				SceGxmTextureBaseFormat eBaseFormat = getBaseFormat(a_eFormat);
				switch (eBaseFormat)
				{
				case SCE_GXM_TEXTURE_BASE_FORMAT_U8:
				case SCE_GXM_TEXTURE_BASE_FORMAT_S8:
					a_uBpp = 8;
					break;
				case SCE_GXM_TEXTURE_BASE_FORMAT_U4U4U4U4:
				case SCE_GXM_TEXTURE_BASE_FORMAT_U8U3U3U2:
				case SCE_GXM_TEXTURE_BASE_FORMAT_U1U5U5U5:
				case SCE_GXM_TEXTURE_BASE_FORMAT_U5U6U5:
				case SCE_GXM_TEXTURE_BASE_FORMAT_S5S5U6:
				case SCE_GXM_TEXTURE_BASE_FORMAT_U8U8:
				case SCE_GXM_TEXTURE_BASE_FORMAT_S8S8:
				case SCE_GXM_TEXTURE_BASE_FORMAT_U16:
				case SCE_GXM_TEXTURE_BASE_FORMAT_S16:
				case SCE_GXM_TEXTURE_BASE_FORMAT_F16:
					a_uBpp = 16;
					break;
				case SCE_GXM_TEXTURE_BASE_FORMAT_U8U8U8U8:
				case SCE_GXM_TEXTURE_BASE_FORMAT_S8S8S8S8:
				case SCE_GXM_TEXTURE_BASE_FORMAT_U2U10U10U10:
				case SCE_GXM_TEXTURE_BASE_FORMAT_U16U16:
				case SCE_GXM_TEXTURE_BASE_FORMAT_S16S16:
				case SCE_GXM_TEXTURE_BASE_FORMAT_F16F16:
				case SCE_GXM_TEXTURE_BASE_FORMAT_F32:
				case SCE_GXM_TEXTURE_BASE_FORMAT_F32M:
				case SCE_GXM_TEXTURE_BASE_FORMAT_X8S8S8U8:
				case SCE_GXM_TEXTURE_BASE_FORMAT_X8U24:
				case SCE_GXM_TEXTURE_BASE_FORMAT_U32:
				case SCE_GXM_TEXTURE_BASE_FORMAT_S32:
				case SCE_GXM_TEXTURE_BASE_FORMAT_SE5M9M9M9:
				case SCE_GXM_TEXTURE_BASE_FORMAT_F11F11F10:
					a_uBpp = 32;
					break;
				case SCE_GXM_TEXTURE_BASE_FORMAT_F16F16F16F16:
				case SCE_GXM_TEXTURE_BASE_FORMAT_U16U16U16U16:
				case SCE_GXM_TEXTURE_BASE_FORMAT_S16S16S16S16:
				case SCE_GXM_TEXTURE_BASE_FORMAT_F32F32:
				case SCE_GXM_TEXTURE_BASE_FORMAT_U32U32:
					a_uBpp = 64;
					break;
				case SCE_GXM_TEXTURE_BASE_FORMAT_PVRT2BPP:
				case SCE_GXM_TEXTURE_BASE_FORMAT_PVRTII2BPP:
					a_uBpp = 2;
					break;
				case SCE_GXM_TEXTURE_BASE_FORMAT_PVRT4BPP:
				case SCE_GXM_TEXTURE_BASE_FORMAT_PVRTII4BPP:
					a_uBpp = 4;
					break;
				case SCE_GXM_TEXTURE_BASE_FORMAT_UBC1:
				case SCE_GXM_TEXTURE_BASE_FORMAT_UBC4:
				case SCE_GXM_TEXTURE_BASE_FORMAT_SBC4:
					a_uBpp = 4;
					break;
				case SCE_GXM_TEXTURE_BASE_FORMAT_UBC2:
				case SCE_GXM_TEXTURE_BASE_FORMAT_UBC3:
				case SCE_GXM_TEXTURE_BASE_FORMAT_UBC5:
				case SCE_GXM_TEXTURE_BASE_FORMAT_SBC5:
					a_uBpp = 8;
					break;
				case SCE_GXM_TEXTURE_BASE_FORMAT_P4:
					a_uBpp = 4;
					break;
				case SCE_GXM_TEXTURE_BASE_FORMAT_P8:
					a_uBpp = 8;
					break;
				case SCE_GXM_TEXTURE_BASE_FORMAT_U8U8U8:
				case SCE_GXM_TEXTURE_BASE_FORMAT_S8S8S8:
					a_uBpp = 24;
					break;
				case SCE_GXM_TEXTURE_BASE_FORMAT_U2F10F10F10:
					a_uBpp = 32;
					break;
				default:
					UPrintf(USTR("ERROR: do not support base format %08X\n\n"), eBaseFormat);
					return false;
				}
				return true;
			}

			bool isPvr(SceGxmTextureFormat a_eFormat)
			{
				SceGxmTextureBaseFormat eBaseFormat = getBaseFormat(a_eFormat);
				switch (eBaseFormat)
				{
				case SCE_GXM_TEXTURE_BASE_FORMAT_PVRT2BPP:
				case SCE_GXM_TEXTURE_BASE_FORMAT_PVRT4BPP:
				case SCE_GXM_TEXTURE_BASE_FORMAT_PVRTII2BPP:
				case SCE_GXM_TEXTURE_BASE_FORMAT_PVRTII4BPP:
					return true;
				default:
					return false;
				}
			}

			bool isUbc(SceGxmTextureFormat a_eFormat)
			{
				SceGxmTextureBaseFormat eBaseFormat = getBaseFormat(a_eFormat);
				switch (eBaseFormat)
				{
				case SCE_GXM_TEXTURE_BASE_FORMAT_UBC1:
				case SCE_GXM_TEXTURE_BASE_FORMAT_UBC2:
				case SCE_GXM_TEXTURE_BASE_FORMAT_UBC3:
				case SCE_GXM_TEXTURE_BASE_FORMAT_UBC4:
				case SCE_GXM_TEXTURE_BASE_FORMAT_UBC5:
				case SCE_GXM_TEXTURE_BASE_FORMAT_SBC4:
				case SCE_GXM_TEXTURE_BASE_FORMAT_SBC5:
					return true;
				default:
					return false;
				}
			}

			bool isBlockCompressed(SceGxmTextureFormat a_eFormat)
			{
				return isUbc(a_eFormat) || isPvr(a_eFormat);
			}

			bool isIndexed(SceGxmTextureFormat a_eFormat)
			{
				SceGxmTextureBaseFormat eBaseFormat = getBaseFormat(a_eFormat);
				switch (eBaseFormat)
				{
				case SCE_GXM_TEXTURE_BASE_FORMAT_P4:
				case SCE_GXM_TEXTURE_BASE_FORMAT_P8:
					return true;
				default:
					return false;
				}
			}

			bool supportsBorderData(SceGxmTextureFormat a_eFormat)
			{
				return !isBlockCompressed(a_eFormat) && !isIndexed(a_eFormat);
			}

			SceGxmTextureBaseFormat getBaseFormat(SceGxmTextureFormat a_eFormat)
			{
				return static_cast<SceGxmTextureBaseFormat>(a_eFormat & SCE_GXM_TEXTURE_BASE_FORMAT_MASK);
			}

			bool isValidInput(const u8* a_pSrcBuffer, size_t a_uSrcSize)
			{
				if (a_uSrcSize >= sizeof(SceGxtHeader))
				{
					const SceGxtHeader* pSceGxtHeader = reinterpret_cast<const SceGxtHeader*>(a_pSrcBuffer);
					return (pSceGxtHeader->tag == SCE_GXT_TAG) && (pSceGxtHeader->version == SCE_GXT_VERSION);
				}
				return false;
			}

		} // namespace Gxt
	} // namespace Texture
} // namespace sce

CGxt::CGxt()
	: m_bVerbose(false)
{
}

CGxt::~CGxt()
{
}

void CGxt::SetFileName(const UString& a_sFileName)
{
	m_sFileName = a_sFileName;
}

void CGxt::SetDirName(const UString& a_sDirName)
{
	m_sDirName = a_sDirName;
}

void CGxt::SetVerbose(bool a_bVerbose)
{
	m_bVerbose = a_bVerbose;
}

bool CGxt::ExportFile()
{
	bool bResult = true;
	FILE* fp = UFopen(m_sFileName.c_str(), USTR("rb"));
	if (fp == nullptr)
	{
		return false;
	}
	fseek(fp, 0, SEEK_END);
	u32 uGxtSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	u8* pGxt = new u8[uGxtSize];
	fread(pGxt, 1, uGxtSize, fp);
	fclose(fp);
	SceGxtHeader* pSceGxtHeader = reinterpret_cast<SceGxtHeader*>(pGxt);
	do
	{
		if (!sce::Texture::Gxt::isValidInput(pGxt, uGxtSize))
		{
			bResult = false;
			UPrintf(USTR("ERROR: unknown version %08X\n\n"), pSceGxtHeader->version);
			break;
		}
		if (pSceGxtHeader->numP4Palettes * SCE_GXT_PALETTE_SIZE_P4 + pSceGxtHeader->numP8Palettes * SCE_GXT_PALETTE_SIZE_P8 > pSceGxtHeader->dataSize)
		{
			bResult = false;
			UPrintf(USTR("ERROR: data size is too small\n\n"));
			break;
		}
		u32 uDataEnd = pSceGxtHeader->dataOffset + pSceGxtHeader->dataSize;
		u32 uPal256Offset = uDataEnd - pSceGxtHeader->numP8Palettes * SCE_GXT_PALETTE_SIZE_P8;
		u32 uPal16Offset = uPal256Offset - pSceGxtHeader->numP4Palettes * SCE_GXT_PALETTE_SIZE_P4;
		for (u32 i = 0; i < pSceGxtHeader->numP4Palettes; i++)
		{
			sce::Texture::Gxt::Palette16 palette16;
			memcpy(palette16.m_data, pGxt + uPal16Offset + i * SCE_GXT_PALETTE_SIZE_P4, SCE_GXT_PALETTE_SIZE_P4);
			m_vPalette16.push_back(palette16);
		}
		for (u32 i = 0; i < pSceGxtHeader->numP8Palettes; i++)
		{
			sce::Texture::Gxt::Palette256 palette256;
			memcpy(palette256.m_data, pGxt + uPal256Offset + i * SCE_GXT_PALETTE_SIZE_P8, SCE_GXT_PALETTE_SIZE_P8);
			m_vPalette256.push_back(palette256);
		}
		uDataEnd = uPal16Offset;
		u32 uDataBegin = pSceGxtHeader->dataOffset;
		if (uDataBegin + pSceGxtHeader->numTextures * sizeof(SceGxtTextureInfo) > uDataEnd)
		{
			bResult = false;
			UPrintf(USTR("ERROR: data size is too small\n\n"));
			break;
		}
		uDataBegin += pSceGxtHeader->numTextures * sizeof(SceGxtTextureInfo);
		SceGxtTextureInfo* pSceGxtTextureInfo = reinterpret_cast<SceGxtTextureInfo*>(pSceGxtHeader + 1);
		for (u32 i = 0; i < pSceGxtHeader->numTextures; i++)
		{
			SceGxtTextureInfo& sceGxtTextureInfo = pSceGxtTextureInfo[i];
			sce::Texture::Gxt::Data data;
			data.m_format = static_cast<SceGxmTextureFormat>(sceGxtTextureInfo.format);
			data.m_type = static_cast<SceGxmTextureType>(sceGxtTextureInfo.type);
			data.m_width = sceGxtTextureInfo.width;
			data.m_height = sceGxtTextureInfo.height;
			data.m_numLevels = sceGxtTextureInfo.mipCount;
			data.m_borderSize = 0;
			u32 uNumFaces = sceGxtTextureInfo.type == SCE_GXM_TEXTURE_CUBE ? 6 : 1;
			u32 uTextureDataSize = 0;
			if (!sce::Texture::Gxt::getTextureDataSize(uTextureDataSize, data.m_width, data.m_height, sceGxtTextureInfo.mipCount, uNumFaces, static_cast<SceGxmTextureFormat>(sceGxtTextureInfo.format), static_cast<SceGxmTextureType>(sceGxtTextureInfo.type)))
			{
				bResult = false;
				break;
			}
			data.m_totalSize = data.m_borderSize + uTextureDataSize;
			u32 uTexDataOffset = sceGxtTextureInfo.dataOffset;
			if ((sceGxtTextureInfo.flags & SCE_GXT_TEXTURE_FLAG_HAS_BORDER_DATA) != 0)
			{
				u32 uBorderDataSize = 0;
				if (!sce::Texture::Gxt::getBorderDataSize(uBorderDataSize, sceGxtTextureInfo.width, sceGxtTextureInfo.height, static_cast<SceGxmTextureFormat>(sceGxtTextureInfo.format)))
				{
					bResult = false;
					break;
				}
				uTexDataOffset += uBorderDataSize;
			}
			if (uTexDataOffset + data.m_totalSize > uDataEnd)
			{
				bResult = false;
				UPrintf(USTR("ERROR: data size is too small\n\n"));
				break;
			}
			data.m_data.resize(data.m_totalSize);
			data.m_palette16 = nullptr;
			data.m_palette256 = nullptr;
			memcpy(&*data.m_data.begin(), pGxt + uTexDataOffset, data.m_totalSize);
			if (sce::Texture::Gxt::isIndexed(static_cast<SceGxmTextureFormat>(sceGxtTextureInfo.format)))
			{
				if (sceGxtTextureInfo.paletteIndex == UINT32_MAX)
				{
					bResult = false;
					UPrintf(USTR("ERROR: palette index %08X error\n\n"), static_cast<n32>(sceGxtTextureInfo.paletteIndex));
					break;
				}
				u32 uBpp = 0;
				if (!sce::Texture::Gxt::getBpp(uBpp, static_cast<SceGxmTextureFormat>(sceGxtTextureInfo.format)))
				{
					bResult = false;
					break;
				}
				switch (uBpp)
				{
				case 4:
					if (sceGxtTextureInfo.paletteIndex >= pSceGxtHeader->numP4Palettes)
					{
						bResult = false;
						UPrintf(USTR("ERROR: palette16 index %08X error\n\n"), static_cast<n32>(sceGxtTextureInfo.paletteIndex));
						break;
					}
					data.m_palette16 = &*(m_vPalette16.begin() + sceGxtTextureInfo.paletteIndex);
					break;
				case 8:
					if (sceGxtTextureInfo.paletteIndex >= pSceGxtHeader->numP8Palettes)
					{
						bResult = false;
						UPrintf(USTR("ERROR: palette256 index %08X error\n\n"), static_cast<n32>(sceGxtTextureInfo.paletteIndex));
						break;
					}
					data.m_palette256 = &*(m_vPalette256.begin() + sceGxtTextureInfo.paletteIndex);
					break;
				}
				if (!bResult)
				{
					break;
				}
			}
			else
			{
				if (sceGxtTextureInfo.paletteIndex != UINT32_MAX)
				{
					bResult = false;
					UPrintf(USTR("ERROR: palette index %08X error\n\n"), static_cast<n32>(sceGxtTextureInfo.paletteIndex));
					break;
				}
			}
			m_vData.push_back(data);
		}
		if (!bResult)
		{
			break;
		}
	} while (false);
	delete[] pGxt;
	if (bResult)
	{
		UMkdir(m_sDirName.c_str());
		for (u32 i = 0; i < static_cast<u32>(m_vData.size()); i++)
		{
			sce::Texture::Gxt::Data& data = m_vData[i];
			u32 uNumFaces = data.m_type == SCE_GXM_TEXTURE_CUBE ? 6 : 1;
			u32 uOffset = 0;
			u32 uBpp = 0;
			if (!sce::Texture::Gxt::getBpp(uBpp, data.m_format))
			{
				bResult = false;
				break;
			}
			u32 uFaceAlignment = (uNumFaces > 1 && data.m_numLevels > 1) ? sce::Texture::Gxt::getFaceAlignment(uBpp, data.m_width) : 1;
			if (sce::Texture::Gxt::isBlockCompressed(data.m_format))
			{
				bResult = false;
				break;
			}
			else
			{
				u32 uWidthAlignment = data.m_type == SCE_GXM_TEXTURE_LINEAR ? SCE_GXM_TEXTURE_IMPLICIT_STRIDE_ALIGNMENT : 1;
				for (u32 j = 0; j < uNumFaces; j++)
				{
					u32 uFaceOffset = uOffset;
					u32 uMipWidth = data.m_width;
					u32 uMipHeight = data.m_height;
					u32 uMipWidthEx = data.m_numLevels > 1 ? sce::Texture::enclosingPowerOf2(data.m_width) : data.m_width;
					u32 uMipHeightEx = data.m_numLevels > 1 ? sce::Texture::enclosingPowerOf2(data.m_height) : data.m_height;
					uMipWidthEx = SCE_ALIGN(uMipWidthEx, uWidthAlignment);
					pvrtexture::CPVRTexture* pPVRTexture = nullptr;
					if (decode(&data, uOffset, uMipWidthEx, uMipHeightEx, &pPVRTexture) == 0)
					{
						UString sPngFileName = Format(USTR("%") PRIUS USTR("/%d_%d.png"), m_sDirName.c_str(), i, j);
						FILE* fpSub = UFopen(sPngFileName.c_str(), USTR("wb"));
						if (fpSub == nullptr)
						{
							delete pPVRTexture;
							bResult = false;
							break;
						}
						if (m_bVerbose)
						{
							UPrintf(USTR("save: %") PRIUS USTR("\n"), sPngFileName.c_str());
						}
						png_structp pPng = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
						if (pPng == nullptr)
						{
							fclose(fpSub);
							delete pPVRTexture;
							bResult = false;
							UPrintf(USTR("ERROR: png_create_write_struct error\n\n"));
							break;
						}
						png_infop pInfo = png_create_info_struct(pPng);
						if (pInfo == nullptr)
						{
							png_destroy_write_struct(&pPng, nullptr);
							fclose(fpSub);
							delete pPVRTexture;
							bResult = false;
							UPrintf(USTR("ERROR: png_create_info_struct error\n\n"));
							break;
						}
						if (setjmp(png_jmpbuf(pPng)) != 0)
						{
							png_destroy_write_struct(&pPng, &pInfo);
							fclose(fpSub);
							delete pPVRTexture;
							bResult = false;
							UPrintf(USTR("ERROR: setjmp error\n\n"));
							break;
						}
						png_init_io(pPng, fpSub);
						png_set_IHDR(pPng, pInfo, data.m_width, data.m_height, 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
						u8* pData = static_cast<u8*>(pPVRTexture->getDataPtr());
						png_bytepp pRowPointers = new png_bytep[data.m_height];
						for (u32 k = 0; k < data.m_height; k++)
						{
							pRowPointers[k] = pData + k * uMipWidthEx * 4;
						}
						png_set_rows(pPng, pInfo, pRowPointers);
						png_write_png(pPng, pInfo, PNG_TRANSFORM_IDENTITY, nullptr);
						png_destroy_write_struct(&pPng, &pInfo);
						delete[] pRowPointers;
						fclose(fpSub);
						delete pPVRTexture;
					}
					else
					{
						bResult = false;
						UPrintf(USTR("ERROR: decode error\n\n"));
						break;
					}
					for (u32 k = 0; k < data.m_numLevels; k++)
					{
						uOffset += uMipHeightEx * ((uMipWidthEx * uBpp + 7) / 8);
						uMipWidth = uMipWidth > 1 ? uMipWidth / 2 : 1;
						uMipHeight = uMipHeight > 1 ? uMipHeight / 2 : 1;
						uMipWidthEx = uMipWidthEx > 1 ? uMipWidthEx / 2 : 1;
						uMipHeightEx = uMipHeightEx > 1 ? uMipHeightEx / 2 : 1;
						uMipWidthEx = SCE_ALIGN(uMipWidthEx, uWidthAlignment);
					}
					if (j < uNumFaces - 1)
					{
						uOffset = uFaceOffset + SCE_ALIGN(uOffset - uFaceOffset, uFaceAlignment);
					}
				}
				if (!bResult)
				{
					break;
				}
			}
		}
	}
	return bResult;
}

bool CGxt::ImportFile()
{
	return false;
}

bool CGxt::IsGxtFile(const UString& a_sFileName)
{
	FILE* fp = UFopen(a_sFileName.c_str(), USTR("rb"));
	if (fp == nullptr)
	{
		return false;
	}
	fseek(fp, 0, SEEK_END);
	u32 uFileSize = ftell(fp);
	if (uFileSize < sizeof(SceGxtHeader))
	{
		fclose(fp);
		return false;
	}
	fseek(fp, 0, SEEK_SET);
	SceGxtHeader sceGxtHeader;
	fread(&sceGxtHeader, sizeof(sceGxtHeader), 1, fp);
	fclose(fp);
	return sceGxtHeader.tag == SCE_GXT_TAG;
}

int CGxt::decode(sce::Texture::Gxt::Data* a_pData, u32 a_uOffset, n32 a_nWidth, n32 a_nHeight, pvrtexture::CPVRTexture** a_pPVRTexture)
{
	u8* pRGBA = nullptr;
	switch (a_pData->m_format)
	{
	case SCE_GXM_TEXTURE_FORMAT_U8U8U8U8_ARGB:
	case SCE_GXM_TEXTURE_FORMAT_U8U8U8U8_RGBA:
	case SCE_GXM_TEXTURE_FORMAT_P8_RGBA:
		break;
	default:
		return 1;
	}
	switch (a_pData->m_format)
	{
	case SCE_GXM_TEXTURE_FORMAT_P8_RGBA:
		if (a_pData->m_palette256 == nullptr)
		{
			return 1;
		}
		pRGBA = new u8[a_nWidth * a_nHeight * 4];
		for (n32 i = 0; i < a_nWidth * a_nHeight; i++)
		{
			*reinterpret_cast<u32*>(pRGBA + i * 4) = *reinterpret_cast<u32*>(a_pData->m_palette256->m_data + *(&*a_pData->m_data.begin() + a_uOffset + i) * 4);
		}
		break;
	default:
		pRGBA = &*a_pData->m_data.begin() + a_uOffset;
		break;
	}
	PVRTextureHeaderV3 pvrTextureHeaderV3;
	switch (a_pData->m_format)
	{
	case SCE_GXM_TEXTURE_FORMAT_U8U8U8U8_ARGB:
		pvrTextureHeaderV3.u64PixelFormat = pvrtexture::PixelType('b', 'g', 'r', 'a', 8, 8, 8, 8).PixelTypeID;
		break;
	case SCE_GXM_TEXTURE_FORMAT_U8U8U8U8_RGBA:
		pvrTextureHeaderV3.u64PixelFormat = pvrtexture::PixelType('a', 'b', 'g', 'r', 8, 8, 8, 8).PixelTypeID;
		break;
	case SCE_GXM_TEXTURE_FORMAT_P8_RGBA:
		pvrTextureHeaderV3.u64PixelFormat = pvrtexture::PixelType('a', 'b', 'g', 'r', 8, 8, 8, 8).PixelTypeID;
		break;
	default:
		break;
	}
	pvrTextureHeaderV3.u32Height = a_nHeight;
	pvrTextureHeaderV3.u32Width = a_nWidth;
	MetaDataBlock metaDataBlock;
	metaDataBlock.DevFOURCC = PVRTEX3_IDENT;
	metaDataBlock.u32Key = ePVRTMetaDataTextureOrientation;
	metaDataBlock.u32DataSize = 3;
	metaDataBlock.Data = new PVRTuint8[metaDataBlock.u32DataSize];
	metaDataBlock.Data[0] = ePVRTOrientRight;
	metaDataBlock.Data[1] = ePVRTOrientUp;
	metaDataBlock.Data[2] = ePVRTOrientIn;
	pvrtexture::CPVRTextureHeader pvrTextureHeader(pvrTextureHeaderV3, 1, &metaDataBlock);
	*a_pPVRTexture = new pvrtexture::CPVRTexture(pvrTextureHeader, pRGBA);
	pvrtexture::Transcode(**a_pPVRTexture, pvrtexture::PVRStandard8PixelType, ePVRTVarTypeUnsignedByteNorm, ePVRTCSpacelRGB);
	switch (a_pData->m_format)
	{
	case SCE_GXM_TEXTURE_FORMAT_P8_RGBA:
		delete[] pRGBA;
		break;
	default:
		break;
	}
	return 0;
}
