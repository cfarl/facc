

//---------------------------------------------------------------------------
#ifndef GeradorArquivo_H
#define GeradorArquivo_H
//---------------------------------------------------------------------------

#include <stdio.h>

class AnalisadorSintatico ;
class AnalisadorSemantico ;

//---------------------------------------------------------------------------
// Fachada para a geracao de arquivos
//---------------------------------------------------------------------------
class GeradorArquivoFacade {
public:
	static char nome_fonte[80];	 // Arquivo contendo o codigo fonte da gramatica fonte
	static char nome_lex_c[80];  // Arquivo contendo o Analisador Lexico
	static char nome_lex_h[80];  // Arquivo contendo o Analisador Lexico
	static char nome_sin_c[80];  // Arquivo contendo o Analisador Sintatico
	static char nome_sin_h[80];  // Arquivo contendo o Analisador Sintatico
	static char nome_tab_h[80];  // Arquivo contendo a tabela do Analisador Sintatico
	static char nome_sem_h[80];  // Arquivo contendo o Analisador Semantico
	static char nome_sem_c[80];  // Arquivo contendo o Analisador Semantico
	static char nome_cmp_c[80];  // Arquivo contendo o Compilador
	static char nome_tpa[80];  // Nome da tabela do parser
	static char nome_prd[80];  // Nome do arq. de Prod.
	static char nome_est[80];  // Nome da arq. de estados

public:
	// Gera arquivos
	static void geraArquivoAnalisadorLexico();
	static void geraArquivoAnalisadorSintatico();
	static void geraArquivoCompilador() ;
	static void gerarArquivoSemantico(AnalisadorSintatico *sintatico, AnalisadorSemantico *semantico);

	// Impressao de tabelas
	static void imprime_header();
	static void imprime_tabelas_do_parser();
	static void imprime_producoes();
	static void imprime_first_itens();

private:
	// Metodos auxiliares na impressao de tabelas
	static void imprime_first();
	static void imprime_itens();
	static void imprime_tab_acao(FILE *saida);
	static int imprime_tab_goto(FILE *saida);
	static void imprime_nterm_esq(FILE *saida);
	static void imprime_nro_dir(FILE *saida);
	static void imprime_term_atrib(FILE *saida);
	static void imprime_nt_filhos(FILE *saida);
	static void imprime_producoes_associadas(FILE *saida);
};


//---------------------------------------------------------------------------
#endif

