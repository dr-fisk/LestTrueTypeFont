#ifndef LEST_TABLE_DATA_H
#define LEST_TABLE_DATA_H

#include <string>

enum TTFRequiredTablesBitMask
{
    TTF_CMAP = 0x0001,
    TTF_GLYF = 0x0002,
    TTF_HEAD = 0x0004,
    TTF_HHEA = 0x0008,
    TTF_HMTX = 0x0010,
    TTF_LOCA = 0x0020,
    TTF_MAXP = 0x0040,
    TTF_NAME = 0x0080,
    TTF_POST = 0x0100,
    TTF_OS2 = 0x0200,
    TTF_REQUIRED = 0x03FF,
    TTF_NOTREQ = 0x0000
};

enum TTFTableType
{
    TABLE_CMAP,
    TABLE_GLYF,
    TABLE_HEAD,
    TABLE_HHEA,
    TABLE_HMTX,
    TABLE_LOCA,
    TABLE_MAXP,
    TABLE_NAME,
    TABLE_POST,
    TABLE_OS2,
    TABLE_CVT,
    TABLE_FPGM,
    TABLE_HDMX,
    TABLE_KERN,
    TABLE_PREP,
    TABLE_VDMX,
    TABLE_GASP,
    TABLE_DSIG,
    TABLE_GDEF,
    TABLE_GSUB,
    TABLE_JSTF,
    TABLE_LTSH,
    TABLE_PCLT
};

struct TableData
{
    TTFRequiredTablesBitMask mask;
    TTFTableType type;
    bool delayProcessing;
};

static const std::string sCMAP = "cmap";
static const std::string sGLYF = "glyf";
static const std::string sHEAD = "head";
static const std::string sHHEA = "hhea";
static const std::string sHMTX = "hmtx";
static const std::string sLOCA = "loca";
static const std::string sMAXP = "maxp";
static const std::string sNAME = "name";
static const std::string sPOST = "post";
static const std::string sOS2  = "OS/2";
static const std::string sCVT  = "cvt ";
static const std::string sFPGM = "fpgm";
static const std::string sHDMX = "hdmx";
static const std::string sKERN = "kern";
static const std::string sPREP = "prep";
static const std::string sVDMX = "VDMX";
static const std::string sGASP = "gasp";
static const std::string sDSIG = "DSIG";
static const std::string sGDEF = "GDEF";
static const std::string sGSUB = "GSUB";
static const std::string sJSTF = "JSTF";
static const std::string sLTSH = "LTSH";
static const std::string sPCLT = "PCLT";

#endif