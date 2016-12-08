
// -----------------------------------------------------------------------
//  Imprime o c�digo fonte do Analisador Sint�tico
// -----------------------------------------------------------------------

#ifndef IMPRIME_COMPILADOR
#define IMPRIME_COMPILADOR

#include <string>
#include "GeradorArquivoFacade.h"
#include "../util/Constantes.h"
#include "../util/Mensagem.h"

//-----------------------------------------------------------------------------
// Metodo que gera o arquivo do Compilador
//-----------------------------------------------------------------------------
void GeradorArquivoFacade::geraArquivoCompilador() {
	// Verifica se o arquivo foi aberto
	FILE *arq_comp ;
	if((arq_comp=fopen(nome_cmp_c,"wt"))==NULL) Mensagem::falha(CRIACAO_ARQ,nome_cmp_c);

	// Gera arquivo do compilador
	std::string fonte =
		"\n"
		"\n#include <iostream>"
		"\n#include \"AnalisadorSintatico.h\""
		"\n#include \"AnalisadorSemantico.h\""
		"\n"
		"\nusing namespace std;"
		"\n"
		"\nint main() {"
		"\n	 string codigoFonte = \"program p1; begin end.\" ;"
		"\n"
		"\n	 // 1) Processa a An�lise Sint�tica do c�digo fonte"
		"\n	 AnalisadorSintatico* analisadorSintatico = new AnalisadorSintatico() ;"
		"\n	 analisadorSintatico->executa(codigoFonte) ;"
		"\n    if(analisadorSintatico->encontrouErro()) {"
		"\n   	     cout << analisadorSintatico->getMsgErroSintatico() << endl;"
		"\n   	     delete analisadorSintatico ;"
		"\n   	     return 0;"
		"\n    } else {"
		"\n    	cout << \"An�lise Sint�tica conclu�da com sucesso!\" << endl;"
		"\n    }"
		"\n"
		"\n    // Recupera dados produzidos pela An�lise Sint�tica"
		"\n     stack<int> pilhaProducoesPrograma = analisadorSintatico->getPilhaProducoesAnaliseSintatica() ;"
		"\n     list<int> listaAtributosPrograma = analisadorSintatico->getListaAtributosAnaliseLexica() ;"
		"\n"
		"\n     // 2) Processa a An�lise Sem�ntica do programa"
		"\n     AnalisadorSemantico* analisadorSemantico = new AnalisadorSemantico() ;"
		"\n     analisadorSemantico->executa(pilhaProducoesPrograma, listaAtributosPrograma) ;"
		"\n	 if(analisadorSemantico->encontrouErro()) {"
		"\n   		cout << analisadorSemantico->getMsgErroSemantico() << endl ;"
		"\n    } else {"
		"\n    	cout << \"An�lise Sem�ntica conclu�da com sucesso!\" << endl;"
		"\n    }"
		"\n"
		"\n 	delete analisadorSintatico ;"
		"\n 	delete analisadorSemantico ;"
		"\n	return 0;"
		"\n}"
		;

	fputs(fonte.c_str(), arq_comp);

	// Fecha o arquivo
	if (fclose(arq_comp)==EOF) Mensagem::falha(FECHAMENTO_ARQ,nome_cmp_c);
}

#endif
