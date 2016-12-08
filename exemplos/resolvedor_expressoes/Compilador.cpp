

#include <iostream>
#include "AnalisadorSintatico.h"
#include "AnalisadorSemantico.h"

using namespace std;

int main() {
	 string codigoFonte = "3*(4+5)-7" ;

	 // 1) Processa a Análise Sintática do código fonte
	 AnalisadorSintatico* analisadorSintatico = new AnalisadorSintatico() ;
	 analisadorSintatico->executa(codigoFonte) ;
    if(analisadorSintatico->encontrouErro()) {
   	     cout << analisadorSintatico->getMsgErroSintatico() << endl;
   	     delete analisadorSintatico ;
   	     return 0;
    } else {
    	cout << "Análise Sintática concluída com sucesso!" << endl;
    }

    // Recupera dados produzidos pela Análise Sintática
     stack<int> pilhaProducoesPrograma = analisadorSintatico->getPilhaProducoesAnaliseSintatica() ;
     list<int> listaAtributosPrograma = analisadorSintatico->getListaAtributosAnaliseLexica() ;

     // 2) Processa a Análise Semântica do programa
     AnalisadorSemantico* analisadorSemantico = new AnalisadorSemantico() ;
     int resultado = analisadorSemantico->executa(pilhaProducoesPrograma, listaAtributosPrograma) ;
	 if(analisadorSemantico->encontrouErro()) {
   		cout << analisadorSemantico->getMsgErroSemantico() << endl ;
    } else {
    	cout << "Análise Semântica concluída com sucesso!" << endl;
    	cout << "Resultado da expressao " << resultado << endl ;
    }

 	delete analisadorSintatico ;
 	delete analisadorSemantico ;
	return 0;
}
