#ifndef __BUILDCFG_BUILDCFG_H__
#define __BUILDCFG_BUILDCFG_H__

/*

1   打开本工程的主资源文件，删除原有的版本信息，将version.rc添加到工程

2   打开 trunk/builder/buildcfg 目录下对应的buildcfg.h文件
	找到该工程所在位置，请用英语定义以下三个宏，如：	

#ifdef ETM_ETDBE // size : 20000
	#pragma comment(linker, "/base:0x4DA00000")   
	#pragma message("\t    ETM_ETDBE")
	#define VI_FILEDESCRIPTION	"Data Import"	//文件描述
	#define VI_INTERNALNAME		"dbe"			//内部名称
	#define VI_ORIGINALFILENAME	"etdbe.dll"		//原始DLL文件名
#endif

3.  在工程设置的 Resources 页的"Preprocessor definitions"中添加本工程对应的宏，如 ETM_ETDBE
	请注意所有配置(Debug/Release/...)都要加上
	在"Additionalresource include directories"中加上Coding/include目录的相对位置
	如 "../../builder/buildcfg"
	
 */

// -------------------------------------------------------------------------

#ifdef _WINDOWS_
#error "include buildcfg.h before include all other files"
#endif

/////////////////////////////////
//exe
/**
here is a exe example:
#ifdef KISM_KAV32
	#pragma message("\t    KISM_KAV32")
	#define VI_FILEDESCRIPTION	"Kingsoft AntiVirus"
	#define VI_INTERNALNAME		"kav32"
	#define VI_ORIGINALFILENAME	"kav32.exe"
#endif
*/

#ifdef BEIKE_SAFE_EXE
	#pragma message("\t    BEIKE_SAFE_EXE")
	#define VI_FILEDESCRIPTION	"金山卫士主程序"
	#define VI_INTERNALNAME		"KSafe"
	#define VI_ORIGINALFILENAME	"KSafe.exe"
#endif

#ifdef VUL_FIX_DLL
    #pragma message("\t    VUL_FIX_DLL")
    #define VI_FILEDESCRIPTION	"KSafe Vulnerability Fix"
    #define VI_INTERNALNAME		"ksafevul"
    #define VI_ORIGINALFILENAME	"ksafevul.dll"
#endif



#endif /* __BUILDCFG_BUILDCFG_H__ */

