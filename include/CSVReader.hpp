#ifndef CSV_READER
#define CSV_READER


#include <iostream>
#include <fstream>
#include <vector>

class CSVReader{
private:
    std::fstream my_csv;
    std::string file_path;
    std::vector<std::string> linha_atual;
    int numero_de_colunas;
    int id_da_linha_atual;
    unsigned long long numero_de_linhas_lido;
    bool arquivo_terminado;

public:
    CSVReader(std::string file_path);
    std::vector<std::string> getLineCSV(); // quando o csv termina isto retorna {};
    std::vector<std::string> getLinhaAtual();
    int getNumeroDeColunas();
    int getIdDaLinhaAtual();
    unsigned long long getNumeroDeLinhasLido();
    bool getArquivoTerminado();
    void setNumeroDeColunas(int num_de_colunas);

};



#endif