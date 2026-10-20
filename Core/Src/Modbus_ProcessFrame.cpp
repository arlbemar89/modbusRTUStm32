#include "ModbusRTU.hpp"
// Punto único de entrada: valida trama y despacha a la función adecuada.
ModbusRTU::Result ModbusRTU::processFrame(const uint8_t *req,std::size_t n,uint8_t *out,std::size_t cap,std::size_t &outLen) {
    outLen=0;
    if(!req || !out) return Result::InvalidArgument;
    if(n<4 || n>MaxFrame || !checkCRC(req,n)) return Result::Ignored;
    const bool broadcast=(req[0]==0);
    if(!broadcast && req[0]!=slaveId_) return Result::Ignored;
    const uint8_t fn=req[1];
    const bool write=(fn==0x05 || fn==0x06 || fn==0x0F || fn==0x10);
    if(broadcast && !write) return Result::Ignored;
    if(cap<5 && !broadcast) return Result::BufferTooSmall;
    switch(fn) {
        case 0x01: return fc01(req,n,out,cap,outLen);
        case 0x02: return fc02(req,n,out,cap,outLen);
        case 0x03: return fc03(req,n,out,cap,outLen);
        case 0x04: return fc04(req,n,out,cap,outLen);
        case 0x05: return fc05(req,n,out,cap,outLen,broadcast);
        case 0x06: return fc06(req,n,out,cap,outLen,broadcast);
        case 0x0F: return fc15(req,n,out,cap,outLen,broadcast);
        case 0x10: return fc16(req,n,out,cap,outLen,broadcast);
        default: return fail(fn,0x01,broadcast,out,outLen);
    }
}
