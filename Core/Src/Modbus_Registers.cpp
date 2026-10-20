#include "ModbusRTU.hpp"
// Cada objeto tiene sus propios bancos de registros.
ModbusRTU::ModbusRTU(uint8_t id) : slaveId_((id >= 1 && id <= 247) ? id : 1) {}
bool ModbusRTU::setSlaveId(uint8_t id) { if(id < 1 || id > 247) return false; slaveId_ = id; return true; }
uint8_t ModbusRTU::slaveId() const { return slaveId_; }
bool ModbusRTU::setCoil(std::size_t i,bool v) { if(i>=CoilCount) return false; coils_[i]=v; return true; }
bool ModbusRTU::getCoil(std::size_t i,bool &v) const { if(i>=CoilCount) return false; v=coils_[i]; return true; }
bool ModbusRTU::setDiscreteInput(std::size_t i,bool v) { if(i>=DiscreteInputCount) return false; discreteInputs_[i]=v; return true; }
bool ModbusRTU::getDiscreteInput(std::size_t i,bool &v) const { if(i>=DiscreteInputCount) return false; v=discreteInputs_[i]; return true; }
bool ModbusRTU::setHoldingRegister(std::size_t i,uint16_t v) { if(i>=HoldingRegisterCount) return false; holdingRegisters_[i]=v; return true; }
bool ModbusRTU::getHoldingRegister(std::size_t i,uint16_t &v) const { if(i>=HoldingRegisterCount) return false; v=holdingRegisters_[i]; return true; }
bool ModbusRTU::setInputRegister(std::size_t i,uint16_t v) { if(i>=InputRegisterCount) return false; inputRegisters_[i]=v; return true; }
bool ModbusRTU::getInputRegister(std::size_t i,uint16_t &v) const { if(i>=InputRegisterCount) return false; v=inputRegisters_[i]; return true; }
