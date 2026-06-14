# Propuesta 3 – Delta como Matriz Dispersa

## 1. Introducción

La Propuesta 3 se basa en construir el autómata usando una matriz para guardar las transiciones. En lugar de usar listas o quedarnos solo con el modelo teórico, esta opción mejora muchísimo el rendimiento del programa. ¿Cómo lo hace? Simplemente transformando los nombres de los estados y de los símbolos en números enteros (por ejemplo, el estado $q_0$ pasa a ser el índice 0, la letra 'a' pasa a ser el 0, y así sucesivamente).

Este nivel de diseño nos resulta particularmente útil para garantizar tiempos de respuesta óptimos durante la evaluación de cadenas. La transición desde un estado consumiendo un símbolo devuelve un conjunto de estados posibles accediendo a la estructura mediante coordenadas `Delta[estado][simbolo]`. Como posible desventaja, requiere un paso adicional de mapeo o traducción entre los nombres originales de los estados/símbolos y sus respectivos índices enteros antes de poder operar.

## 2. Análisis Comparativo con Otras Propuestas

Para justificar la elección de este diseño, analizamos sus diferencias fundamentales respecto a las alternativas planteadas:

### Comparación con la Propuesta 1 (Lista de Transiciones)

La Propuesta 1 almacena transiciones como aristas individuales y disjuntas. La Propuesta 3 agrupa los destinos y permite acceder a ellos de forma instantánea mediante coordenadas, en lugar de recorrer una lista buscando coincidencias.

| Característica | Propuesta 1 | Propuesta 3 (Nuestra Elección) |
|----------------|-------------|-------------------------------|
| **Enfoque** | Guarda aristas del grafo individuales de forma secuencial. | Guarda transiciones en una estructura matricial indexada. |
| **Abstracción** | $O(n)$ dado que requiere búsqueda secuencial. | $O(1)$ gracias al acceso directo por índices. |
| **Almacenamiento** | Requiere múltiples registros por símbolo si hay no determinismo. | Unifica todos los destinos en una sola celda de la matriz. |

### Comparación con la Propuesta 2 (Delta Indexado por Estado)

La Propuesta 2 asocia la lista de transiciones directamente dentro del struct de cada Estado (orientado fuertemente a grafos y punteros).

| Característica | Propuesta 2 | Propuesta 3 (Nuestra Elección) |
|----------------|-------------|-------------------------------|
| **Paradigma** | Estructura orientada a grafos distribuidos en memoria. | Estructura centralizada en una matriz de transición. |
| **Intuitividad** | Requiere saltar entre punteros de estados. | Intercepta filas y columnas matemáticamente. |

### Comparación con la Propuesta 4 (Delta Funcional)

La Propuesta 4 respeta los tipos de datos abstractos originales (State y Symbol), definiendo las rutas mediante la búsqueda de tuplas en un arreglo dinámico. La Propuesta 3 prioriza la velocidad de cómputo exigiendo la conversión numérica previa.

| Característica | Propuesta 4 | Propuesta 3 (Nuestra Elección) |
|----------------|-------------|-------------------------------|
| **Representación de Datos** | Mantiene la integridad de los nombres originales. | Exige la numeración estricta de elementos ($q_0 \to 0$). |
| **Eficiencia de Acceso** | Requiere procesar una búsqueda lineal en el arreglo funcional. | Ofrece un acceso matricial sumamente rápido catalogado como $O(1)$. |

### Comparación con la Propuesta 5 (Estados Compuestos para Determinización)

La Propuesta 5 plantea un modelado basado en subconjuntos enlazados para representar el AFD equivalente. Nuestra elección se centra en el modelo base del AFND matricial.

| Característica | Propuesta 5 | Propuesta 3 (Nuestra Elección) |
|----------------|-------------|-------------------------------|
| **Momento de Uso** | Entra en juego para generar el AFD resultante de la conversión. | Funciona como la estructura base donde reside el autómata original. |
| **Manejo de Estados** | Cada origen y destino es un estado compuesto (subconjunto). | El origen es un índice simple y el destino es un subconjunto (`Tdata`). |

## 3. Conclusión

La Propuesta 3 ofrece una representación de autómatas enfocada en el rendimiento computacional y la eficiencia algorítmica. Convierte toda la estructura del autómata en una matriz, que nos permite encontrar cualquier transición de manera rápida ($O(1)$). Si bien somos conscientes de que este enfoque implica un trabajo previo de mapeo (convertir la semántica de estados y símbolos a índices enteros de base cero), creemos que nos beneficia en el modelado del ingreso del autómata y en las consultas a la función delta. Como equipo, pensamos en la escalabilidad del código, redefinición de estados y la implementación de él en la materia correlativa a Teoría de la Computación.
