# Projeto PRVC — Problema de Roteamento de Veículos Capacitados

## 📘 Descrição

Este projeto implementa um modelo inicial para o **Problema de Roteamento de Veículos Capacitados (PRVC)**. O PRVC é um problema clássico de otimização combinatória cujo objetivo é encontrar um conjunto de rotas de menor custo para atender a um conjunto de clientes, respeitando restrições de capacidade e de uso de veículos.

## 📌 Objetivos

- Ler uma instância do PRVC a partir de um arquivo no formato especificado.
- Representar uma solução viável utilizando estruturas de dados adequadas.
- Calcular a função objetivo de uma solução, penalizando violações de restrições.

## 📁 Estrutura dos Arquivo

- `instancias/*.txt`  
  Diretório com instâncias de teste, incluindo a instância `toy`.

## 🧾 Formato da Instância

```
<numero_de_clientes> <numero_de_veiculos>
<id_deposito> <x> <y> <demanda>
<id_cliente_1> <x> <y> <demanda>
...
<id_cliente_n> <x> <y> <demanda>
<capacidade_veiculo_1> ... <capacidade_veiculo_k>
```

## 🔧 Funcionalidades Implementadas

- Leitura de instância a partir de um arquivo texto.
- Cálculo das distâncias euclidianas entre os nós.
- Estrutura de dados para representar uma solução com todas as informações necessárias (clientes atendidos, rotas, custo, etc.).
- Cálculo da função objetivo considerando penalidades por inviabilidade (por exemplo, excesso de carga, clientes não atendidos, etc.).

## 🧪 Testes

Use a instância `toy` para depuração. 

## 🛠 Requisitos

- Linguagem: C ou C++
- Capacidade: até 500 clientes e 100 veículos
- Compilador compatível: `g++` ou `clang++`

## 📎 Observações

- Todas as variáveis de entrada devem ser globais.
