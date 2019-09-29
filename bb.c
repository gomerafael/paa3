#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//ÁRVORE BINÁRIA DE BUSCA
//num = chave a ser inserida, sad = ponteiro para direita
//sae = ponteiro para a esquerda

int compConsulta = 0, compConstrucao = 0;

typedef struct tree {

	int num;
	struct tree* sad;
	struct tree* sae;
} ArvoreBB; 

//Cria árvore
ArvoreBB* criaArv() {

	return NULL;
}

//Verifica árvore vazia
int arvoreVazia(ArvoreBB* arv) {
	//Retorna 1 se a árvore for vazia e 0 caso contrário
	return arv == NULL;
}

//Insere uma chave na árvore
void inserirArv(ArvoreBB** arv, int num) {
	
	if(*arv == NULL) {
		*arv = (ArvoreBB*) malloc(sizeof(ArvoreBB));//Aloca memória 
		(*arv)->sae = NULL; //Subárvore esquerda NULL
		(*arv)->sad = NULL; //Subárvore direita NULL
		(*arv)->num = num; //Armazena a informação
	} else {
		if(num < (*arv)->num) { //Se o número é menor então vai para a esquerda
			compConstrucao += 1;
			//Percorre pela subárvore a esquerda
			inserirArv(&(*arv)->sae, num);
		}
		if(num > (*arv)->num) { //Se o número é maior então vai para a direita
			compConstrucao += 1;
			//Percorre pela subárvore a direita
			inserirArv(&(*arv)->sad, num);
		}	
	}
}

//Imprime os elementos de forma recursiva e em pré ordem
void imprimeArv(ArvoreBB* arv) {

	printf("<");
	if(!arvoreVazia(arv)) { //Se a árvore não for vazia
		printf("%d ", arv->num); //Mostra raiz
		imprimeArv(arv->sae); //Imprime a sae
		imprimeArv(arv->sad); //Imprime a sad
	}
	printf(">");
}

//Verifica se o elemento está na árvore
int consultaArv(ArvoreBB* arv, int num) {

	if(arvoreVazia(arv)) { //Se a árvore for vazia retorna 0
		return 0;
	}
	//O operador lógico || interrompe a busca quando o elemento for encontrado
	
	//printf("%d----%d  \n", arv->num, num);
	compConsulta += 1;
	if(num > arv->num) {
		consultaArv(arv->sad, num);
	}
	if(num < arv->num) {
		consultaArv(arv->sae, num);
	}
	return arv->num==num; //|| consultaArv(arv->sae, num) || consultaArv(arv->sad, num);
	
}

int main() {

	clock_t tConstrucao, tConsulta;
	int c, d;
	FILE *arch;

	/*********************************************************************
	**********************************************************************
	**INÍCIO DA CONSTRUÇÃO NA ÁRVORE COM BASE NOS ARQUIVOS DE CONSTRUÇÃO**
	**********************************************************************
	*********************************************************************/

	arch = fopen("Construir/Conjunto2/250000.txt","r");
	if(arch == NULL){
		printf("Erro na abertura do arquivo de construção!\n");
		system("pause");
		exit(1);
	}

	ArvoreBB* arv = criaArv();

	tConstrucao = clock();
	
	while(fscanf(arch, "%d", &c) != EOF) {
		//printf("%d ", c);
		inserirArv(&arv, c);
	}
	tConstrucao = clock() - tConstrucao;

	printf("\n");
	//imprimeArv(arv);
	printf("Tempo de construção BB: %lf", ((double)tConstrucao)/((CLOCKS_PER_SEC/1000)));
	printf("\nNúmero de comparações para construção BB: %d", compConstrucao);
	fclose(arch);

	/**********************************************************************
	***********************************************************************
	******INÍCIO DA BUSCA NA ÁRVORE COM BASE NOS ARQUIVOS DE CONSULTA******
	***********************************************************************
	**********************************************************************/

	arch = fopen("Consultar/Conjunto2/250000.txt","r");
	if(arch == NULL) {
		printf("Erro na abertura do arquivo de consulta!\n");
		system("pause");
		exit(1);
	}
	printf("\n");

	tConsulta = clock();
	while(fscanf(arch, "%d", &d) != EOF) {
		//printf("%d ", d);
		consultaArv(arv, d);
	}
	tConsulta = clock() - tConsulta;
	printf("\n");
	printf("Tempo de consulta BB: %lf", ((double)tConsulta)/((CLOCKS_PER_SEC/1000)));
	printf("\nNúmero de comparações para consulta BB: %d", compConsulta);
	
	free(arv);
	fclose(arch);

	return 0;
}
