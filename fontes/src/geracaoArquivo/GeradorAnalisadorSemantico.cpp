

#include "GeradorArquivoFacade.h"
#include "../semantico/AnalisadorSemantico.h"
#include "../sintatico/AnalisadorSintatico.h"


//-------------------------------------------------------------------------------
// Realiza a Analise Semantica. Se requerido, imprime o Analisador Semantico
//-------------------------------------------------------------------------------
void GeradorArquivoFacade::gerarArquivoSemantico(AnalisadorSintatico *sintatico, AnalisadorSemantico *semantico) {
	semantico->execute(true, sintatico->getPilhaProducoesAnaliseSintatica(), sintatico->getListaAtributosAnaliseLexica());
}



