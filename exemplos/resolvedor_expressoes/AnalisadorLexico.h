

//---------------------------------------------------------------------------
#ifndef AnalisadorLexicoH
#define AnalisadorLexicoH
//---------------------------------------------------------------------------

#include <string>
#include <vector>

//------------------------------------------------------------------
// VO que guarda os dados de um token do analisador l�xico
//------------------------------------------------------------------
class Token {

  private:
     short valor ;        // Dado utilizado para efetuar a an�lise sint�tica
     std::string lexema ; // Texto correspondente ao token
     int atributo;        // Atributo do token

  public:
     //---------------------------------------------------------------
     // Construtores, armazena no objeto os dados da palavra reservada
     //---------------------------------------------------------------
     Token(int token, std::string lexema, int atributo)
     	 : valor(token), lexema(lexema), atributo(atributo) { } ;

     //--------------------------------------------------------
     // M�todos get
     //--------------------------------------------------------
     int getValor() const { return this->valor; }
     std::string getLexema() const { return this->lexema; }
     int getAtributo() const { return this->atributo; }
} ;

//------------------------------------------------------------------------------
// Classe que implementa o analisador l�xico do compilador
//------------------------------------------------------------------------------
class AnalisadorLexico {
  private:
      std::string msgErro ;
      std::string codigoFonte ;
      int posCodigoFonte ;

  public:
    // Fun��o que, quando chamada, retorna o pr�ximo token do c�digo fonte
    Token proxToken();
    
    // M�todo Construtor. Inicializa o analisador l�xico.
    AnalisadorLexico(std::string codigoFonte) ;

    // Informa��es usadas para tratamento de erros
    bool encontrouErro() const { return msgErro != ""; };
    std::string getMsgErro() { return msgErro; }
};

//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
