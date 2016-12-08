
//---------------------------------------------------------------------------
#ifndef ProcessadorEstados_H
#define ProcessadorEstados_H
//---------------------------------------------------------------------------

#include <set>
#include "EstruturaDados.h"

class ProcessadorEstados {
public:
	static void geraEstadosLR1(std::set<int> parametrosInformados);

private:
	static  struct EstadoLR1* criaNovoEstado(std::vector<struct ItemCanonico> itensEstado);
	static void atualizaLookAhead(struct EstadoLR1 *est_igual, std::vector<struct ItemCanonico> listaItens);
	static struct ItemCanonico criaNovoItemCanonicoLR1( int nro_prod, int posic_ponto, std::set<int> la);
	static void insere_goto(struct EstadoLR1 *est, struct SimboloLadoDireitoProducao *dir,	int novo_est);
	static int procuraEstado(bool *identico, std::vector<struct ItemCanonico> novoEstado);
	static void conflito_reduce(int tipo, int nro_prod, int termx,int n_est, std::set<int> parametrosInformados);
	static void conflito_shift(std::vector<int> set, int termx, int n_est, std::set<int> parametrosInformados);
	static void adiciona_shift(int termx, int nro_prod);
	static void adiciona_reducao(std::set<int> myset, int nro_prod);
	static void determina_shift(struct EstadoLR1 *est);
	static void determina_reducao(std::set<int> parametrosInformados);
	static std::set<int> gera_look_ahead(int nro_prod, int posic_ponto, std::set<int> la_pai);
	static bool ja_existe_item( int nro_prod, int posic_ponto, std::set<int> la, std::vector<struct ItemCanonico> &listaItens);
	static void expande_estado( int nterm_exp, std::set<int> la, std::vector<struct ItemCanonico> &novoEstado);
	static int getCodigoNaoTerminalLadoDireito( int nro_prod, int posic_ponto);
	static void fechamento(std::vector<struct ItemCanonico> &conjuntoItensCanonicos);
	static void insere_outros_itens(std::vector<struct ItemCanonico> listaItensEstadoAnalisado, int posUltimoItem, struct SimboloLadoDireitoProducao *dir, int tam_estado, int cont_itens, std::vector<struct ItemCanonico> &listaItens, std::set<int> &itens_incluidos);
};

//---------------------------------------------------------------------------
#endif
