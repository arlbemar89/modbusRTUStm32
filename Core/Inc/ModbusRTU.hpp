#ifndef MODBUS_RTU_HPP
#define MODBUS_RTU_HPP
#include <cstddef>
#include <cstdint>

// Motor de protocolo Modbus RTU esclavo. Sin UART ni HAL.
// Se recibe una trama completa de bytes, no texto ASCII.
class ModbusRTU {
public:
    static constexpr std::size_t MaxFrame = 256;
    static constexpr std::size_t CoilCount = 64;
    static constexpr std::size_t DiscreteInputCount = 64;
    static constexpr std::size_t HoldingRegisterCount = 32;
    static constexpr std::size_t InputRegisterCount = 32;
    enum class Result : uint8_t { ReplyReady, BroadcastDone, Ignored, BufferTooSmall, InvalidArgument };
    explicit ModbusRTU(uint8_t slaveId = 1);
    bool setSlaveId(uint8_t id);
    uint8_t slaveId() const;
    bool setCoil(std::size_t address, bool value);
    bool getCoil(std::size_t address, bool &value) const;
    bool setDiscreteInput(std::size_t address, bool value);
    bool getDiscreteInput(std::size_t address, bool &value) const;
    bool setHoldingRegister(std::size_t address, uint16_t value);
    bool getHoldingRegister(std::size_t address, uint16_t &value) const;
    bool setInputRegister(std::size_t address, uint16_t value);
    bool getInputRegister(std::size_t address, uint16_t &value) const;
    static uint16_t calculateCRC(const uint8_t *data, std::size_t length);
    static bool checkCRC(const uint8_t *frame, std::size_t length);
    Result processFrame(const uint8_t *request, std::size_t requestLength,
                        uint8_t *response, std::size_t responseCapacity,
                        std::size_t &responseLength);
private:
    uint8_t slaveId_;
    bool coils_[CoilCount]{};
    bool discreteInputs_[DiscreteInputCount]{};
    uint16_t holdingRegisters_[HoldingRegisterCount]{};
    uint16_t inputRegisters_[InputRegisterCount]{};
    static uint16_t readU16(const uint8_t *p);
    static void writeU16(uint8_t *p, uint16_t value);
    static void appendCRC(uint8_t *frame, std::size_t lengthWithoutCRC);
    static bool validRange(uint16_t start, uint16_t quantity, std::size_t count);
    static std::size_t exceptionFrame(uint8_t id, uint8_t fn, uint8_t exception, uint8_t *out);
    Result fail(uint8_t fn, uint8_t code, bool broadcast, uint8_t *out, std::size_t &outLen) const;
    Result handleReadBits(const uint8_t *req, std::size_t n, uint8_t *out, std::size_t cap, std::size_t &outLen, uint8_t fn);
    Result handleReadRegisters(const uint8_t *req, std::size_t n, uint8_t *out, std::size_t cap, std::size_t &outLen, uint8_t fn);
    Result handleWriteSingle(const uint8_t *req, std::size_t n, uint8_t *out, std::size_t cap, std::size_t &outLen, bool broadcast, uint8_t fn);
    Result handleWriteMultiple(const uint8_t *req, std::size_t n, uint8_t *out, std::size_t cap, std::size_t &outLen, bool broadcast, uint8_t fn);
    Result fc01(const uint8_t*,std::size_t,uint8_t*,std::size_t,std::size_t&);
    Result fc02(const uint8_t*,std::size_t,uint8_t*,std::size_t,std::size_t&);
    Result fc03(const uint8_t*,std::size_t,uint8_t*,std::size_t,std::size_t&);
    Result fc04(const uint8_t*,std::size_t,uint8_t*,std::size_t,std::size_t&);
    Result fc05(const uint8_t*,std::size_t,uint8_t*,std::size_t,std::size_t&,bool);
    Result fc06(const uint8_t*,std::size_t,uint8_t*,std::size_t,std::size_t&,bool);
    Result fc15(const uint8_t*,std::size_t,uint8_t*,std::size_t,std::size_t&,bool);
    Result fc16(const uint8_t*,std::size_t,uint8_t*,std::size_t,std::size_t&,bool);
};
#endif
