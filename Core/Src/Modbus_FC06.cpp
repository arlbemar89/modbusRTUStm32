#include "ModbusRTU.hpp"
// Equipo FC06: explicar campos, validaciones, efectos y respuesta/broadcast.
ModbusRTU::Result ModbusRTU::fc06(const uint8_t *req,std::size_t len,uint8_t *out,std::size_t cap,std::size_t &outLen,bool broadcast) {
    return handleWriteSingle(req,len,out,cap,outLen,broadcast,0x06);
}
