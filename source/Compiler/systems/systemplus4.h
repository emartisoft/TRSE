#ifndef SYSTEMPLUS4_H
#define SYSTEMPLUS4_H


#include "systemmos6502.h"

class SystemPlus4 : public SystemMOS6502
{
public:
    SystemPlus4(CIniFile* settings, CIniFile* proj) : SystemMOS6502(settings, proj) {
        m_processor = MOS6502;
        m_system = PLUS4;
        m_startAddress = 0x1000;
        m_programStartAddress = 0x1010;
    }
};


#endif // SYSTEMPLUS4_H
