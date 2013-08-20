#include<stdio.h>
#include<stdlib.h>

/*LEMBRANDO QUE AQUI SÓ TEM EXEMPLOS PARA LER IMAGENS ASCII*/

char iname[100], oname[100]; //Nome de Entrada e Saída da imagem
char p;	//Apenas para usar apenas o número que identifica o tipo da imagem, esse char armazena sempre 'P'
int key, lin, col; //Dados usados em praticamente todas as funções
//key --tipo da imagem (Ex:. P1, P2, P3, P4, P5, P6)
//lin --Nº de linhas, col --Nº de colunas, R, G, B --Intensidade ou brilho do pixel

void InvCores(){
	int R, G, B, img[10000], i;
	printf("INVERTENDO AS CORES DA IMAGEM SOLICITADA...\n");
	FILE *imgi, *imgo; //arquivos de entrada e saida respectivamente
	imgi = fopen(iname, "r"); //abrindo a imagem apenas para leitura, para garantir que a imagem original não será alterada
	printf("DIGITE O NOME DA IMAGEM DE SAÌDA: ");
	scanf("%s", oname);
	imgo = fopen(oname, "w"); //criação do arquivo de saida da imagem
	fscanf(imgi, "%c%d", &p,&key);		//leitura
	fprintf(imgo, "%c%d\n", p,key);		//impressão do tipo de imagem
	fscanf(imgi, "%d %d", &col, &lin);	//leitura
	fprintf(imgo, "%d %d\n", col, lin);	//impressão das linhas e colunas
	switch (key) {
		case 1:  //Se for P1, no formato .pbm (ASCII)
			for(i=0; i<=col*lin;i++){
				fscanf(imgi, "%d", &img[i]);
				if(img[i]==1)
					fprintf(imgo, "%d ", 0);
				else
					fprintf(imgo, "%d ", 1);
			}
			fclose(imgi);
			fclose(imgo);
			break;
		case 2:  //Se for P2 no formato .pgm (ASCII)
			fscanf(imgi, "%d", &B);
			fprintf(imgo, "%d\n", B);
			for(i=0; i<=col*lin;i++){
				fscanf(imgi, "%d", &img[i]);
				fprintf(imgo, "%d ", B-img[i]);
			}
			fclose(imgi); fclose(imgo);
			break;
		case 3:  //Se for P3 no formato .ppm (ASCII)
			fscanf(imgi, "%d %d %d", &R, &G, &B);
			fprintf(imgo, "%d %d %d\n", R, G, B);
			for(i=0; i<=col*lin;i++){
				fscanf(imgi, "%d", &img[i]);
				fprintf(imgo, "%d ", R-img[i]);
				fscanf(imgi, "%d", &img[i]);
				fprintf(imgo, "%d ", G-img[i]);
				fscanf(imgi, "%d", &img[i]);
				fprintf(imgo, "%d ", B-img[i]);
			}
			fclose(imgi); fclose(imgo);
			break;
		case 4: break;
		case 5: break;
		case 6: break;

	}
	printf("\nPROCESSO CONCLUIDO\n");
}

void Espelhar(){
//por enquando está usando a imagem como um vetor de pixels
//porém, acho que na proxima atualização vai usar matriz

//Ainda tem bugs no formato .pgm

	int temp, img[10000], i;
	printf("ESPELHANDO IMAGEM SOLICITADA...\n");
	FILE *imgi, *imgo; //arquivos de entrada e saida respectivamente
	imgi = fopen(iname, "r"); //abrindo a imagem apenas para leitura, para garantir que a imagem original não será alterada
	printf("DIGITE O NOME DA IMAGEM DE SAÌDA: ");
	scanf("%s", oname);
	imgo = fopen(oname, "w"); //criação do arquivo de saida da imagem
	fscanf(imgi, "%c%d", &p,&key);		//leitura
	fprintf(imgo, "%c%d\n", p,key);		//impressão do tipo de imagem
	fscanf(imgi, "%d %d", &col, &lin);	//leitura
	fprintf(imgo, "%d %d\n", col, lin);	//impressão das linhas e colunas
	switch (key) {
		case 1:  //Se for P1, no formato .pbm (ASCII)
			for(i=0; i<=col*lin;++i)
				fscanf(imgi, "%d", &img[i]);				
			for(i=col*lin; i>=0;--i)
				fprintf(imgo, "%d ", img[i]);
			fclose(imgi);
			fclose(imgo);
			break;
		case 2:  //Se for P2 no formato .pgm (ASCII)
			fscanf(imgi, "%d", &temp);
			fprintf(imgo, "%d\n", temp);
			for(i=0;i<=col*lin;++i)
				fscanf(imgi, "%d", &img[i]);
			for(i=col*lin; i>=0;--i)
				fprintf(imgo, "%d ", img[i]);
			fclose(imgi); fclose(imgo);
			break;
		case 3: break;
		case 4: break;
		case 5: break;
		case 6: break;

	}
	printf("\nPROCESSO CONCLUIDO\n");
}

int main(){
	char opc;
	do {
		printf("\nDigite uma funcao\n1-Inverter Cores\n2-Espelhar\nQ-QUIT ");
		scanf("%c", &opc); //Tem um bug aqui, se ler a variavel opc uma vez só 
		opc = getchar();   //o programa só lê no primeiro loop depois ignora
		if(opc != 'Q'){
			printf("Digite a imagem de entrada: ");
			scanf("%s", iname);
		}
		if(opc=='1')
			InvCores();
		else
		{
			if(opc=='2')
				Espelhar();
			else
				if(opc != 'Q')
					printf("\nVoce digitou uma opção invalida!!\n");
		}
	}while(opc != 'Q');
return 0;
}
