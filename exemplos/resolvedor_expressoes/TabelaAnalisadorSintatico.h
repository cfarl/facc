
//---------------------------------------------------------------------------
#ifndef TabelaAnalisadorSintaticoH
#define TabelaAnalisadorSintaticoH
//---------------------------------------------------------------------------

 /* Tokens a serem enviados para o parser gerado pelo FACC */ 

#define   TOK_TERM001            1  /* + */
#define   TOK_TERM002            2  /* - */
#define   TOK_TERM003            3  /* * */
#define   TOK_TERM004            4  /* / */
#define   TOK_TERM005            5  /* num */
#define   TOK_TERM006            6  /* ( */
#define   TOK_TERM007            7  /* ) */
#define   TOK_ETX                8  /* $ */

#define TRUE  1   /* Valores Booleanos */

#define FALSE 0

//------------------------------------------------------------------------------
//Tabela do Analisador Sintático
//------------------------------------------------------------------------------
class TabelaAnalisadorSintatico {
public:
// Tabela de Acao
static short getAcao(int lin, int col) { 
 static int acao[][8] = {{               /* Tabela de Acao */
    0,    0,    0,    0, 2004, 2005,    0,    0},{
 2006, 2007,    0,    0,    0,    0,    0, 1000},{
 1001, 1001, 2008, 2009,    0,    0, 1001, 1001},{
 1004, 1004, 1004, 1004,    0,    0, 1004, 1004},{
 1007, 1007, 1007, 1007,    0,    0, 1007, 1007},{
    0,    0,    0,    0, 2004, 2005,    0,    0},{
    0,    0,    0,    0, 2004, 2005,    0,    0},{
    0,    0,    0,    0, 2004, 2005,    0,    0},{
    0,    0,    0,    0, 2004, 2005,    0,    0},{
    0,    0,    0,    0, 2004, 2005,    0,    0},{
 2006, 2007,    0,    0,    0,    0, 2015,    0},{
 1002, 1002, 2008, 2009,    0,    0, 1002, 1002},{
 1003, 1003, 2008, 2009,    0,    0, 1003, 1003},{
 1005, 1005, 1005, 1005,    0,    0, 1005, 1005},{
 1006, 1006, 1006, 1006,    0,    0, 1006, 1006},{
 1008, 1008, 1008, 1008,    0,    0, 1008, 1008} };
 return acao[lin][col];
}

short static getGoto(int lin, int col) {
   static short vet_goto[][3] =  {
/*---------------------------------------------------*/  
/*     TABELA DE GOTO                                */  
/*---------------------------------------------------*/  
/* EST   NAO_TERM.                         NOVO EST. */  
/*---------------------------------------------------*/
{    0,  1  /* expressao_aritmetica           */,   1},
{    0,  2  /* termo                          */,   2},
{    0,  3  /* fator                          */,   3},
{    5,  1  /* expressao_aritmetica           */,  10},
{    5,  2  /* termo                          */,   2},
{    5,  3  /* fator                          */,   3},
{    6,  2  /* termo                          */,  11},
{    6,  3  /* fator                          */,   3},
{    7,  2  /* termo                          */,  12},
{    7,  3  /* fator                          */,   3},
{    8,  3  /* fator                          */,  13},
{    9,  3  /* fator                          */,  14 }};
return vet_goto[lin][col] ;
}

// Número de linhas da tabela de goto
static short getNumeroLinhasTabelaGoto() { 
 return 12;
} 


      /*   numero de simbolos a direita */

static int getNroDireita(int pos) { 
static int nro_direita [8] =      {
  1,  3,  3,  1,  3,  3,  1,  3 };
return nro_direita[pos]; 
} 



       /*    nao terminal a esquerda */
static int getNTermEsquerda(int pos) { 
static int nterm_esquerda[8] =    {
  1,  1,  1,  2,  2,  2,  3,  3 };
return nterm_esquerda[pos]; 
} 



       /*    Terminais que possuem atributos */
static int isTermAtrib(int pos) { 
static int term_atrib[8] =    {
 FALSE, FALSE, FALSE, FALSE,  TRUE, FALSE, FALSE, FALSE };
return term_atrib[pos]; 
} 



       /*    Numero de filhos nao terminais de cada producao */
static int getNtFilhos(int pos) { 
static int nt_filhos[8] =    {
  1,  2,  2,  1,  2,  2,  0,  1 };
return nt_filhos[pos]; 
} 



       /*    Numero de producoes associadas a cada nao terminal */
static int getProducoesAssociadas(int pos) { 
static int producoes_associadas[3] =    {
  3,  3,  2 };
return producoes_associadas[pos]; 
} 

}; 


//---------------------------------------------------------------------------
#endif