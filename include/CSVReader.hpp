#ifndef CSV_READER
#define CSV_READER


#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

class CSVReader{
private:
    std::fstream my_csv;
    std::string file_path;
    std::vector<std::string> linha_atual;
    unsigned int numero_de_colunas;
    int id_da_linha_atual;
    unsigned long long numero_de_linhas_lido;
    bool arquivo_terminado;
    uintmax_t tamanho_do_csc;

public:
    CSVReader(std::string file_path);
    std::vector<std::string> getLineCSV(); // quando o csv termina isto retorna {};
    std::vector<std::string> getLinhaAtual();
    unsigned int getNumeroDeColunas();
    int getIdDaLinhaAtual();
    unsigned long long getNumeroDeLinhasLido();
    bool getArquivoTerminado();
    void setNumeroDeColunas(unsigned int num_de_colunas);
    uintmax_t getTamanhoDoCSV();
    void resetarLocalizacaoDoarquivo();// vai ser usado para voltar ao inicio do arquivo
    void fecharArquivo(); 
};


#endif