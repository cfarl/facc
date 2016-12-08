
//---------------------------------------------------------------------------
#ifndef TabelaSimbolos_H
#define TabelaSimbolos_H
//---------------------------------------------------------------------------

#include <map>
#include <vector>
#include <string>

//------------------------------------------------------------------------------
// Entrada na tabela de hash
//------------------------------------------------------------------------------
struct EntradaTabelaHash {
  char   tipo;
  int    posLista;
  std::string texto ;
};

//------------------------------------------------------------------------------
// Tabela Hash
//------------------------------------------------------------------------------
class TabelaHash {
 private:
	std::map<std::string, std::vector<EntradaTabelaHash> > tabela_hash ;

 public:
	//-------------------------------------------------
	// Insere um novo elemento na tabela
	//-------------------------------------------------
	void insere(std::string chave, struct EntradaTabelaHash entrada) ;

	//-------------------------------------------------
	// Remove um elemento da tabela
	//-------------------------------------------------
	std::vector<EntradaTabelaHash>* procura(std::string chave) ;
} ;


//---------------------------------------------------------------------------
#endif
