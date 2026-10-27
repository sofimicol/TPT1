# 1. Introducción

## 1.1 Objetivo

Nuestro objetivo principal en este Trabajo Práctico Transversal consiste en diseñar, desarrollar e implementar un intérprete funcional para un nuevo lenguaje de programación denominado Aleph [Louden, 2004, p. 11]. En el diseño de Aleph nos enfocamos en proporcionar abstracciones generales de alto nivel para el programador, priorizando la manipulación declarativa y algorítmica de colecciones de datos, específicamente conjuntos no ordenados y listas ordenadas, mediante operaciones algebraicas nativas, junto con estructuras de control estructuradas que nos permitan modelar, anidar y recorrer datos de forma sistemática y legible [Louden, 2004, p. 11].

Para evaluar la expresividad, legibilidad y confiabilidad del lenguaje, tomamos casos de estudio concretos provenientes de la Teoría de la Computación. En particular, nuestro lenguaje Aleph debe permitir expresar la definición formal de Autómatas Finitos (AF), ejecutar la conversión de un Autómata Finito No Determinista (AFND) a un Autómata Finito Determinista (AFD), evaluar la aceptación o rechazo de cadenas de entrada y vincular las representaciones estructurales entre Lenguajes Regulares y Gramáticas Regulares.

Consideramos fundamental recalcar que estos algoritmos formales no constituyen primitivas ni tipos de datos predefinidos en la sintaxis de Aleph; representan el dominio de prueba sobre el cual evaluamos la potencia representativa de las abstracciones de datos y de control que hemos diseñado.

Nuestro proyecto abarca las fases de análisis, diseño e implementación de software de sistemas, abordando formalmente el análisis léxico, el análisis sintáctico y la interpretación semántica basada en estructuras de datos dinámicas en memoria.

## 1.2 Sobre el nombre del lenguaje

Denominamos al lenguaje Aleph debido a su relación conceptual con la Teoría de Conjuntos, marco fundacional de su modelo de datos.

En la matemática formal, la secuencia de los números Álef ($\aleph$) nos permite denotar la cardinalidad, concepto que define el tamaño cuantitativo o cantidad de elemento, de conjuntos transfinitos bien ordenados. Georg Cantor desarrolló esta formulación al demostrar que no todos los conjuntos infinitos poseen la misma magnitud cardinal:

* **Álef cero ($\aleph_0$):** Representa la menor cardinalidad transfinita, correspondiente a los conjuntos infinitos numerables, tales como el conjunto de los números naturales ($\mathbb{N}$), los números enteros ($\mathbb{Z}$) o los números racionales ($\mathbb{Q}$).
* **Álef uno ($\aleph_1$):** Representa, bajo la Hipótesis del Continuo de Cantor, el cardinal inmediato superior que describe la potencia del continuo matemático, correspondiente al conjunto de los números reales ($\mathbb{R}$).

En la computación teórica, las propiedades demostradas por Cantor posibilitan que un sistema lógico formal con reglas de derivación finitas sea capaz de definir y procesar conjuntos infinitos de cadenas (lenguajes formales). Asimismo, en la literatura hispanoamericana, Jorge Luis Borges en su obra *El Aleph* (1949) conceptualiza el término como un punto en el espacio que concentra, sin superponerse, la totalidad del universo. En el diseño del lenguaje, buscamos que Aleph sintetice esta propiedad: dotar a la computadora de una sintaxis finita y rigurosa capaz de manipular estructuras jerárquicas y universos de estados formales.

## 1.3 Sobre las herramientas a utilizar en el desarrollo

Para la construcción del procesador del lenguaje Aleph emplearemos las herramientas de generación automática Flex y Bison [Levine, 2009, p. 1]:

* **Flex (Fast Lexical Analyzer Generator):** Herramienta que recibe como especificación de entrada un archivo de texto con pares formados por expresiones regulares y bloques de acciones en código C [Levine, 2009, p. 3]. Su salida es un programa en C que implementa un analizador léxico (scanner o autómata finito determinista subyacente). Su función es procesar el flujo continuo de caracteres del programa fuente en Aleph, agruparlos en unidades mínimas válidas (lexemas) y retornar secuencialmente su categoría sintáctica abstracta (tokens).
* **Bison (GNU Parser Generator):** Generador de analizadores sintácticos que toma como entrada una especificación formal basada en una Gramática Libre de Contexto descrita en notación BNF (Backus-Naur Form) [Levine, 2009, p. 53]. Como salida, genera un programa en C que implementa un analizador sintáctico algorítmico LALR(1) (parser). Su función consiste en verificar que la secuencia lineal de tokens satisfaga las reglas de derivación de la gramática y dirigir la construcción de una estructura jerárquica en memoria: el Árbol de Sintaxis Abstracta (AST).

**Comunicación e Interacción Operativa:**

Bison gobierna la interacción entre ambas herramientas. Durante el proceso de análisis, cada vez que la lógica del parser requiere consumir un símbolo terminal para realizar una operación de desplazamiento (shift) o reducción (reduce), invoca a la función generada por Flex, `yylex()`. Esta rutina reanuda el escaneo de caracteres, consume el lexema correspondiente y devuelve un valor entero representativo del token, comunicando opcionalmente sus atributos semánticos a través de la variable global de comunicación `yylval`.

Bison delimita estrictamente su tarea a la validación de la sintaxis estructural [Levine, 2009, p. 110]. Las comprobaciones de semántica estática y dinámica —como la verificación de tipos, la validación de alcance de identificadores en la Tabla de Símbolos y la consistencia de operaciones sobre colecciones— no son resueltas por Bison; implementaremos estas responsabilidades ad-hoc mediante algoritmos recursivos de visita sobre los nodos del AST.

## 1.4 Tipos de implementación

De acuerdo con la clasificación formal de lenguajes de programación, distinguimos tres métodos fundamentales de implementación [Sebesta, 2012, pp. 25-30]:

* **Compilación:** El programa fuente es traducido de forma global e integral por un compilador hacia un lenguaje destino equivalente, el cual puede consistir en lenguaje de máquina nativo del procesador, lenguaje ensamblador o código intermedio estructurado [Sebesta, 2012, p. 25]. Una vez completada la traducción y enlazadas las dependencias, ejecutamos el código destino de forma autónoma a nivel de hardware, lo que garantiza una máxima velocidad de ejecución.
* **Interpretación Pura:** No traducimos el programa fuente a un lenguaje destino ejecutable. En su lugar, un programa denominado intérprete actúa como una máquina virtual de software que decodifica y ejecuta secuencialmente las sentencias del lenguaje fuente en tiempo real [Sebesta, 2012, p. 28]. Este método nos facilita la depuración y portabilidad a nivel de código fuente, pero presenta una degradación severa en el rendimiento temporal debido a la decodificación repetitiva de sentencias en bucles y estructuras de control.
* **Sistemas de Implementación Híbrida:** En este modelo, traducimos el código fuente inicialmente a un lenguaje intermedio de bajo nivel y representación lineal (comúnmente denominado bytecode) [Sebesta, 2012, p. 29]. Posteriormente, dicho lenguaje intermedio es decodificado y ejecutado por un motor de interpretación o máquina virtual de tiempo de ejecución (como la JVM de Java). Este método nos evita decodificar la sintaxis de alto nivel más de una vez, logrando un compromiso entre portabilidad y velocidad.

### 1.4.1 Enfoque de implementación de Aleph

Con base en los fundamentos conceptuales de Sebesta (2012) y Levine (2009), clasificamos el sistema de ejecución de Aleph formalmente como un intérprete puro con front-end de análisis estático.

Aleph no constituye una implementación híbrida: no generamos ni emitimos un archivo o flujo lineal de código intermedio (bytecode) que deba ser decodificado por una máquina virtual basada en registros o en pila. La fase previa que construimos mediante Flex y Bison opera estrictamente como un módulo de análisis frontal (front-end léxico y sintáctico) cuya única función es transformar el texto plano en una estructura arbórea de datos en memoria dinámica: el Árbol de Sintaxis Abstracta (AST) [Levine, 2009, p. 115].

El AST no es un lenguaje destino autónomo, sino la representación interna en memoria de la estructura formal del programa fuente. Efectuamos la ejecución del programa a través del motor semántico del intérprete, el cual realiza recorridos recursivos en profundidad (árbol de visitas) sobre los nodos del AST [Sebesta, 2012, p. 28]. Cada nodo ejecuta directamente las operaciones lógicas, algebraicas y de control manipulando los estados de las variables en una Tabla de Símbolos activa en memoria.

# 2. Análisis de requerimientos

## 2.1 Un caso de estudio: Los Autómatas Finitos y sus algoritmos

Formalmente, definimos un Autómata Finito Determinista (AFD) como una 5-upla:

$$A = (Q, \Sigma, \delta, q_0, F)$$

Donde:
* $Q$: Conjunto finito, no vacío, de estados.
* $\Sigma$: Alfabeto finito, no vacío, de símbolos de entrada.
* $\delta$: Función total de transición, definida como $\delta: Q \times \Sigma \to Q$.
* $q_0$: Estado inicial, con $q_0 \in Q$.
* $F$: Conjunto de estados de aceptación o finales, con $F \subseteq Q$.

Por su parte, definimos un Autómata Finito No Determinista (AFND) formalmente como una 5-upla:

$$A = (Q, \Sigma, \delta, q_0, F)$$

Donde la diferencia estructural radica en su relación de transición:

$$\delta: Q \times \Sigma \to \mathcal{P}(Q)$$

En un AFND, la evaluación de un símbolo sobre un estado actual produce un subconjunto perteneciente al conjunto de partes de $Q$ (denotado $\mathcal{P}(Q)$ o $2^Q$).

Ejemplo de un AFD sencillo que acepta cadenas binarias que terminan en "01":
* $Q = \{q_0, q_1, q_2\}$
* $\Sigma = \{'0', '1'\}$
* $q_0 = q_0$
* $F = \{q_2\}$
* $\delta$:
  * $(q_0, '0') \to q_1$, $(q_0, '1') \to q_0$
  * $(q_1, '0') \to q_1$, $(q_1, '1') \to q_2$
  * $(q_2, '0') \to q_1$, $(q_2, '1') \to q_0$

Tabla de Transiciones:

| Estado Actual | Entrada '0' | Entrada '1' |
| :--- | :--- | :--- |
| **$\to$ $q_0$** | $q_1$ | $q_0$ |
| **$q_1$** | $q_1$ | $q_2$ |
| **$* q_2$** | $q_1$ | $q_0$ |

### 2.1.1 Aceptación de cadenas

Para evaluar una cadena $w \\in \\Sigma^{\\ast}$ en un AFD, generalizamos formalmente la función de transición a una función de transición extendida $\\hat{\\delta}: Q \\times \\Sigma^{\\ast} \\to Q$, definida inductivamente por:
* $\hat{\delta}(q, \varepsilon) = q$, donde $\varepsilon$ es la cadena vacía.
* $\hat{\delta}(q, aw) = \hat{\delta}(\delta(q, a), w)$, para todo símbolo $a \in \Sigma$ y cadena $w \in \Sigma^*$.

Una cadena $w$ es aceptada por un AFD si y solo si $\hat{\delta}(q_0, w) \in F$. Para un AFND, una cadena $w$ es aceptada si el conjunto de estados alcanzados contiene al menos un estado final: $\hat{\delta}(q_0, w) \cap F \neq \emptyset$.

A continuación transcribimos el algoritmo de reconocimiento implementado en C:

```c
// Algoritmo de Aceptación de Cadenas:
while (aux_cadena != NULL) {
    char c = aux_cadena->data; 
    
    str temp_sym = create_nodo(c);
    int sym_idx = simbolo_a_indice(aut, temp_sym);
    free_str(temp_sym); 
    
    if (sym_idx == -1) {
        printf("\nSímbolo '%c' no pertenece a Sigma. Cadena rechazada.\n", c);
        free_ast(estados_actuales);
        return 0; 
    }
    
    Tdata proximos_estados = create_set();
    Tdata iterador = obtener_data(estados_actuales);
    
    while (iterador != NULL) {
        Tdata estado_nodo = obtener_data(iterador);
        
        if (estado_nodo != NULL && return_type(estado_nodo) == STR) {
            str nombre_estado = obtener_string(estado_nodo);
            int q_actual = estado_a_indice(aut, nombre_estado);
            
            if (q_actual != -1) {
                Tdata destino = transicion_por_indice(aut, q_actual, sym_idx);
                
                if (destino != NULL && obtener_data(destino) != NULL) {
                    Tdata it_dest = obtener_data(destino);
                    while (it_dest != NULL) {
                        if (obtener_data(it_dest) != NULL) {
                            insert_set(&proximos_estados, obtener_data(it_dest));
                        }
                        it_dest = obtener_next(it_dest);
                    }
                }
            }
        }
        iterador = obtener_next(iterador);
    }
    
    free_ast(estados_actuales);
    estados_actuales = proximos_estados;
    
    if (esvacio(estados_actuales) == 1) {
        free_ast(estados_actuales);
        return 0;
    }
    
    aux_cadena = aux_cadena->next;
}
```

##### 2.1.2 Algoritmo de conversión de AFND a AFD (Construcción de Subconjuntos)

**Teorema:** Para todo lenguaje $L$ aceptado por un AFND $A=(Q_A, \Sigma, \delta_A, q_{0A}, F_A)$, existe un AFD equivalente $B=(Q_B, \Sigma, \delta_B, q_{0B}, F_B)$ tal que $L(B)=L(A)$.

La construcción algorítmica establece:
* $Q_B \subseteq \mathcal{P}(Q_A)$: Cada estado del AFD corresponde a un subconjunto de estados del AFND.
* $\delta_B(R,a) = \bigcup_{q \in R} \delta_A(q,a)$, para cada $R \in Q_B$ y $a \in \Sigma$.
* $q_{0B} = \{q_{0A}\}$.
* $F_B = \{R \in Q_B \mid R \cap F_A \neq \emptyset\}$.

A continuación transcribimos la implementación en C del algoritmo:

```c
// Algoritmo de Conversion: 
int procesando = 1;
while (procesando) {
    procesando = 0;
    int act = -1;
    // Buscar el primer estado sin procesar
    for (int i = 0; i < num_estados; i++) {
        if (estados[i].procesado == 0) {
            act = i;
            procesando = 1;
            break;
        }
    }
    if (!procesando) break;
    
    estados[act].procesado = 1;
    
    // Evaluar transiciones con cada símbolo del alfabeto
    Tdata it_sym = obtener_data(afnd->Sigma);
    while (it_sym != NULL) {
        Tdata nodo_sym = obtener_data(it_sym);
        str sym_str = obtener_string(nodo_sym);
        
        // Obtener la unión de todos los destinos para este símbolo
        Tdata destinos = obtener_destinos_dfa(afnd, estados[act].conjunto_nfa, sym_str);
        
        if (esvacio(destinos) == 0) {
            // Verificar si ya descubrimos este conjunto antes
            int idx_existente = -1;
            for (int k = 0; k < num_estados; k++) {
                if (conjuntos_iguales(estados[k].conjunto_nfa, destinos)) {
                    idx_existente = k;
                    break;
                }
            }
            
            int idx_dest;
            if (idx_existente == -1) {
                // Descubrimos un nuevo estado
                estados[num_estados].conjunto_nfa = destinos;
                if (num_estados < 26) {
                    sprintf(estados[num_estados].nombre, "%c", 'A' + num_estados);
                } else {
                    sprintf(estados[num_estados].nombre, "S%d", num_estados);
                }
                estados[num_estados].procesado = 0;
                idx_dest = num_estados;
                num_estados++;
            } else {
                idx_dest = idx_existente;
                free_ast(destinos); // Ya existía, liberamos la memoria
            }
            
            // Guardar la transición para construir la matriz luego
            strcpy(trans[num_trans].origen, estados[act].nombre);
            strcpy(trans[num_trans].destino, estados[idx_dest].nombre);
            extraer_cadena(sym_str, trans[num_trans].simbolo);
            num_trans++;
        } else {
            free_ast(destinos); // Transición vacía (sumidero implícito)
        }
        
        it_sym = obtener_next(it_sym);
    }
}
```

##### Modelado computacional de AF's y requerimientos de abstracción en Aleph

En el diseño de Aleph, modelamos la representación de los Autómatas Finitos prescindiendo de tipos abstractos cerrados en C, utilizando las abstracciones universales del lenguaje:
* Modelamos los conjuntos de estados $Q$, el alfabeto $\Sigma$ y los estados finales $F$ de forma directa mediante la estructura matemática `set`.
* Modelamos computacionalmente la función o relación de transición $\delta$ mediante listas anidadas de tuplas o relaciones: `list Delta = [[q_origen, simbolo, destinos], ...]`.
* Representamos la 5-upla completa agregando estas colecciones en una lista de orden posicional: `list Automata = [Q, Sigma, Delta, q0, F]`.

**Análisis de los algoritmos en C y abstracción requerida:**
Nuestro análisis de las implementaciones precedentes revela que, aunque utilicemos interfaces de funciones como `obtener_data`, `obtener_next`, `insert_set` y `conjuntos_iguales`, el código en C exhibe fallas de encapsulamiento debido al acoplamiento con la estructura de almacenamiento. El uso explícito de punteros directos (`aux_cadena->next`, `aux_cadena->data`) y el avance manual nodo a nodo evidencian que, como programadores, estamos forzados a operar al nivel de enlaces de memoria física.

Este análisis nos fundamenta un requerimiento arquitectónico crítico para el diseño de Aleph: nuestro lenguaje debe proveer abstracciones de control de alto nivel (como el bucle declarativo `for elemento in coleccion`) y operadores de manipulación de colecciones (como `union`, `in`, `insert`) [Louden, 2004, pp. 261-263]. Estas construcciones nos permiten aislarnos por completo del modelo de memoria y del encadenamiento de punteros subyacente, garantizando que los algoritmos matemáticos sobre autómatas se expresen en función de su lógica conjuntista y no de la gestión de memoria dinámica.

#### 2.2 Conceptos sobre lenguajes de programación

Desde la perspectiva teórica de Louden, definimos un lenguaje de programación formalmente como un sistema notacional diseñado para describir computaciones de manera legible tanto para el ser humano como para la máquina [Louden, 2004, p. 1].

Esta conceptualización nos exige un equilibrio en dos frentes complementarios:
* **Legibilidad para el ser humano:** Nuestro lenguaje debe proveer abstracciones de alto nivel orientadas a la naturaleza lógica del problema, reduciendo la carga cognitiva impuesta por los detalles de bajo nivel del hardware (direcciones de memoria, registros y saltos de control) [Louden, 2004, p. 7]. Debe permitirnos la expresión clara, concisa y mantenible de los algoritmos.
* **Procesabilidad por la máquina:** El sistema notacional debe poseer una sintaxis formal, inequívoca y libre de ambigüedad matemática, susceptible de ser analizada por algoritmos deterministas en tiempo y espacio finitos, permitiendo a la computadora asociar un significado semántico unívoco a cada construcción de código [Louden, 2004, p. 8].

Estructuramos Aleph sobre esta definición: traducimos las operaciones conjuntistas y de teoría de lenguajes formales a una notación ejecutable y determinista para el intérprete, preservando la familiaridad de la notación matemática para nosotros como analistas.

#### 2.3 Abstracciones

En la teoría de diseño de lenguajes de programación, consideramos una abstracción como un mecanismo conceptual que nos permite aislar las propiedades esenciales de un objeto o proceso computacional, ocultando los detalles concretos de implementación técnica y gestión de hardware [Louden, 2004, p. 11]. Logramos el control de la complejidad organizando las abstracciones en dos categorías principales, estratificadas en tres niveles progresivos:
* **Abstracción de datos:** Nos permite estructurar y aislar la representación física de los valores computables en memoria [Louden, 2004, pp. 257-260]:
  * **Básica:** Oculta la codificación interna de bits en la memoria de la máquina. Engloba el concepto de variable y los tipos primitivos nativos con sus operaciones escalares asociadas.
  * **Estructurada:** Modela colecciones de valores agregados y las relaciones que existen entre ellos. Oculta la disposición física de la memoria contigua o dispersa (ej. registros, conjuntos, listas, secuencias indexadas).
  * **Unitaria:** Encapsula la definición de un conjunto de datos junto con el repertorio completo de operaciones primitivas que los gobiernan, ocultando la estructura interna bajo una interfaz de acceso (Tipos Abstractos de Datos, módulos, clases).
* **Abstracción de control:** Modela y organiza el flujo secuencial y alternativo de la ejecución del cómputo [Louden, 2004, pp. 11-13]:
  * **Básica:** Sentencias elementales que encapsulan secuencias de microinstrucciones de máquina en una única orden lógica declarativa (ej. la sentencia de asignación o expresiones de cómputo).
  * **Estructurada:** Construcciones lingüísticas que alteran el flujo del contador del programa de manera gobernada mediante condiciones lógicas (guardias), dividiendo el flujo en bloques anidados con puntos definidos de entrada y salida (bifurcaciones if-else e iteraciones while, for).
  * **Unitaria:** Mecanismos que nos permiten tratar un bloque complejo de control de sentencias como una unidad aislada, invocable y parametrizable desde diversos puntos del sistema (procedimientos, métodos, funciones).

##### 2.3.1 Abstracciones de Aleph

Desde la perspectiva funcional del programador, Aleph provee los siguientes niveles de abstracción:

| Nivel | Abstracciones de datos | Abstracciones de control |
| :--- | :--- | :--- |
| **Básico** | `Elemento` | Asignación (`=`) |
| **Estructurado** | `Lista` (colección ordenada) y `Conjunto` (colección no ordenada).<br><br>**Operaciones definidas:** pertenencia (`in`), unión (`union`), inserción (`insert`) y concatenación (`append`). <br><br>*(Nota: `empty` no constituye una operación, sino que actúa como un literal o constante representativa de una colección vacía).* | Selección (`if/else`), iteración (`while`), y recorrido de colecciones (`for..in` / `foreach`). |
| **Unitario** | Aleph no proporciona actualmente mecanismos para que el programador defina nuevas abstracciones unitarias de datos. | Módulos/subprogramas: funciones y procedimientos (`list nombre(params): ... return ... end`). |

**Características y operaciones de las abstracciones:**
A partir de esta clasificación, Aleph estructura su capacidad de cómputo proporcionando el `Elemento` como la unidad fundamental y atómica de información. Para agrupar estos elementos, el lenguaje provee dos abstracciones estructuradas nativas que admiten anidamiento arbitrario: la `Lista` y el `Conjunto`. Sobre estas colecciones, Aleph expone un repertorio de operaciones algebraicas integradas (como la unión y la inserción) que le permiten al usuario manipular las estructuras sin conocer su representación física en la memoria de la máquina. En cuanto al control, la asignación opera como la instrucción elemental que altera el estado, mientras que las sentencias estructuradas y los subprogramas modulan la secuencia lógica en la que estas manipulaciones toman lugar.

**Nota taxonómica sobre operadores matemáticos:**
Cabe destacar rigurosamente que operaciones como `union` (unión de conjuntos) o `in` (pertenencia) no constituyen abstracciones de control. Su naturaleza radica en operar sobre los valores de las abstracciones de datos para producir un nuevo resultado lógico o agregado. Las abstracciones de control, por el contrario, son aquellas sentencias (como las estructuras de selección o iteración) que determinan la organización formal y la secuencia de ejecución del programa.

#### 2.4 Dominio de programación de Aleph

Con base en la taxonomía de dominios de aplicación de lenguajes establecida por Sebesta [2012, pp. 6-8], localizamos a Aleph en el dominio de las **Aplicaciones Científicas**.

Esta decisión se fundamenta en que el propósito específico para el cual diseñamos Aleph es proporcionar abstracciones y operaciones que permitan representar y manipular computacionalmente estructuras matemáticas discretas. El lenguaje estructura su capacidad de cómputo proporcionando operaciones algebraicas integradas sobre elementos, conjuntos y listas. La resolución de problemas que puedan modelarse mediante estas abstracciones y la aplicación de operaciones conjuntistas son características propias de los problemas matemáticos y computacionales abordados en las disciplinas científicas.

Cabe destacar que, dentro de este dominio de aplicaciones científicas, hemos establecido un **caso de aplicación** específico para evaluar el lenguaje: el modelado y procesamiento de **Autómatas Finitos Deterministas y No Deterministas**. Los autómatas y lenguajes formales no constituyen abstracciones incorporadas al núcleo del lenguaje, sino que actúan como nuestro caso de estudio. Nuestro lenguaje provee las abstracciones de datos (conjuntos, listas) y las estructuras de control adecuadas (iteraciones, selecciones) para que el programador pueda expresar formalmente la manipulación de estos autómatas, demostrando así la capacidad científica del lenguaje.

En conclusión, el dominio de Aleph no se define por el cálculo numérico tradicional o el procesamiento simbólico de la Inteligencia Artificial, sino que justifica su pertenencia a las Aplicaciones Científicas por su enfoque directo en la representación algorítmica de la matemática discreta y la teoría de conjuntos.

### 3. Diseño e Implementación

#### 3.1 Sentencias

En el diseño conceptual de lenguajes imperativos, definimos una sentencia (*statement*) como la unidad sintáctica ejecutable mínima que especifica una acción o paso discreto en el proceso de cómputo de la máquina abstracta [Sebesta, 2012, p. 113]. A diferencia de las expresiones —construcciones que evaluamos formalmente para retornar un valor de cómputo—, ejecutamos las sentencias con el objetivo fundamental de generar efectos de estado en el entorno (como modificar la Tabla de Símbolos) o alterar el flujo secuencial del control.

Regimos la organización del flujo de ejecución en Aleph por las tres estructuras fundamentales de la programación estructurada [Sebesta, 2012, pp. 331-335]:
* **Composición (Secuenciación Implícita):** Secuencia lineal en la cual ejecutamos las instrucciones estrictamente una tras otra en el orden textual del código fuente, garantizando invariantes de estado secuenciales.
* **Alternancia (Selección):** Construcción que evalúa una condición lógica o guardia booleana para seleccionar qué bloque de ejecución debe activarse y cuál omitirse (`if-else`).
* **Iteración (Repetición):** Construcción que nos posibilita repetir un cuerpo de sentencias de manera controlada bajo la vigencia de una condición de parada (`while`, `for`), asegurando que cada bloque de control posea un punto único de acceso y de culminación.

##### 3.1.1 Un primer diseño de sentencias de Aleph

A continuación presentamos nuestro primer diseño algorítmico estructurado para la conversión de un AFND a AFD expresado en el lenguaje Aleph, el cual operamos exclusivamente mediante las abstracciones generales del lenguaje:

```aleph
list afnd2afd(list A):
    list A = [{q0, q1}, {'0', '1'}, Delta, q0, {q1}];
    let A = (Q, Sigma, Delta, Q0, F);
    set Q_B = {{q0}};
    boolean Estadefinido = false;
    set R = {q0};
    list DeltaB = empty;

    while (Estadefinido == false) do
        for (a in Sigma) do
            list DeltaAux = [R, a];
            if (DeltaAux in Delta) do
                Estadefinido = true;
            else
                set destinos = empty;
                for (q in R) do
                    destinos = destinos union DeltaA(q, a);
                end
                Q_B = Q_B union {destinos};
                list TransicionB = [R, a, destinos];
                append(DeltaB, TransicionB);
            end
            R = destinos;
        end
    end

    set F_B = empty;
    for (s in Q_B) do
        for (q in s) do
            if (q in F_A) do
                insert(F_B, s);
            end
        end
    end

    list B = [Q_B, Sigma, DeltaB, {q0}, F_B];
    return B;
end
```

#### 3.2 Reconocimiento de patrones. Tokens, Lexemas y Patrones

En la fase inicial de análisis léxico, el procesamiento de las unidades mínimas del lenguaje nos exige distinguir tres conceptos teóricos complementarios [Sebesta, 2012, pp. 114-115]:
* **Lexema:** Secuencia textual concreta de caracteres en el código fuente que coincide formalmente con el patrón asignado a una categoría léxica. Constituye la instancia física leída de la entrada (por ejemplo, `while`, `Q_B`, `==`, `17`).
* **Token:** Símbolo o categoría sintáctica abstracta que identifica el tipo funcional al que pertenece el lexema. Es la unidad que consume el analizador sintáctico (por ejemplo, `IDENTIFICADOR`, `OPERADOR_ASIGNACION`, `PALABRA_CLAVE_WHILE`). Desde el punto de vista sintáctico, un token actúa como un símbolo terminal dentro de la gramática formal.
* **Patrón:** Regla de especificación formal que determina el universo de cadenas de caracteres que pertenecen a un token específico [Sebesta, 2012, p. 115]. Formalizamos los patrones mediante expresiones regulares que el generador Flex compila a autómatas finitos deterministas capaces de resolver la correspondencia léxica en tiempo lineal:

$$Identificador = [A-Z][a-zA-Z0-9\_]*$$

El analizador léxico procesa el texto plano aplicando las expresiones regulares (patrones); en cuanto identifica un lexema que satisface la regla, genera y entrega el token correspondiente a la rutina de análisis sintáctico.

##### 3.2.1 Tokens y lexemas de Aleph

| Lexema               | Token             | Token Act         | Descripción                                                                              |
| -------------------- | ----------------- | ----------------- | ---------------------------------------------------------------------------------------- |
| `set`                | `KW_SET`          | `KW_SET`          | Tipo de dato / Palabra reservada: conjunto no duplicado.                                 |
| `list`               | `KW_LIST`         | `KW_LIST`         | Tipo de dato / Palabra reservada: secuencia o lista ordenada.                            |
| `while`              | `KW_WHILE`        | `KW_WHILE`        | Sentencia de control: inicio de iteración condicionada.                                  |
| `for`                | `KW_FOR`          | `KW_FOR`          | Sentencia de control: iteración sobre elementos de colección.                            |
| `if`                 | `KW_IF`           | `KW_IF`           | Sentencia de control: bifurcación condicional de flujo.                                  |
| `else`               | `KW_ELSE`         | `KW_ELSE`         | Sentencia de control: rama alternativa de selección.                                     |
| `do`                 | `DELIM_DO`        | `DELIM_DO`        | Delimitador de bloque: apertura de cuerpo de control o bucle.                            |
| `end`                | `DELIM_END`       | `DELIM_END`       | Delimitador de bloque: cierre estructural de bloques o funciones.                        |
| `break`              | `KW_BREAK`        | `KW_BREAK`        | Sentencia de control: interrupción forzada de iteración.                                 |
| `return`             | `KW_RETURN`       | `KW_RETURN`       | Sentencia de control: retorno explícito de valor de función.                             |
| `let`                | `KW_LET`          | `KW_LET`          | Declaración: desestructuración o nombramiento formal de componentes.                     |
| `in`                 | `OP_IN`           | `OP_IN`           | Operador relacional: evaluación de pertenencia conjuntista o recorrido.                  |
| `where`              | `KW_WHERE`        | `KW_WHERE`        | Sentencia de control: cláusula de filtrado condicional en bucles.                        |
| `empty`              | `KW_EMPTY`        | `KW_EMPTY`        | Literal / Constante: representa el conjunto o lista sin elementos.                       |
| `union`              | `OP_UNION`        | `OP_UNION`        | Operador de colección: cálculo de la unión algebraica entre conjuntos.                   |
| `intersection`       | `OP_INTERSECTION` | `OP_INTERSECTION` | Operador de colección: cálculo de la intersección entre conjuntos.                       |
| `contain`            | `OP_CONTAIN`      | `OP_CONTAIN`      | Operador de colección: verificación de pertenencia de un elemento en un conjunto o lista.|
| `difference`         | `OP_DIFFERENCE`   | `OP_DIFFERENCE`   | Operador de colección: cálculo de la diferencia algebraica entre conjuntos.              |
| `any`                | `KW_ANY`          | `KW_ANY`          | Función primitiva: verificación existencial en colecciones.                              |
| `append`             | `KW_APPEND`       | `KW_APPEND`       | Función primitiva: inserción de elemento al final de una lista.                          |
| `insert`             | `KW_INSERT`       | `KW_INSERT`       | Función primitiva: incorporación de un elemento a un conjunto.                           |
| `&&`                 | `KW_AND`          | `KW_AND`          | Operador lógico: conjunción (Y lógico).                                                  |
| `!`                  | `KW_NOT`          | `!`               | Operador lógico: negación unaria.                                                        |
| `\|\|`               | `KW_OR`           | `KW_OR`           | Operador lógico: disyunción (O lógico).                                                  |
| `:`                  | `DELIM_COLON`     | `':'`             | Símbolo especial: delimitador de apertura del bloque de función.                         |
| `=`                  | `OP_ASSIGN`       | `'='`             | Operador básico: asignación destructiva de valor a identificador.                        |
| `==`                 | `OP_EQUAL`        | `OP_EQUAL`        | Operador relacional: prueba de igualdad estructural estricta.                            |
| `!=`                 | `OP_NOT_EQUAL`    | `OP_NOT_EQUAL`    | Operador relacional: prueba de desigualdad estructural estricta.                         |
| `;`                  | `DELIM_SEMICOLON` | `';'`             | Delimitador sintáctico: terminación explícita de sentencia.                              |
| `,`                  | `DELIM_COMMA`     | `','`             | Separador sintáctico: división posicional en listas, conjuntos o parámetros.             |
| `(`                  | `DELIM_LPAREN`    | `'('`             | Delimitador sintáctico: apertura de tuplas, condiciones o llamadas.                      |
| `)`                  | `DELIM_RPAREN`    | `')'`             | Delimitador sintáctico: cierre de tuplas, condiciones o llamadas.                        |
| `{`                  | `DELIM_LBRACE`    | `'{'`             | Delimitador sintáctico: apertura de especificación literal de conjunto.                  |
| `}`                  | `DELIM_RBRACE`    | `'}'`             | Delimitador sintáctico: cierre de especificación literal de conjunto.                    |
| `[`                  | `DELIM_LBRACKET`  | `'['`             | Delimitador sintáctico: apertura de especificación literal de lista.                     |
| `]`                  | `DELIM_RBRACKET`  | `']'`             | Delimitador sintáctico: cierre de especificación literal de lista.                       |
| `'0'`,`'1'`,`'a'`    | `LIT_ELEMENT`     | `LIT_ELEMENT`     | Literales: representación atómica de un elemento base para conformar colecciones.        |
| `[A-Z][a-zA-Z0-9_]*` | `ID`              | `ID`              | Identificadores: nombres de variables definidos por el usuario iniciados en mayúscula.   |

### Referencias Bibliográficas
* Levine, J. (2009). *flex & bison* (1ra ed.). O'Reilly Media.
* Louden, K. C. (2004). *Lenguajes de Programación: Principios y Práctica* (2da ed.). Thomson.
* Sebesta, R. W. (2012). *Concepts of Programming Languages* (10ma ed.). Addison-Wesley.

## 3.3 Análisis Léxico

Un analizador léxico realiza, técnicamente, un análisis sintáctico al nivel más bajo de la estructura del programa. Su función principal consiste en reconocer agrupaciones lógicas de caracteres (al identificar subcadenas) mediante la coincidencia de patrones sobre el código de entrada. Dichas agrupaciones se denominan lexemas, mientras que las categorías sintácticas o códigos internos asignados a ellas corresponden a los tokens.


### Descripción de las tareas de un analizador léxico

Las tareas fundamentales de un analizador léxico (scanner) incluyen:

* Identificar y agrupar los caracteres en lexemas válidos basándose en patrones regulares.

* Generar y entregar secuencialmente los tokens al analizador sintáctico (parser) cada vez que este los requiere para la construcción del árbol sintáctico.

* Filtrar y descartar elementos que carecen de significado estructural para el compilador o intérprete, tales como espacios en blanco, tabulaciones, saltos de línea y comentarios.

* Llevar el registro de la posición actual en el código fuente (número de línea y columna) para facilitar la emisión de mensajes de error precisos durante el análisis.

## 3.3.1 Primer analizador léxico de Aleph
### Scanner que imprime por pantalla los tokens

```c
%{
#include <stdio.h>
%}

%option noyywrap

%%
  /* PALABRAS RESERVADAS Y FUNCIONES PRIMITIVAS */
"set"          { printf("KW_SET: %s\n", yytext); }
"list"         { printf("KW_LIST: %s\n", yytext); }
"while"        { printf("KW_WHILE: %s\n", yytext); }
"for"          { printf("KW_FOR: %s\n", yytext); }
"if"           { printf("KW_IF: %s\n", yytext); }
"else"         { printf("KW_ELSE: %s\n", yytext); }
"return"       { printf("KW_RETURN: %s\n", yytext); }
"let"          { printf("KW_LET: %s\n", yytext); }
"where"        { printf("KW_WHERE: %s\n", yytext); }
"empty"        { printf("KW_EMPTY: %s\n", yytext); }
"any"          { printf("KW_ANY: %s\n", yytext); }
"append"       { printf("KW_APPEND: %s\n", yytext); }
"insert"       { printf("KW_INSERT: %s\n", yytext); }

/* OPERADORES DE COLECCIÓN, RELACIONALES Y LÓGICOS */
"in"       { printf("OP_IN: %s\n", yytext); }
"union"    { printf("OP_UNION: %s\n", yytext); }
"inter"    { printf("OP_INTERSECTION: %s\n", yytext); }
"diff"     { printf("OP_DIFFERENCE: %s\n", yytext); }
/*"has"      { printf("OP_HAS: %s\n", yytext); } */

"and"      { printf("KW_AND: %s\n", yytext); }
"or"       { printf("KW_OR: %s\n", yytext); }
"not"      { printf("KW_NOT: %s\n", yytext); }

"=="       { printf("OP_EQUAL: %s\n", yytext); }
"!="       { printf("OP_NOT_EQUAL: %s\n", yytext); }
"="        { printf("OP_ASSIGN: %s\n", yytext); }

  /* DELIMITADORES Y SÍMBOLOS ESPECIALES */
"do"           { printf("DELIM_DO: %s\n", yytext); }
"end"          { printf("DELIM_END: %s\n", yytext); }
":"            { printf("DELIM_COLON: %s\n", yytext); }
";"            { printf("DELIM_SEMICOLON: %s\n", yytext); }
","            { printf("DELIM_COMMA: %s\n", yytext); }
"("            { printf("DELIM_LPAREN: %s\n", yytext); }
")"            { printf("DELIM_RPAREN: %s\n", yytext); }
"{"            { printf("DELIM_LBRACE: %s\n", yytext); }
"}"            { printf("DELIM_RBRACE: %s\n", yytext); }
"["            { printf("DELIM_LBRACKET: %s\n", yytext); }
"]"            { printf("DELIM_RBRACKET: %s\n", yytext); }

  /* LITERALES E IDENTIFICADORES */
\'[^\']\'      { printf("LIT_ELEMENT: %s\n", yytext); }
[A-Z][a-zA-Z0-9_]* { printf("ID: %s\n", yytext); }

  /* ESPACIOS Y ERRORES */
[ \t\n\r]+     { /* ignorar espacios en blanco */ }
.              { printf("Error lexico: Caracter no reconocido %s\n", yytext); }
%%

int main(int argc, char **argv) {
    printf("Iniciando escaner de Aleph. Escriba codigo y presione Ctrl+Z (Windows) o Ctrl+D (Linux) para salir.\n");
    yylex();
    return 0;
}
```
### Scanner que usa enumerados

```c
%{
#include <stdio.h>

enum yytokentype {
    KW_SET = 258,
    KW_LIST = 259,
    KW_WHILE = 260,
    KW_FOR = 261,
    KW_IF = 262,
    KW_ELSE = 263,
    DELIM_DO = 264,
    DELIM_END = 265,
    KW_RETURN = 267,
    KW_LET = 268,
    KW_WHERE = 269,
    KW_EMPTY = 270,
    KW_ANY = 271,
    KW_APPEND = 272,
    KW_INSERT = 273,
    OP_IN = 274,
    OP_UNION = 275,
    OP_INTERSECTION = 276,
    */ OP_HAS = 277, */
    OP_DIFFERENCE = 279,
    KW_AND = 280,
    KW_NOT = 281,
    KW_OR = 282,
    OP_EQUAL = 283,
    OP_NOT_EQUAL = 284,
    OP_ASSIGN = 285,
    DELIM_COLON = 286,
    DELIM_SEMICOLON = 287,
    DELIM_COMMA = 288,
    DELIM_LPAREN = 289,
    DELIM_RPAREN = 290,
    DELIM_LBRACE = 291,
    DELIM_RBRACE = 292,
    DELIM_LBRACKET = 293,
    DELIM_RBRACKET = 294,
    LIT_ELEMENT = 295,
    ID = 296
};
%}

%option noyywrap

%%
  /* PALABRAS RESERVADAS Y FUNCIONES PRIMITIVAS */
"set"          { return KW_SET; }
"list"         { return KW_LIST; }
"while"        { return KW_WHILE; }
"for"          { return KW_FOR; }
"if"           { return KW_IF; }
"else"         { return KW_ELSE; }
"return"       { return KW_RETURN; }
"let"          { return KW_LET; }
"where"        { return KW_WHERE; }
"empty"        { return KW_EMPTY; }
"any"          { return KW_ANY; }
"append"       { return KW_APPEND; }
"insert"       { return KW_INSERT; }
/* OPERADORES DE COLECCIÓN, RELACIONALES Y LÓGICOS */
"in"       { return OP_IN; }
"union"    { return OP_UNION; }
"inter"    { return OP_INTERSECTION; }
"diff"     { return OP_DIFFERENCE; }
*/ "has"      { return OP_HAS; } */

"and"      { return KW_AND; }
"or"       { return KW_OR; }
"not"      { return KW_NOT; }

"=="       { return OP_EQUAL; }
"!="       { return OP_NOT_EQUAL; }
"="        { return OP_ASSIGN; }
  /* DELIMITADORES Y SÍMBOLOS ESPECIALES */
"do"           { return DELIM_DO; }
"end"          { return DELIM_END; }
":"            { return DELIM_COLON; }
";"            { return DELIM_SEMICOLON; }
","            { return DELIM_COMMA; }
"("            { return DELIM_LPAREN; }
")"            { return DELIM_RPAREN; }
"{"            { return DELIM_LBRACE; }
"}"            { return DELIM_RBRACE; }
"["            { return DELIM_LBRACKET; }
"]"            { return DELIM_RBRACKET; }

  /* LITERALES E IDENTIFICADORES */
\'[^\']\'      { return LIT_ELEMENT; }
[A-Z][a-zA-Z0-9_]* { return ID; }

  /* ESPACIOS Y ERRORES */
[ \t\n\r]+     { /* ignorar espacios en blanco */ }
.              { printf("Error lexico: Caracter no reconocido %s\n", yytext); }
%%

int main(int argc, char **argv) {
    int token;
    
    if (argc > 1) {
        FILE *archivo = fopen(argv[1], "r");
        if (!archivo) {
            printf("Error: No se pudo abrir el archivo %s\n", argv[1]);
            return 1;
        }
        yyin = archivo;
    } else {
        printf("\nIniciando analizador lexico. Ingrese codigo y presione Ctrl+Z (Windows) o Ctrl+D (Linux) para finalizar.\n");
    }

    printf("\n=====================================\n");
    printf(" %-15s | %s\n", "ID NUMERICO", "LEXEMA RECONOCIDO");
    printf("=====================================\n");

    while ((token = yylex()) != 0) {
        printf(" %-15d | '%s'\n", token, yytext);
    }
    
    printf("=====================================\n\n");
    return 0;
}
```
***Prefijos***:
KW (Keyword): identifica palabras reservadas que tienen un significado especial en el lenguaje.
DELIM (Delimiter): identifica signos que delimitan, separan o estructuran las construcciones sintácticas.
OP (Operator): identifica operadores que realizan comparaciones, asignaciones u otras operaciones.
LIT (Literal): identifica valores constantes escritos directamente en el código fuente.

## 3.4. Descripción Formal de la Sintaxis
Los lenguajes de programación requieren una interpretación de sus sentencias sin ambigüedades, su descripción, a los fines de comunicar su funcionamiento, tanto a los usuarios de los mismos como a quienes realizan su implementación, requiere herramientas formales.

## SINTAXIS DE LOS LP

La sintaxis de un lenguaje de programación establece el conjunto de reglas formales que determinan cómo deben combinarse los símbolos del alfabeto del lenguaje (tokens) para construir sentencias, expresiones y estructuras válidas. Se enfoca estrictamente en la forma y organización jerárquica del código, siendo independiente del significado computacional o los efectos de ejecución (semántica).

## LAS GLC Y LA DESCRIPCIÓN DE SINTAXIS

Para modelar matemáticamente y especificar la sintaxis de la mayoría de los lenguajes de programación se utilizan las Gramáticas Libres de Contexto (GLC), introducidas por Noam Chomsky. Una GLC consta de cuatro elementos: un conjunto finito de símbolos terminales (los tokens indivisibles), un conjunto de símbolos no terminales (variables sintácticas que representan estructuras agrupadas, como una "sentencia"), un símbolo inicial desde el cual se deriva todo programa válido, y un conjunto de reglas de producción. Estas reglas indican cómo un no terminal puede reescribirse como una secuencia de terminales y no terminales. Esta capacidad recursiva permite a las GLC describir con precisión el anidamiento de estructuras de control (como bucles dentro de condicionales) que caracteriza a los lenguajes imperativos estructurados.

## DEFINICIÓN DE BNF Y EBNF

La Forma de Backus-Naur (BNF) es la notación metalingüística estándar empleada para escribir las reglas de producción de una GLC de manera legible y estandarizada. En BNF, los símbolos no terminales se encierran entre corchetes angulares (ej. `<bucle>`), la derivación se expresa mediante el símbolo `::=` o `->`, y las opciones alternativas se separan con una barra vertical `|`.

La EBNF (Forma de Backus-Naur Extendida) optimiza esta notación incorporando metacaracteres que evitan la sobrecarga de reglas recursivas simples: utiliza corchetes `[...]` para marcar elementos opcionales, llaves `{...}` para representar la repetición de un elemento (cero o más veces), y paréntesis `(...)` para la agrupación de símbolos lógicos.

## 3.3.1. Primera descripción formal de Aleph

A continuación, presentamos un subconjunto general en BNF que abarca las estructuras de control principales y la definición de funciones diseñadas en nuestro primer prototipo algorítmico de Aleph:

```c

<Aleph>            ::= <lts_sent>

<lts_sent>         ::= <sent> ';'
                     | <lts_sent> <sent> ';'
  
<bloque>           ::= DELIM_DO <lts_sent> DELIM_END
                     | DELIM_DO DELIM_END

<sent>             ::= <asignacion>
                     | <expresion> 
                     | <sent_while> 
                     | <sent_if> 
                     | <sent_for>
                     | KW_RETURN <expresion>
                     | KW_BREAK

<asignacion>       ::= ID '=' <expresion>
                     | KW_LET <lts_id> '=' <lts_expresiones>

<lts_id>           ::= ID 
                     | <lts_id> ',' ID

<sent_while>       ::= KW_WHILE '(' <op_log> ')' <bloque>

<sent_for>         ::= KW_FOR '(' ID OP_IN <expresion> ')' <bloque>

<sent_if>          ::= KW_IF '(' <op_log> ')' <bloque>
                     | KW_IF '(' <op_log> ')' <bloque> KW_ELSE <bloque>

<lts_expresiones>  ::= <expresion> 
                     | <lts_expresiones> ',' <expresion>

<lts_lista>        ::= '[' <lts_expresiones> ']'
                     | '[' ']'

<lts_conjunto>     ::= '{' <lts_expresiones> '}'
                     | '{' '}'

<expresion>        ::= <expresion> OP_UNION <termino>
                     | <expresion> OP_INTERSECTION <termino>
                     | <expresion> OP_DIFFERENCE <termino>
                     | <expresion> KW_APPEND <termino>
                     | <expresion> KW_INSERT <termino>
                     | <termino>

<termino>          ::= ID
                     | LIT_ELEMENT
                     | KW_EMPTY
                     | <lts_lista>
                     | <lts_conjunto>
                     | '(' <expresion> ')'

<op_log>           ::= <op_rel>
                     | <op_log> KW_AND <op_rel>
                     | <op_log> KW_OR <op_rel>

<op_rel>           ::= 'KW_NOT' <op_rel>
                     | <expresion> OP_IN <expresion>
                     | <expresion> OP_EQUAL <expresion>
                     | <expresion> OP_NOT_EQUAL <expresion>
                     | KW_EMPTY <expresion>
                     | <expresion> KW_ANY <expresion>
```

## 3.5 Descripción formal de la sintaxis Aleph
Las instrucciones que conforman todo lenguaje se pueden agrupar en instrucciones de asignación y estructuras de control de flujo. Las instrucciones de asignación también se conocen como de transferencia de datos, pues asignan valores de una dirección a otra o de una expresión a una dirección. Las expresiones que son asignadas o evaluadas manipulan datos y retornan un valor determinado. La evaluación de expresiones puede servir entonces tanto para ser asignada a una dirección de memoria (nominada mediante un identificador) o para determinar bifurcaciones en el flujo de ejecución del programa.

Se presentan a continuación las BNF y EBNFs de las diferentes instrucciones que conforman Alpeh.

## 3.5.1 Asignación y Asignación Múltiple
Las asignaciones son parte fundamental de un lenguaje de programación imperativo, sin ellas no sería posible relacionar valores con variables para un uso posterior. Asignación

[poner aquí la BNF de la asignación y borrar esto]

```c
<asignacion> ::= ID '=' <esprecion> | <asignacino_mult>
```

## 3.5.2 Asignación Múltiple
[poner aquí la BNF de la asignación múltiple y borrar esto]

```C
<asignacin_mult>   ::= KW_LET <lts_id> '=' <lts_expresiones>

<lts_id>           ::= ID | ID ',' <lts_id>

<lts_expresiones>  ::= <expresion> | <expresion> ',' <lts_espresiones>
```

## 3.5.3 Asociatividad y Precendencia de operadores
Para transformar valores y observar relaciones entre ellos debemos contar con expresiones que puedan evaluarse. Se presentan a continuación las expresiones de Aleph.

[poner aquí la Def de asociatividad y precedencia de operadores, explicación de cómo se implementan estyos conceptos en las BNF]

Asociatividad y Precedencia:

La precedencia de operadores responde a la pregunta semántica sobre el orden de evaluación cuando una expresión incluye dos operadores diferentes. En una gramática BNF, la precedencia se especifica forzando a que el operador con mayor prioridad se genere en un nivel más bajo (más alejado del símbolo inicial) dentro del árbol de análisis sintáctico.
La asociatividad define qué operador se evalúa primero cuando una expresión contiene dos operadores adyacentes con el mismo nivel de precedencia. En las gramáticas, la asociatividad por la izquierda se indica mediante una regla recursiva por la izquierda (donde el lado izquierdo de la regla aparece al principio del lado derecho), mientras que la recursividad por la derecha indica asociatividad por la derecha.

[poner aquí la BNF de expresiones aritméticas (números ver en sebesta) con precedencia y asociatividad con uso de paréntesis]
```c
<expresion>  ::= <expresion> "+" <termino> 
               | <expresion> "-" <termino> 
               | <termino>

<termino>    ::= <termino> "*" <factor> 
               | <termino> "/" <factor> 
               | <factor>

<factor>     ::= "(" <expresion> ")" 
               | ID 
               | LIT_NUM
```
[poner aquí la Árbol de derivación de una expresión aritmética que use varios operadores y paréntesis]

Arbol de Derivacion para la expresion para `A = B + C * A`:

                  <asignacion>
                 /      |     \
               ID      "="   <expresion>
               |             /    |    \
               A   <expresion>   "+"  <termino>
                       |              /   |   \
                   <termino>   <termino> "*" <factor>
                       |           |            |
                   <factor>    <factor>         ID
                       |           |            |
                       ID          ID           A
                       |           |
                       B           C

[poner aquí la Def árbol de sintaxis abstracta y derivación]

Derivación y Árbol de Sintaxis Abstracta (AST):

Una derivación es una secuencia de aplicaciones de reglas gramaticales que comienza con un símbolo inicial especial y termina en una oración generada sin símbolos no terminales. El árbol de análisis sintáctico (Parse Tree) describe de forma natural la estructura sintáctica jerárquica de las oraciones del lenguaje. Un Árbol de Sintaxis Abstracta (AST) es una representación condensada de este árbol en donde los símbolos no terminales sintácticos innecesarios se eliminan y los operadores pasan a ser los nodos raíz de sus respectivos operandos.

[Poner aquí el árbol de sintaxis abstracta del árbol de derivación antes descripto]

Árbol de Sintaxis Abstracto (AST) para `A = B + C * A`:

                           "="
                          /   \
                         A    "+"
                             /   \
                            B    "*"
                                /   \
                               C     A

[poner aquí la Def operadores sobrecargados]

Operadores Sobrecargados:

El uso múltiple de un mismo símbolo de operador para diferentes propósitos es lo que se conoce formalmente como sobrecarga de operadores. Al estructurar la gramática de Aleph, asignamos lexemas a delimitadores sintácticos (`), ], }`), separadores sintácticos (`.`) y símbolos especiales (`:`). Por su parte, a los operadores de palabra como la unión los identificamos directamente por su token (`OP_UNION`). De esta forma evitamos la sobrecarga, logrando que el árbol de análisis sintáctico determine de forma única la operación. Esto significa que las herramientas de análisis léxico y sintáctico (`como Flex y Bison`) pueden reconocer y procesar las instrucciones directamente desde la estructura de la gramática, sin requerir reglas semánticas complejas para desambiguar el propósito de un símbolo.

[Para cada operador de aleph indicar precedencia, asociatividad y si hay sobrecarga]

* Relacionales (==, !=, KW_IN): Precedencia Baja, Asociatividad Ninguna, Sobrecargados (comparación de distintos tipos).

* Conjuntos/Listas (union, insert, append): Precedencia Media (igual a suma), Asociatividad Izquierda, No sobrecargados.

* Lógicos (KW_AND, KW_OR): Precedencia Muy Baja, Asociatividad Izquierda, No sobrecargados.

### Expresiones: Operaciones con Conjuntos y Listas
[poner aquí la BNF de Operaciones con Conjuntos y Listas]
```C
<expreciones>  ::= ID
                 | LIT_ELEM
                 | <lts_lista>
                 | <lts_conjunto>
                 | <expresion> OP_UNION <expresion>
                 | <expresión> OP_INTERSECTION <expresion>
                 | <expresión> OP_RELATION <exprecion>
                 | <expresion> OP_DIFFERENCE <expresion>
                 | <expresión> KW_APPEND <exprecion>
                 | <expresión> KW_INSERT <exprecion>
```

### Expresiones: Operaciones Relacionales
[poner aquí la BNF de Operaciones Relacionales]
```c
<op_rel>  ::= <expresion> KW_IN <expresion>
            | <expresion> OP_EQUAL <expresion>
            | <expresion> OP_NOT_EQUAL <expresion> 
            | KW_EMPTY <espresion>
            | <expresión> KW_ANY <exprecion>
            | <expresion> "==" <expresion>
            | <expresion> "!=" <expresion>
```

### Expresiones: Operaciones Lógicas
[Def evaluación perezosa e ingenuas]

La evaluación en cortocircuito (perezosa) determina el resultado de una expresión sin necesidad de evaluar la totalidad de los operandos o los operadores. Por el contrario, la evaluación ingenua evalúa sistemáticamente todas las subexpresiones y operandos antes de aplicar el operador lógico final, incluso si el primer operando ya determinó de forma concluyente el valor de la expresión.

[poner aquí la BNF de Operaciones Lógicas]

```c

<op_log>  ::= <OP_rel>
            | KW_NOT <op_log>
            | <op_log> KW_AND <op_rel>
            | <op_log> KW_OR <OP_rel>
```

## 3.5.4 Estructuras de Control
### El control del flujo de ejecución de instrucciones permite alterar el orden en el que cada sentencia es evaluada.

### Alternativa
[Explique sobre la ambigüedad de la sentencia if]

La ambigüedad en el diseño clásico de la sentencia `if` (dangling-else) ocurre porque una gramática genera una forma sentencial para la cual existen dos o más árboles de análisis sintáctico distintos. Esto ocurre cuando un `if` sin `else` está anidado dentro del bloque de otro `if` externo; el compilador no puede determinar de manera unívoca a cuál de los dos `if` pertenece el else subsecuente.

[poner aquí la BNF y EBNF del if y explque sobre la ambiguedad o no de su propuesta]

```c
<sent_if>  ::= KW_IF  '(' <op_log> ')' <bloque>
             | KW_IF  '(' <op_log> ')' <bloque> KW_ELSE <bloque>
```

El uso estricto del no terminal `<bloque>` definido con los marcadores explícitos DELIM_DO y DELIM_END soluciona formalmente la ambigüedad. Al exigir un final de bloque explícito, el árbol de análisis sintáctico se cierra forzosamente, por lo que un else posterior solo puede enlazarse a la estructura if que actualmente permanezca abierta.

```c
<bloque>  ::= DELIM_DO <lista_sent> DELIM_END
            | DELIM_DO DELIM_END
```

### Ciclos
[poner aquí las BNF de las estructuras de control de ciclos que hayan definido]

```c
<sent_while>  ::= KW_WHILE '(' <op_log> ')' <bloque>

<sent_for>    ::= KW_FOR '(' ID OP_IN ID ')' <bloque>
```

## 3.5.5 Subrutinas
Las subrutinas permiten la modularización de un programa, de manera que no deba repetir código cada vez que deseo obtener un resultado aplicando argumentos a un algoritmo bien determinado, al que podemos llamar función o procedimiento, dependiendo de si el mismo devuelve o no algún dato simple o estructurado.

[poner la BNF de la subrutina (dejar para el final)]

## 3.5.6 Gramática completa de Aleph
[poner aquí la BNF completa]

## 3.6 Análisis Sintáctico pagina 178 sebesta capitilo 4
El análisis sintáctico (syntax analysis o parsing) de un lenguaje de programación se define como en proceso encargado de analizar la sintaxis para un programa dado con entrada sintácticamente correcta, construyendo un árbol de análisis sintáctico (parse trees) que se construye con información necesaria obtenida durante el análisis. Estos árboles y sus derivaciones incluyen toda la información sintáctica que necesita un procesador de lenguaje.

## 3.6.1 Metas del Análisis Sintáctico
El analizador sintáctico posee como metas:
1. Revisar y verificar el programa de entrada (código de un determinado Lenguaje de Programación) sea correcto, en caso de encontrar un error, el analizador debe producir un mensaje de diagnóstico, recuperarse de este y continuar el análisis del programa de entrada. Esto se realiza para encontrar la mayor cantidad de errores posibles en un único análisis del programa de entrada.
2. Producir un árbol de análisis sintáctico completo para una entrada sintácticamente correcta. Este árbol (o su rastreo) se utilizará como base para la traducción.

## 3.6.2 Analizadores Top-Down

Constituye un árbol de análisis en preorden. Dado el árbol de análisis sintáctico, el recorrido preorden construyendo desde la raíz, luego su rama izquierda que corresponde con una derivación por izquierda y por último su rama derecha. Consiste en visitar cada nodo antes de seguir sus ramas. En términos de la derivación, se puede como: dado una forma sentencial que es parte de una derivación más a la izquierda, la tarea del analizador es encontrar la siguiente forma sentencial en esa derivación más a la izquierda. 
La forma general de una forma sentencial izquierda es wAα donde: w es una cadena de símbolos terminales, A es un no terminal, y α es una cadena mixta. Como w contiene solo terminales, A es el no terminal más a la izquierda en la forma sentencial, por lo que es el que debe expandirse para obtener la siguiente forma sentencial en una derivación más a la izquierda. El analizador deberá determinar desde la gramática, la producción correcta que contienen a A en su lado izquierdo y que será utilizada para determinar la siguiente forma sentencial, este será el problema de decisión de los analizadores top-down.

### Gramáticas LL
[explicar]

### Analizadores Descendentes Recursivos
Los analizadores descendientes recursivos constan de un conjunto de subprogrmas, mayormente recursivos. Cada uno de los subprogramas se asocia a una variable sintáctica de la Gramática Libre de Contexto. Por ejemplo, dada una gramática que posee las expresiones aritméticas, que en general tiene tres variables sintácticas: <expresión> <termino> <factor>, habrá un módulo asociado a cada de ellas. El código del subprograma, asociada a cada una de las variables, describe el RHS de la producción a la que esta asociado ese subprograma. 
Los analizadores descendentes más comunes eligen el RHS correcto para el no terminal más a la izquierda en la forma sentencial actual comparando el siguiente token de entrada con los primeros símbolos que pueden generarse por los RHS de esas reglas. El RHS que tenga ese token al inicio de la cadena que genera es el correcto. Así que, en la forma sentencial wAα, el analizador usaría cualquier token que sería el primero generado por A para determinar qué regla de A se debe usar para obtener la siguiente forma sentencial.
La elección de la producción no siempre es tan sencilla, ya sea porque se repiten los terminales al inicio de los RHS o los extremos izquierdos tienen una variable sintáctica, por esto, se debe aplicar la Prueba de pares disjuntos sobre la BNF para detectar el problema antes de implementar el analizador y verificar que siempre sea posible elegir la producción correcta. 

[Ejemplo de módulos descendentes recursivos de las expresiones de Aleph]

## 3.6.3 Analizadores Bottom-Up
Un analizador sintáctico ascendente (bottom-up parser) construye un árbol de análisis sintáctico comenzando en las hojas y avanzando hacia la raíz. Este orden de análisis corresponde al inverso de una derivación por la extrema derecha. Es decir, las formas sentenciales de la derivación se producen en orden desde la última hasta la primera.
En términos de la derivación, un analizador ascendente se puede describir de la siguiente manera: Dada una forma sentencial derecha, el analizador debe determinar qué subcadena es el lado derecho (RHS) de la regla de la gramática que debe reducirse a su lado izquierdo (LHS) para producir la forma sentencial anterior en la derivación por la derecha. Por ejemplo, el primer paso para un analizador ascendente es determinar qué subcadena de la oración inicial dada es el RHS que debe reducirse a su LHS correspondiente para obtener la penúltima forma sentencial de la derivación. 
Elegir el RHS correcto a reducir no es sencillo, pues una forma sentencial derecha dada puede incluir más de un RHS de la gramática del lenguaje que se está analizando. El RHS correcto se denomina manejador (handle).
1. Definición de manejador (handle):
γ
γ es el manejador (handle) de la forma sentencial derecha 
β
β si y solo si:

S
⇒
∗
r
m
A
w
⇒
d
e
r
r
m
α
w
S⇒ 
∗
 rmAw⇒ 
der
​
 rmαw
La idea es identificar la producción que se utiliza en el último paso de una derivación por la derecha. Más precisamente, si la producción es 
A
→
α
A→α, entonces 
α
α es el manejador de la forma sentencial derecha 
r
m
α
w
rmαw.

Esta definición, por sí sola, no siempre facilita la identificación del manejador de una forma sentencial derecha. Para comprender mejor este concepto, se definen las frases que están relacionadas con los manejadores.

2. Definición de frase simple: 
γ
γ es una frase simple de la forma sentencial derecha 
β
β si y solo si:

S
⇒
∗
α
A
w
⇒
d
e
r
α
γ
w
S⇒ 
∗
 αAw⇒ 
der
​
 αγw
donde 
A
→
γ
A→γ es la producción aplicada en el último paso de la derivación por la derecha.

Aquí, 
⇒
∗
⇒ 
∗
  significa uno o más pasos de derivación según la convención de tu material; estrictamente, en la notación habitual, 
⇒
∗
⇒ 
∗
  permite cero o más pasos, mientras que 
⇒
+
⇒ 
+
  indica uno o más pasos.
### Funcionamiento de los analizadores LR
Los analizadores sintácticos LR constituyen el algoritmo ascendente (bottom-up) mas difundido y utilizado en la implementación de procesadores de lenguajes deprogramación. Dadas sus características fundamentales: 
- L (Left-to-right) que indica que el flujo de tokens de entrada se explora y analiza de izquierda a derecha. 
- R (Rightmost derivarion): Especifica que la secuencia de análisis construye la inversa de una derivación por extrema derecha.
Todo analizador LR es un reconocedor basado en un Autómata de Pila determinista (PDA). En esta arquitectura, la pila almacena una secuencia alternada de símbolos gramaticales (X) y símbolos de estado numéricos (S): **$$S_0 \, X_1 \, S_1 \, X_2 \, S_2 \dots X_m \, S_m \quad \text{(tope)}$$**
Donde S0 representa el estado inicial del analizador y el estado Sm sera la información relevante acumulada del análisis sintáctico procesado hasta el momento.
Una configuración completa del analizador LR se representa como el par ordenado compuesto por la pila y la cadena restante: **$$(S_0 \, X_1 \, S_1 \, X_2 \, S_2 \dots X_m \, S_m, \quad a_i \, a_{i+1} \dots a_n \$)$$**, donde $ se coloca allí durante la inicialización del analizador para la terminación normal del proceso.
Dada la configuración se define formalmente el proceso del analizador LR basado en la tabla de análisis:
Una tabla de análisis LR consta de dos partes, llamadas ACTION y GOTO. La parte ACTION tiene símbolos de estado como etiquetas de gilas y los símbolos terminales de la gramática como etiquetas de columnas. Dado el estado actual del analizador y el siguiente símbolo de entrada, la tabla especifica qué debe hacer el analizador.   
### Funcionamiento del analizador generado por Bison
El analizador generado por Bison es un parser ascendente (bottom-up) determinista de la familia LALR(1) (Look-Ahead Left-to-Right with 1 token lookahead). Su tarea principal es reconstruir la inversa de una derivación por la extrema derecha a partir del flujo lineal de tokens que le provee el analizador léxico Flex.

Internamente Bison mediante la función yyparse() dirige la ejecución.  Cada vez que la lógica del autómata necesita un nuevo token, invoca a yylex(), para escanear el código fuente en Aleph y le retorna el valor entero del token junto con sus atributos semánticos en yyval.

El parser LALR(1) utiliza un Autómata con Pila donde cada entrada alterna un símbolo gramatical y un símbolo de estado numérico34: $$(S_0, \, X_1 S_1 \, X_2 S_2 \, \dots \, X_m S_m, \quad a_i a_{i+1} \dots \$)$$. 

En cada paso, el parser consulta las tablas generadas durante la compilación de aleph.y mediante: 
- Acción Shift (Desplazamiento): Si la secuencia de tokens aún no completa el lado derecho (RHS) de una regla, el token $a_i$ se desplaza a la pila junto con un nuevo estado $S_n$, y se solicita el siguiente token a Flex.
- Acción Reduce (Reducción):Ocurre cuando en la cima de la pila se ha completado el lado derecho de una regla de producción. Bison retira de la pila los símbolos del manejador junto con sus estados, apila el símbolo no terminal del lado izquierdo (LHS) y consulta la tabla GOTO para determinar el nuevo estado de la cima. Se ejecutan las acciones en C asociadas a la regla.
- Aceptar (Accept) / Error: Se acepta si se reduce la regla inicial (`programa`) al llegar al fin de archivo (`$`). Si no hay transición válida, se invoca a `yyerror()`.

### Primer Analizador Sintáctico de Aleph

### Tokens y Lexemas tabla actualizada

| Lexema               | Token             | Descripción                                                                              |
| -------------------- | ----------------- | ---------------------------------------------------------------------------------------- |
| `set`                | `KW_SET`          | Tipo de dato / Palabra reservada: conjunto no duplicado.                                 |
| `list`               | `KW_LIST`         | Tipo de dato / Palabra reservada: secuencia o lista ordenada.                            |
| `while`              | `KW_WHILE`        | Sentencia de control: inicio de iteración condicionada.                                  |
| `for`                | `KW_FOR`          | Sentencia de control: iteración sobre elementos de colección.                            |
| `if`                 | `KW_IF`           | Sentencia de control: bifurcación condicional de flujo.                                  |
| `else`               | `KW_ELSE`         | Sentencia de control: rama alternativa de selección.                                     |
| `do`                 | `DELIM_DO`        | Delimitador de bloque: apertura de cuerpo de control o bucle.                            |
| `end`                | `DELIM_END`       | Delimitador de bloque: cierre estructural de bloques o funciones.                        |
| `break`              | `KW_BREAK`        | Sentencia de control: interrupción forzada de iteración.                                 |
| `return`             | `KW_RETURN`       | Sentencia de control: retorno explícito de valor de función.                             |
| `let`                | `KW_LET`          | Declaración: desestructuración o nombramiento formal de componentes.                     |
| `in`                 | `OP_IN`           | Operador relacional: evaluación de pertenencia conjuntista o recorrido.                  |
| `where`              | `KW_WHERE`        | Sentencia de control: cláusula de filtrado condicional en bucles.                        |
| `empty`              | `KW_EMPTY`        | Literal / Constante: representa el conjunto o lista sin elementos.                       |
| `union`              | `OP_UNION`        | Operador de colección: cálculo de la unión algebraica entre conjuntos.                   |
| `intersection`       | `OP_INTERSECTION` | Operador de colección: cálculo de la intersección entre conjuntos.                       |
| `contain`            | `OP_CONTAIN`      | Operador de colección: verificación de pertenencia de un elemento en un conjunto o lista.|
| `relation`           | `OP_RELATION`     | Operador relacional: define o evalúa una relación entre expresiones.                     |
| `difference`         | `OP_DIFFERENCE`   | Operador de colección: cálculo de la diferencia algebraica entre conjuntos.              |
| `any`                | `KW_ANY`          | Función primitiva: verificación existencial en colecciones.                              |
| `append`             | `KW_APPEND`       | Función primitiva: inserción de elemento al final de una lista.                          |
| `insert`             | `KW_INSERT`       | Función primitiva: incorporación de un elemento a un conjunto.                           |
| `&&`                 | `KW_AND`          | Operador lógico: conjunción (Y lógico).                                                  |
| `!`                  | `KW_NOT`          | Operador lógico: negación unaria.                                                        |
| `\|\|`               | `KW_OR`           | Operador lógico: disyunción (O lógico).                                                  |
| `:`                  | `DELIM_COLON`     | Símbolo especial: delimitador de apertura del bloque de función.                         |
| `=`                  | `OP_ASSIGN`       | Operador básico: asignación destructiva de valor a identificador.                        |
| `==`                 | `OP_EQUAL`        | Operador relacional: prueba de igualdad estructural estricta.                            |
| `!=`                 | `OP_NOT_EQUAL`    | Operador relacional: prueba de desigualdad estructural estricta.                         |
| `;`                  | `DELIM_SEMICOLON` | Delimitador sintáctico: terminación explícita de sentencia.                              |
| `,`                  | `DELIM_COMMA`     | Separador sintáctico: división posicional en listas, conjuntos o parámetros.             |
| `(`                  | `DELIM_LPAREN`    | Delimitador sintáctico: apertura de tuplas, condiciones o llamadas.                      |
| `)`                  | `DELIM_RPAREN`    | Delimitador sintáctico: cierre de tuplas, condiciones o llamadas.                        |
| `{`                  | `DELIM_LBRACE`    | Delimitador sintáctico: apertura de especificación literal de conjunto.                  |
| `}`                  | `DELIM_RBRACE`    | Delimitador sintáctico: cierre de especificación literal de conjunto.                    |
| `[`                  | `DELIM_LBRACKET`  | Delimitador sintáctico: apertura de especificación literal de lista.                     |
| `]`                  | `DELIM_RBRACKET`  | Delimitador sintáctico: cierre de especificación literal de lista.                       |
| `'0'`,`'1'`,`'a'`    | `LIT_ELEMENT`     | Literales: representación atómica de un elemento base para conformar colecciones.        |
| `[A-Z][a-zA-Z0-9_]*` | `ID`              | Identificadores: nombres de variables definidos por el usuario iniciados en mayúscula.   |

```c

<Aleph>            ::= <lts_sent>

<lts_sent>         ::= <sent> ';'
                       | <lts_sent> <sent> ';'
  
<bloque>           ::= DELIM_DO <lts_sent> DELIM_END
                     | DELIM_DO DELIM_END

<sent>             ::= <asignacion>
                     | <expresion> 
                     | <sent_while> 
                     | <sent_if> 
                     | <sent_for>

<asignacion>        ::= ID '=' <expresion>
                      | KW_LET <lts_id> '=' <lts_expresiones>

<lts_id>            ::= ID 
                      | <lts_id> ',' ID

<sent_while>        ::= KW_WHILE '(' <op_log> ')' <bloque>

<sent_for>          ::= KW_FOR '(' ID OP_IN <expresion> ')' <bloque>

<sent_if>           ::= KW_IF '(' <op_log> ')' <bloque>
                      | KW_IF '(' <op_log> ')' <bloque> KW_ELSE <bloque>

<lts_expresiones>   ::= <expresion> 
                      | <lts_expresiones> ',' <expresion>

<lts_lista>         ::= '[' <lts_expresiones> ']'
                      | '[' ']'

<lts_conjunto>      ::= '{' <lts_expresiones> '}'
                      | '{' '}'

<expresion>         ::= ID
                      | LIT_ELEM
                      | <lts_lista>
                      | <lts_conjunto>
                      | <expresion> OP_UNION <expresion>
                      | <expresion> OP_INTERSECTION <expresion>
                      | <expresion> OP_DIFFERENCE <expresion>
                      | <expresion> KW_APPEND <expresion>
                      | <expresion> KW_INSERT <expresion>

<op_log>           ::= <op_rel>
                     | KW_NOT <op_rel>
                     | KW_NOT '(' <op_rel> '')'
                     | <op_log> KW_AND <op_rel>
                     | <op_log> KW_OR <op_rel>

<op_rel>           ::= <expresion> OP_IN <expresion>
                     | <expresion> "==" <expresion>
                     | <expresion> "!=" <expresion>
                     | KW_EMPTY <expresion>
                     | <expresion> KW_ANY <expresion>

```

