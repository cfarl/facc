

#include "ProcessaEstados.h"

#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "../lexico/Token.h"
#include "../util/Constantes.h"
#include "../util/Mensagem.h"
#include "../util/SetUtil.h"
#include "EstruturaDados.h"

#define SIMB_LADO_DIR_PROD(x,y)       ((EstruturaDados::listaProducoes[x])->listaSimbolosLadoDireito[y])

//----------------------------------------------------
// Funcao para ordenacao das acoes
//----------------------------------------------------
bool ordenaAcao (Acao i, Acao j) {
	return (i.codigoTerminal < j.codigoTerminal);
}


/* --------------------------------------------------------------------------*
 * Descricao : Insere um novo estado na lista de estados.                    *
 * Entrada   : Nenhuma                                                       *
 * Retorno   : Nenhum                                                        *
 *-------------------------------------------------------------------------- */
struct EstadoLR1 * ProcessadorEstados::criaNovoEstado(std::vector<struct ItemCanonico> itensEstado) {
  // Cria novo estado
  struct EstadoLR1 *est = new EstadoLR1();
  est->conflito = false;
  est->acao.clear();
  est->gotox.clear();
  est->num_estado = EstruturaDados::listaEstados.size();
  est->itens = itensEstado ;

  // Adiciona o estado na lista de estados
  EstruturaDados::listaEstados.push_back(est);
  return est ;
}


/* --------------------------------------------------------------------------
 * Descricao : Atualiza os look-ahead do estado, adicionando aqueles que aparecem na lista de itens
 * Entrada   : Ponteiro para estado a ser atualizado
 *-------------------------------------------------------------------------- */
void ProcessadorEstados::atualizaLookAhead(struct EstadoLR1 *est_igual, std::vector<struct ItemCanonico> listaItens) {
  // Faz com que est_igual fique com seus lookahead e aqueles da lista de itens
  for (int i = 0; i < listaItens.size(); i++ ) {
	  struct ItemCanonico itemLista = listaItens[i];
	  struct ItemCanonico itemEstado = est_igual->itens[i];
	  SetUtil::uniao(itemEstado.look_ahead, itemEstado.look_ahead, itemLista.look_ahead);
	  est_igual->itens[i] = itemEstado ;
  }

}

/* --------------------------------------------------------------------------
 * Descricao : Cria um novo item canônico LR(1)
 * Entrada   : numProducao : numero da producao ao qual se refere o item.
 *             posPonto : posicao do ponto na producao (antes do lado direito é zero)
 *             la : conjunto de look-ahead para a producao com ponto
 * Retorno   : Nenhum
 *-------------------------------------------------------------------------- */
struct ItemCanonico ProcessadorEstados::criaNovoItemCanonicoLR1(int numProducao, int posPonto, std::set<int> lookahead) {
  struct ItemCanonico novoItem ;
  novoItem.nro_producao = numProducao;
  novoItem.posic_ponto = posPonto;
  novoItem.look_ahead = lookahead;
  return novoItem ;
}

/* --------------------------------------------------------------------------*
 * Descricao : Insere os novos estados na lista de goto                      *
 * Entrada   : est : ponteiro para estado a ser atualizado                   *
 *             dir : ponteiro para o simbolo referente ao goto               *
 *             novo_est : numero do novo estado                              *
 * Retorno   : Nenhum                                                        *
 *-------------------------------------------------------------------------- */

bool ordenaGoto (Goto i, Goto j) {
	return (i.codigoNaoTerminal < j.codigoNaoTerminal);
}

void ProcessadorEstados::insere_goto(struct EstadoLR1 *est, struct SimboloLadoDireitoProducao *dir, int novo_est) {
  // Símbolo do lado direito da producao é um não terminal
  if (dir->tipo == NAO_TERMINAL)  {
	struct Goto pnovo;
    pnovo.codigoNaoTerminal = dir->codigo;
    pnovo.estado = novo_est;
    est->gotox.push_back(pnovo);
    // Ordena gotox pelo código do não terminal
    std::sort (est->gotox.begin(), est->gotox.end(), ordenaGoto);

  // Símbolo do lado direito da producao é um terminal
  } else {
	int i;
	for (i=0; i < est->acao.size(); i++) { /* so' varre */
		 if(est->acao[i].codigoTerminal == dir->codigo) break ;
	}

	if(i >= est->acao.size())
    	Mensagem::falha(LISTA_ACAO,NULL);
	est->acao[i].acao += novo_est;
  }
}

/* --------------------------------------------------------------------------*
 * Descricao : Verifica se estado calculado ja existe.                       *
 * Entrada   : identico - torna-se verdadeiro se estado for identico         *
 *             est_igual- pont. para estado com mesmo core                   *
 * Retorno   : Numero do estado se os "CORES" forem iguais; FALSE caso       *
 *             contrario.                                                    *
 *-------------------------------------------------------------------------- */

// Retorna true se set 1 é subset de st2
bool subsetSet(std::set<int> set1, std::set<int> set2) {
	if(set1.size() == 0) return true ;

	for (std::set<int>::iterator it=set1.begin(); it!=set1.end(); ++it) {
		if(set2.find(*it) == set2.end()) return false;
	}
	return true ;
}



int ProcessadorEstados::procuraEstado(bool *identico, std::vector<struct ItemCanonico> listaItens) {
	struct EstadoLR1 *est;
	struct ItemCanonico itemEstado, itemLista;
	int i, j;
	for (i = 0; i < EstruturaDados::listaEstados.size(); i++ ) { /* Varre est.  */
		est = EstruturaDados::listaEstados[i];
		if (est->itens.size() == listaItens.size()) {
			*identico = true;
			for (j = 0; j < est->itens.size(); j++ ) {
				itemEstado = est->itens[j];
				itemLista = listaItens[j];

				if ( (itemEstado.nro_producao != itemLista.nro_producao) ||      /* Testa cores */
						(itemEstado.posic_ponto != itemLista.posic_ponto) ) {
					*identico = false;
					break;
				} else if ( !subsetSet(itemLista.look_ahead, itemEstado.look_ahead) ) {
						*identico = false;
				}
			}

			if (j == listaItens.size()) {                             /* Estado ja' existe */
				return i;
			}
		}
	}
	return 0;                                              /* Novo estado */
}

/* --------------------------------------------------------------------------*
 * Descricao : Exibe conflitos de shift-reduce e reduce-reduce (reduce)      *
 * Entrada   : tipo : tipo de cabecalho                                      *
 *             nro_prod : numero da producao com conflito                    *
 *             termx    : terminal que apresentou conflito                   *
 * Retorno   : Nenhum                                                        *
 *-------------------------------------------------------------------------- */

void ProcessadorEstados::conflito_reduce(int tipo, int nro_prod, int termx,int n_est, std::set<int> parametrosInformados) {
	if (tipo == 0) {
		fprintf(stdout, "\nEst(%3d) c/ \'%s\': Conflito Reduce",
				n_est, EstruturaDados::listaTerminais[termx - 1]->nome.c_str() );
		if(SetUtil::contem(parametrosInformados, P_ESTADOS))
			fprintf(EstruturaDados::arq_est, "\nEst(%3d) c/ \'%s\': Conflito Reduce",
					n_est, EstruturaDados::listaTerminais[termx - 1]->nome.c_str() );

	} else if (tipo == 1) {
		fputs("/ Reduce", stdout);
		if(SetUtil::contem(parametrosInformados, P_ESTADOS))
			fputs("/ Reduce", EstruturaDados::arq_est);
	}
	fprintf(stdout, " P(%d)", nro_prod);
	if(SetUtil::contem(parametrosInformados, P_ESTADOS))
		fprintf(EstruturaDados::arq_est, " P(%d)", nro_prod);
}



/* --------------------------------------------------------------------------*
 * Descricao : Exibe conflitos de shift-reduce (shift)                       *
 * Entrada   : set : conjunto de producoes que ocasionaram o conflito        *
 *             termx : terminal que apresentou conflito                      *
 * Retorno   : Nenhum                                                        *
 *-------------------------------------------------------------------------- */

void ProcessadorEstados::conflito_shift(std::vector<int> lista, int termx, int n_est, std::set<int> parametrosInformados) {
  fprintf(stdout, "\nEst(%3d) c/ \'%s\': Conflito Shift",
		  n_est, EstruturaDados::listaTerminais[termx - 1]->nome.c_str() );
  if(SetUtil::contem(parametrosInformados, P_ESTADOS))
    fprintf(EstruturaDados::arq_est, "\nEst(%3d) c/ \'%s\': Conflito Shift",
 	  	      n_est, EstruturaDados::listaTerminais[termx - 1]->nome.c_str() );

  for (int i=0; i < lista.size(); i++) {
    fprintf(stdout, " P(%d)", lista[i]);
    if(SetUtil::contem(parametrosInformados, P_ESTADOS))
      fprintf(EstruturaDados::arq_est, " P(%d)", i);
  }
}

/* --------------------------------------------------------------------------*
 * Descricao : Inclui no conjunto de shift de um terminal uma producao       *
 * Entrada   : termx : terminal a ser atualizado                             *
 *             nro_prod : numero da producao a ser incluida                  *
 * Retorno   : Nenhum                                                        *
 *-------------------------------------------------------------------------- */

void ProcessadorEstados::adiciona_shift(int termx, int nro_prod) {
  EstruturaDados::listaTerminais[termx-1]->shift.push_back(nro_prod);
}


/* --------------------------------------------------------------------------*
 * Descricao : Inclui no conjunto de reducoes dos terminais uma producao     *
 * Entrada   : set : conjunto dos terminais que serao atualizados            *
 *             nro_prod : numero da producao a ser incluida                  *
 * Retorno   : Nenhum                                                        *
 *-------------------------------------------------------------------------- */

void ProcessadorEstados::adiciona_reducao(std::set<int> myset, int nro_prod) {
  for (std::set<int>::iterator i=myset.begin(); i!=myset.end(); ++i)  {
	  EstruturaDados::listaTerminais[*i - 1]->reduce.push_back(nro_prod);
  }
}


/* --------------------------------------------------------------------------*
 * Descricao : Determina as acoes de shift para o estado                     *
 * Entrada   : est : ponteiro para estado                                    *
 * Retorno   : Nenhum                                                        *
 *-------------------------------------------------------------------------- */

void ProcessadorEstados::determina_shift(struct EstadoLR1 *est) {
	for (int i = 1; i <= est->itens.size(); i++) {
		struct ItemCanonico it = est->itens[i-1];
		if (!( EstruturaDados::listaProducoes[it.nro_producao]->listaSimbolosLadoDireito.size() == it.posic_ponto )) { /* Shift */
			struct SimboloLadoDireitoProducao *p = SIMB_LADO_DIR_PROD(it.nro_producao, it.posic_ponto);
			if (p->tipo == TERMINAL) {
				adiciona_shift(p->codigo, it.nro_producao);
			}
		}
	}

	for (int i = 1; i <= EstruturaDados::listaTerminais.size(); i++) {
		struct EntradaTabelaSimbolos *t = EstruturaDados::listaTerminais[i-1];
		if (t->shift.size() > 0) {
			struct Acao act ;
			act.acao = 2000;                             /* Shift assumido */
			act.codigoTerminal = i;
			est->acao.push_back(act) ;
			t->shift.clear();
		}
	}
	// Ordena as acoes pelo nao terminal
	std::sort (est->acao.begin(), est->acao.end(), ordenaAcao);
}

/* --------------------------------------------------------------------------*
 * Descricao : Determina as reducoes e detecta conflitos na gramatica        *
 * Entrada   : Nenhuma                                                       *
 * Retorno   : Nenhum                                                        *
 *-------------------------------------------------------------------------- */

void ProcessadorEstados::determina_reducao(std::set<int> parametrosInformados) {
	char msg[17];
	int red, n_est, conflito_term;

	// Percorre a lista de estados
	for (n_est = 0 ; n_est < EstruturaDados::listaEstados.size() ; n_est++)  {
		struct EstadoLR1 *est = EstruturaDados::listaEstados[n_est];

		// Percorre os itens do estado
		for (int i = 0; i < est->itens.size(); i++) {
			struct ItemCanonico it = est->itens[i];
			// Se o item é uma producao com ponto mais á direita (A ->alfa .), registra 'reducao' para o lookahead do item
			if (EstruturaDados::listaProducoes[it.nro_producao]->listaSimbolosLadoDireito.size() == it.posic_ponto) {
				adiciona_reducao(it.look_ahead, it.nro_producao);

			// Caso contrário, se o ponto aparece aintes de um terminal, registra 'empilhar'
			} else {
				struct SimboloLadoDireitoProducao *simbLadoDireito = SIMB_LADO_DIR_PROD(it.nro_producao, it.posic_ponto);
				if (simbLadoDireito->tipo == TERMINAL) {
					adiciona_shift(simbLadoDireito->codigo, it.nro_producao);
				}
			}
		}

		// Percorre a lista de terminais
		for (int i = 1; i <= EstruturaDados::listaTerminais.size(); i++) {
			struct EntradaTabelaSimbolos *terminal = EstruturaDados::listaTerminais[i-1];
			//if ( t->reduce ) {
			if ( terminal->reduce.size() > 0 ) {
				conflito_term = 0;
				red = terminal->reduce[0];  // Pega primeira reducao

				struct Acao act ;
				if (terminal->shift.size() > 0) {
					conflito_shift(terminal->shift, i,n_est, parametrosInformados);          /* Lista prod com shift */
					conflito_reduce(1, red, i,n_est, parametrosInformados);
					est->conflito = true;                              /* Shift-reduce */
					conflito_term = 1;
				} else {
					act.acao = 1000 + red;           /* Acao = 1000 equivale a ACEITA */
					act.codigoTerminal = i;
					est->acao.push_back(act);
					std::sort (est->acao.begin(), est->acao.end(), ordenaAcao);
				}

				if (terminal->reduce.size() > 1) {  /* Mais de 1 reducao */
					red = terminal->reduce[1];
					if (!terminal->shift.size()) {                           /* Acao = reduce */
						est->conflito = true;                         /* Reduce-reduce */
						conflito_term = 2;
						conflito_reduce(0, act.acao - 1000, i,n_est, parametrosInformados);
					}
					conflito_reduce(2, red, i,n_est, parametrosInformados);
					for(int x=2; x < terminal->reduce.size(); x++) {
						red = terminal->reduce[x];
						conflito_reduce(2, red, i,n_est, parametrosInformados);
					}
				}
				terminal->reduce.clear();

				switch (conflito_term)
				{
				case 1 : strncpy(msg, "; Shift assumido",17); break;
				case 2 : strncpy(msg, "; Reduz pela 1a.",17); break;
				}
				msg[16] = '\0';
				if (conflito_term)	{
					fprintf(stdout, "%s", msg);
					if(SetUtil::contem(parametrosInformados, P_ESTADOS))
						fprintf(EstruturaDados::arq_est, "%s", msg);
				}
			}

			if (terminal->shift.size() > 0) {
				terminal->shift.clear();
			}
		} /* Fim do for de terminais */

	} /* Fim do for de estados */
}

/* --------------------------------------------------------------------------*
 * Descricao : Gera conjunto de look-ahead para um item                      *
 * Entrada   : nro_prod : numero da producao                                 *
 *             posic_ponto: posicao do ponto no item                         *
 *             la_pai : conjunto de look-ahead do item pai.                  *
 * Retorno   : Conjunto look-ahead do item.                                  *
 *-------------------------------------------------------------------------- */
//SET * ProcessadorEstados::gera_look_ahead(int nro_prod, int posic_ponto, SET *la_pai) {
std::set<int> ProcessadorEstados::gera_look_ahead(int nro_prod, int posic_ponto, std::set<int> la_pai) {
	std::set<int> retorno ; // Deletado em closure_conjunto

	struct Producao *producao = EstruturaDados::listaProducoes[nro_prod];

	// Caso A -> B. (Antigo Caso A -> .B)
	// Se o ponto está depois do ultimo simbolo do lado direito da producao, retorna o lookahead do item pai
	if ( producao->listaSimbolosLadoDireito.size() == posic_ponto)  {
		//assign(retorno, la_pai);
		retorno = la_pai ;
		return(retorno);
	}

	// Recupera o simbolo do lado direito que está na posicao do ponto
	struct SimboloLadoDireitoProducao *simbLadoDireito = EstruturaDados::listaProducoes[nro_prod]->listaSimbolosLadoDireito[posic_ponto] ;

    // Caso A -> B.a (Antigo Caso A -> .Ba)
	if ( simbLadoDireito->tipo == TERMINAL )  {
		retorno.insert(simbLadoDireito->codigo);
		return(retorno);
	}

	// Caso A -> B.C
	// Adiciona first de C no lookahead a ser retornado
	retorno = EstruturaDados::listaNaoTerminais[simbLadoDireito->codigo - 1]->first;  /* A -> .BC */
	retorno.erase(VAZIO);
	while(1) {
		if (!SetUtil::contem(EstruturaDados::listaNaoTerminais[simbLadoDireito->codigo - 1]->first, VAZIO))
			return(retorno);
		else {                                       /* VAZIO pertence a first(C) */
			posic_ponto++;
			if (posic_ponto == producao->listaSimbolosLadoDireito.size()) break ;

			simbLadoDireito = EstruturaDados::listaProducoes[nro_prod]->listaSimbolosLadoDireito[posic_ponto] ;
			// Caso A -> B.a
			if ( simbLadoDireito->tipo == TERMINAL ) {
				retorno.insert(simbLadoDireito->codigo);
				return(retorno);
			}
			// Caso A -> B.C
			SetUtil::uniao(retorno, retorno, EstruturaDados::listaNaoTerminais[simbLadoDireito->codigo - 1]->first);
			retorno.erase(VAZIO);
		}
	}
	SetUtil::uniao(retorno, retorno, la_pai);      /* Todos apos o ponto levam em vazio */
	return(retorno);
}

/* --------------------------------------------------------------------------*
 * Descricao : Verifica se o item informado ja existe na lista de itens canonicos
 * Entrada   : nro_prod : numero da producao
 *             posic_ponto: posicao do ponto no item
 *             la : conjunto de look-ahead do item pai.
 * Retorno   : TRUE se existir item com mesmo core, FALSE caso contrario
 *-------------------------------------------------------------------------- */
bool ProcessadorEstados::ja_existe_item( int nro_prod, int posic_ponto, std::set<int> la, std::vector<struct ItemCanonico> &listaItens) {
  // Percorre a lista de itens canonicos
  for (int i =0; i< listaItens.size(); i++) {
	struct ItemCanonico item = listaItens[i];
	// Se existe um item com a producao informada e posicao do ponto faz union com lookahead
    if ((item.nro_producao == nro_prod) && (item.posic_ponto == posic_ponto) ) {
      SetUtil::uniao(item.look_ahead, item.look_ahead, la);
      listaItens[i] = item ;
      return true;                                            /* Mesmo core */
    }
  }
  return false;                                    /* Item ainda nao existe */
}

/* --------------------------------------------------------------------------*
 * Descricao : Incrementa o conjunto novo_estado com novos itens.            *
 * Entrada   : codigoNaoTerminal : nao terminal a ser expandido.             *
 *             la : conjunto de look-ahead do item pai.                      *
 * Retorno   : Nenhum                                                        *
 *-------------------------------------------------------------------------- */

//void ProcessadorEstados::expande_estado(int codigoNaoTerminal, SET *la, struct st_itens *listaItens, int& tamEstado) {
void ProcessadorEstados::expande_estado(int codigoNaoTerminal, std::set<int> la, std::vector<struct ItemCanonico> &listaItens) {
  // Varre a lista de producoes
  for(int i = 1; i < EstruturaDados::listaProducoes.size(); i++) {
	struct Producao *producao = EstruturaDados::listaProducoes[i] ;

	// Se nao for a producao informada na chamada do método nao analisa
    if (producao->codigoLadoEsquerdo != codigoNaoTerminal) continue ;

    // Determina a posicao do ponto no item canonico
    int posPonto ;
    if ( producao->listaSimbolosLadoDireito[0]->tipo == VAZIO ) {
    	posPonto = 1;                                      /* Se A-># gerar A->#. */
    } else {
    	posPonto = 0;                                      /* senao gerar A->.à   */
    }

    // Se o item nao existe na lista de itens do estado, adiciona agora
    if ( !ja_existe_item(i, posPonto, la, listaItens) ) {
    	struct ItemCanonico novoItem = criaNovoItemCanonicoLR1(i, posPonto, la);
       listaItens.push_back(novoItem);
    }
   }              		   /* nro_producao = i, posic_ponto = 0 ou 1 */

}

/* --------------------------------------------------------------------------*
 * Descricao : Recupera o código do terminal do lado direito da producao que esta
 *             na posicao do ponto. Retorna -1 se naquela posicao há um nao terminal
 *             ou se o ponto está após o ultimo simbolo do lado direito
 * Entrada   : nro_prod : numero da producao.
 *             posic_ponto : posicao do ponto no item.
 * Retorno   : Nao terminal na posicao do ponto ou -1 se este nao existir.
 *-------------------------------------------------------------------------- */
//int ProcessadorEstados::getCodigoNaoTerminalLadoDireito( int nro_prod, int posic_ponto) {
int ProcessadorEstados::getCodigoNaoTerminalLadoDireito( int nro_prod, int posic_ponto) {
  // Se o ponto está depois do último simbolo do lado direito da producao retorna -1
  if ( EstruturaDados::listaProducoes[nro_prod]->listaSimbolosLadoDireito.size() == posic_ponto) {
     return -1;
  }

  // Se o simbolo do lado direito da producao, na posicao do ponto, é terminal retorna -1
  if (SIMB_LADO_DIR_PROD(nro_prod, posic_ponto)->tipo == TERMINAL) {
    return -1;
  }

  // Senao, retorna codigo do terminal do lado direito da producao, na posicao do ponto
  return SIMB_LADO_DIR_PROD(nro_prod, posic_ponto)->codigo ;
}

/* --------------------------------------------------------------------------
 * Descricao : Faz o fechamento de um conjunto de itens canonicos LR(1)
 * Algoritmo:
 *    Funcao fechamento(I: conjunto itens LR(1) );
 *    Inicio
 *       Repetir
 *           Para cada item [ A -> alfa . B beta, a ] em I,
 *                cada producao B -> gama da gramática
 *                e cada terminal b em First(beta)
 *                tal que [ B -> .gama, b ] não está em I faça
 *                   incluir [ B -> .gama, b ] em I
 *       Até que não possam ser adicionados mais itens ao conjunto I
 *       retornar I
 *    Fim;
 *-------------------------------------------------------------------------- */
//void ProcessadorEstados::fechamento(struct st_itens *listaItens, int& tamEstado) {
void ProcessadorEstados::fechamento(std::vector<struct ItemCanonico> &conjuntoItens) {
  int i;

  // Para cada item [ A -> alfa . B beta, a ] em I,
  struct ItemCanonico *item ;
  for (int i =0; i< conjuntoItens.size(); i++) {
	  struct ItemCanonico item = conjuntoItens[i] ;

	// Recupera simbolo do lado direito da producao, e que esta na posicao do ponto
	int codigoNaoTerminal = getCodigoNaoTerminalLadoDireito(item.nro_producao, item.posic_ponto) ;

	// Se nao achou um nao terminal na posicao do ponto, passa para o proximo item
	if(codigoNaoTerminal == -1) continue ;

	// Calcula First(beta)
	std::set<int> firstBeta = gera_look_ahead(item.nro_producao, item.posic_ponto + 1, item.look_ahead) ;
	// incluir [ B -> .gama, b ] em I
    expande_estado(codigoNaoTerminal, firstBeta, conjuntoItens);  /* tam_estado alterado em expande_estado */
  }
}

/* --------------------------------------------------------------------------*
 * Descricao : Insere itens adicionais em um novo estado                     *
 * Entrada   : it : ponteiro para item posterior ao que gerou o estado       *
 *             dir : ponteiro para simbolo a ser comparado                   *
 *             tam_estado : tamanho do estado gerador do novo estado         *
 *             cont_itens : posicional do item no conjunto de itens          *
 * Retorno   : Nenhum                                                        *
 *-------------------------------------------------------------------------- */
void ProcessadorEstados::insere_outros_itens(std::vector<struct ItemCanonico> listaItensEstadoAnalisado, int posUltimoItem, struct SimboloLadoDireitoProducao *dir,	int tam_estado, int cont_itens, std::vector<struct ItemCanonico> &listaItens, std::set<int> &itens_incluidos) {

  for ( ; cont_itens <= tam_estado; cont_itens++) {
	struct ItemCanonico item =  listaItensEstadoAnalisado[posUltimoItem] ;
	posUltimoItem++;

    if (item.posic_ponto < EstruturaDados::listaProducoes[item.nro_producao]->listaSimbolosLadoDireito.size() &&
    		(SIMB_LADO_DIR_PROD(item.nro_producao, item.posic_ponto)->tipo == dir->tipo) &&
			(SIMB_LADO_DIR_PROD(item.nro_producao, item.posic_ponto)->codigo == dir->codigo)) {
      struct ItemCanonico novoItem = criaNovoItemCanonicoLR1(item.nro_producao, item.posic_ponto + 1, item.look_ahead);
	  listaItens.push_back(novoItem) ;
      itens_incluidos.insert(cont_itens);
    }
  }
}

/* --------------------------------------------------------------------------
 * Descricao : Cria os estados canônicos do gerador LR(1)
 * Um estado é constituído de um conjunto de itens canônicos.
 * Um item é uma produção com um . do lado direito, entre elementos (A -> X.YZ)
 * e um conjunto de lookaheads para aquela produção com ponto
 *-------------------------------------------------------------------------- */
void ProcessadorEstados::geraEstadosLR1(std::set<int> parametrosInformados) {
    int cont_itens=1;               /* Numero de sequencia do item no conjunto */
  bool identico;                /* Indica se um estado e' identico a um novo */
  bool mudou_conj_est= false;   /* Houve alteracao num estado <= ao corrente */
  bool primeira_vez = true;               /* Calcula goto somente na 1a. vez */

  // Inicializacoes
  std::vector<struct ItemCanonico> listaItensEstadoCorrente;
  std::vector<struct ItemCanonico> listaItensEstadoAnalise;

  // Cria um item para o primeiro estado, informando nele a producao 0, que inicia a gramatica
  std::set<int> look_ahead ;
  //look_ahead.insert(EstruturaDados::tok_etx);
  look_ahead.insert(EstruturaDados::listaTerminais.size()); // Token de fim de arquivo

  // Cria primeiro item canonico, que corresponde à producao inicial com entrada 'token fim programa'
  // O item corresponde a S -> . simbolo inicial gramatica, { token fim programa }
  struct ItemCanonico novoItem = criaNovoItemCanonicoLR1(0, 0, look_ahead);           /* producao 0, posic_ponto 0 */
  listaItensEstadoCorrente.push_back(novoItem) ;

  // Faz fechamento (closure) do item inicial
  fechamento(listaItensEstadoCorrente);

  // Cria primeiro estado
  struct EstadoLR1 *ultimoEstado = criaNovoEstado(listaItensEstadoCorrente);
  struct ItemCanonico item = ultimoEstado->itens[0];   // Deixa item apontando para o 1o item canonico do 1o estado
  listaItensEstadoAnalise = ultimoEstado->itens ;
  int estadoCorrente = 0;
  determina_shift(ultimoEstado);
  listaItensEstadoCorrente.clear();
  std::set<int> itens_incluidos ;

  while(1) {
	  // Criacao de novos estados
	  if ( (item.posic_ponto < EstruturaDados::listaProducoes[item.nro_producao]->listaSimbolosLadoDireito.size() ) &&
			  !SetUtil::contem(itens_incluidos,cont_itens)) {

		  // Cria novo item canonico a partir do item analisado, com o ponto uma posicao a direita
		  struct ItemCanonico novoItem = criaNovoItemCanonicoLR1(item.nro_producao, item.posic_ponto + 1, item.look_ahead);
		  listaItensEstadoCorrente.push_back(novoItem) ;

		  // Cria itens derivados do novo item
		  if (ultimoEstado->itens.size() > cont_itens) {
			  insere_outros_itens(listaItensEstadoAnalise, cont_itens, SIMB_LADO_DIR_PROD(item.nro_producao, item.posic_ponto),
					  ultimoEstado->itens.size(), cont_itens+1, listaItensEstadoCorrente, itens_incluidos);
		  }

		  // Faz o fechamento da lista de itens canonicos
		  fechamento(listaItensEstadoCorrente);
		  identico = false;

		  // Se o conjunto de itens (estado) ainda não existe, cria um novo
		  int numNovoEstado = 0;
		  if ( (numNovoEstado = procuraEstado(&identico, listaItensEstadoCorrente)) == NULL) {
			  struct EstadoLR1* est = criaNovoEstado(listaItensEstadoCorrente);
			  numNovoEstado = EstruturaDados::listaEstados.size()-1 ;
			  listaItensEstadoCorrente.clear();
			  determina_shift(est);

		  // Agora, se o estado ja existe...
		  } else {
			  if (!identico) {                           /* Somente "CORES" iguais  */
				  // Recupera estado com mesmo 'core'
				  struct EstadoLR1 *est_igual = EstruturaDados::listaEstados[numNovoEstado];
				  // Atualiza lookahead do estado
				  atualizaLookAhead(est_igual, listaItensEstadoCorrente);
				  if (numNovoEstado <= estadoCorrente) {
					  mudou_conj_est = true;
				  }
			  }
			  listaItensEstadoCorrente.clear();
		  }

		  if (primeira_vez) {
			  insere_goto(ultimoEstado, SIMB_LADO_DIR_PROD(item.nro_producao, item.posic_ponto), numNovoEstado);
		  }
	  }

	  // Avanca para analisar o proximo item da colecao canonica do estado atual
	  if ( cont_itens < ultimoEstado->itens.size() ) {
		  item = ultimoEstado->itens[cont_itens];
		  cont_itens++;

	  // Passa para o proximo estado
	  } else if (estadoCorrente < EstruturaDados::listaEstados.size()-1) {   /* Passa para o proximo ultimoEstado */
		  estadoCorrente++;
		  ultimoEstado = EstruturaDados::listaEstados[estadoCorrente];
		  item = ultimoEstado->itens[0];
		  listaItensEstadoAnalise = ultimoEstado->itens ;
		  itens_incluidos.clear();
		  cont_itens = 1;

	  } else // Reprocessa estados em funcao de alteracao
		  if (mudou_conj_est) {
		  		  primeira_vez = false;
		  		  mudou_conj_est = false;
		  		  ultimoEstado = EstruturaDados::listaEstados[1];
		  		  item = ultimoEstado->itens[0];
		  		  listaItensEstadoAnalise = ultimoEstado->itens ;
		  		  estadoCorrente = 1;
		  		  itens_incluidos.clear();
		  		  cont_itens = 1;
	   } else
		  break;                                        /* Fim dos estados */
  } /* fim-while */

  determina_reducao(parametrosInformados);
}


