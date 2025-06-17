#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>


typedef enum { t_int, t_float, t_bool, t_string, t_invalido} TipoDeSensores;


int verificar_se_inteiro(const char *str) {
    for (int i = 0; str[i]; i++)
        if (!isdigit(str[i])) return 0;
    return 1;
}


TipoDeSensores conversao_para_valor_equivalentes(const char *str) {
    if (strcmp(str, "CONJ_Z") == 0) return t_int;
    if (strcmp(str, "CONJ_Q") == 0) return t_float;
    if (strcmp(str, "BINARIO") == 0) return t_bool;
    if (strcmp(str, "TEXTO") == 0) return t_string;

    return t_invalido;
}


time_t criar_timestamp(int dia, int mes, int ano, int hora, int min, int seg) {
    struct tm data = {0};
    data.tm_year = ano - 1900;
    data.tm_mon = mes - 1;
    data.tm_mday = dia;
    data.tm_hour = hora;
    data.tm_min = min;
    data.tm_sec = seg;
    data.tm_isdst = -1;
    return mktime(&data);
}


time_t timestamp_aleatorio(time_t inicio, time_t fim) {
    return inicio + rand() % (fim - inicio + 1);
}


void criar_string_random(char *buffer, size_t max_len){
    int tamanho = 3 + rand() % (max_len - 2);

    for (int i = 0; i < tamanho; i++) {
        buffer[i] = (rand() % 2) ? ('a' + rand() % 26) : ('A' + rand() % 26);
    }

    buffer[tamanho] = '\0';
}


int ler_timestamps(int argc, char *argv[], time_t *t_inicio, time_t *t_fim) {
    if (argc < 13) {
        fprintf(stderr, "\n\nOs argumentos apresentados são insuficientes para data/hora.\n");
        return 0;
    }

    for (int i = 1; i <= 12; i++) {
        if (!verificar_se_inteiro(argv[i])) {
            fprintf(stderr, "Argumento inválido para data/hora: '%s'\n", argv[i]);
            return 0;
        }
    }

    int dia_inicio = atoi(argv[1]), mes_inicio = atoi(argv[2]), ano_inicio = atoi(argv[3]);
    int hr_inicio = atoi(argv[4]), min_inicio = atoi(argv[5]), seg_inicio = atoi(argv[6]);
    int dia_fim = atoi(argv[7]), mes_fim = atoi(argv[8]), ano_fim = atoi(argv[9]);
    int hr_fim = atoi(argv[10]), min_fim = atoi(argv[11]), seg_fim = atoi(argv[12]);

    if (dia_inicio < 1 || dia_inicio > 31 || mes_inicio < 1 || mes_inicio > 12 ||
        hr_inicio < 0 || hr_inicio > 23 || min_inicio < 0 || min_inicio > 59 || seg_inicio < 0 || seg_inicio > 59 ||
        dia_fim < 1 || dia_fim > 31 || mes_fim < 1 || mes_fim > 12 ||
        hr_fim < 0 || hr_fim > 23 || min_fim < 0 || min_fim > 59 || seg_fim < 0 || seg_fim > 59) {
        fprintf(stderr, "\nDatas ou horários fora do intervalo válido.\n");
        return 0;
    }

    *t_inicio = criar_timestamp(dia_inicio, mes_inicio, ano_inicio, hr_inicio, min_inicio, seg_inicio);
    *t_fim = criar_timestamp(dia_fim, mes_fim, ano_fim, hr_fim, min_fim, seg_fim);

    if (*t_inicio == (time_t)-1 || *t_fim == (time_t)-1 || *t_fim < *t_inicio) {
        fprintf(stderr, "\nDatas apresentadas são inválidas ou em ordem incorreta.\n");
        return 0;
    }

    return 1;
}


int ler_sensores(int argc, char *argv[], int *num_sensores, char sensores[][17], TipoDeSensores tipos[]) {
    int args_restantes = argc - 13;

    if (args_restantes <= 0) {
        fprintf(stderr, "Informe ao menos um sensor válido: CONJ_Z, CONJ_Q, BINARIO, TEXTO\n");
        return 0;
    }

    if (args_restantes > 4) {
        fprintf(stderr, "Número máximo de sensores permitidos: 4\n");
        return 0;
    }

    const char *sensores_validos[] = {"CONJ_Z", "CONJ_Q", "BINARIO", "TEXTO"};
    TipoDeSensores tipos_validos[] = {t_int, t_float, t_bool, t_string};
    int usados[4] = {0, 0, 0, 0};  

    *num_sensores = 0;

    for (int i = 0; i < args_restantes; i++) {
        const char *sensor = argv[13 + i];
        int valido = 0;
        for (int j = 0; j < 4; j++) {
            if (strcmp(sensor, sensores_validos[j]) == 0) {
                if (usados[j]) {
                    fprintf(stderr, "Invalido, um sensor está repetido: %s\n", sensor);
                    return -1;
                }
                usados[j] = 1;
                strncpy(sensores[*num_sensores], sensor, 16);
                sensores[*num_sensores][16] = '\0';
                tipos[*num_sensores] = tipos_validos[j];
                (*num_sensores)++;
                valido = 1;
                break;
            }
        }
        if (!valido) {
            fprintf(stderr, "Sensor inválido: %s. Sensores válidos: CONJ_Z, CONJ_Q, BINARIO, TEXTO\n", sensor);
            return -1;
        }
    }

    return 1;
}


void gerar_dados(FILE *arquivo, const char *sensor, TipoDeSensores tipo, time_t t_inicio, time_t t_fim) {
    for (int j = 0; j < 2000; j++) {
        time_t ts = timestamp_aleatorio(t_inicio, t_fim);

        switch (tipo) {
            case t_int: {
                int val = rand() % 1000;
                fprintf(arquivo, "%ld %s %d\n", (long)ts, sensor, val);
                break;
            }
            case t_float: {
                float val = (rand() % 10000) / 100.0f;
                fprintf(arquivo, "%ld %s %.2f\n", (long)ts, sensor, val);
                break;
            }
            case t_bool: {
                const char *val = (rand() % 2) ? "true" : "false";
                fprintf(arquivo, "%ld %s %s\n", (long)ts, sensor, val);
                break;
            }
            case t_string: {
                char val[17];
                criar_string_random(val, 16);
                fprintf(arquivo, "%ld %s %s\n", (long)ts, sensor, val);
                break;
            }
            default:
                break;
        }
    }
}


int main(int argc, char *argv[]) {
    srand((unsigned int)time(NULL));
    time_t t_inicio, t_fim;

    if (!ler_timestamps(argc, argv, &t_inicio, &t_fim)) {
        printf("\n\n\nPor favor use esse formato: %s dd mm aaaa hh mm ss dd mm aaaa hh mm ss sensor1 [sensor2...]\n\n\n", argv[0]);
        return 1;
    }

    int num_sensores;
    char sensores[10][17];
    TipoDeSensores tipos[10];

    int res = ler_sensores(argc, argv, &num_sensores, sensores, tipos);
    if (res == 0) {
        fprintf(stderr, "Por favor informe pelo menos um sensor e tipo.\n");
        return 1;
    } else if (res == -1) {
        // Mensagem já foi mostrada na ler_sensores
        return 1;
    }

    FILE *arquivo = fopen("leituras.txt", "w");
    if (!arquivo) {
        fprintf(stderr, "Erro ao criar arquivo de saída.\n");
        return 1;
    }

    for (int i = 0; i < num_sensores; i++) {
        gerar_dados(arquivo, sensores[i], tipos[i], t_inicio, t_fim);
    }

    fclose(arquivo);
    printf("Arquivo com o nome 'leituras.txt' gerado com sucesso.\n");
    return 0;
} 
