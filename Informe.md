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
