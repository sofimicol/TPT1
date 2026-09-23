1. Introducción1.1 ObjetivoEl objetivo principal de este Trabajo Práctico Transversal consiste en diseñar, desarrollar e implementar un intérprete funcional para un nuevo lenguaje de programación denominado Aleph. El diseño de Aleph se enfoca en proporcionar abstracciones generales de alto nivel para el programador, priorizando la manipulación declarativa y algorítmica de colecciones de datos —específicamente conjuntos no ordenados y listas ordenadas— mediante operaciones algebraicas nativas, junto con estructuras de control estructuradas que permitan modelar, anidar y recorrer datos de forma sistemática y legible [Louden, 2004, p. 11].Para evaluar la expresividad, legibilidad y confiabilidad del lenguaje, se toman casos de estudio concretos provenientes de la Teoría de la Computación. En particular, Aleph debe permitir expresar la definición formal de Autómatas Finitos (AF), ejecutar la conversión de un Autómata Finito No Determinista (AFND) a un Autómata Finito Determinista (AFD), evaluar la aceptación o rechazo de cadenas de entrada y vincular las representaciones estructurales entre Lenguajes Regulares y Gramáticas Regulares.Es fundamental recalcar que estos algoritmos formales no constituyen primitivas ni tipos de datos predefinidos en la sintaxis de Aleph; representan el dominio de prueba sobre el cual se evalúa la potencia representativa de las abstracciones de datos y de control diseñadas.El proyecto abarca las fases de análisis, diseño e implementación de software de sistemas, abordando formalmente el análisis léxico, el análisis sintáctico y la interpretación semántica basada en estructuras de datos dinámicas en memoria.1.2 Sobre el nombre del lenguajeEl lenguaje se denomina Aleph debido a su relación conceptual con la Teoría de Conjuntos, marco fundacional de su modelo de datos.En la matemática formal, la secuencia de los números Álef ($\aleph$) permite denotar la cardinalidad —concepto que define el tamaño cuantitativo o cantidad de elementos— de conjuntos transfinitos bien ordenados. Esta formulación fue desarrollada por Georg Cantor al demostrar que no todos los conjuntos infinitos poseen la misma magnitud cardinal:Álef cero ($\aleph_0$): Representa la menor cardinalidad transfinita, correspondiente a los conjuntos infinitos numerables, tales como el conjunto de los números naturales ($\mathbb{N}$), los números enteros ($\mathbb{Z}$) o los números racionales ($\mathbb{Q}$).Álef uno ($\aleph_1$): Representa, bajo la Hipótesis del Continuo de Cantor, el cardinal inmediato superior que describe la potencia del continuo matemático, correspondiente al conjunto de los números reales ($\mathbb{R}$).En la computación teórica, las propiedades demostradas por Cantor posibilitan que un sistema lógico formal con reglas de derivación finitas sea capaz de definir y procesar conjuntos infinitos de cadenas (lenguajes formales). Asimismo, en la literatura hispanoamericana, Jorge Luis Borges en su obra El Aleph (1949) conceptualiza el término como un punto en el espacio que concentra, sin superponerse, la totalidad del universo. En el diseño del lenguaje, Aleph sintetiza esta propiedad: dotar a la computadora de una sintaxis finita y rigurosa capaz de manipular estructuras jerárquicas y universos de estados formales.1.3 Sobre las herramientas a utilizar en el desarrolloPara la construcción del procesador del lenguaje Aleph se emplean las herramientas de generación automática Flex y Bison [Levine, 2009, p. 1]:Flex (Fast Lexical Analyzer Generator): Herramienta que recibe como especificación de entrada un archivo de texto con pares formados por expresiones regulares y bloques de acciones en código C. Su salida es un programa en C que implementa un analizador léxico (scanner o autómata finito determinista subyacente). Su función es procesar el flujo continuo de caracteres del programa fuente en Aleph, agruparlos en unidades mínimas válidas (lexemas) y retornar secuencialmente su categoría sintáctica abstracta (tokens) [Levine, 2009, p. 3].Bison (GNU Parser Generator): Generador de analizadores sintácticos que toma como entrada una especificación formal basada en una Gramática Libre de Contexto descrita en notación BNF (Backus-Naur Form). Como salida, genera un programa en C que implementa un analizador sintáctico algorítmico LALR(1) (parser). Su función consiste en verificar que la secuencia lineal de tokens satisfaga las reglas de derivación de la gramática y dirigir la construcción de una estructura jerárquica en memoria: el Árbol de Sintaxis Abstracta (AST) [Levine, 2009, p. 53].Comunicación e Interacción Operativa:La interacción entre ambas herramientas es gobernada por Bison. Durante el proceso de análisis, cada vez que la lógica del parser requiere consumir un símbolo terminal para realizar una operación de desplazamiento (shift) o reducción (reduce), invoca a la función generada por Flex, yylex(). Esta rutina reanuda el escaneo de caracteres, consume el lexema correspondiente y devuelve un valor entero representativo del token, comunicando opcionalmente sus atributos semánticos a través de la variable global de comunicación yylval.Bison delimita estrictamente su tarea a la validación de la sintaxis estructural. Las comprobaciones de semántica estática y dinámica —como la verificación de tipos, la validación de alcance de identificadores en la Tabla de Símbolos y la consistencia de operaciones sobre colecciones— no son resueltas por Bison; estas responsabilidades son implementadas ad-hoc mediante algoritmos recursivos de visita sobre los nodos del AST [Levine, 2009, p. 110].1.4 Tipos de implementaciónDe acuerdo con la clasificación formal de lenguajes de programación, se distinguen tres métodos fundamentales de implementación [Sebesta, 2012, pp. 25-30]:Compilación: El programa fuente es traducido de forma global e integral por un compilador hacia un lenguaje destino equivalente, el cual puede consistir en lenguaje de máquina nativo del procesador, lenguaje ensamblador o código intermedio estructurado [Sebesta, 2012, p. 25]. Una vez completada la traducción y enlazadas las dependencias, el código destino se ejecuta de forma autónoma a nivel de hardware, lo que garantiza una máxima velocidad de ejecución.Interpretación Pura: El programa fuente no se traduce a un lenguaje destino ejecutable. En su lugar, un programa denominado intérprete actúa como una máquina virtual de software que decodifica y ejecuta secuencialmente las sentencias del lenguaje fuente en tiempo real [Sebesta, 2012, p. 28]. Este método facilita la depuración y portabilidad a nivel de código fuente, pero presenta una degradación severa en el rendimiento temporal debido a la decodificación repetitiva de sentencias en bucles y estructuras de control.Sistemas de Implementación Híbrida: En este modelo, el código fuente es traducido inicialmente a un lenguaje intermedio de bajo nivel y representación lineal (comúnmente denominado bytecode). Posteriormente, dicho lenguaje intermedio es decodificado y ejecutado por un motor de interpretación o máquina virtual de tiempo de ejecución (como la JVM de Java) [Sebesta, 2012, p. 29]. Este método evita decodificar la sintaxis de alto nivel más de una vez, logrando un compromiso entre portabilidad y velocidad.1.4.1 Enfoque de implementación de AlephCon base en los fundamentos conceptuales de Sebesta [2012] y Levine [2009], el sistema de ejecución de Aleph se clasifica formalmente como un intérprete puro con front-end de análisis estático.Aleph no constituye una implementación híbrida: no genera ni emite un archivo o flujo lineal de código intermedio (bytecode) que deba ser decodificado por una máquina virtual basada en registros o en pila. La fase previa construida mediante Flex y Bison opera estrictamente como un módulo de análisis frontal (front-end léxico y sintáctico) cuya única función es transformar el texto plano en una estructura arbórea de datos en memoria dinámica: el Árbol de Sintaxis Abstracta (AST) [Levine, 2009, p. 115].El AST no es un lenguaje destino autónomo, sino la representación interna en memoria de la estructura formal del programa fuente. La ejecución del programa se efectúa a través del motor semántico del intérprete, el cual realiza recorridos recursivos en profundidad (árbol de visitas) sobre los nodos del AST. Cada nodo ejecuta directamente las operaciones lógicas, algebraicas y de control manipulando los estados de las variables en una Tabla de Símbolos activa en memoria [Sebesta, 2012, p. 28].2. Análisis de requerimientos2.1 Un caso de estudio: Los Autómatas Finitos y sus algoritmosFormalmente, un Autómata Finito Determinista (AFD) se define como una 5-upla:$$A = (Q, \Sigma, \delta, q_0, F)$$Donde:$Q$: Conjunto finito, no vacío, de estados.$\Sigma$: Alfabeto finito, no vacío, de símbolos de entrada.$\delta$: Función total de transición, definida como $\delta: Q \times \Sigma \to Q$.$q_0$: Estado inicial, con $q_0 \in Q$.$F$: Conjunto de estados de aceptación o finales, con $F \subseteq Q$.Por su parte, un Autómata Finito No Determinista (AFND) se define formalmente como una 5-upla:$$A = (Q, \Sigma, \delta, q_0, F)$$Donde la diferencia estructural radica en su relación de transición:$$\delta: Q \times \Sigma \to \mathcal{P}(Q)$$En un AFND, la evaluación de un símbolo sobre un estado actual produce un subconjunto perteneciente al conjunto de partes de $Q$ (denotado $\mathcal{P}(Q)$ o $2^Q$).2.1.1 Aceptación de cadenasPara evaluar una cadena $w \in \Sigma^*$ en un AFD, se generaliza formalmente la función de transición a una función de transición extendida $\hat{\delta}: Q \times \Sigma^* \to Q$, definida inductivamente por:$\hat{\delta}(q, \varepsilon) = q$, donde $\varepsilon$ es la cadena vacía.$\hat{\delta}(q, aw) = \hat{\delta}(\delta(q, a), w)$, para todo símbolo $a \in \Sigma$ y cadena $w \in \Sigma^*$.Una cadena $w$ es aceptada por un AFD si y solo si $\hat{\delta}(q_0, w) \in F$. Para un AFND, una cadena $w$ es aceptada si el conjunto de estados alcanzados contiene al menos un estado final: $\hat{\delta}(q_0, w) \cap F \neq \emptyset$.A continuación se transcribe el algoritmo de reconocimiento implementado en C:C// Algoritmo de Aceptación de Cadenas:
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
2.1.2 Algoritmo de conversión de AFND a AFD (Construcción de Subconjuntos)Teorema: Para todo lenguaje $L$ aceptado por un AFND $A = (Q_A, \Sigma, \delta_A, q_{0A}, F_A)$, existe un AFD equivalente $B = (Q_B, \Sigma, \delta_B, q_{0B}, F_B)$ tal que $L(B) = L(A)$.La construcción algorítmica establece:$Q_B \subseteq \mathcal{P}(Q_A)$: Cada estado del AFD corresponde a un subconjunto de estados del AFND.$\delta_B(R, a) = \bigcup_{q \in R} \delta_A(q, a)$, para cada $R \in Q_B$ y $a \in \Sigma$.$q_{0B} = \{q_{0A}\}$.$F_B = \{R \in Q_B \mid R \cap F_A \neq \emptyset\}$.A continuación se transcribe la implementación en C del algoritmo:C// Algoritmo de Conversion: 
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
Modelado computacional de AF's y requerimientos de abstracción en AlephEn el diseño de Aleph, la representación de los Autómatas Finitos se modela prescindiendo de tipos abstractos cerrados en C, utilizando las abstracciones universales del lenguaje:Los conjuntos de estados $Q$, el alfabeto $\Sigma$ y los estados finales $F$ se modelan de forma directa mediante la estructura matemática set.La función o relación de transición $\delta$ se modela computacionalmente mediante listas anidadas de tuplas o relaciones: list Delta = [[q_origen, simbolo, destinos], ...].La 5-upla completa se representa agregando estas colecciones en una lista de orden posicional: list Autómata = [Q, Sigma, Delta, q0, F].Análisis de los algoritmos en C y abstracción requerida:El análisis de las implementaciones precedentes revela que, aunque se utilicen interfaces de funciones como obtener_data, obtener_next, insert_set y conjuntos_iguales, el código en C exhibe fallas de encapsulamiento debido al acoplamiento con la estructura de almacenamiento. El uso explícito de punteros directos (aux_cadena->next, aux_cadena->data) y el avance manual nodo a nodo evidencian que el programador está forzado a operar al nivel de enlaces de memoria física.Este análisis fundamenta un requerimiento arquitectónico crítico para el diseño de Aleph: el lenguaje debe proveer abstracciones de control de alto nivel (como el bucle declarativo for elemento in coleccion) y operadores de manipulación de colecciones (como union, in, insert). Estas construcciones permiten aislar por completo al programador del modelo de memoria y del encadenamiento de punteros subyacente, garantizando que los algoritmos matemáticos sobre autómatas se expresen en función de su lógica conjuntista y no de la gestión de memoria dinámica [Louden, 2004, pp. 261-263].2.2 Conceptos sobre lenguajes de programaciónDesde la perspectiva teórica de Louden, un lenguaje de programación se define formalmente como un sistema notacional diseñado para describir computaciones de manera legible tanto para el ser humano como para la máquina [Louden, 2004, p. 1].Esta conceptualización exige un equilibrio en dos frentes complementarios:Legibilidad para el ser humano: El lenguaje debe proveer abstracciones de alto nivel orientadas a la naturaleza lógica del problema, reduciendo la carga cognitiva impuesta por los detalles de bajo nivel del hardware (direcciones de memoria, registros y saltos de control). Debe permitir la expresión clara, concisa y mantenible de los algoritmos [Louden, 2004, p. 7].Procesabilidad por la máquina: El sistema notacional debe poseer una sintaxis formal, inequívoca y libre de ambigüedad matemática, susceptible de ser analizada por algoritmos deterministas en tiempo y espacio finitos, permitiendo a la computadora asociar un significado semántico unívoco a cada construcción de código [Louden, 2004, p. 8].Aleph se estructura sobre esta definición: traduce las operaciones conjuntistas y de teoría de lenguajes formales a una notación ejecutable y determinista para el intérprete, preservando la familiaridad de la notación matemática para el analista.2.3 AbstraccionesEn la teoría de diseño de lenguajes de programación, una abstracción constituye un mecanismo conceptual que permite aislar las propiedades esenciales de un objeto o proceso computacional, ocultando los detalles concretos de implementación técnica y gestión de hardware [Louden, 2004, p. 11]. El control de la complejidad se logra organizando las abstracciones en dos categorías principales, estratificadas en tres niveles progresivos:Abstracción de datos: Permite estructurar y aislar la representación física de los valores computables en memoria [Louden, 2004, pp. 257-260]:Básica: Oculta la codificación interna de bits en la memoria de la máquina. Engloba el concepto de variable y los tipos primitivos nativos con sus operaciones escalares asociadas.Estructurada: Modela colecciones de valores agregados y las relaciones que existen entre ellos. Oculta la disposición física de la memoria contigua o dispersa (ej. registros, conjuntos, listas, secuencias indexadas).Unitaria: Encapsula la definición de un conjunto de datos junto con el repertorio completo de operaciones primitivas que los gobiernan, ocultando la estructura interna bajo una interfaz de acceso (Tipos Abstractos de Datos, módulos, clases).Abstracción de control: Modela y organiza el flujo secuencial y alternativo de la ejecución del cómputo [Louden, 2004, pp. 11-13]:Básica: Sentencias elementales que encapsulan secuencias de microinstrucciones de máquina en una única orden lógica declarativa (ej. la sentencia de asignación o expresiones de cómputo).Estructurada: Construcciones lingüísticas que alteran el flujo del contador del programa de manera gobernada mediante condiciones lógicas (guardias), dividiendo el flujo en bloques anidados con puntos definidos de entrada y salida (bifurcaciones if-else e iteraciones while, for).Unitaria: Mecanismos que permiten tratar un bloque complejo de control de sentencias como una unidad aislada, invocable y parametrizable desde diversos puntos del sistema (procedimientos, métodos, funciones).2.3.1 Abstracciones de AlephDesde la perspectiva funcional del programador, Aleph provee los siguientes niveles de abstracción:NivelAbstracciones de datos de AlephAbstracciones de control de AlephBásicoVariables tipadas; tipos primitivos de datos (boolean, literales de carácter); literales escalares; operador de asignación (=).Evaluación de expresiones atómicas; evaluación del operador relacional de pertenencia conjuntista (in).EstructuradoColecciones dinámicas agregadas: conjuntos (set) y listas indexadas (list), con soporte para anidamiento arbitrario. Operaciones algebraicas nativas sobre colecciones: unión de conjuntos (union), asignación de conjunto vacío (empty), e inserción (insert, append).Estructuras de bifurcación condicional (if-then-else); estructuras de control iterativo gobernadas por guardia (while); iteración abstracta sobre colecciones (for..in).UnitarioNo aplica explícitamente: Aleph no provee mecanismos de encapsulamiento orientado a objetos o declaración de TADs por el usuario en tiempo de diseño.Definición de funciones reutilizables con paso de parámetros formales y retorno de valores (list nombre(params): ... return ... end).Nota taxonómica: La operación union se clasifica rigurosamente como una abstracción de datos estructurada y no de control, debido a que su evaluación transforma y combina operandos conjuntistas produciendo un nuevo valor agregado de tipo set, sin desviar por sí misma el flujo de ejecución del programa [Louden, 2004, p. 260].2.4 Dominio de programación de AlephCon base en la taxonomía de dominios de aplicación de lenguajes establecida por Sebesta [2012, pp. 6-8], Aleph se localiza en el dominio de las aplicaciones de Inteligencia Artificial (IA) y Procesamiento Simbólico.Sebesta caracteriza al dominio científico clásico por la ejecución masiva de cómputos numéricos matriciales y aritmética de punto flotante de alta precisión (propio de lenguajes como Fortran o C). En contraposición, el procesamiento simbólico y de inteligencia artificial se define por manipular símbolos, caracteres, relaciones lógicas y estructuras complejas no ordenadas o grafos, requiriendo operaciones eficientes sobre listas enlazadas dinámicas y conjuntos matemáticos más que transformaciones sobre matrices numéricas densas [Sebesta, 2012, p. 7].Aleph no realiza cálculo numérico continuo; su propósito es modelar entidades discretas, relaciones formales y reglas estructurales mediante conjuntos, tuplas y listas. Por ende, aunque los autómatas y lenguajes formales actúan como el caso de estudio metodológico, la naturaleza estructural del cómputo en Aleph coincide con las directrices del procesamiento simbólico.3. Diseño e Implementación3.1 SentenciasEn el diseño conceptual de lenguajes imperativos, una sentencia (statement) constituye la unidad sintáctica ejecutable mínima que especifica una acción o paso discreto en el proceso de cómputo de la máquina abstracta [Sebesta, 2012, p. 113]. A diferencia de las expresiones —construcciones que se evalúan formalmente para retornar un valor de cómputo—, las sentencias se ejecutan con el objetivo fundamental de generar efectos de estado en el entorno (como modificar la Tabla de Símbolos) o alterar el flujo secuencial del control.La organización del flujo de ejecución en Aleph se rige por las tres estructuras fundamentales de la programación estructurada [Sebesta, 2012, pp. 331-335]:Composición (Secuenciación Implícita): Secuencia lineal en la cual las instrucciones se ejecutan estrictamente una tras otra en el orden textual del código fuente, garantizando invariantes de estado secuenciales.Alternancia (Selección): Construcción que evalúa una condición lógica o guardia booleana para seleccionar qué bloque de ejecución debe activarse y cuál omitirse (if-else).Iteración (Repetición): Construcción que posibilita repetir un cuerpo de sentencias de manera controlada bajo la vigencia de una condición de parada (while, for), asegurando que cada bloque de control posea un punto único de acceso y de culminación.3.1.1 Un primer diseño de sentencias de AlephA continuación se presenta el primer diseño algorítmico estructurado para la conversión de un AFND a AFD expresado en el lenguaje Aleph, el cual opera exclusivamente mediante las abstracciones generales del lenguaje:Plaintextlist afnd2afd(list A):
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
3.2 Reconocimiento de patrones. Tokens, Lexemas y PatronesEn la fase inicial de análisis léxico, el procesamiento de las unidades mínimas del lenguaje exige distinguir tres conceptos teóricos complementarios [Sebesta, 2012, pp. 114-115]:Lexema: Secuencia textual concreta de caracteres en el código fuente que coincide formalmente con el patrón asignado a una categoría léxica. Constituye la instancia física leída de la entrada (por ejemplo, while, Q_B, ==, 17).Token: Símbolo o categoría sintáctica abstracta que identifica el tipo funcional al que pertenece el lexema. Es la unidad que consume el analizador sintáctico (por ejemplo, IDENTIFICADOR, OPERADOR_ASIGNACION, PALABRA_CLAVE_WHILE). Desde el punto de vista sintáctico, un token actúa como un símbolo terminal dentro de la gramática formal.Patrón: Regla de especificación formal que determina el universo de cadenas de caracteres que pertenecen a un token específico [Sebesta, 2012, p. 115]. Los patrones se formalizan mediante expresiones regulares que el generador Flex compila a autómatas finitos deterministas capaces de resolver la correspondencia léxica en tiempo lineal:$$\text{Identificador} = [A-Z][a-zA-Z0-9\_]^*$$El analizador léxico procesa el texto plano aplicando las expresiones regulares (patrones); en cuanto identifica un lexema que satisface la regla, genera y entrega el token correspondiente a la rutina de análisis sintáctico.3.2.1 Tokens y lexemas de AlephLexema / PatrónToken (Categoría)DescripciónsetKW_SETTipo de dato / Palabra reservada: conjunto no duplicado.listKW_LISTTipo de dato / Palabra reservada: secuencia o lista ordenada.booleanKW_BOOLEANTipo de dato / Palabra reservada: valor de verdad (true/false).whileKW_WHILESentencia de control: inicio de iteración condicionada.forKW_FORSentencia de control: iteración sobre elementos de colección.ifKW_IFSentencia de control: bifurcación condicional de flujo.elseKW_ELSESentencia de control: rama alternativa de selección.doDELIM_DODelimitador de bloque: apertura de cuerpo de control o bucle.endDELIM_ENDDelimitador de bloque: cierre estructural de bloques o funciones.breakKW_BREAKSentencia de control: interrupción forzada de iteración.returnKW_RETURNSentencia de control: retorno explícito de valor de función.letKW_LETDeclaración: desestructuración o nombramiento formal de componentes.inOP_INOperador relacional: evaluación de pertenencia conjuntista o recorrido.whereKW_WHERESentencia de control: cláusula de filtrado condicional en bucles.emptyKW_EMPTYLiteral / Constante: representa el conjunto o lista sin elementos.unionOP_UNIONOperador de colección: cálculo de la unión algebraica entre conjuntos.anyKW_ANYFunción primitiva: verificación existencial en colecciones.appendKW_APPENDFunción primitiva: inserción de elemento al final de una lista.insertKW_INSERTFunción primitiva: incorporación de un elemento a un conjunto.:DELIM_COLONSímbolo especial: delimitador de apertura del bloque de función.=OP_ASSIGNOperador básico: asignación destructiva de valor a identificador.==OP_EQUALOperador relacional: prueba de igualdad estructural estricta.!=OP_NOT_EQUALOperador relacional: prueba de desigualdad estructural estricta.;DELIM_SEMICOLONDelimitador sintáctico: terminación explícita de sentencia.,DELIM_COMMASeparador sintáctico: división posicional en listas, conjuntos o parámetros.(DELIM_LPARENDelimitador sintáctico: apertura de tuplas, condiciones o llamadas.)DELIM_RPARENDelimitador sintáctico: cierre de tuplas, condiciones o llamadas.{DELIM_LBRACEDelimitador sintáctico: apertura de especificación literal de conjunto.}DELIM_RBRACEDelimitador sintáctico: cierre de especificación literal de conjunto.[DELIM_LBRACKETDelimitador sintáctico: apertura de especificación literal de lista.]DELIM_RBRACKETDelimitador sintáctico: cierre de especificación literal de lista.true / falseLIT_BOOLEANLiterales primitivos: constantes booleanas lógicas.'0', '1', 'a'LIT_CHARLiterales primitivos: caracteres constantes atómicos.[A-Z][a-zA-Z0-9_]*IDENTIFIERIdentificadores: nombres de variables definidos por el usuario iniciados en mayúscula.