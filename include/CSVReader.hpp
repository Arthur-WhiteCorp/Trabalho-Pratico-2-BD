#ifndef CSV_READER
#define CSV_READER


#include <iostream>
#include <fstream>


class CSVReader{
private:
    std::fstream my_csv;
    std::string file_path;
    unsigned long long numero_de_colunas;
    unsigned long long numero_de_linhas;
    unsigned long long linha_Atual;
    void openCSV();

public:
    CSVReader(std::string);
    void getLine();
};



#endif