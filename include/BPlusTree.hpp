#ifndef BPLUSTREE_HPP
#define BPLUSTREE_HPP

#include <map>
#include <vector>
#include <string>
#include "BlockManager.hpp"

struct BPlusTreeNode {
    bool isLeaf;                                  // indica se o nó é uma folha
    std::vector<int> keys;                        // chaves no nó
    std::vector<Endereco> children;               // endereços dos filhos (para nós internos)
    std::vector<Endereco> recordBlocks;           // endereços de blocos (para nós folhas)
    Endereco nextNode;                            // endereço do próximo nó folha (apenas para nós folha)

    BPlusTreeNode(bool leaf = true) : isLeaf(leaf), nextNode(0) {}
};

class BPlusTree {
    BlockManager& blockManager;                  
    Endereco rootAddress;                         // endereço da raiz da árvore
    int order;                                    // ordem da árvore B+

public:
    // construtor da árvore B+, inicializando com um BlockManager e ordem
    BPlusTree(BlockManager& bm, int ord = 4) : blockManager(bm), order(ord) {
        // inicializa a raiz como um novo bloco de índice
        BPlusTreeNode rootNode(true);             // cria nó raiz como folha inicialmente
        rootAddress = blockManager.getDiskManager().memoryAlloc(blockManager.getDiskManager().getTamanhoDoBloco());
        blockManager.EscreverBloco(&rootNode, rootAddress);
    }

    // insere uma chave e o bloco associado na árvore
    void insert(int key, const Registro &record) {
        // carrega o nó raiz da árvore
        BPlusTreeNode rootNode;
        blockManager.LerBloco(rootAddress, &rootNode);

        // se a raiz estiver cheia, cria uma nova raiz e realiza split
        if (rootNode.keys.size() >= order - 1) {
            Endereco newRootAddress = blockManager.getDiskManager().memoryAlloc(blockManager.getDiskManager().getTamanhoDoBloco());
            BPlusTreeNode newRoot(false);
            newRoot.children.push_back(rootAddress);

            splitChild(newRoot, rootNode, 0);

            rootAddress = newRootAddress;
            blockManager.EscreverBloco(&newRoot, rootAddress);
        }

        insertNonFull(rootNode, key, record);
    }

    // inserir uma chave em um nó não cheio
    void insertNonFull(BPlusTreeNode& node, int key, const Registro &record) {
        if (node.isLeaf) {
            Endereco blockId = blockManager.getDiskManager().memoryAlloc(blockManager.getDiskManager().getTamanhoDoBloco());
            blockManager.EscreverBloco(&record, blockId);
            
            node.recordBlocks.push_back(blockId);
            node.keys.push_back(key);
            std::sort(node.keys.begin(), node.keys.end());
        } else {
            int i = node.keys.size() - 1;
            while (i >= 0 && key < node.keys[i]) i--;
            i++;
            
            BPlusTreeNode child;
            blockManager.LerBloco(node.children[i], &child);

            if (child.keys.size() >= order - 1) {
                splitChild(node, child, i);
                if (key > node.keys[i]) i++;
            }
            insertNonFull(child, key, record);
            blockManager.EscreverBloco(&child, node.children[i]);
        }
    }

    // divide um nó filho na posição index do nó pai
    void splitChild(BPlusTreeNode& parent, BPlusTreeNode& child, int index) {
        BPlusTreeNode newChild(child.isLeaf);
        int median = order / 2;

        for (int i = median + 1; i < child.keys.size(); i++) {
            newChild.keys.push_back(child.keys[i]);
            if (child.isLeaf) newChild.recordBlocks.push_back(child.recordBlocks[i]);
        }
        
        if (!child.isLeaf) {
            for (int i = median + 1; i < child.children.size(); i++) {
                newChild.children.push_back(child.children[i]);
            }
        }

        Endereco newChildAddress = blockManager.getDiskManager().memoryAlloc(blockManager.getDiskManager().getTamanhoDoBloco());
        blockManager.EscreverBloco(&newChild, newChildAddress);

        parent.keys.insert(parent.keys.begin() + index, child.keys[median]);
        parent.children.insert(parent.children.begin() + index + 1, newChildAddress);

        child.keys.resize(median);
        if (child.isLeaf) child.nextNode = newChildAddress;
    }

    // busca uma chave e retorna o bloco correspondente
    Endereco find(int key, Registro &record) {
        BPlusTreeNode node;
        blockManager.LerBloco(rootAddress, &node);
        int blocksRead = 0;

        while (!node.isLeaf) {
            int i = 0;
            while (i < node.keys.size() && key > node.keys[i]) i++;
            blockManager.LerBloco(node.children[i], &node);
            blocksRead++;
        }

        for (int i = 0; i < node.keys.size(); i++) {
            if (node.keys[i] == key) {
                blockManager.LerBloco(node.recordBlocks[i], &record);
                return blocksRead + 1;
            }
        }
        return -1; // chave não encontrada
    }
};

#endif
