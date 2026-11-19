/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* funciones de ayuda para la calculadora */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "fb3-2.h"

/* tabla de símbolos */
/* aplicar hash a un símbolo */
static unsigned symhash(char *sym){
    unsigned int hash = 0;
    unsigned c;
    
    while(c = *sym++) hash = hash*9 ^ c;
    
    return hash;
}

struct symbol *lookup(char *sym){
    struct symbol *sp = &symtab[symhash(sym)%NHASH];
    int scount = NHASH;
    
    while(--scount >= 0){
        if(sp->name && !strcmp(sp->name, sym)) 
            return sp;
        
        if(!sp->name){
            sp->name = strdup(sym);
            sp->value = 0;
            sp->func = NULL;
            sp->syms = NULL;
            return sp;
        }
        
        if(++sp >= symtab+NHASH)    /* prueba con la siguiente entrada */
            sp = symtab;
    }
    yyerror("la tabla de símbolos está agotada\n");
    abort();
}

/* creación de nodos del árbol de sintaxis abstracta */

struct ast *newast(int nodetype, struct ast *l, struct ast *r){
    struct ast *a = malloc(sizeof(struct ast));
    
    if(!a){
        yyerror("no hay espacio");
        exit(0);
    }
    
    a->nodetype = nodetype;
    a->l = l;
    a->r = r;
    return a;
}

struct ast *newnum(double d){
    struct numval *a = malloc(sizeof(struct numval));
    
    if(!a){
        yyerror("no hay espacio");
        exit(0);
    }
    
    a->nodetype = 'K';
    a->number = d;
    return(struct ast *)a;
}

struct ast *newcmp(int cmptype, struct ast *l, struct ast *r){
    struct ast *a = malloc(sizeof(struct ast));
    
    if(!a){
        yyerror("no hay espacio");
        exit(0);
    }
    
    a->nodetype = '0' + cmptype;
    a->l = l;
    a->r = r;
    return a;
}

struct ast *newfunc(int functype, struct ast *l){
    struct fncall *a = malloc(sizeof(struct fncall));
    
    if(!a){
        yyerror("no hay espacio");
        exit(0);
    }
    
    a->nodetype = 'F';
    a->l = l;
    a->functype = functype;
    return (struct ast *)a;
}

struct ast *newcall(struct symbol *s, struct ast *l){
    struct ufncall *a = malloc(sizeof(struct ufncall));
    
    if(!a){
        yyerror("no hay espacio");
        exit(0);
    }
    
    a->nodetype = 'C';
    a->l = l;
    a->s = s;
    return (struct ast *)a;
}

struct ast *newref(struct symbol *s){
    struct symref *a = malloc(sizeof(struct symref));
    
    if(!a){
        yyerror("no hay espacio");
        exit(0);
    }
    
    a->nodetype = 'N';
    a->s = s;
    return (struct ast *)a;
}

struct ast *newasgn(struct symbol *s, struct ast *v){
    struct symasgn *a = malloc(sizeof(struct symasgn));
    
    if(!a){
        yyerror("no hay espacio");
        exit(0);
    }
    
    a->nodetype = '=';
    a->s = s;
    a->v = v;
    return (struct ast *)a;
}

struct ast *newflow(int nodetype, struct ast *cond, 
                    struct ast *tl, struct ast *el){
    struct flow *a = malloc(sizeof(struct flow));
    
    if(!a){
        yyerror("no hay espacio");
        exit(0);
    }
    
    a->nodetype = nodetype;
    a->cond = cond;
    a->tl = tl;
    a->el = el;
    return (struct ast *)a;
}

/* liberar un árbol AST */
void treefree(struct ast *a){
    switch(a->nodetype){
        case '+':
        case '-':
        case '*':
        case '/':
        case '1': case '2': case '3': case '4': case '5': case '6':
        case 'L':
            treefree(a->r);
            
        case '|':
        case 'M': case 'C': case 'F':
            treefree(a->l);
            
        case 'K': case 'N':
            break;
            
        case '=':
            free(((struct symasgn*)a)->v);
            break;
            
        case 'I': case 'W':
            free(((struct flow *)a)->cond);
            if(((struct flow *)a)->tl)
                treefree(((struct flow *)a)->tl);
            if(((struct flow *)a)->el)
                treefree(((struct flow *)a)->el);
            break;
        default: 
            printf("error interno: libera un nodo incorrecto %c\n", a->nodetype);
    }
    free(a);    /* siempre liberar el nodo actual */
}

struct symlist *newsymlist(struct symbol *sym, struct symlist *next){
    struct symlist *sl = malloc(sizeof(struct symlist));
    
    if(!sl){
        yyerror("no hay espacio");
        exit(0);
    }
    
    sl->sym = sym;
    sl->next = next;
    return sl;
}

/* liberar una lista de símbolos */
void symlistfree(struct symlist *sl){
    struct symlist *nsl;
    
    while(sl){
        nsl = sl->next;
        free(sl);
        sl = nsl;
    }
}

static double callbuiltin(struct fncall *);
static double calluser(struct ufncall *);

double eval(struct ast *a){
    double v;
    
    if(!a){
        yyerror("error interno, evaluación nula");
        return 0.0;
    }
    
    switch(a->nodetype){
        /* constante */
        case 'K': v = ((struct numval *)a)->number; break;
        
        /* referencia */
        case 'N': v = ((struct symref *)a)->s->value; break;
        
        /* asignación */
        case '=': v = ((struct symasgn *)a)->s->value =
                eval(((struct symasgn *)a)->v); break;
        
        /* expresiones */
        case '+': v = eval(a->l) + eval(a->r); break;
        case '-': v = eval(a->l) - eval(a->r); break;
        case '*': v = eval(a->l) * eval(a->r); break;
        case '/': v = eval(a->l) / eval(a->r); break;
        case '|': v = fabs(eval(a->l)); break;
        case 'M': v = -eval(a->l); break;
        
        /* comparaciones */
        case '1': v = (eval(a->l) > eval(a->r)) ? 1 : 0; break;
        case '2': v = (eval(a->l) < eval(a->r)) ? 1 : 0; break;
        case '3': v = (eval(a->l) != eval(a->r)) ? 1 : 0; break;
        case '4': v = (eval(a->l) == eval(a->r)) ? 1 : 0; break;
        case '5': v = (eval(a->l) >= eval(a->r)) ? 1 : 0; break;
        case '6': v = (eval(a->l) <= eval(a->r)) ? 1 : 0; break;
        
        /* flujo de control */
        /* las expresiones nulas están permitidas en la gramática
         * por lo tanto se deben chequear
         */
        
        /* if - then - else */
        case 'I':
            if(eval(((struct flow *)a)->cond) != 0){
                if(((struct flow *)a)->tl){
                    v = eval(((struct flow *)a)->tl);
                }else
                    v = 0.0;        /* valor por defecto */
            }else{
                if(((struct flow *)a)->el){
                    v = eval(((struct flow *)a)->el);
                }else
                    v = 0.0;
            }
            break;
            
        /* while - do */
        case 'W':
            v = 0.0;        /* valor por defecto */
            
            if(((struct flow *)a)->tl){
                while(eval(((struct flow *)a)->cond) != 0)
                    v = eval(((struct flow *)a)->tl);
            }
            break;
            
        /* lista de sentencias */
        case 'L': eval(a->l); v = eval(a->r); break;
        
        case 'F': v = callbuiltin((struct fncall *)a); break;
        
        case 'C': v = calluser((struct ufncall *)a); break;
        
        default: printf("error interno: nodo incorrecto %c\n", a->nodetype);
    }
    return v;
}

static double callbuiltin(struct fncall *f){
    enum bifs functype = f->functype;
    double v = eval(f->l);
    
    switch(functype){
        case B_sqrt:
            return sqrt(v);
        case B_exp:
            return exp(v);
        case B_log:
            return log(v);
        case B_print:
            printf("= %4.4g\n", v);
            return v;
        default:
            yyerror("función incorporada desconocida %d", functype);
            return 0.0;
    }
}

/* definir una función */
void dodef(struct symbol *name, struct symlist *syms, struct ast *func){
    if(name->syms) symlistfree(name->syms);
    if(name->func) treefree(name->func);
    name->syms = syms;
    name->func = func;
}

static double calluser(struct ufncall *f){
    struct symbol *fn = f->s;   /* nombre de la función */
    struct symlist *sl;         /* argumentos formales */
    struct ast *args = f->l;    /* argumentos actuales */
    double *oldval, *newval;    /* valores de argumentos salvados */
    double v;
    int nargs;
    int i;
    
    if(!fn->func){
        yyerror("llamada a una función indefinida: %s", fn->name);
        return 0;
    }
    
    /* cuenta los argumentos */
    sl = fn->syms;
    for(nargs = 0; sl; sl = sl->next)
        nargs++;
    
    /* preparación para salvarlos */
    oldval = (double *)malloc(nargs * sizeof(double));
    newval = (double *)malloc(nargs * sizeof(double));
    if(!oldval || !newval){
        yyerror("No hay lugar en %s", fn->name); 
        return 0.0;
    }
    
    /* evalúa los argumentos */
    for(i=0; i<nargs; i++){
        if(!args){
            yyerror("faltan argumentos en la llamada a %s", fn->name);
            free(oldval);
            free(newval);
            return 0.0;
        }
        
        if(args->nodetype=='L'){    /* si es un nodo lista */
            newval[i] = eval(args->l);
            args = args->r;
        }else{                      /* si es el final de la lista */
            newval[i] = eval(args);
            args = NULL;
        }
    }
    
    /* salva los viejos valores de los argumentos formales, 
     * asigna los nuevos nodos
     */
    sl = fn->syms;
    for(i=0; i<nargs; i++){
        struct symbol* s = sl->sym;
        
        oldval[i] = s->value;
        s->value = newval[i];
        sl = sl->next;
    }
    
    free(newval);
    
    /* evalúa la función */
    v = eval(fn->func);
    
    /* pone los valores reales de los args formales de nuevo */
    sl = fn->syms;
    for(i=0; i<nargs; i++){
        struct symbol *s = sl->sym;
        
        s->value = oldval[i];
        sl = sl->next;
    }
    
    free(oldval);
    return v;
}

