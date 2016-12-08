
//---------------------------------------------------------------------------
#ifndef SETUTIL_H
#define SETUTIL_H
//---------------------------------------------------------------------------

#include <set>

//---------------------------------------------------------------------------
// Classe que define metodos para manipulacao de conjuntos
//---------------------------------------------------------------------------
class SetUtil {
public:
	static void uniao(std::set<int> &destino, std::set<int> set1, std::set<int> set2) ;
	static bool contem(std::set<int> conjunto, int valor) ;
	static bool equivalenteSet(std::set<int> set1, std::set<int> set2) ;
	static int get(std::set<int> set1, int posicao) ;
	void print(std::set<int> set1) ;

};

//---------------------------------------------------------------------------
#endif
