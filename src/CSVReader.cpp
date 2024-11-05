#include "CSVReader.hpp"
#include <iostream>
#include <fstream>
#include <thread>
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
    char c, prev_char;
    std::string campo = "", complete_line = "", null_str_aux = "";
    bool is_inside_quotes = false, is_field_empty = false;
    int fields_qty = 0, null_counter_aux = 0;

    while(my_csv.get(c)) {

        // Ignora espaços em branco e break lines
        if (!is_inside_quotes && ((c == ' ') || (c == '\n'))) {
            continue;
        } else {
            // Verifica o caso de termos "campo1";____;"campo2"
            if (!is_inside_quotes && c == ';' && c == prev_char) {
                fields_qty++;
                complete_line += "NULL";
            }

            // Identifica uma linha completa
            if (fields_qty == 7) {
                // USAR REGEX para extrair os campos
                std::vector<std::string> fields;
                std::string currentField;
                bool inQuotes = false;
                char quoteChar = 0;

                for (size_t i = 0; i < complete_line.size(); ++i) {
                    char c = complete_line[i];

                    if (!inQuotes && (c == '"')) {  // Início de um campo entre aspas
                        inQuotes = true;
                        quoteChar = '"';
                    } else if (inQuotes && c == quoteChar) {  // Fim de um campo entre aspas
                        inQuotes = false;
                    } else if (c == ';' && !inQuotes) {  // Fim de um campo fora de aspas
                        fields.push_back(currentField.empty() ? "NULL" : currentField);
                        currentField.clear();
                    } else {
                        currentField += c;
                    }
                }

                if (!currentField.empty()) {
                    fields.push_back(currentField.empty() ? "NULL" : currentField);
                }

                // Remover aspas dos campos, se presente
                for (std::string& field : fields) {
                    if (field.front() == '"' && field.back() == '"') {
                        field = field.substr(1, field.size() - 2);
                    }
                }
        
                std::cout << fields[0] << std::endl;
                id_da_linha_atual = stoi(fields[0]);

                // std::cout << complete_line << std::endl;
                numero_de_linhas_lido++;
                fields_qty = 0;
                complete_line = "";
                null_str_aux = "";
                null_counter_aux = 0;
                linha_atual.clear();
                if (c == '"') {
                    is_inside_quotes = !is_inside_quotes;
                }
                complete_line += c; 
            } else {

                if (fields_qty == 6 && c != ';' && null_counter_aux <= 4) {
                    null_str_aux += c;
                    null_counter_aux++;
                    if (null_counter_aux == 4 && null_str_aux == "NULL") {
                        fields_qty++;
                    }
                }

                if (c == '"') {
                    is_inside_quotes = !is_inside_quotes;
                }
                complete_line += c; 
            }

            // Incrementa a quantidade de campos ao fechar aspas
            if (!is_inside_quotes && c == '"') {
                fields_qty++;
            }

            prev_char = c;
        }
    }

    numero_de_linhas_lido++;

    std::cout << complete_line << std::endl;

    arquivo_terminado = true;
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
