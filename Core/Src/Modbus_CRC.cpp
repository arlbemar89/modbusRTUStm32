#include "ModbusRTU.hpp"
// CRC-16/Modbus, polinomio reflejado 0xA001, valor inicial 0xFFFF.
uint16_t ModbusRTU::calculateCRC(const uint8_t *data,std::size_t len) {
    if(!data && len) return 0;
    uint16_t crc=0xFFFF;
    for(std::size_t i=0;i<len;++i) {
        crc ^= data[i];
        for(uint8_t bit=0;bit<8;++bit)
            crc=(crc&1u)? static_cast<uint16_t>((crc>>1)^0xA001u) : static_cast<uint16_t>(crc>>1);
    }
    return crc;
}
bool ModbusRTU::checkCRC(const uint8_t *frame,std::size_t len) {
    return frame && len>=4 && len<=MaxFrame &&
           calculateCRC(frame,len-2)==static_cast<uint16_t>(frame[len-2] | (uint16_t(frame[len-1])<<8));
}
