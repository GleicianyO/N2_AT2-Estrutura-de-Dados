# N2_AT2-Estrutura-de-Dados
Prova do segundo bimestre da matéria estrutura de dados.

É recomendado utilizar o programa na seguinte ordem: programa3, programa1 e por ultimo programa2.

---
**Programa 1:**

Esse programa tem como objetivo ler um arquivo.txt que tenha informações no formato: <TIMESTAMP> <ID_SENSOR> <VALOR>, e identificar quais sensores estão presentes nesse arquivo e organizar eles criando um novo .txt com o nome de cada sensor presente. (a criação de um arquivo com as leituras será um trabalho do programa 3.


A **entrada** desse código é feita desta maneira: "./programa1 leituras.txt". E a sua **saída** dependerá dos sensores presentes, mas ela informara quantos sensores foram encontrados, se a tarefa foi finalizada e cria arquivos no mesmo diretório com a seguinte sintaxe: "sensor.txt" ou apresenta algum erro.


Foi utilizado o algoritmo de ordenação "quicksort" da biblioteca <stdlib.h> para cumprir o objetivo de "ordenar as leituras por timestamp em ordem crescente.".


**Estrutura do programa1:** 
"identificar_tipo": recebe um valor em string e identifica se ele é int, float, bool ou uma string.
"obter_sensores": busca ou se não existir cria sensores por id.
"comparação_leituras": usa o quicksort para ordenar de forma crescente.
"salvar_sensores_nos_arquivos": grava os timestamps ordenados com o sensor e o id, uma em cada linha, cria um arquivo com o nome do sensor nele e por último fecha o arquivo.

---
**Programa 2:**

Esse programa tem como objetivo pesuisar a medida de um sensor em específico com base em uma data e hora informada pelo usuário.


A **entrada** é feita por meio de um comando mandado ao terminal no formato: "./programa2 <sensor> dd mm aaaa hh mm ss" e entrega na **saída**:"Leitura mais próxima encontrada foi: Timestamp: dd/mm/aaaa Sensor:<id_sendor> Valor:<valor>" ou apresenta algum erro.


**Estrutura do programa2:** 
"criar_timestamps": Cria e imprime um timestamp baseado na data e hora fornecidos pelo usuário e valida ela.
"busca_binaria": realiza a busca binária para encontrar o valor mais próximo ao solicitado pelo usuário.
"validar_argumentos": valida se o número de argumentos mandados pelo usuário é a quantidade pedida e se estiver certo ele copia e converte a string dos argumentos em data e hora para inteiros.
"carregar_leituras": abre o arquivo <id_sensor>.txt lê e armazena os dados.
"exibir_leitura_proxima": recebe as leituras e um índice, converte o timestamp para o formato padão de data e hora e imprime no console os dados.

---
**Programa 3:**

Cria um arquivo teste com data e hora do início e fim gerados aleatoriamente, nome dos sensores escolhidos pelo usuário e o tipo de amostra a ser representado naquele dado, também escolhido pelo usuário, e cria 2000 leituras aleatórias com aqueles dados.


A **entrada** é feita por meio de um comando mandado ao terminal no formato: "./programa3 dd_ini mm_ini aaaa_ini hr_ini mm_ini ss_ini dd_fin mm_fin aaaa_fin hr_fin mm_fin ss_fin <sensor1> <tipo1> <sensor2> <tipo2>...." (não necessáriamente precisa ter 2 sensores, apenas precisa de 1 obrigatóriamente). E a **saída** é feita desta maneira: "Arquivo com o nome 'leituras.txt' gerado com sucesso." ou apresenta algum erro.


**Estrutura do programa3:** 
"conversao_para_valor_equivalentes": converte a string do tipo que o usuário escolheu para um valor string, float, bool ou int.
"criar_timestamp": cria um timestamp apartir de uma data e horário e converte para uma estrutura UNIX(timestamp).
"timestamp_aleatorio": gera o timestamp aleatório, apartir das duas datas (início e fim).
"criar_string_random": gera uma string aleátoria.
"ler_timestamps": lê os argumentos mandados pelo usuário e converte para timestamps e verifica se a data é válida.
"ler_sensores": lê os argumentos mandados pelo usuário e armazena o nome dos sensores e o tipo de dado.
"gerar_dados": gera 2000 leituras dos dados informados na linha de comando mandada pelo usuário e escreve no formato <timestamp> <sensor> <valor>

---
