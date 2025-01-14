#include "btree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Définition des types pour les commandes
typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum { 
    PREPARE_SUCCESS, 
    PREPARE_UNRECOGNIZED_STATEMENT 
} PrepareResult;

typedef enum { 
    STATEMENT_INSERT, 
    STATEMENT_SELECT 
} StatementType;

typedef struct {
    StatementType type;
    int id;
    char name[32];
} Statement;

typedef struct {
    char* buffer;
    size_t buffer_length;
    ssize_t input_length;
} InputBuffer;

// Fonction pour créer un buffer d'entrée
InputBuffer* new_input_buffer() {
    InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
    input_buffer->buffer = NULL;
    input_buffer->buffer_length = 0;
    input_buffer->input_length = 0;

    return input_buffer;
}

// Fonction pour afficher le prompt
void print_prompt() { 
    printf("db > "); 
}

// Fonction pour lire l'entrée utilisateur
void read_input(InputBuffer* input_buffer) {
    ssize_t bytes_read =
        getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

    if (bytes_read <= 0) {
        printf("Error reading input\n");
        exit(EXIT_FAILURE);
    }

    // Ignorer le caractère de nouvelle ligne
    input_buffer->input_length = bytes_read - 1;
    input_buffer->buffer[bytes_read - 1] = '\0';
}

// Fonction pour libérer le buffer d'entrée
void close_input_buffer(InputBuffer* input_buffer) {
    free(input_buffer->buffer);
    free(input_buffer);
}

// Gestion des commandes spéciales comme `.exit`
MetaCommandResult do_meta_command(InputBuffer* input_buffer) {
    if (strcmp(input_buffer->buffer, ".exit") == 0) {
        return META_COMMAND_SUCCESS;
    } else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

// Préparer une commande à partir de l'entrée utilisateur
PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement) {
    if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
        statement->type = STATEMENT_INSERT;

        // Extraire les paramètres de la commande
        int id;
        char name[32];
        int args_assigned = sscanf(input_buffer->buffer, "insert %d %s", &id, name);
        if (args_assigned < 2) {
            return PREPARE_UNRECOGNIZED_STATEMENT;
        }

        statement->id = id;
        strncpy(statement->name, name, sizeof(statement->name) - 1);
        statement->name[sizeof(statement->name) - 1] = '\0';

        return PREPARE_SUCCESS;
    }

    if (strcmp(input_buffer->buffer, "select") == 0) {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }

    return PREPARE_UNRECOGNIZED_STATEMENT;
}

// Exécuter une commande
void execute_statement(Statement* statement, BTreeNode** root) {
    switch (statement->type) {
        case STATEMENT_INSERT:
            *root = insert_node(*root, statement->id, statement->name);
            printf("Inserted: ID = %d, Name = %s\n", statement->id, statement->name);
            break;
        case STATEMENT_SELECT:
            printf("Table contents:\n");
            inorder_traversal(*root);
            break;
    }
}

// Boucle principale REPL
void repl(void) {
    BTreeNode* root = NULL; // Racine de l'arbre binaire
    InputBuffer* input_buffer = new_input_buffer();

    while (true) {
        print_prompt();
        read_input(input_buffer);

        if (input_buffer->buffer[0] == '.') {
            switch (do_meta_command(input_buffer)) {
                case META_COMMAND_SUCCESS:
                    close_input_buffer(input_buffer);
                    free_tree(root);
                    exit(EXIT_SUCCESS);
                case META_COMMAND_UNRECOGNIZED_COMMAND:
                    printf("Unrecognized command '%s'\n", input_buffer->buffer);
                    continue;
            }
        }

        Statement statement;
        switch (prepare_statement(input_buffer, &statement)) {
            case PREPARE_SUCCESS:
                execute_statement(&statement, &root);
                break;
            case PREPARE_UNRECOGNIZED_STATEMENT:
                printf("Unrecognized keyword at start of '%s'.\n", input_buffer->buffer);
                continue;
        }
    }
}
