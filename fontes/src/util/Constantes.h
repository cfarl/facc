
//---------------------------------------------------------------------------
#ifndef Constantes_H
#define Constantes_H
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Falhas de I/O
//---------------------------------------------------------------------------
#define ABERTURA_ARQ            1            /* Arquivo fonte nao encontrado */
#define CRIACAO_ARQ             2                /* Impossivel criar arquivo */
#define FECHAMENTO_ARQ          3               /* Impossivel fechar arquivo */
#define GRAMATICA_GRANDE        4                 /* Gramatica grande demais */
#define GRAVACAO_ARQ            5            /* Impossivel gravar em arquivo */
#define LEITURA_ARQ  	        6                  /* Impossivel ler arquivo */

//---------------------------------------------------------------------------
// Outros erros
//---------------------------------------------------------------------------
#define NAO_ESPERADO            11                   /* Simbolo nao esperado */
#define ESPERADO                12                       /* Simbolo esperado */
#define SIMBOLO_ESP             13          /* Term, Nterm ou Vazio esperado */
#define ERRO_INVALIDO          99           /* Codigo de erro nao cadastrado */

#define TABELA_ACAO             9    /* Erro na construcao da Tabela de Acao */
#define TABELA_GOTO            10    /* Erro na construcao da Tabela de Goto */
#define DESEMPILHA             11                    /* Erro no uso da pilha */
#define LISTA_ACAO             12 /* Erro na lista de acao do conj. de itens */
#define TABELA_HASH            13   /* Erro na procura de um simbolo na hash */
#define LISTA_NTERM            14   /* Erro na proc. de um simbolo lista nt  */
#define PRODUCAO_NTERM         15   /* Erro de acesso a uma producao de nt   */
#define CODIGO_INEXISTENTE     16   /* Erro de acesso a  codigo da lista     */

#define ALOCACAO_MEMORIA        7      /* Memoria insuficiente para alocacao */

//---------------------------------------------------------------------------
// Erros Lexicos
//---------------------------------------------------------------------------
#define COMENTARIO		1		       /* Erro em comentario */
#define SIMBOLO_DESCONHECIDO    2  /* Simbolo sem significado p/ a linguagem */
#define TERMINAL_VAZIO          3
#define ATRIBUTO_VAZIO          4
#define TERMINAL_NAO_FECHADO    5    /* Fim de arq. antes de fechar terminal */
#define TEXTO_NAO_FECHADO       6       /* Fim de arq. antes de fechar texto */
#define INICIO_ATRIBUTO         7      /* Atributo inicia diferente de letra */

//---------------------------------------------------------------------------
// Erros de Gramatica
//---------------------------------------------------------------------------
#define ATINGE_TERMINAL        21        /* Nao-terminal nao atinge terminal */
#define ATINGIDO_POR_INICIAL   22       /* Nterm nao e' atingido por inicial */
#define PRODUCAO_REPETIDA      23                      /* Producao duplicada */

//---------------------------------------------------------------------------
// Advertencias
//---------------------------------------------------------------------------
#define SIMBOLO_TRUNCADO 1       /* Simbolo estourou maximo de caracteres */
#define TERM_SUSPEITO    2       /* Terminal com branco na(s) extremidade(s) */

//---------------------------------------------------------------------------
// Outras
//---------------------------------------------------------------------------
#define VAZIO   0                           /* usado no look-ahed e tab_simb */
#define P_ESTADOS               5                   /* Impressao dos estados */

//---------------------------------------------------------------------------
#endif
