#include "ModbusRTU.hpp"
#include <cassert>
#include <cstring>
#include <cstdio>
static std::size_t frame(uint8_t *buffer,std::size_t n) {
    uint16_t crc=ModbusRTU::calculateCRC(buffer,n);
    buffer[n++]=uint8_t(crc);buffer[n++]=uint8_t(crc>>8);return n;
}
int main() {
    ModbusRTU mb(1);uint8_t out[256];std::size_t outLen=0;
    assert(mb.setHoldingRegister(0,250));
    uint8_t r03[8]={1,3,0,0,0,2};frame(r03,6);
    assert(mb.processFrame(r03,8,out,sizeof(out),outLen)==ModbusRTU::Result::ReplyReady);
    assert(outLen==9 && out[3]==0 && out[4]==250 && ModbusRTU::checkCRC(out,outLen));
    uint8_t r06[8]={1,6,0,0,0x12,0x34};frame(r06,6);
    assert(mb.processFrame(r06,8,out,sizeof(out),outLen)==ModbusRTU::Result::ReplyReady);
    uint16_t value;assert(mb.getHoldingRegister(0,value)&&value==0x1234);
    uint8_t r01[8]={1,1,0,0,0,3};frame(r01,6);mb.setCoil(0,true);mb.setCoil(2,true);
    assert(mb.processFrame(r01,8,out,sizeof(out),outLen)==ModbusRTU::Result::ReplyReady && out[3]==5);
    uint8_t r02[8]={1,2,0,0,0,3};frame(r02,6);mb.setDiscreteInput(1,true);
    assert(mb.processFrame(r02,8,out,sizeof(out),outLen)==ModbusRTU::Result::ReplyReady && out[3]==2);
    uint8_t r04[8]={1,4,0,0,0,1};frame(r04,6);mb.setInputRegister(0,0xABCD);
    assert(mb.processFrame(r04,8,out,sizeof(out),outLen)==ModbusRTU::Result::ReplyReady && out[3]==0xAB && out[4]==0xCD);
    uint8_t r05[8]={1,5,0,1,0xFF,0};frame(r05,6);
    assert(mb.processFrame(r05,8,out,sizeof(out),outLen)==ModbusRTU::Result::ReplyReady);
    bool bit;assert(mb.getCoil(1,bit)&&bit);
    uint8_t r15[10]={1,0x0F,0,0,0,3,1,0x03};frame(r15,8);
    assert(mb.processFrame(r15,10,out,sizeof(out),outLen)==ModbusRTU::Result::ReplyReady);
    assert(mb.getCoil(1,bit)&&bit && mb.getCoil(2,bit)&&!bit);
    uint8_t r16[13]={1,0x10,0,0,0,2,4,0,10,0,20};frame(r16,11);
    assert(mb.processFrame(r16,13,out,sizeof(out),outLen)==ModbusRTU::Result::ReplyReady);
    assert(mb.getHoldingRegister(1,value)&&value==20);
    r03[7]^=1;assert(mb.processFrame(r03,8,out,sizeof(out),outLen)==ModbusRTU::Result::Ignored);
    uint8_t illegal[8]={1,0x55,0,0,0,1};frame(illegal,6);
    assert(mb.processFrame(illegal,8,out,sizeof(out),outLen)==ModbusRTU::Result::ReplyReady && out[1]==0xD5 && out[2]==1);
    uint8_t bc[8]={0,6,0,0,0,99};frame(bc,6);
    assert(mb.processFrame(bc,8,out,sizeof(out),outLen)==ModbusRTU::Result::BroadcastDone && outLen==0);
    assert(mb.getHoldingRegister(0,value)&&value==99);
    std::puts("TODAS LAS PRUEBAS OK");
}
