#include "CSVReader.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <regex>

CSVReader::CSVReader(std::string file_path){
    this->file_path = file_path;
    my_csv.open(file_path);
    if (!my_csv.is_open()) {
        std::cerr << "Error opening file: " << file_path << std::endl;
        exit(1); // Handle the error
    }
    id_da_linha_atual = 0;
    numero_de_colunas = 0;
    numero_de_linhas_lido = 0;
    arquivo_terminado = false;
    std::filesystem::path p{file_path};

    tamanho_do_csc = std::filesystem::file_size(file_path);



}

void CSVReader::setNumeroDeColunas(unsigned int num_colunas){
    this->numero_de_colunas = num_colunas;
    linha_atual.reserve(numero_de_colunas);
}

std::vector<std::string> CSVReader::getLineCSV(){
    std::string linha, campo, nova_linha_id;
    std::regex quoted_number_regex("\"[0-9]+\"");

    linha_atual.clear();

    if (getline(my_csv,campo,';')){

        linha_atual.emplace_back(campo);

        while ((linha_atual.size()) <= (numero_de_colunas - 1)){ // lê enquanto o numero de colunas for menor ou
            if (linha_atual.size() < numero_de_colunas -1 ){
                getline(my_csv, campo, ';');                
                linha_atual.emplace_back(campo);
            }else{
                getline(my_csv,campo);
                linha_atual.emplace_back(campo);
            }

        }
        std::string numero_sem_aspas = std::regex_replace(linha_atual[0], std::regex("\""), "");
        id_da_linha_atual = stoi(numero_sem_aspas);                       
        numero_de_linhas_lido++;
    }else{
        arquivo_terminado = true;
    }
    return linha_atual;
}
std::vector<std::string> CSVReader::getLinhaAtual(){
    if (linha_atual.empty()){
        std::cout << "Use getLineCSV() primeiro para pegar uma linha" << std::endl;
        return linha_atual;
    }else{
        return linha_atual;
    }
}

unsigned int CSVReader::getNumeroDeColunas(){
    return numero_de_colunas;
}

int CSVReader::getIdDaLinhaAtual(){
    return id_da_linha_atual;
}

unsigned long long CSVReader::getNumeroDeLinhasLido(){
    return numero_de_linhas_lido;
} 

bool CSVReader::getArquivoTerminado(){
    return arquivo_terminado;
}

uintmax_t CSVReader::getTamanhoDoCSV(){
    return tamanho_do_csc;
}


void CSVReader::resetarLocalizacaoDoarquivo(){
    if (my_csv.is_open()) {
        my_csv.clear(); // clear error flags
        my_csv.seekg(0, std::ios::beg); // move to beginning of file
        if (!my_csv) {
            std::cerr << "Error: unable to reset file position" << std::endl;
        }
    } else {
        std::cerr << "Error: file is not open" << std::endl;
    }
    id_da_linha_atual = 0;
    numero_de_linhas_lido = 0;
    arquivo_terminado = false;
    linha_atual.clear();
    linha_atual.shrink_to_fit();
}

void CSVReader::fecharArquivo(){
    my_csv.close();
}

