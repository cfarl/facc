/* -----------------------------------------------------------------------*
 *                                                                        *
 * Programa    : FACC.EXE                                Versao 2.0       *
 *                                                                        *
 * Modulo      : FACC2FIR.C                                               *
 *                                                                        *
 * Autores     : Edward Lucio Vieira Borba  			          *
 *               Claudio Vasconcelos Dobbin				  *
 *                                                                        *
 * Data inicio : 03/07/90                                                 *
 * Data Atual. : 15/09/91                                                 *
 *                                                                        *
 * Descricao   : Gera o conjunto de first para os nao terminais.          *
 *                                                                        *
 * -----------------------------------------------------------------------*/

#include "EstruturaDados.h"
#include <stdio.h>
#include "../util/Constantes.h"
#include "../util/Mensagem.h"
#include "Gramatica.h"
#include "../Lexico/Token.h"
#include "../util/SetUtil.h"
#include <stdlib.h>
#include <string.h>


std::set<int> Gramatica::leva_vazio;            /* Conjunto dos nao-terminais que derivam vazio */

/* --------------------------------------------------------------------------*
 * Descricao :Verifica se todos os nao-terminais atingem um terminal         *
 * Entrada   :Nenhuma                                                        *
 * Retorno   :Nenhum                                                         *
 *-------------------------------------------------------------------------- */
void Gramatica::atinge_terminal() {
	int j;
	bool restringe;

	std::set<int> c0 ;    // contem os nao-terminais que atingem terminais
	std::set<int> c1 ;
	do {
		c0 = c1 ;
		for (int i = 0 ; i < EstruturaDados::listaProducoes.size()-1 ; i++) {
			struct Producao *producao = EstruturaDados::listaProducoes[i+1] ;
			if (!SetUtil::contem(c1,producao->codigoLadoEsquerdo))	{
				for (restringe = false, j = 0 ;	(j < producao->listaSimbolosLadoDireito.size()) && (!restringe); j++) {
					struct SimboloLadoDireitoProducao  *ladoDireito = producao->listaSimbolosLadoDireito[j] ;
					restringe = (ladoDireito->tipo == NAO_TERMINAL) && (!SetUtil::contem(c1,ladoDireito->codigo));
				}

				if (!restringe) {
					c1.insert(producao->codigoLadoEsquerdo);
				}
			}
		}
	} while (!SetUtil::equivalenteSet(c0,c1));

	for (int i = 1; i <= EstruturaDados::listaNaoTerminais.size(); i++) {
		if ( !SetUtil::contem(c1, i) )
			Mensagem::erro(ATINGE_TERMINAL, i);
	}
}

/* --------------------------------------------------------------------------*
 * Descricao :Verifica se simbolo inicial atinge todos os nao-terminais      *
 * Entrada   :Nenhuma                                                        *
 * Retorno   :Nenhum                                                         *
 *-------------------------------------------------------------------------- */

void Gramatica::atingido_por_inicial() {
	std::set<int> c0, c1 ;  // contem os nao-terminais atingidos por inicial
	std::set<int> c2 ;      // contem producoes ja percorridas

	// Insere simbolo inicial em c1
	c1.insert(EstruturaDados::listaProducoes[1]->codigoLadoEsquerdo);
	do {
		c0 = c1 ;
		for (int i = 0, n_prod = 1 ; i < EstruturaDados::listaProducoes.size()-1 ; i++, n_prod++) {
			struct Producao *q = EstruturaDados::listaProducoes[n_prod] ;

			if (SetUtil::contem(c1, q->codigoLadoEsquerdo) && ( !SetUtil::contem(c2, n_prod) ) ) {
				//for (r = q->dir, j = 0 ; j < q->tamanho ; r++, j++) {
				for (int j = 0 ; j < q->listaSimbolosLadoDireito.size() ; j++) {
					struct SimboloLadoDireitoProducao  *r = q->listaSimbolosLadoDireito[j];
					if ( (r->tipo == NAO_TERMINAL) )
						c1.insert(r->codigo);
				}
				c2.insert(n_prod);
			}
		}
	} while (!SetUtil::equivalenteSet(c0, c1));

	for (int i = 1; i <= EstruturaDados::listaNaoTerminais.size(); i++) {
		if (!SetUtil::contem(c1,i))
			Mensagem::erro(ATINGIDO_POR_INICIAL, i);
	}
}

/* --------------------------------------------------------------------------*
 * Descricao :Verifica quais simbolos nao-terminais derivam vazio            *
 * Entrada   :Nenhuma                                                        *
 * Retorno   :Nenhum                                                         *
 *-------------------------------------------------------------------------- */

void Gramatica::derivacao_vazio() {
	std::set<int> c0;  // contem os nao-terminais que derivam vazio
	struct SimboloLadoDireitoProducao  *r;
	int i, j;
	bool restringe ;
	leva_vazio.clear();
	do {
		c0 = leva_vazio;
		for (i = 0 ; i < EstruturaDados::listaProducoes.size()-1 ; i++) {
			struct Producao *q = EstruturaDados::listaProducoes[i+1] ;
			if (!SetUtil::contem(leva_vazio, q->codigoLadoEsquerdo)) {
				for (restringe = false, j = 0 ;	(j < q->listaSimbolosLadoDireito.size()) && (!restringe); j++) {
					 r = q->listaSimbolosLadoDireito[j];
					 restringe = (r->tipo == TERMINAL) || ( (r->tipo == NAO_TERMINAL) && (!SetUtil::contem(leva_vazio, r->codigo)) );
				}

				if (!restringe)
					leva_vazio.insert(q->codigoLadoEsquerdo);
			}
		}
	} while (!SetUtil::equivalenteSet(c0,leva_vazio));
}


//--------------------------------------------------------------------
// Verifica se a gramatica está de acordo com as regras definidas
//--------------------------------------------------------------------
void Gramatica::critica_gramatica() {
  EstruturaDados::ajustaListaTerminais();
  EstruturaDados::ajustaListaProducoes();
  atinge_terminal();
  atingido_por_inicial();
  derivacao_vazio();
}

/* --------------------------------------------------------------------------*
 * Descricao :Calcula first do lado direito de cada producao                 *
 * Entrada   : struct lado_dir *q : ponteiro p/ lado direito da producao     *
 *             tam : tamanho do lado direito da producao                     *
 * Retorno   :Nenhum                                                         *
 *-------------------------------------------------------------------------- */
std::set<int> Gramatica::first_lado_direito(std::vector<struct SimboloLadoDireitoProducao *> listaSimbolosLadoDireito) {
	std::set<int> retorno;

	// Se o primeiro simbolo e vazio, retorna conjunto com VAZIO
	if (listaSimbolosLadoDireito[0]->tipo == VAZIO) {
		retorno.insert(VAZIO);
		return retorno;
	}

	// Caso contrario
	bool vazio;
	int i;
	for (vazio = true, i = 0; i < listaSimbolosLadoDireito.size() && vazio; i++) {
		if (listaSimbolosLadoDireito[i]->tipo == TERMINAL) {
			vazio = false;
			retorno.insert(listaSimbolosLadoDireito[i]->codigo);
		} else	{
			// nao-terminal apontado por "q"
			struct EntradaTabelaSimbolos* naoTerminal = EstruturaDados::listaNaoTerminais[listaSimbolosLadoDireito[i]->codigo - 1];
			SetUtil::uniao(retorno, retorno, naoTerminal->first);
			vazio = SetUtil::contem(naoTerminal->first, VAZIO);
		}
	}
	if (!vazio) {  // ultimo simbolo nao leva em vazio
		retorno.erase(VAZIO);
	}

	return retorno;
}

/* --------------------------------------------------------------------------*
 * Descricao :Calcula first dos simbolos nao-terminais                       *
 * Entrada   :Nenhuma                                                        *
 * Retorno   :Nenhum                                                         *
 *-------------------------------------------------------------------------- */

void Gramatica::calcula_first() {
	// Percorre a lista de nao terminais
	for (int i = 1; i <= EstruturaDados::listaNaoTerminais.size(); i++) {
		struct EntradaTabelaSimbolos *naoTerminal = EstruturaDados::listaNaoTerminais[i-1] ;
		naoTerminal->first.clear();
		// Adiciona 'vazio' no first do nao terminal se ele deriva 'vazio'
		if (SetUtil::contem(leva_vazio, i)) {
			naoTerminal->first.insert(VAZIO);
		}
	}
	leva_vazio.clear();

	// Percorre a lista de producoes
	for (int i = 0 ; i < EstruturaDados::listaProducoes.size()-1 ; i++) {
		struct Producao *producaoLista = EstruturaDados::listaProducoes[i+1] ;
		// Adiciona 'terminal' no first do nao terminal se o lado direito inicia com 'terminal'
		if (producaoLista->listaSimbolosLadoDireito[0]->tipo == TERMINAL) {
			struct EntradaTabelaSimbolos *naoTerminal = EstruturaDados::listaNaoTerminais[producaoLista->codigoLadoEsquerdo - 1];
			naoTerminal->first.insert(producaoLista->listaSimbolosLadoDireito[0]->codigo) ;
		}
	}

	std::set<int> set_aux ;
	bool mudou = true ;
	while (mudou) {
		mudou = false;

		// Percorre a lista de producoes
		for (int i = 0 ; i < EstruturaDados::listaProducoes.size()-1 ; i++) {
			struct Producao *producaoLista = EstruturaDados::listaProducoes[i+1] ;

			// Adiciona first do lado direito da procucao no first do nao terminal
			struct EntradaTabelaSimbolos *naoTerminal = EstruturaDados::listaNaoTerminais[producaoLista->codigoLadoEsquerdo - 1];
			std::set<int> set_aux2 = first_lado_direito(producaoLista->listaSimbolosLadoDireito);
			SetUtil::uniao(set_aux, naoTerminal->first, set_aux2);

			if (!SetUtil::equivalenteSet(naoTerminal->first, set_aux)) {
				naoTerminal->first = set_aux ;
				mudou = true;
			}
		} // fim for
	} // fim while
}


