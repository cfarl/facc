
//---------------------------------------------------------------------------
#ifndef TokenH
#define TokenH
//---------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Tokens reconhecidos pela analise lexica
//-----------------------------------------------------------------------------
#define   TOK_PONTO_VIRGULA      1                                      /* ; */
#define   TOK_NTERM              2                                  /* nterm */
#define   TOK_SETA               3                                     /* -> */
#define   TOK_TEXTO_PAR          4                            /* (texto_par) */
#define   TOK_LOCAL              5                                  /* LOCAL */
#define   TOK_TEXTO_COD          6                            /* {texto_cod} */
#define   TOK_OU                 7                                      /* | */
#define   TOK_VAZIO              8                                      /* # */
#define   TOK_TERM               9                                   /* term */
#define   TOK_ATRIB             10                                 /* .atrib */
#define   TOK_ETX               11                                      /* $ */

//-----------------------------------------------------------------------------
// Tipo de tokens
//-----------------------------------------------------------------------------
#define TERMINAL    1
#define NAO_TERMINAL   2
#define ATRIBUTO   3


//---------------------------------------------------------------------------
#endif
