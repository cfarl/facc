/* -----------------------------------------------------------------------*
 *                                                                        *
 * Programa    : FACC.EXE                                Versao 2.0       *
 *                                                                        *
 * Modulo      : FACCMEM.C                                                *
 *                                                                        *
 * Autores     : Edward Lucio Vieira Borba  			          *
 *                                                                        *
 * Data inicio : 11/04/91                                                 *
 * Data Atual. : 15/08/91                                                 *
 *                                                                        *
 * Descricao   : Gerencia as transferencias de listas para os arrays,     *
 *               bem como a liberacao destes ultimos   ao  final  da      *
 *               execucao ou quando nao sao mais necessarios.             *
 *                                                                        *
 * -----------------------------------------------------------------------*/

#include "EstruturaDados.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lexico/Token.h"
#include "../util/Constantes.h"
#include "../util/Mensagem.h"

std::vector<struct EntradaTabelaSimbolos*> EstruturaDados::listaTerminais;
std::vector<struct EntradaTabelaSimbolos*> EstruturaDados::listaNaoTerminais;
std::vector<struct EntradaTabelaSimbolos*> EstruturaDados::listaAtributos;
std::vector<struct Producao*> EstruturaDados::listaProducoes;
std::vector<struct EstadoLR1 *> EstruturaDados::listaEstados;
std::vector<std::string> EstruturaDados::listaTexto;

FILE * EstruturaDados::arq_fonte; 			      /* Arquivo com gramatica fonte */
FILE * EstruturaDados::arq_est;  		         /* Arquivo estados */

int EstruturaDados::linhaCorrenteFonte;	                        /* Numero da linha corrente do fonte */

bool EstruturaDados::linha_errada;                     /****** Indica erro anterior na linha */
bool EstruturaDados::linha_suspeita;                 /* Indica advertencia anterior na linha */
bool EstruturaDados::houve_erro=false;                          /* Indica erros na gramatica */

/* --------------------------------------------------------------------------*
 * Descricao : Libera as areas alocadas quando ocorrer erro.                 *
 * Entrada   : Nenhuma                                                       *
 * Retorno   : Nenhum                                                        *
 *-------------------------------------------------------------------------- */
void EstruturaDados::liberaListasIniciais() {
  // Lista de terminais
  for (unsigned int i=0; i< EstruturaDados::listaTerminais.size(); i++) {
    free(EstruturaDados::listaTerminais[i]);
  }

  // Lista de atributos
  for (unsigned int i=0; i< EstruturaDados::listaAtributos.size(); i++) {
	  free(EstruturaDados::listaAtributos[i]);
  }

  // Lista de nao terminais
  for (unsigned int i=0; i< EstruturaDados::listaNaoTerminais.size(); i++) {
    free(listaNaoTerminais[i]);
  }

  // Lista de producoes
  for (unsigned int i=0; i< EstruturaDados::listaProducoes.size(); i++) {
	struct Producao *tp = EstruturaDados::listaProducoes[i];
	for (unsigned int j=0; j< tp->listaSimbolosLadoDireito.size(); j++) {                  /* Lado direito       */
		free(tp->listaSimbolosLadoDireito[j]);
    }
    free(EstruturaDados::listaProducoes[i]);
  }

}


//--------------------------------------------------------------------------
// Descricao : Libera a memoria alocada para a lista de estados
//--------------------------------------------------------------------------
void EstruturaDados::liberaListaEstados() {
	  // Libera lista de estados
	  for (unsigned int w=0; w < EstruturaDados::listaEstados.size(); w++) {
		  struct EstadoLR1 *est = EstruturaDados::listaEstados[w];
		  free(est) ;
	  }
}

//--------------------------------------------------------------------------
// Descricao : Insere símbolo de fim de programa na lista de terminais
//--------------------------------------------------------------------------
void EstruturaDados::ajustaListaTerminais() {
  // Insere símbolo de fim de programa na lista de terminais
  struct EntradaTabelaSimbolos *terminal = new EntradaTabelaSimbolos();
  terminal->nome = "$";
  EstruturaDados::listaTerminais.push_back(terminal) ;
}


/* --------------------------------------------------------------------------*
 * Descricao : Adiciona a producao ACCEPT -> Simbolo Inicial da gramatica no *
 *             inicio da lista de producoes. Verifica se existem producoes   *
 *             repetidas.                                                    *
 *-------------------------------------------------------------------------- */
void EstruturaDados::ajustaListaProducoes() {
  // Insere no inicio da lista de producoes a producao ACCEPT -> Simbolo Inicial da gramatica
  struct Producao * novaProducao = new Producao();
  novaProducao->codigoLadoEsquerdo = 0;
  novaProducao->ladoEsquerdo = "ACCEPT" ;

  struct SimboloLadoDireitoProducao* novoLadoDireito = new SimboloLadoDireitoProducao();
  novoLadoDireito->codigo = EstruturaDados::listaProducoes[0]->codigoLadoEsquerdo;
  novoLadoDireito->tipo = NAO_TERMINAL;
  novoLadoDireito->nome = EstruturaDados::listaNaoTerminais[0]->nome ;
  novaProducao->listaSimbolosLadoDireito.push_back(novoLadoDireito);
  listaProducoes.insert(listaProducoes.begin(), novaProducao) ;

 // Verifica se existem producoes repetidas
 for (unsigned int h = 1;  h < EstruturaDados::listaProducoes.size() ; h++) {
	struct Producao* producaoLista = EstruturaDados::listaProducoes[h];
    //filhos_nt.push_back(producaoLista->numNaoTerminaisLadoDireito);

    // Compara nova producao com as existentes, para verificar se ela é repetida
    bool repetido = false;
    for (unsigned int u=h+1; u < listaProducoes.size() && (!repetido); u++) {
    	struct Producao *paux = listaProducoes[u];
    	if ((paux->codigoLadoEsquerdo == producaoLista->codigoLadoEsquerdo) && (paux->listaSimbolosLadoDireito.size() == producaoLista->listaSimbolosLadoDireito.size())) {
    		repetido = true;
    		for (unsigned int i = 0; (i < producaoLista->listaSimbolosLadoDireito.size()) && repetido; i++) {
    			struct SimboloLadoDireitoProducao *dirAux = paux->listaSimbolosLadoDireito[i];
    			struct SimboloLadoDireitoProducao *dirProdLista = producaoLista->listaSimbolosLadoDireito[i];
    			repetido = (dirProdLista->codigo == dirAux->codigo) && (dirProdLista->tipo == dirAux->tipo) ;
    		}
      }
    }

    if (repetido) {
    	Mensagem::erro(PRODUCAO_REPETIDA, producaoLista->linha);
    }
  }
}
