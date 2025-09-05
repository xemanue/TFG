# Validación de patrones generados por un producto electrónico microcontrolado 

- **Autor:** Jose Manuel García Cazorla
- **Tutor:** Andrés María Roldán Aranda

## Memoria

La memoria de este proyecto está escrita en LaTeX.

### Dependencias

- Latexmk, disponible a través de [TeX Live](https://www.tug.org/texlive/) entre otros
- GNU Make

### Compilación

Simplemente ejecutar `make` desde el directorio `docs`.

## Firmware

### Dependencias

- [PlatformIO](https://platformio.org/) (compilación y carga del código)
- [Doxygen](https://www.doxygen.nl/index.html) (generación de la documentación)
- GNU Make

### Compilación

Para compilar y cargar el código en un Arduino Mega 2560, usar la interfaz de [PlatformIO](https://platformio.org/).

### Documentación

Para generar la documentación, ejecutar `make` desde el directorio `code/firmware`. Después, abrir `index.hmtl`, generado en el directorio `doxyfiles/html`.

## Interfaz

### Dependencias

- Python v3.13.7
- Librerías incluidas en `requirements.txt`
- [Doxygen](https://www.doxygen.nl/index.html) (generación de la documentación)
- GNU Make

### Uso

En primer lugar, se ha de [generar un entorno virtual de Python](https://docs.python.org/3/library/venv.html).

Desde dicho entorno, instalar las librerías necesarias ejecutando `pip install -r requirements.txt`

Para ejecutar, usar `./run` o `.\run.ps1`, dependiendo del sistema operativo.

### Documentación

Para generar la documentación, ejecutar `make` desde el directorio `code/ui`. Después, abrir `index.hmtl`, generado en el directorio `doxyfiles/html`.
