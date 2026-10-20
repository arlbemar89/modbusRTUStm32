#include "ModbusRTU.hpp"
// Equipo FC15: explicar campos, validaciones, efectos y respuesta/broadcast.
ModbusRTU::Result ModbusRTU::fc15(const uint8_t *req,std::size_t len,uint8_t *out,std::size_t cap,std::size_t &outLen,bool broadcast) {
    return handleWriteMultiple(req,len,out,cap,outLen,broadcast,0x0F);
}
