

#include <iostream>
#include "AnalisadorSintatico.h"
#include "AnalisadorSemantico.h"

using namespace std;

int main() {
	 string codigoFonte = "3*(4+5)-7" ;

	 // 1) Processa a An�lise Sint�tica do c�digo fonte
	 AnalisadorSintatico* analisadorSintatico = new AnalisadorSintatico() ;
	 analisadorSintatico->executa(codigoFonte) ;
    if(analisadorSintatico->encontrouErro()) {
   	     cout << analisadorSintatico->getMsgErroSintatico() << endl;
   	     delete analisadorSintatico ;
   	     return 0;
    } else {
    	cout << "An�lise Sint�tica conclu�da com sucesso!" << endl;
    }

    // Recupera dados produzidos pela An�lise Sint�tica
     stack<int> pilhaProducoesPrograma = analisadorSintatico->getPilhaProducoesAnaliseSintatica() ;
     list<int> listaAtributosPrograma = analisadorSintatico->getListaAtributosAnaliseLexica() ;

     // 2) Processa a An�lise Sem�ntica do programa
     AnalisadorSemantico* analisadorSemantico = new AnalisadorSemantico() ;
     int resultado = analisadorSemantico->executa(pilhaProducoesPrograma, listaAtributosPrograma) ;
	 if(analisadorSemantico->encontrouErro()) {
   		cout << analisadorSemantico->getMsgErroSemantico() << endl ;
    } else {
    	cout << "An�lise Sem�ntica conclu�da com sucesso!" << endl;
    	cout << "Resultado da expressao " << resultado << endl ;
    }

 	delete analisadorSintatico ;
 	delete analisadorSemantico ;
	return 0;
}
