
//---------------------------------------------------------------------------
#ifndef AnalisadorSintaticoH
#define AnalisadorSintaticoH
//---------------------------------------------------------------------------

#include "../lexico/AnalisadorLexico.h"
#include <list>
#include <stack>

//------------------------------------------------------------------------------
// Classe que implementa o analisador sintatico do compilador
//------------------------------------------------------------------------------
class AnalisadorSintatico {
private:
	std::list <int> listaAtributos ;         // Guarda os atributos léxicos para a análise semântica
	std::stack<int> pilhaProducoesBottomUp ; // Pilha que guarda as producoes na ação de reduzir
	std::stack<int> pilhaProducoesTopDown ;  // Pilha que guarda a sequencia de producoes chamadas no semântico

	// Funções auxiliares
	void limpaVariaveis();
	void transforma_producoes_analisador_semantico(int topo);
	const int determina_desvio(int estado,int nao_terminal);
	void trata_erro(int tipo, int estado, int *token, AnalisadorLexico *lexico, std::stack<int> *pilhaEstados);

public:
	//------------------------------------------------------------------------------
	// Realiza a analise sintatica
	//------------------------------------------------------------------------------
	void execute();

	//-----------------------------------------------------------------
	// Recupera a pilha de producoes seguida pela analise sintatica
	//-----------------------------------------------------------------
	std::stack<int> getPilhaProducoesAnaliseSintatica() { return this->pilhaProducoesTopDown ; }

	//---------------------------------------------------------------------
	// Recupera a lista de atributos obtidos durante a analise lexica
	//---------------------------------------------------------------------
	std::list<int> getListaAtributosAnaliseLexica() { return this->listaAtributos ; }

} ;


//---------------------------------------------------------------------------
#endif

