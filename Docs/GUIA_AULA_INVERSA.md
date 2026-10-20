# Aula inversa — Motor Modbus RTU esclavo (STM32 + C++)

## Propósito
Cada equipo prepara una mini-clase (10–12 min) sobre una función; demuestra una trama completa, su CRC, la lógica de código, resultado en depurador y al menos un caso de error. Los demás equipos entregan una explicación de la respuesta recibida.

## Etapas sugeridas
1. Antes de clase: leer archivo asignado, su función `handle...` del archivo común y la documentación de Modbus Application Protocol; trazar una trama byte a byte.
2. En clase: explicar objetivo y campos (3 min), recorrer el código (4 min), ejecutar una prueba normal y otra inválida (3 min), responder preguntas (2 min).
3. Después de clase: entregar diagrama de flujo de la función, tabla de tramas, evidencia de ejecución y propuesta de mejora.

## Reparto
- Equipo 1: Modbus_CRC.cpp y Modbus_Helpers.cpp (orden de bytes CRC, desplazamientos).
- Equipo 2: Modbus_FC01.cpp y Modbus_ReadCommon.cpp (bits coils, LSB primero).
- Equipo 3: Modbus_FC02.cpp y Modbus_ReadCommon.cpp (inputs discretos).
- Equipo 4: Modbus_FC03.cpp y Modbus_ReadCommon.cpp (registros big endian).
- Equipo 5: Modbus_FC04.cpp y Modbus_ReadCommon.cpp (registros de solo lectura Modbus).
- Equipo 6: Modbus_FC05.cpp y Modbus_WriteCommon.cpp (0xFF00/0x0000).
- Equipo 7: Modbus_FC06.cpp y Modbus_WriteCommon.cpp (eco de solicitud).
- Equipo 8: Modbus_FC15.cpp y Modbus_WriteCommon.cpp (empaquetado de bits).
- Equipo 9: Modbus_FC16.cpp y Modbus_WriteCommon.cpp (byte count, registros).
- Equipo 10: Modbus_ProcessFrame.cpp (enrutamiento, ID, broadcast, CRC, excepciones).
- Tema transversal: Modbus_Registers.cpp (POO, atributos privados, métodos públicos).

**Importante:** el código de las FC está deliberadamente separado en archivos individuales; su implementación compartida está en ReadCommon/WriteCommon para evitar duplicar la lógica. Cada equipo debe explicar ambos archivos, no solo el wrapper.

## Preguntas para cada presentación
1. ¿Qué bytes tiene la petición y qué representa cada uno?
2. ¿Qué validaciones se hacen antes de leer o cambiar datos?
3. ¿Qué diferencia hay entre número hexadecimal y arreglo de bytes?
4. ¿Qué respuesta correcta se produce? Calcular CRC.
5. ¿Qué pasa si se cambia dirección, función, longitud o CRC?
6. ¿Cómo comprobaría el resultado en STM32CubeIDE sin UART?

## Rúbrica / 100 puntos
- Dominio del protocolo y estructura de trama: 25
- Explicación del código C++, POO y operaciones bit a bit: 25
- Demostración y casos de prueba: 25
- Claridad pedagógica y participación del equipo: 15
- Preguntas y análisis de errores: 10

## Precauciones
- Se procesan únicamente tramas RTU completas; detección por tiempos 3.5 caracteres y transporte UART se enseñarán después.
- Direcciones internas de registros base 0. Los rótulos 40001/30001 son una convención documental, no bytes del protocolo.
- Broadcast (ID 0) solo para escrituras, sin respuesta.
- Cada registro ocupa 16 bits y los valores se transmiten MSB primero; CRC se transmite LSB primero.
