
//---------------------------------------------------------------------------
#ifndef Mensagem_H
#define Mensagem_H
//---------------------------------------------------------------------------

//------------------------------------------------------------------
// Classe para tratamento de mensagens de erro
//------------------------------------------------------------------
class Mensagem {
public:
  static void falha(int, char *);                     /* Falhas irrecuperaveis */
  static void erro(int,int);              /* Tratamento de erros de compilacao */
  static void advertencia(int);                    /* Mensagens de Advertencia */
} ;

//---------------------------------------------------------------------------
#endif
