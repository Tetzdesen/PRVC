#ifndef CABECALHO_H_INCLUDED
#define CABECALHO_H_INCLUDED

#define MAX_CLI 501
#define MAX_VEI 100

typedef struct tSolucao{
    int mat_sol[MAX_VEI][MAX_CLI];
    int vet_qtd_cli_vei[MAX_VEI];
    int vet_sol[MAX_CLI+MAX_VEI+1];

    int vet_pes_vei[MAX_VEI];
    double fo;
}Solucao;

int num_cli, num_vei;
int vet_dem_cli[MAX_CLI];
int vet_cap_vei[MAX_VEI];
double mat_custo[MAX_CLI][MAX_CLI];

void criar_solucao(Solucao& s);
void criar_he_ale(Solucao& s);
void criar_he_gu(Solucao& s);

void remover_cliente(Solucao& s, const int& vei, const int&pos);
void inserir_cliente(Solucao& s, const int& vei, const int&cli);

void gerar_vizinho(Solucao& s);
void calcular_fo2(Solucao& s);
void calcular_fo(Solucao& s);
void testar_dados(char* arq);
void ler_dados(char* arq);


#endif // CABECALHO_H_INCLUDED
