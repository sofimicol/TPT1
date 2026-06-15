#ifndef TAD_TABLAMAPEO_H
#define TAD_TABLAMAPEO_H
typedef struct {
	Tdata estados_originales[MAX_ESTADOS]; 
	int cantidad_actual;                   
} TablaMapeo;

void inicializar_tabla(TablaMapeo*);
int renombrar_estado(TablaMapeo*, Tdata);
#endif
