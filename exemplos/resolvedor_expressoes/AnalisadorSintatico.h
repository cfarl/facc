

//---------------------------------------------------------------------------
#ifndef AnalisadorSintaticoH
#define AnalisadorSintaticoH
//---------------------------------------------------------------------------

#include <stack>
#include <list>
#include <string>
#include "AnalisadorLexico.h"

//------------------------------------------------------------------------------
// Classe que implementa o analisador sint�tico do compilador
//------------------------------------------------------------------------------
class AnalisadorSintatico {

private:
	std::list <int> listaAtributos ;         // Guarda os atributos l�xicos para a an�lise sem�ntica
	std::stack<int> pilhaProducoesBottomUp ; // Pilha que guarda as producoes na a��o de reduzir
	std::stack<int> pilhaProducoesTopDown ;  // Pilha que guarda a sequencia de producoes chamadas no sem�ntico
	std::string msgErro ;

	// Fun��es auxiliares
	void limpaVariaveis() ;
	void transforma_producoes_analisador_semantico(int topo) ;
	int determina_desvio(int &estado,int &nao_terminal) ;
	void trata_erro(int &tipo,int &estado, Token tokenCorrente) ;

public:
	//------------------------------------------------------------------------------
	// Realiza a an�lise sint�tica
	//------------------------------------------------------------------------------
	void executa(std::string codigoFonte);

	//-----------------------------------------------------------------
	// Recupera a pilha de producoes seguida pela analise sint�tica
	//-----------------------------------------------------------------
	std::stack<int> getPilhaProducoesAnaliseSintatica() { return this->pilhaProducoesTopDown ; }

	//---------------------------------------------------------------------
	// Recupera a lista de atributos obtidos durante a an�lise l�xica
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
