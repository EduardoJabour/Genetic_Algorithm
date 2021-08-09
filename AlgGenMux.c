#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define N1 2
#define N2 3
#define LINHAS 32 //2^(N1+N2) // 16 // 64 // 256 // 1024
#define COLUNAS (N1 + N2) // Este valor é sempre o mesmo que DIMENSAO_ENTRADA do algoritmo antigo

#define MAXIMO_MUTACOES 3
#define NUMERO_MAX_GERACOES 3000000
#define GERACOES_SEM_MUDANCA 300000
#define PERSISTENCIA 1000
#define INDIVIDUOS_POR_GERACAO 4
#define QTD_CH 2

#define NUMERO_PORTAS 5
#define AND 100
#define OR 200
#define NAND 300
#define NOR 400
#define XOR 500

#define LINHAS_NOS 2
#define COLUNAS_NOS 6
#define NUMERO_NOS ( N1 + N2 + (COLUNAS_NOS * LINHAS_NOS) )

// Versao 15. 01/06/2020

int GeraInt(int LimiteSuperior){ // Gera um valor entre zero(inclusive) e limite superior(nunca retornara o LimiteSuperior) 
    return(rand()%LimiteSuperior);
}

int SorteiaPorta(){
    int porta = GeraInt(5)+1;
    switch(porta){
        case 1:
            return AND;
            break;
        case 2:
            return OR;
            break;
        case 3:
            return NAND;
            break;
        case 4:
            return NOR;
            break;
        case 5:
            return XOR;
            break;
    }
}

char * GetNomePorta(int p){
    switch(p){
        case 100:
            return "AND";
            break;
        case 200:
            return "OR";
            break;
        case 300:
            return "NAND";
            break;
        case 400:
            return "NOR";
            break;
        case 500:
            return "XOR";
            break;
    }
}

int BinarioParaDecimal(int* b, int tam) {
    int i = 0, pot = 1, dec = 0;
	
	for (i = tam-1; i >= 0 ; i--){
		dec += pot*b[i];
		pot *= 2;
	}
	return (dec);
}

void DecimalParaBinario(int* bin, int dec) {
    int j=0, i;
	int temp[COLUNAS];
	while( dec != 0){
		temp[j++] = dec%2;
		dec/=2;
	}
	for (; j < COLUNAS ; j++ )
        temp[j] = 0;
	for ( i = 0 ; i < COLUNAS ; i++ )
		bin[i] = temp[COLUNAS-i-1];
}

void PreencheTabelaVerdade(int** AB){ 
    int i, valor = 0;
    for ( i = 0 ; i < LINHAS ; i++ )
        DecimalParaBinario(AB[i],valor++);
}

void ImprimeVetor(int* VetSolDec, int tam){
	int i;
    printf("\n");
	for (i = 0; i < tam; i++)
		printf("%d ",VetSolDec[i]);
}

void GravaVetor(int* VetSolDec, int tam, FILE *ArqS){
	int i;
    fprintf(ArqS, "\n");
	for (i = 0; i < tam; i++)
		fprintf(ArqS, "%d ",VetSolDec[i]);
}

void PreencheVetorSolucaoDecimal(int* VetSolDec, int** AB){
	int i, tam;
	
	for (i = 0; i < LINHAS; i++){
//		printf ("%d * ", BinarioParaDecimal(AB[i], N1));
//		printf ("%d \n", BinarioParaDecimal(AB[i]+N1, N2));
		VetSolDec[i] = (BinarioParaDecimal(AB[i], N1)) * (BinarioParaDecimal(AB[i]+N1, N2));
	}
}

void PreencheTabelaSolucao(int** S, int* VetSolDec){ 
	int i = 0;
    for ( i = 0 ; i < LINHAS ; i++ )
		DecimalParaBinario(S[i],VetSolDec[i]);
}

void ImprimeMatriz(int** Mat, int linhas, int colunas){
    int i, j;
    printf("\n");
    
    for ( i = 0 ; i < linhas ; i++ ){
        for ( j = 0 ; j < colunas ; j++ ){
			printf("%d ",Mat[i][j]);
		}
        printf("\n");
    }
    printf("\n");
}

void GravaMatriz(int** Mat, int linhas, int colunas, FILE *ArqS){
    int i, j;
    fprintf(ArqS, "\n");
    
    for ( i = 0 ; i < linhas ; i++ ){
        for ( j = 0 ; j < colunas ; j++ ){
			fprintf(ArqS, "%d ",Mat[i][j]);
		}
        fprintf(ArqS, "\n");
    }
    fprintf(ArqS, "\n");
}

void ImprimeIndividuo(int** Ind){
    int i, j;
    printf("\n");
    
    for ( i = 0 ; i < QTD_CH ; i++ ){
        for ( j = 0 ; j < (3*(NUMERO_NOS-COLUNAS)+3)-1 ; j++ ){
			printf("%d ",Ind[i][j]);
			if (j%3 == 0){
				printf("| ");
			}
		}
		if (i == 0 && Ind[i][(3*(NUMERO_NOS-COLUNAS)+3)-1] < -1){
			printf("| %d", (-1)*Ind[i][(3*(NUMERO_NOS-COLUNAS)+3)-1]);	
		}else{
			printf("| %d", Ind[i][(3*(NUMERO_NOS-COLUNAS)+3)-1]);
		}
        printf("\n");
    }
    printf("\n");
}

void GravaIndividuo(int** Ind, FILE *ArqS){
    int i, j;
    fprintf(ArqS, "\n");
    
    for ( i = 0 ; i < QTD_CH ; i++ ){
        for ( j = 0 ; j < (3*(NUMERO_NOS-COLUNAS)+3)-1 ; j++ ){
			fprintf(ArqS, "%d ",Ind[i][j]);
			if (j%3 == 0){
				fprintf(ArqS, "| ");
			}
		}
		if (i == 0 && Ind[i][(3*(NUMERO_NOS-COLUNAS)+3)-1] < -1){
			fprintf(ArqS, "| %d", (-1)*Ind[i][(3*(NUMERO_NOS-COLUNAS)+3)-1]);	
		}else{
			fprintf(ArqS, "| %d", Ind[i][(3*(NUMERO_NOS-COLUNAS)+3)-1]);
		}
        fprintf(ArqS, "\n");
    }
    fprintf(ArqS, "\n");
}

void ImprimeABS(int** AB, int** S){
    int i, j, num = 0;
    printf("\n");
    
    for ( j = 0 ; j < N1 ; j++ ) // Apenas um cabecalho
    	printf("A ");
    for( j = 0 ; j < N2-1 ; j++ )
    	printf("B ");
    for( j = 0 ; j < 1 ; j++ )
    	printf("B |");
    for( j = 0 ; j < COLUNAS ; j++ )
    	printf("S%d |", num++);
    printf("\n");
    for( j = 0 ; j < COLUNAS*2 ; j++ ){
    	if (j == COLUNAS*2-1){
			printf("---|");
		}else{
			printf("---");
		}
	}
	printf("\n");
    
	for ( i = 0 ; i < LINHAS ; i++ ){
        for ( j = 0 ; j < COLUNAS ; j++ ){
			if (j < COLUNAS){
            	if (j == COLUNAS-1){
            		printf("%d | ",AB[i][j]);
				}else{
					printf("%d ",AB[i][j]);
				}
			}
        }
        for( j = 0 ; j < COLUNAS ; j++ ){
			if (j < COLUNAS){
            	printf("%d | ",S[i][j]);
			}
        }
        printf("\n");
    }
    printf("\n");
}

void GravaABS(int** AB, int** S, FILE *ArqS){
    int i, j, num = 0;
    fprintf(ArqS, "\n");
    
    for ( j = 0 ; j < N1 ; j++ ) // Apenas um cabecalho
    	fprintf(ArqS, "A ");
    for( j = 0 ; j < N2-1 ; j++ )
    	fprintf(ArqS, "B ");
    for( j = 0 ; j < 1 ; j++ )
    	fprintf(ArqS, "B |");
    for( j = 0 ; j < COLUNAS ; j++ )
    	fprintf(ArqS, "S%d |", num++);
    fprintf(ArqS, "\n");
    for( j = 0 ; j < COLUNAS*2 ; j++ ){
    	if (j == COLUNAS*2-1){
			fprintf(ArqS, "---|");
		}else{
			fprintf(ArqS, "---");
		}
	}
	fprintf(ArqS, "\n");
    
	for ( i = 0 ; i < LINHAS ; i++ ){
        for ( j = 0 ; j < COLUNAS ; j++ ){
			if (j < COLUNAS){
            	if (j == COLUNAS-1){
            		fprintf(ArqS, "%d | ",AB[i][j]);
				}else{
					fprintf(ArqS, "%d ",AB[i][j]);
				}
			}
        }
        for( j = 0 ; j < COLUNAS ; j++ ){
			if (j < COLUNAS){
            	fprintf(ArqS, "%d | ",S[i][j]);
			}
        }
        fprintf(ArqS, "\n");
    }
    fprintf(ArqS, "\n");
}

void ImprimeABCSParte(int** AB, int** C, int** S, int ChaveMUX00, int** MatPosLinhas){
    int i, j, c, k=0, num = 0;
    printf("\n\n");
    
    for ( j = 0 ; j < N1 ; j++ ) // Apenas um cabecalho
    	printf("A ");
    for( j = 0 ; j < N2-1 ; j++ )
    	printf("B ");
    for( j = 0 ; j < 1 ; j++ )
    	printf("B |");
    	
    for( j = 0 ; j < QTD_CH-1 ; j++ )
    	printf("C%d ", j);	
	if (QTD_CH > 2){
		printf("C%d |", j);	
	}else{
		printf("C%d|", j);	
	}
		
    for( j = 0 ; j < COLUNAS ; j++ )
    	printf("S%d |", num++);
    	
    printf("\n");
    
	if (QTD_CH > 2){
		printf("-");
	}
    for( j = 0 ; j < ((COLUNAS*2)+QTD_CH) ; j++ ){
    	if (j == ((COLUNAS*2)+QTD_CH)-1){
			printf("---|");
		}else{
			printf("---");
		}
	}
	printf("\n");
    
	for ( i = 0 ; i < LINHAS ; i++ ){
		if (i == MatPosLinhas[k][ChaveMUX00]){
			k++;
			for ( j = 0 ; j < COLUNAS ; j++ ){
				if (j < COLUNAS){
	            	if (j == COLUNAS-1){
	            		printf("%d | ",AB[i][j]);
					}else{
						printf("%d ",AB[i][j]);
					}
				}
	        }
	        
	        for( j = 0 ; j < QTD_CH ; j++ ){
	        	if (j == QTD_CH-1){
	           		printf("%d | ",C[i][j]);
				}else{
					printf("%d ",C[i][j]);
					if(QTD_CH > 2){
						printf(" ");
					}
				}
			}
			
	        for( j = 0 ; j < COLUNAS ; j++ ){
				if (j < COLUNAS){
	            	printf("%d | ",S[i][j]);
				}
	        }
	        printf("\n");
    	}
    }
    printf("\n\n");
}

void GravaABCSParte(int** AB, int** C, int** S, int ChaveMUX00, int** MatPosLinhas, FILE *ArqS){
    int i, j, c, k=0, num = 0;
    fprintf(ArqS, "\n\n");
    
    for ( j = 0 ; j < N1 ; j++ ) // Apenas um cabecalho
    	fprintf(ArqS, "A ");
    for( j = 0 ; j < N2-1 ; j++ )
    	fprintf(ArqS, "B ");
    for( j = 0 ; j < 1 ; j++ )
    	fprintf(ArqS, "B |");
    	
    for( j = 0 ; j < QTD_CH-1 ; j++ )
    	fprintf(ArqS, "C%d ", j);	
	if (QTD_CH > 2){
		fprintf(ArqS, "C%d |", j);	
	}else{
		fprintf(ArqS, "C%d|", j);	
	}
		
    for( j = 0 ; j < COLUNAS ; j++ )
    	fprintf(ArqS, "S%d |", num++);
    	
    fprintf(ArqS, "\n");
    
	if (QTD_CH > 2){
		fprintf(ArqS, "-");
	}
    for( j = 0 ; j < ((COLUNAS*2)+QTD_CH) ; j++ ){
    	if (j == ((COLUNAS*2)+QTD_CH)-1){
			fprintf(ArqS, "---|");
		}else{
			fprintf(ArqS, "---");
		}
	}
	fprintf(ArqS, "\n");
    
	for ( i = 0 ; i < LINHAS ; i++ ){
		if (i == MatPosLinhas[k][ChaveMUX00]){
			k++;
			for ( j = 0 ; j < COLUNAS ; j++ ){
				if (j < COLUNAS){
	            	if (j == COLUNAS-1){
	            		fprintf(ArqS, "%d | ",AB[i][j]);
					}else{
						fprintf(ArqS, "%d ",AB[i][j]);
					}
				}
	        }
	        
	        for( j = 0 ; j < QTD_CH ; j++ ){
	        	if (j == QTD_CH-1){
	           		fprintf(ArqS, "%d | ",C[i][j]);
				}else{
					fprintf(ArqS, "%d ",C[i][j]);
					if(QTD_CH > 2){
						fprintf(ArqS, " ");
					}
				}
			}
			
	        for( j = 0 ; j < COLUNAS ; j++ ){
				if (j < COLUNAS){
	            	fprintf(ArqS, "%d | ",S[i][j]);
				}
	        }
	        fprintf(ArqS, "\n");
    	}
    }
    fprintf(ArqS, "\n\n");
}

void ImprimeABCS(int** AB, int** C, int** S){
    int i, j, num = 0;
    printf("\n\n");
    
    for ( j = 0 ; j < N1 ; j++ ) // Apenas um cabecalho
    	printf("A ");
    for( j = 0 ; j < N2-1 ; j++ )
    	printf("B ");
    for( j = 0 ; j < 1 ; j++ )
    	printf("B |");
    	
    for( j = 0 ; j < QTD_CH-1 ; j++ )
    	printf("C%d ", j);	
	if (QTD_CH > 2){
		printf("C%d |", j);	
	}else{
		printf("C%d|", j);	
	}
		
    for( j = 0 ; j < COLUNAS ; j++ )
    	printf("S%d |", num++);
    	
    printf("\n");
    
	if (QTD_CH > 2){
		printf("-");
	}
    for( j = 0 ; j < ((COLUNAS*2)+QTD_CH) ; j++ ){
    	if (j == ((COLUNAS*2)+QTD_CH)-1){
			printf("---|");
		}else{
			printf("---");
		}
	}
	printf("\n");
    
	for ( i = 0 ; i < LINHAS ; i++ ){
        for ( j = 0 ; j < COLUNAS ; j++ ){
			if (j < COLUNAS){
            	if (j == COLUNAS-1){
            		printf("%d | ",AB[i][j]);
				}else{
					printf("%d ",AB[i][j]);
				}
			}
        }
        
        for( j = 0 ; j < QTD_CH ; j++ ){
        	if (j == QTD_CH-1){
           		printf("%d | ",C[i][j]);
			}else{
				printf("%d ",C[i][j]);
				if(QTD_CH > 2){
					printf(" ");
				}
			}
		}
		
        for( j = 0 ; j < COLUNAS ; j++ ){
			if (j < COLUNAS){
            	printf("%d | ",S[i][j]);
			}
        }
        printf("\n");
    }
    printf("\n\n");
}

void GravaABCS(int** AB, int** C, int** S, FILE *ArqS){
    int i, j, num = 0;
    fprintf(ArqS, "\n\n");
    
    for ( j = 0 ; j < N1 ; j++ ) // Apenas um cabecalho
    	fprintf(ArqS, "A ");
    for( j = 0 ; j < N2-1 ; j++ )
    	fprintf(ArqS, "B ");
    for( j = 0 ; j < 1 ; j++ )
    	fprintf(ArqS, "B |");
    	
    for( j = 0 ; j < QTD_CH-1 ; j++ )
    	fprintf(ArqS, "C%d ", j);	
	if (QTD_CH > 2){
		fprintf(ArqS, "C%d |", j);	
	}else{
		fprintf(ArqS, "C%d|", j);	
	}
		
    for( j = 0 ; j < COLUNAS ; j++ )
    	fprintf(ArqS, "S%d |", num++);
    	
    fprintf(ArqS, "\n");
    
	if (QTD_CH > 2){
		fprintf(ArqS, "-");
	}
    for( j = 0 ; j < ((COLUNAS*2)+QTD_CH) ; j++ ){
    	if (j == ((COLUNAS*2)+QTD_CH)-1){
			fprintf(ArqS, "---|");
		}else{
			fprintf(ArqS, "---");
		}
	}
	fprintf(ArqS, "\n");
    
	for ( i = 0 ; i < LINHAS ; i++ ){
        for ( j = 0 ; j < COLUNAS ; j++ ){
			if (j < COLUNAS){
            	if (j == COLUNAS-1){
            		fprintf(ArqS, "%d | ",AB[i][j]);
				}else{
					fprintf(ArqS, "%d ",AB[i][j]);
				}
			}
        }
        
        for( j = 0 ; j < QTD_CH ; j++ ){
        	if (j == QTD_CH-1){
           		fprintf(ArqS, "%d | ",C[i][j]);
			}else{
				fprintf(ArqS, "%d ",C[i][j]);
				if(QTD_CH > 2){
					fprintf(ArqS, " ");
				}
			}
		}
		
        for( j = 0 ; j < COLUNAS ; j++ ){
			if (j < COLUNAS){
            	fprintf(ArqS, "%d | ",S[i][j]);
			}
        }
        fprintf(ArqS, "\n");
    }
    fprintf(ArqS, "\n\n");
}

void GeraMatrizTopologia(int** m){
    //padrao: de cima pra baixo e depois vai andando da esquerda para a direita
    int i, j, no = COLUNAS + 1;
    for ( i = 0 ; i < COLUNAS_NOS; i++ ){
        for ( j = 0 ; j < LINHAS_NOS; j++ )
            m[j][i] = no++;
    }
}


void GeraIndividuoInicial(int** Individuo){
    int i, j;
    int ColunaAtualNaTopologia = 0, LinhaAtualNaTopologia = 0;
    
	for (i = 0; i < QTD_CH; i++){
		Individuo[i][0] = 1;
	}
	
    for (j = 0; j < QTD_CH; j++){
    	ColunaAtualNaTopologia = 0;
		LinhaAtualNaTopologia = 0;
	    for (i = 1; i <= 3*(NUMERO_NOS-COLUNAS); i += 3){
	        Individuo[j][i] = Individuo[j][i+1] = GeraInt((COLUNAS + ColunaAtualNaTopologia * LINHAS_NOS)) + 1;
	        while (Individuo[j][i] == Individuo[j][i+1]){
	            Individuo[j][i+1] = GeraInt((COLUNAS + ColunaAtualNaTopologia * LINHAS_NOS)) + 1;
	        }
	        Individuo[j][i+2] = SorteiaPorta();
	        if (LinhaAtualNaTopologia++ == LINHAS_NOS-1){
	            LinhaAtualNaTopologia = 0;
	            ColunaAtualNaTopologia++;
	        }
	    }
	    Individuo[j][i++] = GeraInt(NUMERO_NOS)+1;
	    Individuo[j][i] = -1;
	}
}

int ResolvePorta(int EntradaUm, int EntradaDois, int Porta){
    switch(Porta){
        case 100:
            return EntradaUm & EntradaDois;
            break;
        case 200:
            return EntradaUm | EntradaDois;
            break;
        case 300:
            return !(EntradaUm & EntradaDois);
            break;
        case 400:
            return !(EntradaUm | EntradaDois);
            break;
        case 500:
            return EntradaUm ^ EntradaDois;
            break;
    }
}

int CalculaSaida(int* LinhaInd, int* AB){ // ind virou LinhaInd e e virou AB
    int TopologiaAtual[LINHAS_NOS][COLUNAS_NOS];
    int i, j, k = 1, a, b;    // k = Posicao No Individuo
    int Pos = COLUNAS + 1;    //posicao na MatrizTopologia
    int NohAtual;
    
    if ( LinhaInd[3*(NUMERO_NOS-COLUNAS)+3-1-1] <= COLUNAS ){
        return AB[LinhaInd[3*(NUMERO_NOS-COLUNAS)+3-1-1]-1];
    }
    
	for ( j = 0 ; j < COLUNAS_NOS; j++ ){
        for ( i = 0 ; i < LINHAS_NOS; i++){
            //printf("\n Entrada: %d %d %d %d. Trinca em analise: %d %d %s",AB[0],AB[1],AB[2],AB[3],LinhaInd[k],LinhaInd[k+1],GetNomePorta(ind[k+2]));
            if ( LinhaInd[k] <= COLUNAS ){  // Entrada um eh porta de entrada
                if ( LinhaInd[k+1] <= COLUNAS ){    // Entrada dois eh porta de entrada
                    TopologiaAtual[i][j] = ResolvePorta(AB[LinhaInd[k]-1], AB[LinhaInd[k+1]-1], LinhaInd[k+2]);
                    //printf("\tResultado: %d (ponto 1)",TopologiaAtual[i][j]);
                }
                else{   // Entrada dois nao eh porta de entrada (tem que pegar o estado atual na TopologiaAtual)
                    // Achando o estado atual na TopologiaAtual
                    NohAtual = COLUNAS + 1;
                    for ( b = 0 ; b < COLUNAS_NOS; b++ ){
                        for ( a = 0 ; a < LINHAS_NOS ; a++ ){
                            if ( NohAtual++ == LinhaInd[k+1] ){
                                TopologiaAtual[i][j] = ResolvePorta(AB[LinhaInd[k]-1], TopologiaAtual[a][b], LinhaInd[k+2]);
                                //printf("\tResultado: %d (ponto 2)",TopologiaAtual[i][j]);
                                b = COLUNAS_NOS;
                                break;
                            }
                        }
                    }
                }
            }
            else{   // Entrada um nao eh porta de entrada (tem que pegar o estado atual na TopologiaAtual)
                if ( LinhaInd[k+1] <= COLUNAS ){    // Entrada dois eh porta de entrada
                    // Achando o estado atual na TopologiaAtual
                    NohAtual = COLUNAS + 1;
                    for ( b = 0 ; b < COLUNAS_NOS; b++ ){
                        for ( a = 0 ; a < LINHAS_NOS ; a++ ){
                            if ( NohAtual++ == LinhaInd[k] ){
                                TopologiaAtual[i][j] = ResolvePorta(TopologiaAtual[a][b], AB[LinhaInd[k+1]-1], LinhaInd[k+2]);
                                //printf("\tResultado: %d (ponto 3)",TopologiaAtual[i][j]);
                                b = COLUNAS_NOS;
                                break;
                            }
                        }
                    }
                }
                else{   // Entrada dois nao eh porta de entrada (tem que pegar o estado atual na TopologiaAtual)
                    // Achando o estado atual na TopologiaAtual
                    int AuxEntrUm, AuxEntrDois;
                    NohAtual = COLUNAS + 1;
                    for ( b = 0 ; b < COLUNAS_NOS; b++ ){
                        for ( a = 0 ; a < LINHAS_NOS ; a++ ){
                            if ( NohAtual++ == LinhaInd[k] ){
                                AuxEntrUm = TopologiaAtual[a][b];
                                b = COLUNAS_NOS;
                                break;
                            }
                        }
                    }
                    NohAtual = COLUNAS + 1;
                    for ( b = 0 ; b < COLUNAS_NOS; b++ ){
                        for ( a = 0 ; a < LINHAS_NOS ; a++ ){
                            if ( NohAtual++ == LinhaInd[k+1] ){
                                AuxEntrDois = TopologiaAtual[a][b];
                                b = COLUNAS_NOS;
                                break;
                            }
                        }
                    }
                    TopologiaAtual[i][j] = ResolvePorta(AuxEntrUm, AuxEntrDois, LinhaInd[k+2]);
                    //printf("\tResultado: %d (ponto 4)",TopologiaAtual[i][j]);
                }
            }
            if ( Pos++ == LinhaInd[3*(NUMERO_NOS-COLUNAS)+3-1-1] ){  //pos = posicao da saida no individuo
                return(TopologiaAtual[i][j]);
            }
            k+=3;
        }
    }
}

void CalculaC (int** C, int** Individuo, int** AB){
    int i, j; 
    
    for ( j = 0 ; j < QTD_CH ; j++ ){
	    for ( i = 0 ; i < LINHAS ; i++ ){
	        C[i][j] = CalculaSaida(Individuo[j], AB[i]);
	    }
	}
	
}

void PreencheMatPosLinhas(int** C, int** MatPosLinhas){
	int i, c;
	int k = 0;
	
	for ( c = 0; c < (int)pow(2, QTD_CH); c++){ // Varrendo as possibilidades de chaves
		for ( i = 0 ; i < LINHAS ; i++ ){ // Varrendo as linhas da tabela verdade entrada
			if (BinarioParaDecimal(C[i], QTD_CH) == c){
				MatPosLinhas[k][c] = i;
				k++;
			}
		}
		k = 0;
	}
	
}

int AvaliaIndividuoParaUmSMUXSegundaCamada(int** S, int ColS, int** C, int** MatPosLinhas, int ChaveMUX00){
    int i, j, k, c;
    int Zeros[(int)pow(2, QTD_CH)], Uns[(int)pow(2, QTD_CH)];
    int Ava = 0;
    int Perfeito = 0;
    
    // Zerando os vetores e variaveis
    k = 0;
	for ( c = 0 ; c < (int)pow(2, QTD_CH) ; c++ ){
	    Zeros[c] = 0;
	    Uns[c] = 0;
	}
    
    // Conta zeros e uns
	for ( i = 0 ; i < LINHAS ; i++ ){ // Varrendo as linhas da tabela verdade entrada
		for ( c = 0; c < (int)pow(2, QTD_CH); c++){ // Varrendo as possibilidades de chaves
			if (BinarioParaDecimal(C[i], QTD_CH) == c && i == MatPosLinhas[k][ChaveMUX00]){
				k++;
       			if (S[i][ColS] == 0){
       				Zeros[c]++;
				}else{
					Uns[c]++;
				}
			}	
		}
	}
	/*
	for ( c = 0 ; c < (int)pow(2, QTD_CH) ; c++ ){
	    printf(" %d - ", Zeros[c]); 
		printf("%d   ", Uns[c]); 
	    printf("\n"); 
	}
	*/
    for ( c = 0 ; c < (int)pow(2, QTD_CH) ; c++ ){
        if (Uns[c] == 0 && Zeros[c] == 0){
        	Ava += COLUNAS;
        	Perfeito++;
		}else{
			if (Zeros[c] > Uns[c]){
	        	if (Uns[c] != 0){
	        		Ava += round((float)Zeros[c] / (float)Uns[c]);
				}else{
					Ava += ( LINHAS / (N1*N2) ) * Zeros[c];
					Perfeito++;
				}
			}else{
				if (Zeros[c] != 0){
	        		Ava += round((float)Uns[c] / (float)Zeros[c]);
				}else{
					Ava += ( LINHAS / (N1*N2) ) * Uns[c];
					Perfeito++;
				}
			} 			
		}
    }

	if ( Perfeito == (int)pow(2, QTD_CH) ){
		return ((-1)*Ava);
	}else{
		return Ava;
	}
	
}

int AvaliaIndividuoParaUmS(int** S, int ColS, int** C){
    int i, j, k, c;
    int Zeros[(int)pow(2, QTD_CH)], Uns[(int)pow(2, QTD_CH)];
    int Ava = 0;
    int Perfeito = 0;
    
    // Zerando os vetores 
    for ( c = 0 ; c < (int)pow(2, QTD_CH) ; c++ ){
	    Zeros[c] = 0;
	    Uns[c] = 0;
	}
    
    // Conta zeros e uns
	for ( i = 0 ; i < LINHAS ; i++ ){ // Varrendo as linhas da tabela verdade entrada
		for ( c = 0; c < (int)pow(2, QTD_CH); c++){ // Varrendo as possibilidades de chaves
			if (BinarioParaDecimal(C[i], QTD_CH) == c){
       			if (S[i][ColS] == 0){
       				Zeros[c]++;
				}else{
					Uns[c]++;
				}
			}	
		}
	}
	/*
	for ( c = 0 ; c < (int)pow(2, QTD_CH) ; c++ ){
	    printf(" %d - ", Zeros[c]); 
		printf("%d   ", Uns[c]); 
	    printf("\n"); 
	}
	*/
    for ( c = 0 ; c < (int)pow(2, QTD_CH) ; c++ ){
        if (Uns[c] == 0 && Zeros[c] == 0){
        	Ava += COLUNAS;
        	Perfeito++;
		}else{
			if (Zeros[c] > Uns[c]){
	        	if (Uns[c] != 0){
	        		Ava += round((float)Zeros[c] / (float)Uns[c]);
				}else{
					Ava += ( LINHAS / (N1*N2) ) * Zeros[c];
					Perfeito++;
				}
			}else{
				if (Zeros[c] != 0){
	        		Ava += round((float)Uns[c] / (float)Zeros[c]);
				}else{
					Ava += ( LINHAS / (N1*N2) ) * Uns[c];
					Perfeito++;
				}
			} 			
		}
    }

	if ( Perfeito == (int)pow(2, QTD_CH) ){
		return ((-1)*Ava);
	}else{
		return Ava;
	}
	
}

int RetornaSituacaoPortaMUX00(int** S, int ColS, int** C, int Chave){
    int i;
    int Zeros = 0, Uns = 0;
	
	/* A codificacao que sera utilizada
	0 = porta travada em zero
	1 = porta travada em um
	-1 = porta nao utilizada
	2 = porta com necessidade de mais um MUX
	*/
	
    // Conta zeros e uns
	for ( i = 0 ; i < LINHAS ; i++){ // Varrendo as linhas da tabela verdade entrada
		if (BinarioParaDecimal(C[i], QTD_CH) == Chave){
       		if (S[i][ColS] == 0){
       			Zeros++;
			}else{
				Uns++;
			}
		}	
	}
	
	// Identifica se a porta eh zero, um ou se ainda eh preciso adicionar MUX
    if (Uns == 0 && Zeros == 0){
		return (-1);
	}else{
	if (Zeros > Uns){
	   	if (Uns != 0){
	   		return (2); // Mais zeros do que Uns e Uns nao eh 0
		}else{
			return (0); // Mais zeros do que Uns e Uns eh 0
		}
		}else{
			if (Zeros != 0){
	      		return (2); // Mais Uns do que Zeros e Zeros nao eh 0
			}else{
				return (1); // Mais Uns do que Zeros e Zeros eh 0
			}
		}
	}
}

int RetornaSituacaoPortaMUXCamadaSecundaria(int** S, int ColS, int** C, int Chave, int** MatPosLinhas, int ChaveMUX00){
    int i, k = 0;
    int Zeros = 0, Uns = 0;
	
	/* A codificacao que sera utilizada
	0 = porta travada em zero
	1 = porta travada em um
	-1 = porta nao utilizada
	2 = porta com necessidade de mais um MUX
	*/
	
    // Conta zeros e uns
	for ( i = 0 ; i < LINHAS ; i++){ // Varrendo as linhas da tabela verdade entrada
		if (i == MatPosLinhas[k][ChaveMUX00]){
       		k++;
       		if (BinarioParaDecimal(C[i], QTD_CH) == Chave){
				if (S[i][ColS] == 0){
	       			Zeros++;
				}else{
					Uns++;
				}       			
			}
		}	
	}
	
	// Identifica se a porta eh zero, um ou se ainda eh preciso adicionar MUX
    if (Uns == 0 && Zeros == 0){
		return (-1);
	}else{
	if (Zeros > Uns){
	   	if (Uns != 0){
	   		return (2); // Mais zeros do que Uns e Uns nao eh 0
		}else{
			return (0); // Mais zeros do que Uns e Uns eh 0
		}
		}else{
			if (Zeros != 0){
	      		return (2); // Mais Uns do que Zeros e Zeros nao eh 0
			}else{
				return (1); // Mais Uns do que Zeros e Zeros eh 0
			}
		}
	}
}


int RetornaOcorrenciaDaChaveNoMUX00(int** C, int Chave){
	int i, OcorrenciaDeterminadaChave = 0;
	
	for ( i = 0 ; i < LINHAS ; i++){ // Varrendo as linhas da tabela verdade entrada
		if (BinarioParaDecimal(C[i], QTD_CH) == Chave){
       		OcorrenciaDeterminadaChave++;
		}	
	}
	
	return OcorrenciaDeterminadaChave;
}

int RetornaOcorrenciaDaChaveNoMUXCamadaSecundaria(int** C, int Chave, int** MatPosLinhas, int ChaveMUX00){
	int i, k = 0, OcorrenciaDeterminadaChave = 0;
	
	for ( i = 0 ; i < LINHAS ; i++){ // Varrendo as linhas da tabela verdade entrada
		if (i == MatPosLinhas[k][ChaveMUX00]){
			k++;
			if (BinarioParaDecimal(C[i], QTD_CH) == Chave){
				OcorrenciaDeterminadaChave++;
			}
		}	
	}
	
	return OcorrenciaDeterminadaChave;
}

int SelecionaMelhorIndividuoDaGeracao(int** Geracao){
    int MaiorRank = -1;
    int Posicao = -1;
	int i;
	int Perfeito = -1;
    
    for ( i = 0 ; i < INDIVIDUOS_POR_GERACAO*QTD_CH ; i+= QTD_CH ){
        if  ( Geracao[i][3*(NUMERO_NOS-COLUNAS)+3-1] < -2){
			Posicao = i;
			return(Posicao);
		}else{
	        if ( Geracao[i][3*(NUMERO_NOS-COLUNAS)+3-1] > MaiorRank ){
	            MaiorRank = Geracao[i][3*(NUMERO_NOS-COLUNAS)+3-1];
	            Posicao = i;
	        }			
		}
    }
    return(Posicao);
}

void ModificaIndividuosDaGeracao(int** Geracao, int MDM){
    int i, j, k, CMutado;
    int PosicaoAMudar, EntradaTemp;
    int ColunaAtualNaTopologia = 0, LinhaAtualNaTopologia = 0;
    
    for ( k = QTD_CH ; k < QTD_CH*INDIVIDUOS_POR_GERACAO ; k = k + QTD_CH ){
		for ( j = 1 ; j <= (MAXIMO_MUTACOES*MDM) ; j++ ){
	        ColunaAtualNaTopologia = 0; // Reseta a coluna da topologia para a nova mutacao
	        LinhaAtualNaTopologia = 0; // Reseta a linha da topologia para a nova mutacao
	        PosicaoAMudar = GeraInt(3*(NUMERO_NOS-COLUNAS)+3-2) + 1;
	        for ( i = 1 ; i <= 3*(NUMERO_NOS-COLUNAS) ; i = i + 3 ){ // Nos
	            CMutado = GeraInt(QTD_CH);
				if ( i == PosicaoAMudar ){
	                EntradaTemp = GeraInt((COLUNAS + ColunaAtualNaTopologia * LINHAS_NOS)) + 1;
	                while (EntradaTemp == Geracao[k+CMutado][i+1]){
	                    EntradaTemp = GeraInt((COLUNAS + ColunaAtualNaTopologia * LINHAS_NOS)) + 1;
	                }
	                Geracao[k+CMutado][i] = EntradaTemp;
	                i = 3*(NUMERO_NOS-COLUNAS)+1;
	                break;
	            }
	            if ( i+1 == PosicaoAMudar ){
	                Geracao[k+CMutado][i+1] = Geracao[k+CMutado][i];
	                while (Geracao[k+CMutado][i] == Geracao[k+CMutado][i+1]){
	                    Geracao[k+CMutado][i+1] = GeraInt((COLUNAS + ColunaAtualNaTopologia * LINHAS_NOS)) + 1;
	                }
	                i = 3*(NUMERO_NOS-COLUNAS)+1;
	                break;
	            }
	            if ( i+2 == PosicaoAMudar ){
	                Geracao[k+CMutado][i+2] = SorteiaPorta();
	                i = 3*(NUMERO_NOS-COLUNAS)+1;
	                break;
	            }
	            if (LinhaAtualNaTopologia++ == LINHAS_NOS-1){
	                LinhaAtualNaTopologia = 0;
	                ColunaAtualNaTopologia++;
	            }
	        }
	        if ( i == PosicaoAMudar ){  // Saida
	            Geracao[k+CMutado][i] = GeraInt(NUMERO_NOS)+1;
	        }
	        // Deixei o rank anterior inalterado, em algum momento futuro ele devera ser recalculado
	    }
	}
}

void ModificaSaidaIndividuosDaGeracao(int** Geracao){
	int i, j, k, CMutado;
	
	CMutado = GeraInt(QTD_CH);
	for ( k = QTD_CH ; k < QTD_CH*INDIVIDUOS_POR_GERACAO ; k += QTD_CH ){    	
		Geracao[k+CMutado][3*(NUMERO_NOS-COLUNAS)+1] = GeraInt(NUMERO_NOS)+1;
	}
	
}

// ------------------------------------------------------ main ------------------------------------------------------ //
int main(){
	
	// -------------------- Definir a coluna do S a ser selecionada -------------------- //
	int Si;
	printf("Informar a coluna do S a ser trabalhada ( valor entre 0 e %d ) : ", COLUNAS-1);
	scanf("%d", &Si);
	
	if (Si < 0 || Si > COLUNAS-1){
		printf("Valor digitado invalido. Nao existe coluna de S correspondente.");
		return(1);
	}
	
	// -------------------- Gerar um TXT de resposta do programa -------------------- //
	char NomeArqSaida[30] = "Resultado_S";
    char StrSi[3];
    StrSi[0] = Si-Si%10+'0';
    StrSi[1] = Si%10+'0';
    StrSi[2] = '\0';
    
    strcat(NomeArqSaida, StrSi);
    strcat(NomeArqSaida, ".txt");
    
    FILE *ArqSaida = fopen(NomeArqSaida,  "w");
    
    fprintf(ArqSaida, "N1 : %d \nN2 : %d \nLINHAS : %d \nCOLUNAS : %d \n\n", N1, N2, LINHAS, COLUNAS);
	fprintf(ArqSaida, "MAXIMO_MUTACOES : %d \nNUMERO_MAX_GERACOES : %d \nGERACOES_SEM_MUDANCA : %d \nPERSISTENCIA : %d \nINDIVIDUOS_POR_GERACAO : %d \nQTD_CH : %d \n\n", MAXIMO_MUTACOES, NUMERO_MAX_GERACOES, GERACOES_SEM_MUDANCA, PERSISTENCIA, INDIVIDUOS_POR_GERACAO, QTD_CH);
	fprintf(ArqSaida, "NUMERO_PORTAS : %d \nAND : %d \nOR : %d \nNAND : %d NOR : %d \nXOR : %d \n", NUMERO_PORTAS, AND, OR, NAND, NOR, XOR);
	fprintf(ArqSaida, "LINHAS_NOS : %d \nCOLUNAS_NOS : %d \nNUMERO_NOS : %d \n\n", LINHAS_NOS, COLUNAS_NOS, NUMERO_NOS);
	
	// ------------------------- Gerar o Enunciado do problema ------------------------- //
    int i, j, k;
	int** AB = NULL;
	int** S = NULL;
	int* VetSolDec = NULL;
	
    //Aloca AB
    AB = (int**) malloc(LINHAS * sizeof(int*));
    if (!AB){ printf("\n\n ERRO DE ALOCACAO DE MEMORIA. PROGRAMA ENCERRADO \n\n"); return(1);}
    for ( i = 0 ;  i < LINHAS ; i++ ){
        AB[i] = (int*)malloc(COLUNAS * sizeof(int));
        if (!AB[i]){ printf("\n\n ERRO DE ALOCACAO DE MEMORIA. PROGRAMA ENCERRADO \n\n"); return(1);}
    }
    
    //Aloca S
    S = (int**) malloc(LINHAS * sizeof(int*));
    if (!S){ printf("\n\n ERRO DE ALOCACAO DE MEMORIA. PROGRAMA ENCERRADO \n\n"); return(1);}
    for ( i = 0 ;  i < LINHAS ; i++ ){
        S[i] = (int*)malloc(COLUNAS * sizeof(int));
        if (!S[i]){ printf("\n\n ERRO DE ALOCACAO DE MEMORIA. PROGRAMA ENCERRADO \n\n"); return(1);}
    }
    
    //Aloca VetSolDec
    VetSolDec = (int*) malloc(LINHAS * sizeof(int));
    if (!VetSolDec){ printf("\n\n ERRO DE ALOCACAO DE MEMORIA. PROGRAMA ENCERRADO \n\n"); return(1);}
    
	PreencheTabelaVerdade(AB);
	PreencheVetorSolucaoDecimal(VetSolDec, AB);
	PreencheTabelaSolucao(S, VetSolDec);
	 ImprimeABS(AB, S);

	// ------------------------- Gerar o primeiro Individuo ------------------------- //	
	int** MatrizTopologia = NULL; //Referente aos nos para gerar Ci
	int** Geracao = NULL; // QTD_CH Individuos
	int** Individuo = NULL; //ID, NO,NO,PORTA, ..., NO,NO,PORTA, SAIDA, PONTUACAO
	int** IndividuoSolucaoMUX00 = NULL; // Apenas uma variavel que armazena o melhor individuo da rodada do programa para o primeiro MUX
	
	srand(time(NULL));
	
    //Aloca Matriz Topologia
    MatrizTopologia = (int**) malloc(LINHAS_NOS * sizeof(int*));
    if (!MatrizTopologia){ printf("\n\n ERRO DE ALOCACAO DE MEMORIA. PROGRAMA ENCERRADO \n\n"); return(1);}
    for ( i = 0 ;  i < LINHAS_NOS ; i++ ){
        MatrizTopologia[i] = (int*)malloc(COLUNAS_NOS * sizeof(int));
        if (!MatrizTopologia[i]){ printf("\n\n ERRO DE ALOCACAO DE MEMORIA. PROGRAMA ENCERRADO \n\n"); return(1);}
    }
    
    //Aloca Geracao
    Geracao = (int**) malloc((INDIVIDUOS_POR_GERACAO * QTD_CH) * sizeof(int*));
    if (!Geracao){ printf("\n\n ERRO DE ALOCACAO DE MEMORIA. PROGRAMA ENCERRADO \n\n"); return(1);}
    for ( i = 0 ;  i < (INDIVIDUOS_POR_GERACAO * QTD_CH); i++ ){
        Geracao[i] = (int*)malloc((3*(NUMERO_NOS-COLUNAS)+3) * sizeof(int));
        if (!Geracao[i]){ printf("\n\n ERRO DE ALOCACAO DE MEMORIA. PROGRAMA ENCERRADO \n\n"); return(1);}
    }
    
    //Aloca Individuo // contem um par de circuitos, um para cada Ci do MUX
    Individuo = (int**) malloc(QTD_CH * sizeof(int*));
    if (!Individuo){ printf("\n\n ERRO DE ALOCACAO DE MEMORIA. PROGRAMA ENCERRADO \n\n"); return(1);}
    for ( i = 0 ;  i < QTD_CH; i++ ){
        Individuo[i] = (int*)malloc((3*(NUMERO_NOS-COLUNAS)+3) * sizeof(int));
        if (!Individuo[i]){ printf("\n\n ERRO DE ALOCACAO DE MEMORIA. PROGRAMA ENCERRADO \n\n"); return(1);}
    }
    
    //Aloca Individuo Solucao do MUX 00 // contem um par de circuitos, um para cada Ci do MUX
    IndividuoSolucaoMUX00 = (int**) malloc(QTD_CH * sizeof(int*));
    if (!IndividuoSolucaoMUX00){ printf("\n\n ERRO DE ALOCACAO DE MEMORIA. PROGRAMA ENCERRADO \n\n"); return(1);}
    for ( i = 0 ;  i < QTD_CH; i++ ){
        IndividuoSolucaoMUX00[i] = (int*)malloc((3*(NUMERO_NOS-COLUNAS)+3) * sizeof(int));
        if (!IndividuoSolucaoMUX00[i]){ printf("\n\n ERRO DE ALOCACAO DE MEMORIA. PROGRAMA ENCERRADO \n\n"); return(1);}
    }
    
	GeraMatrizTopologia(MatrizTopologia);
    printf("\nMatriz Topologia : \n");
    fprintf (ArqSaida, "\nMatriz Topologia : \n");
    ImprimeMatriz (MatrizTopologia, LINHAS_NOS, COLUNAS_NOS);
    GravaMatriz (MatrizTopologia , LINHAS_NOS, COLUNAS_NOS, ArqSaida);
	
	GeraIndividuoInicial(Individuo);
	// ImprimeIndividuo(Individuo);
	
	// -------------------- Calcula saidas do individuo original Ci Ci+1  -------------------- //
	int** C = NULL; // QTD_CH colunas com Ci e Ci+1 e numero de linhas igual ao numero de linhas da tabela verdade
	
    //Aloca MatrizC
    C = (int**) malloc(LINHAS * sizeof(int*));
    if (!C){ printf("\n\n ERRO DE ALOCACAO DE MEMORIA. PROGRAMA ENCERRADO \n\n"); return(1);}
    for ( i = 0 ;  i < LINHAS ; i++ ){
        C[i] = (int*)malloc(QTD_CH * sizeof(int));
        if (!C[i]){ printf("\n\n ERRO DE ALOCACAO DE MEMORIA. PROGRAMA ENCERRADO \n\n"); return(1);}
    }	
 		
	CalculaC(C, Individuo, AB);
 	
	// -------------------- Avalia Individuo e Preenche a primeira geracao com o primeiro Individuo -------------------- //
	Individuo[0][3*(NUMERO_NOS-COLUNAS)+3-1] = AvaliaIndividuoParaUmS(S, Si, C); //Avaliacao eh colocada apenas no primeiro circuito do individuo
	ImprimeIndividuo(Individuo);

    for ( i = 0 ;  i < INDIVIDUOS_POR_GERACAO*QTD_CH ; i+=QTD_CH ){
        for ( j = 0 ; j < 3*(NUMERO_NOS-COLUNAS)+3 ; j++ ){
            for (k = 0; k < QTD_CH; k++){
				Geracao[i+k][j] = Individuo[k][j];
    		}
		}
    }
    ImprimeMatriz(Geracao, QTD_CH*INDIVIDUOS_POR_GERACAO, (3*(NUMERO_NOS-COLUNAS)+3));

	// Imprimir a Matriz do primeiro individuo Gerado
	ImprimeABCS(AB, C, S);
	GravaABCS(AB, C, S, ArqSaida);

	// -------------------- Rotina para o primeiro MUX ate o numero estabelecido de geracoes -------------------- //
	int NumeroGeracao = 1;
	int MelhorRank = -1;
	int MelhorRankAteOMomento = -1;
	int MelhorIndividuoGeracao = 0;
	int MultiplicadorDeMutacoes = 1;
	int Cont = 1, Sinal = 0, Per = 0;
	
	while (NumeroGeracao <= NUMERO_MAX_GERACOES && MelhorRank > -2){ // colocar no futuro como condicao do whili  && MelhorRank < RANK_OBJETIVO_PRIMEIRO_MUX 
		
		// Introduz mutacoes nos outros (n-1) Individuos da geracao
		ModificaIndividuosDaGeracao(Geracao, MultiplicadorDeMutacoes);
				
		// Avalia cada novo individuo da geracao
		for ( i = QTD_CH ;  i < INDIVIDUOS_POR_GERACAO*QTD_CH ; i+= QTD_CH ){ 
        	CalculaC(C, &Geracao[i], AB); // A passagem de parametro com o & pode ser substituida em outras partes do programa pra simplificar o codigo
           	Geracao[i][3*(NUMERO_NOS-COLUNAS)+3-1] = AvaliaIndividuoParaUmS(S, Si, C);
		}
        ImprimeMatriz(Geracao, QTD_CH*INDIVIDUOS_POR_GERACAO, (3*(NUMERO_NOS-COLUNAS)+3));
		// Seleciona o melhor individuo dessa nova geracao
		MelhorIndividuoGeracao = SelecionaMelhorIndividuoDaGeracao(Geracao); // Retorna a posicao do primeiro circuito do melhor individuo da geracao
		MelhorRank = Geracao[MelhorIndividuoGeracao][3*(NUMERO_NOS-COLUNAS)+3-1];
		
        // Coloca o melhor no topo da geracao
        for (k = 0; k < QTD_CH; k++){
			for ( j = 0 ; j < 3*(NUMERO_NOS-COLUNAS)+3 ; j++ ){
            	Geracao[0+k][j] = Geracao[MelhorIndividuoGeracao+k][j];
			}
        }

        // Individuo recebe o melhor individuo ate o momento
        for (k = 0; k < QTD_CH; k++){
			for ( j = 0 ; j < 3*(NUMERO_NOS-COLUNAS)+3 ; j++ ){
            	Individuo[0+k][j] = Geracao[MelhorIndividuoGeracao+k][j];
			}
        }
		
		// Replica este melhor individuo na matriz Geracao e coloca o melhor individuo em Individuo
	    for ( i = QTD_CH ;  i < INDIVIDUOS_POR_GERACAO*QTD_CH ; i+=QTD_CH ){
        	for ( j = 0 ; j < 3*(NUMERO_NOS-COLUNAS)+3 ; j++ ){
            	for (k = 0; k < QTD_CH; k++){
					Geracao[i+k][j] = Geracao[k][j];
    			}
			}
    	}
        
        if (MelhorRank > MelhorRankAteOMomento){
            printf("\n\n Evolucao!!!!  Geracao: %d. \t Melhor rank ate esta geracao: %d \t Valor do Multiplicador de Mutacoes : %d \n", NumeroGeracao, MelhorRank, MultiplicadorDeMutacoes);
            fprintf (ArqSaida, "\n\n Evolucao!!!!  Geracao: %d. \t Melhor rank ate esta geracao: %d \t Valor do Multiplicador de Mutacoes : %d \n", NumeroGeracao, MelhorRank, MultiplicadorDeMutacoes);
            ImprimeIndividuo(Individuo);
            GravaIndividuo(Individuo, ArqSaida);
            MelhorRankAteOMomento = MelhorRank;
            Cont = 1;
            MultiplicadorDeMutacoes = 1;
            Sinal = 0;
        }else{
        	Cont++;
        	if (Sinal == 1){
	        	Per++;
				if (Per > PERSISTENCIA){
					ModificaSaidaIndividuosDaGeracao(Geracao);
					Per = 0;
					MultiplicadorDeMutacoes++;
		        	printf(", %d", MultiplicadorDeMutacoes);	
				}
			}
		}		

		if (Cont%GERACOES_SEM_MUDANCA == 0){
			MultiplicadorDeMutacoes++;
			Sinal = 1;
			printf("\n\n Apos %d geracoes sem evolucao no individuo, na geracao %d o Multiplicador de Mutacoes recebeu %d", GERACOES_SEM_MUDANCA, NumeroGeracao, MultiplicadorDeMutacoes);
		}

		if (MultiplicadorDeMutacoes > (3*(NUMERO_NOS-COLUNAS)+3-2)*2/MAXIMO_MUTACOES){
			MultiplicadorDeMutacoes = 1;
			Sinal = 0;
		}

		NumeroGeracao++;
		if (NumeroGeracao%(NUMERO_MAX_GERACOES/20) == 0){
            printf("\n\n Geracao %d. Melhor rank ate agora: %d. Trabalhando no S%d.", NumeroGeracao, MelhorRank, Si);
        }
	}
	
	// Colocar o melhor Individuo encontrado dentro de Individuo Solucao para armazenar
    for (k = 0; k < QTD_CH; k++){
		for ( j = 0 ; j < 3*(NUMERO_NOS-COLUNAS)+3 ; j++ ){
           	IndividuoSolucaoMUX00[k][j] = Individuo[k][j];
		}
    }
	
	// Imprimir resposta apos buscar a solucao para um determinado Si
	CalculaC(C, IndividuoSolucaoMUX00, AB);
	ImprimeABCS(AB, C, S);
	GravaABCS(AB, C, S, ArqSaida);
	
	if (MelhorRank < -2){
		printf ("Encontrado na geracao : %d \nCom Rank : %d \nTemos como Individuo que soluciona S%d : ", NumeroGeracao-1, (-1)*MelhorRank, Si);
		fprintf (ArqSaida, "Encontrado na geracao : %d \nCom Rank : %d \nTemos como Individuo que soluciona S%d : ", NumeroGeracao-1, (-1)*MelhorRank, Si);
	}else{
		printf ("Apos avaliar %d geracoes nao foi possivel encontrar um Individuo que solucionasse S%d. \nCom um Rank de %d o melhor Individuo encontrado foi : ", NumeroGeracao-1, Si, MelhorRankAteOMomento);
		fprintf (ArqSaida, "Apos avaliar %d geracoes nao foi possivel encontrar um Individuo que solucionasse S%d. \nCom um Rank de %d o melhor Individuo encontrado foi : ", NumeroGeracao-1, Si, MelhorRankAteOMomento);
		
	}

	ImprimeIndividuo(IndividuoSolucaoMUX00);
	GravaIndividuo(IndividuoSolucaoMUX00, ArqSaida);

	// Gerando a resposta do primeiro MUX
	int* SituacaoPortasMUX00 = NULL;
	int* SituacaoPortasMUXAux = NULL;
	int* OcorrenciaDasChaves = NULL;
	
	//Aloca SituacaoPortasMUX00
    SituacaoPortasMUX00 = (int*) malloc(((int)pow(2, QTD_CH)) * sizeof(int));
    if (!SituacaoPortasMUX00){ printf("\n\n ERRO DE ALOCACAO DE MEMORIA. PROGRAMA ENCERRADO \n\n"); return(1);}
    
    //Aloca SituacaoPortasMUXAux
    SituacaoPortasMUXAux = (int*) malloc(((int)pow(2, QTD_CH)) * sizeof(int));
    if (!SituacaoPortasMUXAux){ printf("\n\n ERRO DE ALOCACAO DE MEMORIA. PROGRAMA ENCERRADO \n\n"); return(1);}
    
	//Aloca OcorrenciaDasChaves
    OcorrenciaDasChaves = (int*) malloc(((int)pow(2, QTD_CH)) * sizeof(int));
    if (!OcorrenciaDasChaves){ printf("\n\n ERRO DE ALOCACAO DE MEMORIA. PROGRAMA ENCERRADO \n\n"); return(1);}    
    
    // Preenche o vetor SituacaoPortasMUX00 e OcorrenciaDasChaves
    for (i = 0; i < ((int)pow(2, QTD_CH)); i++){
    	SituacaoPortasMUX00[i] = RetornaSituacaoPortaMUX00(S, Si, C, i);
    	OcorrenciaDasChaves[i] = RetornaOcorrenciaDaChaveNoMUX00(C, i);
	}
	
	printf ("\nSituacao de cada porta do MUX00 : ");
	fprintf (ArqSaida, "\nSituacao de cada porta do MUX00 : ");
	ImprimeVetor(SituacaoPortasMUX00, ((int)pow(2, QTD_CH)));
	GravaVetor(SituacaoPortasMUX00, ((int)pow(2, QTD_CH)), ArqSaida);
	printf ("\nLegenda \nporta travada em zero = 0 \nporta travada em um = 1 \nporta nao utilizada = -1 \nporta com necessidade de mais um MUX = 2 ");
	fprintf (ArqSaida, "\nLegenda \nporta travada em zero = 0 \nporta travada em um = 1 \nporta nao utilizada = -1 \nporta com necessidade de mais um MUX = 2 ");
	printf ("\n\nNumero de vezes que cada porta do MUX00 foi utilizada : ");
	fprintf (ArqSaida, "\n\nNumero de vezes que cada porta do MUX00 foi utilizada : ");
	ImprimeVetor(OcorrenciaDasChaves, ((int)pow(2, QTD_CH)));
	GravaVetor(OcorrenciaDasChaves, ((int)pow(2, QTD_CH)), ArqSaida);
	
	// Preparacoes para entrar na segunda camada de MUX
	int** MatPosLinhas = NULL; 
	int ChaveMUX00;
		
	//Aloca MatPosLinhas
    MatPosLinhas = (int**) malloc(LINHAS * sizeof(int*));
    if (!MatPosLinhas){ printf("\n\n ERRO DE ALOCACAO DE MEMORIA. PROGRAMA ENCERRADO \n\n"); return(1);}
    for ( i = 0 ;  i < LINHAS ; i++ ){
        MatPosLinhas[i] = (int*)malloc((int)pow(2, QTD_CH) * sizeof(int));
        if (!MatPosLinhas[i]){ printf("\n\n ERRO DE ALOCACAO DE MEMORIA. PROGRAMA ENCERRADO \n\n"); return(1);}
    }
    
    // Preencher a MatPosLinhas com -1 em todas as posicoes, depois preencher as posicoes das chaves do MUX00 por cima. As ultimas linhas dessa matriz sempre serao -1.
    for (i = 0; i < LINHAS; i++){
		for (j = 0; j < (int)pow(2, QTD_CH); j++){
    		MatPosLinhas[i][j] = -1;
    	}
	}
	PreencheMatPosLinhas(C, MatPosLinhas);
    // ImprimeMatriz(MatPosLinhas, LINHAS, (int)pow(2, QTD_CH));
    
    // Zerar variaveis para que possam ser reutilizadas
    NumeroGeracao = 1;
	MelhorRank = -1;
	MelhorRankAteOMomento = -1;
	MelhorIndividuoGeracao = 0;
	MultiplicadorDeMutacoes = 1;
	Cont = 1;
	Sinal = 0;
	Per = 0;
    
	// -------------------------------- Rotina para a segunda camada de MUX -------------------------------- //
	for(ChaveMUX00 = 0; ChaveMUX00 < ((int)pow(2, QTD_CH)); ChaveMUX00++){ // Rodar uma vez para cada porta do MUX00
		// Gera Ind inicial
		GeraIndividuoInicial(Individuo);
		// Calcula o C deste primeiro Ind
		CalculaC(C, Individuo, AB);
		// Avalia este primeiro Ind
		Individuo[0][3*(NUMERO_NOS-COLUNAS)+3-1] = AvaliaIndividuoParaUmSMUXSegundaCamada(S, Si, C, MatPosLinhas, ChaveMUX00);
		// Preenche geracao com o Ind
		for ( i = 0 ;  i < INDIVIDUOS_POR_GERACAO*QTD_CH ; i+=QTD_CH ){
	        for ( j = 0 ; j < 3*(NUMERO_NOS-COLUNAS)+3 ; j++ ){
	            for (k = 0; k < QTD_CH; k++){
					Geracao[i+k][j] = Individuo[k][j];
	    		}
			}
	    }
	    // Verificamos se e necessario adicionar um novo MUX para essa entrada do MUX00, caso seja entramos em um while
		if (SituacaoPortasMUX00[ChaveMUX00] == 2){ // Se a porta ainda nao esta travada colocamos mais um MUX e entramos na rotina
			printf ("\n\n\nTrabalhando agora no MUX0%d que entra na porta %d do MUX00.\nA seguir a parte da matriz que sera trabalhada neste MUX", ChaveMUX00+1, ChaveMUX00);
			fprintf (ArqSaida, "\n\n\nTrabalhando agora no MUX0%d que entra na porta %d do MUX00.\nA seguir a parte da matriz que sera trabalhada neste MUX", ChaveMUX00+1, ChaveMUX00);
			
			// Imprimindo as matrizes que serao trabalhadas (ou nao) nos proximos MUX
			ImprimeABCSParte (AB, C, S, ChaveMUX00, MatPosLinhas);
			GravaABCSParte (AB, C, S, ChaveMUX00, MatPosLinhas, ArqSaida);
				
			while (NumeroGeracao <= NUMERO_MAX_GERACOES && MelhorRank > -2){
				// Introduz mutacoes nos outros (n-1) Individuos da geracao
				ModificaIndividuosDaGeracao(Geracao, MultiplicadorDeMutacoes);
						
				// Avalia cada novo individuo da geracao
				for ( i = QTD_CH ;  i < INDIVIDUOS_POR_GERACAO*QTD_CH ; i+= QTD_CH ){ 
		        	CalculaC(C, &Geracao[i], AB);
		           	Geracao[i][3*(NUMERO_NOS-COLUNAS)+3-1] = AvaliaIndividuoParaUmSMUXSegundaCamada(S, Si, C, MatPosLinhas, ChaveMUX00);
				}
		        
				// Seleciona o melhor individuo dessa nova geracao
				MelhorIndividuoGeracao = SelecionaMelhorIndividuoDaGeracao(Geracao); // Retorna a posicao do primeiro circuito do melhor individuo da geracao
				MelhorRank = Geracao[MelhorIndividuoGeracao][3*(NUMERO_NOS-COLUNAS)+3-1];
				
		        // Coloca o melhor no topo da geracao
		        for (k = 0; k < QTD_CH; k++){
					for ( j = 0 ; j < 3*(NUMERO_NOS-COLUNAS)+3 ; j++ ){
		            	Geracao[0+k][j] = Geracao[MelhorIndividuoGeracao+k][j];
					}
		        }
		
		        // Individuo recebe o melhor individuo ate o momento
		        for (k = 0; k < QTD_CH; k++){
					for ( j = 0 ; j < 3*(NUMERO_NOS-COLUNAS)+3 ; j++ ){
		            	Individuo[0+k][j] = Geracao[MelhorIndividuoGeracao+k][j];
					}
		        }
				
				// Replica este melhor individuo na matriz Geracao e coloca o melhor individuo em Individuo
			    for ( i = QTD_CH ;  i < INDIVIDUOS_POR_GERACAO*QTD_CH ; i+=QTD_CH ){
		        	for ( j = 0 ; j < 3*(NUMERO_NOS-COLUNAS)+3 ; j++ ){
		            	for (k = 0; k < QTD_CH; k++){
							Geracao[i+k][j] = Geracao[k][j];
		    			}
					}
		    	}
		        
		        if (MelhorRank > MelhorRankAteOMomento){
            		printf("\n\n Evolucao!!!!  Geracao: %d. \t Melhor rank ate esta geracao: %d \t Valor do Multiplicador de Mutacoes : %d \n", NumeroGeracao, MelhorRank, MultiplicadorDeMutacoes);
            		fprintf (ArqSaida, "\n\n Evolucao!!!!  Geracao: %d. \t Melhor rank ate esta geracao: %d \t Valor do Multiplicador de Mutacoes : %d \n", NumeroGeracao, MelhorRank, MultiplicadorDeMutacoes);
		            ImprimeIndividuo(Individuo);
		            GravaIndividuo(Individuo, ArqSaida);
		            MelhorRankAteOMomento = MelhorRank;
		            Cont = 1;
		            MultiplicadorDeMutacoes = 1;
		            Sinal = 0;
		        }else{
		        	Cont++;
		        	if (Sinal == 1){
			        	Per++;
						if (Per > PERSISTENCIA){
							ModificaSaidaIndividuosDaGeracao(Geracao);
							Per = 0;
							MultiplicadorDeMutacoes++;
				        	printf(", %d", MultiplicadorDeMutacoes);
						}
					}
				}		
		
				if (Cont%GERACOES_SEM_MUDANCA == 0){
					MultiplicadorDeMutacoes++;
					Sinal = 1;
					printf("\n\n Apos %d geracoes sem evolucao no individuo, na geracao %d o Multiplicador de Mutacoes recebeu %d", GERACOES_SEM_MUDANCA, NumeroGeracao, MultiplicadorDeMutacoes);
				}
		
				if (MultiplicadorDeMutacoes > (3*(NUMERO_NOS-COLUNAS)+3-2)*2/MAXIMO_MUTACOES){
					MultiplicadorDeMutacoes = 1;
					Sinal = 0;
				}
		
				NumeroGeracao++;
				if (NumeroGeracao%(NUMERO_MAX_GERACOES/20) == 0){
		            printf("\n\n Geracao %d. Melhor rank ate agora: %d. Trabalhando no S%d.", NumeroGeracao, MelhorRank, Si);
		        }
			}
		
		// Imprimir Matriz com o C gerado pelo Individuo que soluciona o MUX
		CalculaC(C, Individuo, AB);
		ImprimeABCSParte (AB, C, S, ChaveMUX00, MatPosLinhas);
		GravaABCSParte (AB, C, S, ChaveMUX00, MatPosLinhas, ArqSaida);
		
		if (MelhorRank < -2){
			printf ("Encontrado na geracao : %d \nCom Rank : %d \nTemos como Individuo que soluciona o MUX0%d para o S%d : ", NumeroGeracao-1, (-1)*MelhorRank, ChaveMUX00+1, Si);
			fprintf (ArqSaida, "Encontrado na geracao : %d \nCom Rank : %d \nTemos como Individuo que soluciona o MUX0%d para o S%d : ", NumeroGeracao-1, (-1)*MelhorRank, ChaveMUX00+1, Si);
		}else{
			printf ("Apos avaliar %d geracoes nao foi possivel encontrar um Individuo que solucionasse o MUX%d para S%d. \nCom um Rank de %d o melhor Individuo encontrado foi : ", NumeroGeracao-1, ChaveMUX00+1, Si, MelhorRankAteOMomento);
			fprintf (ArqSaida, "Apos avaliar %d geracoes nao foi possivel encontrar um Individuo que solucionasse o MUX%d para S%d. \nCom um Rank de %d o melhor Individuo encontrado foi : ", NumeroGeracao-1, ChaveMUX00+1, Si, MelhorRankAteOMomento);
		}
	
		ImprimeIndividuo(Individuo);
		GravaIndividuo(Individuo, ArqSaida);

	    // Vamos reutilizar os vetores usados para encontrar a resposta do MUX00
	    for (i = 0; i < ((int)pow(2, QTD_CH)); i++){
	    	SituacaoPortasMUXAux[i] = RetornaSituacaoPortaMUXCamadaSecundaria(S, Si, C, i, MatPosLinhas, ChaveMUX00);
	    	OcorrenciaDasChaves[i] = RetornaOcorrenciaDaChaveNoMUXCamadaSecundaria(C, i, MatPosLinhas, ChaveMUX00);
		}
		
		printf ("\nSituacao de cada porta do MUX0%d : ", ChaveMUX00+1);
		fprintf (ArqSaida, "\nSituacao de cada porta do MUX0%d : ", ChaveMUX00+1);
		ImprimeVetor(SituacaoPortasMUXAux, ((int)pow(2, QTD_CH)));
		GravaVetor(SituacaoPortasMUXAux, ((int)pow(2, QTD_CH)), ArqSaida);
		printf ("\n\nNumero de vezes que cada porta do MUX0%d foi utilizada : ", ChaveMUX00+1);
		fprintf (ArqSaida, "\n\nNumero de vezes que cada porta do MUX0%d foi utilizada : ", ChaveMUX00+1);
		ImprimeVetor(OcorrenciaDasChaves, ((int)pow(2, QTD_CH)));
		GravaVetor(OcorrenciaDasChaves, ((int)pow(2, QTD_CH)), ArqSaida);
		}
	}
	
	// -------------------- A -------------------- //
	// -------------------- A -------------------- //
	
	fclose(ArqSaida);
}
