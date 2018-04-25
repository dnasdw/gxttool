#include "gxttool.h"
#include "gxt.h"

CGxtTool::SOption CGxtTool::s_Option[] =
{
	{ USTR("export"), USTR('e'), USTR("export from the target file") },
	{ USTR("import"), USTR('i'), USTR("import to the target file") },
	{ USTR("check"), USTR('c'), USTR("check if the target file is a gxt file") },
	{ USTR("test-palette"), 0, USTR("test all palette") },
	{ USTR("file"), USTR('f'), USTR("the target file") },
	{ USTR("dir"), USTR('d'), USTR("the dir for the target file") },
	{ USTR("verbose"), USTR('v'), USTR("show the info") },
	{ USTR("help"), USTR('h'), USTR("show this help") },
	{ nullptr, 0, nullptr }
};

CGxtTool::CGxtTool()
	: m_eAction(kActionNone)
	, m_bVerbose(false)
{
}

CGxtTool::~CGxtTool()
{
}

int CGxtTool::ParseOptions(int a_nArgc, UChar* a_pArgv[])
{
	if (a_nArgc <= 1)
	{
		return 1;
	}
	for (int i = 1; i < a_nArgc; i++)
	{
		int nArgpc = static_cast<int>(UCslen(a_pArgv[i]));
		if (nArgpc == 0)
		{
			continue;
		}
		int nIndex = i;
		if (a_pArgv[i][0] != USTR('-'))
		{
			UPrintf(USTR("ERROR: illegal option\n\n"));
			return 1;
		}
		else if (nArgpc > 1 && a_pArgv[i][1] != USTR('-'))
		{
			for (int j = 1; j < nArgpc; j++)
			{
				switch (parseOptions(a_pArgv[i][j], nIndex, a_nArgc, a_pArgv))
				{
				case kParseOptionReturnSuccess:
					break;
				case kParseOptionReturnIllegalOption:
					UPrintf(USTR("ERROR: illegal option\n\n"));
					return 1;
				case kParseOptionReturnNoArgument:
					UPrintf(USTR("ERROR: no argument\n\n"));
					return 1;
				case kParseOptionReturnOptionConflict:
					UPrintf(USTR("ERROR: option conflict\n\n"));
					return 1;
				}
			}
		}
		else if (nArgpc > 2 && a_pArgv[i][1] == USTR('-'))
		{
			switch (parseOptions(a_pArgv[i] + 2, nIndex, a_nArgc, a_pArgv))
			{
			case kParseOptionReturnSuccess:
				break;
			case kParseOptionReturnIllegalOption:
				UPrintf(USTR("ERROR: illegal option\n\n"));
				return 1;
			case kParseOptionReturnNoArgument:
				UPrintf(USTR("ERROR: no argument\n\n"));
				return 1;
			case kParseOptionReturnOptionConflict:
				UPrintf(USTR("ERROR: option conflict\n\n"));
				return 1;
			}
		}
		i = nIndex;
	}
	return 0;
}

int CGxtTool::CheckOptions()
{
	if (m_eAction == kActionNone)
	{
		UPrintf(USTR("ERROR: nothing to do\n\n"));
		return 1;
	}
	if (m_eAction != kActionHelp)
	{
		if (m_sFileName.empty())
		{
			UPrintf(USTR("ERROR: no --file option\n\n"));
			return 1;
		}
		if (m_eAction != kActionCheck)
		{
			if (m_sDirName.empty())
			{
				UPrintf(USTR("ERROR: no --dir option\n\n"));
				return 1;
			}
			if (!CGxt::IsGxtFile(m_sFileName))
			{
				UPrintf(USTR("ERROR: %") PRIUS USTR(" is not a gxt file\n\n"), m_sFileName.c_str());
				return 1;
			}
		}
	}
	return 0;
}

int CGxtTool::Help()
{
	UPrintf(USTR("gxttool %") PRIUS USTR(" by dnasdw\n\n"), AToU(GXTTOOL_VERSION).c_str());
	UPrintf(USTR("usage: gxttool [option...] [option]...\n"));
	UPrintf(USTR("sample:\n"));
	UPrintf(USTR("  gxttool -evfd input.gxt outputdir\n"));
	UPrintf(USTR("  gxttool -ivfd output.gxt inputdir\n"));
	UPrintf(USTR("  gxttool -cf input.bin\n"));
	UPrintf(USTR("  gxttool --test-palette -vfd input.gxt testdir\n"));
	UPrintf(USTR("\n"));
	UPrintf(USTR("option:\n"));
	SOption* pOption = s_Option;
	while (pOption->Name != nullptr || pOption->Doc != nullptr)
	{
		if (pOption->Name != nullptr)
		{
			UPrintf(USTR("  "));
			if (pOption->Key != 0)
			{
				UPrintf(USTR("-%c,"), pOption->Key);
			}
			else
			{
				UPrintf(USTR("   "));
			}
			UPrintf(USTR(" --%-8") PRIUS, pOption->Name);
			if (UCslen(pOption->Name) >= 8 && pOption->Doc != nullptr)
			{
				UPrintf(USTR("\n%16") PRIUS, USTR(""));
			}
		}
		if (pOption->Doc != nullptr)
		{
			UPrintf(USTR("%") PRIUS, pOption->Doc);
		}
		UPrintf(USTR("\n"));
		pOption++;
	}
	return 0;
}

int CGxtTool::Action()
{
	if (m_eAction == kActionExport)
	{
		if (!exportFile())
		{
			UPrintf(USTR("ERROR: export file failed\n\n"));
			return 1;
		}
	}
	if (m_eAction == kActionImport)
	{
		if (!importFile())
		{
			UPrintf(USTR("ERROR: import file failed\n\n"));
			return 1;
		}
	}
	if (m_eAction == kActionCheck)
	{
		if (!CGxt::IsGxtFile(m_sFileName))
		{
			return 1;
		}
	}
	if (m_eAction == kActionTestPalette)
	{
		if (!testPalette())
		{
			UPrintf(USTR("ERROR: test palette failed\n\n"));
			return 1;
		}
	}
	if (m_eAction == kActionHelp)
	{
		return Help();
	}
	return 0;
}

CGxtTool::EParseOptionReturn CGxtTool::parseOptions(const UChar* a_pName, int& a_nIndex, int a_nArgc, UChar* a_pArgv[])
{
	if (UCscmp(a_pName, USTR("export")) == 0)
	{
		if (m_eAction == kActionNone)
		{
			m_eAction = kActionExport;
		}
		else if (m_eAction != kActionExport && m_eAction != kActionHelp)
		{
			return kParseOptionReturnOptionConflict;
		}
	}
	else if (UCscmp(a_pName, USTR("import")) == 0)
	{
		if (m_eAction == kActionNone)
		{
			m_eAction = kActionImport;
		}
		else if (m_eAction != kActionImport && m_eAction != kActionHelp)
		{
			return kParseOptionReturnOptionConflict;
		}
	}
	else if (UCscmp(a_pName, USTR("check")) == 0)
	{
		if (m_eAction == kActionNone)
		{
			m_eAction = kActionCheck;
		}
		else if (m_eAction != kActionCheck && m_eAction != kActionHelp)
		{
			return kParseOptionReturnOptionConflict;
		}
	}
	else if (UCscmp(a_pName, USTR("test-palette")) == 0)
	{
		if (m_eAction == kActionNone)
		{
			m_eAction = kActionTestPalette;
		}
		else if (m_eAction != kActionTestPalette && m_eAction != kActionHelp)
		{
			return kParseOptionReturnOptionConflict;
		}
	}
	else if (UCscmp(a_pName, USTR("file")) == 0)
	{
		if (a_nIndex + 1 >= a_nArgc)
		{
			return kParseOptionReturnNoArgument;
		}
		m_sFileName = a_pArgv[++a_nIndex];
	}
	else if (UCscmp(a_pName, USTR("dir")) == 0)
	{
		if (a_nIndex + 1 >= a_nArgc)
		{
			return kParseOptionReturnNoArgument;
		}
		m_sDirName = a_pArgv[++a_nIndex];
	}
	else if (UCscmp(a_pName, USTR("verbose")) == 0)
	{
		m_bVerbose = true;
	}
	else if (UCscmp(a_pName, USTR("help")) == 0)
	{
		m_eAction = kActionHelp;
	}
	return kParseOptionReturnSuccess;
}

CGxtTool::EParseOptionReturn CGxtTool::parseOptions(int a_nKey, int& a_nIndex, int m_nArgc, UChar* a_pArgv[])
{
	for (SOption* pOption = s_Option; pOption->Name != nullptr || pOption->Key != 0 || pOption->Doc != nullptr; pOption++)
	{
		if (pOption->Key == a_nKey)
		{
			return parseOptions(pOption->Name, a_nIndex, m_nArgc, a_pArgv);
		}
	}
	return kParseOptionReturnIllegalOption;
}

bool CGxtTool::exportFile()
{
	CGxt gxt;
	gxt.SetFileName(m_sFileName);
	gxt.SetDirName(m_sDirName);
	gxt.SetVerbose(m_bVerbose);
	return gxt.ExportFile();
}

bool CGxtTool::importFile()
{
	CGxt gxt;
	gxt.SetFileName(m_sFileName);
	gxt.SetDirName(m_sDirName);
	gxt.SetVerbose(m_bVerbose);
	return gxt.ImportFile();
}

bool CGxtTool::testPalette()
{
	CGxt gxt;
	gxt.SetFileName(m_sFileName);
	gxt.SetDirName(m_sDirName);
	gxt.SetVerbose(m_bVerbose);
	return gxt.TestPalette();
}

int UMain(int argc, UChar* argv[])
{
	CGxtTool tool;
	if (tool.ParseOptions(argc, argv) != 0)
	{
		return tool.Help();
	}
	if (tool.CheckOptions() != 0)
	{
		return 1;
	}
	return tool.Action();
}
