

//---------------------------------------------------------------------------
#ifndef AnalisadorLexicoH
#define AnalisadorLexicoH
//---------------------------------------------------------------------------

#include <string>
#include <vector>

//------------------------------------------------------------------
// VO que guarda os dados de um token do analisador léxico
//------------------------------------------------------------------
class Token {

  private:
     short valor ;        // Dado utilizado para efetuar a análise sintática
     std::string lexema ; // Texto correspondente ao token
     int atributo;        // Atributo do token

  public:
     //---------------------------------------------------------------
     // Construtores, armazena no objeto os dados da palavra reservada
     //---------------------------------------------------------------
     Token(int token, std::string lexema, int atributo)
     	 : valor(token), lexema(lexema), atributo(atributo) { } ;

     //--------------------------------------------------------
     // Métodos get
     //--------------------------------------------------------
     int getValor() const { return this->valor; }
     std::string getLexema() const { return this->lexema; }
     int getAtributo() const { return this->atributo; }
} ;

//------------------------------------------------------------------------------
// Classe que implementa o analisador léxico do compilador
//------------------------------------------------------------------------------
class AnalisadorLexico {
  private:
      std::string msgErro ;
      std::string codigoFonte ;
      int posCodigoFonte ;

  public:
    // Função que, quando chamada, retorna o próximo token do código fonte
    Token proxToken();
    
    // Método Construtor. Inicializa o analisador léxico.
    AnalisadorLexico(std::string codigoFonte) ;

    // Informações usadas para tratamento de erros
    bool encontrouErro() const { return msgErro != ""; };
    std::string getMsgErro() { return msgErro; }
};

//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
