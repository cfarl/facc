
/* -----------------------------------------------------------------------*
 * Processador Semantico gerado pelo FACC 2.0                             *
 *                                                                        *
 *  Software desenvolvido na Universidade de Brasilia - UnB, 1990         *
 *  e aprimorado em 1991.                                                 *
 *                                                                        *
 *  Autor: Edward Lucio Vieira Borba                                      *
 *                                                                        *
 *       Este software nao deve ser usado para fins comerciais sem acordo *
 *  previo com o autor.                                                   *
 * -----------------------------------------------------------------------*/


#include "AnalisadorSemantico.h"  /* Prototipos das funcoes */ 

//-----------------------------------------------
// Chama o analisador semantico
//-----------------------------------------------
int AnalisadorSemantico::executa(std::stack<int> pilhaProducoesPrograma, std::list<int> listaAtributosPrograma) {
   this->pilhaProducoesPrograma = pilhaProducoesPrograma ;
   this->listaAtributosPrograma = listaAtributosPrograma ;
   return expressao_aritmetica() ;
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
	listaAtributosPrograma.pop_front();
	return valor;
}



int AnalisadorSemantico::expressao_aritmetica()
{
   switch( prox_prod() )
   {
      // expressao_aritmetica -> termo
      case   1:
         return termo();
      break;

      // expressao_aritmetica -> expressao_aritmetica '+' termo
      case   2: {
         int op1 = expressao_aritmetica();
         int op2 = termo();
         return op1 + op2 ;
      }
      break;

      // expressao_aritmetica -> expressao_aritmetica '-' termo
      case   3: {
         int op1 = expressao_aritmetica();
         int op2 = termo();
         return op1 - op2 ;
      }
      break;
   }
}


int AnalisadorSemantico::termo()
{
   switch( prox_prod() )
   {
   	  // termo -> fator
      case   4:
         return fator();
      break;

      // termo -> termo '*' fator
      case   5: {
         int op1 = termo();
         int op2 = fator();
         return op1 * op2 ;
      }
      break;

      // termo -> termo '/' fator
      case   6: {
         int op1 = termo();
         int op2 = fator();
         return op1 / op2 ;
      } break;
   }
}


int AnalisadorSemantico::fator()
{
   switch( prox_prod() )
   {
      // fator -> 'num'.numatrib
      case   7: {
        int numero = prox_atributo();
        return numero ;
      } break;

      // fator -> '(' expressao_aritmetica ')'
      case   8:
         return expressao_aritmetica();
      break;
   }
}
