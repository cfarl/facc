/* -----------------------------------------------------------------------*
 *                                                                        *
 * Programa    : FACC.EXE                                Versao 2.0       *
 *                                                                        *
 * Modulo      : FACCMSG.C                                                *
 *                                                                        *
 * Autores     : Edward Lucio Vieira Borba  			          *
 *                                                                        *
 * Data inicio : 31/03/91                                                 *
 * Data Atual. : 09/09/91                                                 *
 *                                                                        *
 * Descricao   : Mensagens de erro e advertencia do FACC.                 *
 *                                                                        *
 * -----------------------------------------------------------------------*/

#include "../rotinas/EstruturaDados.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lexico/Token.h"
#include "Constantes.h"
#include "Mensagem.h"


#define MAX_SIMB            30            /* Tamanho maximo de identificador */

#define SAIDA_FALHA	1		 /* Abandono por falha na compilacao */

/* --------------------------------------------------------------------------*
 * Descricao :Emissao de mensagens de falhas irrecuperaveis de compilacao.   *
 * Entrada   :tipo_falha : tipo da falha de compilacao                       *
 *            arq        : arquivo sendo manipulado                          *
 * Retorno   :Nenhum.                                                        *
 *-------------------------------------------------------------------------- */

void Mensagem::falha(int tipo_falha, char *arq)
{
  char texto[81];                  /* Endereco do texto da mensagem de falha */

  texto[0]='\0';
  switch(tipo_falha)
  {                             /* Falhas de IO */
    case ABERTURA_ARQ:
	sprintf(texto,"Arquivo %s nao encontrado",arq);
    	break;
    case LEITURA_ARQ:
	sprintf(texto,"Impossivel ler o arquivo %s",arq);
    	break;
    case CRIACAO_ARQ:
	sprintf(texto,"Impossivel criar arquivo %s",arq);
    	break;
    case FECHAMENTO_ARQ:
	sprintf(texto,"Impossivel fechar arquivo %s",arq);
    	break;
    case GRAVACAO_ARQ:
	sprintf(texto,"Impossivel gravar no arquivo %s",arq);
	break;
				/* Falhas de Manipulacao de memoria */
    case GRAMATICA_GRANDE:
	strcpy(texto,"Numero de simbolos da gramatica maior que o permitido");
	break;
    case ALOCACAO_MEMORIA:
	strcpy(texto,"Memoria insuficiente. Programa muito grande");
	break;
				/* Falhas internas nao previstas */
    case TABELA_ACAO:
	strcpy(texto,"Falha na Tabela de Acao do parser");
    	break;
    case TABELA_GOTO:
	strcpy(texto,"Falha na Tabela de Goto do parser");
    	break;
    case DESEMPILHA:
	strcpy(texto,"Falha na pilha de estados");
    	break;
    case LISTA_ACAO:
	strcpy(texto,"Falha na procura de um terminal na lista de acao");
    	break;
    case TABELA_HASH:
	strcpy(texto,"Falha na procura de um simbolo na hash");
	break;
    case PRODUCAO_NTERM:
	strcpy(texto,"Falha de acesso a uma producao associada a um nterm");
	break;

    default:
	strcpy(texto,"Falha nao cadastrada");
    	break;
  }

  fprintf(stdout,"\nL.%03d: %s.", EstruturaDados::linhaCorrenteFonte,texto);
  fputs("\nCompilacao abortada.\n",stdout);
  exit(SAIDA_FALHA);
}

/* --------------------------------------------------------------------------*
 * Descricao : Emissao de mensagens de erros durante a compilacao            *
 * Entrada   : tipo_erro : tipo do erro de compilacao                        *
 *             param     : nro de linha ; token ; cod. de simbolo ;          *
 *                         nro de producao.                                  *
 * Retorno   : Nenhum.                                                       *
 *-------------------------------------------------------------------------- */

void Mensagem::erro(int tipo_erro, int param)

{
  char texto[81];                       /* Mensagem relativa ao tipo de erro */

  if(!EstruturaDados::houve_erro)
	  EstruturaDados::houve_erro=true;
  texto[0] = '\0';

  switch(tipo_erro)
  {                             /* Erros Lexicos */

    case COMENTARIO:
	sprintf(texto,"Comentario aberto na linha %d nao fechado", param);
	break;
    case TERMINAL_NAO_FECHADO:
	sprintf(texto,"Terminal da linha %d nao fechado",param);
	break;
    case TEXTO_NAO_FECHADO:
	sprintf(texto,"Texto da linha %d nao fechado",param);
	break;
    case TERMINAL_VAZIO:
	strcpy(texto,"Terminal nao pode ser deixado em branco");
	break;
    case ATRIBUTO_VAZIO:
	strcpy(texto,"Atributo nao pode ser deixado em branco");
	break;
    case SIMBOLO_DESCONHECIDO:
	strcpy(texto,"Simbolo invalido para a gramatica");
	break;
    case INICIO_ATRIBUTO:
	strcpy(texto,"Atributo deve iniciar por letra");
	break;


				/* Erros Sintaticos */
    case ESPERADO:
    case NAO_ESPERADO:
      switch (param)
      {
        case TOK_PONTO_VIRGULA:
		strcpy(texto,";");
		break;
	case TOK_NTERM:
		strcpy(texto,"Nao Terminal");
		break;
	case TOK_SETA:
		strcpy(texto,"->");
		break;
	case TOK_TEXTO_PAR:
		strcpy(texto,"( Parametro de nao terminal )");
		break;
	case TOK_LOCAL:
		strcpy(texto,"LOCAL");
		break;
	case TOK_TEXTO_COD:
		strcpy(texto,"{ Codigo } ");
		break;
	case TOK_OU:
		strcpy(texto,"|");
		break;
	case TOK_VAZIO:
		strcpy(texto,"#");
		break;
	case TOK_TERM:
		strcpy(texto,"Terminal");
		break;
	case TOK_ATRIB:
		strcpy(texto,"Atributo de Terminal");
		break;
	case TOK_ETX:
		strcpy(texto,"Fim do arquivo");
		break;
	default:
		sprintf(texto,"Erro interno. Token(%d) indefinido", param);
		break;
      }
      if (tipo_erro == ESPERADO)
	strcat(texto," esperado");
      else  /* NAO_ESPERADO */
	strcat(texto," nao esperado");
      break;

    case SIMBOLO_ESP:
	strcpy(texto," Simbolo (terminal, nao terminal ou #) esperado ");
	break;

				/* Erros da Gramatica */

    case ATINGE_TERMINAL:
	sprintf(texto,"Nao-terminal %s nao deriva terminal",
			EstruturaDados::listaNaoTerminais[param -1]->nome.c_str());

	break;
    case ATINGIDO_POR_INICIAL:
	sprintf(texto,"Simbolo inicial nao deriva nao-terminal %s" ,
			EstruturaDados::listaNaoTerminais[param -1]->nome.c_str());
	break;
    case PRODUCAO_REPETIDA:
	sprintf(texto,"Producao da linha %d duplicada na gramatica", param);
	break;
    default:
	strcpy(texto,"Erro interno. Tipo de erro indefinido");
	break;
  }

  /* ------ Erro Semantico tem Mensagem diferenciada e pode ser mais de
	    uma por linha
  */

  if( (ATINGE_TERMINAL <= tipo_erro) && (tipo_erro <= PRODUCAO_REPETIDA) )
     fprintf(stdout,"\nErro Semantico: %s.",texto);
  else
    if(!EstruturaDados::linha_errada)
    {
      fprintf(stdout,"\nErro L.%3d: %s.", EstruturaDados::linhaCorrenteFonte,texto);
      EstruturaDados::linha_errada=true;
    }
}

/* --------------------------------------------------------------------------*
 * Descricao : Emissao de mensagens de advertencia na compilacao             *
 * Entrada   : tipo_advertencia:                                             *
 * Retorno   : Nenhum.                                                       *
 *-------------------------------------------------------------------------- */

void Mensagem::advertencia(int tipo_advertencia)
{
  char texto[81];                        /* Mensagem relativa `a advertencia */
  if(!EstruturaDados::houve_erro)
	  EstruturaDados::houve_erro=true;
  texto[0] = '\0';

  switch(tipo_advertencia)
  {
    case SIMBOLO_TRUNCADO:
		sprintf(texto,"Simbolo truncado na posicao %2d",MAX_SIMB);
		break;
    case TERM_SUSPEITO:
		strcpy(texto,"Terminal com branco na(s) extremidade(s)");
		break;
    default:
	    strcpy(texto,"Erro interno. Tipo de advertencia indefinida");
	    break;

  }
  if(!EstruturaDados::linha_suspeita)
  {
    fprintf(stdout,"\nAdvertencia L.%3d: %s.", EstruturaDados::linhaCorrenteFonte,texto);
    EstruturaDados::linha_suspeita=true;
  }
}
