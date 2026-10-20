# ModbusRTU esclavo por funciones — STM32CubeIDE

Implementación de la **misma API** que la versión monolítica anterior, pero ahora distribuida por funciones en archivos `.cpp`. No necesita HAL/UART; usa arreglos `uint8_t`. C++11, sin asignación dinámica.

## Instalación
1. **Conservar** `main.cpp` que ya utiliza, y el archivo de cabecera `Core/Inc/ModbusRTU.hpp`.
2. Copiar **todos** los archivos `.cpp` de esta carpeta `Core/Src` al `Core/Src` del proyecto STM32CubeIDE.
3. **Eliminar o excluir del build el anterior** `ModbusRTU.cpp` monolítico. Si permanece, habrá errores de definiciones múltiples.
4. Actualizar `ModbusRTU.hpp` con el adjunto; no incluir archivos `.cpp` directamente.
5. Hacer Project > Clean y Build; garantizar compilación y enlace con C++.
6. Mantener `main.cpp` del proyecto generado, evitando reemplazar las inicializaciones de CubeMX.

## API pública conservada
`ModbusRTU modbus(1); modbus.setHoldingRegister(0,250); modbus.processFrame(rx,sizeof(rx),tx,sizeof(tx),txLength);`

Resultados: `ReplyReady`, `BroadcastDone`, `Ignored`, `BufferTooSmall`, `InvalidArgument`.

## Archivos
- `Modbus_FC01/02/03/04/05/06/15/16.cpp`: entrada independiente por función.
- `Modbus_ReadCommon.cpp`, `Modbus_WriteCommon.cpp`: implementación compartida para enseñar reutilización y evitar código duplicado.
- `Modbus_CRC.cpp`: cálculo/verificación.
- `Modbus_Helpers.cpp`: endianidad, excepciones, longitud y CRC de respuesta.
- `Modbus_Registers.cpp`: construcción del objeto y bancos de memoria.
- `Modbus_ProcessFrame.cpp`: enrutador/validador.

## Prueba local (Linux o MinGW)
`g++ -std=c++11 -Wall -Wextra -I Core/Inc Core/Src/*.cpp Tests/test_modbus.cpp -o test_modbus && ./test_modbus`

## Restricciones didácticas
Solo esclavo; buffers completos; sin UART ni demarcación temporal de trama; no implementar seguridad industrial ni operación en campo sin validar límites y casos de uso.

Leer `Docs/GUIA_AULA_INVERSA.md` para asignaciones y evaluación.
