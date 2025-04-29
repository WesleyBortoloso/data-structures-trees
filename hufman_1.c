#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char character;
    struct Node *left;
    struct Node *right;
} Node;

Node* createNode(char character) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->character = character;
    node->left = node->right = NULL;
    return node;
}

void insert(Node* root, const char* code, char character) {
    Node* current = root;
    for (int i = 0; code[i] != '\0'; i++) {
        if (code[i] == '0') {
            if (!current->left) current->left = createNode('\0');
            current = current->left;
        } else {
            if (!current->right) current->right = createNode('\0');
            current = current->right;
        }
    }
    current->character = character;
}

void decode(Node* root, const char* encoded) {
    Node* current = root;
    for (int i = 0; encoded[i] != '\0'; i++) {
        current = (encoded[i] == '0') ? current->left : current->right;

        if (current->left == NULL && current->right == NULL) {
            printf("%c", current->character);
            current = root;
        }
    }
    printf("\n");
}

int main() {
    struct {
        char character;
        char *code;
    } dictionary[] = {
        {'a', "010"}, {'c', "11110"}, {'e', "1100"}, {'i', "1110"},
        {'l', "1101"}, {'m', "100"}, {'o', "1011"}, {'P', "011011"},
        {'p', "01001"}, {'r', "1111"}, {'s', "0110"}, {' ', "0111"}
    };

    int dict_size = sizeof(dictionary) / sizeof(dictionary[0]);

    Node* root = createNode('\0');
    for (int i = 0; i < dict_size; i++) {
        insert(root, dictionary[i].code, dictionary[i].character);
    }

    const char* encoded =
        "011010100011011000110000010001010101011101100000100001111110000111100101010101100100100001010011001101100100001100110101110011111111010101001110010011000110111100111000011110011101011111111101110010110011010111001100001010010001101100111101011110110110111111111101010100101";

    printf("Frase decodificada: ");
    decode(root, encoded);

    return 0;
}
