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
    srand(time(0));
    Solucao sol, clone;
    ler_dados("instancias/instancia-toy.txt");
    testar_dados(" ");

    heu_cons_ale_gul(sol, 20);
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

    memcpy(&clone, &sol, sizeof(sol));

    printf("\n VIZINHANCA \n");
    gerar_vizinho(clone);
    calcular_fo(clone);

    printf("\n\n\n");
    for (int i = 0; i < num_vei; i++)
    {
        for (int j = 0; j < clone.vet_qtd_cli_vei[i]; j++)
            printf("%d ", clone.mat_sol[i][j]);
        printf("\n");
    }
    printf("\nQTD: ");
    for (int i = 0; i < num_vei; i++)
        printf("%d ", clone.vet_qtd_cli_vei[i]);

    printf("\nPESO: ");
    for (int i = 0; i < num_vei; i++)
        printf("%d ", clone.vet_pes_vei[i]);

    printf("\n\nFO: %.2f\n", clone.fo);

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
    }
}

void criar_he_ale(Solucao& s){
    memset(&s.vet_qtd_cli_vei, 0, sizeof(s.vet_qtd_cli_vei));
    memset(&s.vet_pes_vei, 0, sizeof(s.vet_pes_vei));
    memset(visitado, 0, sizeof(visitado));

    visitado[0] = 1;

    int counter = 0, cli, vei, pos;
    while (counter < num_cli) {
        cli = 1 + rand() % num_cli;
        if (visitado[cli]) continue;

        vei = rand() % num_vei;
        pos = s.vet_qtd_cli_vei[vei];

        s.mat_sol[vei][pos] = cli;
        s.vet_qtd_cli_vei[vei]++;
        s.vet_pes_vei[vei] += vet_dem_cli[cli];
        visitado[cli] = 1;
        counter++;
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

int cliente_menor_custo(int linha) {
    int menor = 2147483647;
    int indice = -1;

    for (int j = 1; j <= num_cli; j++) {
        if (j == linha || visitado[j]) continue;

        if (mat_custo[linha][j] < menor) {
            menor = mat_custo[linha][j];
            indice = j;
        }
    }

    return indice;
}

void criar_he_gu(Solucao& s) {
    memset(&s.vet_qtd_cli_vei, 0, sizeof(s.vet_qtd_cli_vei));
    memset(s.vet_pes_vei, 0, sizeof(s.vet_pes_vei));
    memset(visitado, 0, sizeof(visitado));

    visitado[0] = 1;

    int cli_maior = cliente_maior_demanda();
    int cli_menor;

    // Aloca o cliente de maior demanda primeiro
    for (int j = 0; j < num_vei; j++) {
        if (visitado[cli_maior]) break;
        if (vet_dem_cli[cli_maior] <= vet_cap_vei[j]) {
            int pos = s.vet_qtd_cli_vei[j];
            s.mat_sol[j][pos] = cli_maior;
            s.vet_qtd_cli_vei[j]++;
            s.vet_pes_vei[j] += vet_dem_cli[cli_maior];
            visitado[cli_maior] = 1;
            break;
        }
    }

    // Aloca os demais clientes baseando-se no custo relativo
    int clientes_alocados = 1;
    while (clientes_alocados < num_cli) {
        cli_menor = cliente_menor_custo(cli_maior); // recalcula sempre

        // se todos os possíveis foram visitados, termina
        if (visitado[cli_menor]) break;

        int alocado = 0;
        for (int j = 0; j < num_vei; j++) {
            if (s.vet_pes_vei[j] + vet_dem_cli[cli_menor] <= vet_cap_vei[j]) {
                int pos = s.vet_qtd_cli_vei[j];
                s.mat_sol[j][pos] = cli_menor;
                s.vet_qtd_cli_vei[j]++;
                s.vet_pes_vei[j] += vet_dem_cli[cli_menor];
                visitado[cli_menor] = 1;
                clientes_alocados++;
                cli_maior = cli_menor; // atualiza referência para próxima busca
                alocado = 1;
                break;
            }
        }

        if (!alocado) break; // não foi possível alocar o cliente atual
    }
}


void heu_cons_ale_gul(Solucao& s, const double& percentual){

    int qtd = MAX(1, (percentual / 100.00) * num_cli);
    memset(&s.vet_qtd_cli_vei, 0, sizeof(s.vet_qtd_cli_vei));
    memset(&s.vet_pes_vei, 0, sizeof(s.vet_pes_vei));
    memset(visitado, 0, sizeof(visitado));

    visitado[0] = 1;

    int counter = 0, cli, vei, pos;
    while (counter < qtd) {
        cli = 1 + rand() % qtd;
        if (visitado[cli]) continue;

        vei = rand() % num_vei;
        pos = s.vet_qtd_cli_vei[vei];

        s.mat_sol[vei][pos] = cli;
        s.vet_qtd_cli_vei[vei]++;
        s.vet_pes_vei[vei] += vet_dem_cli[cli];
        visitado[cli] = 1;
        counter++;
    }

    int cli_maior = cliente_maior_demanda();
    int cli_menor;

    // Aloca o cliente de maior demanda primeiro
    for (int j = 0; j < num_vei; j++) {
        if (visitado[cli_maior]) break;
        if (vet_dem_cli[cli_maior] <= vet_cap_vei[j]) {
            int pos = s.vet_qtd_cli_vei[j];
            s.mat_sol[j][pos] = cli_maior;
            s.vet_qtd_cli_vei[j]++;
            s.vet_pes_vei[j] += vet_dem_cli[cli_maior];
            visitado[cli_maior] = 1;
            break;
        }
    }

    // Aloca os demais clientes baseando-se no custo relativo
    int clientes_alocados = qtd+1;
    while (clientes_alocados < num_cli) {
        cli_menor = cliente_menor_custo(cli_maior); // recalcula sempre

        // se todos os possíveis foram visitados, termina
        if (visitado[cli_menor]) break;

        int alocado = 0;
        for (int j = 0; j < num_vei; j++) {
            if (s.vet_pes_vei[j] + vet_dem_cli[cli_menor] <= vet_cap_vei[j]) {
                int pos = s.vet_qtd_cli_vei[j];
                s.mat_sol[j][pos] = cli_menor;
                s.vet_qtd_cli_vei[j]++;
                s.vet_pes_vei[j] += vet_dem_cli[cli_menor];
                visitado[cli_menor] = 1;
                clientes_alocados++;
                cli_maior = cli_menor; // atualiza referência para próxima busca
                alocado = 1;
                break;
            }
        }

        if (!alocado) break; // não foi possível alocar o cliente atual
    }

}

void remover_cliente(Solucao& s, const int& vei, const int& pos) {
    int cli = s.mat_sol[vei][pos]; // Salva antes de deslocar
    for (int i = pos; i < s.vet_qtd_cli_vei[vei] - 1; i++)
        s.mat_sol[vei][i] = s.mat_sol[vei][i + 1];
    s.vet_qtd_cli_vei[vei]--;
    s.vet_pes_vei[vei] -= vet_dem_cli[cli];
}

void inserir_cliente(Solucao& s, const int& vei, const int& pos, const int& cli) {
    for (int i = s.vet_qtd_cli_vei[vei]; i > pos; i--)
        s.mat_sol[vei][i] = s.mat_sol[vei][i - 1];
    s.mat_sol[vei][pos] = cli;
    s.vet_qtd_cli_vei[vei]++;
    s.vet_pes_vei[vei] += vet_dem_cli[cli];
}

void gerar_vizinho(Solucao& s) {
    int vei1, vei2, pos1, pos2, cli;

    do {
        vei1 = rand() % num_vei;
    } while (s.vet_qtd_cli_vei[vei1] == 0);

    do {
        vei2 = rand() % num_vei;
    } while ((vei1 == vei2));

    pos1 = rand() % s.vet_qtd_cli_vei[vei1];
    cli = s.mat_sol[vei1][pos1];

    // Verifica se cabe no novo veículo
    if (s.vet_pes_vei[vei2] + vet_dem_cli[cli] > vet_cap_vei[vei2])
        return;

    pos2 = rand() % (s.vet_qtd_cli_vei[vei2] + 1);

    remover_cliente(s, vei1, pos1);
    inserir_cliente(s, vei2, pos2, cli);
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