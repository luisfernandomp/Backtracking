#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<locale.h>

#define CHAVE '*'
#define ANDAR_HORIZONTAL 'H'
#define ANDAR_VERTICAL 'V'
#define MAX_LINHA 256

int encontrar_chave(char** v, int** aux, int linha, int coluna, int q_linhas, int q_colunas);
int verificar_caminho(char** v, int** aux, int linha, int coluna, int q_linhas, int q_colunas);

/// <summary>
/// Função para realizar a leitura do teclado
/// </summary>
/// <param name="buffer">Ponteiro de char</param>
/// <param name="tamanho">Tamanho do buffer</param>
/// <returns>Um inteiro (1 - sucesso, 0 - erro)</returns>
int ler_linha(char* buffer, size_t tamanho) {
	if (fgets(buffer, tamanho, stdin) == NULL) {
		printf("Erro na leitura da linha.\n");
		return 0;
	}

	size_t len = strlen(buffer);

	if (len > 0 && buffer[len - 1] == '\n') {
		buffer[len - 1] = '\0';
	}

	return 1;
}

/// <summary>
/// Função para desalocar recursos utilizados para o array auxiliar e o para armazenar o que for lido do teclado
/// </summary>
/// <param name="s">Ponteiro para char (array)</param>
/// <param name="v">Ponteiro para inteiro (array)</param>
/// <param name="n">Quantidade de linhas</param>
void desalocar_recursos(char** s, int** v, int n) {
	for (int j = 0; j < n; j++)
	{
		free(v[j]);
		free(s[j]);
	}

	free(v);
	free(s);
}

int main(void) {
	setlocale(LC_ALL, "pt_BR");

	char buffer_linha[MAX_LINHA];
	int andares = 0, linha_inicial = 0, coluna_inicial = 0;
	char** predio;
	int** aux;
	size_t corredores = 0;

	if (!ler_linha(buffer_linha, sizeof(buffer_linha))){
		return 1;
	}

	andares = atoi(buffer_linha);
	predio = calloc(andares, sizeof(char*));
	aux = calloc(andares, sizeof(int*));

	for (int i = 0; i < andares; i++) {
		if (!ler_linha(buffer_linha, sizeof(buffer_linha))) {
			return 1;
		}

		corredores = strlen(buffer_linha);

		predio[i] = calloc(corredores + 1, sizeof(char));
		aux[i] = calloc(corredores, sizeof(int));

		if (predio[i] == NULL || aux[i] == NULL) {
			printf("Erro ao alocar memória!");
			desalocar_recursos(predio, aux, andares);

			return 1;
		}

		strcpy(predio[i], buffer_linha);
	}

	if (!ler_linha(buffer_linha, sizeof(buffer_linha))) {
		return 1;
	}

	if (sscanf(buffer_linha, "%d %d", &linha_inicial, &coluna_inicial) != 2) {
		printf("Formato inválido!");
		return 1;
	}
		
	int resultado = encontrar_chave(predio, aux, linha_inicial, coluna_inicial, andares, corredores);

	if (resultado) {
		printf("Chave encontrada no Edifício João Calvino!\n");
	}
	else {
		printf("Não conseguimos encontrar a chave no Edifício João Calvino.");
	}

	desalocar_recursos(predio, aux, andares);

	return 0;
}

/// <summary>
/// Função para verificar se o próximo caminho é válido, ou seja, se está dentro dos limites da matriz e se
/// é um caminho que já não foi percorrido.
/// </summary>
/// <param name="v">Vetor de entrada (prédio)</param>
/// <param name="aux">Vetor auxiliar</param>
/// <param name="linha">Linha atual</param>
/// <param name="coluna">Coluna atual</param>
/// <param name="q_linhas">Quantidade de linhas</param>
/// <param name="q_colunas">Quantidade de colunas</param>
/// <returns>Um inteiro que indica se o caminho é válido</returns>
int verificar_caminho(char **v, int **aux, int linha, int coluna, int q_linhas, int q_colunas) {

	if (linha > q_linhas - 1 || linha < 0) // Limites da matriz
		return 0;

	if (coluna > q_colunas - 1 || coluna < 0) // Limites da matriz
		return 0;

	if (aux[linha][coluna] == 1) // Caminho já visitado
		return 0;

	return 1;
}

/// <summary>
/// Função responsável por encontrar um caminho possível até a chave usando chamadas recursivas
/// </summary>
/// <param name="v">Matriz de char (o prédio)</param>
/// <param name="aux">Matriz auxiliar para marcar os caminhos já visitados, evitando refazê-los</param>
/// <param name="linha">Linha atual que estamos na matriz v</param>
/// <param name="coluna">Coluna atual que estamos na matriz v</param>
/// <param name="q_linhas">Quantidade total de linhas</param>
/// <param name="q_colunas">Quantidade total de colunas</param>
/// <returns>
/// Um inteiro que indica se a chave foi encontrada, assim quando a chave é encontrada por uma chamada
/// recursiva, nenhum caminho mais precisa ser testado, ele sobe de volta pela cadeia de chamadas recursivas.
/// </returns>
int encontrar_chave(char** v, int** aux, int linha, int coluna, int q_linhas, int q_colunas) {

	if (!verificar_caminho(v, aux, linha, coluna, q_linhas, q_colunas))
	{
		return 0;
	}

	if (v[linha][coluna] == CHAVE) { // Nosso caso base, chave encontrada
		return 1;
	}
	else {
		/* Marcamos na matriz auxiliar que já tentamos percorrer esse caminho,
		 Como queremos apenas descobrir se é possível encontrar a chave para um caminho, 
		 não voltamos o seu valor para 0, por que queremos encontrar somente o primeiro caminho possível,
		 não todas as possibilidades. */

		aux[linha][coluna] = 1;

		if (v[linha][coluna] == ANDAR_VERTICAL) {// Verifica se na nossa posição atual podemos nos mover verticalmente
			/*
				Aqui fazemos duas chamadas recursivas para V, pois queremos cobrir todas as possibilidades.
				Então tentamos tanto subir como descer na matriz.
			*/

			if (verificar_caminho(v, aux, linha + 1, coluna, q_linhas, q_colunas)) {
				if (encontrar_chave(v, aux, linha + 1, coluna, q_linhas, q_colunas)) {
					return 1;
				}
			}

			if (verificar_caminho(v, aux, linha - 1, coluna, q_linhas, q_colunas)) {
				if (encontrar_chave(v, aux, linha - 1, coluna, q_linhas, q_colunas)) {
					return 1;
				}
			}
		}
		else if(v[linha][coluna] == ANDAR_HORIZONTAL) { // Verifica se na nossa posição atual podemos nos mover horizontalmente
			/*
				Aqui fazemos duas chamadas recursivas para H, pois queremos cobrir todas as possibilidades.
				Então tentamos tanto ir para direita como para esquerda.
			*/
			
			if (verificar_caminho(v, aux, linha, coluna + 1, q_linhas, q_colunas)) {
				if (encontrar_chave(v, aux, linha, coluna + 1, q_linhas, q_colunas))
				{
					return 1;
				}
			}

			if (verificar_caminho(v, aux, linha, coluna - 1, q_linhas, q_colunas)) {
				if (encontrar_chave(v, aux, linha, coluna - 1, q_linhas, q_colunas)) {
					return 1;
				}
			}
		}

		return 0;
	}
}
