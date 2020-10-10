#pragma once
#include "../../../DNASPLIB/LLD.h"
#include "../../../DNASPLIB/dsbob2.h"

#if _WIN32
extern "C" int __declspec(dllexport)InitMath(DSBOB2* dsbob2, CLLD* lld, int isErrLog = 0, int isPrintMidResult = 0);
#elif __linux__
int InitMath(DSBOB2* dsbob2, CLLD* lld, int isErrLog = 0, int isPrintMidResult = 0);
#else
#endif


