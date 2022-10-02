#include <iostream>
#include "lib.h"

#define DE_SCRIPT_API extern "C" __declspec(dllexport)

DE_SCRIPT_API void funci() { Print(); }