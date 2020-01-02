#ifndef SYSTEMOK64_H
#define SYSTEMOK64_H

#include "abstractsystem.h"
#include "systemmos6502.h"

class SystemOK64 : public SystemMOS6502
{
public:
    SystemOK64(CIniFile* settings, CIniFile* proj) : SystemMOS6502(settings, proj) {
        m_processor = MOS6502;
        m_system = OK64;
        m_startAddress = 0x03FF;
        m_programStartAddress = 0x03FF;
        m_memorySize = 65536 + 65536*16;
    }
};

#endif // SYSTEMOK64_H
