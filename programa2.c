#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

typedef struct 
{
    time_t timestamp;
    char id_sensor[20];
    char valor[20];
} Leituras;

time_t criar_timestamp(int dia, int mes, int ano, int hora, int min, int seg) {
    struct tm t = {0};
    t.tm_year = ano - 1900;
    t.tm_mon = mes - 1;
    t.tm_mday = dia;
    t.tm_hour = hora;
    t.tm_min = min;
    t.tm_sec = seg;
    t.tm_isdst = -1;

    time_t resultado = mktime(&t);

    if (resultado == -1) return -1;

    if (t.tm_mday != dia || t.tm_mon != mes - 1 || t.tm_year != ano - 1900 ||
        t.tm_hour != hora || t.tm_min != min || t.tm_sec != seg) {
        return -1;
    }

    return resultado;
}


int busca_binaria(Leituras *leituras, int tamanho, time_t procurando) {
    int inicio = 0, fim = tamanho - 1;
    int indice_mais_proximo = -1;
    time_t menor_diferenca = LONG_MAX;

    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        time_t atual = leituras[meio].timestamp;
        time_t diferenca = labs(atual - procurando);

        if (diferenca < menor_diferenca) {
            menor_diferenca = diferenca;
            indice_mais_proximo = meio;
        }

        if (atual < procurando)
            inicio = meio + 1;
        else
            fim = meio - 1;
    }

    return indice_mais_proximo;
}


int validar_argumentos(int argc, char *argv[], char *nome_sensor, int *dia, int *mes, int *ano, int *hora, int *min, int *seg) {
    if (argc != 9) {
        printf("Utilize a seguinte ordem: %s sensor dd mm aaaa hh mm ss\n", argv[0]);
        return 0;
    }

    strcpy(nome_sensor, argv[1]);
    *dia = atoi(argv[2]);
    *mes = atoi(argv[3]);
    *ano = atoi(argv[4]);
    *hora = atoi(argv[5]);
    *min = atoi(argv[6]);
    *seg = atoi(argv[7]);

    return 1;
}


int carregar_leituras(const char *nomeSensor, Leituras *leituras, int max_leituras) {
    char nome_arquivo[30];

    sprintf(nome_arquivo, "%s.txt", nomeSensor);

    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo '%s'.\n", nome_arquivo);
        return -1;
    }

    int total = 0;
    long long temp_ts;
    while (fscanf(arquivo, "%lld %19s %19s", &temp_ts,
                  leituras[total].id_sensor,
                  leituras[total].valor) == 3) {
        leituras[total].timestamp = (time_t)temp_ts;
        total++;
        if (total >= max_leituras) break;
    }

    fclose(arquivo);
    return total;
}


void exibir_leitura_proxima(Leituras *leituras, int indice) {
    struct tm *tm_info = gmtime(&leituras[indice].timestamp);

    char buffer[30];

    strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", tm_info);

    printf("\n\n\nLeitura mais próxima encontrada foi:\n");
    printf("Timestamp: %s\n", buffer);
    printf("Sensor: %s\n", leituras[indice].id_sensor);
    printf("Valor: %s\n\n\n", leituras[indice].valor);
}


int main(int argc, char *argv[]) {
    char nomeSensor[20];
    int dia, mes, ano, hora, min, seg;

    if (!validar_argumentos(argc, argv, nomeSensor, &dia, &mes, &ano, &hora, &min, &seg))
        return 1;

    time_t timestamp_procurado = criar_timestamp(dia, mes, ano, hora, min, seg);
    if (timestamp_procurado == -1) {
        printf("Data/hora inválida: %02d/%02d/%04d %02d:%02d:%02d\n", dia, mes, ano, hora, min, seg);
        return 1;
    }

    Leituras leituras[5000];
    int total = carregar_leituras(nomeSensor, leituras, 5000);

    if (total < 1) {
        printf("Arquivo está vazio ou mal formatado.\n");
        return 1;
    }

    int indice = busca_binaria(leituras, total, timestamp_procurado);

    if (indice != -1) {
        exibir_leitura_proxima(leituras, indice);
    } else {
        printf("Nenhuma leitura encontrada.\n");
    }

    return 0;
}





