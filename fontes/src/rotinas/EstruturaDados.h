

//---------------------------------------------------------------------------
#ifndef EstruturaDados_H
#define EstruturaDados_H
//---------------------------------------------------------------------------

#include <vector>
#include <string>
#include <set>


//------------------------------------------------------------------------------
// Lista de producoes da gramatica
//------------------------------------------------------------------------------
struct Producao {
	std::string ladoEsquerdo;  // Nome do simbolo do lado esquerdo da producao
	int  codigoLadoEsquerdo;   // Codigo do nao terminal do lado esquerdo da producao
	int linha;
	int numNaoTerminaisLadoDireito;
	std::vector<struct SimboloLadoDireitoProducao*> listaSimbolosLadoDireito;
};

//------------------------------------------------------------------------------
// Guarda dados de um simbolos a direita da producao (terminal ou nao terminal)
//------------------------------------------------------------------------------
struct SimboloLadoDireitoProducao {
  int    codigo;  // Codigo do terminal/nao terminal do lado direito da producao
  char   tipo;    // Terminal, Nao-Terminal, Vazio
  std::string nome ;
};


//----------------------------------------------------
// Entrada da tabela de simbolos
//----------------------------------------------------
struct EntradaTabelaSimbolos {
  std::string nome ;
  int producao_corrente = 0;
  std::vector<int> listaProducaoAssociada;   // So associado a simbolo nao terminal
  bool possuiAtributo;
  std::set<int> first;
  std::vector<int> shift;
  std::vector<int> reduce;
};


//-------------------------------------------------------------------------------
// Representa um item da coleção canônica de itens LR(1)
// Um item tem a forma [ producao com ponto, lookahead ]
// (Ex. [ A -> alfa . B beta , a ], onde a é um terminal
//-------------------------------------------------------------------------------
struct ItemCanonico {
  int nro_producao;   // Producao da gramatica ( ex. A -> alfa B beta )
  int posic_ponto;    // Posicao do ponto na producao (antes do lado direito é zero)
  std::set<int> look_ahead ;
};

//--------------------------------------------------------------------------
// Representa um estado LR(1)
// Um estado é definido por um conjunto de itens canonicos
//--------------------------------------------------------------------------
struct EstadoLR1 {
  int num_estado;
  bool conflito;                               /* +-----------------+  */
  std::vector<struct ItemCanonico> itens;      /* !   estado        !  */
  std::vector<struct Acao> acao;               /* +-----------------+  */
  std::vector<struct Goto> gotox;              /*  !     !       !     */
};                                             /* item  acao    goto   */

//--------------------------------------------------
// Representa uma acao da tabela sintatica
//--------------------------------------------------
struct Acao {
  int codigoTerminal, acao;
};

//--------------------------------------------------
// Representa um goto da tabela sintatica
//--------------------------------------------------
struct Goto {
  int codigoNaoTerminal, estado;
};

class EstruturaDados {
public:
	//---------------------------------------------------------------------
	// Listas utilizadas pelo programa
	//---------------------------------------------------------------------
	static std::vector<struct EntradaTabelaSimbolos*> listaTerminais;
	static std::vector<struct EntradaTabelaSimbolos*> listaNaoTerminais;
	static std::vector<struct EntradaTabelaSimbolos*> listaAtributos;
	static std::vector<struct Producao*> listaProducoes;
	static std::vector<std::string> listaTexto;
	static std::vector<struct EstadoLR1 *> listaEstados;

	static FILE *arq_fonte;  // Arquivo com gramatica fonte
	static FILE *arq_est;  	 // Arquivo estados

	static int linhaCorrenteFonte;	 // Numero da linha corrente do fonte

	static bool linha_errada;                     /****** Indica erro anterior na linha */
	static bool linha_suspeita;                 /* Indica advertencia anterior na linha */
	static bool houve_erro;                          /* Indica erros na gramatica */


	//---------------------------------------------------------------------
	// Metodos da classe
	//---------------------------------------------------------------------
	static void liberaListasIniciais();
	static void liberaListaEstados();
	static void ajustaListaProducoes();
	static void ajustaListaTerminais();
};


//---------------------------------------------------------------------------
#endif
