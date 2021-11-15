# Emulador MIPS
Este es un proyecto con el propósito de realizar un emulador (incompleto) de la arquitectura MIPS capaz de emular los proyectos [Wizard of Wor-ASM](https://github.com/zkwinkle/Wizard-of-Wor-ASM) o [MIPS-PONG](https://github.com/AndrewHamm/MIPS-Pong). Está realizado en C y utiliza la librería SDL para crear el bitmap display.

## Instrucciones de uso
Para compilar el juego solo corra el comando `make` estando en la carpeta "root" del proyecto. Luego para ejecutar el simulador de solo corra `./mips`. Para salir de la aplicación presione la tecla 'Esc'.

Para escoger cuál juego correr por default está cargado Wizard of Wor, pero si desea correr Pong solo debe cambiarle el nombre a los archivos 'data.hex' y 'text.hex' a alguna otra cosa y luego quitarle el sufijo de 'pong_' a los archivos 'pong_data.hex' y 'pong_text.hex'.
El simulador solo sabe que debe de buscar los archivos 'data.hex' y 'text.hex'.

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
