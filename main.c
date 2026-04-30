#include "TAD_ast.h"
#include "TAD_set.h"
#include "TAD_lista.h"
#include "TAD_str.h"

int main() {
	
	printf("===== TEST TOTAL DE ESTRUCTURAS =====\n\n");
	
	// =========================================================
	// STRINGS
	// =========================================================
	Tdata s1 = create_str(); s1->string = load2("1");
	Tdata s2 = create_str(); s2->string = load2("2");
	Tdata s3 = create_str(); s3->string = load2("3");
	Tdata s4 = create_str(); s4->string = load2("4");
	Tdata s5 = create_str(); s5->string = load2("5");
	Tdata s6 = create_str(); s6->string = load2("A");
	Tdata s7 = create_str(); s7->string = load2("B");
	Tdata s8 = create_str(); s8->string = load2("C");
	Tdata s9 = create_str(); s9->string = load2("D");
	
	// =========================================================
	// LISTAS
	// =========================================================
	Tdata L1 = create_list(); append(&L1, s1); append(&L1, s2);
	Tdata L2 = create_list(); append(&L2, s3); append(&L2, s4);
	Tdata L3 = create_list(); append(&L3, s6); append(&L3, s7); append(&L3, s8);append(&L3, s9);
	Tdata Ln = create_list();
	append(&Ln, L1);
	append(&Ln, L2);
	append(&Ln, L3);
	
	printf("L1 = "); mostrarArbol(L1); printf("\n");
	printf("L2 = "); mostrarArbol(L2); printf("\n");
	printf("L3 = "); mostrarArbol(L3); printf("\n");
	printf("Lista de lista = "); mostrarArbol(Ln); printf("\n\n");
	
	// =========================================================
	// LISTAS OPERACIONES
	// =========================================================
	Tdata Lconcat = concatena_list(L1, L2);
	printf("concat(L1, L2) = "); mostrarArbol(Lconcat); printf("\n");
	Tdata Lconcat1 = concatena_list(L1, Ln);
	printf("concat(L1, Ln) = "); mostrarArbol(Lconcat1); printf("\n");
	
	printf("search(L1, 1) =");
	if(search(L1, s1)==1){
		printf("Pertenece");	
	}
	else{
		printf("No pertenece");
	}
	printf("\n");
	// =========================================================
	// CONJUNTOS
	// =========================================================
	printf("\n-------------CONJUNTOS-----------------\n");
	Tdata A = create_set();
	insert_set(&A, s1);
	//insert_set(&A, s2);
	insert_set(&A, L1);
	insert_set(&A, s1); // duplicado
	
	Tdata B = create_set();
	insert_set(&B, s1);
	insert_set(&B, s3);
	insert_set(&B, L1);
	printf("A = "); mostrarArbol(A); printf("\n");
	printf("B = "); mostrarArbol(B); printf("\n\n");
	// =========================================================
	// OPERACIONES CONJUNTOS
	// =========================================================
	Tdata U = unionset(A, B);
	printf("A U B = "); mostrarArbol(U); printf("\n");
	
	Tdata I = intersection_set(A, B);
	printf("A n B = "); mostrarArbol(I); printf("\n");
	
	Tdata D = difference_set(A, B);
	printf("A - B = "); mostrarArbol(D); printf("\n\n");
	
	// =========================================================
	// SUBSET / EQUALS
	// =========================================================
	printf("A ? B = %d\n", subset(A, B));
	printf("B ? A = %d\n", subset(B, A));
	
	// =========================================================
	// VACÍO
	// =========================================================
	Tdata V = create_set();
	
	printf("Vacio= "); mostrarArbol(V); printf("\n");
	
	printf("Vacio ? A = %d\n", subset(V, A));
	printf("A ? Vacio = %d\n", subset(A, V));
	
	printf("A U V = "); mostrarArbol(unionset(A, V)); printf("\n");
	printf("A n V = "); mostrarArbol(intersection_set(A, V)); printf("\n\n");
	
	// =========================================================
	// PRODUCTO CARTESIANO
	// =========================================================
	printf("A x B = ");
	mostrarArbol(prod_cartesiano(A, B));
	printf("\n\n");
	
	// =========================================================
	// COPIAS
	// =========================================================
	Tdata original = create_set();
	insert_set(&original, s1);
	
	Tdata copia = copy_ast(original);
	
	printf("Original = "); mostrarArbol(original); printf("\n");
	printf("Copia = "); mostrarArbol(copia); printf("\n");
	
	insert_set(&original, s2);
	
	printf("Original modificado = "); mostrarArbol(original); printf("\n");
	printf("Copia = "); mostrarArbol(copia); printf("\n\n");
	
	// =========================================================
	// IGUALDAD ESTRUCTURAL
	// =========================================================
	Tdata set1 = create_set();
	Tdata set2 = create_set();
	
	Tdata x1 = create_str(); x1->string = load2("2");
	Tdata x2 = create_str(); x2->string = load2("2");
	
	insert_set(&set1, x1);
	insert_set(&set2, x2);
	
	printf("equals(set1, set2) = %d\n\n", equals_node(set1, set2));
	
	// =========================================================
	// CONJUNTOS ANIDADOS
	// =========================================================
	Tdata subA = create_set();
	insert_set(&subA, s1);
	insert_set(&subA, s2);
	
	Tdata subB = create_set();
	insert_set(&subB, s2);
	insert_set(&subB, s3);
	
	Tdata setDeSets = create_set();
	insert_set(&setDeSets, subA);
	insert_set(&setDeSets, subB);
	
	printf("setDeSets = "); mostrarArbol(setDeSets); printf("\n");
	
	// =========================================================
	// MIXTOS
	// =========================================================
	Tdata mixto = create_set();
	insert_set(&mixto, L1);
	insert_set(&mixto, subA);
	insert_set(&mixto, s4);
	
	printf("mixto = "); mostrarArbol(mixto); printf("\n");
	
	printf("belongs(mixto, L1) = %d\n", belongs(mixto, L1));
	printf("belongs(mixto, subA) = %d\n\n", belongs(mixto, subA));
	
	// =========================================================
	// MIXTO OPERACIONES
	// =========================================================
	Tdata otroMixto = create_set();
	insert_set(&otroMixto, subA);
	insert_set(&otroMixto, s5);
	
	printf("mixto n otroMixto = ");
	mostrarArbol(intersection_set(mixto, otroMixto));
	printf("\n");
	
	printf("mixto - otroMixto = ");
	mostrarArbol(difference_set(mixto, otroMixto));
	printf("\n\n");
	
	// =========================================================
	// DUPLICADOS ESTRUCTURALES
	// =========================================================
	Tdata dupTest = create_set();
	Tdata subA_copy = copy_ast(subA);
	
	insert_set(&dupTest, subA);
	insert_set(&dupTest, subA_copy);
	
	printf("dupTest = "); mostrarArbol(dupTest); printf("\n\n");
	
	// =========================================================
	// PRODUCTO CARTESIANO COMPLEJO
	// =========================================================
	printf("setDeSets x mixto = ");
	mostrarArbol(prod_cartesiano(setDeSets, mixto));
	printf("\n\n");
	
	// =========================================================
	// LISTAS PROFUNDAS
	// =========================================================
	Tdata deepList = create_list();
	append(&deepList, L1);
	append(&deepList, Ln);
	
	printf("deepList = "); mostrarArbol(deepList); printf("\n");
	printf("search(deepList, L1) =");
	if( search(deepList, L1)){
		printf("Pertenece");
	}
	else{
		printf("No pertenece");
	}
	
	return 0;
}
