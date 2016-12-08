
// -----------------------------------------------------------------------
//  Imprime a tabela sintática do Analisador Sintático
// -----------------------------------------------------------------------


#include "../rotinas/EstruturaDados.h"
#include "GeradorArquivoFacade.h"
#include <string.h>
#include "../lexico/Token.h"
#include "../util/Mensagem.h"
#include "../util/Constantes.h"


//----------------------------------------------------------------------------------------------------------
// Metodo que gera o arquivo contendo as tabelas do Analisador Sintatico (acao, goto, nterm a dir,tam dir)
//----------------------------------------------------------------------------------------------------------
void GeradorArquivoFacade::imprime_tabelas_do_parser() {
  // Verifica abertura de arquivo
  FILE *arq_tpa;
  if( (arq_tpa=fopen(GeradorArquivoFacade::nome_tpa,"wt") ) == NULL) Mensagem::falha(CRIACAO_ARQ,GeradorArquivoFacade::nome_tpa);

  imprime_tab_acao(arq_tpa);
  imprime_tab_goto(arq_tpa);
  fputs("\n\n      /*   numero de simbolos a direita */\n",arq_tpa);
  imprime_nro_dir(arq_tpa);
  fputs("\n\n       /*    nao terminal a esquerda */\n",arq_tpa);
  imprime_nterm_esq(arq_tpa);
  fputs("\n\n       /*    Terminais que possuem atributos */",arq_tpa);
  fputs("\n                      /*  TRUE  1 ; FALSE 0 ->"
	"  Valores Booleanos */\n",arq_tpa);
  imprime_term_atrib(arq_tpa);
  fputs("\n\n       /*    Numero de filhos nao terminais de cada producao */\n",arq_tpa);
  imprime_nt_filhos(arq_tpa);
  fputs("\n\n       /*    Numero de producoes associadas a cada nao terminal */\n",arq_tpa);
  imprime_producoes_associadas(arq_tpa);

  // Fecha arquivo
  if (fclose(arq_tpa)==EOF) Mensagem::falha(FECHAMENTO_ARQ,GeradorArquivoFacade::nome_tpa);
}


/* --------------------------------------------------------------------------
 * Descricao : Imprime arquivo de cabecalho para o parser   contendo
 *             definicoes dos tokens terminais e nao-terminais
 * Parametros:
 * Glob.acc  : arq_h
 * Glob.alt  : arq_h
 * Retorno   :
 */

void GeradorArquivoFacade::imprime_header() {
  // Verifica abertura de arquivo
  FILE *arq_t;
  if((arq_t=fopen(GeradorArquivoFacade::nome_tab_h,"wt"))==NULL) Mensagem::falha(CRIACAO_ARQ,GeradorArquivoFacade::nome_tab_h);

  /* ------------ Tokens ------------------------------------------------ */
  fputs("\n//---------------------------------------------------------------------------", arq_t);
  fputs("\n#ifndef TabelaAnalisadorSintaticoH", arq_t);
  fputs("\n#define TabelaAnalisadorSintaticoH", arq_t);
  fputs("\n//---------------------------------------------------------------------------", arq_t);

  fputs("\n\n /* Tokens a serem enviados para o parser gerado pelo FACC */ \n", arq_t);
  int i;
  struct Terminal *t;
  for(i = 1; i < EstruturaDados::listaTerminais.size(); i++)
    fprintf(arq_t,"\n#define   TOK_TERM%03d           %3d  /* %s */", i, i,  EstruturaDados::listaTerminais[i-1]->nome.c_str());
  fprintf(arq_t,"\n#define   TOK_ETX               %3d  /* %s */", EstruturaDados::listaTerminais.size(), EstruturaDados::listaTerminais[i-1]->nome.c_str());

  /* ------------ Valores booleanos ------------------------ */
   fputs("\n\n#define TRUE  1   /* Valores Booleanos */",arq_t);
   fputs("\n\n#define FALSE 0\n",arq_t);

  /* ------------ Tabela de Acao --------------------------- */
   fprintf(arq_t, "\n//------------------------------------------------------------------------------");
   fprintf(arq_t, "\n//Tabela do Analisador Sintático");
   fprintf(arq_t, "\n//------------------------------------------------------------------------------");
   fprintf(arq_t, "\nclass TabelaAnalisadorSintatico {");
   fprintf(arq_t, "\npublic:");
   fprintf(arq_t, "\n// Tabela de Acao");
   fprintf(arq_t, "\nstatic short getAcao(int lin, int col) { ");

  //fprintf(arq_t, "\n static int acao[][%d] = {{ ", indice_term);
   fprintf(arq_t, "\n static int acao[][%d] = {{ ", EstruturaDados::listaTerminais.size());
  imprime_tab_acao(arq_t);
  fputs(" };",arq_t);
  fputs("\n return acao[lin][col];",arq_t);
  fputs("\n}\n",arq_t);


  /* ------------ Tabela de Goto ---------------------------------------- */
  fputs("\nshort static getGoto(int lin, int col) {",arq_t);
  fputs("\n   static short vet_goto[][3] =  {",arq_t);
  //fputs(" \n \nstatic struct st_tab_goto vet_goto[] =  { ",arq_t);
  int numLinhasTabGoto = imprime_tab_goto(arq_t);
  fputs(" }};",arq_t);
  fputs("\nreturn vet_goto[lin][col] ;",arq_t);
  fputs("\n}\n",arq_t);
  fputs("\n// Número de linhas da tabela de goto",arq_t);
  fputs("\nstatic short getNumeroLinhasTabelaGoto() { ",arq_t);
  fprintf(arq_t,"\n return %d;",numLinhasTabGoto);
  fputs("\n} ",arq_t);


  /* ------------ Nro_direita --------------------------- */
  fputs("\n\n\n      /*   numero de simbolos a direita */\n",arq_t);
  fputs("\nstatic int getNroDireita(int pos) { ",arq_t);
  //fprintf(arq_t,"\nstatic int nro_direita [%d] =      {\n",nro_producoes);
  fprintf(arq_t,"\nstatic int nro_direita [%d] =      {\n", EstruturaDados::listaProducoes.size()-1);
  imprime_nro_dir(arq_t);
  fputs(" };",arq_t);
  fputs("\nreturn nro_direita[pos]; ",arq_t);
  fputs("\n} ",arq_t);

  /* ------------ Nterm_esquerda --------------------------- */
  fputs("\n\n\n\n       /*    nao terminal a esquerda */",arq_t);
  fputs("\nstatic int getNTermEsquerda(int pos) { ",arq_t);
  fprintf(arq_t,"\nstatic int nterm_esquerda[%d] =    {\n", EstruturaDados::listaProducoes.size()-1);
  imprime_nterm_esq(arq_t);
  fputs(" };",arq_t);
  fputs("\nreturn nterm_esquerda[pos]; ",arq_t);
  fputs("\n} ",arq_t);

  /* ------------ Term_atrib ------------------------------- */
  fputs("\n\n\n\n       /*    Terminais que possuem atributos */",arq_t);
  fputs("\nstatic int isTermAtrib(int pos) { ",arq_t);
  //fprintf(arq_t,"\nstatic int term_atrib[%d] =    {\n",indice_term);
  fprintf(arq_t,"\nstatic int term_atrib[%d] =    {\n", EstruturaDados::listaTerminais.size());
  imprime_term_atrib(arq_t);
  fputs(" };",arq_t);
  fputs("\nreturn term_atrib[pos]; ",arq_t);
  fputs("\n} ",arq_t);

  /* ------------ nt_filhos  ------------------------------- */
  fputs("\n\n\n\n       /*    Numero de filhos nao terminais de cada producao */",arq_t);
  fputs("\nstatic int getNtFilhos(int pos) { ",arq_t);
  fprintf(arq_t,"\nstatic int nt_filhos[%d] =    {\n", EstruturaDados::listaProducoes.size()-1);
  imprime_nt_filhos(arq_t);
  fputs(" };",arq_t);
  fputs("\nreturn nt_filhos[pos]; ",arq_t);
  fputs("\n} ",arq_t);


  /* ------------ producoes associadas --------------------- */
  fputs("\n\n\n\n       /*    Numero de producoes associadas a cada nao terminal */",arq_t);
  fputs("\nstatic int getProducoesAssociadas(int pos) { ",arq_t);
  //fprintf(arq_t,"\nstatic int producoes_associadas[%d] =    {\n",indice_nterm);
  fprintf(arq_t,"\nstatic int producoes_associadas[%d] =    {\n", EstruturaDados::listaNaoTerminais.size());
  imprime_producoes_associadas(arq_t);
  fputs(" };",arq_t);
  fputs("\nreturn producoes_associadas[pos]; ",arq_t);
  fputs("\n} ",arq_t);
  fputs("\n",arq_t);
  fputs("\n}; \n\n",arq_t);
  fputs("\n//---------------------------------------------------------------------------",arq_t);
  fputs("\n#endif",arq_t);

  // Fecha arquivo
  if (fclose(arq_t)==EOF) Mensagem::falha(FECHAMENTO_ARQ,nome_tab_h);
}



/* --------------------------------------------------------------------------
 * Descricao : Imprime tabela de acao
 * Parametros: saida (E/S) - arquivo no qual deve ser impresso
 * Glob.acc  :
 * Glob.alt  :
 * Retorno   :
 */
void GeradorArquivoFacade::imprime_tab_acao(FILE *saida) {
  struct EstadoLR1 *est;
  //struct st_acao *act;
  int i, quebra;
  int imp= 1;
  bool prim = true;

  fputs("              /* Tabela de Acao */\n",saida);

  if ( (EstruturaDados::listaTerminais.size() < 12) && (EstruturaDados::listaEstados.size() < 60 ) )
	  quebra = EstruturaDados::listaTerminais.size();
  else
    quebra = 12;

  int contador = 0 ;
  int posAcao = 0;
  for (int w=0; w < EstruturaDados::listaEstados.size(); w++)  {
	est = EstruturaDados::listaEstados[w];
	for (posAcao = 0, i = 1; (posAcao < est->acao.size()) && (i <= EstruturaDados::listaTerminais.size()); i++) {
	  struct Acao act = est->acao[posAcao] ;
      if (!prim) {
    	  const char* virgula = (contador % EstruturaDados::listaTerminais.size()) == 0 ? ",{" : "," ;
    	  const char* novalin = (contador % EstruturaDados::listaTerminais.size()) == 0 ? ",{\n" : ",\n" ;
    	 fputs( (imp++ % quebra) ? virgula : novalin,saida);
      }

      if (prim) {
     	  prim = false;                                       /* Nao faz nada */
      }

      if ( act.codigoTerminal != i) {          /* Preenche terminal sem acao com 0 */
	     fputs( "    0",saida);
	     contador++;
      } else {
	     fprintf(saida," %4d", act.acao);
	     //act = act->prox;
	     posAcao++;
	     contador++;
      }

      if((contador % EstruturaDados::listaTerminais.size()) == 0) {
    	  fprintf(saida,"}");
      }
    }

	for ( ; i <= EstruturaDados::listaTerminais.size() ; i++) {  /* Completa zeros no final do estado */
      fputs( (imp++ % quebra) ?",    0" : ",\n    0",saida);
      contador++ ;
      if((contador % EstruturaDados::listaTerminais.size()) == 0) {
         fprintf(saida,"}");
      }
    }
  }
}

/* --------------------------------------------------------------------------
 * Descricao : Imprime tabela de goto
 * Parametros: saida (E/S) - arquivo no qual deve ser impresso
 * Glob.acc  :
 * Glob.alt  :
 * Retorno   :
 */
int GeradorArquivoFacade::imprime_tab_goto(FILE *saida) {
  struct EstadoLR1 *est;
  //struct st_goto *got;
  int i;
  bool prim=true;

  fputs("\n/*---------------------------------------------------*/  ", saida);
  fputs("\n/*     TABELA DE GOTO                                */  ", saida);
  fputs("\n/*---------------------------------------------------*/  ", saida);
  fputs("\n/* EST   NAO_TERM.                         NOVO EST. */  ", saida);
  fputs("\n/*---------------------------------------------------*/", saida);

  int numLinhasTabela = 0;
  for (i = 0; i < EstruturaDados::listaEstados.size(); i++)  {
	est = EstruturaDados::listaEstados[i];

    //for (got = est->gotox; got ; got=got->prox ) {
	for(int j=0; j < est->gotox.size(); j++) {
		struct Goto got = est->gotox[j];
      if (prim) {
	      fprintf(saida, "\n{  %3d, %2d  /* %-30s */,  %2d", i, got.codigoNaoTerminal,
	       //(nterm + got->cod_nterm - 1)->nome, got->estado);
	    	EstruturaDados::listaNaoTerminais[got.codigoNaoTerminal - 1]->nome.c_str(), got.estado);
	      prim = false;
      }  else {
	     fprintf(saida, "},\n{  %3d, %2d  /* %-30s */,  %2d", i,got.codigoNaoTerminal,
	    	EstruturaDados::listaNaoTerminais[got.codigoNaoTerminal - 1]->nome.c_str(), got.estado);
      }
      numLinhasTabela++;
    }

  }

  return numLinhasTabela ;
}

/* --------------------------------------------------------------------------
 * Descricao : Imprime tabela de numero nao terminal simbolo a esquerda
 * Parametros: saida (E/S) - arquivo no qual deve ser impresso
 * Glob.acc  :
 * Glob.alt  :
 * Retorno   :
 */
 void GeradorArquivoFacade::imprime_nterm_esq(FILE *saida) {
  struct Producao *pr;
  int imp, i;

  for (i = 0, imp = 1; i < EstruturaDados::listaProducoes.size()-2; i++) {
	  pr = EstruturaDados::listaProducoes[i+1];
	  fprintf(saida, (imp++ % 15) ?" %2d," : " %2d,\n",pr->codigoLadoEsquerdo);
  }
  fprintf(saida, (imp++ % 15) ?" %2d" : " %2d\n", pr->codigoLadoEsquerdo);
}

/* --------------------------------------------------------------------------
 * Descricao : Imprime tabela de numero de simbolos a direita
 * Parametros: saida (E/S) - arquivo no qual deve ser impresso
 * Glob.acc  :
 * Glob.alt  :
 * Retorno   :
 */
void GeradorArquivoFacade::imprime_nro_dir(FILE *saida) {
	struct Producao *pr;
	int imp, i, t;

	//for (pr = producao + 1 , i = 0, imp = 1; i < (nro_producoes-1); pr++, i++) {
	for (i = 0, imp = 1; i < EstruturaDados::listaProducoes.size()-2; i++) {
		pr = EstruturaDados::listaProducoes[i+1];

		//if (pr->dir->tipo == VAZIO)
		if (pr->listaSimbolosLadoDireito[0]->tipo == VAZIO)
			t = 0;
		else t = pr->listaSimbolosLadoDireito.size();
		fprintf(saida, (imp++ % 15) ?" %2d," : " %2d,\n",t);
	}
	pr = EstruturaDados::listaProducoes[i+1];
	//if (pr->dir->tipo == VAZIO)
	if (pr->listaSimbolosLadoDireito[0]->tipo == VAZIO)
		t = 0;
	else t = pr->listaSimbolosLadoDireito.size();
	fprintf(saida, (imp++ % 15) ?" %2d" : " %2d\n", t);
}

/* --------------------------------------------------------------------------
 * Descricao : Imprime a tabela de terminais indicando te atributos ou nao
 * Parametros: saida (E/S) - arquivo no qual deve ser impresso
 * Glob.acc  :
 * Glob.alt  :
 * Retorno   :
 */
void GeradorArquivoFacade::imprime_term_atrib(FILE *saida) {
  int imp, i;
  char atrib[6];

  for (i = 0, imp = 1; i < EstruturaDados::listaTerminais.size()-1; i++)  {
    if (EstruturaDados::listaTerminais[i]->possuiAtributo == true)
       strcpy(atrib," TRUE");
    else
       strcpy(atrib,"FALSE");
    fprintf(saida, (imp++ % 10) ?" %s," : " %s,\n",atrib);
  }
  if (EstruturaDados::listaTerminais[i-1]->possuiAtributo == true)
     strcpy(atrib," TRUE");
  else
     strcpy(atrib,"FALSE");
  fprintf(saida, (imp++ % 10) ?" %s" : " %s\n", atrib);
}

/* --------------------------------------------------------------------------
 * Descricao : Imprime a tabela de nterm filhos de cada producao
 * Parametros: saida (E/S) - arquivo no qual deve ser impresso
 * Glob.acc  :
 * Glob.alt  :
 * Retorno   :
 */
void GeradorArquivoFacade::imprime_nt_filhos(FILE *saida) {
  int i, imp;
  for (i = 0, imp=1 ; i < EstruturaDados::listaProducoes.size()-2; i++) {
     //fprintf(saida, (imp++ % 15) ?" %2d," : " %2d,\n", EstruturaDados::filhos_nt[i]);
	 fprintf(saida, (imp++ % 15) ?" %2d," : " %2d,\n", EstruturaDados::listaProducoes[i+1]->numNaoTerminaisLadoDireito);
  }
  //fprintf(saida, (imp++ % 15) ?" %2d" : " %2d,\n", EstruturaDados::filhos_nt[i]);
  fprintf(saida, (imp++ % 15) ?" %2d" : " %2d,\n", EstruturaDados::listaProducoes[i+1]->numNaoTerminaisLadoDireito);
}

/* --------------------------------------------------------------------------
 * Descricao : Imprime a tabela das prod. associadas a um nterm
 * Parametros: saida (E/S) - arquivo no qual deve ser impresso
 * Glob.acc  :
 * Glob.alt  :
 * Retorno   :
 */
void GeradorArquivoFacade::imprime_producoes_associadas(FILE *saida) {
  int i, imp;
  //for (i = 1, imp=1 ; i < EstruturaDados::nt_asso.size()-1 ; i++ ) {
  for (i = 0, imp=1 ; i < EstruturaDados::listaNaoTerminais.size()-1 ; i++ ) {
    //fprintf(saida, (imp++ % 15) ?" %2d," : " %2d,\n", EstruturaDados::nt_asso[i]->listaProd.size());
	  fprintf(saida, (imp++ % 15) ?" %2d," : " %2d,\n", EstruturaDados::listaNaoTerminais[i]->listaProducaoAssociada.size());
  }
  //fprintf(saida, (imp++ % 15) ?" %2d" : " %2d,\n", EstruturaDados::nt_asso[i]->listaProd.size());
  fprintf(saida, (imp++ % 15) ?" %2d" : " %2d,\n", EstruturaDados::listaNaoTerminais[i]->listaProducaoAssociada.size());
}



/* --------------------------------------------------------------------------
 * Descricao : Imprime as producoes de forma expandida e numeradas
 * Parametros: saida (E) - arquivo a ser impresso
 * Glob.acc  :
 * Glob.alt  :
 * Retorno   :
 */
void GeradorArquivoFacade::imprime_producoes() {
	// Verifica se o arquivo foi aberto
	FILE *arq_prd;
	if( (arq_prd=fopen(GeradorArquivoFacade::nome_prd,"wt") ) == NULL) Mensagem::falha(CRIACAO_ARQ,GeradorArquivoFacade::nome_prd);

	fputs("\n/* Conjunto expandido e numerado das producoes */\n",arq_prd);
	for (int i = 0; i < EstruturaDados::listaProducoes.size()-1; i++) {
		struct Producao *p = EstruturaDados::listaProducoes[i+1];

		fprintf(arq_prd,"\n/* %2d */   ",i+1);
		fprintf(arq_prd,"%s ->", EstruturaDados::listaNaoTerminais[p->codigoLadoEsquerdo -1]->nome.c_str() );
		for(int j=0; j < p->listaSimbolosLadoDireito.size(); j++) {
			struct SimboloLadoDireitoProducao *q = p->listaSimbolosLadoDireito[j];
			switch(q->tipo) {
			case NAO_TERMINAL:
				fprintf(arq_prd," %s", EstruturaDados::listaNaoTerminais[q->codigo -1]->nome.c_str() );
				break;
			case TERMINAL:
				fprintf(arq_prd," '%s'", EstruturaDados::listaTerminais[q->codigo -1]->nome.c_str() );
				break;
			case VAZIO:
				fputs(" #",arq_prd);
				break;
			default : Mensagem::falha(ERRO_INVALIDO,NULL); break;

			}
		}
		fputs(" ;",arq_prd);
	}

	// Fecha arquivo
	if (fclose(arq_prd)==EOF) Mensagem::falha(FECHAMENTO_ARQ,GeradorArquivoFacade::nome_prd);

}

/* --------------------------------------------------------------------------
 * Descricao : Imprime todos os conjuntos de itens e libera suas areas.
 * Parametros:
 * Glob.acc  : arq_est
 * Glob.alt  : arq_est
 * Retorno   :
 */
void GeradorArquivoFacade::imprime_itens() {
	fputs(" \n\n Conjunto de Estados criados durante a geracao do Parser\n",
			EstruturaDados::arq_est);

	// Percorre a lista de estados
	for (int n = 0; n < EstruturaDados::listaEstados.size(); n++)  {
		// Escreve o estado, que corresponde ao numero da producao
		struct EstadoLR1 *estado = EstruturaDados::listaEstados[n];
		fprintf(EstruturaDados::arq_est, "\nEstado(%d)", n);
		if (estado->conflito)
			fputs("                                       *** CONFLITO ***", EstruturaDados::arq_est);

		// Percorre os itens caninicos do estado
		struct ItemCanonico  item;
		for (int i = 0; i < estado->itens.size(); i++) {
			item = estado->itens[i];
			// Escreve o numero da producao entre parenteses, o nao terminal correspondente a producao
			// e o simbolo -> (lado esquerdo da producao)
			int numProducao = item.nro_producao ;
			if ( numProducao > 0 ) {
				fprintf(EstruturaDados::arq_est, "\n (%3d) %s ->", item.nro_producao,
						EstruturaDados::listaNaoTerminais[EstruturaDados::listaProducoes[numProducao]->codigoLadoEsquerdo - 1 ]->nome.c_str() );
			} else {
				fputs("\n (  0) ACEITA ->", EstruturaDados::arq_est);
			}

			// Escreve agora o lado direito da producao
			for (int j=0; j < EstruturaDados::listaProducoes[numProducao]->listaSimbolosLadoDireito.size(); j++) {
				// Recupera o simbolo na posicao j do lado direito da producao
				struct SimboloLadoDireitoProducao *dir = EstruturaDados::listaProducoes[numProducao]->listaSimbolosLadoDireito[j];

				// Se a posicao do ponto, informada no item canonico, é igual a j, coloca o ponto aqui
				if (j == item.posic_ponto) {
					fputc('.', EstruturaDados::arq_est);
				}

				// Escreve o simbolo, se for um terminal
				if (dir->tipo == TERMINAL) {
					fprintf(EstruturaDados::arq_est, " '%s'", EstruturaDados::listaTerminais[dir->codigo - 1]->nome.c_str());

				// Escreve o simbolo, se for um nao terminal
				} else if (dir->tipo == NAO_TERMINAL) {
						fprintf(EstruturaDados::arq_est, " %s", EstruturaDados::listaNaoTerminais[dir->codigo - 1]->nome.c_str());
				}
			}

			// Se o ponto do item aparece apos o ultimo simbolo do lado direito, insere o ponto agora
			if ( item.posic_ponto == EstruturaDados::listaProducoes[numProducao]->listaSimbolosLadoDireito.size()) {
				fputc('.', EstruturaDados::arq_est);
			}

			// Escreve agora todos os lookaheads (tokens) informados no item
			fputs("  { ", EstruturaDados::arq_est);
			for (std::set<int>::iterator k=item.look_ahead.begin(); k!=item.look_ahead.end(); ++k)
				fprintf(EstruturaDados::arq_est, "%s ", EstruturaDados::listaTerminais[*k - 1]->nome.c_str());
			fputc('}', EstruturaDados::arq_est);
		}
	}

}

void GeradorArquivoFacade::imprime_first_itens() {
	// Verifica se o arquivo foi aberto
	if( (EstruturaDados::arq_est=fopen(GeradorArquivoFacade::nome_est,"wt") ) == NULL) Mensagem::falha(CRIACAO_ARQ,GeradorArquivoFacade::nome_est);

	imprime_first();
	imprime_itens();

    // Fecha o arquivo
	if (fclose(EstruturaDados::arq_est)==EOF) Mensagem::falha(FECHAMENTO_ARQ,GeradorArquivoFacade::nome_est);
}

/* --------------------------------------------------------------------------
 * Descricao : Imprime  o conjunto de first gerado.
 * Parametros: saida (E/S) - arquivo de saida (arq_fir ou arq_est)
 * Glob.acc  :
 * Glob.alt  :
 * Retorno   :
 */
void GeradorArquivoFacade::imprime_first() {
	fputs(" \n\n Conjunto de first's para os nao terminais da gramatica \n", EstruturaDados::arq_est);
	for (int i=0; i< EstruturaDados::listaNaoTerminais.size(); i++) {
		struct EntradaTabelaSimbolos *naoTerminal= EstruturaDados::listaNaoTerminais[i];
		fprintf(EstruturaDados::arq_est, "\n first(%s)",naoTerminal->nome.c_str());

		for (std::set<int>::iterator j=naoTerminal->first.begin(); j!=naoTerminal->first.end(); ++j) {
			if ( *j==0 ) {
				fputs("  VAZIO", EstruturaDados::arq_est);
			} else {
				fprintf(EstruturaDados::arq_est, " %s", EstruturaDados::listaTerminais[*j - 1]->nome.c_str());
			}
		}
	}
}

