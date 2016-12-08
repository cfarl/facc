
#include "TabelaHash.h"
#include <algorithm>

//-----------------------------------------------------
// Insere um novo elemento na tabela de hash
//-----------------------------------------------------
void TabelaHash::insere(std::string chave, struct EntradaTabelaHash entrada) {
	std::transform(chave.begin(), chave.end(), chave.begin(), ::tolower);
	std::vector<EntradaTabelaHash>* listaChave = procura(chave);
	if(listaChave == 0) {
		std::vector<EntradaTabelaHash> novo ;
		tabela_hash[chave] = novo ;
		listaChave = &(tabela_hash[chave]);
	}
	listaChave->push_back(entrada);
}

//-----------------------------------------------------
// Procura um novo elemento na tabela de hash
//-----------------------------------------------------
struct std::vector<EntradaTabelaHash>* TabelaHash::procura(std::string texto) {
	std::string chave = texto ;
	std::transform(chave.begin(), chave.end(), chave.begin(), ::tolower);
	if ( tabela_hash.find(chave) == tabela_hash.end() ) {
		return 0 ;
	} else {
		return &(tabela_hash[chave]);
	}
}


