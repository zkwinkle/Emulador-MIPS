# Emulador MIPS
Este es un proyecto con el propósito de realizar un emulador (incompleto) de la arquitectura MIPS capaz de emular el proyecto [MIPS-PONG](https://github.com/AndrewHamm/MIPS-Pong). Está realizado en C y utiliza la librería SDL para crear el bitmap display.

## Instrucciones de uso
Para compilar el juego solo corra el comando `make` estando en la carpeta "root" del proyecto. Luego para ejecutar el simulador de Pong en ensamblador solo corra `./mips`.

## Datos técnicos

### Memorias

Las memorias simuladas para correr el programa Pong son:

- Banco de registros (32 direcciones)
- Heap (32kB)
- Stack (32kB)
- Static Data (4kB)
- MMIO (4 direcciones)
- Text (Archivo de texto)

Para todas las memorias se hace aritmética simple para redireccionar las direcciones de memoria de MARS a los índices de los arrays respectivos; excepto por la memoria Text, las instrucciones se leen directamente del archivo text.hex en tiempo real y las direcciones se traducen al número de línea en el archivo.

#### Memoria de instrucciones
El mismo archivo text.hex es tratado como la memoria de instrucciones, el $pc es solo el índice de por cuál línea del archivo se está. Por este motivo las direcciones de memoria de instrucciones se tratan de 1 en 1 (en vez de 4 en 4).
