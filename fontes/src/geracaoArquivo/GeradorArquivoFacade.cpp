

#include "GeradorArquivoFacade.h"


char GeradorArquivoFacade::nome_fonte[80];	// Arquivo contendo o codigo fonte da gramatica fonte
char GeradorArquivoFacade::nome_lex_c[80] = "AnalisadorLexico.cpp"; // Arquivo contendo o Analisador Lexico
char GeradorArquivoFacade::nome_lex_h[80] = "AnalisadorLexico.h";   // Arquivo contendo o Analisador Lexico
char GeradorArquivoFacade::nome_sin_c[80] = "AnalisadorSintatico.cpp";  // Arquivo contendo o Analisador Sintatico
char GeradorArquivoFacade::nome_sin_h[80] = "AnalisadorSintatico.h";    // Arquivo contendo o Analisador Sintatico
char GeradorArquivoFacade::nome_tab_h[80] = "TabelaAnalisadorSintatico.h"; // Arquivo contendo a tabela do Analisador Sintatico
char GeradorArquivoFacade::nome_sem_h[80] = "AnalisadorSemantico.h";   // Arquivo contendo o Analisador Semantico
char GeradorArquivoFacade::nome_sem_c[80] = "AnalisadorSemantico.cpp"; // Arquivo contendo o Analisador Semantico
char GeradorArquivoFacade::nome_cmp_c[80] = "Compilador.cpp";  		 // Arquivo contendo o Compilador

char GeradorArquivoFacade::nome_tpa[80];  // Nome da tabela do parser
char GeradorArquivoFacade::nome_prd[80];  // Nome do arq. de Prod.
char GeradorArquivoFacade::nome_est[80];  // Nome da arq. de estados

