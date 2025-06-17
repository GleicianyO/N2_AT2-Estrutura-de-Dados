#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum
{
    t_int,
    t_float,
    t_bool,
    t_string,
    t_invalido
} TiposDeDados;

typedef struct
{
    long timestamp;
    char valor[100];
} Leituras;

typedef struct 
{
    char id[50];
    TiposDeDados dados;
    Leituras leitura[10000];
    int total;
} Sensor;

Sensor sensores[100];
int num_sensores = 0;

TiposDeDados identificar_tipo(const char *valor){
    if (strcmp(valor, "true") == 0 || strcmp(valor, "false") == 0) return t_bool;

    int ponto = 0, i = 0, digitos = 0;
    if (valor[0] == '+' || valor[0] == '-') i++;

    for (; valor[i]; i++) {
        if (isdigit(valor[i])) digitos++;
        else if (valor[i] == '.' && !ponto) ponto = 1;
        else return t_string;
    }

    if (digitos == 0) {
        return t_string;
    } else {
        if (ponto) {
            return t_float;
        } else {
            return t_int;
        }
    }
}

Sensor *obter_sensor(const char *id, const char *valor){
    for (int i = 0; i < num_sensores; i++) {
        if (strcmp(sensores[i].id, id) == 0) return &sensores[i];
    }

    if (num_sensores >= 100) {
        fprintf(stderr, "Erro: número máximo de sensores atingido.\n");
        exit(EXIT_FAILURE);
    }

    Sensor *new = &sensores[num_sensores++];
    strncpy(new->id, id, 49);
    new->id[49] = '\0';
    new->dados = identificar_tipo(valor);
    new->total = 0;
    return new;
}

int comparacao_leituras(const void *a, const void *b){
    const Leituras *la = a, *lb = b;
    if (lb->timestamp < la->timestamp) return -1;
    if (lb->timestamp > la->timestamp) return 1;
    return 0;
}

void salvar_sensores_nos_arquivos(){
    for (int i = 0; i < num_sensores; i++) {
        Sensor *sensor = &sensores[i];

        qsort(sensor->leitura, sensor->total, sizeof(Leituras), comparacao_leituras);

        char nomeArquivo[128];
        snprintf(nomeArquivo, sizeof(nomeArquivo), "%s.txt", sensor->id);

        FILE *arquivo = fopen(nomeArquivo, "w");
        if (!arquivo) {
            fprintf(stderr, "Aviso: não foi possível criar '%s'\n", nomeArquivo);
            continue;
        }

        for (int j = 0; j < sensor->total; j++) {
            fprintf(arquivo, "%ld %s %s\n", sensor->leitura[j].timestamp, sensor->id, sensor->leitura[j].valor);
        }

        fclose(arquivo);
    }
}

int main(int argc, char *argv[]){

    if (argc != 2) {
        fprintf(stderr, "\n\nErro: número incorreto de argumentos.\n");
        fprintf(stderr, "\nUso correto: %s <arquivo_de_entrada>\n", argv[0]);
        fprintf(stderr, "\nDica: você deve fornecer o nome de um arquivo com os dados de entrada. Ex: leituras.txt .\n\n");
        return EXIT_FAILURE;
    }

    FILE *arquivo_entrada = fopen(argv[1], "r");
    if (!arquivo_entrada) {
        perror("\n\nErro ao abrir arquivo para leitura. Verifique se o arquivo existe ou se foi escrito de maneira errada.\n\n");
        return EXIT_FAILURE;
    }

    long timestamp;
    char id[50] = {0};
    char valor[100] = {0};
    int linha = 0;

    while (1) {
        linha++;

        id[0] = '\0';
        valor[0] = '\0';

        int campos = fscanf(arquivo_entrada, "%ld %49s %99s", &timestamp, id, valor);
        if (campos == EOF) break;

        if (campos != 3) {
            fprintf(stderr, "Linha %d inválida: formato incorreto (esperado: <timestamp> <id_sensor> <valor>)\n", linha);
            int ch;
            while ((ch = fgetc(arquivo_entrada)) != '\n' && ch != EOF);
            continue;
        }

        if (timestamp < 0) {
            fprintf(stderr, "Linha %d ignorada: timestamp é negativo (%ld)\n", linha, timestamp);
            continue;
        }

        TiposDeDados tipo_detectado = identificar_tipo(valor);
        Sensor *sensor = obter_sensor(id, valor);

        if (sensor->dados != tipo_detectado) {
            fprintf(stderr, "Linha %d ignorada: tipo conflitante para sensor '%s' (esperado: %d, encontrado: %d)\n",
                    linha, sensor->id, sensor->dados, tipo_detectado);
            continue;
        }

        if (sensor->total >= 10000) {
            fprintf(stderr, "Linha %d ignorada: limite de leituras para '%s'\n", linha, sensor->id);
            continue;
        }

        sensor->leitura[sensor->total].timestamp = timestamp;
        strncpy(sensor->leitura[sensor->total].valor, valor, 99);
        sensor->leitura[sensor->total].valor[99] = '\0';
        sensor->total++;
    }

    fclose(arquivo_entrada);

    if (num_sensores == 0) {
        fprintf(stderr, "\nNão foi achado nenhum dado válido.\n");
        return EXIT_FAILURE;
    }

    salvar_sensores_nos_arquivos();
    printf("\n\n O processamento foi finalizado e encontrei o total de %d sensores.\n\n", num_sensores);
    return EXIT_SUCCESS;
}
