/* -----------------------------------------------------------------------*
 *                                                                        *
 * Programa    : FACC.EXE                                Versao 2.0       *
 *                                                                        *
 * Modulo      : FACCADM.C                                                *
 *                                                                        *
 * Autores     : Edward Lucio Vieira Borba  			          *
 *                                                                        *
 * Data inicio : 24/03/91                                                 *
 * Data Atual. : 08/10/91                                                 *
 *                                                                        *
 * Descricao   : Administrador do FACC. Gerencia a integracao entre os    *
 *               modulos do projeto.                                      *
 *                                                                        *
 * -----------------------------------------------------------------------*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <list>
#include <set>
#include <stack>
#include <string>
#include <utility>

#include "geracaoArquivo/GeradorArquivoFacade.h"
#include "rotinas/EstruturaDados.h"
#include "rotinas/Gramatica.h"
#include "rotinas/ProcessaEstados.h"
#include "semantico/AnalisadorSemantico.h"
#include "sintatico/AnalisadorSintatico.h"
#include "util/Constantes.h"
#include "util/Mensagem.h"
#include "util/SetUtil.h"

/* --------------- Parametros de impressao do FACC ------------------------- */

#define P_FONTE                 1                     /* Impressao do fonte  */
#define P_TABELA                2                     /* Impressao da tabela */
#define P_PRODUCOES             3                     /* Impressao do first  */
#define P_PROCSEM               4                     /* Impressao do Procsem.  */

#define SAIDA_NORMAL	 0				/* Final do programa */

std::string listaArquivosImpressos ;

/* --------------------------------------------------------------------------*
 * Descricao : Tela de Apresentacao do software.                             *
 * Entrada   : Nenhuma.                                                      *
 * Retorno   : Nenhum.                                                       *
 *-------------------------------------------------------------------------- */
 void apresentacao() {
	fputs(
	"\n FACC 3.0 - Ferramenta de Apoio a Construcao de Compiladores "
	"\n Um gerador de Parser LR e Processador Semantico para um esquema de compilacao em 2 passos."
	"\n       Por   Edward Lucio Vieira Borba"
	"\n             Luiz Reginaldo Almeida Fleury Curado"
	"\n Uso:                                               "
	"\n    FACC [caminho] gramatica[.GRA] [opcoes]       "
	"\n       caminho = localizacao do arquivo c/ a gramatica"
	"\n       gramatica[.GRA] = arquivo com a gramatica "
	"\n       opcoes:                                           "
	"\n            -p = Analisador Sintatico (Parser) "
	"\n            -s = Analisador Semantico"
	"\n            -t = Tabelas do Analisador Sintatico "
	"\n            -i = Informacoes para geracao do Analisador Sintatico"
	"\n            -g = Gramatica expandida numerada "
	"\n            -v = Apenas verifica consistencia da gramatica (default)\n"
	"\n   Obs: ATENCAO!. o nome da gramatica so deve ter ate 5 caracteres: XXXXX[.GRA]\n\n"
			,stderr);

  //system("del faccerro");
  exit(EXIT_FAILURE);
}

 /* --------------------------------------------------------------------------*
  * Descricao : Imprime os arquivos solicitados pelo usuario.                 *
  * Entrada   : Nenhuma.                                                      *
  * Retorno   : Nenhum.                                                       *
  *-------------------------------------------------------------------------- */
 void imprime_arquivos(std::set<int> parametrosInformados, AnalisadorSintatico *sintatico, AnalisadorSemantico *semantico) {
	 if(SetUtil::contem(parametrosInformados, P_PROCSEM)) {
		 GeradorArquivoFacade::gerarArquivoSemantico(sintatico, semantico);
		 GeradorArquivoFacade::geraArquivoCompilador() ;
	 }

	 if(SetUtil::contem(parametrosInformados, P_FONTE)) {
		 GeradorArquivoFacade::imprime_header();
		 GeradorArquivoFacade::geraArquivoAnalisadorLexico();
		 GeradorArquivoFacade::geraArquivoAnalisadorSintatico();
	 }

	 if(SetUtil::contem(parametrosInformados, P_TABELA)) {
		 GeradorArquivoFacade::imprime_tabelas_do_parser();
	 }

	 if(SetUtil::contem(parametrosInformados, P_PRODUCOES)) {
		 GeradorArquivoFacade::imprime_producoes();
	 }

	 if(SetUtil::contem(parametrosInformados, P_ESTADOS)) {
		 GeradorArquivoFacade::imprime_first_itens();
	 }
 }


/* --------------------------------------------------------------------------*
 * Descricao : Monta os nomes dos arquivos a serem gerados e o conjunto de   *
 *             parametros de impressao.                                      *
 * Entrada   : num_parm: numero de parametros do programa                    *
 *             val_parm: valor dos parametros do programa                    *
 * Retorno   : Nenhum.                                                       *
 *-------------------------------------------------------------------------- */
std::set<int> recuperaParametrosLinhaComando(int num_parm, char **argv) {
  int parametro;
//  int tam_nome_fonte;
  char *p,*q;

  listaArquivosImpressos = "Arquivo(s) gerado(s): \n" ;

  strlwr(GeradorArquivoFacade::nome_fonte); /* transforma nome fonte p/ minusculo */

  if(strrchr(GeradorArquivoFacade::nome_fonte,'.')==NULL)    /* Incorpora extensao no nome do arq. */
    strcat(GeradorArquivoFacade::nome_fonte,".gra");
  
  if ((strrchr(GeradorArquivoFacade::nome_fonte,':') != NULL) || /* Verifica se existe caminho */
      (strrchr(GeradorArquivoFacade::nome_fonte,'\\') != NULL) )
  {
      p = strrchr(GeradorArquivoFacade::nome_fonte,'.');
      while ((*p != '\\') && (*p != ':') )
	 p--;
      p++; /* posiciona no inicio do nome */
  } else {
     p = GeradorArquivoFacade::nome_fonte;
  }

  // Abre o arquivo contendo a gramatica
  if((EstruturaDados::arq_fonte=fopen(GeradorArquivoFacade::nome_fonte,"rt"))==NULL) {
    Mensagem::falha(ABERTURA_ARQ, GeradorArquivoFacade::nome_fonte);
  }

  // Recupera os parametros informados na linha de comando
  std::set<int> parametrosInformados ;
  for(int i = 2 ; i < num_parm; i++) {
      if (toupper(argv[i][0] != '-') ) {
    	  apresentacao(); /* Parametros invalidos */
      }

      // Verifica o parametro informado
      parametro = toupper(argv[i][1]);
      switch(parametro) {
      	  // -p = Analisador Sintatico (Parser)
      	  case 'P' :
			parametrosInformados.insert(P_FONTE) ;

			// Adiciona nome dos arquivos na lista de arquivos gerados
			listaArquivosImpressos += GeradorArquivoFacade::nome_lex_c ;
			listaArquivosImpressos += "\n";
			listaArquivosImpressos += GeradorArquivoFacade::nome_lex_h ;
			listaArquivosImpressos += "\n";
			listaArquivosImpressos += GeradorArquivoFacade::nome_sin_c ;
			listaArquivosImpressos += "\n";
			listaArquivosImpressos += GeradorArquivoFacade::nome_sin_h ;
			listaArquivosImpressos += "\n";
			listaArquivosImpressos += GeradorArquivoFacade::nome_tab_h ;
			listaArquivosImpressos += "\n";
		   break;

		  // -t = Tabelas do Analisador Sintatico
      	  case 'T' :
			parametrosInformados.insert(P_TABELA) ;

			strcpy(GeradorArquivoFacade::nome_tpa, GeradorArquivoFacade::nome_fonte);
			strcpy(strrchr(GeradorArquivoFacade::nome_tpa,'.'),".tab");
			//nome_tpa = nome_fonte ; nome_tpa += ".tab" ;
			listaArquivosImpressos += GeradorArquivoFacade::nome_tpa ;
			listaArquivosImpressos += "\n";
			break;

	  	  // -i = Informacoes para geracao do Analisador Sintatico"
	      case 'I' :
	      	parametrosInformados.insert(P_ESTADOS) ;
	      	strcpy(GeradorArquivoFacade::nome_est, GeradorArquivoFacade::nome_fonte);
	      	strcpy(strrchr(GeradorArquivoFacade::nome_est,'.'),".ger");
	      	//nome_est = nome_fonte ; nome_est += ".ger" ;
	      	listaArquivosImpressos += GeradorArquivoFacade::nome_est ;
	      	listaArquivosImpressos += "\n";
	      	break;

      	  // -s = Analisador Semantico"
      	  case 'S' :
  			parametrosInformados.insert(P_PROCSEM) ;

  			// Adiciona nome dos arquivos na lista de arquivos gerados
  			listaArquivosImpressos += GeradorArquivoFacade::nome_sem_h ;
  			listaArquivosImpressos += "\n";
  			listaArquivosImpressos += GeradorArquivoFacade::nome_sem_c ;
  			listaArquivosImpressos += "\n";
  			listaArquivosImpressos += GeradorArquivoFacade::nome_cmp_c ;
  			listaArquivosImpressos += "\n";
  			break;

  		  // -g = Gramatica expandida numerada "
          case 'G' :
  			parametrosInformados.insert(P_PRODUCOES) ;

        	strcpy(GeradorArquivoFacade::nome_prd, GeradorArquivoFacade::nome_fonte);
        	strcpy(strrchr(GeradorArquivoFacade::nome_prd,'.'),".prd");
  			//nome_prd = nome_fonte ; nome_prd += ".prd" ;
        	listaArquivosImpressos += GeradorArquivoFacade::nome_prd ;
        	listaArquivosImpressos += "\n";
        	break;

      	  case 'V' : break;

      	  default : apresentacao(); break;
      }
    }

  return parametrosInformados ;
}


/* --------------------------------------------------------------------------*
 * Descricao :Controle Geral do Fluxo do Programa.                           *
 * Retorno   :Nenhum.                                                        *
 *-------------------------------------------------------------------------- */

int main(int argc, char *argv[]) {
  // Recupera o nome do programa fonte
  if (argc>1) {
    strcpy(GeradorArquivoFacade::nome_fonte,argv[1]);
  } else {
    apresentacao();
  }

  // Recupera os parametros que foram informados na linha de comando
  std::set<int> parametrosLinhaComando = recuperaParametrosLinhaComando(argc,argv);

  // Realiza a analise sintatica da gramatica
  fprintf(stderr,"\nCompilando %s. A g u a r d e . . . ", GeradorArquivoFacade::nome_fonte);
  AnalisadorSintatico sintatico ;
  sintatico.execute();

  if (EstruturaDados::houve_erro) {
	  EstruturaDados::liberaListasIniciais();
  } else {
    fputs("\nSintaxe da Gramatica OK !",stderr);
    // Verifica se a gramatica está de acordo com o esperado
    Gramatica::critica_gramatica();

    // Realiza a analise semantica da gramatica
    AnalisadorSemantico semantico ;
    semantico.execute(false, sintatico.getPilhaProducoesAnaliseSintatica(), sintatico.getListaAtributosAnaliseLexica());

    if (!EstruturaDados::houve_erro) {
      fputs("\nSemantica da Gramatica OK !",stderr);
       fputs("\nFim da Compilacao !",stderr);

       // Gera estados para a gramatica LALR1 informada no codigo fonte
       fputs("\n\nVerificando ambiguidades. Aguarde . . .",stderr);
       Gramatica::calcula_first();
       ProcessadorEstados::geraEstadosLR1(parametrosLinhaComando);

       // Faz a geracao de arquivos do facc
       if (parametrosLinhaComando.size() > 0) {
		 fprintf(stderr,"\n%s", listaArquivosImpressos.c_str());
		 imprime_arquivos(parametrosLinhaComando, &sintatico, &semantico);
		 fputs("\nArquivos  gerados !",stderr);
       } else {
    	   fputs("\nAnalise da gramatica concluida !",stderr);
       }
       EstruturaDados::liberaListaEstados();
    }
  }

  // Fecha o arquivo da gramatica
  if (fclose(EstruturaDados::arq_fonte)==EOF) {
     Mensagem::falha(FECHAMENTO_ARQ, GeradorArquivoFacade::nome_fonte);
  }

  exit(SAIDA_NORMAL);
}
