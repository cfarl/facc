
//---------------------------------------------------------------------------
#ifndef Gramatica_H
#define Gramatica_H
//---------------------------------------------------------------------------

class Gramatica {
private:
  static std::set<int> leva_vazio; // Conjunto dos nao-terminais que derivam vazio

  static std::set<int> first_lado_direito(std::vector<struct SimboloLadoDireitoProducao *> q) ;
  static void atinge_terminal();
  static void atingido_por_inicial();
  static void derivacao_vazio() ;
public:
  static void calcula_first();
  static void critica_gramatica();

};

//---------------------------------------------------------------------------
#endif
