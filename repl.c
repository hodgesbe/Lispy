#include <stdio.h>
#include <stdlib.h>
#include "mpc/mpc.h"

/* Inludes both readline and history */
#ifdef _WIN32
#include<string.h>

static char buffer[2048];

/* Custom readline function to use
 * if running on Windows
 */
char* readline(char* prompt)
{
    fputs(prompt, stdout);
    fgets(buffer, 2048, stdin);
    char* cpy = malloc(strlen(buffer)+1);
    strcpy(cpy, buffer);
    cpy[strlen(cpy)-1] = '\0';
    return cpy;
}

/* Stub add_history function */
void add_history(char* unused) {}

/* If not on Windows */
#else

#include <editline/readline.h>
#endif

int main(int argc, char** argv)
{
    /* mpc Grammer Definition */

    /* Create Parsers */
    mpc_parser_t* Number   = mpc_new("number");
    mpc_parser_t* Operator = mpc_new("operator");
    mpc_parser_t* Expr     = mpc_new("expr");
    mpc_parser_t* Lispy    = mpc_new("lispy");

    /* Define with following language */
    mpca_lang(MPCA_LANG_DEFAULT,
            "                                                      \
                number   : /-?[0-9]+/ ;                            \
                operator : '+' | '-' | '*' | '/';                  \
                expr     : <number> | '(' <operator> <expr>+ ')' ; \
                lispy    : /^/ <operator> <expr>+ /$/ ;            \
            ",
            Number, Operator, Expr, Lispy);


    /* Print Version and Exit Information */
    puts("Lispy Version 0.0.0.0.2");
    puts("Press Ctrl+c to Exit\n");

    /* In a never ending loop */
    while (1) {

        /* Output our prompt and get input */
        char * input = readline("lispy> ");

        /* Add input to history */
        add_history(input);

        /* Try to parse input */
        mpc_result_t r;

        if (mpc_parse("<stdin>", input, Lispy, &r))
        {
            /* On Success Print the AST */
            mpc_ast_print(r.output);
            mpc_ast_delete(r.output);
        } else /* Print Error */
        {
            mpc_err_print(r.error);
            mpc_err_delete(r.error);
        }

        /* Free retrieved input */
        free(input);
    }

    /* Underfine and Delete our Parsers */
    mpc_cleanup(4, Number, Operator, Expr, Lispy);

    return 0;
}
