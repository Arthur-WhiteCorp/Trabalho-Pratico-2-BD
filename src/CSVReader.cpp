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
    numero_da_linha_atual = 0;
    numero_de_colunas = 0;
    arquivo_terminado = false;

}

void CSVReader::setNumeroDeColunas(int num_colunas){
    this->numero_de_colunas = num_colunas;

}

std::vector<std::string> CSVReader::getLineCSV(){
    std::string linha, campo, nova_linha_id;
    std::regex quoted_number_regex("\"[0-9]+\"");

    if (getline(my_csv,linha)){
        std::stringstream separador_de_linha(linha);
        std::stringstream detector_de_nova_linha(linha);

        getline(detector_de_nova_linha,nova_linha_id,';');
        if (std::regex_match(nova_linha_id,quoted_number_regex)){
            linha_atual.clear(); // limpa a linha atual se achar uma nova linha;
            std::string numero_sem_aspas = std::regex_replace(nova_linha_id, std::regex("\""), "");
            numero_da_linha_atual = stoi(numero_sem_aspas);
        };

        while (getline(separador_de_linha, campo, ';')){
            linha_atual.push_back(campo);
        }
    
    }else{
        arquivo_terminado = true;
        my_csv.close();
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

int CSVReader::getNumeroDeColunas(){
    return numero_de_colunas;
}

int CSVReader::getNumeroDaLinhaAtual(){
    return numero_da_linha_atual;
}
bool CSVReader::getArquivoTerminado(){
    return arquivo_terminado;
}
