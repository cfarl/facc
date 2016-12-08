/* -----------------------------------------------------------------------*
 *                                                                        *
 * Programa    : FACC.EXE                                Versao 2.0       *
 *                                                                        *
 * Modulo      : FACCSIN.C                                                *
 *                                                                        *
 * Autores     : Edward Lucio Vieira Borba  			          *
 *                                                                        *
 * Data inicio : 26/03/91                                                 *
 * Data Atual. : 14/09/91                                                 *
 *                                                                        *
 * Descricao   : Analisador sintatico do Interpretador de gramaticas.     *
 *                                                                        *
 * -----------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include "../util/Mensagem.h"
#include "../lexico/Token.h"
#include "../util/Constantes.h"
#include "TabelaAnalisadorSintatico.h"   /* Tabelas a serem usadas na analise sintatica */
#include "AnalisadorSintatico.h"         /* Tabelas a serem usadas na analise sintatica */


//------------------------------------------------------------------------------
// A��es assumidas pelo analisador sint�tico
//------------------------------------------------------------------------------
#define ACAO_EMPILHAR(x)  (x>2000)
#define ACAO_REDUZIR(x)   ((x>1000)&&(x<2000))
#define ACAO_ACEITAR      1000
#define ACAO_ERRO(x)      (x<1000)
#define ACAO_FALHA(x)     (x==-1)


//------------------------------------------------------------------------------
/* Controla a analise sintatica do interpretador de gramaticas */
//------------------------------------------------------------------------------
void AnalisadorSintatico::execute() {

  // Inicializa o analisador sint�tico
  limpaVariaveis() ;

  // Inicio da an�lise sint�tica de empilhar e reduzir - empilha o estado zero
  std::stack<int> pilhaEstados ;
  pilhaEstados.push(0);

  // Inicializa o analisador lexico, recupera o primeiro token do codigo fonte
  AnalisadorLexico lexico ;
  int token = lexico.proxToken() ;

  // Se n�o foi gerado um erro l�xico, guarda o atributo do token, se ele tiver
  if(TabelaAnalisadorSintatico::isTermAtrib(token-1) == TRUE) {
	  listaAtributos.push_back(lexico.atributo_terminal);
  }

  // Loop: Consulta a tabela sint�tica, tomando o estado no topo da pilha de estados e o token corrente.
  int acaoAnalisador ;
  while ((acaoAnalisador = TabelaAnalisadorSintatico::getAcao((int)pilhaEstados.top(), token - 1)) != ACAO_ACEITAR) {

	//�������������������������������������������������������������������������
	// 1) Se a a��o for falhar...�
	//�������������������������������������������������������������������������
    if( ACAO_FALHA(acaoAnalisador)) {
    	Mensagem::falha(TABELA_ACAO,NULL);
      break ;

	 //�������������������������������������������������������������������������
	 // 2) Se a a��o for erro...
	 //�������������������������������������������������������������������������
  	} else if( ACAO_ERRO(acaoAnalisador) ) {
  		trata_erro(acaoAnalisador, (int)pilhaEstados.top(), &token, &lexico, &pilhaEstados);

  		while(pilhaEstados.size() != 0) {
  			pilhaEstados.pop();
  		}
  		break ;


  	//�������������������������������������������������������������������������
  	// 3) Se a a��o for reduzir por alguma producao...
  	//�������������������������������������������������������������������������
    } else if( ACAO_REDUZIR(acaoAnalisador) ) {
    	int producao = acaoAnalisador - 1000;

    	// // Remove |B| estados da pilha de estados, onde |B| � o tamanho do lado direito da producao (handle)
    	//desempilha(nro_direita[producao -1], &topo_estado);
    	for( int i=0; i < TabelaAnalisadorSintatico::getNroDireita(producao -1); i++) {
    	   	pilhaEstados.pop();
    	}

    	int nao_terminal = TabelaAnalisadorSintatico::getNTermEsquerda(producao-1);
    	pilhaEstados.push(determina_desvio((int)pilhaEstados.top(), nao_terminal));

    	// Guarda a producao na pilha de producoes
	    pilhaProducoesBottomUp.push(producao);

    //�������������������������������������������������������������������������
    // 4) Se a a��o for empilhar....
    //�������������������������������������������������������������������������
    } else {
    	pilhaEstados.push(acaoAnalisador - 2000);

    	// Pede um novo token ao analisador l�xico
    	token = lexico.proxToken();

    	// Se n�o foi gerado um erro l�xico, guarda o atributo do token, se ele tiver
    	if(TabelaAnalisadorSintatico::isTermAtrib(token-1) == TRUE) {
    		listaAtributos.push_back(lexico.atributo_terminal);
    	}
    }
  } /* fim do while */

  // Retira estado I0 e I1
  pilhaEstados.pop();
  pilhaEstados.pop();
/*
 puts("\n---------- Pilha posfix--------------------------");
  for (pil = topo_reducao_posfix,i=0; pil != NULL; pil=pil->ant,i++)
    printf("\nproducao(%d) [%d]",i,pil->valor);
*/

  // Se an�lise sint�tica ocorreu com sucesso, prepara a pilha de producoes para
  // que seja utilizada pelo analisador sem�ntico
  if( acaoAnalisador == ACAO_ACEITAR && pilhaProducoesBottomUp.size() > 0 ) {
      int topo = pilhaProducoesBottomUp.top();
      pilhaProducoesBottomUp.pop();
      transforma_producoes_analisador_semantico(topo);
  }
}

//----------------------------------------------------------------------------------
// M�todo que, dado um estado e um n�o terminal, retorna o estado correspondente
// na entrada da tabela sint�tica desvio(estado,n�o-terminal)
// A tabela � representada pelo vetor vet_goto
//----------------------------------------------------------------------------------
 const int AnalisadorSintatico::determina_desvio(int estado,int nao_terminal) {
	int numElementosVetor = TabelaAnalisadorSintatico::getNumeroLinhasTabelaGoto() ;
	for( int i=0; i< numElementosVetor; i++ ) {
		if( (TabelaAnalisadorSintatico::getGoto(i,0) == estado) && (TabelaAnalisadorSintatico::getGoto(i,1) == nao_terminal) ) {
		   return TabelaAnalisadorSintatico::getGoto(i,2) ;
		}
	}
	return -1 ;
 }


 //------------------------------------------------------------------------------
 // Transforma a pilha de producoes que est� no formato bottom-up para top-down
 //------------------------------------------------------------------------------
 void AnalisadorSintatico::transforma_producoes_analisador_semantico(int topo) {

	 for (int i=1; i <= TabelaAnalisadorSintatico::getNtFilhos(topo -1); i++) {
		 int topo = pilhaProducoesBottomUp.top();
		 pilhaProducoesBottomUp.pop();
		 transforma_producoes_analisador_semantico(topo);
	 }

	 int nao_terminal = TabelaAnalisadorSintatico::getNTermEsquerda(topo-1);

	 // So empilha reducao de nao terminal com mais de uma producao associada
	 if(TabelaAnalisadorSintatico::getProducoesAssociadas(nao_terminal - 1) > 1) {
		 pilhaProducoesTopDown.push(topo);
	 }

 }


 //------------------------------------------------------------------------------
 // M�todo utilit�rios, para limpar as estruras de dados
 //------------------------------------------------------------------------------
 void AnalisadorSintatico::limpaVariaveis() {
	 this->listaAtributos.clear();
	 while(this->pilhaProducoesTopDown.size() > 0) this->pilhaProducoesTopDown.pop() ;
	 while( this->pilhaProducoesBottomUp.size() > 0 ) this->pilhaProducoesBottomUp.pop() ;
 }


 //------------------------------------------------------------------------------
 // Func�o que trata erro
 //------------------------------------------------------------------------------
void AnalisadorSintatico::trata_erro(int tipo, int estado, int *token, AnalisadorLexico *lexico, std::stack<int> *pilhaEstados) {
  switch(tipo) { /* Mensagem a  ser enviada */
    case 1:  Mensagem::erro(NAO_ESPERADO, *token);
	     *token= lexico->proxToken();
	     if(TabelaAnalisadorSintatico::isTermAtrib(*token-1) == TRUE) {
	    	 listaAtributos.push_back(lexico->atributo_terminal);
	     }
	     break;
    case 2:  Mensagem::erro(NAO_ESPERADO, *token);
	     break;       /* Fim de arquivo nao esp. */

    case 3:  Mensagem::erro(ESPERADO, TOK_NTERM);
    	 pilhaEstados->push(4) ; // Assume nterm
	     break;
    case 4:  Mensagem::erro(ESPERADO, TOK_PONTO_VIRGULA);
    	pilhaEstados->push(5) ; // Assume ponto e virgula
	     break;
    case 5:  Mensagem::erro(ESPERADO, TOK_SETA);
    	pilhaEstados->push(11) ; // Assume ->
	     break;
    case 6:  Mensagem::erro(ESPERADO, TOK_NTERM);
    	 pilhaEstados->push(21) ; // Assume nterm
    	 break;
    case 7:  Mensagem::erro(ESPERADO, TOK_TERM);
	     pilhaEstados->push(22) ; // Empilha term
	     break;
    case 8:  Mensagem::erro(SIMBOLO_ESP, 0);
         pilhaEstados->push(21) ; // Empilha nterm
	     break;
    case 9:  Mensagem::erro(ESPERADO, TOK_TEXTO_COD);
         pilhaEstados->push(12) ; // Assume codigo
	     break;
    default: Mensagem::erro(ERRO_INVALIDO, 0);
	     printf("\n acao->%d, Estado:%d token:%d", tipo, estado,*token-1);
	     exit(0);
  }
}
