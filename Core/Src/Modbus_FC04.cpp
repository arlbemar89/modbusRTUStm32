#include "ModbusRTU.hpp"
// Equipo FC04: explicar campos, validaciones y cómo se codifica la respuesta.
ModbusRTU::Result ModbusRTU::fc04(const uint8_t *req,std::size_t len,uint8_t *out,std::size_t cap,std::size_t &outLen) {
    return handleReadRegisters(req,len,out,cap,outLen,0x04);
}
