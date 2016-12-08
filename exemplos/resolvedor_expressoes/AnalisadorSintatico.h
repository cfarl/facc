

//---------------------------------------------------------------------------
#ifndef AnalisadorSintaticoH
#define AnalisadorSintaticoH
//---------------------------------------------------------------------------

#include <stack>
#include <list>
#include <string>
#include "AnalisadorLexico.h"

//------------------------------------------------------------------------------
// Classe que implementa o analisador sintático do compilador
//------------------------------------------------------------------------------
class AnalisadorSintatico {

private:
	std::list <int> listaAtributos ;         // Guarda os atributos léxicos para a análise semântica
	std::stack<int> pilhaProducoesBottomUp ; // Pilha que guarda as producoes na ação de reduzir
	std::stack<int> pilhaProducoesTopDown ;  // Pilha que guarda a sequencia de producoes chamadas no semântico
	std::string msgErro ;

	// Funções auxiliares
	void limpaVariaveis() ;
	void transforma_producoes_analisador_semantico(int topo) ;
	int determina_desvio(int &estado,int &nao_terminal) ;
	void trata_erro(int &tipo,int &estado, Token tokenCorrente) ;

public:
	//------------------------------------------------------------------------------
	// Realiza a análise sintática
	//------------------------------------------------------------------------------
	void executa(std::string codigoFonte);

	//-----------------------------------------------------------------
	// Recupera a pilha de producoes seguida pela analise sintática
	//-----------------------------------------------------------------
	std::stack<int> getPilhaProducoesAnaliseSintatica() { return this->pilhaProducoesTopDown ; }

	//---------------------------------------------------------------------
	// Recupera a lista de atributos obtidos durante a análise léxica
	//---------------------------------------------------------------------
	std::list<int> getListaAtributosAnaliseLexica() { return this->listaAtributos ; }

	//------------------------------------------------------------------------------
	// Tratamento de erros
	//------------------------------------------------------------------------------
	std::string getMsgErroSintatico() { return msgErro; }
	bool encontrouErro() const { return msgErro != ""; };
} ;


//---------------------------------------------------------------------------

#endif
