#ifndef AnalisadorSemantico_H
#define AnalisadorSemantico_H

#include <list>
#include <stack>
#include <stdio.h>

//----------------------------------------------------------
// Analisador semantico para processamento da gramatica
//----------------------------------------------------------
class AnalisadorSemantico {
private:
	 std::list<int> listaAtributosPrograma ;
	 std::stack<int> pilhaProducoesPrograma ;
	 bool geraArquivo ;
	 int GL_prod_ativa;
	 int GL_spc;
	 FILE *arq_sem_c ;
	 FILE *arq_sem_h ;

	 //-----------------------------------------
	 // Metodos da analise semantica
	 //-----------------------------------------
	 int prox_atributo() ;
	 int prox_prod() ;
	 void espec();
	 void lista_prod();
	 void prod();
	 void param_opc(char tipo);
	 void local_opc();
	 void codigo_opc();
	 void lista_dir(int nt, char tipo_proc);
	 void parte_dir(char tipo_proc);
	 void lista_simb();
	 void simb();
	 void atrib_opc(int te);
	 char tipo_procedimento(int nt);
	 int prod_analise(int nt);
	 void inclui_texto(int tipo);

public:
	 //-----------------------------------------
	 // Executa a analise semantica
	 //-----------------------------------------
	 void execute(bool geraArquivo, std::stack<int> pilhaProducoesPrograma, std::list<int> listaAtributosPrograma) ;
};

#endif



