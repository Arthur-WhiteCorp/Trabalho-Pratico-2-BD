#include <iostream>
#include "BPlusTree.hpp"

// buscar pela B+Tree no índice primário
void seek1(BPlusTree& tree, int id) {
    std::string resultado = tree.search(id);
    if (resultado != "Artigo não encontrado") {
        std::cout << "Artigo encontrado! Título: " << resultado << std::endl;
        std::cout << "Blocos lidos:" << std::endl;
    } else {
        std::cout << resultado << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Uso: seek1 <ID>" << std::endl;
        return 1;
    }

    int id = std::stoi(argv[1]);
    BPlusTree tree; // árvore deve ser carregada pelo upload

    // chamada para buscar pelo índice primário (ID)
    seek1(tree, id);

    return 0;
}


