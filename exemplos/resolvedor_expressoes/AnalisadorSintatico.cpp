
/* -----------------------------------------------------------------------*
 * Analisador sintatico gerado pelo FACC 2.02                             *
 *                                                                        *
 *  Software desenvolvido na Universidade de Brasilia - UnB, 1990         *
 *  e aprimorado em 1991.                                                 *
 *                                                                        *
 *  Autores  : Edward Lucio Vieira Borba                                  *
 *             Claudio Vasconcelos Dobbin                                 *
 *             Luiz Reginaldo Almeida Fleury Curado                       *
 *                                                                        *
 *       Este software nao deve ser usado para fins comerciais sem acordo *
 *  previo com os autores.                                                *
 * -----------------------------------------------------------------------*
 *  A fun็ใo anasin faz a anแlise sintแtica. O analisador sintแtico usa o
 *  analisador l้xico, chamando o m้todo proxToken() para recuperar o pr๓ximo
 *  token do c๓digo fonte.
 * -----------------------------------------------------------------------*/

#include "TabelaAnalisadorSintatico.h" // Tabela do parser gerado pelo FACC
#include "AnalisadorSintatico.h"
#include <stdlib.h>

//------------------------------------------------------------------------------
// A็๕es assumidas pelo analisador sintแtico
//------------------------------------------------------------------------------
#define ACAO_EMPILHAR(x)(x>2000)
#define ACAO_REDUZIR(x) ((x>1000)&&(x<2000))
#define ACAO_ACEITAR    1000
#define ACAO_ERRO(x)    (x<1000)
#define ACAO_FALHA(x)   (x==-1)

//------------------------------------------------------------------------------
/* M้todo que efetua a Anแlise Sintแtica do tipo Empilhar-Reduzir  */
//------------------------------------------------------------------------------
void AnalisadorSintatico::executa(std::string codigoFonte) { // retorna 0 se ok, 1 se deu erro

  // Inicializa o analisador sintแtico
  limpaVariaveis() ;

  // Inicio da anแlise sintแtica de empilhar e reduzir - empilha o estado zero
  std::stack<int> pilhaEstados ;
  pilhaEstados.push(0);

  // Inicializa o analisador lexico, recupera o primeiro token do codigo fonte
  AnalisadorLexico lex(codigoFonte) ;
  Token tokenCorrente = lex.proxToken() ;

  // Se foi gerado um erro lexico, guarda a mensagem de erro
  if ( lex.encontrouErro() ) {
      msgErro = lex.getMsgErro();
      return ;
  }

  // Se nใo foi gerado um erro l้xico, guarda o atributo do token, se ele tiver
  if(TabelaAnalisadorSintatico::isTermAtrib(tokenCorrente.getValor() -1) == TRUE) {
      listaAtributos.push_back( tokenCorrente.getAtributo() );
  }

  // Loop: Consulta a tabela sintแtica, tomando o estado no topo da pilha de estados e o token corrente.
  int acaoAnalisador;
  while (( (acaoAnalisador = TabelaAnalisadorSintatico::getAcao(pilhaEstados.top(),tokenCorrente.getValor() - 1)) != ACAO_ACEITAR )) {

     //จจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจ
     // 1) Se a a็ใo for falhar...จ
     //จจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจ
	 if( ACAO_FALHA(acaoAnalisador) ) {
	     msgErro = "Erro na anแlise sintแtica: Tabela de a็ใo inconsistente" ;
         break;
	 }

	 //จจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจ
	 // 2) Se a a็ใo for erro...
	 //จจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจ
	 else if( ACAO_ERRO(acaoAnalisador) ) {
	    trata_erro(acaoAnalisador, pilhaEstados.top(), tokenCorrente);
        break;

     //จจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจ
     // 3) Se a a็ใo for reduzir por alguma producao...
     //จจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจ
	 } else if( ACAO_REDUZIR(acaoAnalisador) ) {
	    int producao = acaoAnalisador - 1000;

	    // Remove |B| estados da pilha de estados, onde |B| ้ o tamanho do lado direito da producao (handle)
	    for( int i=0; i < TabelaAnalisadorSintatico::getNroDireita(producao -1); i++) {
	    	pilhaEstados.pop();
	    }

	    // Seja es o estado ao topo da pilha, A o nใo-terminal เ esquerda na produ็ใo sendo analisada
	    // Usa es e A para determinar e empilhar o estado correspondente a desvio( A, es )
	    int nao_terminal = TabelaAnalisadorSintatico::getNTermEsquerda(producao-1);
	    pilhaEstados.push( determina_desvio(pilhaEstados.top(), nao_terminal) );

	    // Guarda a producao na pilha de producoes
	    pilhaProducoesBottomUp.push(producao);

	 //จจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจ
	 // 4) Se a a็ใo for empilhar....
	 //จจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจจ
	 } else {
	     pilhaEstados.push(acaoAnalisador - 2000);

         // Pede um novo token ao analisador l้xico
         tokenCorrente = lex.proxToken();

         // Se foi gerado um erro lexico, guarda a mensagem de erro
         if ( lex.encontrouErro() ) {
             msgErro = lex.getMsgErro();
             break;
         }

  	 // Se nใo foi gerado um erro l้xico, guarda o atributo do token, se ele tiver
	 if(TabelaAnalisadorSintatico::isTermAtrib(tokenCorrente.getValor()-1) == TRUE)  {
 	        listaAtributos.push_back( tokenCorrente.getAtributo() );
     }
	}
  }

  // Se anแlise sintแtica ocorreu com sucesso, prepara a pilha de producoes para
  // que seja utilizada pelo analisador semโntico
  if( acaoAnalisador == ACAO_ACEITAR && pilhaProducoesBottomUp.size() > 0 ) {
      int topo = pilhaProducoesBottomUp.top();
      pilhaProducoesBottomUp.pop();
      transforma_producoes_analisador_semantico( topo );
  }
 }


 //----------------------------------------------------------------------------------
 // M้todo que, dado um estado e um nใo terminal, retorna o estado correspondente
 // na entrada da tabela sintแtica desvio(estado,nใo-terminal)
 // A tabela ้ representada pelo vetor vet_goto
//----------------------------------------------------------------------------------
 int AnalisadorSintatico::determina_desvio(int &estado,int &nao_terminal) {
 	int numElementosVetor = TabelaAnalisadorSintatico::getNumeroLinhasTabelaGoto() ;
 	for( int i=0; i< numElementosVetor; i++ ) {
 		if( (TabelaAnalisadorSintatico::getGoto(i,0) == estado) && (TabelaAnalisadorSintatico::getGoto(i,1) == nao_terminal) ) {
 		   return TabelaAnalisadorSintatico::getGoto(i,2) ;
 		}
 	}
 	return -1 ;
 }

 //------------------------------------------------------------------------------
 // Transforma a pilha de producoes que estแ no formato bottom-up para top-down
 //------------------------------------------------------------------------------
 void AnalisadorSintatico::transforma_producoes_analisador_semantico(int topo) {

   for (int i=1; i <= TabelaAnalisadorSintatico::getNtFilhos(topo -1); i++) {
      int topo = pilhaProducoesBottomUp.top();
      pilhaProducoesBottomUp.pop();
      transforma_producoes_analisador_semantico( topo );
   }

   int nao_terminal = TabelaAnalisadorSintatico::getNTermEsquerda(topo-1);

   // So empilha reducao de nao terminal com mais de uma producao associada
   if(TabelaAnalisadorSintatico::getProducoesAssociadas(nao_terminal - 1) > 1) {
       pilhaProducoesTopDown.push(topo);
   }
 }

 //------------------------------------------------------------------------------
 // M้todo utilitแrios, para limpar as estruras de dados
 //------------------------------------------------------------------------------
 void AnalisadorSintatico::limpaVariaveis() {
	 this->listaAtributos.clear();
	 while(this->pilhaProducoesTopDown.size() > 0) this->pilhaProducoesTopDown.pop() ;
	 while( this->pilhaProducoesBottomUp.size() > 0 ) this->pilhaProducoesBottomUp.pop() ;
	 this->msgErro = "" ;
 }


 //------------------------------------------------------------------------------
 // Funcใo que trata erro
 //------------------------------------------------------------------------------
 void AnalisadorSintatico::trata_erro(int &tipo,int &estado, Token tokenCorrente) {
	char cadeia[30];
	itoa( tokenCorrente.getValor(), cadeia, 10 );
   this->msgErro = "Token nใo esperado: " + std::string(cadeia) ;
 }
