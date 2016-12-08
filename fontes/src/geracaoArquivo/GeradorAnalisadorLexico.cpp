
// -----------------------------------------------------------------------
//  Imprime o código fonte do Analisador Sintático
// -----------------------------------------------------------------------

#include <string>
#include "../util/Constantes.h"
#include "../util/Mensagem.h"
#include "GeradorArquivoFacade.h"

//-----------------------------------------------------------------------------
// Metodo que gera o arquivo do Analisador Lexico
//-----------------------------------------------------------------------------
void GeradorArquivoFacade::geraArquivoAnalisadorLexico() {
	// Verifica abertura de arquivos
	FILE *arq_lex_c, *arq_lex_h;
    if((arq_lex_c=fopen(nome_lex_c,"wt"))==NULL) Mensagem::falha(CRIACAO_ARQ,nome_lex_c);
    if((arq_lex_h=fopen(nome_lex_h,"wt"))==NULL) Mensagem::falha(CRIACAO_ARQ,nome_lex_h);

	// Gera arquivo .h do lexico
    std::string fonte =
	"\n"
	"\n//---------------------------------------------------------------------------"
	"\n#ifndef AnalisadorLexicoH"
	"\n#define AnalisadorLexicoH"
	"\n//---------------------------------------------------------------------------"
	"\n"
	"\n#include <string>"
	"\n#include <vector>"
	"\n"
	"\n//------------------------------------------------------------------"
	"\n// VO que guarda os dados de um token do analisador léxico"
	"\n//------------------------------------------------------------------"
	"\nclass Token {"
	"\n"
	"\n  private:"
	"\n     short valor ;        // Dado utilizado para efetuar a análise sintática"
	"\n     std::string lexema ; // Texto correspondente ao token"
	"\n     int atributo;        // Atributo do token"
	"\n"
	"\n  public:"
	"\n     //---------------------------------------------------------------"
	"\n     // Construtores, armazena no objeto os dados da palavra reservada"
	"\n     //---------------------------------------------------------------"
	"\n     Token(int token, std::string lexema, int atributo)"
	"\n     	 : valor(token), lexema(lexema), atributo(atributo) { } ;"
	"\n"
	"\n     //--------------------------------------------------------"
	"\n     // Métodos get"
	"\n     //--------------------------------------------------------"
	"\n     int getValor() const { return this->valor; }"
	"\n     std::string getLexema() const { return this->lexema; }"
	"\n     int getAtributo() const { return this->atributo; }"
	"\n} ;"
	"\n"
	"\n//------------------------------------------------------------------------------"
	"\n// Classe que implementa o analisador léxico do compilador"
	"\n//------------------------------------------------------------------------------"
	"\nclass AnalisadorLexico {"
	"\n  private:"
	"\n      std::string msgErro ;"
	"\n  public:"
	"\n    // Função que, quando chamada, retorna o próximo token do código fonte"
	"\n    Token proxToken();"
	"\n    "
	"\n    // Método Construtor. Inicializa o analisador léxico."
	"\n    AnalisadorLexico(std::string codigoFonte) ;"
	"\n"
	"\n    // Informações usadas para tratamento de erros"
	"\n    bool encontrouErro() const { return msgErro != \"\"; };"
	"\n    std::string getMsgErro() { return msgErro; }"
	"\n};"
	"\n"
	"\n//---------------------------------------------------------------------------"
	"\n#endif"
	"\n//---------------------------------------------------------------------------"
	"\n" ;
    fputs(fonte.c_str(), arq_lex_h);

	// Gera arquivo .cpp do lexico
	fonte =
	"\n"
	"\n#include \"AnalisadorLexico.h\""
	"\n"
	"\n//------------------------------------------------------------------------------"
	"\n// Método Construtor. Inicializa o analisador léxico."
	"\n//------------------------------------------------------------------------------"
	"\nAnalisadorLexico::AnalisadorLexico(std::string codigoFonte) {"
	"\n}"
	"\n"
	"\n //---------------------------------------------------------------"
	"\n // Interface entre o analisador sintático e o analisador léxico."
	"\n // Retorna o token reconhecido"
	"\n //---------------------------------------------------------------"
	"\n Token AnalisadorLexico::proxToken() {"
	"\n	 return Token(1, \"program\", 0) ;"
	"\n }"
	;
	fputs(fonte.c_str(), arq_lex_c);

	// Fecha arquivos
    if (fclose(arq_lex_c)==EOF) Mensagem::falha(FECHAMENTO_ARQ,nome_lex_c);
    if (fclose(arq_lex_h)==EOF) Mensagem::falha(FECHAMENTO_ARQ,nome_lex_h);
}


