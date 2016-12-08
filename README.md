# facc
FACC - Ferramenta de Apoio a Construcao do Compiladores

Essa ferramenta,de posse de uma gramática livre de contexto, produz arquivos contendo código fonte em C++ que correspondem ao esqueleto de um compilador.

<img src="https://2.bp.blogspot.com/-w0IooB6or1Y/WEhWIFYmkVI/AAAAAAAABS4/blSeZRGvqLsEC-Z5hRCb9ZR6Q13o4sx3gCLcB/s1600/facc.jpg" border="0">

<b>Como assim?</b> 

Tomando como entrada a gramática fornecida, que especifica as produções de uma linguagem (como a linguagem Pascal), essa ferramenta gera código fonte que permite o reconhecimento da linguagem. 

O código gerado inclui um analisador sintático LALR(1), o esqueleto de um analisador léxico já integrado ao analisador sintático, além do esqueleto de um analisador semântico, que contém métodos correspondentes às produções da gramática. 

A ferramenta gera ainda o código fonte do compilador, que faz a chamada para a análise sintática, e em seguida a chamada para a análise semântica do código fonte escrito na linguagem reconhecida pelo compilador.

<img src="https://1.bp.blogspot.com/-5_Ajw47nIzE/WEhc1NXcVjI/AAAAAAAABTI/97WUN5dgp-A_zgD6fwJJromPA5Wy5J9NgCLcB/s1600/compilador.jpg" border="0">

<b>E como surgiu essa ferramenta?</b>

O FACC surgiu na Universidade de Brasília, e foi bastante utilizado na disciplina de Compiladores como ferramenta de apoio para construção de Tradutores. 

Foi graças a ele que nasceu o compilador Pascalzim, que usa na análise sintática e na análise semântica os fontes gerados por essa ferramenta.
 
<b>E depois?</b> 

Os fontes da ferramenta foram deixados na UnB, mas infelizmente não houve continuidade na evolução da ferramenta. 

Como era uma ferramenta essencial para o desenvolvimento do Pascalzim, consegui entrar em contato com os desenvolvedores para pegar o código fonte da ferramenta.
Então, depois de fazer um refatoramento no código, migrando-o de C para C++, e adicionando novas funcionalidades, foi criada essa versão aqui disponibilizada. 

Acesse o arquivo leiame.txt para maiores informações.
