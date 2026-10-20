#include "ModbusRTU.hpp"
#include <cstring>
ModbusRTU::Result ModbusRTU::handleWriteSingle(const uint8_t *req,std::size_t n,uint8_t *out,std::size_t cap,std::size_t &outLen,bool broadcast,uint8_t fn) {
    if(n!=8) return Result::Ignored;
    uint16_t addr=readU16(req+2),value=readU16(req+4);
    if(fn==0x05 && value!=0xFF00 && value!=0x0000) return fail(fn,0x03,broadcast,out,outLen);
    if(addr>=(fn==0x05?CoilCount:HoldingRegisterCount)) return fail(fn,0x02,broadcast,out,outLen);
    if(!broadcast && cap<8) return Result::BufferTooSmall;
    if(fn==0x05) coils_[addr]=(value==0xFF00);
    else holdingRegisters_[addr]=value;
    if(broadcast) return Result::BroadcastDone;
    std::memcpy(out,req,8);outLen=8;return Result::ReplyReady;
}
ModbusRTU::Result ModbusRTU::handleWriteMultiple(const uint8_t *req,std::size_t n,uint8_t *out,std::size_t cap,std::size_t &outLen,bool broadcast,uint8_t fn) {
    if(n<9) return Result::Ignored;
    uint16_t start=readU16(req+2),qty=readU16(req+4);
    uint8_t byteCount=req[6];bool bits=(fn==0x0F);
    if(qty==0 || qty>(bits?1968:123)) return fail(fn,0x03,broadcast,out,outLen);
    const std::size_t expected=bits?(qty+7u)/8u:std::size_t(qty)*2u;
    if(byteCount!=expected) return fail(fn,0x03,broadcast,out,outLen);
    if(n!=9u+byteCount) return Result::Ignored;
    if(!validRange(start,qty,bits?CoilCount:HoldingRegisterCount)) return fail(fn,0x02,broadcast,out,outLen);
    if(!broadcast && cap<8) return Result::BufferTooSmall;
    for(uint16_t i=0;i<qty;++i) {
        if(bits) coils_[start+i]=(req[7+i/8]&(1u<<(i%8)))!=0;
        else holdingRegisters_[start+i]=readU16(req+7+2u*i);
    }
    if(broadcast) return Result::BroadcastDone;
    std::memcpy(out,req,6);appendCRC(out,6);outLen=8;return Result::ReplyReady;
}
