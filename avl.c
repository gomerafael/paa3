#include <stdio.h> 
#include <stdlib.h>
#include <time.h>

int compConsulta = 0, compConstrucao = 0;

//AVL
struct Node {

	int num; 
	struct Node *sae; 
	struct Node *sad; 
	int altura; 
}; 

//Cria a árvore
struct Node* criaArv() {

	return NULL;
}

//Função utilizada para obter o maior de dois números
int max(int a, int b); 

//Função para obter a altura da árvore 
int altura(struct Node *N) { 
	if (N == NULL) 
		return 0; 
	return N->altura; 
} 

//Maior entre dois números 
int max(int a, int b) { 
	return (a > b)? a : b; 
} 

//Função auxiliar que aloca um novo nó
struct Node* novoNo(int num) { 
	struct Node* no = (struct Node*) malloc(sizeof(struct Node)); 
	no->num = num; 
	no->sae = NULL; 
	no->sad = NULL; 
	no->altura = 1; //Novo nó na folha 
	return(no); 
} 

//Rotação a direita
struct Node *rotacaoDireita(struct Node *y) { 
	struct Node *x = y->sae; 
	struct Node *T2 = x->sad; 

	//Rotaciona
	x->sad = y; 
	y->sae = T2; 

	//Atualiza alturas
	y->altura = max(altura(y->sae), altura(y->sad))+1; 
	x->altura = max(altura(x->sae), altura(x->sad))+1; 

	return x; 
} 

//Rotação a esquerda
struct Node *rotacaoEsquerda(struct Node *x) { 
	struct Node *y = x->sad; 
	struct Node *T2 = y->sae; 

	//Rotaciona
	y->sae = x; 
	x->sad = T2; 

	//Atualiza alturas
	x->altura = max(altura(x->sae), altura(x->sad))+1; 
	y->altura = max(altura(y->sae), altura(y->sad))+1; 

	return y; 
} 

//Obtém fator de balanceamento
int balanceamento(struct Node *N) { 
	if (N == NULL) 
		return 0; 
	return altura(N->sae) - altura(N->sad); 
} 

//Função recursiva para inserir na árvore avl
struct Node* inserirAvl(struct Node* no, int num) { 
	
	if (no == NULL) 
		return(novoNo(num));

	compConstrucao += 1;
	if (num < no->num) {
		no->sae = inserirAvl(no->sae, num);
	} else {
   		if (num > no->num) {
			no->sad = inserirAvl(no->sad, num);
		} else {
			return no;
		}
	}

	no->altura = 1 + max(altura(no->sae), altura(no->sad)); 

	int fbalanco = balanceamento(no); 


	//POSSÍVEIS CASOS DE ROTAÇÃO NA ÁRVORE (simples esq, simples dir, dupla esq, dupla dir)
	if (fbalanco > 1 && num < no->sae->num) 
		return rotacaoDireita(no); 

	if (fbalanco < -1 && num > no->sad->num) 
		return rotacaoEsquerda(no); 

	if (fbalanco > 1 && num > no->sae->num) 
	{ 
		no->sae = rotacaoEsquerda(no->sae); 
		return rotacaoDireita(no); 
	} 

	if (fbalanco < -1 && num < no->sad->num) 
	{ 
		no->sad = rotacaoDireita(no->sad); 
		return rotacaoEsquerda(no); 
	} 

	return no; 
} 

//Imprime em pré ordem
void imprimeArv(struct Node* no) {

	printf("<");
	if(!arvoreVazia(no)) { //Se a árvore não for vazia
		printf("%d ", no->num); //Mostra raiz
		imprimeArv(no->sae); //Imprime a sae
		imprimeArv(no->sad); //Imprime a sad
	}
	printf(">");
}

//Busca na árvore
int consultaArv(struct Node* no, int num) {

	if(arvoreVazia(no)) { //Se a árvore for vazia retorna 0
		return 0;
	}
	compConsulta += 1;
	if(num > no->num) {
		consultaArv(no->sad, num);
	}
	if(num < no->num) {
		consultaArv(no->sae, num);
	}
	//O operador lógico || interrompe a busca quando o elemento for encontrado
	return no->num==num; //|| consultaArv(no->sae, num) || consultaArv(no->sad, num);
}

//Verifica se árvore está vazia
int arvoreVazia(struct Node* no) {
	//Retorna 1 se a árvore for vazia e 0 caso contrário
	return no == NULL;
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
	if(arch == NULL) {
		printf("Erro na abertura!\n");
		system("pause");
		exit(1);
	}

	struct Node *arv = criaArv(); 

	tConstrucao = clock();
	while(fscanf(arch, "%d", &c) != EOF) {
		//printf("%d ", c);
		arv = inserirAvl(arv, c);
	}
	tConstrucao = clock() - tConstrucao;
	
	//imprimeArv(arv);

	printf("\n");
	printf("Tempo de construção AVL: %lf", ((double)tConstrucao)/((CLOCKS_PER_SEC/1000)));
	printf("\nNúmero de comparações para construção AVL: %d", compConstrucao);
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
	printf("Tempo de consulta AVL: %lf", ((double)tConsulta)/((CLOCKS_PER_SEC/1000)));
	printf("\nNúmero de comparações para consulta AVL: %d", compConsulta);
	free(arv);
	fclose(arch);

	return 0; 
} 
