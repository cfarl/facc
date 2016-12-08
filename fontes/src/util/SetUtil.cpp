/* -----------------------------------------------------------------------*
 *                                                                        *
 * Programa    : FACC.EXE                                Versao 2.0       *
 *                                                                        *
 * Modulo      : FACCMEM.C                                                *
 *                                                                        *
 * Autores     : Edward Lucio Vieira Borba  			          *
 *                                                                        *
 * Data inicio : 11/04/91                                                 *
 * Data Atual. : 15/08/91                                                 *
 *                                                                        *
 * Descricao   : Gerencia as transferencias de listas para os arrays,     *
 *               bem como a liberacao destes ultimos   ao  final  da      *
 *               execucao ou quando nao sao mais necessarios.             *
 *                                                                        *
 * -----------------------------------------------------------------------*/

#include "SetUtil.h"
#include <iostream>

void SetUtil::uniao(std::set<int> &destino, std::set<int> set1, std::set<int> set2) {
	destino.clear() ;

	for (std::set<int>::iterator it=set1.begin(); it!=set1.end(); ++it)
		destino.insert(*it);

	for (std::set<int>::iterator it=set2.begin(); it!=set2.end(); ++it)
			destino.insert(*it);
}

bool SetUtil::contem(std::set<int> conjunto, int valor) {
	return conjunto.find(valor) != conjunto.end() ;
}

// Retorna true se set 1 é equivalente a st2
bool SetUtil::equivalenteSet(std::set<int> set1, std::set<int> set2) {
	//int x = set1.size();
	//int y = set2.size();
	if(set1.size() != set2.size()) return false ;

	for (std::set<int>::iterator it=set1.begin(); it!=set1.end(); ++it) {
		if(set2.find(*it) == set2.end()) return false;
	}
	return true ;
}

// Retorna true se set 1 é equivalente a st2
int SetUtil::get(std::set<int> set1, int posicao) {
	int contador = 0;
	std::set<int>::iterator it=set1.begin();
	while(contador != posicao) ++it;
    return *it ;
}

// Retorna true se set 1 é equivalente a st2
void printSet(std::set<int> set1) {
	std::cout << "---------" << std::endl ;
	for (std::set<int>::iterator it=set1.begin(); it!=set1.end(); ++it) {
		std::cout << *it << " " << std::endl ;
	}
}

