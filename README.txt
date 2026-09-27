Tetris - Proyecto de Estructuras de Datos - Anddy Zúñiga Vega 

Este es un proyecto base para trabajar con la biblioteca SFML (http://www.sfml-dev.org). 

Toda la interfaz visual, renderizado de figuras, textos y ventanas emergentes fueron desarrollados utilizando la biblioteca SFML 2.5 (Simple and Fast Multimedia Library).

La documentación de SFML se encuentra en el directorio MinGW/SFML-2.1/doc de ZinjaI (se puede acceder directamente mediante el último ícono de la barra de herramientas).

En la distribución para Windows el enlazado de la biblioteca SFML se realiza de forma estática. Para realizarlo contra la versión dinámica debe quitar el -s al final del nombre de cada librería en las opciones de enlazado, y la macro SFML_STATIC de las opciones de compilación. En este caso también será necesario colocar las dlls que están en el directorio MinGW/SFML-2.1/bin en un lugar donde el ejecutable pueda encontrarlas (por ejemplo, en la carpeta del proyecto si el ejecutable la utiliza como directorio de trabajo).

Un juego Tetris desarrollado en C++ aplicando programación orientada a objetos bajo una arquitectura similar a MVC (Modelo-Vista-Controlador).

Este proyecto fue desarrollado como entrega final para la asignatura de Estructuras de Datos, implementando estructuras dinámicas.

Cómo Jugar (Controles)
El objetivo es sobrevivir el mayor tiempo posible y hacer líneas para ganar puntos.

Durante la partida:
- Flecha Izquierda / Flecha Derecha: Mover la pieza a los lados.
- Flecha Arriba: Rotar la pieza.
- Flecha Abajo: Acelerar la caída de la pieza.
- Barra Espaciadora: Caída instantánea (Hard Drop).
- Tecla C: Guardar la pieza actual en la reserva (Hold) o cambiarla por la guardada.
- Tecla Esc: Pausar el juego.

Modo Replay (Al terminar el juego):
- Flecha Izquierda / A: Retroceder un paso en el tiempo.
- Flecha Derecha / D: Avanzar un paso en el tiempo.
- Enter / Esc: Salir del modo replay.

¿Cómo funcionan los Eventos?
El juego cuenta con un sistema de eventos programados (como acelerar la velocidad del juego o bonificadores temporales). Estos no ocurren al azar, sino que utilizan una Cola de Prioridad (ColaEventos) implementada con listas enlazadas. 

Cada vez que el juego genera un evento, este se inserta automáticamente en la posición correcta de la cola dependiendo de su momento de disparo (tiempo en milisegundos). El motor del juego revisa constantemente el frente de la cola y, cuando el tiempo del juego alcanza el tiempo del evento, este se extrae y aplica su efecto en la pantalla.

Compilación y Ejecución:

El proyecto está configurado para ser compilado en el IDE ZinjaI.

1. Asegúrate de tener ZinjaI instalado junto con el compilador MinGW.
2. Descarga la carpeta completa del proyecto (Asegurándote de que la carpeta interna assets esté presente).
3. Abre el archivo del proyecto llamado TetrisA.zpr.
4. En ZinjaI, presiona la tecla F9 (o ve al menú Ejecutar -> Compilar y Ejecutar).