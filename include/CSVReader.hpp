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
    int numero_da_linha_atual;
    bool arquivo_terminado;

public:
    CSVReader(std::string file_path);
    std::vector<std::string> getLineCSV();
    std::vector<std::string> getLinhaAtual();
    int getNumeroDeColunas();
    int getNumeroDaLinhaAtual();
    bool getArquivoTerminado();
    void setNumeroDeColunas(int num_de_colunas);

};



#endif