#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <memory.h>
#include <string.h>
#include <math.h>

#include "cabecalho.h"

#define MAX(X,Y)((X > Y) ? (X) : (Y))

const int PESO_CAP = 100;

int main()
{
   // srand(time(0));
    Solucao sol, clone;
    ler_dados("instancias/instancia-toy.txt");
    testar_dados(" ");

    /*
    sol.mat_sol[0][0] = 1;
    sol.mat_sol[0][1] = 3;
    sol.mat_sol[0][2] = 5;
    sol.mat_sol[1][0] = 2;
    sol.mat_sol[1][1] = 4;
    sol.vet_qtd_cli_vei[0] = 3;
    sol.vet_qtd_cli_vei[1] = 2;

    sol.vet_sol[0] = 0;
    sol.vet_sol[1] = 1;
    sol.vet_sol[2] = 3;
    sol.vet_sol[3] = 5;
    sol.vet_sol[4] = 0;
    sol.vet_sol[5] = 2;
    sol.vet_sol[6] = 4;
    sol.vet_sol[7] = 0;
*/

    heu_cons_ale_gul(sol, 10);
    calcular_fo(sol);

    printf("\n\n\n");
    for (int i = 0; i < num_vei; i++)
    {
        for (int j = 0; j < sol.vet_qtd_cli_vei[i]; j++)
            printf("%d ", sol.mat_sol[i][j]);
        printf("\n");
    }
    printf("\nQTD: ");
    for (int i = 0; i < num_vei; i++)
        printf("%d ", sol.vet_qtd_cli_vei[i]);

    printf("\nPESO: ");
    for (int i = 0; i < num_vei; i++)
        printf("%d ", sol.vet_pes_vei[i]);

    printf("\n\nFO: %.2f\n", sol.fo);


    //calcular_fo2(sol);
    //printf("\n\nFO: %.2f\n", sol.fo);

    printf("\n VIZINHAN�A \n");
    gerar_vizinho(sol);
    calcular_fo(sol);

    printf("\n\n\n");
    for (int i = 0; i < num_vei; i++)
    {
        for (int j = 0; j < sol.vet_qtd_cli_vei[i]; j++)
            printf("%d ", sol.mat_sol[i][j]);
        printf("\n");
    }
    printf("\nQTD: ");
    for (int i = 0; i < num_vei; i++)
        printf("%d ", sol.vet_qtd_cli_vei[i]);

    printf("\nPESO: ");
    for (int i = 0; i < num_vei; i++)
        printf("%d ", sol.vet_pes_vei[i]);

    printf("\n\nFO: %.2f\n", sol.fo);

    return 0;
}

void criar_solucao(Solucao& s)
{
    int vei = 0;
    memset(&s.vet_qtd_cli_vei, 0, sizeof(s.vet_qtd_cli_vei));
    for (int j = 1; j <= num_cli; j++)
    {
        vei = vei % num_vei;
        s.mat_sol[vei][s.vet_qtd_cli_vei[vei]] = j;
        s.vet_qtd_cli_vei[vei]++;
        vei++;
        //if (vei == num_vei)
        //    vei = 0;
    }
}

void criar_he_ale(Solucao& s){
    memset(&s.vet_qtd_cli_vei, 0, sizeof(s.vet_qtd_cli_vei));
    int vetor[num_cli];

    for(int i = 0; i < num_cli; i++){
        vetor[i] = i + 1;
    }

   // Embaralha os elementos
    for (int i = num_cli - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        std::swap(vetor[i], vetor[j]);
    }

    int vei, cli;
    for(int i = 0; i < num_cli; i++){
        vei = rand() % num_vei;
        s.mat_sol[vei][s.vet_qtd_cli_vei[vei]] = vetor[i];
        s.vet_qtd_cli_vei[vei]++;
    }
}

int cliente_maior_demanda() {
    int maior = -1;
    int indice = -1;

    for (int i = 1; i <= num_cli; i++) {
        if (vet_dem_cli[i] > maior) {
            maior = vet_dem_cli[i];
            indice = i;
        }
    }

    return indice;
}

void criar_he_gu(Solucao& s){
    memset(&s.vet_qtd_cli_vei, 0, sizeof(s.vet_qtd_cli_vei));
    memset(s.vet_pes_vei, 0, sizeof(s.vet_pes_vei));

    bool visitado[MAX_CLI] = {false};
    visitado[0] = true;
    int cli_maior = cliente_maior_demanda();
    for (int j = 0; j < num_vei; j++) {
        if (visitado[cli_maior]) continue;
        if (vet_dem_cli[cli_maior] <= vet_cap_vei[j]) {
            int pos = s.vet_qtd_cli_vei[j];
            s.mat_sol[j][pos] = cli_maior;
            s.vet_qtd_cli_vei[j]++;
            s.vet_pes_vei[j] += vet_dem_cli[cli_maior];
            visitado[cli_maior] = true;
            break;
        }
    }

    for (int i = 1; i <= num_cli; i++) {
        if (visitado[i]) continue;
        for (int j = 0; j < num_vei; j++) {
            if (s.vet_pes_vei[j] + vet_dem_cli[i] <= vet_cap_vei[j]) {
                int pos = s.vet_qtd_cli_vei[j];
                s.mat_sol[j][pos] = i;
                s.vet_qtd_cli_vei[j]++;
                s.vet_pes_vei[j] += vet_dem_cli[i];
                visitado[i] = true;
                break;
            }
        }
    }
}

void heu_cons_ale_gul(Solucao& s, const double& percentual){
    int qtd = MAX(1, (percentual / 100.00) * num_cli);
    printf(" %d ", qtd);
    int vetor[num_cli];
    int vei, cli;
    int cli_maior = cliente_maior_demanda();
    bool visitado[MAX_CLI] = {false};
    visitado[0] = true;

    memset(&s.vet_qtd_cli_vei, 0, sizeof(s.vet_qtd_cli_vei));
    memset(s.vet_pes_vei, 0, sizeof(s.vet_pes_vei));

    for(int i = 0; i < qtd; i++){
        vetor[i] = i + 1;
    }

   // Embaralha os elementos
    for (int i = qtd - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        std::swap(vetor[i], vetor[j]);
    }

    for(int i = 0; i < qtd; i++){
        vei = rand() % num_vei;
        s.mat_sol[vei][s.vet_qtd_cli_vei[vei]] = vetor[i];
        s.vet_qtd_cli_vei[vei]++;
    }

    for (int j = 0; j < num_vei; j++) {
        if (visitado[cli_maior]) continue;
        if (vet_dem_cli[cli_maior] <= vet_cap_vei[j]) {
            int pos = s.vet_qtd_cli_vei[j];
            s.mat_sol[j][pos] = cli_maior;
            s.vet_qtd_cli_vei[j]++;
            s.vet_pes_vei[j] += vet_dem_cli[cli_maior];
            visitado[cli_maior] = true;
            break;
        }
    }

    for (int i = qtd; i < num_cli; i++) {
        if (visitado[i]) continue;
        for (int j = 0; j < num_vei; j++) {
            if (s.vet_pes_vei[j] + vet_dem_cli[i] <= vet_cap_vei[j]) {
                int pos = s.vet_qtd_cli_vei[j];
                s.mat_sol[j][pos] = i;
                s.vet_qtd_cli_vei[j]++;
                s.vet_pes_vei[j] += vet_dem_cli[i];
                visitado[i] = true;
                break;
            }
        }
    }
}

void remover_cliente(Solucao& s, const int& vei, const int&pos){
    for (int i = pos; i < s.vet_qtd_cli_vei[vei] - 1; i++)
        s.mat_sol[vei][i] = s.mat_sol[vei][i + 1];
    s.vet_qtd_cli_vei[vei]--;
}

void inserir_cliente(Solucao& s, const int& vei, const int&cli){
    s.mat_sol[vei][s.vet_qtd_cli_vei[vei]] = cli;
    s.vet_qtd_cli_vei[vei]++;
}

void gerar_vizinho(Solucao& s){
    // Sortear um veiculo e um cliente

    int vei1, vei2, pos, cli;

    do{
        vei1 = rand() % num_vei;
    } while(s.vet_qtd_cli_vei[vei1] == 0);

    do {
            vei2 = rand() % num_vei;}
    while(vei1 == vei2);

    pos = rand() % s.vet_qtd_cli_vei[vei1];
    cli = s.mat_sol[vei1][pos];
    printf(" %d ", pos);
    remover_cliente(s, vei1, pos);
    inserir_cliente(s, vei2, cli);
}

void calcular_fo(Solucao& s)
{
    s.fo = 0;
    memset(&s.vet_pes_vei, 0, sizeof(s.vet_pes_vei));
    for (int i = 0; i < num_vei; i++) // linha
    {
        s.fo += mat_custo[0][s.mat_sol[i][0]];
        s.vet_pes_vei[i] += vet_dem_cli[s.mat_sol[i][0]];
        for (int j = 1; j < s.vet_qtd_cli_vei[i]; j++) // coluna
        {
            s.fo += mat_custo[s.mat_sol[i][j-1]][s.mat_sol[i][j]];
            s.vet_pes_vei[i] += vet_dem_cli[s.mat_sol[i][j]];
        }
        s.fo += mat_custo[s.mat_sol[i][s.vet_qtd_cli_vei[i]-1]][0];
        //---
        s.fo += PESO_CAP * MAX(0, s.vet_pes_vei[i] - vet_cap_vei[i]);
    }
}

/*
void calcular_fo2(Solucao& s)
{
    int vei = 0;
    s.fo = 0;
    memset(&s.vet_pes_vei, 0, sizeof(s.vet_pes_vei));
    for (int j = 1; j < num_cli + num_vei + 1; j++)
    {
        if (s.vet_sol[j] == 0)
            vei++;
        s.fo += mat_custo[s.vet_sol[j-1]][s.vet_sol[j]];
        s.vet_pes_vei[vei] += vet_dem_cli[s.vet_sol[j]];
    }
    for (int i = 0; i < num_vei; i++)
        s.fo += PESO_CAP * MAX(0, s.vet_pes_vei[i] - vet_cap_vei[i]);
}*/

void testar_dados(char* arq)
{
    FILE* f;
    if (!strcmp(arq, " "))
        f = stdout;
    else
        f = fopen(arq, "w");
    fprintf(f, "%d %d\n", num_cli, num_vei);
    for (int i = 0; i < num_vei; i++)
        fprintf(f, "%d ", vet_cap_vei[i]);
    fprintf(f, "\n");
    for (int i = 0; i <= num_cli; i++)
    {
        for (int j = 0; j <= num_cli; j++)
            printf("%.2f ", mat_custo[i][j]);
        fprintf(f, "\n");
    }
    if (strcmp(arq, " "))
        fclose(f);
}

void ler_dados(char* arq)
{
    int aux;
    int vetX[MAX_CLI];
    int vetY[MAX_CLI];
    FILE* f = fopen(arq, "r");
    fscanf(f, "%d %d", &num_cli, &num_vei);
    for (int j = 0; j <= num_cli; j++)
        fscanf(f, "%d %d %d %d", &aux, &vetX[j], &vetY[j], &vet_dem_cli[j]);
    for (int i = 0; i < num_vei; i++)
        fscanf(f, "%d", &vet_cap_vei[i]);
    fclose(f);
    //---
    for (int i = 0; i <= num_cli; i++)
        for (int j = 0; j <= num_cli; j++)
            mat_custo[i][j] = sqrt(pow(vetX[i]-vetX[j],2)+pow(vetY[i]-vetY[j],2));
}
