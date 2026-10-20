#include "ModbusRTU.hpp"
uint16_t ModbusRTU::readU16(const uint8_t *p) { return static_cast<uint16_t>((uint16_t(p[0])<<8)|p[1]); }
void ModbusRTU::writeU16(uint8_t *p,uint16_t value) { p[0]=uint8_t(value>>8);p[1]=uint8_t(value); }
void ModbusRTU::appendCRC(uint8_t *frame,std::size_t len) {
    const uint16_t crc=calculateCRC(frame,len);
    frame[len]=uint8_t(crc);frame[len+1]=uint8_t(crc>>8);
}
bool ModbusRTU::validRange(uint16_t start,uint16_t quantity,std::size_t count) {
    return quantity>0 && start<count && quantity<=count-start;
}
std::size_t ModbusRTU::exceptionFrame(uint8_t id,uint8_t fn,uint8_t code,uint8_t *out) {
    out[0]=id;out[1]=uint8_t(fn|0x80u);out[2]=code;appendCRC(out,3);return 5;
}
ModbusRTU::Result ModbusRTU::fail(uint8_t fn,uint8_t code,bool broadcast,uint8_t *out,std::size_t &outLen) const {
    if(broadcast) return Result::Ignored;
    outLen=exceptionFrame(slaveId_,fn,code,out);
    return Result::ReplyReady;
}
