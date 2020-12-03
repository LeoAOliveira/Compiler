/*
 
 Leonardo Amorim de Oliveira (TIA: 31842178)
 Joao Victor Rizotto Coelho (TIA: 31841287)
 Fabricio Guilhermo Beatrice de Oliveira (TIA: 31835597)
 
 Computer Science - 06G
 September-November, 2020
 
 *** IMPORTANT ***
 Note that for this program, the repetition of
 _f identifier within the scope of the main function
 DOESN'T generates error!
 
 -> IMPLEMENTATION OF THE SYMBOLS TABLE
 
 We created a structure called Symbol (line 96),
 where it is possible to store the identifier name information
 and in what scope it is present. Soon after, we performed the
 creation of the symbolTable vector (line 108) of the Symbol type, which
 represents the Symbol Table as a whole. For scope control
 system current, the current scope int variable (line 107).
 The Symbol Table is initialized in the function initializeTable (line 425)
 with empty preset value (line 109). During the syntactic analysis
 recursive, the current scope increases and decreases according to a new
 scope and inserted or removed. Upon removal, the function call is performed
 removeOnTable (line 447) to remove the symbols present in the scope that
 it ceased to exist.
 In the identifier function (line 1.076) the function existsNaTabela (line 461) and
 called to check for the existence of a symbol with the same identifier.
 If it is a statement, check only at the current scope. Otherwise,
 checks on all existing scopes. Again in the identifier function
 the insertionNaTable function is called (line 436) if it is a
 declaration to insert a new symbol in the Symbol Table.
 Finally, in the command function (line 715) calls the function printTable (line 477)
 in the plot appearance, which performs the printing of the Symbol Table in that
 moment, indicating the corresponding code line.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* LEXEMAS */
#define _ERROR 0                /* Erro lexicon */
#define _IF 1                  /* if (palavra reservada) */
#define _IFH 2                 /* if (palavra reservada) */
#define _INT 3                 /* int (palavra reservada) */
#define _FUN 4                 /* fun (palavra reservada) */
#define _FAL 5                 /* fal (palavra reservada / tipo de dado) */
#define _DO 6                  /* do (palavra reservada) */
#define _DIV 7                 /* div (palavra reservada / operador) */
#define _ELSE 8                /* else (palavra reservada) */
#define _LOG 9                 /* log (palavra reservada) */
#define _MAIN 10               /* main(palavra reservada) */
#define _THEN 11               /* then (palavra reservada) */
#define _TRU 12                /* tru (palavra reservada / tipo de dado) */
#define _PLOT 13               /* plot (palavra reservada) */
#define _WHILE 14              /* while(palavra reservada) */
#define _PLUS 15               /* + (operador) */
#define _MINOR 16              /* - (operador) */
#define _TIMES 17              /* * (operador) */
#define _LESS 18               /* < (operador) */
#define _LESS_EQUAL 19         /* <= (operador) */
#define _LESS_MORE 20          /* <> (operador) */
#define _MORE 21               /* > (operador) */
#define _MORE_EQUAL 22         /* >= (operador) */
#define _EQUAL_EQUAL 23        /* == (operador) */
#define _TWO_DOTS_EQUAL 24     /* := (operador) */
#define _PARENTESES_OPEN 25    /* ( (delimitador) */
#define _PARENTESES_CLOSE 26   /* ) (delimitador) */
#define _COMMA 27              /* , (delimitador) */
#define _DOT_COMMA 28          /* ; (delimitador) */
#define _DOT 29                /* . (delimitador) */
#define _KEY_OPEN 30           /* { (delimitador) */
#define _KEY_CLOSE 31          /* } (delimitador) */
#define _IDENTIFIER 32         /* _[a...Z] (identifier) */
#define _COMMENT 33            /* / * * / (comentario) */
#define _NUMBER 34             /* [0...9] (numbers) */
#define _SPACE 35              /* Espacos em branco */
#define _TAB 36                /* Espacos em branco */
#define _LINE 37               /* Nova linha */

/* HELPERS */
#define SIZE 100

/* STRUCTURES */
/*
Structure: Representation of a symbol in the symbol table, containing its name and scope.
*/
typedef struct Symbol {
  char name;
  int scope;
}Symbol;

/* DECLARACOES (variaveis excepcionalmente globais)*/
int tokens[SIZE];
int lookahead;
int program[SIZE][SIZE];
char identifiers[SIZE][SIZE];
int indexID = 0;
int currentScope = 0;
Symbol symbolsTable[SIZE]; 
Symbol empty = {.name = '1', .scope = -1};

/* declaration DE TODAS FUNCOES */
void readFile();
void initTable();
void errorHandling(int codeLine, char errorType[]);
int lexiconnAnalysis(char tape[]);
int syntacticAnalysis();
int scanner(char *tape, int *first, int *codeLine, int *t);
int match(int token, int tokens[SIZE][SIZE], int *position, int *codeLine);
int prog(int tokens[SIZE][SIZE], int *position, int *codeLine);
int funcDeclaration(int tokens[SIZE][SIZE], int *position, int *codeLine);
int block(int tokens[SIZE][SIZE], int *position, int *codeLine);
int partVarDeclaration(int tokens[SIZE][SIZE], int *position, int *codeLine);
int varDeclaration(int tokens[SIZE][SIZE], int *position, int *codeLine);
int listID(int tokens[SIZE][SIZE], int *position, int *codeLine);
int declareFunc(int tokens[SIZE][SIZE], int *position, int *codeLine);
int formalParameter(int tokens[SIZE][SIZE], int *position, int *codeLine);
int composedCommand(int tokens[SIZE][SIZE], int *position, int *codeLine);
int command(int tokens[SIZE][SIZE], int *position, int *codeLine);
int assignment(int tokens[SIZE][SIZE], int *position, int *codeLine);
int callFunc(int tokens[SIZE][SIZE], int *position, int *codeLine);
int parameter(int tokens[SIZE][SIZE], int *position, int *codeLine);
int conditionalCommand(int tokens[SIZE][SIZE], int *position, int *codeLine);
int repetitiveCommand(int tokens[SIZE][SIZE], int *position, int *codeLine);
int expression(int tokens[SIZE][SIZE], int *position, int *codeLine);
int relation(int tokens[SIZE][SIZE], int *position, int *codeLine);
int simpleExpression(int tokens[SIZE][SIZE], int *position, int *codeLine);
int term(int tokens[SIZE][SIZE], int *position, int *codeLine);
int factor(int tokens[SIZE][SIZE], int *position, int *codeLine);
int number(int tokens[SIZE][SIZE], int *position, int *codeLine);
int identifier(int tokens[SIZE][SIZE], int *position, int *codeLine, int declaration);
void insertTable(Symbol novoSymbol);
void removeOnTable();
int existsOnTable(Symbol Symbol, int declaration);
int R1(int tokens[SIZE][SIZE], int *position, int *codeLine);
int R2(int tokens[SIZE][SIZE], int *position, int *codeLine);
int R3(int tokens[SIZE][SIZE], int *position, int *codeLine);
int R4(int tokens[SIZE][SIZE], int *position, int *codeLine);
int R5(int tokens[SIZE][SIZE], int *position, int *codeLine);
int R6(int tokens[SIZE][SIZE], int *position, int *codeLine);
int R7(int tokens[SIZE][SIZE], int *position, int *codeLine);
int R8(int tokens[SIZE][SIZE], int *position, int *codeLine);
int R9(int tokens[SIZE][SIZE], int *position, int *codeLine);
int R10(int tokens[SIZE][SIZE], int *position, int *codeLine);

/* IMPLEMENTATION */

int main(int argc,char *argv[]) {
  readFile();
  system("PAUSE");
}

/*
Function: Performs the reading of the .txt file and calls the lexical and syntactic analysis functions
*/
void readFile() {

  int i = 0;
  int j = 0;

  for (i=0; i<SIZE; i++)
    for (j=0; j<SIZE; j++)
      program[i][j] = -1;

  for (i=0; i<SIZE; i++)
    for (j=0; j<SIZE; j++)
      identifiers[i][j] = '1';
  
  FILE *txtFile;
  txtFile = fopen("example.txt", "r");

  char readFile[SIZE];
  int codeLine = 0;
  
  if (txtFile == NULL){
    printf("Problem opening txtFile");
    return;
  }

  char codigoTxt[1000] = "";
  
  while (fgets(readFile, SIZE, txtFile) != NULL) {
    codeLine++;
    strcat(codigoTxt, readFile);
  }

  fclose(txtFile);

  if (lexiconnAnalysis(codigoTxt)) {
    if (syntacticAnalysis()) 
      printf("\nProgram successfully analyzed.");
  }
}

/*
Function: Prints an error message for the user
parameters: codeLine = line of code with the error; errorType = the type of error that occurred.
*/
void errorHandling(int codeLine, char errorType[]) {
  printf("\nError %s on line %d", errorType, codeLine);
}

/*
Function: Performs the lexical analysis of the program by calling the scanner function and treating all possible returns.
parameter: tape = list of program characters, according to the txtFile .txt read in the readFile () function.
Return: integer 0 (lexical error found) or 1 (lexical analysis performed successfully).
*/
int lexiconnAnalysis(char tape[]) {

  int first = 0;
  int *pointer_first = &first;

  int codeLine = 0;
  int *pointer_codeLine = &codeLine;
  
  int tapeSize = strlen(tape);
  int result;

  int t = 0;
  int *pointer_t = &t;

  int i = 0;
  for (i=0; i<SIZE; i++)
      tokens[i] = -1;
  
  while (first < tapeSize && tape[*pointer_first]) {

    int auxiliar = first;
    result = scanner(tape, pointer_first, pointer_codeLine, pointer_t);
    
    switch (result) {
      case _ERROR:
        codeLine++;
        errorHandling(codeLine, "lexicon");
        return 0;
      case _IF:
        tokens[t] = _IF;
        t++;
        break;
      case _IFH: 
        tokens[t] = _IFH;
        t++;
        break;
      case _INT:
        tokens[t] = _INT;
        t++;
        break;
      case _FUN: 
        tokens[t] = _FUN;
        t++;
        break;
      case _FAL: 
        tokens[t] = _FAL;
        t++;
        break;
      case _DO: 
        tokens[t] = _DO;
        t++;
        break;
      case _DIV: 
        tokens[t] = _DIV;
        t++;
        break;
      case _ELSE: 
        tokens[t] = _ELSE;
        t++;
        break;
      case _LOG: 
        tokens[t] = _LOG;
        t++;
        break;
      case _MAIN: 
        tokens[t] = _MAIN;
        t++;
          break;
      case _THEN: 
        tokens[t] = _THEN;
        t++;
        break;
      case _TRU: 
        tokens[t] = _TRU;
        t++;
        break;
      case _PLOT: 
        tokens[t] = _PLOT;
        t++;
        break;
      case _WHILE:
        tokens[t] = _WHILE; 
        t++;
        break;
      case _PLUS: 
        tokens[t] = _PLUS;
        t++;
        break;
      case _MINOR: 
        tokens[t] = _MINOR;
        t++;
        break;
      case _TIMES: 
        tokens[t] = _TIMES;
        t++;
        break;
      case _LESS: 
        tokens[t] = _LESS;
        t++;
        break;
      case _LESS_EQUAL: 
        tokens[t] = _LESS_EQUAL;
        t++;
        break;
      case _LESS_MORE: 
        tokens[t] = _LESS_MORE;
        t++;
        break;
      case _MORE: 
        tokens[t] = _MORE;
        t++;
        break;
      case _MORE_EQUAL: 
        tokens[t] = _MORE_EQUAL;
        t++;
        break;
      case _EQUAL_EQUAL: 
        tokens[t] = _EQUAL_EQUAL;
        t++;
        break;
      case _TWO_DOTS_EQUAL: 
        tokens[t] = _TWO_DOTS_EQUAL;
        t++;
        break;
      case _PARENTESES_OPEN: 
        tokens[t] = _PARENTESES_OPEN;
        t++;
        break;
      case _PARENTESES_CLOSE: 
        tokens[t] = _PARENTESES_CLOSE;
        t++;
        break;
      case _COMMA: 
        tokens[t] = _COMMA;
        t++;
        break;
      case _DOT_COMMA:
        tokens[t] = _DOT_COMMA;
        t++;
        break;
      case _DOT: 
        tokens[t] = _DOT;
        t++;
        break;
      case _KEY_OPEN: 
        tokens[t] = _KEY_OPEN;
        t++;
        break;
      case _KEY_CLOSE:
        tokens[t] = _KEY_CLOSE;
        t++;
        break;
      case _IDENTIFIER:
        tokens[t] = _IDENTIFIER;
        t++;
        break;
      case _COMMENT:
        tokens[t] = _COMMENT;
        t++;
        break;
      case _NUMBER:
        tokens[t] = _NUMBER;
        t++;
        break;
      case _LINE:
        codeLine++;
        indexID = 0;
        int j;
        for (j=0; j<t; j++) {
          program[codeLine-1][j] = tokens[j];
        }
        t = 0;
        int i;
        for (i=0; i<SIZE; i++)
          tokens[i] = -1;
        break;
      default:
        break;
    }
  }
  return 1;
}

/*
Function: Performs the syntactic analysis of the program recursively, starting with the prog (...) function. SIZEwell assigns the initial value of the lookahead and calls the function that initializes the symbol table.
Return: integer 0 (parsing error found) or 1 (parsing performed successfully).
*/
int syntacticAnalysis() {

  int position = 0;
  int codeLine = 0;
  int scope = 0;

	lookahead = program[codeLine][position];

  initTable();
  
  if (prog(program, &position, &codeLine)) {
    return 1;
  } else {
    errorHandling(codeLine, "syntactic");
    return 0;
  }
}

/*
Function: Fill the table with empty symbols.
*/
void initTable() {
  indexID = 0;
  int i;
  for (i=0; i<SIZE; i++)
    symbolsTable[i] = empty;
}

/*
Function: Insert a new symbol in the table in the first available space.
parameters: Symbol = Symbol which will be inserted in the table.
*/
void insertTable(Symbol novoSymbol) {
  int i = 0;
  while (symbolsTable[i].name != empty.name) {
    i++;
  }
  symbolsTable[i] = novoSymbol;
}

/*
Function: Removes all identifiers belonging to the most recent local scope.
*/
void removeOnTable() {
  int i = SIZE-1;
  for (i=SIZE-1; i>=0; i--) {
    if (symbolsTable[i].scope == currentScope) {
      symbolsTable[i] = empty;
    }
  }
}

/*
Function: Checks whether the passed identifier is already present in the symbol table.
parameters: symbol = Symbol which will be verified; declaration = checks whether the identifier belongs to a declaration or not.
Return: integer 0 (does not exist) or 1 (does exist).
*/
int existsOnTable(Symbol Symbol, int declaration) {
  int i = SIZE-1;

  while ((declaration) ? symbolsTable[i].scope == currentScope : i>=0) {
    if (Symbol.name == symbolsTable[i].name)
      return 1;
    i--;
  }
  return 0;
}

/*
Function: Prints the look and is aesthetically pleasing.
parameters: codeLine = line of code at the printed time.
Return: integer 0 (does not exist) or 1 (does exist).
*/
void printTable(int codeLine) {
  printf("\nSymbol Table\n(Print performed on line %d)\n\n", codeLine+1);
  int i;
  int scope = currentScope;
  for (i=SIZE-1; i>=0; i--) {
    if (symbolsTable[i].name != empty.name) {
      printf("| %d: _%c\n", i, symbolsTable[i].name);
      if (i-1>=0)
        if (symbolsTable[i-1].scope != scope) {
          printf("|\n");
          scope--;
        }
    }
  }
  printf("\n");
}


/*
Function: Cycles through the program's token matrix to apply the rules.
parameters: token = Current word; tokens = example.txt array; position = current position of the matrix; line of current matrix code.
Return: integer 0 (failure) or 1 (success).
*/
int match(int token, int tokens[SIZE][SIZE], int *position, int *codeLine){

  if (lookahead == token) {
    if (tokens[*codeLine][*position+1] != -1) {
      lookahead = tokens[*codeLine][++(*position)];
    } else {
      (*codeLine)++;
      *position = 0;
      indexID = 0;
      while (tokens[*codeLine][*position] == -1) {
        (*codeLine)++;
      }
      lookahead = tokens[*codeLine][*position];
    }
		return 1;
    
	} else {
    return 0;
  }
}

/*
Function: Checks whether the token is inside a comment.
parameters: tokens = Array of example.txt; position = current position of the matrix; line of current matrix code.
*/
void comentario(int tokens[SIZE][SIZE], int *position, int *codeLine) {
  while (lookahead == _COMMENT) {
    match(_COMMENT, tokens, position, codeLine);
  }
}

/*
Function: Creation of the main function of the program.
parameters: tokens = Array of example.txt; position = current position of the matrix; line of current matrix code.
Return: integer 0 (failure) or 1 (success).
*/
int prog(int tokens[SIZE][SIZE], int *position, int *codeLine) {
  comentario(tokens, position, codeLine);
  if (R1(tokens, position, codeLine) &&
      match(_MAIN, tokens, position, codeLine)) {
        currentScope++;
  } else {
    return 0;
  }
      
  if (block(tokens, position, codeLine) &&
      match(_DOT, tokens, position, codeLine)) {
        removeOnTable();
        currentScope--;
        return 1;       
  } else {
    return 0;
  }
}

/*
Function: declaration of variables and commands within the main.
parameters: tokens = Array of example.txt; position = current position of the matrix; line of current matrix code.
Return: integer 0 (failure) or 1 (success).
*/
int block(int tokens[SIZE][SIZE], int *position, int *codeLine){
  comentario(tokens, position, codeLine);
  if(R2(tokens, position, codeLine) && 
    composedCommand(tokens, position, codeLine))
    return 1;
  else
    return 0;
}

/*
Function: Creation of where a variable will be created.
parameters: tokens = Array of example.txt; position = current position of the matrix; line of current matrix code.
Return: integer 0 (failure) or 1 (success).
*/
int partVarDeclaration(int tokens[SIZE][SIZE], int *position, int *codeLine) {
  comentario(tokens, position, codeLine);
  if (lookahead == _INT || lookahead == _LOG) {
    if(varDeclaration(tokens, position, codeLine) && 
      R3(tokens, position, codeLine))
      return 1;
    else
      return 0;
  } else {
    return 0;
  }
}

/*
Function: variable declaration.
parameters: tokens = Array of example.txt; position = current position of the matrix; line of current matrix code.
Return: integer 0 (failure) or 1 (success).
*/
int varDeclaration(int tokens[SIZE][SIZE], int *position, int *codeLine){
  comentario(tokens, position, codeLine);
  if (lookahead == _INT) {
    if (match(_INT, tokens, position, codeLine) && 
        listID(tokens, position, codeLine) && 
        match(_DOT_COMMA, tokens, position, codeLine))
          return 1;
    else 
      return 0;
  } else if (lookahead == _LOG) {
    if(match(_LOG, tokens, position, codeLine) && 
      listID(tokens, position, codeLine) && 
      match(_DOT_COMMA, tokens, position, codeLine))
        return 1;
    else 
      return 0;
  } else {
    return 0;
  }
}

/*
Function: Creation of a declaration list of identifiers.
parameters: tokens = Array of example.txt; position = current position of the matrix; line of current matrix code.
Return: integer 0 (failure) or 1 (success).
*/
int listID(int tokens[SIZE][SIZE], int *position, int *codeLine) {
  comentario(tokens, position, codeLine);
  if (lookahead == _IDENTIFIER) {
    if(identifier(tokens, position, codeLine, 1) && 
        R4(tokens, position, codeLine))
      return 1;
    else
      return 0;
  }
  return 0;
}

/*
Function: declaration of a function.
parameters: tokens = Array of example.txt; position = current position of the matrix; line of current matrix code.
Return: integer 0 (failure) or 1 (success).
*/
int funcDeclaration(int tokens[SIZE][SIZE], int *position, int *codeLine) { 
  comentario(tokens, position, codeLine);
  if(R5(tokens, position, codeLine))
    return 1;
  else
    return 0;
}

/*
Function: declaration of a function.
parameters: tokens = Array of example.txt; position = current position of the matrix; line of current matrix code.
Return: integer 0 (failure) or 1 (success).
*/
int decFunc(int tokens[SIZE][SIZE], int *position, int *codeLine) { 
  comentario(tokens, position, codeLine);
  if (lookahead == _FUN) {
    if(match(_FUN, tokens, position, codeLine) && 
      identifier(tokens, position, codeLine, 1) && 
      match(_PARENTESES_OPEN, tokens, position, codeLine)) {
        currentScope++;
      } else
        return 0;
    
    if(formalParameter(tokens, position, codeLine) &&
      match(_PARENTESES_CLOSE, tokens, position, codeLine) &&
      block(tokens, position, codeLine) &&
      match(_DOT, tokens, position, codeLine)) {
        removeOnTable();
        currentScope--;
        return 1;
      } else
        return 0;
  }
  return 0;
}

/*
Function: declaration of a formal function parameter.
parameters: tokens = Array of example.txt; position = current position of the matrix; line of current matrix code.
Return: integer 0 (failure) or 1 (success).
*/
int formalParameter(int tokens[SIZE][SIZE], int *position, int *codeLine) {
  comentario(tokens, position, codeLine);
  if (lookahead == _INT) {
    if (match(_INT, tokens, position, codeLine) && 
      identifier(tokens, position, codeLine, 1)) 
        return 1;
    else 
      return 0;
  } else if (lookahead == _LOG) {
    if (match(_INT, tokens, position, codeLine) && 
      identifier(tokens, position, codeLine, 1)) 
        return 1;
    else
      return 0;
  } else {
    return 0;
  }
}

/*
Function: declaration of a compound command.
parameters: tokens = Array of example.txt; position = current position of the matrix; line of current matrix code.
Return: integer 0 (failure) or 1 (success).
*/
int composedCommand(int tokens[SIZE][SIZE], int *position, int *codeLine) {
  comentario(tokens, position, codeLine);
  if (command(tokens, position, codeLine) && 
    match(_DOT_COMMA, tokens, position, codeLine) && 
    R6(tokens, position, codeLine)) 
      return 1;
  else 
    return 0;
}

/*
Function: declaration of a command.
parameters: tokens = Array of example.txt; position = current position of the matrix; line of current matrix code.
Return: integer 0 (failure) or 1 (success).
*/
int command(int tokens[SIZE][SIZE], int *position, int *codeLine) {
  comentario(tokens, position, codeLine);
  int auxiliar_position = *position;
  int auxiliar_lookahead = lookahead;
  
  if (assignment(tokens, position, codeLine)) {
    return 1;
  } else {
    *position = auxiliar_position;
    lookahead = auxiliar_lookahead;
  }
  
  if (callFunc(tokens, position, codeLine)) {
    return 1;
  } else {
    *position = auxiliar_position;
    lookahead = auxiliar_lookahead;
  }

  if (conditionalCommand(tokens, position, codeLine)) {
    return 1;
  } else {
    *position = auxiliar_position;
    lookahead = auxiliar_lookahead;
  }

  if (repetitiveCommand(tokens, position, codeLine)) {
    return 1;
  } else {
    *position = auxiliar_position;
    lookahead = auxiliar_lookahead;
  }

  if (lookahead == _PLOT) {
    if (match(_PLOT, tokens, position, codeLine) && 
      match(_PARENTESES_OPEN, tokens, position, codeLine) && 
      identifier(tokens, position, codeLine, 0) && 
      match(_PARENTESES_CLOSE, tokens, position, codeLine)) {
        printTable(*codeLine);
        return 1;
    } else {
      *position = auxiliar_position;
      lookahead = auxiliar_lookahead;
    }
  }
  return 0;
}

/*
Function: declaration of an assignment.
parameters: tokens = Array of example.txt; position = current position of the matrix; line of current matrix code.
Return: integer 0 (failure) or 1 (success).
*/
int assignment(int tokens[SIZE][SIZE], int *position, int *codeLine) {
  comentario(tokens, position, codeLine);
  if (lookahead == _IDENTIFIER) {
    if (match(_IDENTIFIER, tokens, position, codeLine) && 
      match(_TWO_DOTS_EQUAL, tokens, position, codeLine) && 
      simpleExpression(tokens, position, codeLine)) 
        return 1;
    else
      return 0;
  }
  return 0;
}

/*
Function: Creates the function call.
parameters: tokens = Array of example.txt; position = current position of the matrix; line of current matrix code.
Return: integer 0 (failure) or 1 (success).
*/
int callFunc(int tokens[SIZE][SIZE], int *position, int *codeLine) {
  comentario(tokens, position, codeLine);
  if (lookahead == _IDENTIFIER) {
    if (identifier(tokens, position, codeLine, 0) && 
      match(_PARENTESES_OPEN, tokens, position, codeLine) && 
      parameter(tokens, position, codeLine) && 
      match(_PARENTESES_CLOSE, tokens, position, codeLine)) 
        return 1;
    else 
      return 0;
  }
  return 0;
}

/*
Function: Declare a parameter.
parameters: tokens = Array of example.txt; position = current position of the matrix; line of current matrix code.
Return: integer 0 (failure) or 1 (success).
*/
int parameter(int tokens[SIZE][SIZE], int *position, int *codeLine) {
  comentario(tokens, position, codeLine);

  int auxiliar_position = *position;
  int auxiliar_lookahead = lookahead;

  if (identifier(tokens, position, codeLine, 0)) {
    return 1;
  } else {
    *position = auxiliar_position;
    lookahead = auxiliar_lookahead;
  }

  if (number(tokens, position, codeLine)) {
    return 1;
  } else {
    *position = auxiliar_position;
    lookahead = auxiliar_lookahead;
  }

  if (match(_TRU, tokens, position, codeLine)) {
    return 1;
  } else {
    *position = auxiliar_position;
    lookahead = auxiliar_lookahead;
  }

  if (match(_FAL, tokens, position, codeLine)) {
    return 1;
  } else {
    *position = auxiliar_position;
    lookahead = auxiliar_lookahead;
  }
  
  return 0;
}

/*
Function: Declare a conditional structure command.
parameters: tokens = Array of example.txt; position = current position of the matrix; line of current matrix code.
Return: integer 0 (failure) or 1 (success).
*/
int conditionalCommand(int tokens[SIZE][SIZE], int *position, int *codeLine) {
  comentario(tokens, position, codeLine);
  if (lookahead == _IF) {
    if (match(_IF, tokens, position, codeLine) && 
        match(_PARENTESES_OPEN, tokens, position, codeLine) && 
        expression(tokens, position, codeLine) && 
        match(_PARENTESES_CLOSE, tokens, position, codeLine) && 
        match(_THEN, tokens, position, codeLine)) {
          currentScope++;
    } else 
      return 0;
        
    if (composedCommand(tokens, position, codeLine) &&
        R7(tokens, position, codeLine) &&
        match(_IFH, tokens, position, codeLine)) {
          removeOnTable();
          currentScope--;
          return 1;
    } else
      return 0;
  }
  return 0;
}

/*
Function: Declares a repetitive structure command.
parameters: tokens = Array of example.txt; position = current position of the matrix; line of current matrix code.
Return: integer 0 (failure) or 1 (success).
*/
int repetitiveCommand(int tokens[SIZE][SIZE], int *position, int *codeLine) { 
  comentario(tokens, position, codeLine);
  if (lookahead == _DO) {
    if (match(_DO, tokens, position, codeLine)) {
      currentScope++;
    } else
      return 0;
    
    if (composedCommand(tokens, position, codeLine)) {
      removeOnTable();
      currentScope--;
    } else
      return 0;

    if (match(_WHILE, tokens, position, codeLine) &&
      match(_PARENTESES_OPEN, tokens, position, codeLine) &&
      expression(tokens, position, codeLine) &&
      match(_PARENTESES_CLOSE, tokens, position, codeLine))
        return 1;
    else
      return 0;
  }
  return 0;
}

/*
Function: Declares a relational expression.
parameters: tokens = Array of example.txt; position = current position of the matrix; line of current matrix code.
Return: integer 0 (failure) or 1 (success).
*/
int expression(int tokens[SIZE][SIZE], int *position, int *codeLine) { 
  comentario(tokens, position, codeLine);
  if (simpleExpression(tokens, position, codeLine)) {
    if (lookahead == _PARENTESES_CLOSE) {
      return 1;
    } else {
      if (relation(tokens, position, codeLine) 
        && simpleExpression(tokens, position, codeLine)) {
          return 1;
      } else {
        return 0;
      }
    }
  } else {
    return 0;
  }
}

/*
Function: Declare a relation.
parameters: tokens = Array of example.txt; position = current position of the matrix; line of current matrix code.
Return: integer 0 (failure) or 1 (success).
*/
int relation(int tokens[SIZE][SIZE], int *position, int *codeLine) { 
  comentario(tokens, position, codeLine);

  int auxiliar_position = *position;
  int auxiliar_lookahead = lookahead;

  if (match(_EQUAL_EQUAL, tokens, position, codeLine)) {
    return 1;
  } else {
    *position = auxiliar_position;
    lookahead = auxiliar_lookahead;
  }

  if (match(_LESS_MORE, tokens, position, codeLine)) {
    return 1;
  } else {
    *position = auxiliar_position;
    lookahead = auxiliar_lookahead;
  }

  if (match(_LESS, tokens, position, codeLine)) {
    return 1;
  } else {
    *position = auxiliar_position;
    lookahead = auxiliar_lookahead;
  }

  if (match(_LESS_EQUAL, tokens, position, codeLine)) {
    return 1;
  } else {
    *position = auxiliar_position;
    lookahead = auxiliar_lookahead;
  }

  if (match(_MORE_EQUAL, tokens, position, codeLine)) {
    return 1;
  } else {
    *position = auxiliar_position;
    lookahead = auxiliar_lookahead;
  }

  if (match(_MORE, tokens, position, codeLine)) {
    return 1;
  } else {
    *position = auxiliar_position;
    lookahead = auxiliar_lookahead;
  }
  
  return 0;
}

/*
Function: Declares a simple expression.
parameters: tokens = Array of example.txt; position = current position of the matrix; line of current matrix code.
Return: integer 0 (failure) or 1 (success).
*/
int simpleExpression(int tokens[SIZE][SIZE], int *position, int *codeLine) { 
  comentario(tokens, position, codeLine);
  if(R8(tokens, position, codeLine) && 
    term(tokens, position, codeLine) &&
    R9(tokens, position, codeLine))
      return 1;
  else
    return 0;
}

/*
Function: Declares a term.
parameters: tokens = Array of example.txt; position = current position of the matrix; line of current matrix code.
Return: integer 0 (failure) or 1 (success).
*/
int term(int tokens[SIZE][SIZE], int *position, int *codeLine) { 
  comentario(tokens, position, codeLine);
  if (factor(tokens, position, codeLine) && 
    R10(tokens, position, codeLine))
      return 1;
  else
    return 0;
}

/*
Function: Declare a factor.
parameters: tokens = Array of example.txt; position = current position of the matrix; line of current matrix code.
Return: integer 0 (failure) or 1 (success).
*/
int factor(int tokens[SIZE][SIZE], int *position, int *codeLine) {
  comentario(tokens, position, codeLine);
  int auxiliar_position = *position;
  int auxiliar_lookahead = lookahead;

  if (identifier(tokens, position, codeLine, 0)) {
    return 1;
  } else {
    *position = auxiliar_position;
    lookahead = auxiliar_lookahead;
  }

  if (number(tokens, position, codeLine)) {
    return 1;
  } else {
    *position = auxiliar_position;
    lookahead = auxiliar_lookahead;
  }

  if (match(_TRU, tokens, position, codeLine)) {
    return 1;
  } else {
    *position = auxiliar_position;
    lookahead = auxiliar_lookahead;
  }

  if (match(_FAL, tokens, position, codeLine)) {
    return 1;
  } else {
    *position = auxiliar_position;
    lookahead = auxiliar_lookahead;
  }
  
  if (lookahead == _PARENTESES_OPEN) {
    if (match(_PARENTESES_OPEN, tokens, position, codeLine) &&
      simpleExpression(tokens, position, codeLine) &&
      match(_PARENTESES_CLOSE, tokens, position, codeLine)) {
        return 1;
    } else {
      *position = auxiliar_position;
      lookahead = auxiliar_lookahead;
    }
  }

  return 0;
}

/*
Function: Declare a number.
parameters: tokens = Array of example.txt; position = current position of the matrix; line of current matrix code.
Return: integer 0 (failure) or 1 (success).
*/
int number(int tokens[SIZE][SIZE], int *position, int *codeLine) {
  comentario(tokens, position, codeLine);
  return match(_NUMBER, tokens, position, codeLine);
}

/*
Function: Declare an identifier.
parameters: tokens = Array of example.txt; position = current position of the matrix; line of current matrix code.
Return: integer 0 (failure) or 1 (success).
*/
int identifier(int tokens[SIZE][SIZE], int *position, int *codeLine, int declaration) {
  comentario(tokens, position, codeLine);

  char name = identifiers[*codeLine][indexID];
  Symbol Symbol = {.name = name, .scope = currentScope};
  indexID++;

  if (declaration) 
    if (!existsOnTable(Symbol, declaration))
      insertTable(Symbol);
    else
      return 0;
  else
    if (!existsOnTable(Symbol, declaration))
      return 0;

  return match(_IDENTIFIER, tokens, position, codeLine);
}

/*
Function: declaration of the Program construction rule.
parameters: tokens = Array of example.txt; position = current position of the matrix; line of current matrix code.
Return: integer 0 (failure) or 1 (success).
*/
int R1(int tokens[SIZE][SIZE], int *position, int *codeLine) { 
  comentario(tokens, position, codeLine);
  if (lookahead == _FUN) {
    if (funcDeclaration(tokens, position, codeLine) &&
        R1(tokens, position, codeLine))
          return 1;
    else
      return 0;
  } else {
    return 1;
  }
}

/*
Function: declaration of the Rule of construction of the block of commands of the program.
parameters: tokens = Array of example.txt; position = current position of the matrix; line of current matrix code.
Return: integer 0 (failure) or 1 (success).
*/
int R2(int tokens[SIZE][SIZE], int *position, int *codeLine) { 
  comentario(tokens, position, codeLine);
  if (lookahead == _INT || lookahead == _LOG) {
    if (partVarDeclaration(tokens, position, codeLine)) return 1;
    else return 0;
  } else {
    return 1;
  }
}

/*
Function: declaration of part of the variable declaration rule.
parameters: tokens = Array of example.txt; position = current position of the matrix; line of current matrix code.
Return: integer 0 (failure) or 1 (success).
*/
int R3(int tokens[SIZE][SIZE], int *position, int *codeLine) { 
  comentario(tokens, position, codeLine);
  if (lookahead == _INT || lookahead == _LOG) {
    if (varDeclaration(tokens, position, codeLine) && 
    R3(tokens, position, codeLine))
      return 1;
    else
      return 0;
  } else {
    return 1;
  }
}

/*
Function: declaration of the Rule of construction of the list of identifiers.
parameters: tokens = Array of example.txt; position = current position of the matrix; line of current matrix code.
Return: integer 0 (failure) or 1 (success).
*/
int R4(int tokens[SIZE][SIZE], int *position, int *codeLine) { 
  comentario(tokens, position, codeLine);
  if (lookahead == _COMMA) {
    if (match(_COMMA, tokens, position, codeLine) && 
      identifier(tokens, position, codeLine, 1) && 
      R4(tokens, position, codeLine))
        return 1;
    else
      return 0;
  } else {
    return 1;
  }
}

/*
Function: declaration of the Variable declaration rule.
parameters: tokens = Array of example.txt; position = current position of the matrix; line of current matrix code.
Return: integer 0 (failure) or 1 (success).
*/
int R5(int tokens[SIZE][SIZE], int *position, int *codeLine) {
  comentario(tokens, position, codeLine);
  if (lookahead == _FUN) {
    if (decFunc(tokens, position, codeLine) && 
        R5(tokens, position, codeLine))
          return 1;
    else
      return 0;
  } else {
    return 1;
  }
}

/*
Function: declaration of the Rule of construction of compound command.
parameters: tokens = Array of example.txt; position = current position of the matrix; line of current matrix code.
Return: integer 0 (failure) or 1 (success).
*/
int R6(int tokens[SIZE][SIZE], int *position, int *codeLine) {
  comentario(tokens, position, codeLine);
  if (lookahead == _IDENTIFIER || 
      lookahead == _FUN || 
      lookahead == _IF ||
      lookahead == _DO ||
      lookahead == _PLOT) {
        if (command(tokens, position, codeLine) && 
            match(_DOT_COMMA, tokens, position, codeLine) && 
            R6(tokens, position, codeLine)) 
              return 1;
        else 
          return 0;
  } else {
    return 1;
  }
}

/*
Function: declaration of the conditional command rule.
parameters: tokens = Array of example.txt; position = current position of the matrix; line of current matrix code.
Return: integer 0 (failure) or 1 (success).
*/
int R7(int tokens[SIZE][SIZE], int *position, int *codeLine) {
  comentario(tokens, position, codeLine);
  if (lookahead == _ELSE) {
    if (match(_ELSE, tokens, position, codeLine) && 
        composedCommand(tokens, position, codeLine)) 
          return 1;
    else 
      return 0;
  } else {
    return 1;
  }
}

/*
Function: declaration of the Rule of construction of simple expression.
parameters: tokens = Array of example.txt; position = current position of the matrix; line of current matrix code.
Return: integer 0 (failure) or 1 (success).
*/
int R8(int tokens[SIZE][SIZE], int *position, int *codeLine) {
  comentario(tokens, position, codeLine);
  if (lookahead == _PLUS) {
    return match(_PLUS, tokens, position, codeLine);
  } else if (lookahead == _MINOR) {
    return match(_MINOR, tokens, position, codeLine);
  } else {
    return 1;
  }
}

/*
Function: declaration of the Rule of construction of simple expression.
parameters: tokens = Array of example.txt; position = current position of the matrix; line of current matrix code.
Return: integer 0 (failure) or 1 (success).
*/
int R9(int tokens[SIZE][SIZE], int *position, int *codeLine) {
  comentario(tokens, position, codeLine);
  if (lookahead == _PLUS) {
    if (match(_PLUS, tokens, position, codeLine) && 
      term(tokens, position, codeLine) &&
      R9(tokens, position, codeLine))
        return 1;
    else
      return 0;
  } else if (lookahead == _MINOR) {
    if (match(_MINOR, tokens, position, codeLine) && 
      term(tokens, position, codeLine) &&
      R9(tokens, position, codeLine))
        return 1;
    else
      return 0;
  } else {
    return 1;
  }
}

/*
Function: declaration of the Factor rule.
parameters: tokens = Array of example.txt; position = current position of the matrix; line of current matrix code.
Return: integer 0 (failure) or 1 (success).
*/
int R10(int tokens[SIZE][SIZE], int *position, int *codeLine) { 
  comentario(tokens, position, codeLine);
  if (lookahead == _TIMES) {
    if (match(_TIMES, tokens, position, codeLine) &&
      factor(tokens, position, codeLine) && 
      R10(tokens, position, codeLine))
        return 1;
    else
      return 0;
  } else if (lookahead == _DIV) {
    if (match(_DIV, tokens, position, codeLine) &&
      factor(tokens, position, codeLine) && 
      R10(tokens, position, codeLine))
        return 1;
    else
      return 0;
  } else {
    return 1;
  }
}

/*
Function: Consume the tokens from the example.txt document.
parameters: tape = list of characters stored through txtFile example.txt, first = place where it starts to consume again, codeLine = current document line and t = current token.
Return: Integers that represent the program's tokens, as defined with #define in the LEXEMAS section (line 22).
*/
int scanner(char *tape, int *first, int *codeLine, int *t) {

  int start = *first;
  char characterFromTape;

  q0:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == 'i') {
      goto q1;
    }
    if (characterFromTape == 'f') {
      goto q9;
    }
    if (characterFromTape == 'd') {
      goto q16;
    }
    if (characterFromTape == 'e') {
      goto q22;
    }
    if (characterFromTape == 'l') {
      goto q27;
    }
    if (characterFromTape == 'm') {
      goto q31;
    }
    if (characterFromTape == 't') {
      goto q36;
    }
    if (characterFromTape == 'p') {
      goto q44;
    }
    if (characterFromTape == 'w') {
      goto q49;
    }
    if (characterFromTape == '+') {
      goto q55;
    }
    if (characterFromTape == '-') {
      goto q57;
    }
    if (characterFromTape == '*') {
      goto q59;
    }
    if (characterFromTape == '<') {
      goto q61;
    }
    if (characterFromTape == '>') {
      goto q67;
    }
    if (characterFromTape == '=') {
      goto q71;
    }
    if (characterFromTape == ':') {
      goto q74;
    }
    if (characterFromTape == '(') {
      goto q79;
    }
    if (characterFromTape == ')') {
      goto q78;
    }
    if (characterFromTape == ',') {
      goto q81;
    }
    if (characterFromTape == ';') {
      goto q83;
    }
    if (characterFromTape == '.') {
      goto q85;
    }
    if (characterFromTape == '{') {
      goto q96;
    }
    if (characterFromTape == '}') {
      goto q98;
    }
    if (characterFromTape == '_') {
      goto q93;
    }
    if (characterFromTape == '/') {
      goto q87;
    }
    if (characterFromTape == ' ') {
      return _SPACE;
    }
    if (characterFromTape == '\t') {
      return _TAB;
    }
    if (characterFromTape == '\n') {
      return _LINE;
    }
    if(characterFromTape>='0' && characterFromTape <='9') {
      goto q89;
    }
    goto q_ERROR;

  q1:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == 'f') {
      goto q2;
    }
    if (characterFromTape == 'n') {
      goto q6;
    }
    goto q_ERROR;

  q2:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == ' ') {
      goto q3;
    }
    if (characterFromTape == 'h') {
      goto q4;
    }
    goto q_ERROR;

  q3:
    return _IF;

  q4:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == ' ') {
      goto q5;
    }
    goto q_ERROR;

  q5:
    return _IFH;

  q6:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == 't') {
      goto q7;
    }
    goto q_ERROR;

  q7:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == ' ') {
      goto q8;
    }
    goto q_ERROR;

  q8:
    return _INT;

  q9:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == 'u') {
      goto q10;
    }
    if (characterFromTape == 'a') {
      goto q11;
    }
    goto q_ERROR;

  q10:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == 'n') {
      goto q12;
    }
    goto q_ERROR;

  q11:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == 'l') {
      goto q13;
    }
    goto q_ERROR;

  q12:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == ' ') {
      goto q14;
    }
    goto q_ERROR;

  q13:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == ' ') {
      goto q15;
    }
    goto q_ERROR;

  q14:
    return _FUN;

  q15:
    return _FAL;

  q16:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == 'o') {
      goto q17;
    }
    if (characterFromTape == 'i') {
      goto q19;
    }
    goto q_ERROR;

  q17:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == ' ') {
      goto q18;
    }
    goto q_ERROR;

  q18:
    return _DO;

  q19:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == 'v') {
      goto q20;
    }
    goto q_ERROR;

  q20:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == ' ') {
      goto q21;
    }
    goto q_ERROR;

  q21:
    return _DIV;

  q22:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == 'l') {
      goto q23;
    }
    goto q_ERROR;

  q23:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == 's') {
      goto q24;
    }
    goto q_ERROR;

  q24:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == 'e') {
      goto q25;
    }
    goto q_ERROR;

  q25:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == ' ') {
      goto q26;
    }
    goto q_ERROR;

  q26:
    return _ELSE;

  q27:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == 'o') {
      goto q28;
    }
    goto q_ERROR;

  q28:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == 'g') {
      goto q29;
    }
    goto q_ERROR;

  q29:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == ' ') {
      goto q30;
    }
    goto q_ERROR;

  q30:
    return _LOG;

  q31:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == 'a') {
      goto q32;
    }
    goto q_ERROR;

  q32:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == 'i') {
      goto q33;
    }
    goto q_ERROR;

  q33:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == 'n') {
      goto q34;
    }
    goto q_ERROR;

  q34:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == ' ') {
      goto q35;
    }
    goto q_ERROR;

  q35:
    return _MAIN;

  q36:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == 'h') {
      goto q38;
    }
    if (characterFromTape == 'r') {
      goto q37;
    }
    goto q_ERROR;

  q37:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == 'u') {
      goto q42;
    }
    goto q_ERROR;

  q38:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == 'e') {
      goto q39;
    }
    goto q_ERROR;

  q39:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == 'n') {
      goto q40;
    }
    goto q_ERROR;

  q40:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == ' ') {
      goto q41;
    }
    goto q_ERROR;

  q41:
    return _THEN;

  q42:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == ' ') {
      goto q43;
    }
    goto q_ERROR;

  q43:
    return _TRU;

  q44:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == 'l') {
      goto q45;
    }
    goto q_ERROR;

  q45:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == 'o') {
      goto q46;
    }
    goto q_ERROR;

  q46:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == 't') {
      goto q47;
    }
    goto q_ERROR;

  q47:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == ' ') {
      goto q48;
    }
    goto q_ERROR;

  q48:
    return _PLOT;

  q49:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == 'h') {
      goto q50;
    }
    goto q_ERROR;

  q50:
    characterFromTape = tape[*first];
    *first += 1;
   if (characterFromTape == 'i') {
      goto q51;
    }
    goto q_ERROR;

  q51:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == 'l') {
      goto q52;
    }
    goto q_ERROR;

  q52:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == 'e') {
      goto q53;
    }
    goto q_ERROR;

  q53:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == ' ') {
      goto q54;
    }
    goto q_ERROR;

  q54:
    return _WHILE;

  q55:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == ' ') {
      goto q56;
    }
    goto q_ERROR;

  q56:
    return _PLUS;

  q57:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == ' ') {
      goto q58;
    }
    goto q_ERROR;

  q58:
    return _MINOR;

  q59:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == ' ') {
      goto q60;
    }
    goto q_ERROR;

  q60:
    return _TIMES;

  q61:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == ' ') {
      goto q62;
    }
    if (characterFromTape == '=') {
      goto q63;
    }
    if (characterFromTape == '>') {
      goto q65;
    }
    goto q_ERROR;

  q62:
    return _LESS;

  q63:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == ' ') {
      goto q64;
    }
    goto q_ERROR;

  q64:
    return _LESS_EQUAL;

  q65:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == ' ') {
      goto q66;
    }
    goto q_ERROR;

  q66:
    return _LESS_MORE;

  q67:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == ' ') {
      goto q68;
    }
    goto q_ERROR;

  q68:
    return _MORE;

  q69:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == ' ') {
      goto q70;
    }
    goto q_ERROR;

  q70:
    return _MORE_EQUAL;

  q71:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == '=') {
      goto q72;
    }
    goto q_ERROR;

  q72:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == ' ') {
      goto q73;
    }
    goto q_ERROR;

  q73:
    return _EQUAL_EQUAL;

  q74:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == '=') {
      goto q75;
    }
    goto q_ERROR;

  q75:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == ' ') {
      goto q76;
    }
    goto q_ERROR;

  q76:
    return _TWO_DOTS_EQUAL;

  q77:
    return _PARENTESES_OPEN;

  q78:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == ' ') {
      goto q80;
    }
    goto q_ERROR;

  q79:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == ' ') {
      goto q77;
    }
    goto q_ERROR;

  q80:
    return _PARENTESES_CLOSE;

  q81:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == ' ') {
      goto q82;
    }
    goto q_ERROR;

  q82:
    return _COMMA;

  q83:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == ' ') {
      goto q84;
    }
    goto q_ERROR;

  q84:
    return _DOT_COMMA;

  q85:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == ' ') {
      goto q86;
    }
    goto q_ERROR;

  q86:
    return _DOT;

  q87:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == '*') {
      goto q88;
    }
    goto q_ERROR;

  q88:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == 'a' || characterFromTape == 'b' || characterFromTape == 'c' ||
        characterFromTape == 'd' || characterFromTape == 'e' || characterFromTape == 'f' ||
        characterFromTape == 'g' || characterFromTape == 'h' || characterFromTape == 'i' ||
        characterFromTape == 'j' || characterFromTape == 'k' || characterFromTape == 'l' ||
        characterFromTape == 'm' || characterFromTape == 'n' || characterFromTape == 'o' ||
        characterFromTape == 'p' || characterFromTape == 'q' || characterFromTape == 'r' ||
        characterFromTape == 's' || characterFromTape == 't' || characterFromTape == 'u' ||
        characterFromTape == 'v' || characterFromTape == 'w' || characterFromTape == 'x' ||
        characterFromTape == 'y' || characterFromTape == 'z' || characterFromTape == 'A' ||
        characterFromTape == 'B' || characterFromTape == 'C' || characterFromTape == 'D' ||
        characterFromTape == 'E' || characterFromTape == 'F' || characterFromTape == 'G' ||
        characterFromTape == 'H' || characterFromTape == 'I' || characterFromTape == 'J' ||
        characterFromTape == 'K' || characterFromTape == 'L' || characterFromTape == 'M' ||
        characterFromTape == 'N' || characterFromTape == 'O' || characterFromTape == 'P' ||
        characterFromTape == 'Q' || characterFromTape == 'R' || characterFromTape == 'S' ||
        characterFromTape == 'T' || characterFromTape == 'U' || characterFromTape == 'V' ||
        characterFromTape == 'W' || characterFromTape == 'X' || characterFromTape == 'Y' ||
        characterFromTape == 'Z' || characterFromTape == '0' || characterFromTape == '1' ||
        characterFromTape == '2' || characterFromTape == '3' || characterFromTape == '4' ||
        characterFromTape == '5' || characterFromTape == '6' || characterFromTape == '7' ||
        characterFromTape == '8' || characterFromTape == '9' || characterFromTape == '(' ||
        characterFromTape == ')' || characterFromTape == '!' || characterFromTape == '@' ||
        characterFromTape == '#' || characterFromTape == '$' || characterFromTape == '&' ||
        characterFromTape == '%' || characterFromTape == ' ' || characterFromTape == '\t') {
          goto q88;
    }
    if (characterFromTape == '\n') {
      *codeLine += 1;
      indexID = 0;
      tokens[*t] = _COMMENT;
      program[*codeLine-1][*t] = tokens[*t];
      *t += 1;
      int i = 0;
      for (i=0; i<SIZE; i++)
        tokens[i] = -1;
      *t = 0;
      goto q88;
    }
    if (characterFromTape == '*') {
      goto q90;
    }
    goto q_ERROR;

  q89:
    characterFromTape = tape[*first];
    *first += 1;
    if(characterFromTape>='0' && characterFromTape <='9') {
      goto q89;
    }
    if (characterFromTape == ' ') {
      goto q100;
    }
    goto q_ERROR;

  q90:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == '/') {
      goto q91;
    } else {
      goto q88;
    }

  q91:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == ' ') {
      goto q92;
    }
    goto q_ERROR;

  q92:
    return _COMMENT;

  q93:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == 'a' || characterFromTape == 'b' || characterFromTape == 'c' ||
        characterFromTape == 'd' || characterFromTape == 'e' || characterFromTape == 'f' ||
        characterFromTape == 'g' || characterFromTape == 'h' || characterFromTape == 'i' ||
        characterFromTape == 'j' || characterFromTape == 'k' || characterFromTape == 'l' ||
        characterFromTape == 'm' || characterFromTape == 'n' || characterFromTape == 'o' ||
        characterFromTape == 'p' || characterFromTape == 'q' || characterFromTape == 'r' ||
        characterFromTape == 's' || characterFromTape == 't' || characterFromTape == 'u' ||
        characterFromTape == 'v' || characterFromTape == 'w' || characterFromTape == 'x' ||
        characterFromTape == 'y' || characterFromTape == 'z' || characterFromTape == 'A' ||
        characterFromTape == 'B' || characterFromTape == 'C' || characterFromTape == 'D' ||
        characterFromTape == 'E' || characterFromTape == 'F' || characterFromTape == 'G' ||
        characterFromTape == 'H' || characterFromTape == 'I' || characterFromTape == 'J' ||
        characterFromTape == 'K' || characterFromTape == 'L' || characterFromTape == 'M' ||
        characterFromTape == 'N' || characterFromTape == 'O' || characterFromTape == 'P' ||
        characterFromTape == 'Q' || characterFromTape == 'R' || characterFromTape == 'S' ||
        characterFromTape == 'T' || characterFromTape == 'U' || characterFromTape == 'V' ||
        characterFromTape == 'W' || characterFromTape == 'X' || characterFromTape == 'Y' ||
        characterFromTape == 'Z' ) {
          goto q94;
    }
    goto q_ERROR;

  q94:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == 'a' || characterFromTape == 'b' || characterFromTape == 'c' ||
        characterFromTape == 'd' || characterFromTape == 'e' || characterFromTape == 'f' ||
        characterFromTape == 'g' || characterFromTape == 'h' || characterFromTape == 'i' ||
        characterFromTape == 'j' || characterFromTape == 'k' || characterFromTape == 'l' ||
        characterFromTape == 'm' || characterFromTape == 'n' || characterFromTape == 'o' ||
        characterFromTape == 'p' || characterFromTape == 'q' || characterFromTape == 'r' ||
        characterFromTape == 's' || characterFromTape == 't' || characterFromTape == 'u' ||
        characterFromTape == 'v' || characterFromTape == 'w' || characterFromTape == 'x' ||
        characterFromTape == 'y' || characterFromTape == 'z' || characterFromTape == 'A' ||
        characterFromTape == 'B' || characterFromTape == 'C' || characterFromTape == 'D' ||
        characterFromTape == 'E' || characterFromTape == 'F' || characterFromTape == 'G' ||
        characterFromTape == 'H' || characterFromTape == 'I' || characterFromTape == 'J' ||
        characterFromTape == 'K' || characterFromTape == 'L' || characterFromTape == 'M' ||
        characterFromTape == 'N' || characterFromTape == 'O' || characterFromTape == 'P' ||
        characterFromTape == 'Q' || characterFromTape == 'R' || characterFromTape == 'S' ||
        characterFromTape == 'T' || characterFromTape == 'U' || characterFromTape == 'V' ||
        characterFromTape == 'W' || characterFromTape == 'X' || characterFromTape == 'Y' ||
        characterFromTape == 'Z' ) {
          goto q94;
    }
    if (characterFromTape == ' ') {
      goto q95;
    }

    goto q_ERROR;

  q95:
    identifiers[*codeLine][indexID] = tape[start+1];
    indexID++;
    return _IDENTIFIER;

  q96:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == ' ') {
      goto q97;
    }
    goto q_ERROR;

  q97:
    return _KEY_OPEN;

  q98:
    characterFromTape = tape[*first];
    *first += 1;
    if (characterFromTape == ' ') {
      goto q99;
    }
    goto q_ERROR;

  q99:
    return _KEY_CLOSE;

  q100:
    return _NUMBER;

  q_ERROR:
    return _ERROR;
}
