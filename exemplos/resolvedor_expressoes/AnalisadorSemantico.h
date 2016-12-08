
//---------------------------------------------------------------------------
#ifndef AnalisadorSemanticoH
#define AnalisadorSemanticoH
//---------------------------------------------------------------------------

#include <stack>
#include <list>
#include <string>

//------------------------------------------------------------------
// Classe que implementa o analisador semantico do compilador
//------------------------------------------------------------------
class AnalisadorSemantico {
  public:      
	//------------------------------------------------------------------------------
	// Chama o analisador semantico
	//------------------------------------------------------------------------------
	int executa(std::stack<int> pilhaProducoesPrograma, std::list<int> listaAtributosPrograma) ;

  private:
	 std::stack<int> pilhaProducoesPrograma ;
	 std::list<int> listaAtributosPrograma ;
	 std::string msgErro ;
	 int prox_prod() ;
	 int prox_atributo() ;

  public:      
 //------------------------------------------------------------------------------
 // Tratamento de erros
 //------------------------------------------------------------------------------
 std::string getMsgErroSemantico() { return msgErro; }
 bool encontrouErro() const { return msgErro != ""; };

  private:
//------------------------------------------------------------------------------
// Funções para percorrer a árvore sintática
//------------------------------------------------------------------------------
int expressao_aritmetica();
int termo();
int fator();
};

//---------------------------------------------------------------------------
 #endif
