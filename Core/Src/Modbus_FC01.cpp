#include "ModbusRTU.hpp"
// Equipo FC01: explicar campos, validaciones y cómo se codifica la respuesta.
ModbusRTU::Result ModbusRTU::fc01(const uint8_t *req,std::size_t len,uint8_t *out,std::size_t cap,std::size_t &outLen) {
    return handleReadBits(req,len,out,cap,outLen,0x01);
}
