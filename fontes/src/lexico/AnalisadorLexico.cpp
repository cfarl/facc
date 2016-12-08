
//-----------------------------------------------------------------------
// Analisador lexico do Interpretador de Gramaticas.
//-----------------------------------------------------------------------

#include "AnalisadorLexico.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>

#include "../geracaoArquivo/GeradorArquivoFacade.h"
#include "../rotinas/EstruturaDados.h"
#include "../util/Constantes.h"
#include "../util/Mensagem.h"
#include "Token.h"


#define ERRO_FTELL -1L
#define NL 0xA

//--------------------------------------------------
/** Construtor da classe */
//--------------------------------------------------
AnalisadorLexico::AnalisadorLexico() {
	EstruturaDados::linhaCorrenteFonte = 0;
	atributo_terminal = 0;
	proxCaractere();
}


//--------------------------------------------------------------------------
// Le um caractere do arquivo fonte, descartando caracteres de controle
//---------------------------------------------------------------------------
void AnalisadorLexico::proxCaractere() {
  do {
    charCorrente=fgetc(EstruturaDados::arq_fonte);
    if(charCorrente == NL) {
    	novaLinha();
    }
    if(ferror(EstruturaDados::arq_fonte)) {
      Mensagem::falha(LEITURA_ARQ, GeradorArquivoFacade::nome_fonte);
    }
  } while(iscntrl(charCorrente));
  return;
}


//---------------------------------------------------------
// Insere o simbolo na tabela hash
//---------------------------------------------------------
void AnalisadorLexico::insereTabelaHash(std::string identificador, char tipoSimbolo, int posLista) {
  // Inclui uma nova entrada na tabela de hash
  EntradaTabelaHash novaEntrada ;
  novaEntrada.tipo= tipoSimbolo ;
  novaEntrada.posLista= posLista;
  novaEntrada.texto= identificador;
  tabela_hash.insere(identificador, novaEntrada);
}

//-----------------------------------------------------------------
// Insere o identificador no final da lista correpondente ao tipo
// Retorna o tamanho da lista em que o simbolo foi adicionado
//-----------------------------------------------------------------
int AnalisadorLexico::insereLista(std::string identificador, char tipoSimbolo) {
  // Cria nova entrada para lista de terminais, atributos ou nao-terminais
  struct EntradaTabelaSimbolos *entradaTabSimbolos = new EntradaTabelaSimbolos();
  entradaTabSimbolos->nome = identificador ;

  // Inclui a entrada na lista correspondente ao tipo do simbolo
  if(tipoSimbolo == TERMINAL) {
	  EstruturaDados::listaTerminais.push_back(entradaTabSimbolos);
	  return EstruturaDados::listaTerminais.size() ;

  } else if(tipoSimbolo ==  ATRIBUTO) {
	  EstruturaDados::listaAtributos.push_back(entradaTabSimbolos);
	  return EstruturaDados::listaAtributos.size() ;

  } else if(tipoSimbolo == NAO_TERMINAL) {
	  EstruturaDados::listaNaoTerminais.push_back(entradaTabSimbolos);
	  return EstruturaDados::listaNaoTerminais.size() ;
  }
}


//-------------------------------------------------------------------------------------
// Retorna a posicao do identificador na lista do tipo informado (TERM, NTERM, ATRIB)
//-------------------------------------------------------------------------------------
int AnalisadorLexico::procuraTabelaHash(std::string identificador, char tipoSimbolo) {
  // Procura as entradas da tabela hash que estao associadas ao texto
  std::vector<EntradaTabelaHash> *listaEntradas = tabela_hash.procura(identificador);

  // Se a entrada existe na tabela hash, retorna seu codigo
  for(unsigned int i=0; listaEntradas!=NULL && i < listaEntradas->size(); i++) {
	  EntradaTabelaHash entrada = (*listaEntradas)[i] ;
	  if (identificador == entrada.texto && tipoSimbolo == entrada.tipo) {
		  return entrada.posLista ;
	  }
  }

  // Se a entrada nao foi encontrada na tabela de hash, insere agora
  int posLista = insereLista(identificador, tipoSimbolo);
  insereTabelaHash(identificador, tipoSimbolo, posLista);
  return posLista ;
}

//--------------------------------------------------------------------------
// Inicia nova linha
//--------------------------------------------------------------------------
void AnalisadorLexico::novaLinha() {
	EstruturaDados::linhaCorrenteFonte++;
	EstruturaDados::linha_errada= false;
	EstruturaDados::linha_suspeita= false;
}

//--------------------------------------------------------------------------
// Elimina comentario na forma /* */
//--------------------------------------------------------------------------
void AnalisadorLexico::eliminaComentario() {
  int linhaComentario = EstruturaDados::linhaCorrenteFonte ;
  proxCaractere();

  do {
	// Procura o * do */ que fecha o comentario
    while (charCorrente!='*' && charCorrente!=EOF) {
      if(charCorrente==NL) {
    	 novaLinha();
      }
      proxCaractere();
    }

    // Pega proximo caractere
    if (charCorrente!=EOF) {
      proxCaractere();
    }
  } while (charCorrente!='/' && charCorrente!=EOF);


  if (charCorrente=='/') {
    proxCaractere();
    return ;
  }

  // Se saiu do loop e chegou aqui, encontrou fim de arquivo no meio do comentario
  Mensagem::erro(COMENTARIO, linhaComentario );
}


/* --------------------------------------------------------------------------*
 * Descricao : Inclui um simbolo na lista de producoes                       *
 * Entrada   : tipo_simb -> tipo do simbolo (VAZIO,TERM,NTERM)               *
 *             codigo    -> codigo do simbolo                                *
 *             parte_direita -> indica se e' uma producao c/ nao-terminal    *
 *                              igual ao da producao anterior.               *
 * Retorno   : Nenhum                                                        *
 *-------------------------------------------------------------------------- */
void AnalisadorLexico::incluiListaProducoes(char tipoSimbolo, int codigo, bool *ladoDireitoProducao) {
  // Se esta do lado ESQUERDA da producao
  if (!(*ladoDireitoProducao)) {
	  // Recupera o nao terminal que tem o codigo informado
	  struct EntradaTabelaSimbolos* naoTerminal = EstruturaDados::listaNaoTerminais[codigo-1];

    // Cria uma nova producao
    struct Producao *producao = new Producao();
    producao->codigoLadoEsquerdo = codigo;
    producao->numNaoTerminaisLadoDireito = 0;
    producao->linha = EstruturaDados::linhaCorrenteFonte;
    producao->ladoEsquerdo = naoTerminal->nome ;

    // Adiciona a producao na lista de producoes
    EstruturaDados::listaProducoes.push_back(producao);

    // Associa a nova producao ao nao terminal que corresponde ao codigo
    naoTerminal->listaProducaoAssociada.push_back(EstruturaDados::listaProducoes.size()-1);

    // A producao criada passa a ser a producao atual
    producaoAtual = producao;

    // Apos nt do lado esq. assume-se: lado dir
    *ladoDireitoProducao = true;
    return ;
  }

  // Se esta na parte DIREITA da producao
  // Cria lado direito da producao
  struct SimboloLadoDireitoProducao *pLadoDireito = new SimboloLadoDireitoProducao();
  pLadoDireito->tipo = tipoSimbolo;
  pLadoDireito->codigo = codigo;
  producaoAtual->listaSimbolosLadoDireito.push_back(pLadoDireito); // simbolo a direita

  if (tipoSimbolo == NAO_TERMINAL) {
   	 producaoAtual->numNaoTerminaisLadoDireito++;
   	 pLadoDireito->nome = EstruturaDados::listaNaoTerminais[codigo-1]->nome ;
  } else if (tipoSimbolo == TERMINAL) {
	  pLadoDireito->nome = EstruturaDados::listaTerminais[codigo-1]->nome ;
  }
}

//--------------------------------------------------------------------------
// Reconhece um identificador no formato L{L|D}*
//--------------------------------------------------------------------------
 int AnalisadorLexico::reconheceIdentificador(char tipoIdentificador) {
  std::string identificadorLido = "" ;

  // Recupera o identificador
  while (isalnum(charCorrente) || charCorrente=='_' ) {
    identificadorLido += charCorrente ;
    proxCaractere();
  } 

  // Se o identificador for o nome de um atributo, deve ter ao menos um caractere
  if((tipoIdentificador == ATRIBUTO) && (identificadorLido.size() == 0)) {
	  Mensagem::erro(ATRIBUTO_VAZIO, 0);
	  return TOK_ATRIB;
  }

  // Se encontrou a palavra 'local' retorna token local
  if (stricmp(identificadorLido.c_str(),"LOCAL") == 0) {
	  return TOK_LOCAL;
  }

  // Caso contrario procura o identificador na tabela hash
  atributo_terminal = procuraTabelaHash(identificadorLido, tipoIdentificador);
  return (tipoIdentificador == NAO_TERMINAL) ? TOK_NTERM : TOK_ATRIB ;
}


/* --------------------------------------------------------------------------*
 * Descricao : Reconhece um texto de parametro ou codigo                     *
 * Entrada   : tipo_texto -> tipo do texto a reconhecer                      *
 *             delimitador -> delimitador inicio de texto                    *
 * Retorno   : Token do tipo de texto reconhecido                            *
 *-------------------------------------------------------------------------- */
 int AnalisadorLexico::reconheceTexto(char tipo_texto, char delimitadorAbertura) {
  // Se o texto for Texto de parametro, delimitador de fim é )
  // Se o texto for Texto de codigo, delimitador de fim é }
  char delimitadorFechamento = (tipo_texto == TOK_TEXTO_PAR) ? ')' : '}' ;

  // linha de inicio do texto
  int linhaTexto = EstruturaDados::linhaCorrenteFonte;

  int abre=1; // conta abre_delimitador
  std::string textoLido = "" ;
  while ( !((charCorrente == delimitadorFechamento) && (abre == 0)) && charCorrente!=EOF ) {
    proxCaractere();

    // Conta delimitador que abre texto
    if (charCorrente == delimitadorAbertura) {
      abre++;

    // Se achou o delimitador que fecha o texto, adiciona na lista de textos
    } else if (charCorrente ==  delimitadorFechamento) {
      if (--abre == 0) {
    	  EstruturaDados::listaTexto.push_back(textoLido);
      }

    // Registra nova linha
    } else if (charCorrente == NL) {
      novaLinha();

    // Verifica se alcancou fim de arquivo
    } else if ( charCorrente == EOF) {
    	Mensagem::erro(TEXTO_NAO_FECHADO,linhaTexto);

    // Adiciona o caractere lido no texto
    } else {
    	textoLido += charCorrente ;
    }
  }

  // Retorna
  proxCaractere();
  return(tipo_texto);
 }

//--------------------------------------------------------------------------
// Reconhece um terminal na forma '|" L{L|D}* '|"
// Entrada   : delimitador -> delimitador usado para o terminal
//             codigo -> codigo do terminal
//--------------------------------------------------------------------------
 int AnalisadorLexico::reconheceTerminal(char delimitador) {
  int linhaTerminal = EstruturaDados::linhaCorrenteFonte;  // Linha de inicio do terminal

  std::string cadeiaLida = "" ;
  proxCaractere();

  // Recupera cadeia
  while (charCorrente!=delimitador && charCorrente!=EOF) {
	  // Se encontrou fim de linha, atualiza linha corrente
	  if (charCorrente==NL) {
		  novaLinha();
		  proxCaractere();

	  // Guarda caractere na cadeia
	  } else {
		  cadeiaLida+= charCorrente ;
		  proxCaractere();
	  }
  }

  proxCaractere();

  // Verifica se a cadeia ficou aberta
  if (charCorrente==EOF) {
	  Mensagem::erro(TERMINAL_NAO_FECHADO,linhaTerminal);
	  return TOK_TERM ;
  }

  // Verifica se o terminal esta em branco
  if ((cadeiaLida.size()==0) || (cadeiaLida.size() == strspn(cadeiaLida.c_str()," ")) ) {
	  Mensagem::erro(TERMINAL_VAZIO, 0);
	  return TOK_TERM ;
  }

  // Verifica se o terminal comeca ou termina com branco
  if ((cadeiaLida[0]==' ') || (cadeiaLida[cadeiaLida.size()-1]==' ')) {
	 Mensagem::advertencia(TERM_SUSPEITO);
  }

  // Retorna token para terminal
  atributo_terminal = procuraTabelaHash(cadeiaLida, TERMINAL);
  return TOK_TERM;
}

/* --------------------------------------------------------------------------*
 * Descricao : Le o proximo simbolo da especificacao                         *
 * Entrada   : Nenhuma                                                       *
 * Retorno   : Token do simbolo lido                                         *
 *-------------------------------------------------------------------------- */
int AnalisadorLexico::proxToken() {
  // Indica que estamos analisando os tokens no lado direito de uma producao
  static bool ladoDireitoProducao = false ;

  // Token retornado pelo metodo
  char tokenRetorno = -1;

  // Loop para retornar o proximo token
  while ((tokenRetorno==-1) && (charCorrente!=EOF)) {

	// Descarta espaco em branco e quebra de linha
    while (charCorrente == ' ' || charCorrente == NL ) {
      	proxCaractere();
    }

    // Elimina comentario na forma /* */
    if (charCorrente=='/') {
      proxCaractere();
      if (charCorrente=='*') {
    	  eliminaComentario();
      } else {
    	  Mensagem::erro(SIMBOLO_DESCONHECIDO, 0);
      }

    // Se comeca com caractere, reconhece nao terminal
    } else if (isalpha(charCorrente)) {
      tokenRetorno = reconheceIdentificador(NAO_TERMINAL) ;
      if ((!EstruturaDados::houve_erro) && (tokenRetorno == TOK_NTERM)) {       /* So insere nterm */
    	  incluiListaProducoes(NAO_TERMINAL, atributo_terminal, &ladoDireitoProducao);
      }
    }

    // Se comeca com ponto, reconhece atributo de terminal
    else if (charCorrente=='.') {
      proxCaractere();
      if (!isalpha(charCorrente)) {
    	  Mensagem::erro(INICIO_ATRIBUTO, 0);
      }
      tokenRetorno = reconheceIdentificador(ATRIBUTO);
    }

    // Se comeca com aspas, reconhece terminal
    else if ( (charCorrente=='\'') || (charCorrente=='"') ) {
      tokenRetorno = reconheceTerminal(charCorrente);
      if (!EstruturaDados::houve_erro) {
    	  incluiListaProducoes(TERMINAL, atributo_terminal, &ladoDireitoProducao);
      }
    }

    // Reconhece texto de parametro
    else if (charCorrente == '(') {
      tokenRetorno = reconheceTexto(TOK_TEXTO_PAR,charCorrente);
    }

    // Reconhece texto de codigo
    else if ( charCorrente == '{' ) {
      tokenRetorno = reconheceTexto(TOK_TEXTO_COD,charCorrente);
    }

    // Reconhece simbolos
    else switch (charCorrente) {
      // Reconhece ->
      case '-' :
    	  proxCaractere();
		  if (charCorrente=='>') {
		    proxCaractere();
		    tokenRetorno = TOK_SETA;
		  } else {
			Mensagem::erro(SIMBOLO_DESCONHECIDO, 0);
		  }
		  break;

	  // Reconhece OU
      case '|'  :
    	  proxCaractere();
		  if (!EstruturaDados::houve_erro && producaoAtual != NULL) {
			  ladoDireitoProducao = false;
		    // Cria nova producao com o simbolo corrente da producao atual
		    incluiListaProducoes(NAO_TERMINAL, producaoAtual->codigoLadoEsquerdo, &ladoDireitoProducao);
		  }
		  tokenRetorno = TOK_OU;
		  break;

	  // Reconhece ;
      case ';'  :
    	  proxCaractere();
		  ladoDireitoProducao = false;
		  tokenRetorno = TOK_PONTO_VIRGULA;
		  break;

	  // Reconhece vazio
      case '#'  :
    	  proxCaractere();
		  if (!EstruturaDados::houve_erro) {
		     incluiListaProducoes(VAZIO, VAZIO, &ladoDireitoProducao);
		  }
		  tokenRetorno = TOK_VAZIO;
		  break;

	  // Simbolo desconhecido pelo facc
      default   :
    	  if (charCorrente!=EOF) {
    	  	  Mensagem::erro(SIMBOLO_DESCONHECIDO, 0);
    	  	  proxCaractere();
		  } break;
    }
  }

  // Token de erro
  if (tokenRetorno==-1) {
    return(TOK_ETX);
  }

  return tokenRetorno ;
} /* fim le_token  */
