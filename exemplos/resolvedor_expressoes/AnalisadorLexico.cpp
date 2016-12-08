

#include "AnalisadorLexico.h"

//------------------------------------------------------------------------------
// Método Construtor. Inicializa o analisador léxico.
//------------------------------------------------------------------------------
AnalisadorLexico::AnalisadorLexico(std::string codigoFonte) {
	 this->codigoFonte = codigoFonte ;
	 this->posCodigoFonte = 0;
}

 //---------------------------------------------------------------
 // Interface entre o analisador sintático e o analisador léxico.
 // Retorna o token reconhecido
 //---------------------------------------------------------------
 Token AnalisadorLexico::proxToken() {
	 // Verifica se chegou ao fim do programa
	 if(posCodigoFonte >= codigoFonte.size()) return Token(8, "$", 0) ;

	 char charCodigoFonte = codigoFonte[posCodigoFonte++] ;

	 // Retorna token para simbolos
	 if(charCodigoFonte == '+') return Token(1, "+", 0) ;
	 if(charCodigoFonte == '-') return Token(2, "-", 0) ;
	 if(charCodigoFonte == '*') return Token(3, "*", 0) ;
	 if(charCodigoFonte == '/') return Token(4, "/", 0) ;
	 if(charCodigoFonte == '(') return Token(6, "(", 0) ;
	 if(charCodigoFonte == ')') return Token(7, ")", 0) ;

	 // Retorna token para digitos
	 if(charCodigoFonte == '0') return Token(5, "0", 0) ;
	 if(charCodigoFonte == '1') return Token(5, "1", 1) ;
	 if(charCodigoFonte == '2') return Token(5, "2", 2) ;
	 if(charCodigoFonte == '3') return Token(5, "3", 3) ;
	 if(charCodigoFonte == '4') return Token(5, "4", 4) ;
	 if(charCodigoFonte == '5') return Token(5, "5", 5) ;
	 if(charCodigoFonte == '6') return Token(5, "6", 6) ;
	 if(charCodigoFonte == '7') return Token(5, "7", 7) ;
	 if(charCodigoFonte == '8') return Token(5, "8", 8) ;
	 if(charCodigoFonte == '9') return Token(5, "9", 9) ;
 }
