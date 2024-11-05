#include <iostream>
#include "BPlusTree.hpp"

// buscar pela B+Tree no índice secundário (Título)
void seek2(BPlusTree& tree, std::string titulo) {
    // assumindo que o ID é a chave e o título é o valor associado na B+Tree
    std::string resultado = tree.search(std::hash<std::string>{}(titulo)); // hash para o título como chave
    if (resultado != "Artigo não encontrado") {
        std::cout << "Artigo encontrado! ID: " << resultado << std::endl;
        std::cout << "Blocos lidos: " << std::endl;
    } else {
        std::cout << resultado << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Uso: seek2 <Título>" << std::endl;
        return 1;
    }

    std::string titulo = argv[1];
    BPlusTree tree; //árvore deve ter sido carregada pelo `upload.cpp`

    // chamada para buscar o artigo pelo índice secundário (Título)
    seek2(tree, titulo);

    return 0;
}

