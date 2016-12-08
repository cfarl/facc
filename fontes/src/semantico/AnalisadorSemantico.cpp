
// -----------------------------------------------------------------------
//  Imprime o código do Analisador Semântico
// -----------------------------------------------------------------------
#include "../lexico/TabelaHash.h"
#include "../geracaoArquivo/GeradorArquivoFacade.h"
#include "AnalisadorSemantico.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lexico/Token.h"
#include "../util/Mensagem.h"
#include "../util/Constantes.h"
#include "../rotinas/EstruturaDados.h"

#define NL 0xA

//-----------------------------------------------
// Construtor da classe
//-----------------------------------------------
void AnalisadorSemantico::execute(bool geraArquivo, std::stack<int> pilhaProducoes, std::list<int> listaAtributos) {
	this->pilhaProducoesPrograma = pilhaProducoes ;
	this->listaAtributosPrograma = listaAtributos ;
	this->geraArquivo = geraArquivo ;
	this->GL_prod_ativa = 0;

	// Verifica se os arquivos estao abertos
	if(geraArquivo) {
		if((arq_sem_h=fopen(GeradorArquivoFacade::nome_sem_h,"wt"))==NULL) Mensagem::falha(CRIACAO_ARQ,GeradorArquivoFacade::nome_sem_h);
		if((arq_sem_c=fopen(GeradorArquivoFacade::nome_sem_c,"wt"))==NULL) Mensagem::falha(CRIACAO_ARQ,GeradorArquivoFacade::nome_sem_c);
	}

	// Realiza a analise semantica da gramatica
	 espec();

	// Fecha os arquivos
	if(geraArquivo) {
		if (fclose(arq_sem_h)==EOF) Mensagem::falha(FECHAMENTO_ARQ,GeradorArquivoFacade::nome_sem_h);
		if (fclose(arq_sem_c)==EOF) Mensagem::falha(FECHAMENTO_ARQ,GeradorArquivoFacade::nome_sem_c);
	}

}

//-----------------------------------------------
// Recupera a proxima producao
//-----------------------------------------------
int AnalisadorSemantico::prox_prod() {
	int retorno= this->pilhaProducoesPrograma.top() ;
	pilhaProducoesPrograma.pop() ;
	return retorno;
}

//-----------------------------------------------
// Recupera o proximo atributo
//-----------------------------------------------
int AnalisadorSemantico::prox_atributo() {
	int valor =  listaAtributosPrograma.front();
	this->listaAtributosPrograma.pop_front();
	return valor;
}

#define PARAM_DECL 1   /* Declaracao de funcao */
#define PARAM_CHAM 2   /* Chamada de funcao    */
#define CODIGO     3   /* Codigo */
#define VAR_LOCAL  4   /* decl. de var local   */

#define PROC_SWITCH 1  /* Tipos de procedimento */
#define PROC_NORMAL 2


static int z;


/* --------------------------------------------------------------------------
 * Regra     : Espec -> Lista_prod ';' ;
 * Parametros:
 * Retorno   :
 */
 void AnalisadorSemantico::espec() {
    if (geraArquivo) {
      fputs("\n/* -----------------------------------------------------------------------*",arq_sem_c);
      fputs("\n * Processador Semantico gerado pelo FACC 3.0                             *",arq_sem_c);
      fputs("\n *                                                                        *",arq_sem_c);
      fputs("\n *  Software desenvolvido na Universidade de Brasilia - UnB, 1990         *",arq_sem_c);
      fputs("\n *  e aprimorado em 1991.                                                 *",arq_sem_c);
      fputs("\n *                                                                        *",arq_sem_c);
      fputs("\n *  Autor: Edward Lucio Vieira Borba                                      *",arq_sem_c);
      fputs("\n *                                                                        *",arq_sem_c);
      fputs("\n *       Este software nao deve ser usado para fins comerciais sem acordo *",arq_sem_c);
      fputs("\n *  previo com o autor.                                                   *",arq_sem_c);
      fputs("\n * -----------------------------------------------------------------------*/",arq_sem_c);
      fputs("\n",arq_sem_c);
      fprintf(arq_sem_c,"\n\n#include \"%s\"  /* Prototipos das funcoes */ \n",GeradorArquivoFacade::nome_sem_h);
      fputs("\n//-----------------------------------------------",arq_sem_c);
      fputs("\n// Chama o analisador semantico",arq_sem_c);
      fputs("\n//-----------------------------------------------",arq_sem_c);
      fputs("\nvoid AnalisadorSemantico::executa(std::stack<int> pilhaProducoesPrograma, std::list<int> listaAtributosPrograma) {",arq_sem_c);
      fputs("\n   this->pilhaProducoesPrograma = pilhaProducoesPrograma ;",arq_sem_c);
      fputs("\n   this->listaAtributosPrograma = listaAtributosPrograma ;",arq_sem_c);
      fprintf(arq_sem_c,"\n   %s() ;", EstruturaDados::listaNaoTerminais[0]->nome.c_str());
      fputs("\n}",arq_sem_c);
      fputs("\n",arq_sem_c);
      fputs("\n//-----------------------------------------------",arq_sem_c);
      fputs("\n// Recupera a proxima producao",arq_sem_c);
      fputs("\n//-----------------------------------------------",arq_sem_c);
      fputs("\nint AnalisadorSemantico::prox_prod() {",arq_sem_c);
      fputs("\n	int retorno= this->pilhaProducoesPrograma.top() ;",arq_sem_c);
      fputs("\n	pilhaProducoesPrograma.pop() ;",arq_sem_c);
      fputs("\n	return retorno;",arq_sem_c);
      fputs("\n}",arq_sem_c);
      fputs("\n",arq_sem_c);
      fputs("\n//-----------------------------------------------",arq_sem_c);
      fputs("\n// Recupera o proximo atributo",arq_sem_c);
      fputs("\n//-----------------------------------------------",arq_sem_c);
      fputs("\nint AnalisadorSemantico::prox_atributo() {",arq_sem_c);
      fputs("\n	int valor =  listaAtributosPrograma.front();",arq_sem_c);
      fputs("\n	listaAtributosPrograma.pop_front();",arq_sem_c);
      fputs("\n	return valor;",arq_sem_c);
      fputs("\n}\n",arq_sem_c);

      // Grava arquivo de header do semantico
      fputs("\n//---------------------------------------------------------------------------",arq_sem_h);
      fputs("\n#ifndef AnalisadorSemanticoH",arq_sem_h);
      fputs("\n#define AnalisadorSemanticoH",arq_sem_h);
      fputs("\n//---------------------------------------------------------------------------",arq_sem_h);
      fputs("\n",arq_sem_h);
      fputs("\n#include <stack>",arq_sem_h);
      fputs("\n#include <list>",arq_sem_h);
      fputs("\n#include <string>",arq_sem_h);
      fputs("\n",arq_sem_h);
      fputs("\n//------------------------------------------------------------------",arq_sem_h);
      fputs("\n// Classe que implementa o analisador semantico do compilador",arq_sem_h);
      fputs("\n//------------------------------------------------------------------",arq_sem_h);
      fputs("\nclass AnalisadorSemantico {",arq_sem_h);
      fputs("\n  public:      ",arq_sem_h);
      fputs("\n	//------------------------------------------------------------------------------",arq_sem_h);
      fputs("\n	// Chama o analisador semantico",arq_sem_h);
      fputs("\n	//------------------------------------------------------------------------------",arq_sem_h);
      fputs("\n	void executa(std::stack<int> pilhaProducoesPrograma, std::list<int> listaAtributosPrograma) ;",arq_sem_h);
      fputs("\n",arq_sem_h);
      fputs("\n  private:",arq_sem_h);
      fputs("\n	 std::stack<int> pilhaProducoesPrograma ;",arq_sem_h);
      fputs("\n	 std::list<int> listaAtributosPrograma ;",arq_sem_h);
      fputs("\n	 std::string msgErro ;",arq_sem_h);
      fputs("\n	 int prox_prod() ;",arq_sem_h);
      fputs("\n	 int prox_atributo() ;",arq_sem_h);
      fputs("\n",arq_sem_h);
      fputs("\n  public:      ",arq_sem_h);
      fputs("\n //------------------------------------------------------------------------------",arq_sem_h);
      fputs("\n // Tratamento de erros",arq_sem_h);
      fputs("\n //------------------------------------------------------------------------------",arq_sem_h);
      fputs("\n std::string getMsgErroSemantico() { return msgErro; }",arq_sem_h);
      fputs("\n bool encontrouErro() const { return msgErro != \"\"; };",arq_sem_h);
      fputs("\n",arq_sem_h);
      fputs("\n  private:",arq_sem_h);
      fputs("\n//------------------------------------------------------------------------------",arq_sem_h);
      fputs("\n// Funções para percorrer a árvore sintática",arq_sem_h);
      fputs("\n//------------------------------------------------------------------------------",arq_sem_h);
    }
    lista_prod();

    if (geraArquivo) {
      fputs("\n};",arq_sem_h);
 	  fputs("\n\n//---------------------------------------------------------------------------",arq_sem_h);
 	  fputs("\n #endif",arq_sem_h);
    }
 }

/* --------------------------------------------------------------------------
 * Regra     : Lista_prod -> Lista_prod ';' Prod |
 *                           Prod ;
 * Parametros:
 * Retorno   :
 */

 void AnalisadorSemantico::lista_prod() {
	 switch ( prox_prod() ) {

	 //----------------------------------------------------
	 // Lista_prod -> Lista_prod ';' Prod
	 //----------------------------------------------------
	 case  2: lista_prod();

	 //----------------------------------------------------
	 // Lista_prod -> Prod
	 //----------------------------------------------------
	 case  3: prod();
	 break;

	 default: printf("\nerro interno (FACC2SEM.C): producao (%d) fora de ordem",z);exit(-1);
	 }
 }

/* --------------------------------------------------------------------------
 * Regra     : Prod -> 'nterm' Param_opc Local_opc '->' Lista_dir ;
 * Parametros:
 * Glob.Acc  : GL_prod_ativa - (E) - producao ativa do nao terminal
 * Glob.Alt  : GL_prod_ativa - (E) - producao ativa do nao terminal
 * Retorno   :
 */

 void AnalisadorSemantico::prod() {
	// Recupera atributo do nao terminal
    int atributoNaoTerminal = prox_atributo();

    // Imprime nome do metodo
    if (geraArquivo) {
       fprintf(arq_sem_c,"\n\n\nvoid AnalisadorSemantico::%s", EstruturaDados::listaNaoTerminais[atributoNaoTerminal-1]->nome.c_str());
       fprintf(arq_sem_h,"\nvoid %s", EstruturaDados::listaNaoTerminais[atributoNaoTerminal-1]->nome.c_str());
    }

    GL_prod_ativa = prod_analise(atributoNaoTerminal); /* Atualiza ponteiro da prod corrente de nt */

    // Processa parametros opcionais
    param_opc(PARAM_DECL);

    /// Inicia corpo do metodo
    if (geraArquivo) {
       fputs("\n{",arq_sem_c);
    }

    // Processa variaveis locais
    local_opc();

    // Verifica se o nao terminal tem mais de uma producao (switch) ou nao
    char tipo_proc = tipo_procedimento(atributoNaoTerminal);

    // Inicia switch
    if(geraArquivo && tipo_proc == PROC_SWITCH) {
    	fputs("\n   switch( prox_prod() )\n   {", arq_sem_c);
    }

    // Processa lado direito da producao
    lista_dir(atributoNaoTerminal, tipo_proc);

    // Termina switch
    if(geraArquivo && tipo_proc == PROC_SWITCH) {
        fputs("\n   }", arq_sem_c);
    }

    // Termina o corpo do metodo
    if (geraArquivo) {
       fputs("\n}", arq_sem_c);
    }
 }

/* --------------------------------------------------------------------------
 * Regra     : Param_opc -> '(texto_par)' |
 *                           # ;
 * Parametros: tipo - (E) - determina decl. ou cham. de procedimento
 * Retorno   :
 */

 void AnalisadorSemantico::param_opc(char tipo)  {
	 switch ( prox_prod() ) {
	 //----------------------------------------------------
	 // Param_opc -> '(texto_par)'
	 //----------------------------------------------------
	 case  5:
		 // Inclui parametros
		 if (geraArquivo) {
			 inclui_texto(tipo);
		 }
		 break;

		 // Inclui fim do nome do metodo
	 //----------------------------------------------------
	 //	 Param_opc -> #
	 //----------------------------------------------------
	 case  6:
		 if (geraArquivo) {
			 if (tipo == PARAM_DECL) {
				 fputs("()", arq_sem_c);
				 fputs("();", arq_sem_h);
			 } else {
				 fputs("();", arq_sem_c);
			 }
		 }
		 break;

	 default: printf("\nerro interno (FACC2SEM.C): producao (%d) fora de ordem",z);exit(-1);
    }
 }

/* --------------------------------------------------------------------------
 * Regra     : Local_opc -> 'local' '{texto_cod}' |
 *                           # ;
 * Parametros:
 * Retorno   :
 */

 void AnalisadorSemantico::local_opc() {
    GL_spc = 3;
    switch ( prox_prod() ) {
	 //----------------------------------------------------
     // Local_opc -> 'local' '{texto_cod}'
	 //----------------------------------------------------
    case  7:
    	// Inclui declaracoes de variaveis locais ao procedimento
    	if (geraArquivo) inclui_texto(VAR_LOCAL);
    break;

    //----------------------------------------------------
    // Local_opc ->  #
    //----------------------------------------------------
    case  8: break;

    default: printf("\nerro interno (FACC2SEM.C): producao (%d) fora de ordem",z);exit(-1);
    }
 }

/* --------------------------------------------------------------------------
 * Regra     : Codigo_opc -> Codigo_opc '{texto_cod}' |
 *                           # ;
 * Parametros:
 * Retorno   :
 */

 void AnalisadorSemantico::codigo_opc() {
	 switch ( prox_prod() )  {
	 //----------------------------------------------------
	 // Codigo_opc -> Codigo_opc '{texto_cod}'
	 //----------------------------------------------------
	 case  9:
		 codigo_opc();
		 // inclui codigo no procedimento
		 if (geraArquivo) {
			 inclui_texto(CODIGO);
		 }
		 break;

     //----------------------------------------------------
	 // Codigo_opc -> #
	 //----------------------------------------------------
	 case 10: break;
	 default: printf("\nerro interno (FACC2SEM.C): producao (%d) fora de ordem",z);exit(-1);
	 }
 }


/* --------------------------------------------------------------------------
 * Regra     : Lista_dir -> Lista_dir '|' Codigo_opc Parte_dir |
 *                          Codigo_opc Parte_dir ;
 * Parametros: nt            - (E) - nao terminal em analise
 *             tipo_proc     - (E) - tipo do procedimento
 * Glob.Acc  : GL_prod_ativa - (E) - producao ativa do nao terminal
 * Glob.Alt  : GL_prod_ativa - (E) - producao ativa do nao terminal
 * Retorno   :
 */

 void AnalisadorSemantico::lista_dir(int nt, char tipo_proc) {
	 switch ( prox_prod() ) {
	 //----------------------------------------------------
	 // Lista_dir -> Lista_dir '|' Codigo_opc Parte_dir
	 //----------------------------------------------------
	 case 11:
		 lista_dir(nt, tipo_proc) ;

	 //----------------------------------------------------
	 // Lista_dir -> Codigo_opc Parte_dir
	 //----------------------------------------------------
	 case 12:
		 // Inclui case
		 if (tipo_proc == PROC_SWITCH) {
			 if (geraArquivo) {
				 fprintf(arq_sem_c,"\n      case %3d:",GL_prod_ativa+1);
			 }
			 GL_prod_ativa=prod_analise(nt);  // Incrementa prod_ativa de nt_prod_asso
			 GL_spc = 9;
		 } else {
			 GL_spc = 3;
		 }

		 // Processa Codigo_opc
		 codigo_opc();

		 // Processa Parte_dir
		 parte_dir(tipo_proc);
		 break;

	 default: printf("\nerro interno (FACC2SEM.C): producao (%d) fora de ordem",z);exit(-1);
    }
 }

/* --------------------------------------------------------------------------
 * Regra     : Parte_dir -> Lista_simb |
 *                          '#' Codigo_opc ;
 * Parametros: tipo_proc - (E) - tipo do procedimento
 * Retorno   :
 */

 void AnalisadorSemantico::parte_dir(char tipo_proc) {
    switch ( prox_prod() ) {

	   //----------------------------------------------------
       // Parte_dir -> Lista_simb
	   //----------------------------------------------------
       case 13:
    	   GL_spc = (tipo_proc == PROC_SWITCH) ? 9 : 3;
    	   // Processa Lista_simb
    	   lista_simb();
    	   if (geraArquivo && tipo_proc == PROC_SWITCH) {
    		   fputs("\n      break;",arq_sem_c);
    	   }
    	   break;

       //----------------------------------------------------
       // Parte_dir -> '#' Codigo_opc
       //----------------------------------------------------
       case 14:
    	   GL_spc = (tipo_proc == PROC_SWITCH) ? 9 : 3;
	       codigo_opc();
	       if(geraArquivo && tipo_proc == PROC_SWITCH ) {
	    	   fputs("\n      break;",arq_sem_c);
	       }
	       break;

       default: printf("\nerro interno (FACC2SEM.C): producao (%d) fora de ordem",z);exit(-1);
    }
 }

/* --------------------------------------------------------------------------
 * Regra     : Lista_simb -> Lista_simb Simb Codigo_opc |
 *                           Simb Codigo_opc ;
 * Parametros:
 * Retorno   :
 */

 void AnalisadorSemantico::lista_simb() {
	 switch ( prox_prod() ) {

	 //----------------------------------------------------
	 // Lista_simb -> Lista_simb Simb Codigo_opc
	 //----------------------------------------------------
	 case 15:
		 lista_simb();

	 //----------------------------------------------------
	 // Lista_simb -> Simb Codigo_opc
	 //----------------------------------------------------
	 case 16:
		 simb();
		 codigo_opc();
		 break;

	 default: printf("\nerro interno (FACC2SEM.C): producao (%d) fora de ordem",z);exit(-1);
	 }
 }

/* --------------------------------------------------------------------------
 * Regra     : Simb -> 'nterm' Param_opc |
 *                     'term' Atrib_opc ;
 * Parametros:
 * Retorno   :
 */

 void AnalisadorSemantico::simb() {
	 //int nte, te;

	 switch ( prox_prod() ) {

	 //----------------------------------------------------
	 // Simb -> 'nterm' Param_opc
	 //----------------------------------------------------
	 case 17: {
		 int nte = prox_atributo();
		 // Imprime nome proc
		 if (geraArquivo) {
			 fprintf(arq_sem_c,"\n%*s%s",GL_spc," ", EstruturaDados::listaNaoTerminais[nte-1]->nome.c_str());
		 }
		 param_opc(PARAM_CHAM);
	 }
	 break;

	 //----------------------------------------------------
	 // Simb -> 'term' Atrib_opc
	 //----------------------------------------------------
	 case 18: {
		 int te = prox_atributo();
		 atrib_opc(te);
	 }
	 break;

	 default: printf("\nerro interno (FACC2SEM.C): producao (%d) fora de ordem",z);exit(-1);
	 }
 }

/* --------------------------------------------------------------------------
 * Regra     : Atrib_opc -> '.atrib'  |
 *                         # ;
 * Parametros: te - (E) - terminal com atributo
 * Retorno   :
 */

 void AnalisadorSemantico::atrib_opc(int te) {
	 switch ( prox_prod() ){

	 //----------------------------------------------------
	 // Atrib_opc -> '.atrib'
	 //----------------------------------------------------
	 case 19: {
		 int atributo = prox_atributo();
		 EstruturaDados::listaTerminais[te-1]->possuiAtributo  = true;
		 if (geraArquivo) {
			 fprintf(arq_sem_c,"\n%*sprox_atributo();",GL_spc," ");
		 }
	 }
	 break;

	 //----------------------------------------------------
	 // Atrib_opc -> #
	 //----------------------------------------------------
	 case 20:break;

	 default: printf("\nerro interno (FACC2SEM.C): producao (%d) fora de ordem",z);exit(-1);
	 }
 }





/* --------------------------------------------------------------------------*
 * Descricao : Informa de procedimento e do tipo switch ou nao
 * Parametros: nt - (E) - nao-terminal em analise
 * Retorno   : tipo de procedimento (SWITCH ou NORMAL)
 */

 char AnalisadorSemantico::tipo_procedimento(int atributoNaoTerminal) {
   if(EstruturaDados::listaNaoTerminais[atributoNaoTerminal-1]->listaProducaoAssociada.size() > 1)
      return(PROC_SWITCH);
   else
      return(PROC_NORMAL);
 }


/* --------------------------------------------------------------------------*
 * Descricao : Posiciona na producao corrente do nao-terminal informado
 * Parametros: nt - (E) - nao-terminal em analise
 * Retorno   : producao em analise
 */

 int AnalisadorSemantico::prod_analise(int nt) {
   static int nt_uso = 1;

   if(nt_uso != nt) {
	   EstruturaDados::listaNaoTerminais[nt_uso-1]->producao_corrente = 0;
      nt_uso = nt;
   }

   int numProducao = EstruturaDados::listaNaoTerminais[nt-1]->producao_corrente++ ;
   int retorno = EstruturaDados::listaNaoTerminais[nt-1]->listaProducaoAssociada[numProducao] ;
   return retorno ;
 }


/* --------------------------------------------------------------------------*
 * Descricao : Inclui um codigo no processador semantico
 * Parametros: codigo - (E) - chave do codigo a ser inserido
 *             tipo   - (E) - tipo de codigo a ser inserido
 * Retorno   : nenhum
 */

 void AnalisadorSemantico::inclui_texto(int tipo) {
    static int h = 0;

    if(EstruturaDados::listaTexto.size() == 0) {
        Mensagem::falha(CODIGO_INEXISTENTE,NULL);
    }

    // Recupera o texto da lista
    std::string textoLista = EstruturaDados::listaTexto[h];
    h++;

    switch(tipo) {
      // Adiciona texto para parametro de chamada
      case PARAM_CHAM:
	  fputc('(',arq_sem_c);
	  for(unsigned int i=0; i < textoLista.size(); i++) {
	    char ch = textoLista[i];
	    if (ch == NL) {
	      fputc('\n',arq_sem_c);
	      for(int j=0; j < GL_spc; j++)
		     fputc(' ', arq_sem_c);
	    }
	    else
	       fputc(ch,arq_sem_c);
	  }
	  fputs(");",arq_sem_c);
	  break;

	  // Adiciona texto para declaracao de parametro
       case PARAM_DECL:
	  fputc('(',arq_sem_c);
	  fputc('(',arq_sem_h);
	  for(unsigned int i=0; i < textoLista.size(); i++) {
	    char ch = textoLista[i];
	    if (ch == NL) {
	      fputc('\n',arq_sem_c);
	      fputc('\n',arq_sem_h);
	       for(int j=0; j < GL_spc; j++) {
		      fputc(' ', arq_sem_c);
		      fputc(' ', arq_sem_h);
	       }
	    }
	    else {
	       fputc(ch,arq_sem_c);
	       fputc(ch,arq_sem_h);
	    }
	  }
	  fputc(')',arq_sem_c);
	  fputc(')',arq_sem_h);
	  fputc(';',arq_sem_h);
	  break;

	  // Adiciona texto para variavel local
       case VAR_LOCAL:
       case CODIGO:
	  fputc('\n',arq_sem_c);
	  for(int j=0; j < GL_spc; j++)
	     fputc(' ', arq_sem_c);

	  for(unsigned int i=0; i < textoLista.size(); i++) {
	    char ch = textoLista[i];
	    if (ch == NL) {
	      fputc('\n',arq_sem_c);
	       for(int j=0; j < GL_spc; j++)
		  fputc(' ', arq_sem_c);
	    }
	    else
	       fputc(ch,arq_sem_c);
	  }
	  if (tipo == VAR_LOCAL) {
	     fputc('\n',arq_sem_c);
	  }
	  break;
    }
 }




