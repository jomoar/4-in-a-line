#include <stdio.h>
#include <string.h>
#include "mt19937ar.h"

/*declaração de funções*/
int quemGanha();
int joga(int col, int jogador, int grelhaJogo[][7]);
void desenharJogo();

/* grelha de estado do jogo */
int grelha[6][7] = {
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0}
};

/*dificuldade*/
int nivel = 10000;

/* numero de jogadas possíveis, vai decrementando ao longo dos turnos, há um empate se chegar a 0 e não houver ainda um vencedor */
int contagem = 42;

int main() {
	/* indicador do turno */
	int turno = 1;

	/* vence indica se há vencedor (1 ou 2) ou não (0) */
	int vence = 0;
	
	/* variável para controlo de loops */
	int i;

	/* seed do gerador */
	init_genrand(&turno);
	
	/* variáveis para MC */
	int melhorCol;
	int maiorN;
	int Nv;
	int iter;
	int contagemMC;
	int VenceMC;
	int grelhaMC[6][7];
	int colAleatoria;
	
	while (turno != -10) {

		switch(turno) {
			
			/* turno do jogador 1 (utilizador) */
			case 1 :
				desenharJogo();
				printf("Em que coluna pretende jogar? (1-7)\n");
				scanf("%d", &i);//pede a jogada ao utilizador e guarda em i
				i-=1;
				turno=joga(i,turno,grelha);
				/*se a jogada for sucedida, turno=2, caso contrário repete-se o case 1*/
				if (turno==2){
					contagem-=1;
					vence = quemGanha(grelha);
					if (vence!=0){// se houver um vencedor passa para o ecra de victoria
						turno = 3;
					}
				}			
				break;
			
			/* turno do jogador 2 (máquina)*/
			case 2:
				
				melhorCol = -1;//variavel para guardar a melhor coluna encontrada
				maiorN = 0;// e respectivo numero de victorias
				
				for (i=0; i<7; i++){ //para cada coluna
					
					if (grelha[0][i]==0){// testa se é possível jogar
						iter = nivel;
						Nv = 0;
					}
					else{// senão for possível, não faz nada e passa para o próximo i
						iter = 0;
					}	
					while (iter>0){// se for possível
						
						memcpy(grelhaMC, grelha, sizeof(grelhaMC));// copia estado do jogo para grelhaMC
						turno = 2;// prepara o turno
						VenceMC = 0;// 0 quando há nenhum vencedor
						
						turno = joga(i, turno, grelhaMC);// joga e passa o turno
						contagemMC = contagem-1;// subtrai um na contagem devido à jogada feita
						VenceMC = quemGanha(grelhaMC);// devolve 2 se a maquina tiver ganho, 0 se não
						
						while ((VenceMC==0) && (contagemMC>0)){// enquanto não há vencedor e há jogadas possíveis
					
							colAleatoria = genrand_real2()*7;//gera aleatório inteiro entre 0 e 6 (7 valores discretos)
							
							if (grelhaMC[0][colAleatoria]!=0){// se jogar nessa coluna for impossivel, não faz nada
							/* nada */}
							else{
								turno = joga(colAleatoria, turno, grelhaMC); // caso contrário joga, testa a ver se vence e decrementa a contagem de jogadas 
								VenceMC = quemGanha(grelhaMC);
								contagemMC--;
							}
						}	
						if (VenceMC==2){// se ganha, incrementa o numero de victorias
							Nv+=1;
						}
						iter--;
					}
					if (Nv>maiorN){// se o numero de victorias final for maior que o obtido para o ultimo i
						maiorN = Nv;// guarda esse número em maiorN
						melhorCol = i;//e o 'i' em melhorCol
					}
				}
				
				turno = 2;
				turno = joga(melhorCol, turno, grelha);//faz a jogada na melhorCol encontrada
				printf("O computador joga na coluna %d (%d%%)", melhorCol, (maiorN*100)/nivel );
				contagem-=1;
				vence = quemGanha(grelha);//verifica vencedor
				if (vence!=0){//se houver um vencedor, passa para o ecra de victoria
					turno = 3;
				}
				else if (contagem==0){// se não houver vencedor e sobrarem 0 jogadas, passa para o ecra de empate
					turno = 5;
				}
				break;
		
			/* fim de jogo com victoria */	
			case 3 :
				desenharJogo();	
				printf("O vencedor é o jogador %d!\n", vence);
				turno = -10;// assim termina-se com o while que contem o jogo
				
				break;
			
			/* fim de jogo com empate */
			case 4 :				
				desenharJogo();
				printf("O jogo termina em empate!\n");
				turno = -10;

				break;
		}
	}
	
	return 0;
}

/*Função para desenhar a grelha do jogo*/
void desenharJogo() {
	printf("\n\n\n\n\n\n\n\n\n\n\n\n");
	printf("| %d | %d | %d | %d | %d | %d | %d |\n", grelha[0][0], grelha[0][1], grelha[0][2], grelha[0][3], grelha[0][4], grelha[0][5], grelha[0][6] );
	printf("|---|---|---|---|---|---|---|\n");
	printf("| %d | %d | %d | %d | %d | %d | %d |\n", grelha[1][0], grelha[1][1], grelha[1][2], grelha[1][3], grelha[1][4], grelha[1][5], grelha[1][6] );
	printf("|---|---|---|---|---|---|---|\n");
	printf("| %d | %d | %d | %d | %d | %d | %d |\n", grelha[2][0], grelha[2][1], grelha[2][2], grelha[2][3], grelha[2][4], grelha[2][5], grelha[2][6] );
	printf("|---|---|---|---|---|---|---|\n");
	printf("| %d | %d | %d | %d | %d | %d | %d |\n", grelha[3][0], grelha[3][1], grelha[3][2], grelha[3][3], grelha[3][4], grelha[3][5], grelha[3][6] );
	printf("|---|---|---|---|---|---|---|\n");
	printf("| %d | %d | %d | %d | %d | %d | %d |\n", grelha[4][0], grelha[4][1], grelha[4][2], grelha[4][3], grelha[4][4], grelha[4][5], grelha[4][6] );
	printf("|---|---|---|---|---|---|---|\n");
	printf("| %d | %d | %d | %d | %d | %d | %d |\n", grelha[5][0], grelha[5][1], grelha[5][2], grelha[5][3], grelha[5][4], grelha[5][5], grelha[5][6] );
	printf("\n");
}

/*Função para efectuar jogadas;
 * aceita como argumentos o número do jogador, a coluna na qual pretende jogar e um pointer para a matriz do jogo*/
int joga(int col, int jogador, int grelhaJogo[][7]) { // array decai para--->int (*grelhaJogo)[7]
	int i;
	if ( *(*grelhaJogo+col) != 0) {// se a primeira casa da coluna estiver ocupada a jogada é impossível, devolve o numero do proprio jogador
		return jogador;
	}
	else {
		for ( i=1; i<6; i++ ) {
			if ( *(*(grelhaJogo+i)+col) != 0 ) { // mete o numero do proprio jogador na ultima linha livre que encontra na coluna 'col'
				*(*(grelhaJogo+(i-1))+col) = jogador;
				if (jogador == 1){//                    e devolve o número do adversário
					return 2;
				}
				else{
					return 1;
				}
			}
		}
		*(*(grelhaJogo+5)+col) = jogador;
		if (jogador==1) {
			return 2;
		}
		else{
			return 1;
		}
	}
}

/*Função para verificar se o jogo termina*/
int quemGanha( int grelhaJogo[][7] )
{
	int c;
	int l;
	for ( l=0; l<3; l++ ) { // verifica na vertical
		for ( c=0; c<7; c++) {
			if ( *(*(grelhaJogo+l)+c) != 0 &&
					*(*(grelhaJogo+l)+c) == *(*(grelhaJogo+l+1)+c) && 
					*(*(grelhaJogo+l)+c) == *(*(grelhaJogo+l+2)+c) && 
					*(*(grelhaJogo+l)+c) == *(*(grelhaJogo+l+3)+c) ){ 
				return *(*(grelhaJogo+l)+c); // devolve o numero do jogador detectado com 4 em linha
			}
		}
	}
	for ( c=0; c<4; c++ ) { // horizontal
		for ( l=0; l<6; l++) {
			if ( *(*(grelhaJogo+l)+c) != 0 &&
					*(*(grelhaJogo+l)+c) == *(*(grelhaJogo+l)+c+1) && 
					*(*(grelhaJogo+l)+c) == *(*(grelhaJogo+l)+c+2) && 
					*(*(grelhaJogo+l)+c) == *(*(grelhaJogo+l)+c+3) ){
				return *(*(grelhaJogo+l)+c);
			}
		}
	}
	for ( c=0; c<4; c++ ) { // diagonais...
		for ( l=0; l<3; l++) { // direita para baixo
			if ( *(*(grelhaJogo+l)+c) != 0 &&
					*(*(grelhaJogo+l)+c) == *(*(grelhaJogo+l+1)+c+1) && 
					*(*(grelhaJogo+l)+c) == *(*(grelhaJogo+l+2)+c+2) && 
					*(*(grelhaJogo+l)+c) == *(*(grelhaJogo+l+3)+c+3) ){
				return *(*(grelhaJogo+l)+c);
			}
		}
		for ( l=3; l<6; l++) { // direita para cima
			if ( *(*(grelhaJogo+l)+c) != 0 &&
					*(*(grelhaJogo+l)+c) == *(*(grelhaJogo+l-1)+c+1) && 
					*(*(grelhaJogo+l)+c) == *(*(grelhaJogo+l-2)+c+2) && 
					*(*(grelhaJogo+l)+c) == *(*(grelhaJogo+l-3)+c+3) ){
				return *(*(grelhaJogo+l)+c);
			}
		}
	}
	return 0; // devolve 0 se nao encontrar 4 em linha
}
