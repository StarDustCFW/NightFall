/*Copyright (c) 2020 D3fau4

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

#include <stdio.h>
#include <switch.h>
#include "spl/spl.hpp"

namespace spl
{

    static constexpr u32 ExosphereHasRcmBugPatch = 65004;
    static constexpr u32 ExosphereEmummcType = 65007;
    
    std::string GetHardwareType(void)
    {
        Result ret = 0;
		std::string MDL="Unknown";
		//Model of switch board
		SetSysProductModel modelo;
		if (R_FAILED(ret = setsysGetProductModel(&modelo))) {
			printf("setsysGetProductModel() Failied: 0x%x.\n\n", ret);
		} else {
			switch(modelo) {
				case SetSysProductModel_Invalid:
					MDL="Invalid";// Erista normal
					break;
				case SetSysProductModel_Nx:
					MDL="Icosa";// Erista normal
					break;
				case SetSysProductModel_Copper:
					MDL="Copper";// Erista prototype
					break;
				case SetSysProductModel_Iowa:
					MDL="Iowa";// Mariko retail
					break;
				case SetSysProductModel_Hoag:
					MDL="Hoag";// Mariko lite
					break;
				case SetSysProductModel_Calcio:
					MDL="Calcio";// Mariko prototype
					break;
				case SetSysProductModel_Aula:
					MDL="Aula";// nx-abcd board
					break;
			}
		}
        return MDL;
    }

    bool HasRCMbugPatched(void)
    {
        Result ret = 0;
        u64 has_rcm_bug_patch;
        if (R_SUCCEEDED(ret = splGetConfig(static_cast<SplConfigItem>(ExosphereHasRcmBugPatch), &has_rcm_bug_patch)))
        {
            return has_rcm_bug_patch;
        }
        else
        {
            return has_rcm_bug_patch;
        }
    }

    bool HasEmummc(void)
    {
        Result ret = 0;
        u64 IsEmummc;

        if (R_SUCCEEDED(ret = splGetConfig(static_cast<SplConfigItem>(ExosphereEmummcType), &IsEmummc)))
        {
            return IsEmummc;
        }
        else
        {
            return IsEmummc;
        }
    }

} // namespace spl