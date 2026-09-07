# 1. Introducción
## 1.1 Objetivo

El objetivo principal del Trabajo Práctico Transversal es diseñar, desarrollar e implementar un intérprete funcional para un nuevo lenguaje de programación denominado Aleph, orientado al trabajo con Autómatas Finitos (AF) y vinculado con la Teoría de Conjuntos, que constituye uno de los pilares estructurales del lenguaje. La idea es que Aleph permita llevar a la práctica los conceptos y algoritmos estudiados en Teoría de la Computación I, brindando al usuario la posibilidad de definir, almacenar, representar, manipular y analizar autómatas, y de ejecutar sobre ellos los distintos algoritmos vistos durante la asignatura.

En particular, el lenguaje deberá permitir representar Autómatas Finitos, convertir un Autómata Finito No Determinista (AFND) en un Autómata Finito Determinista (AFD), determinar si una cadena pertenece al lenguaje aceptado por un autómata, visualizar sus estructuras y obtener representaciones equivalentes. También se busca trabajar con las relaciones existentes entre Autómatas Finitos, Lenguajes Regulares (LR) y Gramáticas Regulares (GR), permitiendo, por ejemplo, obtener a partir de un autómata una Gramática Regular que genere el mismo lenguaje que este acepta.

El desarrollo de Aleph permitirá, además, aplicar de manera concreta las distintas etapas involucradas en la construcción de un lenguaje de programación. Para ello se abordarán el análisis léxico, sintáctico y semántico, utilizando herramientas como Flex y Bison, y se diseñarán las estructuras de datos necesarias para representar y procesar la información durante la ejecución. Entre estas estructuras se contemplan los Árboles de Sintaxis Abstracta (AST) y la Tabla de Símbolos, junto con las estructuras necesarias para representar los autómatas, sus estados, alfabetos, transiciones y demás elementos relacionados con la Teoría de Conjuntos.

A lo largo del proyecto también se busca analizar y fundamentar las decisiones tomadas durante el diseño del lenguaje, teniendo en cuenta aspectos como la legibilidad, capacidad de escritura, expresividad y confiabilidad, procurando que Aleph resulte suficientemente sencillo y claro para el usuario, pero al mismo tiempo preciso para su procesamiento computacional con objetivo final el abstraer la implementación realizada en el TPT de la asignatira anterior. Esto permitirá comprender no solo cómo se utilizan las herramientas para construir un lenguaje, sino también las dificultades, posibilidades y limitaciones que surgen al momento de diseñarlo e implementarlo.

La construcción del lenguaje se desarrollará siguiendo las etapas clásicas de un proyecto de software: análisis, diseño e implementación, dejando fuera la etapa de mantenimiento.

De esta forma, el proyecto permitirá vincular los conceptos teóricos estudiados con una implementación concreta, pasando de la definición formal de autómatas, lenguajes y gramáticas a un sistema capaz de representarlos y operar sobre ellos de manera práctica.

## 1.2. Sobre el nombre del lenguaje
El lenguaje a desarrollar se denominará Aleph puesto que este nombre tiene una fuerte vinculación con la Teoría de Conjuntos, estructura que será el pilar del desarrollo del nuevo lenguaje.

En las matemáticas, particularmente en la teoría de conjuntos, los números de Aleph son una secuencia de números utilizados para representar la cardinalidad (o tamaño) de conjuntos infinitos que pueden ser bien ordenados. Fueron introducidos por el matemático Georg Cantor, quien se dio cuenta de que conjuntos infinitos pueden tener diferentes cardinalidades y son nombrados por el símbolo que usaba para denotarlos, la letra semita Aleph ($\aleph$).

Los números Aleph difieren del infinito ($\infty$) comúnmente encontrado en álgebra y cálculo, en que los Alephs miden los tamaños de conjuntos, mientras que el infinito será un punto extremo de la línea de números reales extendida.

Se denominan cardinalidades de conjuntos transfinitos:

Álef cero ($\aleph_0$): Representa el primer y más pequeño número cardinal transfinito, correspondiente al tamaño exacto de los conjuntos infinitamente numerables, como el de los números naturales ($\mathbb{N}$), los números enteros ($\mathbb{Z}$) o las fracciones racionales ($\mathbb{Q}$).

Álef uno ($\aleph_1$): El siguiente cardinal que describe el tamaño del continuo numérico (el conjunto de los números reales $\mathbb{R}$ o la cantidad infinita de puntos en un segmento de recta), de acuerdo con la clásica Hipótesis del Continuo.

En la teoría cantoriana, surge la propiedad de que en conjuntos infinitos "el todo no es mayor que alguna de sus partes". Un conjunto infinito puede tener una correspondencia biunívoca (uno a uno) con un subconjunto propio de sí mismo (por ejemplo, hay tantos números pares como números naturales en su totalidad, compartiendo ambos la cardinalidad $\aleph_0$), rompiendo con el axioma clásico de Euclides y dando origen a una nueva aritmética lógica útil para la computación formal.

También Jorge Luis Borges, en "El Aleph" (1949), traslada al campo literario y cultural esta idea.

En el relato, el Aleph es descrito como una pequeña esfera de apenas unos centímetros que se encuentra en el sótano de una casa, pero que contiene a todos los demás puntos del universo de manera simultánea e indistinta, objeto capaz de alojar el todo en una parte.

Aleph, en nuestro contexto, integra el pensamiento de construir una jerarquía ordenada de cardinales de algo pensado como un infinito singular y la capacidad de un lenguaje de programación de describir infinitos mundos lógicos mediante sintaxis finita.

## 1.3. Sobre las herramientas a utilizar en el desarrollo

Flex y Bison son herramientas diseñadas para escritores de compiladores e intérpretes, aunque también son útiles para muchas aplicaciones que podrían interesar a personas que no escriben compiladores. Cualquier aplicación que busque patrones en su entrada o que tenga un lenguaje de entrada o comandos es un buen candidato para Flex y Bison. Además, permiten un prototipado rápido de aplicaciones, modificaciones fáciles y un mantenimiento sencillo.

¿De dónde vinieron Flex y Bison? 

Bison desciende de Yacc, un generador de analizadores sintácticos creado entre 1975 y 1978 por Stephen C. Johnson en Bell Labs. Como su nombre, abreviatura de “Yet Another Compiler-Compiler” (otro compilador de compiladores más), sugiere, muchas personas estaban escribiendo generadores de analizadores sintácticos en esa época. La herramienta de Johnson combinaba una base teórica sólida gracias al trabajo de análisis de D. E. Knuth, lo que hacía que sus analizadores fueran extremadamente fiables, y una sintaxis de entrada conveniente.

Esto la hizo muy popular entre los usuarios de sistemas Unix, aunque la licencia restrictiva bajo la cual se distribuía Unix en ese entonces limitaba su uso fuera del mundo académico y del Bell System. Alrededor de 1985, Bob Corbett, un estudiante de posgrado en la Universidad de California, Berkeley, reimplementó Yacc usando algoritmos internos algo mejorados, lo que evolucionó a Berkeley Yacc. Como su versión era más rápida que la de Bell y se distribuía bajo la flexible licencia de Berkeley, rápidamente se convirtió en la versión más popular de Yacc.

Richard Stallman, de la Free Software Foundation (FSF), adaptó el trabajo de Corbett para el proyecto GNU, donde ha crecido para incluir una gran cantidad de nuevas características mientras evolucionaba hasta convertirse en la versión actual de Bison. Bison ahora se mantiene como un proyecto de la FSF y se distribuye bajo la Licencia Pública de GNU.

En 1975, Mike Lesk y Eric Schmidt escribieron Lex, un generador de analizadores léxicos, siendo la mayor parte de la programación realizada por Schmidt. Lo vieron tanto como una herramienta independiente como un compañero del Yacc de Johnson. Lex también se volvió bastante popular, a pesar de ser relativamente lento y tener errores. 

Alrededor de 1987, Vern Paxson del Lawrence Berkeley Lab tomó una versión de Lex escrita en Ratfor (una versión extendida de Fortran popular en ese momento) y la tradujo a C, llamándola Flex, por “Fast Lexical Analyzer Generator” (Generador Rápido de Analizadores Léxicos).

Los programas de Bison tienen (no por coincidencia) la misma estructura de tres partes que los programas de Flex, con declaraciones, reglas y código C.

Por otra parte, La técnica de coincidencia de patrones que usa flex es bastante rápida y suele tener aproximadamente la misma velocidad que un escáner hecho a mano. Para escáneres más complejos con muchos patrones, un escáner flex podría incluso ser más rápido, ya que el código escrito a mano generalmente hará muchas comparaciones por carácter, mientras que flex siempre hace una sola. La versión de flex de un escáner es invariablemente mucho más corta que el C equivalente, lo que lo hace mucho más fácil de depurar. En general, si las reglas para dividir un flujo de entrada en tokens pueden describirse mediante expresiones regulares, flex es la herramienta a elegir.

Aunque usamos Flex para generar nuestros analizadores léxicos, mientras que Flex reconoce expresiones regulares, Bison reconoce gramáticas completas. Flex divide el flujo de entrada en partes (tokens), y luego Bison toma estas partes y las agrupa de manera lógica.

Bison toma una gramática que especificamos y escribe un analizador que reconoce "oraciones" válidas en esa gramática. Usamos el término oración para una gramática del lenguaje C como las oraciones son programas C sintácticamente válidos.

Los programas pueden ser sintácticamente válidos pero semánticamente inválidos, por ejemplo, un programa en C que asigna una cadena a una variable `int`. Bison solo maneja la sintaxis; la otra validación depende de nosotros.

## 1.4 Tipos de implementación

Los sistemas de implementación de los primeros lenguajes de programación de alto nivel, construidos a finales de los años 50, fueron de los sistemas de software más complejos de esa época.

En los años 60, se realizaron esfuerzos de investigación intensivos para entender y formalizar el proceso de construcción de estas implementaciones de lenguajes de alto nivel. El mayor éxito de esos esfuerzos se dio en el área del análisis de sintaxis, principalmente porque esa parte del proceso de implementación es una aplicación de partes de la teoría de autómatas y la teoría de lenguajes formales que en ese momento ya se comprendían bien. 
Los lenguajes de programación se pueden implementar mediante cualquiera de tres métodos generales.
### Compilador
Traduce programas a lenguaje de máquina, que se puede ejecutar directamente en la computadora.El lenguaje que un compilador traduce se llama lenguaje fuente.

La implementación de compiladores tiene la ventaja de una ejecución de programas muy rápida, una vez que el proceso de traducción está completo.

La mayoría de las implementaciones de producción de lenguajes, como C, COBOL, C++ y Ada, se hacen mediante compiladores.
El proceso de compilación y ejecución del programa se da en varias fases, siendo las más importantes:

1. Análisis léxico
El analizador léxico reúne los caracteres del programa fuente en unidades léxicas.

Las unidades léxicas de un programa son:

Identificadores
Palabras reservadas
Operadores
Símbolos de puntuación
El analizador léxico ignora los comentarios en el programa fuente porque el compilador no los necesita.

2. Análisis sintáctico
Toma las unidades léxicas producidas por el analizador léxico y las utiliza para construir estructuras jerárquicas llamadas árboles de análisis, estos representan la estructura sintáctica del programa.En otros casos, se genera y se utiliza directamente la información que se requeriría para construir un árbol.

3. Análisis semántico: comprueba que el programa tenga un significado válido. Por ejemplo, detecta errores de tipos.

4. Generación de código intermedio: transforma el programa a una representación intermedia entre el lenguaje fuente y el lenguaje máquina.

5. Optimización: mejora el código intermedio para obtener un programa más rápido, pequeño o eficiente.

6. Generación de código: convierte el código intermedio optimizado en lenguaje máquina. La tabla de símbolos funciona como una base de datos para el proceso de compilación, sus contenidos principales son la información de tipo y atributos de cada nombre definido por el usuario en el programa. Esta información es colocada en la tabla de símbolos por los analizadores léxico y sintáctico y es utilizada por el analizador semántico y el generador de código.

El compilador construye llamadas a los programas del sistema necesarios cuando el programa del usuario los requiere. Antes de que los programas en lenguaje de máquina producidos por un compilador puedan ejecutarse, los programas necesarios del sistema operativo deben ser encontrados y enlazados con el programa del usuario. La operación de enlace conecta el programa del usuario con los programas del sistema al colocar las direcciones de los puntos de entrada de los programas del sistema en las llamadas a ellos dentro del programa del usuario. El código del usuario y el del sistema juntos a veces se llaman módulo de carga o imagen ejecutable. 

El proceso de reunir los programas del sistema y enlazarlos con los programas de usuario se llama enlazado y carga, o a veces solo enlazado. Esto se realiza mediante un programa del sistema llamado enlazador. Además de los programas del sistema, los programas de usuario a menudo deben enlazarse con programas de usuario previamente compilados que residen en bibliotecas. Así que el enlazador no solo enlaza un programa dado con los programas del sistema, sino que también puede enlazarlo con otros programas de usuario. 

La velocidad de la conexión entre la memoria de una computadora y su procesador usualmente determina la velocidad de la computadora, porque las instrucciones a menudo pueden ejecutarse más rápido de lo que pueden trasladarse al procesador para su ejecución. Esta conexión se llama el cuello de botella de von Neumann; es el principal factor limitante en la velocidad de las computadoras con arquitectura de von Neumann. 

### Interpretación Pura
Con este enfoque, los programas son interpretados por otro programa llamado intérprete, sin ninguna traducción.

El programa intérprete actúa como una simulación de software de una máquina cuyo ciclo de búsqueda-ejecución se ocupa de declaraciones de programas en lenguaje de alto nivel en lugar de instrucciones de máquina. Esta simulación de software, obviamente, proporciona una máquina virtual para el lenguaje. 

La interpretación pura tiene la ventaja de permitir una fácil implementación de muchas operaciones de depuración a nivel de código fuente, porque todos los mensajes de error en tiempo de ejecución pueden referirse a unidades a nivel de fuente. Por ejemplo, si se encuentra que un índice de un arreglo está fuera de rango, el mensaje de error puede indicar fácilmente la línea de origen y el nombre del arreglo. 

Por otro lado, este método tiene la seria desventaja de que la ejecución es de 10 a 100 veces más lenta que en los sistemas compilados. La fuente principal de esta lentitud es la decodificación de las declaraciones del lenguaje de alto nivel, que son mucho más complejas que las instrucciones de lenguaje de máquina (aunque puede haber menos declaraciones que instrucciones en código máquina equivalente). Además, sin importar cuántas veces se ejecute una declaración, debe decodificarse cada vez. Por lo tanto, la decodificación de declaraciones, más que la conexión entre el procesador y la memoria, es el cuello de botella de un intérprete puro.

Otra desventaja de la interpretación pura es que a menudo requiere más espacio. Además del programa fuente, la tabla de símbolos debe estar presente durante la interpretación. Además, el programa fuente puede almacenarse de una forma diseñada para un acceso y modificación fáciles en lugar de una que proporcione tamaño mínimo. Aunque algunos lenguajes simples de los años 60 (APL, SNOBOL y LISP) eran puramente interpretados, para los años 80, el enfoque rara vez se usaba en lenguajes de alto nivel.

### Sistemas de Implementación Híbrida
Este método es más rápido que la interpretación pura porque las instrucciones del lenguaje fuente se decodifican solo una vez. Tales implementaciones se llaman sistemas de implementación híbridos. 

En lugar de traducir el código del lenguaje intermedio a código máquina, simplemente interpreta el código intermedio.

Un ejemplo es Perl, se implementa con un sistema híbrido. Los programas en Perl se compilan parcialmente para detectar errores antes de la interpretación y simplificar el intérprete. Las implementaciones iniciales de Java eran todas híbridas. Su forma intermedia, llamada bytecode, proporciona portabilidad a cualquier máquina que tenga un intérprete de bytecode y un sistema de ejecución asociado. Juntos, esto se llama la Máquina Virtual de Java. 

Hoy en día hay sistemas que traducen bytecode de Java a código máquina para una ejecución más rápida. Un sistema de implementación Just-in-Time (JIT) traduce inicialmente los programas a un lenguaje intermedio. Luego, durante la ejecución, compila los métodos del lenguaje intermedio a código máquina cuando se llaman. La versión en código máquina se guarda para llamadas posteriores. Los sistemas JIT se usan ahora ampliamente para programas Java. Además, todos los lenguajes .NET se implementan con un sistema JIT. A veces, un implementador puede proporcionar implementaciones tanto compiladas como interpretadas para un lenguaje. 

En estos casos, se usa el intérprete para desarrollar y depurar programas. Luego, después de alcanzar un estado (relativamente) libre de errores, los programas se compilan para aumentar su velocidad de ejecución.

## 1.4.1 Enfoque de implementación de Aleph

El lenguaje Aleph se implementa mediante un enfoque híbrido de interpretación basado en Árboles de Sintaxis Abstracta (AST):

Aleph no interpreta el archivo de texto línea por línea. El código fuente es procesado estáticamente por un analizador léxico (Flex) y un analizador sintáctico (Bison). Estas herramientas validan la gramática formal del lenguaje (definida en BNF) y traducen el flujo de caracteres en una representación intermedia estructurada en memoria llamado Árbol de Sintaxis Abstracta (AST).
Aleph define una computadora virtual cuya representación de datos en tiempo de ejecución: el AST (para la estructura de control de las expresiones) y la Tabla de Símbolos  que actúa como el diccionario dinámico para asociar identificadores, variables de conjuntos, tipos y ámbitos en memoria.
Dado el AST sin errores sintácticos, el motor semántico de Aleph actúa como un intérprete de software que evalúa las expresiones mediante algoritmos recursivos que realizan un recorrido del árbol. Al visitar los nodos del AST, se simulan por software las operaciones matemáticas de la Teoría de Conjuntos (unión, intersección, diferencia) y la lógica de control, interactuando con la Tabla de Símbolos en tiempo de ejecución.
La arquitectura de Aleph utiliza la fase de compilación frontal con la manipulación interactiva de conjuntos y autómatas en memoria que brinda flexibilidad y control dinámico.

