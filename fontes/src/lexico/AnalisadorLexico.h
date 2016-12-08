
//---------------------------------------------------------------------------
#ifndef Lexico_H
#define Lexico_H
//---------------------------------------------------------------------------

#include "TabelaHash.h"

//------------------------------------------------------------------------------
// Classe que implementa o Analisador Léxico do Facc
//------------------------------------------------------------------------------
class AnalisadorLexico {
 private:
	void proxCaractere();
	void eliminaComentario();
	int reconheceIdentificador(char tipo_token);
	int reconheceTerminal(char delimitador);
	int reconheceTexto(char tipo_texto, char del_abre);

	void insereTabelaHash(std::string identificador, char tipoSimbolo, int posLista);
	int insereLista(std::string identificador, char tipoSimbolo);
	int procuraTabelaHash(std::string texto, char tipo);
	void novaLinha();
	void incluiListaProducoes(char tipoSimbolo, int codigo,bool *ladoDireitoProducao);

	TabelaHash tabela_hash ;

	struct Producao *producaoAtual;   /* Prod. corrente da lista de prod. */
	char charCorrente;                /* Ultimo caractere lido do fonte */


 public:
	AnalisadorLexico();

	//---------------------------------------------------------------------------
	// Atributo de um terminal
	//---------------------------------------------------------------------------
	int atributo_terminal;

	//---------------------------------------------------------------------------
	// Método que, quando chamado, retorna o próximo token da gramática
	//---------------------------------------------------------------------------
	int proxToken();
} ;

//---------------------------------------------------------------------------
#endif
