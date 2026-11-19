/* Ejemplo fb2-4 concordancia de texto*/
%option noyywrap nodefault yylineno case-insensitive
/* La tabla de Símbolos */
%{
#include <string.h>

/*estrucutra de los elementos de la tabla de símbolos*/
struct symbol { /* una palabra (idetntificador) */
    char* name;
    struct ref* reflist;
};
struct ref {
    struct ref* next;
    char* filename;
    int flags;
    int lineno;
};

/* Definición de una Tabla de Símbolos simple de tamaño fijo*/
#define NHASH 9997
struct symbol symtab[NHASH]; /*vector cuyos elementos son de tipo struct symbol*/

/*función lookup que recibe como parámetro un puntero a char y retorna un puntero 
a la estructura symbol*/
struct symbol* lookup(char*); 

/*función addref de cuatro argumentos, un entero dos punteros a char y por 
último otro entero*/
void addref(int, char*, char*,int);

/*función para imprimir lista de referencias de cada palabra*/
void printrefs();

/* variable global que almacena el nombre actual del archivo de entrada */
char *curfilename; 
%}

%%
"a" |
"de" |
"de" |
"del" |
"e" |
"el" |
"en" |
"es" |
"esa" |
"eso" |
"esta" |
"estas" |
"este" |
"estos" |
"la" |
"las" |
"lo" |
"los" |
"no" |
"o" |
"para" |
"pero" |
"por" |
"que" |
"se" |
"si" |
"u" |
"un" |
"una" |
"unas" |
"unos" |
"y"   /*ignorar todo hasta aquí*/
[a-z]+ { addref(yylineno, curfilename, yytext, 0); }
.|\n /* ignorar todo lo demás */
%%
/* módulo principal de del programa de concordancia*/
int main(argc, argv)
int argc;
char **argv;
{
    int i;
    if(argc < 2) { /* leer desde la entrada estándar */
        curfilename = "(stdin)";
        yylineno = 1;
        yylex();
    } else
    for(i = 1; i < argc; i++) {
        FILE *f = fopen(argv[i], "r");
        if(!f) {
            perror(argv[1]);
            return (1);
        }
        curfilename = argv[i]; /* for addref */
        yyrestart(f);
        yylineno = 1;
        yylex();
        fclose(f);
    }
    printrefs();
}
/*FUNCIÓN HASH*/
/*La función hash también es bastante simple: para cada caracter, multiplique el hash 
anterior por 9 y luego realizar xor al nuevo caracter, haciendo toda la aritmética sin 
signo, puesto que así se ignora el desbordamiento. 
La operación XOR o disyunción lógica retorna 0 si ambos bits son iguales y 1 si son 
diferentes. 
Se podría utilizar cualquier otra operación que permita asociar a la cadena un número*/
static unsigned symhash( {
    unsigned int hash = 0;
    unsigned c;
    
    while (c = *sym++) 
        hash = hash*9 ^ c; /*El operador XOR binario o disyunción lógica en C es ^ */
    return hash;
}
/*FUNCIÓN LOOK UP*/
/*Toma una cadena y devuelve la dirección de la entrada a la tabla para ese identificador, creando
una nueva entrada si aún no hay una. La técnica de búsqueda se conoce como hashing con
prueba lineal. Utiliza una función hash para convertir la cadena (identificador) en un número de 
entrada en para la tabla, luego verifica el estado de la entrada y, si ya está tomada por un símbolo 
diferente, busca linealmente hasta encuentrar una entrada libre.*/
struct symbol* lookup(char* sym) {
    /*obtine la dirección de memoria de la posición del vector obtenida con la función symhash. Saca
    módulo NHASH para no pasarse del tamaño del vector. 
    Se usa la variable sp para recorrer el vector, moviendo punteros y no índices*/
    struct symbol* sp = &symtab[symhash(sym)%NHASH]; 
    int scount = NHASH; /* how many have we looked at */
    
    while(--scount >= 0) {
        /*Si la cadena está ya almacenada en la tabla de símbolos, 
        rertorna el registro almacenado en esa posición*/
        if(sp->name && !strcmp(sp->name, sym)) /*strcmp=0 (falso) cuando son iguales*/
            return sp; /*se produce un break de la función*/
        /*Si no está ocupada la entrada, se agrega la nueva cadena*/
        if(!sp->name) { /* new entry */
            sp->name = strdup(sym); /*realiza una copia de la cadena (nuevo puntero)*/
            sp->reflist = 0;
            return sp; /*se produce un break de la función*/
        }
        /*Si la entrada está ocupada, mira la siguiente posición del vector.
        symtab almacena la dirección de la posición cero del vector. 
        NNASH es la cantidad de posiciones del vector. Al sumar symtab con NHASH se obtiene la dirección
        de memoria de la última posición del vector
        ++sp pasa al siguiente. Si al aumentar en uno se pasa del tamaño del vector, volver al primero,
        es un recorrido circular*/
        if(++sp >= symtab+NHASH) 
            sp = symtab; /* try the next entry */
    }
    /*Si se recorre todas la posiciones del vector y no se pudo encontrar lugar para almacenar la 
    nueva entrada, la tabla está llena, se muestra cartel de tabla desbordada*/
    fputs("symbol table overflow\n", stderr);
    abort(); /*termina la ejecución del programa (del scanner)*/
}

/*addref es  llamada desde el interior del escáner para agregar una referencia a una palabra en 
particular; se implementa como una lista enlazada de punteros a estructuras asociada a cada símbolo. 
No agrega una referencia si el símbolo ya tiene una referencia al mismo número de línea y nombre de 
archivo. No se hace una copia del nombre del archivo, porque sabemos que el llamador entrega una 
cadena que no cambiará. Tampoco se copia la palabra, ya que la lookup se encargará de eso si es 
necesario.*/
void addref(int lineno, char *filename, char *word, int flags){
    struct ref *r;
    struct symbol *sp = lookup(word);
    /*No se deben hacer referencias con el mismo archivo y número de línea, es decir duplicar
    las referencias*/
    if (sp->reflist && sp->reflist->lineno == lineno && sp->reflist->filename == filename) 
        return;
    r = malloc(sizeof(struct ref));
    if(!r) { /*verificar que hay espacio de memoria*/
        fputs("out of space\n", stderr); 
        abort(); 
    }
    /*la nueva referencia se agrega al inicio de la lista*/ 
    r->next = sp->reflist;
    r->filename = filename;
    r->lineno = lineno;
    r->flags = flags;
    sp->reflist = r;
}

/* functón auxiliar para ordenar */
static int symcompare(const void *xa, const void *xb) {
    const struct symbol *a = xa;
    const struct symbol *b = xb;
    if(!a->name) {
        if(!b->name) return 0; /* ambas cadenas vacías */
            return 1; /* Poner cadenas vacías al final*/
    }
    if(!b->name) return -1;
    return strcmp(a->name, b->name);
}
void printrefs(){
    struct symbol *sp;
    /*función qsort de C ordena un vector
    qsort(void *base, size_t nitems, size_t size, int (*compar)(const void *, const void*))
    base − puntero al primer elemento del vector a ordenar
    nitems − Cantidad de elementos del vector
    size − Tamaño en bytes de los elementos del vector
    compar − Función para comparar elementos*/
    qsort(symtab, NHASH, sizeof(struct symbol), symcompare); /* ordenar la tabla de símbolos*/
    for(sp = symtab; sp->name && sp < symtab+NHASH; sp++) {
        char *prevfn = NULL; /* último nombre de archivo impreso, para evitar duplicaciones*/
        /*como las refrencias se agregan al inicio de la lista, se invierte la lista para mostrar
        las referencias en orden de aparición*/
        struct ref *rp = sp->reflist;
        struct ref *rpp = 0; /* referencia anterior */
        struct ref *rpn;     /* siguiente referencia */
        do {
            rpn = rp->next;  /*se resguarda el siguiente*/     
            rp->next = rpp;  /*se da vuelta el enlace*/
            /*preparar la siguiente iteración*/
            rpp = rp;        /*el previo es el actual*/
            rp = rpn;        /*el actual es el siguiente*/    
        } while(rp); /*el último registro tiene next=0*/
        /* imprimir los identificadores (palabras) y sus referencias*/
        printf("%10s", sp->name);
        /*recorrer cada una de las referencias, desde la primera referencia hasta que sea cero*/ 
        /*del ciclo anterior rpp finaliza apuntando al primer registro de la lista*/
        for(rp = rpp; rp; rp = rp->next) { 
            if(rp->filename == prevfn) {
                printf(" %d", rp->lineno);
            } else {
                printf(" %s:%d", rp->filename, rp->lineno);
                prevfn = rp->filename;
            }
        }
        printf("\n");
    }
}