# N2_AT2-Estrutura-de-Dados
Prova do segundo bimestre da matéria estrutura de dados.

É recomendado utilizar o programa na seguinte ordem: programa3, programa1 e por ultimo programa2.

Programa 1: 
Esse programa tem como objetivo ler um arquivo.txt que tenha informações no formato: <TIMESTAMP> <ID_SENSOR> <VALOR>, e identificar quais sensores estão presentes nesse arquivo e organizar eles criando um novo .txt com o nome de cada sensor presente. (a criação de um arquivo com as leituras será um trabalho do programa 3.

A entrada desse código é feita desta maneira: "./programa1 leituras.txt". E a sua saída dependerá dos sensores presentes, mas ela informara quantos sensores foram encontrados e se a tarefa foi finalizada.

Foi utilizado o algoritmo de ordenação "quicksort" da biblioteca <stdlib.h> para cumprir o objetivo de "ordenar as leituras por timestamp em ordem crescente.".

Estrutura do código: 
"identificar_tipo": recebe um valor em string e identifica se ele é int, float, bool ou uma string.
"obter_sensores": 
