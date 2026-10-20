#include "ModbusRTU.hpp"
#include <cstring>
// La misma lógica de empaquetado sirve para FC01/02 (bits) y FC03/04 (registros).
ModbusRTU::Result ModbusRTU::handleReadBits(const uint8_t *req,std::size_t n,uint8_t *out,std::size_t cap,std::size_t &outLen,uint8_t fn) {
    if(n!=8) return Result::Ignored;
    uint16_t start=readU16(req+2), qty=readU16(req+4);
    if(qty==0 || qty>2000) return fail(fn,0x03,false,out,outLen);
    if(!validRange(start,qty,fn==0x01?CoilCount:DiscreteInputCount)) return fail(fn,0x02,false,out,outLen);
    const std::size_t bytes=(qty+7u)/8u, required=3u+bytes+2u;
    if(cap<required) return Result::BufferTooSmall;
    out[0]=slaveId_;out[1]=fn;out[2]=uint8_t(bytes);
    std::memset(out+3,0,bytes);
    for(uint16_t i=0;i<qty;++i) {
        bool value=fn==0x01?coils_[start+i]:discreteInputs_[start+i];
        if(value) out[3+i/8] |= uint8_t(1u<<(i%8));
    }
    appendCRC(out,required-2);outLen=required;return Result::ReplyReady;
}
ModbusRTU::Result ModbusRTU::handleReadRegisters(const uint8_t *req,std::size_t n,uint8_t *out,std::size_t cap,std::size_t &outLen,uint8_t fn) {
    if(n!=8) return Result::Ignored;
    uint16_t start=readU16(req+2),qty=readU16(req+4);
    if(qty==0 || qty>125) return fail(fn,0x03,false,out,outLen);
    if(!validRange(start,qty,fn==0x03?HoldingRegisterCount:InputRegisterCount)) return fail(fn,0x02,false,out,outLen);
    const std::size_t bytes=std::size_t(qty)*2u,required=3u+bytes+2u;
    if(cap<required) return Result::BufferTooSmall;
    out[0]=slaveId_;out[1]=fn;out[2]=uint8_t(bytes);
    for(uint16_t i=0;i<qty;++i) {
        uint16_t value=fn==0x03?holdingRegisters_[start+i]:inputRegisters_[start+i];
        writeU16(out+3+2u*i,value);
    }
    appendCRC(out,required-2);outLen=required;return Result::ReplyReady;
}
